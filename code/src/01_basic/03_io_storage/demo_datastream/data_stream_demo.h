#pragma once

#include <QString>
#include <QDataStream>
#include <QFile>

#include "project_model.h"

class DataStreamDemo
{
public:
    // 1. QDataStream 基础数据类型序列化与大小端字节序 (Endianness)
    static void testBasicTypesAndEndianness();

    // 2. Qt 常用复合容器与数据结构内置流式序列化
    static void testQtContainersSerialization();

    // 3. 自定义复杂 C++ 对象工程存档序列化与反序列化
    static void testCustomProjectSerialization();

    // 4. 工程版本管理与向前/向后兼容机制 (Magic Header & Format Version)
    static void testVersionControlAndCompatibility();

    // 5. 内存二进制打包与私有网络协议封包 (QByteArray 序列化缓冲区)
    static void testMemoryPacketProtocol();
};
