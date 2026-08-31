#include "string_demo.h"

#include <QTextCodec>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <iostream>

static void printLine(const QString &title, const QString &content)
{
    std::cout << QStringLiteral("  • %1 : %2")
                    .arg(title.leftJustified(26, ' '))
                    .arg(content)
                    .toUtf8().constData()
              << std::endl;
}

void StringDemo::testQStringBasics()
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [模块 1] QString 高效操作、格式化与切片" << std::endl;
    std::cout << "============================================================" << std::endl;

    // 1. 动态 arg 格式化 (进制、补位、对齐)
    int code = 42;
    int hexVal = 0x2A;
    double price = 129.8567;
    QString formatted = QStringLiteral("ID: %1 | 16进制: 0x%2 | 价格: ¥%3")
        .arg(code, 6, 10, QChar('0'))                      // 10进制，总宽6位，不足补0 -> 000042
        .arg(hexVal, 4, 16, QChar('0')).toUpper()          // 16进制大写，总宽4位 -> 002A
        .arg(price, 0, 'f', 2);                            // 浮点数保留2位小数 -> 129.86

    printLine(QStringLiteral("格式化输出"), formatted);

    // 2. 字符串切片截取 (section, left, right, mid, chopped)
    QString url = QStringLiteral("https://api.github.com/v1/repos/study/study_qt/commits");
    QString host = url.section(QStringLiteral("/"), 2, 2);     // 截取第2段 -> api.github.com
    QString repo = url.section(QStringLiteral("/"), 4, 5);     // 截取第4到5段 -> repos/study
    QString sub = url.mid(8, 14);                              // mid(起始, 长度) -> api.github.com
    QString chopped = url.chopped(8);                          // 去除末尾8个字符

    printLine(QStringLiteral("原始 URL"), url);
    printLine(QStringLiteral("截取 Host (section)"), host);
    printLine(QStringLiteral("截取 Repo (section)"), repo);
    printLine(QStringLiteral("截取 Mid (8, 14)"), sub);
    printLine(QStringLiteral("截取 Chop (末尾8)"), chopped);

    // 3. 空白字符整理 (trimmed, simplified)
    QString rawText = QStringLiteral("  \t\n  Hello \t\t Qt \n 5.15.2   \r\n  ");
    printLine(QStringLiteral("原始带多余空白"), rawText);
    printLine(QStringLiteral("trimmed (首尾)"), rawText.trimmed());
    printLine(QStringLiteral("simplified (合并)"), rawText.simplified());

    // 4. 数值与进制安全转换
    QString hexStr = QStringLiteral("FF0A");
    bool ok = false;
    int decimal = hexStr.toInt(&ok, 16);
    printLine(QStringLiteral("HEX 转 10进制"), QStringLiteral("%1 -> %2 (状态: %3)").arg(hexStr).arg(decimal).arg(ok ? QStringLiteral("成功") : QStringLiteral("失败")));
}

void StringDemo::testQStringRef()
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [模块 2] QStringRef 经典只读子串引用 (Qt 5 专属优化)" << std::endl;
    std::cout << "============================================================" << std::endl;

    QString httpHeader = QStringLiteral("HTTP/1.1 200 OK\r\nContent-Length: 4096\r\nServer: Nginx/1.24");
    printLine(QStringLiteral("原始 HTTP 报文"), httpHeader);

    // 1. 使用 leftRef / midRef / rightRef 进行零堆分配引用
    QStringRef protoRef = httpHeader.leftRef(8);               // "HTTP/1.1"
    QStringRef statusCodeRef = httpHeader.midRef(9, 3);        // "200"

    printLine(QStringLiteral("协议版本 (leftRef)"), protoRef.toString());
    printLine(QStringLiteral("状态码 (midRef)"), statusCodeRef.toString());
    printLine(QStringLiteral("状态码直接转 int"), QString::number(statusCodeRef.toInt()));

    // 2. 使用 splitRef 进行高性能批量拆分 (返回 QVector<QStringRef>，零临时字符串拷贝)
    QString lineData = QStringLiteral("ID1001:Sensor_Temperature:25.6:Status_OK");
    QVector<QStringRef> parts = lineData.splitRef(QStringLiteral(":"));

    std::cout << "  splitRef 零拷贝解析字段列表 :" << std::endl;
    for (int i = 0; i < parts.size(); ++i) {
        std::cout << QStringLiteral("    [%1] %2 (位置: %3, 长度: %4)")
                        .arg(i)
                        .arg(parts[i].toString().leftJustified(20, ' '))
                        .arg(parts[i].position())
                        .arg(parts[i].length())
                        .toUtf8().constData()
                  << std::endl;
    }
}

void StringDemo::testQStringView()
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [模块 3] QStringView 现代零拷贝视图 (C++17 / Qt 6 兼容)" << std::endl;
    std::cout << "============================================================" << std::endl;

    QString logLine = QStringLiteral("2026-08-28 14:30:00 [WARN] [NetworkWorker:102] Connection timed out after 3000ms");

    // QStringView 仅保存指针和长度，不产生任何堆内存分配与复制
    QStringView view(logLine);
    QStringView datePart = view.left(10);
    QStringView levelPart = view.mid(20, 6);
    int msgStart = view.indexOf(QStringLiteral("] ")) + 2;
    QStringView msgPart = view.mid(msgStart);

    printLine(QStringLiteral("原始日志字符串"), logLine);
    printLine(QStringLiteral("QStringView 日期切片"), datePart.toString());
    printLine(QStringLiteral("QStringView 级别切片"), levelPart.toString());
    printLine(QStringLiteral("QStringView 消息切片"), msgPart.toString());
}

void StringDemo::testQByteArrayRef()
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [模块 4] 自定义 QByteArrayRef 字节流切片引用实现与验证" << std::endl;
    std::cout << "============================================================" << std::endl;

    // 模拟工业协议原始二进制报文: [起始符 0x7E, 命令 0x05, 长度 0x04, 数据 "9999", 校验 0x23, 结束符 0x7E]
    QByteArray rawFrame;
    rawFrame.append(static_cast<char>(0x7E));
    rawFrame.append(static_cast<char>(0x05));
    rawFrame.append(static_cast<char>(0x04));
    rawFrame.append("9999");
    rawFrame.append(static_cast<char>(0x23));
    rawFrame.append(static_cast<char>(0x7E));

    printLine(QStringLiteral("原始帧 HEX 报文"), rawFrame.toHex(' ').toUpper());

    // 1. 基于自定义 QByteArrayRef 进行零拷贝视图包装
    QByteArrayRef frameRef(rawFrame);
    printLine(QStringLiteral("QByteArrayRef 完整大小"), QStringLiteral("%1 字节").arg(frameRef.size()));
    printLine(QStringLiteral("起始符检查 startsWith"), frameRef.startsWith(0x7E) ? QStringLiteral("正确 (0x7E)") : QStringLiteral("错误"));
    printLine(QStringLiteral("结束符检查 endsWith"), frameRef.endsWith(0x7E) ? QStringLiteral("正确 (0x7E)") : QStringLiteral("错误"));

    // 2. 零拷贝提取 Payload 负载切片
    QByteArrayRef payloadRef = frameRef.mid(3, 4); // 提取 "9999"
    printLine(QStringLiteral("负载切片 payloadRef"), payloadRef.toByteArray());
    printLine(QStringLiteral("负载数值 toInt() 解析"), QString::number(payloadRef.toInt()));

    // 3. 切片修剪 trimmed 与比较操作符
    QByteArrayRef paddedRef("   \t [STATUS_READY] \r\n  ");
    QByteArrayRef trimmedRef = paddedRef.trimmed();
    printLine(QStringLiteral("空白修剪 trimmed()"), trimmedRef.toByteArray());
    printLine(QStringLiteral("与 QByteArray 比较 =="), (trimmedRef == QByteArray("[STATUS_READY]")) ? QStringLiteral("匹配 (True)") : QStringLiteral("不匹配 (False)"));
}

void StringDemo::testQStringList()
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [模块 5] QStringList 字符串列表与批处理" << std::endl;
    std::cout << "============================================================" << std::endl;

    QString csvLine = QStringLiteral("Apple, Banana, Orange, Blueberry, Avocado, Cherry, Apricot");
    
    // 拆分并去除多余空白
    QStringList fruits = csvLine.split(QStringLiteral(","), Qt::SkipEmptyParts);
    for (int i = 0; i < fruits.size(); ++i) {
        fruits[i] = fruits[i].trimmed();
    }

    printLine(QStringLiteral("原始列表"), fruits.join(QStringLiteral(", ")));

    // 过滤以 'A' 开头的元素
    QStringList filtered = fruits.filter(QStringLiteral("A"), Qt::CaseInsensitive);
    printLine(QStringLiteral("过滤含 'A' 元素"), filtered.join(QStringLiteral(", ")));

    // 排序
    fruits.sort(Qt::CaseInsensitive);
    printLine(QStringLiteral("字母排序后"), fruits.join(QStringLiteral(", ")));

    // 拼接合并
    QString joined = fruits.join(QStringLiteral(" -> "));
    printLine(QStringLiteral("自定义箭头拼接"), joined);
}

void StringDemo::testQByteArray()
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [模块 6] QByteArray 字节流、HEX 与 Base64 编解码" << std::endl;
    std::cout << "============================================================" << std::endl;

    // 模拟二进制数据包
    QByteArray rawPacket;
    rawPacket.append(static_cast<char>(0xAA));
    rawPacket.append(static_cast<char>(0x55));
    rawPacket.append(static_cast<char>(0x01));
    rawPacket.append(static_cast<char>(0x04));
    rawPacket.append("\x10\x20\x30\x40", 4);
    rawPacket.append(static_cast<char>(0xFF));

    // 转换为大写十六进制可视化字符串
    QString hexFormatted = rawPacket.toHex(' ').toUpper();
    printLine(QStringLiteral("原始字节流 HEX"), hexFormatted);
    printLine(QStringLiteral("数据包字节数"), QStringLiteral("%1 字节").arg(rawPacket.size()));

    // 从十六进制字符串还原回二进制
    QByteArray restored = QByteArray::fromHex(hexFormatted.toLatin1());
    printLine(QStringLiteral("HEX 还原是否一致"), (rawPacket == restored) ? QStringLiteral("完全一致 (True)") : QStringLiteral("不一致 (False)"));

    // Base64 编码与解码
    QByteArray plainText = "Hello, Qt 5.15 / C++17 String Architecture!";
    QByteArray base64Data = plainText.toBase64();
    QByteArray decodedText = QByteArray::fromBase64(base64Data);

    printLine(QStringLiteral("Base64 编码前"), QString::fromUtf8(plainText));
    printLine(QStringLiteral("Base64 编码后"), QString::fromLatin1(base64Data));
    printLine(QStringLiteral("Base64 解码后"), QString::fromUtf8(decodedText));
}

void StringDemo::testEncodingConversion()
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [模块 7] 编码转换 (UTF-8 / GBK / Local8Bit) 与乱码防范" << std::endl;
    std::cout << "============================================================" << std::endl;

    QString chineseText = QStringLiteral("系统化 Qt 实战开发：避免中文乱码最佳实践！");
    printLine(QStringLiteral("原始 QString 文本"), chineseText);

    // 1. 转为 UTF-8 字节流
    QByteArray utf8Bytes = chineseText.toUtf8();
    printLine(QStringLiteral("UTF-8 字节流 HEX"), QStringLiteral("%1 (%2 字节)").arg(QString::fromLatin1(utf8Bytes.toHex(' ').toUpper())).arg(utf8Bytes.size()));

    // 2. 转为 GBK 字节流 (常用于对接遗留 Windows 本地 API 或老旧工控串口协议)
    QTextCodec *gbkCodec = QTextCodec::codecForName("GBK");
    if (gbkCodec) {
        QByteArray gbkBytes = gbkCodec->fromUnicode(chineseText);
        printLine(QStringLiteral("GBK 字节流 HEX"), QStringLiteral("%1 (%2 字节)").arg(QString::fromLatin1(gbkBytes.toHex(' ').toUpper())).arg(gbkBytes.size()));

        // 从 GBK 字节流还原回 QString
        QString fromGbk = gbkCodec->toUnicode(gbkBytes);
        printLine(QStringLiteral("GBK 解码还原"), fromGbk);
    } else {
        printLine(QStringLiteral("GBK 编解码器"), QStringLiteral("当前环境未找到 GBK 编码器"));
    }

    // 3. 为什么必须使用 QStringLiteral
    printLine(QStringLiteral("乱码防范规范"), QStringLiteral("界面中文强制使用 QStringLiteral(\"...\") 或 tr(\"...\")，杜绝运行时编码转换丢失。"));
}

void StringDemo::testRegex()
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [模块 8] QRegularExpression 正则表达式模式匹配与提取" << std::endl;
    std::cout << "============================================================" << std::endl;

    QString configText = QStringLiteral("server=192.168.1.100; port=8080; timeout=3000ms; max_connections=50;");
    printLine(QStringLiteral("原始配置报文"), configText);
    std::cout << "  正则提取键值对列表 :" << std::endl;

    // 匹配 key=value 键值对
    QRegularExpression regex(QStringLiteral("(\\w+)=([^;]+);"));
    QRegularExpressionMatchIterator iterator = regex.globalMatch(configText);

    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString key = match.captured(1);
        QString value = match.captured(2);
        std::cout << QStringLiteral("    • %1 = %2").arg(key.leftJustified(16, ' '), value).toUtf8().constData() << std::endl;
    }
}
