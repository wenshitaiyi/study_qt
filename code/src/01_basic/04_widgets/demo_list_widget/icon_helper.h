#pragma once

#include <QIcon>
#include <QString>
#include <QVector>

/**
 * @brief 游戏图标绘制与加载辅助工具类
 * 
 * 使用 QPainter 纯代码动态绘制 50 个 64x64 高质感游戏装备图标，
 * 包含品质渐变底板、高光边框与专属装备符号，并自动持久化保存到 data/icons 目录。
 */
class IconHelper
{
public:
    /// @brief 确保本地存在 50 个 64x64 图标，若不存在则使用 QPainter 自动批量绘制并保存为 PNG
    static void ensureIconsExist();

    /// @brief 获取指定编号 (1~50) 的图标
    static QIcon getIcon(int index);

    /// @brief 获取图标存储目录绝对路径
    static QString getIconsDir();

    /// @brief 使用 QPainter 动态绘制单个 64x64 图标
    static QPixmap drawIcon(int index, int size = 64);
};
