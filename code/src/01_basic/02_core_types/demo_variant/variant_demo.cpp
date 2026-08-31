#include "variant_demo.h"

#include <iostream>
#include <iomanip>

static void printTitle(const QString &title)
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  " << title.toUtf8().constData() << std::endl;
    std::cout << "============================================================" << std::endl;
}

static void printLine(const QString &title, const QString &content)
{
    std::cout << QStringLiteral("  • %1 : %2")
                    .arg(title.leftJustified(30, ' '))
                    .arg(content)
                    .toUtf8().constData()
              << std::endl;
}

void VariantDemo::testBasicGameAttributes()
{
    printTitle(QStringLiteral("[模块 1] 英雄属性万能字典 (QVariantMap)"));

    // 构建一个英雄角色动态属性字典 (包含各种不同基础数据类型)
    QVariantMap hero;
    hero[QStringLiteral("heroName")] = QStringLiteral("亚瑟王 (King Arthur)");
    hero[QStringLiteral("level")] = 85;
    hero[QStringLiteral("attackPower")] = 3450.75;
    hero[QStringLiteral("defense")] = 1820;
    hero[QStringLiteral("critRate")] = 0.45; // 45% 暴击率
    hero[QStringLiteral("isAlive")] = true;
    hero[QStringLiteral("guildName")] = QStringLiteral("圆桌骑士团 (Knights of the Round)");

    std::cout << "  1. 遍历 QVariantMap 读取英雄各属性 :" << std::endl;
    printLine(QStringLiteral("英雄姓名 (toString)"), hero.value(QStringLiteral("heroName")).toString());
    printLine(QStringLiteral("英雄等级 (toInt)"), QString::number(hero.value(QStringLiteral("level")).toInt()));
    printLine(QStringLiteral("基础攻击力 (toDouble)"), QString::number(hero.value(QStringLiteral("attackPower")).toDouble(), 'f', 2));
    printLine(QStringLiteral("暴击概率 (百分比显示)"), QStringLiteral("%1%").arg(hero.value(QStringLiteral("critRate")).toDouble() * 100, 0, 'f', 1));
    printLine(QStringLiteral("存活状态 (toBool)"), hero.value(QStringLiteral("isAlive")).toBool() ? QStringLiteral("存活中 (Alive)") : QStringLiteral("已阵亡 (Dead)"));
    printLine(QStringLiteral("所属公会 (toString)"), hero.value(QStringLiteral("guildName")).toString());
}

void VariantDemo::testGameInventoryList()
{
    printTitle(QStringLiteral("[模块 2] 异构游戏背包系统 (QVariantList)"));

    // 注册自定义类型元数据
    qRegisterMetaType<GameItem>("GameItem");

    // 一个通用的背包槽位列表 QVariantList，允许混合存放任意类型的数据与道具！
    QVariantList backpack;

    // 格子 0: 基础整型金币
    backpack.append(500000);
    // 格子 1: 字符串消耗品
    backpack.append(QStringLiteral("特效大生命药水 x 10"));
    // 格子 2: 浮点数双倍经验卡增益倍率
    backpack.append(2.5);
    // 格子 3: 自定义结构体 SSR 神装武器！
    GameItem excalibur(1001, QStringLiteral("誓约胜利之剑 (Excalibur)"), QStringLiteral("SSR"), 9999.0, 100);
    backpack.append(QVariant::fromValue(excalibur));

    std::cout << "  背包物品栏遍历解包 (异构多态存储) :" << std::endl;
    for (int i = 0; i < backpack.size(); ++i) {
        const QVariant &slot = backpack.at(i);
        QString typeName = QString::fromLatin1(slot.typeName());

        if (slot.canConvert<GameItem>()) {
            GameItem item = slot.value<GameItem>();
            std::cout << QStringLiteral("    [槽位 %1] 【自定义武器】品阶:%2 | 名称:%3 | 攻击:+%4 | 耐久:%5 (类型:%6)")
                            .arg(i)
                            .arg(item.rarity)
                            .arg(item.name)
                            .arg(item.attackPower)
                            .arg(item.durability)
                            .arg(typeName)
                            .toUtf8().constData()
                      << std::endl;
        } else if (slot.type() == QVariant::Int) {
            std::cout << QStringLiteral("    [槽位 %1] 【游戏金币】%2 G (类型:int)").arg(i).arg(slot.toInt()).toUtf8().constData() << std::endl;
        } else if (slot.type() == QVariant::String) {
            std::cout << QStringLiteral("    [槽位 %1] 【消耗道具】%2 (类型:QString)").arg(i).arg(slot.toString()).toUtf8().constData() << std::endl;
        } else if (slot.type() == QVariant::Double) {
            std::cout << QStringLiteral("    [槽位 %1] 【增益倍率】%2x 经验卡 (类型:double)").arg(i).arg(slot.toDouble()).toUtf8().constData() << std::endl;
        }
    }
}

void VariantDemo::testCustomStructPackingAndUnpacking()
{
    printTitle(QStringLiteral("[模块 3] 自定义结构体包装 (Q_DECLARE_METATYPE + qRegisterMetaType)"));

    // 1. 注册元类型
    int typeIdBuff = qRegisterMetaType<SkillBuff>("SkillBuff");
    int typeIdProfile = qRegisterMetaType<PlayerProfile>("PlayerProfile");

    printLine(QStringLiteral("SkillBuff 注册 TypeId"), QString::number(typeIdBuff));
    printLine(QStringLiteral("PlayerProfile 注册 TypeId"), QString::number(typeIdProfile));

    // 2. 包装自定义 Buff 对象到 QVariant
    SkillBuff dragonRoar(QStringLiteral("远古龙之怒 (Dragon's Fury)"), 1.65, 45);
    QVariant varBuff = QVariant::fromValue(dragonRoar);

    printLine(QStringLiteral("varBuff 存储类型名 typeName()"), QString::fromLatin1(varBuff.typeName()));
    printLine(QStringLiteral("varBuff userType() 标识码"), QString::number(varBuff.userType()));

    // 3. 安全取出并还原 (value<T>() 与 qvariant_cast<T>())
    SkillBuff unpackedBuff = varBuff.value<SkillBuff>();
    SkillBuff castedBuff = qvariant_cast<SkillBuff>(varBuff);

    printLine(QStringLiteral("value<T>() 解包 Buff 名称"), unpackedBuff.buffName);
    printLine(QStringLiteral("解包伤害加成倍率"), QStringLiteral("+%1%").arg((unpackedBuff.attackMultiplier - 1.0) * 100.0, 0, 'f', 1));
    printLine(QStringLiteral("解包持续时间 (秒)"), QStringLiteral("%1 秒").arg(castedBuff.durationSec));
}

void VariantDemo::testNestedQuestRewardConfig()
{
    printTitle(QStringLiteral("[模块 4] 复杂嵌套配置：世界 Boss 掉落与任务奖励"));

    // 构建一个由 Map 嵌套 List 嵌套 自定义结构的复杂数据树
    QVariantMap bossDropConfig;
    bossDropConfig[QStringLiteral("bossName")] = QStringLiteral("黑龙领主·奈法利安");
    bossDropConfig[QStringLiteral("recommendLevel")] = 90;
    bossDropConfig[QStringLiteral("guaranteedGold")] = 88888;

    // 掉落装备列表
    QVariantList dropEquipments;
    dropEquipments.append(QVariant::fromValue(GameItem(2001, QStringLiteral("黑龙之爪"), QStringLiteral("SSR"), 1580.0, 100)));
    dropEquipments.append(QVariant::fromValue(GameItem(2002, QStringLiteral("龙鳞胸甲"), QStringLiteral("SR"), 450.0, 120)));
    dropEquipments.append(QVariant::fromValue(GameItem(2003, QStringLiteral("炽热龙晶戒指"), QStringLiteral("SSR"), 2100.0, 80)));

    bossDropConfig[QStringLiteral("equipmentDrops")] = dropEquipments;

    // 解析复杂嵌套数据
    printLine(QStringLiteral("击败首领"), bossDropConfig.value(QStringLiteral("bossName")).toString());
    printLine(QStringLiteral("保底金币奖励"), QStringLiteral("%1 G").arg(bossDropConfig.value(QStringLiteral("guaranteedGold")).toInt()));

    QVariantList parsedDrops = bossDropConfig.value(QStringLiteral("equipmentDrops")).toList();
    std::cout << "  获得战利品装备列表 (" << parsedDrops.size() << " 件) :" << std::endl;
    for (const auto &varItem : parsedDrops) {
        if (varItem.canConvert<GameItem>()) {
            GameItem item = varItem.value<GameItem>();
            std::cout << QStringLiteral("    🎁 [%1] %2 (攻击加成: +%3)").arg(item.rarity.leftJustified(4, ' '), item.name.leftJustified(18, ' ')).arg(item.attackPower).toUtf8().constData() << std::endl;
        }
    }
}

void VariantDemo::testTypeCheckingAndSafeConversion()
{
    printTitle(QStringLiteral("[模块 5] 类型安全判定 (canConvert / type / userType) 与避坑"));

    // 1. 成功隐式类型转换 (QString -> int / double)
    QVariant strNumber(QStringLiteral("6688"));
    printLine(QStringLiteral("strNumber 原始数据"), strNumber.toString());
    printLine(QStringLiteral("canConvert<int>() 判定"), strNumber.canConvert<int>() ? QStringLiteral("可以转换 (True)") : QStringLiteral("不可转换 (False)"));
    printLine(QStringLiteral("成功转换为 int"), QString::number(strNumber.toInt()));

    // 2. 非法类型转换保护 (QString 非数字 -> int 失败保护)
    QVariant invalidStr(QStringLiteral("MagicSpell_Fireball"));
    bool ok = false;
    int failedResult = invalidStr.toInt(&ok);
    printLine(QStringLiteral("非数字文本尝试转 int"), QStringLiteral("转换结果=%1 (转换状态 ok=%2, 优雅降级不崩溃)").arg(failedResult).arg(ok ? "True" : "False"));

    // 3. QVariant::isValid() 与 QVariant::isNull()
    QVariant emptyVariant;
    printLine(QStringLiteral("未初始化 Variant isValid()"), emptyVariant.isValid() ? QStringLiteral("有效 (True)") : QStringLiteral("无效 (False, 相当于未赋值)"));
    printLine(QStringLiteral("未初始化 Variant isNull()"), emptyVariant.isNull() ? QStringLiteral("True (空)") : QStringLiteral("False"));
}
