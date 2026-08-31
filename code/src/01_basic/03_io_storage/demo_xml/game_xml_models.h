#pragma once

#include <QString>
#include <QList>

// ============================================================================
// RPG 游戏任务结构体
// ============================================================================
struct GameQuest
{
    int id{0};
    QString title;
    int minLevel{1};
    int rewardGold{0};
    int rewardExp{0};
    QString targetNpc;

    GameQuest() = default;
    GameQuest(int qId, const QString &qTitle, int reqLvl, int gold, int exp, const QString &npc)
        : id(qId), title(qTitle), minLevel(reqLvl), rewardGold(gold), rewardExp(exp), targetNpc(npc)
    {
    }
};

// ============================================================================
// RPG 游戏武器装备图鉴结构体
// ============================================================================
struct GameWeapon
{
    int id{0};
    QString name;
    QString rarity; // "SSR", "SR", "R"
    int attackDamage{0};
    QString element; // "Holy", "Frost", "Shadow"
    QString description;

    GameWeapon() = default;
    GameWeapon(int wId, const QString &wName, const QString &wRarity, int dmg, const QString &elem, const QString &desc)
        : id(wId), name(wName), rarity(wRarity), attackDamage(dmg), element(elem), description(desc)
    {
    }
};
