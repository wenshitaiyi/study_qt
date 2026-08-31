# demo_displays (Qt 常用展示控件与游戏界面实战工程)

## 1. 简介与功能
本 Demo 是一个基于 Qt Widgets 的多维展示控件实战应用程序。结合 **RPG 游戏数据与战场监控展示（英雄立绘徽章绘制、富文本内链与外链跳转、动态技能光效帧动画、热更新下载进度条、跨服天梯匹配跑马灯、首领垂直血量/怒气槽、副本通关液晶倒计时与连击计数器）** 业务场景，深入剖析 Qt 提供的各类核心展示组件。

## 2. 核心技术点与模块划分

| 标签页 | 控件类型与专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **Tab 1** | **图文展示与超链接** | `QLabel`, `linkActivated`, `QPainter` 绘制徽章, `QMovie` 动图控制 | 富文本排版、自定义协议内链（`item://`、`quest://`）与外链分流拦截、技能动态帧播放/暂停/倍速控制。 |
| **Tab 2** | **进度条与状态加载** | `QProgressBar`, `setRange(0, 0)` 跑马灯, `setFormat()`, `Qt::Vertical` 垂直槽 | 资源热更确定百分比下载进度、天梯匹配不确定忙碌动画、Boss 垂直双向血条与怒气槽增减。 |
| **Tab 3** | **液晶仪表与连击计数** | `QLCDNumber`, `display("MM:SS")`, `setDecMode / setHexMode / setBinMode`, `setSegmentStyle()` | 副本限时 10 分钟倒计时、战斗 Hits 连击数动态递增与 10/16/2 进制自由切换。 |
| **Tab 4** | **副本通关结算战报卡** | 综合混排 `QLabel` + `QLCDNumber` + `QProgressBar` | 通关积分、升级经验条与神装掉落综合战报面板无缝拼装。 |

## 3. 适用场景
- 游戏/桌面应用公告栏与内外部超链接拦截处理。
- 任务下载、解压、文件拷贝进度反馈与网络等待跑马灯。
- 工业设备数字表盘读数、倒计时时钟与高频计数器展示。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_displays

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_displays
```
