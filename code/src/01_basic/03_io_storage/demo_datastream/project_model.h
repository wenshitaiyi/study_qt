#pragma once

#include <QString>
#include <QColor>
#include <QRect>
#include <QDateTime>
#include <QList>
#include <QDataStream>

// ============================================================================
// CAD / 矢量设计器 图元结构体
// ============================================================================
struct GraphicShape
{
    int shapeType{0};    // 0: 矩形, 1: 圆形, 2: 文本
    QString label;
    QRect boundingBox;
    QColor fillColor;    // v2 版本新增字段 (测试向后兼容)

    GraphicShape() = default;
    GraphicShape(int type, const QString &lbl, const QRect &rect, const QColor &color = Qt::white)
        : shapeType(type), label(lbl), boundingBox(rect), fillColor(color)
    {
    }
};

// ============================================================================
// CAD 工程文件完整模型
// ============================================================================
class CadProject
{
public:
    static constexpr quint32 MAGIC_HEADER = 0x51544341; // "QTCA" (Qt CAD)
    static constexpr quint32 CURRENT_VERSION = 2;       // 当前格式版本

    CadProject() = default;
    CadProject(const QString &name, const QString &author);

    QString projectName() const { return m_projectName; }
    QString author() const { return m_author; }
    QDateTime createTime() const { return m_createTime; }
    QList<GraphicShape> shapes() const { return m_shapes; }

    void addShape(const GraphicShape &shape) { m_shapes.append(shape); }

    // 序列化与反序列化流操作符
    friend QDataStream &operator<<(QDataStream &out, const CadProject &proj);
    friend QDataStream &operator>>(QDataStream &in, CadProject &proj);

private:
    QString m_projectName;
    QString m_author;
    QDateTime m_createTime;
    QList<GraphicShape> m_shapes;
};

// 重载图元输入输出流操作符
QDataStream &operator<<(QDataStream &out, const GraphicShape &shape);
QDataStream &operator>>(QDataStream &in, GraphicShape &shape);
