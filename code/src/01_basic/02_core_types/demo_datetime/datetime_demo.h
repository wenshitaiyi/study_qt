#pragma once

#include <QString>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QElapsedTimer>

class DateTimeDemo
{
public:
    // 1. QDateTime / QDate / QTime 构造、格式化与逆向解析
    static void testFormattingAndParsing();

    // 2. 时间加减运算、跨度差值与日历辅助判定 (闰年/每月天数/星期)
    static void testCalculationsAndCalendar();

    // 3. Unix 时间戳与毫秒时间戳互相转换
    static void testTimestampConversions();

    // 4. 时区 (UTC / Local / QTimeZone) 转换与偏移量
    static void testTimeZones();

    // 5. QElapsedTimer 高精度微秒/纳秒级性能测试与耗时统计
    static void testElapsedTimerBenchmark();
};
