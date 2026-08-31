#pragma once

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>

class FileDemo
{
public:
    // 1. QFile 读写模式 (覆盖写、追加写、按行读与全量读)
    static void testFileReadWrite();

    // 2. QFile 文件生命周期管理 (拷贝、重命名、移动与安全删除)
    static void testFileLifecycle();

    // 3. QFileInfo 文件元数据与权限深度解析
    static void testFileInfoInspection();

    // 4. QDir 目录创建、删除与多条件过滤遍历
    static void testDirOperations();

    // 5. 递归目录树形扫描与层次化结构输出
    static void testRecursiveDirectoryScan();
};
