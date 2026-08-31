#include "game_hero.h"

GameHero::GameHero(int id, const QString &name, HeroRole role, QObject *parent)
    : QObject(parent)
    , m_heroId(id)
    , m_heroName(name)
    , m_role(role)
    , m_level(1)
    , m_health(1000)
    , m_attackPower(150.0)
{
}

void GameHero::setHeroName(const QString &name)
{
    if (m_heroName != name) {
        m_heroName = name;
        emit heroNameChanged(m_heroName);
    }
}

void GameHero::setRole(HeroRole role)
{
    if (m_role != role) {
        m_role = role;
        emit roleChanged(m_role);
    }
}

void GameHero::setLevel(int lvl)
{
    if (m_level != lvl) {
        m_level = lvl;
        emit levelChanged(m_level);
    }
}

void GameHero::setHealth(int hp)
{
    hp = qMax(0, hp);
    if (m_health != hp) {
        bool wasAlive = isAlive();
        m_health = hp;
        emit healthChanged(m_health);

        if (wasAlive && !isAlive()) {
            emit isAliveChanged(false);
            emit heroDied(m_heroName);
        } else if (!wasAlive && isAlive()) {
            emit isAliveChanged(true);
        }
    }
}

int GameHero::castSkill(const QString &skillName, int multiplier)
{
    int totalDamage = static_cast<int>(m_attackPower * multiplier);
    qInfo() << QStringLiteral("[GameHero::castSkill] 英雄【%1】施放技能「%2」，造成暴击总伤害: %3 点！")
               .arg(m_heroName).arg(skillName).arg(totalDamage);
    return totalDamage;
}

void GameHero::takeDamage(int damage)
{
    qInfo() << QStringLiteral("[GameHero::takeDamage] 英雄【%1】受到 %2 点敌方伤害！").arg(m_heroName).arg(damage);
    setHealth(m_health - damage);
}

QString GameHero::getFullProfile() const
{
    return QStringLiteral("【英雄档案】ID: %1 | 姓名: %2 | 等级: Lv.%3 | 生命: %4 | 攻击力: %5 | 状态: %6")
        .arg(m_heroId).arg(m_heroName).arg(m_level).arg(m_health).arg(m_attackPower)
        .arg(isAlive() ? QStringLiteral("存活") : QStringLiteral("阵亡"));
}

void GameHero::revive(int restoreHp)
{
    qInfo() << QStringLiteral("[GameHero::revive (槽函数)] 英雄【%1】接受复活之光，恢复生命值: %2！").arg(m_heroName).arg(restoreHp);
    setHealth(restoreHp);
}

void GameHero::levelUpBonus()
{
    setLevel(m_level + 1);
    m_attackPower += 25.0;
    setHealth(m_health + 200);
    qInfo() << QStringLiteral("[GameHero::levelUpBonus (槽函数)] 英雄【%1】升级至 Lv.%2！攻击力提升为 %3，生命恢复。")
               .arg(m_heroName).arg(m_level).arg(m_attackPower);
}
