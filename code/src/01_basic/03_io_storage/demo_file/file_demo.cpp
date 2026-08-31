#include "file_demo.h"

#include <QCoreApplication>
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

// 获取本 Demo 专用的安全沙箱测试目录
static QString getSandboxDir()
{
    QString baseDir = QCoreApplication::applicationDirPath() + QStringLiteral("/sandbox_file_demo");
    QDir().mkpath(baseDir);
    return baseDir;
}

void FileDemo::testFileReadWrite()
{
    printTitle(QStringLiteral("[模块 1] QFile 常用打开模式与基本读写"));

    QString filePath = getSandboxDir() + QStringLiteral("/app_demo.log");

    // 1. 覆盖写入模式 (WriteOnly | Truncate | Text)
    {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            file.write("# === Application System Log Header ===\n");
            file.write(QStringLiteral("[INIT] 系统初始化完成于 %1\n")
                           .arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")))
                           .toUtf8());
            file.close();
            printLine(QStringLiteral("覆盖写入文件 (Truncate)"), QStringLiteral("写入成功 -> %1").arg(filePath));
        } else {
            printLine(QStringLiteral("写入文件失败"), file.errorString());
        }
    }

    // 2. 追加写入模式 (WriteOnly | Append | Text)
    {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            file.write("[INFO] 客户端连接成功: IP=127.0.0.1, Port=8080\n");
            file.write("[WARN] 内存使用率超过阈值 (72.5%)\n");
            file.close();
            printLine(QStringLiteral("追加日志记录 (Append)"), QStringLiteral("追加成功 (追加 2 行)"));
        }
    }

    // 3. 按行读取 (readLine)
    {
        std::cout << "  3. 按行逐行读取文件内容 (readLine) :" << std::endl;
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            int lineNum = 1;
            while (!file.atEnd()) {
                QByteArray line = file.readLine();
                std::cout << QStringLiteral("    [%1] %2")
                                .arg(lineNum++, 2, 10, QChar('0'))
                                .arg(QString::fromUtf8(line).trimmed())
                                .toUtf8().constData()
                          << std::endl;
            }
            file.close();
        }
    }

    // 4. 全量读取 (readAll)
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fullContent = file.readAll();
            printLine(QStringLiteral("全量读取大小 (readAll)"), QStringLiteral("%1 字节").arg(fullContent.size()));
            file.close();
        }
    }
}

void FileDemo::testFileLifecycle()
{
    printTitle(QStringLiteral("[模块 2] QFile 生命周期管理 (拷贝、重命名、删除)"));

    QString srcPath = getSandboxDir() + QStringLiteral("/app_demo.log");
    QString backupPath = getSandboxDir() + QStringLiteral("/app_demo_backup.log");
    QString archivedPath = getSandboxDir() + QStringLiteral("/app_demo_archived.log");

    // 1. 拷贝文件 (QFile::copy)
    if (QFile::exists(backupPath)) {
        QFile::remove(backupPath); // 若目标已存在需先移除
    }
    bool copyOk = QFile::copy(srcPath, backupPath);
    printLine(QStringLiteral("拷贝源文件至 Backup"), copyOk ? QStringLiteral("拷贝成功 (True)") : QStringLiteral("拷贝失败 (False)"));

    // 2. 重命名 / 移动文件 (QFile::rename)
    if (QFile::exists(archivedPath)) {
        QFile::remove(archivedPath);
    }
    bool renameOk = QFile::rename(backupPath, archivedPath);
    printLine(QStringLiteral("重命名 Backup -> Archived"), renameOk ? QStringLiteral("重命名成功 (True)") : QStringLiteral("重命名失败 (False)"));

    // 3. 安全删除文件 (QFile::remove)
    bool removeOk = QFile::remove(archivedPath);
    printLine(QStringLiteral("删除归档文件 (remove)"), removeOk ? QStringLiteral("删除成功 (True)") : QStringLiteral("删除失败 (False)"));
    printLine(QStringLiteral("归档文件是否存在 (exists)"), QFile::exists(archivedPath) ? QStringLiteral("存在") : QStringLiteral("已不存在 (False)"));
}

void FileDemo::testFileInfoInspection()
{
    printTitle(QStringLiteral("[模块 3] QFileInfo 文件元数据与属性获取"));

    // 创建一个复合后缀的多重测试文件
    QString complexFilePath = getSandboxDir() + QStringLiteral("/package.release.tar.gz");
    QFile dummyFile(complexFilePath);
    if (dummyFile.open(QIODevice::WriteOnly)) {
        dummyFile.write(QByteArray(2048, 'X')); // 写入 2KB 虚拟数据
        dummyFile.close();
    }

    QFileInfo info(complexFilePath);

    printLine(QStringLiteral("完整文件路径 (filePath)"), info.filePath());
    printLine(QStringLiteral("绝对规范路径 (canonical)"), info.canonicalFilePath());
    printLine(QStringLiteral("所在目录路径 (path)"), info.path());
    printLine(QStringLiteral("完整文件名 (fileName)"), info.fileName());
    printLine(QStringLiteral("基础文件名 (baseName)"), info.baseName());
    printLine(QStringLiteral("完整基础文件名 (completeBaseName)"), info.completeBaseName());
    printLine(QStringLiteral("后缀名 (suffix)"), info.suffix());
    printLine(QStringLiteral("完整后缀名 (completeSuffix)"), info.completeSuffix());
    printLine(QStringLiteral("文件物理大小 (size)"), QStringLiteral("%1 字节 (%2 KB)").arg(info.size()).arg(info.size() / 1024.0, 0, 'f', 2));
    printLine(QStringLiteral("最后修改时间 (lastModified)"), info.lastModified().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
    printLine(QStringLiteral("权限 (读 / 写 / 执行)"), QStringLiteral("R:%1 | W:%2 | X:%3")
        .arg(info.isReadable() ? "Yes" : "No")
        .arg(info.isWritable() ? "Yes" : "No")
        .arg(info.isExecutable() ? "Yes" : "No"));
    printLine(QStringLiteral("类型判断 (isFile / isDir)"), QStringLiteral("isFile=%1, isDir=%2")
        .arg(info.isFile() ? "True" : "False")
        .arg(info.isDir() ? "True" : "False"));
}

void FileDemo::testDirOperations()
{
    printTitle(QStringLiteral("[模块 4] QDir 目录创建、删除与多条件过滤遍历"));

    QString sandbox = getSandboxDir();
    QDir dir(sandbox);

    // 1. 创建多级子目录 (mkpath)
    QString subDir1 = QStringLiteral("docs/manual");
    QString subDir2 = QStringLiteral("images/icons");
    dir.mkpath(subDir1);
    dir.mkpath(subDir2);
    printLine(QStringLiteral("创建多级目录 (mkpath)"), QStringLiteral("成功创建 docs/manual 与 images/icons"));

    // 2. 创建临时测试文件
    QFile(sandbox + QStringLiteral("/readme.txt")).open(QIODevice::WriteOnly);
    QFile(sandbox + QStringLiteral("/banner.png")).open(QIODevice::WriteOnly);
    QFile(sandbox + QStringLiteral("/config.ini")).open(QIODevice::WriteOnly);

    // 3. 遍历当前目录 (按名称排序，目录优先)
    std::cout << "  3. 遍历沙箱主目录 (QDir::DirsFirst | QDir::Name) :" << std::endl;
    QFileInfoList entryList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name);
    for (const auto &item : entryList) {
        QString typeTag = item.isDir() ? QStringLiteral("[DIR] ") : QStringLiteral("[FILE]");
        std::cout << QStringLiteral("    %1 %2 (大小: %3 字节)")
                        .arg(typeTag)
                        .arg(item.fileName().leftJustified(22, ' '))
                        .arg(item.size())
                        .toUtf8().constData()
                  << std::endl;
    }

    // 4. 按通配符过滤 (NameFilters)
    QStringList txtFiles = dir.entryList(QStringList() << QStringLiteral("*.txt") << QStringLiteral("*.ini"), QDir::Files);
    printLine(QStringLiteral("通配符过滤 (*.txt; *.ini)"), txtFiles.join(QStringLiteral(", ")));
}

// 递归扫描辅助函数
static void scanDirInternal(const QDir &dir, int depth)
{
    QString indent(depth * 3, ' ');
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name);

    for (const auto &entry : entries) {
        if (entry.isDir()) {
            std::cout << QStringLiteral("%1├── [DIR]  %2/").arg(indent, entry.fileName()).toUtf8().constData() << std::endl;
            QDir subDir(entry.absoluteFilePath());
            scanDirInternal(subDir, depth + 1);
        } else {
            std::cout << QStringLiteral("%1└── [FILE] %2 (%3 字节)").arg(indent, entry.fileName()).arg(entry.size()).toUtf8().constData() << std::endl;
        }
    }
}

void FileDemo::testRecursiveDirectoryScan()
{
    printTitle(QStringLiteral("[模块 5] 递归目录树形扫描范式"));

    QString sandbox = getSandboxDir();
    QDir rootDir(sandbox);

    std::cout << "  开始对沙箱测试目录进行树形递归扫描 :" << std::endl;
    std::cout << "  " << rootDir.dirName().toUtf8().constData() << "/" << std::endl;
    scanDirInternal(rootDir, 1);
}
