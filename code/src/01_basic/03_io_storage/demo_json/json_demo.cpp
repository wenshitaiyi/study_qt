#include "json_demo.h"
#include "json_utils.h"

#include <QVariantMap>
#include <QVariantList>
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

void JsonDemo::testBasicConstructionAndFormatting()
{
    printTitle(QStringLiteral("[模块 1] 游戏英雄档案构建与格式化输出 (Indented vs Compact)"));

    // 1. 构建 RPG 游戏英雄角色 JSON 配置
    QJsonObject heroObj;
    heroObj[QStringLiteral("heroName")] = QStringLiteral("亚瑟王·潘德拉贡");
    heroObj[QStringLiteral("classType")] = QStringLiteral("圣骑士 (Paladin)");
    heroObj[QStringLiteral("level")] = 85;
    heroObj[QStringLiteral("isAwakened")] = true;

    // 嵌套基础战斗属性
    QJsonObject combatStats;
    combatStats[QStringLiteral("maxHp")] = 18500;
    combatStats[QStringLiteral("attackPower")] = 3450.5;
    combatStats[QStringLiteral("defense")] = 1920;
    combatStats[QStringLiteral("critRate")] = 0.35;
    heroObj[QStringLiteral("combatStats")] = combatStats;

    // 嵌套已穿戴神装列表
    QJsonArray equippedItems;
    equippedItems.append(QStringLiteral("誓约胜利之剑 (Excalibur)"));
    equippedItems.append(QStringLiteral("永恒王权守护战甲 (SSR)"));
    equippedItems.append(QStringLiteral("炽热龙晶祝福指环 (SSR)"));
    heroObj[QStringLiteral("equippedItems")] = equippedItems;

    // 2. 序列化为 QJsonDocument
    QJsonDocument doc(heroObj);

    printLine(QStringLiteral("Compact 紧凑格式大小 (网络下发)"), QStringLiteral("%1 字节").arg(doc.toJson(QJsonDocument::Compact).size()));
    printLine(QStringLiteral("Indented 缩进格式大小 (本地存档)"), QStringLiteral("%1 字节").arg(doc.toJson(QJsonDocument::Indented).size()));

    std::cout << "\n  --- Indented 格式化游戏英雄数据展示 --- :\n"
              << doc.toJson(QJsonDocument::Indented).constData() << std::endl;
}

void JsonDemo::testParsingAndErrorHandling()
{
    printTitle(QStringLiteral("[模块 2] 游戏服务器登录协议解析与语法错误捕获"));

    // 1. 合法游戏网关响应报文解析
    QString validJson = QString::fromUtf8(R"raw_json({
        "cmd": "AUTH_LOGIN_RESP",
        "code": 200,
        "payload": {
            "playerId": 9527,
            "sessionToken": "JWT_TOKEN_GAME_9527_XYZ",
            "serverRealm": "艾泽拉斯-一区 (霜之哀伤)"
        }
    })raw_json");
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(validJson.toUtf8(), &error);

    QString parseStatus = (error.error == QJsonParseError::NoError) ? QStringLiteral("解析成功 (NoError)") : error.errorString();
    printLine(QStringLiteral("登录响应报文解析状态"), parseStatus);
    printLine(QStringLiteral("提取指令名 (cmd)"), doc.object().value(QStringLiteral("cmd")).toString());
    printLine(QStringLiteral("提取玩家 ID (playerId)"), QString::number(doc.object().value(QStringLiteral("payload")).toObject().value(QStringLiteral("playerId")).toInt()));
    printLine(QStringLiteral("所属大区 (serverRealm)"), doc.object().value(QStringLiteral("payload")).toObject().value(QStringLiteral("serverRealm")).toString());

    // 2. 非法报文语法错误捕获 (模拟网络传输残缺丢包)
    QString brokenJson = QStringLiteral("{\"cmd\": \"ATTACK_TARGET\", \"damage\": 500, ");
    QJsonParseError badError;
    QJsonDocument badDoc = QJsonDocument::fromJson(brokenJson.toUtf8(), &badError);

    QString badStatus = (badError.error != QJsonParseError::NoError) 
        ? QStringLiteral("成功捕获异常 -> %1 (错误偏移量: %2)").arg(badError.errorString()).arg(badError.offset)
        : QStringLiteral("未报错");
    printLine(QStringLiteral("残缺报文语法错误捕获"), badStatus);
}

void JsonDemo::testVariantConversion()
{
    printTitle(QStringLiteral("[模块 3] 游戏战斗结算数据 (QVariant 与 JSON 互转)"));

    // 1. QVariantMap 构建战斗副本结算战报
    QVariantMap battleResult;
    battleResult[QStringLiteral("dungeonId")] = QStringLiteral("DUNGEON_DRAGON_LAIR_01");
    battleResult[QStringLiteral("killBossTimeSec")] = 185;
    battleResult[QStringLiteral("expGained")] = 50000;
    battleResult[QStringLiteral("goldReward")] = 8888;
    battleResult[QStringLiteral("isFirstClear")] = true;

    QJsonObject jsonObj = QJsonObject::fromVariantMap(battleResult);
    printLine(QStringLiteral("战报 QVariantMap 转为 QJsonObject"), QString::fromUtf8(QJsonDocument(jsonObj).toJson(QJsonDocument::Compact)));

    // 2. QJsonObject 还原回 QVariantMap
    QVariantMap restoredMap = jsonObj.toVariantMap();
    printLine(QStringLiteral("还原回 QVariantMap 字段数"), QStringLiteral("%1 个结算条目").arg(restoredMap.size()));
    printLine(QStringLiteral("获取结算经验值 (expGained)"), QString::number(restoredMap.value(QStringLiteral("expGained")).toInt()));
}

void JsonDemo::testJsonDeepMerge()
{
    printTitle(QStringLiteral("[模块 4] 高级工具实战：英雄平衡性热更补丁深度合并 (Deep Merge)"));

    // 英雄基础属性配置模板 (Base)
    QString baseHeroJson = QString::fromUtf8(R"raw_json({
        "heroId": 1001,
        "heroName": "恶魔猎手·伊利丹",
        "skills": {
            "fel_rush": {
                "name": "邪能冲撞",
                "damage": 800,
                "cooldown": 10
            },
            "metamorphosis": {
                "name": "恶魔变形",
                "attackBuff": 1.50,
                "duration": 30
            }
        },
        "baseSpeed": 350
    })raw_json");

    // 平衡性热更新差异补丁 (Patch: 增强邪能冲撞伤害，恶魔变形附加吸血词条，新增专属被动)
    QString patchJson = QString::fromUtf8(R"raw_json({
        "skills": {
            "fel_rush": {
                "damage": 1200,
                "cooldown": 8
            },
            "metamorphosis": {
                "lifeSteal": 0.25
            },
            "chaos_strike": {
                "name": "混乱打击 [补丁新增]",
                "damage": 950
            }
        },
        "baseSpeed": 365
    })raw_json");

    QJsonObject baseObj = QJsonDocument::fromJson(baseHeroJson.toUtf8()).object();
    QJsonObject patchObj = QJsonDocument::fromJson(patchJson.toUtf8()).object();

    // 执行深度合并
    QJsonObject mergedObj = JsonUtils::deepMerge(baseObj, patchObj);

    std::cout << "  --- 深度递归合并后的最终英雄平衡性配置 --- :\n"
              << QJsonDocument(mergedObj).toJson(QJsonDocument::Indented).constData() << std::endl;

    printLine(QStringLiteral("移速 baseSpeed (被补丁增强)"), QString::number(mergedObj.value(QStringLiteral("baseSpeed")).toInt()));
    printLine(QStringLiteral("邪能冲撞伤害 (800 -> 1200)"), QString::number(mergedObj.value(QStringLiteral("skills")).toObject().value(QStringLiteral("fel_rush")).toObject().value(QStringLiteral("damage")).toInt()));
    printLine(QStringLiteral("恶魔变形吸血 (补丁新增词条)"), QStringLiteral("%1%").arg(mergedObj.value(QStringLiteral("skills")).toObject().value(QStringLiteral("metamorphosis")).toObject().value(QStringLiteral("lifeSteal")).toDouble() * 100.0, 0, 'f', 1));
    printLine(QStringLiteral("混乱打击 (补丁新增技能)"), mergedObj.value(QStringLiteral("skills")).toObject().value(QStringLiteral("chaos_strike")).toObject().value(QStringLiteral("name")).toString());
}

void JsonDemo::testPathBasedQueryAndUpdate()
{
    printTitle(QStringLiteral("[模块 5] 高级工具实战：游戏跨服战力路径式检索与动态更新"));

    // 构建一个大型跨服军团联赛 JSON 数据树
    QString realmDataStr = QString::fromUtf8(R"raw_json({
        "gameWorld": {
            "crossRealmLeague": {
                "season": "S9",
                "topGuilds": [
                    {
                        "guildName": "星辰之翼",
                        "leader": {
                            "name": "光辉领主",
                            "combatPower": 985000
                        }
                    },
                    {
                        "guildName": "诸神黄昏",
                        "leader": {
                            "name": "暗夜裁决者",
                            "combatPower": 962000
                        }
                    }
                ]
            }
        }
    })raw_json");

    QJsonObject root = QJsonDocument::fromJson(realmDataStr.toUtf8()).object();

    // 1. 点分路径与数组下标安全读取 (getByPath)
    QJsonValue season = JsonUtils::getByPath(root, QStringLiteral("gameWorld.crossRealmLeague.season"));
    QJsonValue guild0Leader = JsonUtils::getByPath(root, QStringLiteral("gameWorld.crossRealmLeague.topGuilds[0].leader.name"));
    QJsonValue guild0Power = JsonUtils::getByPath(root, QStringLiteral("gameWorld.crossRealmLeague.topGuilds[0].leader.combatPower"));
    QJsonValue notFoundVal = JsonUtils::getByPath(root, QStringLiteral("gameWorld.crossRealmLeague.topGuilds[0].non_exist.stat"));

    printLine(QStringLiteral("路径: crossRealmLeague.season"), season.toString());
    printLine(QStringLiteral("路径: topGuilds[0].leader.name"), guild0Leader.toString());
    printLine(QStringLiteral("路径: topGuilds[0]...combatPower"), QString::number(guild0Power.toInt()));
    
    QString notFoundStr = notFoundVal.isUndefined() ? QStringLiteral("True (安全防御返回 Undefined，杜绝崩溃)") : QStringLiteral("False");
    printLine(QStringLiteral("路径: 不存在字段 isUndefined()"), notFoundStr);

    // 2. 路径式动态写入与自动创建中间层级 (setByPath)
    JsonUtils::setByPath(root, QStringLiteral("gameWorld.crossRealmLeague.seasonReward.championTitle"), QStringLiteral("天下无双·九五之尊"));
    JsonUtils::setByPath(root, QStringLiteral("gameWorld.crossRealmLeague.seasonReward.diamondPrize"), 100000);

    QJsonValue rewardTitle = JsonUtils::getByPath(root, QStringLiteral("gameWorld.crossRealmLeague.seasonReward.championTitle"));
    QJsonValue diamondPrize = JsonUtils::getByPath(root, QStringLiteral("gameWorld.crossRealmLeague.seasonReward.diamondPrize"));

    printLine(QStringLiteral("自动建层写入: 冠军专属称号"), rewardTitle.toString());
    printLine(QStringLiteral("自动建层写入: 冠军钻石奖励"), QStringLiteral("%1 钻").arg(diamondPrize.toInt()));
}
