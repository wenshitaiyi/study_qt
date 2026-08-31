#include "json_utils.h"

#include <QStringList>
#include <QRegularExpression>

namespace JsonUtils
{

// 将路径字符串（如 "server.cluster[0].node.port"）拆解为原子 Token 列表
static QStringList tokenizePath(const QString &path)
{
    QStringList tokens;
    // 规范化：将 "[0]" 替换为 ".[0]"
    QString normalized = path;
    normalized.replace(QStringLiteral("["), QStringLiteral(".["));
    
    QStringList parts = normalized.split('.', QString::SkipEmptyParts);
    for (const QString &part : parts) {
        QString trimmed = part.trimmed();
        if (!trimmed.isEmpty()) {
            tokens.append(trimmed);
        }
    }
    return tokens;
}

QJsonObject deepMerge(const QJsonObject &base, const QJsonObject &patch)
{
    QJsonObject result = base;
    for (auto it = patch.constBegin(); it != patch.constEnd(); ++it) {
        const QString &key = it.key();
        const QJsonValue &patchVal = it.value();

        if (result.contains(key) && result.value(key).isObject() && patchVal.isObject()) {
            // 两者都是对象，递归深合并
            result[key] = deepMerge(result.value(key).toObject(), patchVal.toObject());
        } else {
            // 基础类型、数组或非共有键直接覆盖/插入
            result[key] = patchVal;
        }
    }
    return result;
}

QJsonValue getByPath(const QJsonValue &root, const QString &path)
{
    QStringList tokens = tokenizePath(path);
    if (tokens.isEmpty()) {
        return root;
    }

    QJsonValue current = root;
    for (const QString &token : tokens) {
        if (token.startsWith('[') && token.endsWith(']')) {
            // 数组下标处理
            bool ok = false;
            int index = token.mid(1, token.length() - 2).toInt(&ok);
            if (!ok || !current.isArray()) {
                return QJsonValue::Undefined;
            }
            QJsonArray arr = current.toArray();
            if (index < 0 || index >= arr.size()) {
                return QJsonValue::Undefined;
            }
            current = arr.at(index);
        } else {
            // 对象字段键名处理
            if (!current.isObject()) {
                return QJsonValue::Undefined;
            }
            QJsonObject obj = current.toObject();
            if (!obj.contains(token)) {
                return QJsonValue::Undefined;
            }
            current = obj.value(token);
        }
    }
    return current;
}

// 递归辅助设置路径
static bool setByPathInternal(QJsonObject &currentObj, const QStringList &tokens, int index, const QJsonValue &newValue)
{
    if (index >= tokens.size()) {
        return false;
    }

    const QString &token = tokens[index];
    if (index == tokens.size() - 1) {
        // 最后一个叶子节点，执行写入
        currentObj[token] = newValue;
        return true;
    }

    // 中间节点：若不存在或不是对象则自动创建
    QJsonObject childObj;
    if (currentObj.contains(token) && currentObj.value(token).isObject()) {
        childObj = currentObj.value(token).toObject();
    }

    bool success = setByPathInternal(childObj, tokens, index + 1, newValue);
    if (success) {
        currentObj[token] = childObj;
    }
    return success;
}

bool setByPath(QJsonObject &root, const QString &path, const QJsonValue &newValue)
{
    QStringList tokens = tokenizePath(path);
    if (tokens.isEmpty()) {
        return false;
    }

    return setByPathInternal(root, tokens, 0, newValue);
}

} // namespace JsonUtils
