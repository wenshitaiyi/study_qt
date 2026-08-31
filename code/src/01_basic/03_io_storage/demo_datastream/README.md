# demo_datastream (Qt 二进制数据流持久化与版本兼容实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 Qt 二进制数据流序列化体系（`QDataStream`），涵盖基础数据类型大小端字节序（BigEndian / LittleEndian）、Qt 复合容器与图元直接存取、操作符重载（`operator<<` / `operator>>`）实现复杂 CAD 工程存档、魔数（Magic Header）校验与数据格式版本向前/向后兼容机制、以及内存私有二进制网络协议封包与解包。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **字节序与基础类型序列化** | `setByteOrder(BigEndian / LittleEndian)`, `<<`, `>>` | 跨平台 x86/ARM/网络协议大小端字节对齐与转换。 |
| **模块 2** | **Qt 容器与内置对象流式存取** | `QDataStream::setVersion()`, `QMap`, `QList`, `QPoint`, `QColor` | 复杂嵌套容器与 GUI 图形属性免手写解析一键持久化。 |
| **模块 3** | **自定义对象操作符重载** | `friend operator<<`, `friend operator>>` | 工业 CAD / 设计器复杂工程文件存档（`.cadproj`）的无缝序列化与还原。 |
| **模块 4** | **魔数校验与版本兼容** | `MAGIC_HEADER`, `CURRENT_VERSION`, `ReadCorruptData` | 识别非法/损坏文件，支持旧版本历史存档向新版本升级时的平滑兼容。 |
| **模块 5** | **内存二进制封包解包** | `QDataStream(&byteArray)`, `writeRawData()`, `readRawData()` | 工业私有 TCP/UDP 网络通信协议定长包头与变长包体的高性能打包与解析。 |

## 3. 适用场景
- 客户端自定义工程文件格式（如 Photoshop `.psd`、CAD `.dwg` 类似机制的专属二进制存档）。
- 高性能私有二进制 TCP / 串口通信协议的序列化与反序列化。
- 复杂对象树内存快照存储与撤销/重做（Undo/Redo）历史状态暂存。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_datastream

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_datastream
```
