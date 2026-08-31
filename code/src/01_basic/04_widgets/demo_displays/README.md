# demo_displays (Qt 常用展示控件使用指南)

## 1. 简介与定位
本 Demo 是一个纯粹的、面向教学与查阅参考的 Qt 常用展示与输出控件实战工程。按照**标准控件罗列 + 核心 API 演示 + 详细使用规范与注意事项**的形式组织，帮助开发者快速掌握图文标签、动态进度条、数字液晶屏与悬停气泡帮助。

## 2. 核心控件与 API 对照表

| 标签页 | 控件类型 | 核心 API / 知识点 | 解决的问题 / 注意事项 |
| :--- | :--- | :--- | :--- |
| **Tab 1** | **QLabel / QMovie** | `setTextFormat(Qt::RichText)`, `linkActivated`, `setPixmap()`, `QMovie::start()/stop()/setSpeed()` | 富文本排版与内部/外部超链接拦截处理；静态位图绘制与 GIF 动图播放控制。注意：高质量位图缩放建议使用 `pixmap.scaled(Qt::KeepAspectRatio)`。 |
| **Tab 2** | **QProgressBar** | `setRange(min, max)`, `setValue()`, `setFormat("%p% (%v/%m)")`, `setRange(0, 0)`, `setOrientation(Qt::Vertical)` | 确定进度模式；自定义文本百分比格式；耗时未知任务的跑马灯流动模式；垂直方向与反向增长。 |
| **Tab 3** | **QLCDNumber** | `display()`, `setDigitCount()`, `setSegmentStyle()`, `setDecMode() / setHexMode() / setBinMode()` | 仿真液晶时钟与高频计数器；支持 Outline/Filled/Flat 三种液晶样式以及十进制/十六进制/二进制一键切换。 |
| **Tab 4** | **QToolTip & QWhatsThis** | `setToolTip(QString)`, `setWhatsThis(QString)` | 控件鼠标悬停浮动富文本气泡；按 `Shift + F1` 唤起上下文帮助说明。 |

## 3. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_displays

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_displays
```
