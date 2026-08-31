#include "data_stream_demo.h"

#include <QCoreApplication>
#include <QDir>
#include <QMap>
#include <QPoint>
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
                    .arg(title.leftJustified(32, ' '))
                    .arg(content)
                    .toUtf8().constData()
              << std::endl;
}

static QString getSandboxDir()
{
    QString baseDir = QCoreApplication::applicationDirPath() + QStringLiteral("/sandbox_datastream_demo");
    QDir().mkpath(baseDir);
    return baseDir;
}

void DataStreamDemo::testBasicTypesAndEndianness()
{
    printTitle(QStringLiteral("[模块 1] 基础数据类型序列化与大小端字节序"));

    quint32 testValue = 0x12345678;

    // 1. 大端字节序 (BigEndian，Qt 默认网络字节序)
    QByteArray bigEndianBuf;
    {
        QDataStream out(&bigEndianBuf, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::BigEndian);
        out << testValue;
    }

    // 2. 小端字节序 (LittleEndian，x86/x64 原生字节序)
    QByteArray littleEndianBuf;
    {
        QDataStream out(&littleEndianBuf, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::LittleEndian);
        out << testValue;
    }

    printLine(QStringLiteral("原始 32 位整型"), QStringLiteral("0x12345678"));
    printLine(QStringLiteral("BigEndian 序列化字节流"), bigEndianBuf.toHex(' ').toUpper());
    printLine(QStringLiteral("LittleEndian 序列化字节流"), littleEndianBuf.toHex(' ').toUpper());

    // 3. 反序列化校验
    quint32 restoredValue = 0;
    QDataStream in(&bigEndianBuf, QIODevice::ReadOnly);
    in.setByteOrder(QDataStream::BigEndian);
    in >> restoredValue;
    printLine(QStringLiteral("反序列化读取还原结果"), QStringLiteral("0x%1 (一致: %2)")
        .arg(restoredValue, 8, 16, QChar('0')).toUpper()
        .arg(restoredValue == testValue ? QStringLiteral("True") : QStringLiteral("False")));
}

void DataStreamDemo::testQtContainersSerialization()
{
    printTitle(QStringLiteral("[模块 2] Qt 复杂复合容器与内置类型流式序列化"));

    QString filePath = getSandboxDir() + QStringLiteral("/containers.dat");

    // 1. 准备复杂数据结构
    QMap<QString, QPoint> landmarks;
    landmarks[QStringLiteral("Station_Alpha")] = QPoint(120, 350);
    landmarks[QStringLiteral("Station_Beta")] = QPoint(860, 420);
    landmarks[QStringLiteral("Central_Hub")] = QPoint(500, 500);

    QList<double> sensorReadings = {23.5, 24.1, 23.8, 25.0, 24.6};

    // 2. 序列化写入
    {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_5_15);
            out << landmarks << sensorReadings;
            file.close();
            printLine(QStringLiteral("容器数据持久化成功"), filePath);
        }
    }

    // 3. 逆向反序列化读取
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_5_15);

            QMap<QString, QPoint> restoredLandmarks;
            QList<double> restoredSensors;
            in >> restoredLandmarks >> restoredSensors;
            file.close();

            printLine(QStringLiteral("读取 QMap 地标数量"), QStringLiteral("%1 个").arg(restoredLandmarks.size()));
            for (auto it = restoredLandmarks.cbegin(); it != restoredLandmarks.cend(); ++it) {
                std::cout << QStringLiteral("    • %1 -> (%2, %3)")
                                .arg(it.key().leftJustified(15, ' '))
                                .arg(it.value().x())
                                .arg(it.value().y())
                                .toUtf8().constData()
                          << std::endl;
            }
            printLine(QStringLiteral("读取传感器采集点数量"), QStringLiteral("%1 个采样点").arg(restoredSensors.size()));
        }
    }
}

void DataStreamDemo::testCustomProjectSerialization()
{
    printTitle(QStringLiteral("[模块 3] 自定义复杂 C++ 对象工程存档序列化"));

    QString projPath = getSandboxDir() + QStringLiteral("/train_cockpit.cadproj");

    // 1. 创建完整 CAD 矢量工程并添加图元
    CadProject project(QStringLiteral("高铁驾驶舱仪表盘设计工程"), QStringLiteral("Alex_Senior_Architect"));
    project.addShape(GraphicShape(0, QStringLiteral("主仪表背景底框"), QRect(50, 50, 400, 300), QColor(30, 39, 46)));
    project.addShape(GraphicShape(1, QStringLiteral("速度刻度外环"), QRect(100, 100, 200, 200), QColor(52, 152, 219)));
    project.addShape(GraphicShape(2, QStringLiteral("实时速度文本标签"), QRect(150, 240, 100, 30), QColor(255, 255, 255)));

    // 2. 序列化持久化至二进制文件
    {
        QFile file(projPath);
        if (file.open(QIODevice::WriteOnly)) {
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_5_15);
            out << project;
            file.close();
            printLine(QStringLiteral("CAD 工程二进制存盘"), QStringLiteral("成功 -> %1 (%2 字节)").arg(projPath).arg(file.size()));
        }
    }

    // 3. 从二进制存档文件完整还原
    {
        QFile file(projPath);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_5_15);

            CadProject loadedProject;
            in >> loadedProject;
            file.close();

            printLine(QStringLiteral("还原工程名称"), loadedProject.projectName());
            printLine(QStringLiteral("还原设计作者"), loadedProject.author());
            printLine(QStringLiteral("还原创建时间"), loadedProject.createTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
            printLine(QStringLiteral("还原图元总数"), QStringLiteral("%1 个图元").arg(loadedProject.shapes().size()));

            std::cout << "  图元详情列表 :" << std::endl;
            for (const auto &shape : loadedProject.shapes()) {
                std::cout << QStringLiteral("    • [类型 %1] %2 (边界: x:%3, y:%4, w:%5, h:%6 | 颜色:%7)")
                                .arg(shape.shapeType)
                                .arg(shape.label.leftJustified(18, ' '))
                                .arg(shape.boundingBox.x())
                                .arg(shape.boundingBox.y())
                                .arg(shape.boundingBox.width())
                                .arg(shape.boundingBox.height())
                                .arg(shape.fillColor.name())
                                .toUtf8().constData()
                          << std::endl;
            }
        }
    }
}

void DataStreamDemo::testVersionControlAndCompatibility()
{
    printTitle(QStringLiteral("[模块 4] 魔数校验与数据格式版本兼容控制"));

    // 1. 模拟非法损坏/未知文件 (魔数不匹配)
    QByteArray corruptedData = QByteArray::fromHex("DEADBEEF0000000148656C6C6F");
    QDataStream in(corruptedData);
    in.setVersion(QDataStream::Qt_5_15);

    CadProject target;
    in >> target;

    printLine(QStringLiteral("非法魔数文件反序列化状态"), (in.status() == QDataStream::ReadCorruptData) ? QStringLiteral("检测到魔数错误 (ReadCorruptData)，安全拦截拒绝加载！") : QStringLiteral("正常"));
    printLine(QStringLiteral("数据版本控制核心思想"), QStringLiteral("头部写入固定魔数 (Magic) 校验文件格式合法性，紧跟格式版本号 (Version) 以实现历史存档兼容升级。"));
}

void DataStreamDemo::testMemoryPacketProtocol()
{
    printTitle(QStringLiteral("[模块 5] 内存二进制打包与私有网络协议封包"));

    // 模拟自定义私有工业控制网络协议包格式:
    // [起始标识 2B (0xAA55)][指令码 2B (0x0102)][包体长度 4B][JSON/序列化载荷]

    quint16 frameHeader = 0xAA55;
    quint16 commandId = 0x0102; // 如 "CMD_EMERGENCY_BRAKE"
    QString payloadJson = QStringLiteral("{\"targetId\":99,\"action\":\"STOP\",\"force\":100}");
    QByteArray payloadBytes = payloadJson.toUtf8();
    quint32 bodyLength = static_cast<quint32>(payloadBytes.size());

    // 1. 内存打包 (Packing)
    QByteArray packet;
    {
        QDataStream packStream(&packet, QIODevice::WriteOnly);
        packStream.setByteOrder(QDataStream::BigEndian); // 网络字节序
        packStream << frameHeader << commandId << bodyLength;
        packStream.writeRawData(payloadBytes.constData(), payloadBytes.size());
    }

    printLine(QStringLiteral("完整网络协议数据包大小"), QStringLiteral("%1 字节").arg(packet.size()));
    printLine(QStringLiteral("封包原始十六进制字节流"), packet.toHex(' ').toUpper());

    // 2. 模拟网络接收端解包 (Unpacking)
    {
        QDataStream unpackStream(packet);
        unpackStream.setByteOrder(QDataStream::BigEndian);

        quint16 rxHeader = 0;
        quint16 rxCmd = 0;
        quint32 rxLen = 0;

        unpackStream >> rxHeader >> rxCmd >> rxLen;

        QByteArray rxPayload(rxLen, '\0');
        unpackStream.readRawData(rxPayload.data(), rxLen);

        printLine(QStringLiteral("解包包头 (Header)"), QStringLiteral("0x%1").arg(rxHeader, 4, 16, QChar('0')).toUpper());
        printLine(QStringLiteral("解包指令码 (Command)"), QStringLiteral("0x%1").arg(rxCmd, 4, 16, QChar('0')).toUpper());
        printLine(QStringLiteral("解包数据体长度 (Length)"), QStringLiteral("%1 字节").arg(rxLen));
        printLine(QStringLiteral("解包载荷数据 (Payload)"), QString::fromUtf8(rxPayload));
    }
}
