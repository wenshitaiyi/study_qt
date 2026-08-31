# demo_debug (日志重定向与格式化控制台工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，系统演示 Qt 日志重定向机制（`qInstallMessageHandler`）、自定义日志格式化、按日期文件滚存、多线程并发日志落盘安全以及控制台 ANSI 彩色输出。

## 2. 核心技术点
- **`qInstallMessageHandler` 机制**：将 Qt 内部的 `qDebug()`, `qInfo()`, `qWarning()`, `qCritical()`, `qFatal()` 统一拦截重定向到自定义的 `Logger::messageHandler`。
- **结构化日志格式化**：提取 `QMessageLogContext` 中的源文件名、行号、调用函数，结合当前高精度时间戳（毫秒）与线程 ID 进行格式化排版：
  ```text
  [2026-08-28 14:15:00.123] [INFO ] [Thread:0x7ff8] [main.cpp:45] 系统运行正常
  ```
- **按日期日志文件滚存**：自动按 `logs/app_YYYY-MM-DD.log` 命名存储日志，UTF-8 编码写入。
- **多线程并发安全**：使用 `QMutex` 与 `QMutexLocker` 确保多子线程高频输出日志时文件写入不发生交错撕裂。
- **控制台 ANSI 彩色输出**：针对不同日志等级（Debug/Info/Warn/Crit）自动呈现不同颜色。

## 3. 适用场景
- 生产环境桌面客户端后台运行日志落盘。
- 自动化服务、批处理工具与守护进程的运行追踪。
- 现场客户机器异常与崩溃定位排查。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_debug

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_debug
```
