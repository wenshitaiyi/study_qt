# demo_file (Qt 文件读写与目录遍历实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 Qt 基础文件与目录操作 API（`QFile`, `QFileInfo`, `QDir`），重点演示文件的只读/覆盖写/追加写、文件拷贝/重命名/删除生命周期管理、文件元数据与权限解析、目录创建与过滤遍历、以及通用的递归扫描树形打印算法。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **QFile 常用读写模式** | `open(ReadOnly / WriteOnly / Append / Truncate)`, `readLine()`, `readAll()`, `write()` | 日志文件行式追加与全量配置文件读写。 |
| **模块 2** | **文件生命周期管理** | `QFile::copy()`, `QFile::rename()`, `QFile::remove()`, `QFile::exists()` | 软件升级时自动备份旧文件、安全重命名与过期缓存删除。 |
| **模块 3** | **QFileInfo 元数据解析** | `baseName()`, `completeBaseName()`, `suffix()`, `completeSuffix()`, `size()`, `lastModified()`, `isWritable()` | 复杂复合扩展名（如 `.tar.gz`）精确解析、文件属性与访问权限核验。 |
| **模块 4** | **QDir 目录操作与过滤** | `mkpath()`, `rmpath()`, `entryInfoList(QDir::Files / Dirs)`, `entryList(NameFilters)` | 自动多级创建不存在的工作目录、按文件类型后缀批量筛选。 |
| **模块 5** | **树形递归扫描范式** | 深度优先递归遍历算法、树状缩进格式化 | 深度扫描任意目录层级结构并统计大小。 |

## 3. 适用场景
- 客户端日志记录与持久化。
- 应用程序工作目录、缓存目录的初始化自检与创建。
- 轻量级本地文件属性核验与目录遍历。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_file

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_file
```
