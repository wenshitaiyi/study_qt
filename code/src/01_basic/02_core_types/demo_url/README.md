# demo_url (URL 解析与网络参数构建实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 Qt URL 与 URI 解析体系（`QUrl`, `QUrlQuery`），涵盖 URL 组成部分提取、查询参数键值对动态构建与修改、本地文件路径与 `file:///` 协议互转、Percent Encoding 百分号编码以及 RESTful API 请求 URL 链式建造者模式。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **URL 组成部分精准提取** | `scheme()`, `host()`, `port()`, `path()`, `fileName()`, `fragment()`, `isLocalFile()` | 完整拆解网络与本地 URL 的各级元数据。 |
| **模块 2** | **QUrlQuery 查询参数处理** | `QUrlQuery`, `queryItems()`, `hasQueryItem()`, `addQueryItem()`, `removeQueryItem()` | HTTP GET 请求参数动态构建、参数去重与追加。 |
| **模块 3** | **本地路径与相对路径解析** | `QUrl::fromLocalFile()`, `QUrl::toLocalFile()`, `baseUrl.resolved(relative)` | 跨平台本地文件协议转换（处理空格与特殊符号）与相对链接拼接。 |
| **模块 4** | **Percent Encoding 编码** | `QUrl::toPercentEncoding()`, `QUrl::fromPercentEncoding()`, `FullyEncoded`, `PrettyDecoded` | 规避 URL 中文与特殊符号（`&`, `=`, ` `）传输乱码。 |
| **模块 5** | **UrlBuilder 链式建造者** | 链式 API 封装设计 | 优雅构建标准化 RESTful HTTP API 请求地址。 |

## 3. 适用场景
- 基于 `QNetworkAccessManager` 的 RESTful 网络接口调用与 API 参数封装。
- 浏览器内核（QWebEngineView / WebView）跨平台本地资源与网页交互。
- 多平台桌面客户端本地文件与外部网络链接统一抽象。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_url

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_url
```
