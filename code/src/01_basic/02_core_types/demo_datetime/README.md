# demo_datetime (时间日期与性能基准全体系实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 Qt 日期时间体系（`QDateTime`, `QDate`, `QTime`, `QTimeZone`）与高精度性能基准分析工具（`QElapsedTimer`），涵盖多格式化解析、时间加减差值运算、Unix 时间戳转换、多时区转换及纳秒/微秒级算法耗时评测。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **格式化与逆向解析** | `toString()`, `fromString()`, `Qt::ISODateWithMs`, `Qt::RFC2822Date` | 标准化日志时间戳格式化、解析外部接口多格式时间字符串。 |
| **模块 2** | **时间运算与日历判定** | `addDays()`, `addSecs()`, `daysTo()`, `secsTo()`, `isLeapYear()`, `dayOfWeek()` | 任务倒计时、过期时间计算、闰年与月份天数判断。 |
| **模块 3** | **Unix 时间戳互转** | `toSecsSinceEpoch()`, `fromSecsSinceEpoch()`, `toMSecsSinceEpoch()`, `fromMSecsSinceEpoch()` | 网络协议、数据库与前端毫秒/秒级时间戳双向转换。 |
| **模块 4** | **多时区计算** | `toUTC()`, `toTimeZone()`, `QTimeZone`, `offsetFromUtc()` | 跨国跨地区系统本地时间与世界标准时间（UTC/GMT）转换。 |
| **模块 5** | **高精度性能评测** | `QElapsedTimer`, `nsecsElapsed()`, `elapsed()`, `hasExpired()` | 关键算法耗时基准评测、高精度网络/硬件响应超时检测。 |

## 3. 适用场景
- 客户端日志记录与定时任务调度。
- 跨国业务系统时区适配。
- 算法优化与性能瓶颈诊断基准测试。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_datetime

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_datetime
```
