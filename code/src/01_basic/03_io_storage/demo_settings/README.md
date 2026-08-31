# demo_settings (QSettings 配置持久化与企业级包装实战)

## 1. 简介与功能
本 Demo 是一个带图形界面（`.ui` 结合 `QDialog`）的桌面应用程序。对标真实企业级项目开发标准，将 `QSettings` 深度封装为类型安全、带默认值回退、支持单例访问的 `AppSettings` 配置管理类。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **企业级配置包装单例类** | `AppSettings::instance()`, `std::unique_ptr<QSettings>` | 避免业务代码中到处硬编码 Key 字符串，提供强类型 Get/Set 接口与缺省值安全回退。 |
| **模块 2** | **INI 格式与 UTF-8 编码** | `QSettings::IniFormat`, `setIniCodec("UTF-8")` | 跨平台可读性好，支持纯文本直接手工编辑与多语言中文字符存取。 |
| **模块 3** | **窗口几何状态记忆** | `saveGeometry()`, `restoreGeometry()` | 软件关闭时自动记录窗口位置、尺寸，启动时无缝恢复上次布局。 |
| **模块 4** | **INI 实时预览与同步** | `sync()`, `clear()`, `rawIniContent()` | 支持 UI 界面与磁盘 INI 文件双向实时同步、热加载与一键重置默认参数。 |

## 3. 适用场景
- 客户端软件偏好设置（深浅主题、多语言、开机自启）。
- 服务端连接配置（IP 地址、端口号、超时重试次数）。
- 窗口上次关闭位置、表格列宽、分栏布局状态记忆。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_settings

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_settings
```
