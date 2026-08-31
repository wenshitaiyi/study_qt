#pragma once

#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>

#include "game_types.h"

class VariantDemo
{
public:
    // 1. 英雄属性万能字典 (QVariantMap): 基本数据类型的万能装载与读取
    static void testBasicGameAttributes();

    // 2. 异构游戏背包 (QVariantList): 任意类型混合存储的无缝实现
    static void testGameInventoryList();

    // 3. 自定义结构体包装 (Q_DECLARE_METATYPE + qRegisterMetaType) 实战
    static void testCustomStructPackingAndUnpacking();

    // 4. 复杂嵌套数据结构：怪物 Boss 掉落与任务奖励配置 (Map 嵌套 List 嵌套 自定义对象)
    static void testNestedQuestRewardConfig();

    // 5. 类型安全判定 (canConvert / typeName / userType) 与类型转换避坑
    static void testTypeCheckingAndSafeConversion();
};
