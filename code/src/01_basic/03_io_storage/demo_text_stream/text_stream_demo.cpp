#include "text_stream_demo.h"

#include <QCoreApplication>
#include <QTextCodec>
#include <QDir>
#include <QDateTime>
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

static QString getSandboxDir()
{
    QString baseDir = QCoreApplication::applicationDirPath() + QStringLiteral("/sandbox_text_stream_demo");
    QDir().mkpath(baseDir);
    return baseDir;
}

void TextStreamDemo::testFormatManipulators()
{
    printTitle(QStringLiteral("[模块 1] QTextStream 格式化流操纵符 (Manipulators)"));

    QString filePath = getSandboxDir() + QStringLiteral("/system_report.txt");
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");

        // 输出格式化报表表头
        out << QStringLiteral("====================================================================\n");
        out << QStringLiteral("                 工业物联网网关节点巡检监控报表                     \n");
        out << QStringLiteral("====================================================================\n");
        
        // 使用 qSetFieldWidth 与 left/right 格式化表格列宽
        out << left << qSetFieldWidth(12) << QStringLiteral("节点ID")
            << left << qSetFieldWidth(20) << QStringLiteral("设备名称")
            << right << qSetFieldWidth(14) << QStringLiteral("IP地址(Hex)")
            << right << qSetFieldWidth(12) << QStringLiteral("CPU温度(°C)")
            << right << qSetFieldWidth(10) << QStringLiteral("状态")
            << qSetFieldWidth(0) << "\n";

        out << QStringLiteral("--------------------------------------------------------------------\n");

        // 模拟输出 3 条数据行
        out << left << qSetFieldWidth(12) << 1001
            << left << qSetFieldWidth(20) << "Main_Gateway_01"
            << right << hex << showbase << qSetFieldWidth(14) << 0xC0A8010A // 192.168.1.10
            << dec << noshowbase << forcesign << forcepoint << right << qSetFieldWidth(12) << 45.65
            << noforcesign << right << qSetFieldWidth(10) << "NORMAL"
            << qSetFieldWidth(0) << "\n";

        out << left << qSetFieldWidth(12) << 1002
            << left << qSetFieldWidth(20) << "Sensor_Hub_North"
            << right << hex << showbase << qSetFieldWidth(14) << 0xC0A80114 // 192.168.1.20
            << dec << noshowbase << forcesign << forcepoint << right << qSetFieldWidth(12) << 78.20
            << noforcesign << right << qSetFieldWidth(10) << "WARN"
            << qSetFieldWidth(0) << "\n";

        out.flush();
        file.close();
        printLine(QStringLiteral("报表生成成功"), filePath);
    }

    // 读回并在控制台打印
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        std::cout << in.readAll().toUtf8().constData() << std::endl;
        file.close();
    }
}

void TextStreamDemo::testEncodingAndBOM()
{
    printTitle(QStringLiteral("[模块 2] 多字符集编码处理 (UTF-8 带 BOM 与 GBK/GB18030)"));

    QString utf8BomPath = getSandboxDir() + QStringLiteral("/report_utf8_bom.txt");
    QString gbkPath = getSandboxDir() + QStringLiteral("/report_gbk.txt");

    // 1. 写入带 UTF-8 BOM 头的文本 (常用于 Windows Excel 或旧版软件识别)
    {
        QFile file(utf8BomPath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out.setGenerateByteOrderMark(true); // 自动在文件起始写入 0xEF, 0xBB, 0xBF
            out << QStringLiteral("这是带 UTF-8 BOM 头的中文数据测试内容。\n");
            file.close();
            printLine(QStringLiteral("写入 UTF-8 带 BOM 文件"), utf8BomPath);
        }
    }

    // 2. 写入 GBK / GB18030 编码文本 (兼容旧工业系统和 Windows CMD)
    QTextCodec *gbkCodec = QTextCodec::codecForName("GB18030");
    if (gbkCodec) {
        QFile file(gbkPath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec(gbkCodec);
            out << QStringLiteral("这是 GB18030 / GBK 编码的中文字符串。\n");
            file.close();
            printLine(QStringLiteral("写入 GB18030 编码文件"), gbkPath);
        }

        // 用 GBK 正确逆向解码读取
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec(gbkCodec);
            QString readContent = in.readLine();
            file.close();
            printLine(QStringLiteral("GB18030 解码读取内容"), readContent);
        }
    }
}

void TextStreamDemo::testLineEndings()
{
    printTitle(QStringLiteral("[模块 3] 跨平台换行符自动适配 (CRLF vs LF)"));

    QString textModePath = getSandboxDir() + QStringLiteral("/text_mode.txt");
    QString binaryModePath = getSandboxDir() + QStringLiteral("/binary_mode.txt");

    // 1. 开启 QIODevice::Text 模式写入
    {
        QFile file(textModePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "Line 1\n";
            out << "Line 2\n";
            file.close();
        }
    }

    // 2. 未开启 QIODevice::Text 模式 (纯二进制写入)
    {
        QFile file(binaryModePath);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream out(&file);
            out << "Line 1\n";
            out << "Line 2\n";
            file.close();
        }
    }

    printLine(QStringLiteral("Text 模式文件大小 (Windows下自动转为 CRLF)"), QStringLiteral("%1 字节").arg(QFile(textModePath).size()));
    printLine(QStringLiteral("Binary 模式文件大小 (保持原始 LF)"), QStringLiteral("%1 字节").arg(QFile(binaryModePath).size()));
    printLine(QStringLiteral("换行符适配准则"), QStringLiteral("跨平台文本读写必须指定 QIODevice::Text 标记，QTextStream 会自动在各平台透明处理 \\r\\n 与 \\n。"));
}

void TextStreamDemo::testCsvReportProcessing()
{
    printTitle(QStringLiteral("[模块 4] 实战：CSV 报表文件结构化生成与逐行解析"));

    QString csvPath = getSandboxDir() + QStringLiteral("/production_metrics.csv");

    // 1. 结构化导出 CSV 文件
    {
        QFile file(csvPath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");

            // 写入 CSV 表头
            out << "MetricId,DeviceName,YieldRate,CycleTimeMs,Inspector\n";
            // 写入数据行 (包含逗号特殊转义)
            out << "101,SMT_Mount_01,99.45,1250,\"Zhang, San\"\n";
            out << "102,AOI_Optical_02,98.80,850,\"Li, Si\"\n";
            out << "103,Laser_Welder_03,99.95,2100,\"Wang, Wu\"\n";

            file.close();
            printLine(QStringLiteral("CSV 报表生成成功"), csvPath);
        }
    }

    // 2. 逐行读取并结构化解析 CSV
    {
        QFile file(csvPath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("UTF-8");

            QString header = in.readLine();
            printLine(QStringLiteral("读取 CSV 表头"), header);

            std::cout << "  逐行解析数据记录 :" << std::endl;
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.trimmed().isEmpty()) continue;

                // 简易 CSV 拆分 (演示提取)
                QStringList tokens = line.split(',');
                if (tokens.size() >= 4) {
                    std::cout << QStringLiteral("    [ID:%1] 设备:%2 | 良率:%3% | 节拍:%4ms")
                                    .arg(tokens[0].leftJustified(4, ' '))
                                    .arg(tokens[1].leftJustified(18, ' '))
                                    .arg(tokens[2].leftJustified(6, ' '))
                                    .arg(tokens[3])
                                    .toUtf8().constData()
                              << std::endl;
                }
            }
            file.close();
        }
    }
}

void TextStreamDemo::testMemoryStringStream()
{
    printTitle(QStringLiteral("[模块 5] 内存字符串流 (QString 内存流缓冲)"));

    QString memoryBuffer;
    QTextStream memStream(&memoryBuffer);

    // 1. 内存中高效格式化拼接复杂文本
    memStream << "Transaction[" << hex << showbase << 0xABCDEF << dec << noshowbase
              << "] Amount=" << 12850.50
              << " Currency=USD"
              << " Timestamp=" << QDateTime::currentSecsSinceEpoch();

    printLine(QStringLiteral("内存流格式化拼接结果"), memoryBuffer);

    // 2. 逆向从格式化文本中提取数值
    QString inputData = QStringLiteral("400 800 1200.50");
    QTextStream reader(&inputData);
    int width, height;
    double ratio;
    reader >> width >> height >> ratio;

    printLine(QStringLiteral("逆向流提取结果"), QStringLiteral("Width=%1, Height=%2, Ratio=%3").arg(width).arg(height).arg(ratio));
}
