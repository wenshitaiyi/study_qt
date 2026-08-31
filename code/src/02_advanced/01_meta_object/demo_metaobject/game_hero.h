#pragma once

#include <QObject>
#include <QString>
#include <QDebug>

/**
 * @brief 游戏英雄领域模型类 (专门用于深入演示 Qt 元对象系统反射特性)
 * 
 * 涵盖知识点：
 * 1. Q_OBJECT 宏：开启元对象内省与信号槽支持
 * 2. Q_CLASSINFO 宏：附加类级元数据键值对 (如 Author, Version, Schema)
 * 3. Q_ENUM 宏：注册枚举类型到元对象系统，支持字符串与枚举双向反射转换
 * 4. Q_PROPERTY 宏：声明静态属性 (READ, WRITE, NOTIFY, RESET)
 * 5. Q_INVOKABLE 宏：修饰普通成员函数，使其可通过 QMetaObject::invokeMethod 按字符串名字动态调用
 */
class GameHero : public QObject
{
    Q_OBJECT

    // 1. 类元信息附加
    Q_CLASSINFO("Author", "Antigravity Team")
    Q_CLASSINFO("Version", "2.1.0")
    Q_CLASSINFO("Description", "RPG Hero Actor with Complete MetaObject Capabilities")

    // 2. 静态属性系统声明
    Q_PROPERTY(int heroId READ heroId WRITE setHeroId)
    Q_PROPERTY(QString heroName READ heroName WRITE setHeroName NOTIFY heroNameChanged)
    Q_PROPERTY(HeroRole role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(int health READ health WRITE setHealth NOTIFY healthChanged)
    Q_PROPERTY(double attackPower READ attackPower WRITE setAttackPower RESET resetAttackPower)
    Q_PROPERTY(bool isAlive READ isAlive NOTIFY isAliveChanged)

public:
    // 枚举定义并注册到元系统
    enum HeroRole {
        Tank,       // 坦克防战
        Warrior,    // 武器狂战
        Mage,       // 毁灭法师
        Assassin,   // 暗影刺客
        Priest      // 神圣牧师
    };
    Q_ENUM(HeroRole)

    explicit GameHero(int id, const QString &name, HeroRole role, QObject *parent = nullptr);
    ~GameHero() override = default;

    // 属性 Getter & Setter
    int heroId() const { return m_heroId; }
    void setHeroId(int id) { m_heroId = id; }

    QString heroName() const { return m_heroName; }
    void setHeroName(const QString &name);

    HeroRole role() const { return m_role; }
    void setRole(HeroRole role);

    int level() const { return m_level; }
    void setLevel(int lvl);

    int health() const { return m_health; }
    void setHealth(int hp);

    double attackPower() const { return m_attackPower; }
    void setAttackPower(double power) { m_attackPower = power; }
    void resetAttackPower() { m_attackPower = 100.0; }

    bool isAlive() const { return m_health > 0; }

    // ========================================================================
    // 3. Q_INVOKABLE 动态可调用函数
    // ========================================================================
    Q_INVOKABLE int castSkill(const QString &skillName, int multiplier);
    Q_INVOKABLE void takeDamage(int damage);
    Q_INVOKABLE QString getFullProfile() const;

public slots:
    // 公共槽函数 (默认已被元对象系统注册为可调用)
    void revive(int restoreHp);
    void levelUpBonus();

signals:
    // 信号声明
    void heroNameChanged(const QString &newName);
    void roleChanged(HeroRole newRole);
    void levelChanged(int newLevel);
    void healthChanged(int newHealth);
    void isAliveChanged(bool alive);
    void heroDied(const QString &heroName);

private:
    int m_heroId{1001};
    QString m_heroName;
    HeroRole m_role{Warrior};
    int m_level{1};
    int m_health{1000};
    double m_attackPower{100.0};
};
