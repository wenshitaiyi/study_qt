#pragma once

#include <QString>
#include <QUrl>
#include <QUrlQuery>

class UrlDemo
{
public:
    // 1. QUrl 基础解析 (Scheme, Host, Port, Path, Fragment)
    static void testUrlComponents();

    // 2. QUrlQuery 查询参数键值对提取与动态构建
    static void testUrlQuery();

    // 3. 本地文件路径与 file:/// 协议双向互转及相对路径解析
    static void testLocalFileAndRelativePath();

    // 4. 百分号编码 (Percent Encoding) 与中文/特殊符号转义
    static void testPercentEncoding();

    // 5. 实战工具：RESTful API 请求 URL 链式建造者
    static void testUrlBuilderPattern();
};
