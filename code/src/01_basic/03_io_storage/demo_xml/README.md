# demo_xml (Qt XML 流式高性能解析与生成实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 Qt 提供的基于流的高性能 XML 处理组件（`QXmlStreamReader` 与 `QXmlStreamWriter`）。采用 **RPG 游戏数据系统（游戏主支线任务列表、神装图鉴数据库、NPC 分支剧情对话树）** 作为业务场景，演示流式增量读写、属性与文本提取、以及 XML 语法异常与行列号精准捕获。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **流式 XML 高性能构建** | `QXmlStreamWriter`, `writeStartDocument()`, `writeStartElement()`, `writeAttribute()`, `writeTextElement()` | 大规模游戏数据/工业报文流式写入，内存占用极低且排版整齐。 |
| **模块 2** | **事件驱动增量流式解析** | `QXmlStreamReader`, `readNext()`, `tokenType()`, `attributes()`, `readElementText()` | 无需一次性将整棵 DOM 树载入内存，逐 Token 增量提取任务与武器图鉴实体。 |
| **模块 3** | **语法错误与异常精准捕获** | `hasError()`, `errorString()`, `lineNumber()`, `columnNumber()` | 防御非法/残缺的 XML 数据，快速排查定位错误行号与列号。 |
| **模块 4** | **复杂层级剧情对话树解析** | `DialogueTree`, `Choices`, `Option` 递归遍历 | 游戏 NPC 动态剧情分支、条件跳转对话树的高效流式读取。 |

## 3. 适用场景
- 游戏配置文件读取（任务表、装备属性表、NPC 对话树）。
- 跨平台 Office / 工业级 XML 报文与数据交互。
- 矢量 SVG 图元与图层信息解析。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_xml

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_xml
```
