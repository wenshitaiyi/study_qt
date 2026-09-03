#pragma once

#include "colourmap.h"
#include <QString>
#include <QStringList>
#include <QMap>
#include <memory>

class ColourMapManager {
public:
    static ColourMapManager& instance();

    // 扫描并加载目标目录下的全部 .clr 文件
    bool initialize(const QString &dirPath);
    QString directoryPath() const { return m_dirPath; }

    // 获取所有可用色带名称列表
    QStringList availableMapNames() const;

    // 按名称获取色带
    std::shared_ptr<ColourMap> getMap(const QString &name);

    // 获取默认回退色带（优先 batlow，次选列表中首个，或程序内建色带）
    std::shared_ptr<ColourMap> defaultMap();

    // 注册自定义色带
    void registerMap(const std::shared_ptr<ColourMap> &map);

private:
    ColourMapManager();
    ~ColourMapManager() = default;
    ColourMapManager(const ColourMapManager&) = delete;
    ColourMapManager& operator=(const ColourMapManager&) = delete;

    void createFallbackBatlow();

    QString m_dirPath;
    QMap<QString, std::shared_ptr<ColourMap>> m_maps;
    std::shared_ptr<ColourMap> m_fallbackMap;
};
