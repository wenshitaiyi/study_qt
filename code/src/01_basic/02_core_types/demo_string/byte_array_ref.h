#pragma once

#include <QByteArray>
#include <QString>
#include <QDebug>
#include <cstring>
#include <algorithm>

/**
 * @brief 自定义轻量级 QByteArrayRef (字节流切片只读引用视图)
 * @details 类似于 Qt 5 的 QStringRef，提供对连续字节缓冲区的零拷贝视图封装。
 *          不持有内存所有权，避免高频切片与协议解析时的内存分配与拷贝开销。
 */
class QByteArrayRef
{
public:
    constexpr QByteArrayRef() noexcept
        : m_data(nullptr)
        , m_size(0)
    {
    }

    constexpr QByteArrayRef(const char *data, int size) noexcept
        : m_data(data)
        , m_size(size < 0 ? 0 : size)
    {
    }

    QByteArrayRef(const char *str) noexcept
        : m_data(str)
        , m_size(str ? static_cast<int>(std::strlen(str)) : 0)
    {
    }

    QByteArrayRef(const QByteArray &ba) noexcept
        : m_data(ba.constData())
        , m_size(ba.size())
    {
    }

    QByteArrayRef(const QByteArray &ba, int pos, int length = -1) noexcept
    {
        if (pos < 0 || pos > ba.size()) {
            m_data = nullptr;
            m_size = 0;
            return;
        }
        m_data = ba.constData() + pos;
        if (length < 0 || pos + length > ba.size()) {
            m_size = ba.size() - pos;
        } else {
            m_size = length;
        }
    }

    // 基础属性
    [[nodiscard]] constexpr const char *data() const noexcept { return m_data; }
    [[nodiscard]] constexpr const char *constData() const noexcept { return m_data; }
    [[nodiscard]] constexpr int size() const noexcept { return m_size; }
    [[nodiscard]] constexpr int length() const noexcept { return m_size; }
    [[nodiscard]] constexpr bool isEmpty() const noexcept { return m_size == 0; }
    [[nodiscard]] constexpr bool isNull() const noexcept { return m_data == nullptr; }

    // 元素访问
    [[nodiscard]] char at(int i) const
    {
        Q_ASSERT(i >= 0 && i < m_size);
        return m_data[i];
    }

    [[nodiscard]] char operator[](int i) const
    {
        return at(i);
    }

    // 零拷贝切片操作 (返回新的引用视图)
    [[nodiscard]] QByteArrayRef left(int n) const noexcept
    {
        if (n <= 0) return QByteArrayRef();
        if (n >= m_size) return *this;
        return QByteArrayRef(m_data, n);
    }

    [[nodiscard]] QByteArrayRef right(int n) const noexcept
    {
        if (n <= 0) return QByteArrayRef();
        if (n >= m_size) return *this;
        return QByteArrayRef(m_data + (m_size - n), n);
    }

    [[nodiscard]] QByteArrayRef mid(int pos, int n = -1) const noexcept
    {
        if (pos < 0 || pos > m_size) return QByteArrayRef();
        int maxLen = m_size - pos;
        if (n < 0 || n > maxLen) {
            n = maxLen;
        }
        return QByteArrayRef(m_data + pos, n);
    }

    [[nodiscard]] QByteArrayRef chopped(int n) const noexcept
    {
        if (n <= 0) return *this;
        if (n >= m_size) return QByteArrayRef();
        return QByteArrayRef(m_data, m_size - n);
    }

    // 空白清洗切片
    [[nodiscard]] QByteArrayRef trimmed() const noexcept
    {
        if (isEmpty()) return *this;
        int start = 0;
        while (start < m_size && static_cast<unsigned char>(m_data[start]) <= ' ') {
            ++start;
        }
        int end = m_size - 1;
        while (end >= start && static_cast<unsigned char>(m_data[end]) <= ' ') {
            --end;
        }
        return QByteArrayRef(m_data + start, end - start + 1);
    }

    // 搜索查找
    [[nodiscard]] int indexOf(char ch, int from = 0) const noexcept
    {
        if (from < 0) from = 0;
        for (int i = from; i < m_size; ++i) {
            if (m_data[i] == ch) return i;
        }
        return -1;
    }

    [[nodiscard]] bool startsWith(char ch) const noexcept
    {
        return !isEmpty() && m_data[0] == ch;
    }

    [[nodiscard]] bool startsWith(const QByteArrayRef &other) const noexcept
    {
        if (other.m_size > m_size) return false;
        return std::memcmp(m_data, other.m_data, other.m_size) == 0;
    }

    [[nodiscard]] bool endsWith(char ch) const noexcept
    {
        return !isEmpty() && m_data[m_size - 1] == ch;
    }

    // 转换为实体对象与数值解析
    [[nodiscard]] QByteArray toByteArray() const
    {
        if (!m_data || m_size <= 0) return QByteArray();
        return QByteArray(m_data, m_size);
    }

    [[nodiscard]] QByteArray toHex(char separator = '\0') const
    {
        return toByteArray().toHex(separator);
    }

    [[nodiscard]] int toInt(bool *ok = nullptr, int base = 10) const
    {
        return toByteArray().toInt(ok, base);
    }

    [[nodiscard]] double toDouble(bool *ok = nullptr) const
    {
        return toByteArray().toDouble(ok);
    }

    // 比较运算符
    friend bool operator==(const QByteArrayRef &lhs, const QByteArrayRef &rhs) noexcept
    {
        if (lhs.size() != rhs.size()) return false;
        if (lhs.data() == rhs.data()) return true;
        return std::memcmp(lhs.data(), rhs.data(), lhs.size()) == 0;
    }

    friend bool operator!=(const QByteArrayRef &lhs, const QByteArrayRef &rhs) noexcept
    {
        return !(lhs == rhs);
    }

    friend bool operator==(const QByteArrayRef &lhs, const QByteArray &rhs) noexcept
    {
        return lhs == QByteArrayRef(rhs);
    }

    friend bool operator==(const QByteArray &lhs, const QByteArrayRef &rhs) noexcept
    {
        return QByteArrayRef(lhs) == rhs;
    }

private:
    const char *m_data{nullptr};
    int m_size{0};
};

inline QDebug operator<<(QDebug dbg, const QByteArrayRef &ref)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "QByteArrayRef(\"" << ref.toByteArray() << "\", size=" << ref.size() << ")";
    return dbg;
}
