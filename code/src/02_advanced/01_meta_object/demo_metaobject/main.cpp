#include <QCoreApplication>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMetaMethod>
#include <QMetaEnum>
#include <QMetaClassInfo>
#include <QMetaType>
#include <QTextStream>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "game_hero.h"

static QTextStream s_out(stdout);

void printLine(const QString &text)
{
    s_out << text << QStringLiteral("\n");
    s_out.flush();
}

void printSectionHeader(const QString &title)
{
    s_out << QStringLiteral("\n======================================================================\n");
    s_out << QStringLiteral("  ") << title << QStringLiteral("\n");
    s_out << QStringLiteral("======================================================================\n");
    s_out.flush();
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SetConsoleOutputCP(65001); // 开启 UTF-8 控制台代码页
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    s_out.setCodec("UTF-8");
#endif

    QCoreApplication app(argc, argv);
    app.setApplicationName("demo_metaobject");

    // 实例化一个游戏英雄对象用于全方位元对象内省
    GameHero paladin(1001, QStringLiteral("乌瑟尔·光明使者"), GameHero::Tank);

    // ========================================================================
    // 1. 类层次与类元信息内省 (Class Info & Inheritance)
    // ========================================================================
    printSectionHeader(QStringLiteral("1. 类层次与类元信息内省 (Q_CLASSINFO & Inheritance)"));

    const QMetaObject *meta = paladin.metaObject();
    printLine(QStringLiteral("当前对象类名 (className): ") + QString::fromLatin1(meta->className()));
    printLine(QStringLiteral("父类名称 (superClass):     ") + (meta->superClass() ? QString::fromLatin1(meta->superClass()->className()) : QStringLiteral("None")));
    printLine(QStringLiteral("是否继承自 QObject:        ") + (paladin.inherits("QObject") ? QStringLiteral("true") : QStringLiteral("false")));
    printLine(QStringLiteral("是否继承自 QWidget:        ") + (paladin.inherits("QWidget") ? QStringLiteral("true") : QStringLiteral("false")));

    printLine(QStringLiteral("\n--- 类附加元数据 (Q_CLASSINFO) ---"));
    for (int i = 0; i < meta->classInfoCount(); ++i) {
        QMetaClassInfo info = meta->classInfo(i);
        printLine(QStringLiteral("  [%1] %2 = \"%3\"").arg(i).arg(QString::fromLatin1(info.name())).arg(QString::fromUtf8(info.value())));
    }

    // ========================================================================
    // 2. 静态属性系统内省与读写 (Q_PROPERTY)
    // ========================================================================
    printSectionHeader(QStringLiteral("2. 静态属性系统内省与反射读写 (Q_PROPERTY)"));

    printLine(QStringLiteral("属性总数 (包含继承自 QObject 的属性): %1").arg(meta->propertyCount()));
    printLine(QStringLiteral("当前类自定义属性起始偏移 (propertyOffset): %1\n").arg(meta->propertyOffset()));

    for (int i = 0; i < meta->propertyCount(); ++i) {
        QMetaProperty prop = meta->property(i);
        QVariant val = prop.read(&paladin);

        printLine(QStringLiteral("  [%1] 属性名: %2 | 类型: %3 | 当前值: %4 | 可读: %5 | 可写: %6 | 具备NOTIFY: %7")
                     .arg(i, 2)
                     .arg(QString::fromLatin1(prop.name()).leftJustified(12, ' '))
                     .arg(QString::fromLatin1(prop.typeName()).leftJustified(10, ' '))
                     .arg(val.toString().leftJustified(12, ' '))
                     .arg(prop.isReadable() ? QStringLiteral("Y") : QStringLiteral("N"))
                     .arg(prop.isWritable() ? QStringLiteral("Y") : QStringLiteral("N"))
                     .arg(prop.hasNotifySignal() ? QStringLiteral("Y") : QStringLiteral("N")));
    }

    printLine(QStringLiteral("\n--- 通过 QObject::setProperty 反射修改属性 ---"));
    printLine(QStringLiteral("修改前 heroName: ") + paladin.property("heroName").toString());
    paladin.setProperty("heroName", QStringLiteral("白银之手大领主·乌瑟尔"));
    printLine(QStringLiteral("修改后 heroName: ") + paladin.property("heroName").toString());

    // ========================================================================
    // 3. 动态属性系统 (Dynamic Properties)
    // ========================================================================
    printSectionHeader(QStringLiteral("3. 动态属性系统 (setProperty / property 运行时动态附加)"));

    // 运行时无侵入式附加额外业务属性 (常用于 QSS 状态选择器、业务标签)
    paladin.setProperty("isVipPlayer", true);
    paladin.setProperty("guildName", QStringLiteral("白银骑士团"));
    paladin.setProperty("serverZone", 102);

    printLine(QStringLiteral("已动态附加属性列表 (dynamicPropertyNames):"));
    for (const QByteArray &name : paladin.dynamicPropertyNames()) {
        QVariant val = paladin.property(name.constData());
        printLine(QStringLiteral("  * 动态属性: %1 | 类型: %2 | 属性值: %3")
                     .arg(QString::fromLatin1(name).leftJustified(14, ' '))
                     .arg(QString::fromLatin1(val.typeName()).leftJustified(8, ' '))
                     .arg(val.toString()));
    }

    // ========================================================================
    // 4. 枚举元系统双向反射 (Q_ENUM / QMetaEnum)
    // ========================================================================
    printSectionHeader(QStringLiteral("4. 枚举元系统双向反射 (Q_ENUM / QMetaEnum)"));

    QMetaEnum roleEnum = QMetaEnum::fromType<GameHero::HeroRole>();
    printLine(QStringLiteral("枚举类型名称: %1 | 包含键值数量: %2").arg(QString::fromLatin1(roleEnum.name())).arg(roleEnum.keyCount()));

    printLine(QStringLiteral("遍历所有枚举映射关系:"));
    for (int i = 0; i < roleEnum.keyCount(); ++i) {
        printLine(QStringLiteral("  (%1) -> %2").arg(roleEnum.value(i)).arg(QString::fromLatin1(roleEnum.key(i))));
    }

    // 字符串 -> 枚举值
    const char *searchKey = "Mage";
    int enumVal = roleEnum.keyToValue(searchKey);
    printLine(QStringLiteral("\n[反射转换] 字符串 \"%1\" -> 枚举整型值: %2").arg(QString::fromLatin1(searchKey)).arg(enumVal));

    // 枚举值 -> 字符串
    GameHero::HeroRole testRole = GameHero::Priest;
    const char *keyName = roleEnum.valueToKey(testRole);
    printLine(QStringLiteral("[反射转换] 枚举常量 GameHero::Priest -> 字符串名: \"%1\"").arg(QString::fromLatin1(keyName)));

    // ========================================================================
    // 5. 方法、信号与槽元内省 (QMetaMethod)
    // ========================================================================
    printSectionHeader(QStringLiteral("5. 方法、信号与槽元内省 (QMetaMethod)"));

    printLine(QStringLiteral("方法总数: %1 (类自身起始偏移: %2)\n").arg(meta->methodCount()).arg(meta->methodOffset()));

    for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
        QMetaMethod method = meta->method(i);
        QString typeStr;
        switch (method.methodType()) {
        case QMetaMethod::Method:      typeStr = QStringLiteral("Method (Q_INVOKABLE)"); break;
        case QMetaMethod::Signal:      typeStr = QStringLiteral("Signal (信号)");        break;
        case QMetaMethod::Slot:        typeStr = QStringLiteral("Slot (槽函数)");         break;
        case QMetaMethod::Constructor: typeStr = QStringLiteral("Constructor");         break;
        }

        printLine(QStringLiteral("  [%1] 类型: %2 | 签名: %3 | 返回值: %4")
                     .arg(i, 2)
                     .arg(typeStr.leftJustified(22, ' '))
                     .arg(QString::fromLatin1(method.methodSignature()).leftJustified(32, ' '))
                     .arg(method.typeName() ? QString::fromLatin1(method.typeName()) : QStringLiteral("void")));
    }

    // ========================================================================
    // 6. 动态方法调用 (QMetaObject::invokeMethod)
    // ========================================================================
    printSectionHeader(QStringLiteral("6. 动态方法调用 (QMetaObject::invokeMethod 运行时字符串调度)"));

    // 场景 A：调用带返回值的 Q_INVOKABLE 成员函数
    int damageOutput = 0;
    bool success = QMetaObject::invokeMethod(&paladin, "castSkill",
                                            Qt::DirectConnection,
                                            Q_RETURN_ARG(int, damageOutput),
                                            Q_ARG(QString, QStringLiteral("神圣风暴 (Divine Storm)")),
                                            Q_ARG(int, 4));
    printLine(QStringLiteral("调用 castSkill 成功: %1 | 得到返回值 damageOutput: %2 点伤害")
                 .arg(success ? QStringLiteral("Y") : QStringLiteral("N")).arg(damageOutput));

    // 场景 B：调用无返回值的 Q_INVOKABLE 成员函数
    QMetaObject::invokeMethod(&paladin, "takeDamage",
                              Qt::DirectConnection,
                              Q_ARG(int, 350));

    // 场景 C：按字符串名称调用槽函数 (revive)
    QMetaObject::invokeMethod(&paladin, "revive",
                              Qt::DirectConnection,
                              Q_ARG(int, 1500));

    // 场景 D：调用获取档案方法
    QString profileText;
    QMetaObject::invokeMethod(&paladin, "getFullProfile",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(QString, profileText));
    printLine(QStringLiteral("调用 getFullProfile 返回:\n  -> ") + profileText);

    // ========================================================================
    // 7. 元对象系统信号通知监听
    // ========================================================================
    printSectionHeader(QStringLiteral("7. 属性与信号槽联动通知测试"));

    QObject::connect(&paladin, &GameHero::healthChanged, [](int newHp) {
        printLine(QStringLiteral("[信号触发] 监听到 healthChanged 信号，最新生命值: %1").arg(newHp));
    });

    QObject::connect(&paladin, &GameHero::heroDied, [](const QString &name) {
        printLine(QStringLiteral("[危险警告] 英雄【%1】英勇战死沙场！触发 heroDied 信号！").arg(name));
    });

    printLine(QStringLiteral("对英雄造成致命伤害 2000 点..."));
    paladin.takeDamage(2000);

    printLine(QStringLiteral("\n======================================================================"));
    printLine(QStringLiteral("  demo_metaobject 元对象系统全部核心特性演示完成！"));
    printLine(QStringLiteral("======================================================================\n"));

    return 0;
}
