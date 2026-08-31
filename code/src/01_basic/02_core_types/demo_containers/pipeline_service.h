#pragma once

// 【强迫症模式】：Qt 官方容器前向声明专属头文件 <qcontainerfwd.h>
// 仅包含 QVector, QList, QMap, QHash, QSet, QStringList 等模板类的前置声明，
// 零容器实现代码开销，极大提升大型工程头文件解析与编译速度！
#include <QtCore/qcontainerfwd.h>
#include <QString>

class ForwardDeclaredItem;

// 演示纯虚接口类：所有方法均使用容器引用与前向声明类型
class IPipelineService
{
public:
    virtual ~IPipelineService() = default;

    virtual void processBatch(const QVector<ForwardDeclaredItem> &items) = 0;
    virtual void setFilterTags(const QStringList &tags) = 0;
    virtual void collectMetrics(QMap<QString, int> &outMetrics) const = 0;
};
