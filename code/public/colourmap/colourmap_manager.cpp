#include "colourmap_manager.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>

ColourMapManager& ColourMapManager::instance()
{
    static ColourMapManager s_instance;
    return s_instance;
}

ColourMapManager::ColourMapManager()
{
    createFallbackBatlow();
}

void ColourMapManager::createFallbackBatlow()
{
    m_fallbackMap = std::make_shared<ColourMap>(QStringLiteral("batlow_builtin"));
    // 内置 batlow 关键特征节点
    m_fallbackMap->addStop(0.00,   1,  25,  89, 255);
    m_fallbackMap->addStop(0.20,  24,  85,  98, 255);
    m_fallbackMap->addStop(0.40,  87, 118,  71, 255);
    m_fallbackMap->addStop(0.60, 179, 142,  47, 255);
    m_fallbackMap->addStop(0.80, 251, 166, 137, 255);
    m_fallbackMap->addStop(1.00, 250, 204, 250, 255);
}

bool ColourMapManager::initialize(const QString &dirPath)
{
    m_dirPath = dirPath;
    QDir dir(dirPath);
    if (!dir.exists()) {
        qWarning() << "[ColourMapManager] Directory does not exist:" << dirPath;
        return false;
    }

    QStringList filters;
    filters << QStringLiteral("*.clr");
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files | QDir::Readable, QDir::Name);

    m_maps.clear();
    for (const QFileInfo &info : fileList) {
        auto map = std::make_shared<ColourMap>();
        if (map->loadFromFile(info.absoluteFilePath())) {
            m_maps.insert(map->name(), map);
        }
    }

    qInfo() << "[ColourMapManager] Loaded" << m_maps.size() << "colourmaps from:" << dirPath;
    return !m_maps.isEmpty();
}

QStringList ColourMapManager::availableMapNames() const
{
    if (m_maps.isEmpty()) {
        return QStringList() << m_fallbackMap->name();
    }
    return m_maps.keys();
}

std::shared_ptr<ColourMap> ColourMapManager::getMap(const QString &name)
{
    if (m_maps.contains(name)) {
        return m_maps.value(name);
    }
    return defaultMap();
}

std::shared_ptr<ColourMap> ColourMapManager::defaultMap()
{
    if (m_maps.contains(QStringLiteral("batlow"))) {
        return m_maps.value(QStringLiteral("batlow"));
    }
    if (!m_maps.isEmpty()) {
        return m_maps.first();
    }
    return m_fallbackMap;
}

void ColourMapManager::registerMap(const std::shared_ptr<ColourMap> &map)
{
    if (map && map->isValid()) {
        m_maps.insert(map->name(), map);
    }
}
