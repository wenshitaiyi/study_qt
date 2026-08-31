#pragma once

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>

namespace JsonUtils
{
    // 1. JSON 深度递归合并 (Deep Merge)
    // 将 patch 中的所有字段递归合并入 base，相同层级的对象合并，基础值则覆盖
    QJsonObject deepMerge(const QJsonObject &base, const QJsonObject &patch);

    // 2. 基于点分与下标路径提取 JSON 值 (如 "database.pool.max_size" 或 "servers[0].ip")
    QJsonValue getByPath(const QJsonValue &root, const QString &path);

    // 3. 基于点分路径设置/更新 JSON 值 (支持自动创建缺失的中间对象)
    bool setByPath(QJsonObject &root, const QString &path, const QJsonValue &newValue);
}
