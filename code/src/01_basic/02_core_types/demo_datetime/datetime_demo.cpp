#include "datetime_demo.h"

#include <QTimeZone>
#include <QThread>
#include <iostream>
#include <vector>
#include <numeric>

static void printTitle(const QString &title)
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  " << title.toUtf8().constData() << std::endl;
    std::cout << "============================================================" << std::endl;
}

static void printLine(const QString &title, const QString &content)
{
    std::cout << QStringLiteral("  • %1 : %2")
                    .arg(title.leftJustified(26, ' '))
                    .arg(content)
                    .toUtf8().constData()
              << std::endl;
}

void DateTimeDemo::testFormattingAndParsing()
{
    printTitle(QStringLiteral("[模块 1] 日期时间构造、多格式化与逆向解析"));

    // 1. 获取当前系统时间
    QDateTime current = QDateTime::currentDateTime();
    printLine(QStringLiteral("默认当前时间"), current.toString());
    printLine(QStringLiteral("ISO 8601 标准格式"), current.toString(Qt::ISODateWithMs));
    printLine(QStringLiteral("自定义中文格式"), current.toString(QStringLiteral("yyyy年MM月dd日 hh时mm分ss秒.zzz毫秒 (dddd)")));
    printLine(QStringLiteral("RFC 2822 网络格式"), current.toString(Qt::RFC2822Date));

    // 2. 从自定义字符串逆向解析 (fromString)
    QString rawStr = QStringLiteral("2026-10-01 08:30:00.500");
    QDateTime parsed = QDateTime::fromString(rawStr, QStringLiteral("yyyy-MM-dd hh:mm:ss.zzz"));
    printLine(QStringLiteral("原始文本字符串"), rawStr);
    printLine(QStringLiteral("逆向解析后有效性"), parsed.isValid() ? QStringLiteral("解析有效 (Valid)") : QStringLiteral("无效 (Invalid)"));
    printLine(QStringLiteral("解析后提取年份/月/日"), QStringLiteral("%1年 %2月 %3日").arg(parsed.date().year()).arg(parsed.date().month()).arg(parsed.date().day()));
}

void DateTimeDemo::testCalculationsAndCalendar()
{
    printTitle(QStringLiteral("[模块 2] 时间加减运算、时间差与日历属性"));

    QDateTime now = QDateTime::currentDateTime();

    // 1. 时间加减运算
    QDateTime future30Days = now.addDays(30);
    QDateTime past500Hours = now.addSecs(-500 * 3600);
    printLine(QStringLiteral("当前时间基准"), now.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss")));
    printLine(QStringLiteral("30 天后的时间"), future30Days.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss")));
    printLine(QStringLiteral("500 小时前的时间"), past500Hours.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss")));

    // 2. 计算两个时间之间的跨度 (daysTo, secsTo, msecsTo)
    QDateTime targetEvent(QDate(2026, 12, 31), QTime(23, 59, 59));
    qint64 diffDays = now.daysTo(targetEvent);
    qint64 diffSecs = now.secsTo(targetEvent);
    printLine(QStringLiteral("距离 2026年终倒计时"), QStringLiteral("相差 %1 天 (约 %2 秒)").arg(diffDays).arg(diffSecs));

    // 3. 日历属性判断 (闰年、当月天数、星期几)
    QDate today = QDate::currentDate();
    bool isLeap = QDate::isLeapYear(today.year());
    int daysInCurrentMonth = today.daysInMonth();
    int dayOfWeek = today.dayOfWeek(); // 1 = 周一, 7 = 周日

    QString weekDayName;
    switch (dayOfWeek) {
    case 1: weekDayName = QStringLiteral("星期一 (Monday)"); break;
    case 2: weekDayName = QStringLiteral("星期二 (Tuesday)"); break;
    case 3: weekDayName = QStringLiteral("星期三 (Wednesday)"); break;
    case 4: weekDayName = QStringLiteral("星期四 (Thursday)"); break;
    case 5: weekDayName = QStringLiteral("星期五 (Friday)"); break;
    case 6: weekDayName = QStringLiteral("星期六 (Saturday)"); break;
    case 7: weekDayName = QStringLiteral("星期日 (Sunday)"); break;
    }

    printLine(QStringLiteral("今年 (%1年) 是否闰年").arg(today.year()), isLeap ? QStringLiteral("是 (Leap Year)") : QStringLiteral("否 (平年)"));
    printLine(QStringLiteral("当前月份天数"), QStringLiteral("%1 天").arg(daysInCurrentMonth));
    printLine(QStringLiteral("今天是星期几"), weekDayName);
}

void DateTimeDemo::testTimestampConversions()
{
    printTitle(QStringLiteral("[模块 3] Unix 秒级与毫秒级时间戳互转"));

    QDateTime now = QDateTime::currentDateTime();

    // 1. 获取 Unix 时间戳 (秒与毫秒)
    qint64 secondsSinceEpoch = now.toSecsSinceEpoch();
    qint64 msecsSinceEpoch = now.toMSecsSinceEpoch();

    printLine(QStringLiteral("当前秒级时间戳 (Unix)"), QString::number(secondsSinceEpoch));
    printLine(QStringLiteral("当前毫秒级时间戳 (Unix)"), QString::number(msecsSinceEpoch));

    // 2. 从时间戳还原 QDateTime
    QDateTime restoredFromSecs = QDateTime::fromSecsSinceEpoch(secondsSinceEpoch);
    QDateTime restoredFromMSecs = QDateTime::fromMSecsSinceEpoch(msecsSinceEpoch);

    printLine(QStringLiteral("秒级时间戳还原"), restoredFromSecs.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss")));
    printLine(QStringLiteral("毫秒级时间戳还原"), restoredFromMSecs.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss.zzz")));
}

void DateTimeDemo::testTimeZones()
{
    printTitle(QStringLiteral("[模块 4] 时区 (UTC / Local / QTimeZone) 转换"));

    QDateTime localNow = QDateTime::currentDateTime();
    QDateTime utcNow = localNow.toUTC();

    printLine(QStringLiteral("本地时间 (Local)"), localNow.toString(Qt::ISODate));
    printLine(QStringLiteral("世界标准时间 (UTC)"), utcNow.toString(Qt::ISODate));
    printLine(QStringLiteral("与 UTC 的时区偏移量"), QStringLiteral("%1 小时").arg(localNow.offsetFromUtc() / 3600.0, 0, 'f', 1));

    // 使用 QTimeZone 指定特定时区
    QTimeZone tokyoZone("Asia/Tokyo");
    QTimeZone londonZone("Europe/London");
    QTimeZone newyorkZone("America/New_York");

    if (tokyoZone.isValid()) {
        QDateTime tokyoTime = localNow.toTimeZone(tokyoZone);
        printLine(QStringLiteral("东京时间 (Asia/Tokyo)"), tokyoTime.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss (t)")));
    }
    if (londonZone.isValid()) {
        QDateTime londonTime = localNow.toTimeZone(londonZone);
        printLine(QStringLiteral("伦敦时间 (Europe/London)"), londonTime.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss (t)")));
    }
    if (newyorkZone.isValid()) {
        QDateTime nyTime = localNow.toTimeZone(newyorkZone);
        printLine(QStringLiteral("纽约时间 (America/New_York)"), nyTime.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss (t)")));
    }
}

void DateTimeDemo::testElapsedTimerBenchmark()
{
    printTitle(QStringLiteral("[模块 5] QElapsedTimer 高精度微秒/纳秒耗时统计"));

    QElapsedTimer timer;
    timer.start();

    // 模拟 1: 高精度休眠耗时测试
    QThread::msleep(20);
    qint64 sleepElapsedNsecs = timer.nsecsElapsed();
    printLine(QStringLiteral("QThread::msleep(20) 实测"), QStringLiteral("%1 ns (%2 μs / %3 ms)")
        .arg(sleepElapsedNsecs)
        .arg(sleepElapsedNsecs / 1000.0, 0, 'f', 2)
        .arg(sleepElapsedNsecs / 1000000.0, 0, 'f', 3));

    // 模拟 2: 密集计算算法耗时基准
    timer.restart();
    std::vector<int> numbers(1000000);
    std::iota(numbers.begin(), numbers.end(), 1);
    long long sum = std::accumulate(numbers.begin(), numbers.end(), 0LL);
    qint64 calcElapsedNsecs = timer.nsecsElapsed();

    printLine(QStringLiteral("100万整数生成与求和结果"), QString::number(sum));
    printLine(QStringLiteral("算法执行纯计算耗时"), QStringLiteral("%1 μs (微秒) / %2 ms (毫秒)")
        .arg(calcElapsedNsecs / 1000.0, 0, 'f', 2)
        .arg(calcElapsedNsecs / 1000000.0, 0, 'f', 3));
    printLine(QStringLiteral("计时器超时状态 hasExpired(50)"), timer.hasExpired(50) ? QStringLiteral("已超过 50ms") : QStringLiteral("未超时 (<50ms)"));
}
