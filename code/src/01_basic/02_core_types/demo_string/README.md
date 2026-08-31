# demo_string (字符串与字节流全体系实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 Qt 字符串与字节流操作核心体系，涵盖 `QString` 格式化、`QStringRef` 经典子串引用、`QStringView` 现代零拷贝视图、**自定义 `QByteArrayRef` 字节流零拷贝引用**、`QStringList` 列表批处理、`QByteArray` 字节流编解码、UTF-8 / GBK 编码转换及 `QRegularExpression` 正则模式提取。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **QString 高效操作与切片** | `arg(对齐/补位/进制)`, `section()`, `mid()`, `chopped()`, `trimmed()`, `simplified()`, `toInt(ok, 16)` | 报文精确格式化、URL/路径多段拆解、空白清洗与进制安全转换。 |
| **模块 2** | **QStringRef 经典子串引用** | `QStringRef`, `leftRef()`, `midRef()`, `splitRef()` | Qt 5 专属优化，`splitRef()` 返回 `QVector<QStringRef>` 彻底避免子串分裂时的堆内存拷贝。 |
| **模块 3** | **QStringView 零拷贝视图** | `QStringView`, `left()`, `mid()`, `toString()` | C++17 / Qt 6 前瞻通用只读视图，兼顾字面量与 `QString` 零拷贝切片。 |
| **模块 4** | **自定义 QByteArrayRef 实现** | `QByteArrayRef`, `left()`, `mid()`, `toHex()`, `startsWith()`, `trimmed()` | 弥补 Qt 5 缺失 `QByteArrayRef` 的空白，为高频网络/串口二进制原始协议提供高性能零拷贝视图包装。 |
| **模块 5** | **QStringList 列表批处理** | `split()`, `join()`, `filter()`, `sort(Qt::CaseInsensitive)` | CSV/配置列表按分隔符拆装与快速关键字筛选。 |
| **模块 6** | **QByteArray 字节流处理** | `toHex(' ').toUpper()`, `fromHex()`, `toBase64()`, `fromBase64()` | 串口/网络二进制私有协议打包、HEX 报文调试打印与 Base64 传输。 |
| **模块 7** | **字符集编码与乱码防范** | `toUtf8()`, `toLocal8Bit()`, `QTextCodec (GBK)`, `QStringLiteral` | 跨平台中文路径、对接老旧 GBK 设备协议、彻底避免 MSVC 中文乱码。 |
| **模块 8** | **QRegularExpression 正则提取** | `QRegularExpression`, `globalMatch()`, `QRegularExpressionMatch` | 键值对配置报文 `key=value;` 快速自动化捕获解析。 |

## 3. 适用场景
- 串口/网络通信底层二进制数据帧零拷贝解包与封包。
- 文本文件、配置文件、URL 与日志高频报文解析。
- 跨平台（Windows / Linux）中文编码兼容与国际化处理。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_string

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_string
```
