#include "xml_stream_demo.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
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
                    .arg(title.leftJustified(34, ' '))
                    .arg(content)
                    .toUtf8().constData()
              << std::endl;
}

static QString getSandboxDir()
{
    QString baseDir = QCoreApplication::applicationDirPath() + QStringLiteral("/sandbox_xml_demo");
    QDir().mkpath(baseDir);
    return baseDir;
}

void XmlStreamDemo::testStreamWriting()
{
    printTitle(QStringLiteral("[模块 1] 基于流的高性能 XML 游戏数据导出构建 (QXmlStreamWriter)"));

    QString filePath = getSandboxDir() + QStringLiteral("/game_database.xml");
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        printLine(QStringLiteral("创建文件失败"), filePath);
        return;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(4);

    // 1. 写入 XML 声明与注释
    writer.writeStartDocument(QStringLiteral("1.0"));
    writer.writeComment(QStringLiteral("MMORPG 游戏世界数据库 (任务与神器图鉴)"));

    // 2. 根节点 <GameDatabase>
    writer.writeStartElement(QStringLiteral("GameDatabase"));
    writer.writeAttribute(QStringLiteral("version"), QStringLiteral("2.5.0"));
    writer.writeAttribute(QStringLiteral("gameRealm"), QStringLiteral("艾泽拉斯"));

    // 3. 写入任务列表 <Quests>
    writer.writeStartElement(QStringLiteral("Quests"));
    {
        writer.writeStartElement(QStringLiteral("Quest"));
        writer.writeAttribute(QStringLiteral("id"), QStringLiteral("101"));
        writer.writeAttribute(QStringLiteral("title"), QStringLiteral("击败熔岩巨龙·灭世者"));
        writer.writeAttribute(QStringLiteral("minLevel"), QStringLiteral("60"));
        writer.writeAttribute(QStringLiteral("rewardGold"), QStringLiteral("5000"));
        writer.writeAttribute(QStringLiteral("rewardExp"), QStringLiteral("120000"));
        writer.writeAttribute(QStringLiteral("targetNpc"), QStringLiteral("守护者·卡德加"));
        writer.writeEndElement(); // </Quest>

        writer.writeStartElement(QStringLiteral("Quest"));
        writer.writeAttribute(QStringLiteral("id"), QStringLiteral("102"));
        writer.writeAttribute(QStringLiteral("title"), QStringLiteral("寻找失落的远古圣杯"));
        writer.writeAttribute(QStringLiteral("minLevel"), QStringLiteral("45"));
        writer.writeAttribute(QStringLiteral("rewardGold"), QStringLiteral("3200"));
        writer.writeAttribute(QStringLiteral("rewardExp"), QStringLiteral("85000"));
        writer.writeAttribute(QStringLiteral("targetNpc"), QStringLiteral("大主教·本尼迪塔斯"));
        writer.writeEndElement(); // </Quest>
    }
    writer.writeEndElement(); // </Quests>

    // 4. 写入神装图鉴列表 <Weapons>
    writer.writeStartElement(QStringLiteral("Weapons"));
    {
        writer.writeStartElement(QStringLiteral("Weapon"));
        writer.writeAttribute(QStringLiteral("id"), QStringLiteral("2001"));
        writer.writeAttribute(QStringLiteral("rarity"), QStringLiteral("SSR"));
        writer.writeAttribute(QStringLiteral("element"), QStringLiteral("Holy"));
        writer.writeTextElement(QStringLiteral("Name"), QStringLiteral("灰烬使者 (Ashbringer)"));
        writer.writeTextElement(QStringLiteral("Damage"), QStringLiteral("4250"));
        writer.writeTextElement(QStringLiteral("Description"), QStringLiteral("大领主莫格莱尼的传奇神圣巨剑"));
        writer.writeEndElement(); // </Weapon>

        writer.writeStartElement(QStringLiteral("Weapon"));
        writer.writeAttribute(QStringLiteral("id"), QStringLiteral("2002"));
        writer.writeAttribute(QStringLiteral("rarity"), QStringLiteral("SSR"));
        writer.writeAttribute(QStringLiteral("element"), QStringLiteral("Frost"));
        writer.writeTextElement(QStringLiteral("Name"), QStringLiteral("霜之哀伤 (Frostmourne)"));
        writer.writeTextElement(QStringLiteral("Damage"), QStringLiteral("4500"));
        writer.writeTextElement(QStringLiteral("Description"), QStringLiteral("蕴含巫妖王统御之力的符文魔剑"));
        writer.writeEndElement(); // </Weapon>
    }
    writer.writeEndElement(); // </Weapons>

    writer.writeEndElement(); // </GameDatabase>
    writer.writeEndDocument();

    file.close();

    printLine(QStringLiteral("XML 数据库流式导出状态"), QStringLiteral("成功生成 -> %1 (%2 字节)").arg(filePath).arg(file.size()));
}

void XmlStreamDemo::testStreamReading()
{
    printTitle(QStringLiteral("[模块 2] 基于流的事件驱动增量游戏数据解析 (QXmlStreamReader)"));

    QString filePath = getSandboxDir() + QStringLiteral("/game_database.xml");
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        printLine(QStringLiteral("打开 XML 文件失败"), filePath);
        return;
    }

    QXmlStreamReader reader(&file);

    QList<GameQuest> quests;
    QList<GameWeapon> weapons;
    QString gameVersion;
    QString gameRealm;

    while (!reader.atEnd() && !reader.hasError()) {
        QXmlStreamReader::TokenType token = reader.readNext();

        if (token == QXmlStreamReader::StartElement) {
            QStringRef elementName = reader.name();

            if (elementName == QLatin1String("GameDatabase")) {
                gameVersion = reader.attributes().value(QStringLiteral("version")).toString();
                gameRealm = reader.attributes().value(QStringLiteral("gameRealm")).toString();
            } else if (elementName == QLatin1String("Quest")) {
                GameQuest q;
                auto attrs = reader.attributes();
                q.id = attrs.value(QStringLiteral("id")).toInt();
                q.title = attrs.value(QStringLiteral("title")).toString();
                q.minLevel = attrs.value(QStringLiteral("minLevel")).toInt();
                q.rewardGold = attrs.value(QStringLiteral("rewardGold")).toInt();
                q.rewardExp = attrs.value(QStringLiteral("rewardExp")).toInt();
                q.targetNpc = attrs.value(QStringLiteral("targetNpc")).toString();
                quests.append(q);
            } else if (elementName == QLatin1String("Weapon")) {
                GameWeapon w;
                auto attrs = reader.attributes();
                w.id = attrs.value(QStringLiteral("id")).toInt();
                w.rarity = attrs.value(QStringLiteral("rarity")).toString();
                w.element = attrs.value(QStringLiteral("element")).toString();

                // 增量读取 Weapon 子节点
                while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == QLatin1String("Weapon"))) {
                    reader.readNext();
                    if (reader.isStartElement()) {
                        if (reader.name() == QLatin1String("Name")) {
                            w.name = reader.readElementText();
                        } else if (reader.name() == QLatin1String("Damage")) {
                            w.attackDamage = reader.readElementText().toInt();
                        } else if (reader.name() == QLatin1String("Description")) {
                            w.description = reader.readElementText();
                        }
                    }
                }
                weapons.append(w);
            }
        }
    }

    file.close();

    printLine(QStringLiteral("读取游戏数据库版本"), gameVersion);
    printLine(QStringLiteral("读取游戏大区"), gameRealm);
    printLine(QStringLiteral("解析任务总数"), QStringLiteral("%1 个主支线任务").arg(quests.size()));
    for (const auto &q : quests) {
        std::cout << QStringLiteral("    • [任务 #%1] %2 (等级要求: Lv.%3 | 奖励: %4 金币, %5 经验 | NPC: %6)")
                        .arg(q.id)
                        .arg(q.title.leftJustified(22, ' '))
                        .arg(q.minLevel)
                        .arg(q.rewardGold)
                        .arg(q.rewardExp)
                        .arg(q.targetNpc)
                        .toUtf8().constData()
                  << std::endl;
    }

    printLine(QStringLiteral("解析神装图鉴总数"), QStringLiteral("%1 件神器").arg(weapons.size()));
    for (const auto &w : weapons) {
        std::cout << QStringLiteral("    • [%1][%2属性] %3 (攻击力: +%4 | 描述: %5)")
                        .arg(w.rarity)
                        .arg(w.element)
                        .arg(w.name.leftJustified(24, ' '))
                        .arg(w.attackDamage)
                        .arg(w.description)
                        .toUtf8().constData()
                  << std::endl;
    }
}

void XmlStreamDemo::testErrorHandling()
{
    printTitle(QStringLiteral("[模块 3] XML 语法错误与非法节点精准定位"));

    // 故意构造未闭合且格式畸形的 XML 报文
    QString corruptedXml = QString::fromUtf8(R"raw_xml(<?xml version="1.0" encoding="UTF-8"?>
<GameDatabase>
    <Quest id="101" title="熔岩巨龙">
        <RewardGold>5000</BrokenTag>
    </Quest>
</GameDatabase>
)raw_xml");

    QXmlStreamReader reader(corruptedXml);

    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
    }

    if (reader.hasError()) {
        printLine(QStringLiteral("语法错误检测状态"), QStringLiteral("成功拦截畸形 XML 数据！"));
        printLine(QStringLiteral("错误类型描述"), reader.errorString());
        printLine(QStringLiteral("错误所在行号 (lineNumber)"), QStringLiteral("第 %1 行").arg(reader.lineNumber()));
        printLine(QStringLiteral("错误所在列号 (columnNumber)"), QStringLiteral("第 %1 列").arg(reader.columnNumber()));
    } else {
        printLine(QStringLiteral("语法错误检测状态"), QStringLiteral("未检测到错误"));
    }
}

void XmlStreamDemo::testNpcDialogueTreeStreaming()
{
    printTitle(QStringLiteral("[模块 4] 游戏 NPC 多分支剧情对话树增量流式解析"));

    QString dialogueXml = QString::fromUtf8(R"raw_xml(<?xml version="1.0" encoding="UTF-8"?>
<DialogueTree npcName="迪卡·凯恩 (Deckard Cain)" location="崔斯特姆大教堂">
    <Node id="1" speaker="凯恩">
        <Text>你好，年轻的赫拉迪姆传承者，请留步听我说一段关于庇护之地的远古传说。</Text>
        <Choices>
            <Option targetNode="2" text="1. 告诉我关于暗黑破坏神复活的真相"/>
            <Option targetNode="3" text="2. 我需要你帮我鉴定赫拉迪姆方块"/>
            <Option targetNode="99" text="3. [告辞离开]"/>
        </Choices>
    </Node>
</DialogueTree>
)raw_xml");

    QXmlStreamReader reader(dialogueXml);

    QString npcName;
    QString location;
    QString dialogueText;
    QList<QString> choiceList;

    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();

        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("DialogueTree")) {
                npcName = reader.attributes().value(QStringLiteral("npcName")).toString();
                location = reader.attributes().value(QStringLiteral("location")).toString();
            } else if (reader.name() == QLatin1String("Text")) {
                dialogueText = reader.readElementText();
            } else if (reader.name() == QLatin1String("Option")) {
                choiceList.append(reader.attributes().value(QStringLiteral("text")).toString());
            }
        }
    }

    printLine(QStringLiteral("NPC 角色名称"), npcName);
    printLine(QStringLiteral("所处场景位置"), location);
    printLine(QStringLiteral("NPC 当前主台词"), dialogueText);
    printLine(QStringLiteral("玩家可选剧情分支"), QStringLiteral("%1 个分支选项").arg(choiceList.size()));
    for (const auto &opt : choiceList) {
        std::cout << QStringLiteral("    ➜ %1").arg(opt).toUtf8().constData() << std::endl;
    }
}
