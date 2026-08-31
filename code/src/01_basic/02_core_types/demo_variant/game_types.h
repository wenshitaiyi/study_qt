#pragma once

#include <QString>
#include <QMetaType>
#include <QDebug>

// ============================================================================
// 游戏装备道具结构体
// ============================================================================
struct GameItem
{
    int itemId{0};
    QString name;
    QString rarity; // 如 "SSR", "SR", "R"
    double attackPower{0.0};
    int durability{100};

    // 默认构造与拷贝构造
    GameItem() = default;
    GameItem(int id, const QString &n, const QString &r, double atk, int dur = 100)
        : itemId(id), name(n), rarity(r), attackPower(atk), durability(dur)
    {
    }
};

// ============================================================================
// 技能增益 Buff 结构体
// ============================================================================
struct SkillBuff
{
    QString buffName;
    double attackMultiplier{1.0};
    int durationSec{30};

    SkillBuff() = default;
    SkillBuff(const QString &name, double multi, int sec)
        : buffName(name), attackMultiplier(multi), durationSec(sec)
    {
    }
};

// ============================================================================
// 玩家档案结构体
// ============================================================================
struct PlayerProfile
{
    int uid{0};
    QString nickname;
    int level{1};
    double maxHp{1000.0};
    bool isVip{false};

    PlayerProfile() = default;
    PlayerProfile(int id, const QString &name, int lvl, double hp, bool vip)
        : uid(id), nickname(name), level(lvl), maxHp(hp), isVip(vip)
    {
    }
};

// 【关键宏】：必须在全局作用域使用 Q_DECLARE_METATYPE 声明自定义类型，
// 使其能够被 QVariant 安全包装和提取！
Q_DECLARE_METATYPE(GameItem)
Q_DECLARE_METATYPE(SkillBuff)
Q_DECLARE_METATYPE(PlayerProfile)
