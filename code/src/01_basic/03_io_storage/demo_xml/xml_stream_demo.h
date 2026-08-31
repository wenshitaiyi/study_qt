#pragma once

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "game_xml_models.h"

class XmlStreamDemo
{
public:
    // 1. 基于流的高性能 XML 游戏数据导出与格式化构建 (QXmlStreamWriter)
    static void testStreamWriting();

    // 2. 基于流的事件驱动增量游戏数据解析 (QXmlStreamReader)
    static void testStreamReading();

    // 3. XML 语法错误与非法标签精准捕获 (hasError, lineNumber, columnNumber)
    static void testErrorHandling();

    // 4. RPG 游戏 NPC 分支对话树与任务状态增量读写
    static void testNpcDialogueTreeStreaming();
};
