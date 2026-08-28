# demo_console (控制台工程)

## 1. 简介与功能
本 Demo 演示基于 `QCoreApplication` 创建无 GUI 界面的 Qt 控制台应用程序。

## 2. 核心技术点
- `QCoreApplication` 的生命周期与事件循环。
- `QCommandLineParser` 与 `QCommandLineOption` 实现命令行参数解析与 `--help` / `--version` 自动生成。
- `qDebug()` 控制台格式化输出与时间戳获取。

## 3. 适用场景
- 后台无界面服务程序。
- 命令行批处理小工具。
- 嵌入式/Linux 守护进程。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_console

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_console --name "Qt Developer"
```
