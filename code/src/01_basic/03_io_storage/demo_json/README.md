# demo_json (Qt JSON 解析、深度合并与路径检索实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 Qt JSON 体系（`QJsonDocument`, `QJsonObject`, `QJsonArray`, `QJsonValue`），除了涵盖基础构造、紧凑/缩进序列化输出、语法解析错误捕获、与 `QVariant` 互相转换之外，**重点提供了高级实战算法工具库**：
1. **JSON 对象深度递归合并（Deep Merge）**：支持配置模板与个性化差异补丁（JSON Patch）的多层递归无损融合。
2. **点分路径式数据读取与动态写入（`getByPath` / `setByPath`）**：支持如 `"infrastructure.database.connection.pool.maxConnections"` 或数组下标 `"clusters[0].ip"` 的一键安全读取与自动建层写入。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **JSON 构建与格式化输出** | `QJsonObject`, `QJsonArray`, `toJson(Compact / Indented)` | 网络通信最小体积传输与日志配置文件美化排版。 |
| **模块 2** | **JSON 解析与错误捕获** | `QJsonDocument::fromJson()`, `QJsonParseError`, `errorString()`, `offset` | 接口调用时防范非法格式与畸形数据，精准定位语法错误偏移量。 |
| **模块 3** | **QVariant 互转** | `fromVariantMap()`, `toVariantMap()` | 与 Qt Model/View、属性系统无缝数据流转。 |
| **模块 4** | **JSON 深度合并 (Deep Merge)** | `JsonUtils::deepMerge(base, patch)` | 实现云端下发增量配置补丁与本地默认配置的树形深度融合。 |
| **模块 5** | **路径式检索与动态更新** | `JsonUtils::getByPath()`, `JsonUtils::setByPath()` | 避免繁琐的多层 `obj.value().toObject().value()` 判空取值，支持一行代码快速定位与动态写入。 |

## 3. 适用场景
- RESTful HTTP API 接口报文解析与封装。
- 现代客户端复杂 JSON 配置文件的读取、热更新与局部增量补丁合并。
- 低代码平台与图元属性树 JSON 数据存取。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_json

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_json
```
