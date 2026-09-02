#pragma once

#include <QString>
#include <QMetaType>

/**
 * @brief 自定义业务数据包结构体 (用于演示信号槽自定义类型传递与 qRegisterMetaType)
 * 
 * 必须满足 Qt 元类型系统要求：
 * 1. 具备默认构造函数 (Default Constructor)
 * 2. 具备拷贝构造函数 (Copy Constructor)
 * 3. 具备析构函数 (Destructor)
 */
struct PlayerPacket
{
    int playerId{1001};
    QString playerName{QStringLiteral("灰烬使者·乌瑟尔")};
    int level{85};
    double combatPower{99800.0};

    // 格式化输出
    QString toString() const {
        return QStringLiteral("PlayerPacket[ID: %1, 玩家: %2, 等级: Lv.%3, 战力: %4]")
            .arg(playerId).arg(playerName).arg(level).arg(combatPower);
    }
};

// 注册到 Qt 元类型声明系统 (使 QVariant 与 QueuedConnection 能够识别该类型)
Q_DECLARE_METATYPE(PlayerPacket)

/**
 * @brief 用于 void* 通用指针传递测试的底层上下文结构
 */
struct RawContextData
{
    int contextId{888};
    QString message{QStringLiteral("底层 C 风格通用上下文数据")};
    quintptr rawTimestamp{0};
};

Q_DECLARE_METATYPE(RawContextData*)
