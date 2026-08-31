# demo_text_stream (Qt 文本流读写与格式化实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 `QTextStream` 文本流的核心能力：涵盖流操作符（Manipulators）格式化表格报表输出、多字符集编码（UTF-8, UTF-8 带 BOM, GBK/GB18030）读写无乱码处理、跨平台换行符（CRLF/LF）透明转换、CSV 报表文件结构化导出与逆向解析、以及内存字符串流（`QString` 内存缓冲）。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **QTextStream 流操作符** | `qSetFieldWidth()`, `left`, `right`, `hex`, `dec`, `showbase`, `forcesign` | 格式化输出对齐规整的工业巡检报表。 |
| **模块 2** | **多字符集编码与 BOM** | `setCodec("UTF-8" / "GB18030")`, `setGenerateByteOrderMark(true)` | 解决 Windows Excel 打开乱码、旧版工控机 GBK 文本互通。 |
| **模块 3** | **跨平台换行符处理** | `QIODevice::Text`, `\r\n` vs `\n` | 屏蔽 Windows 与 Linux/macOS 换行符差异。 |
| **模块 4** | **CSV 报表导出与解析** | CSV 表头与数据行读写、逗号转义处理 | 生产报表导出与数据源逐行导入。 |
| **模块 5** | **内存字符串流缓冲** | `QTextStream stream(&str)`, 流提取 `>>` | 内存中高效拼接复杂日志与反向流提取数值。 |

## 3. 适用场景
- 自动化生成报表、系统运行日志与导出 CSV/TXT 数据。
- 旧系统 GBK/GB18030 字符集协议对接。
- 文本配置文件解析与内存流式字符串组装。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_text_stream

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_text_stream
```
