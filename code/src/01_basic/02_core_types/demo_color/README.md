# demo_color (颜色空间与整型转换实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 Qt 颜色体系（`QColor`, `QRgb` / `uint32_t`）、ARGB 与 RGBA 字节序差异、通道取色与位运算重组、HSV/HSL 色彩空间变换以及智能明暗色与对比度文字反色判定。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **基础构造与 16 进制颜色** | `QColor("steelblue")`, `#RRGGBB`, `#AARRGGBB`, `name(HexArgb)` | 界面样式表 CSS 颜色解析、带 Alpha 半透明颜色定义。 |
| **模块 2** | **无符号整型 (uint32_t) 转换** | `QColor::fromRgba()`, `color.rgba()`, `color.rgb()`, `QRgb` | 图像底层像素数据与 `QColor` 对象之间的高性能无缝转换。 |
| **模块 3** | **通道取色与位运算合成** | `qAlpha()`, `qRed()`, `(argb >> 24) & 0xFF`, ARGB 与 RGBA 互转 | 解决 Qt (ARGB/BGRA) 与 OpenGL/Web/CSS (RGBA) 字节序不一致问题，实现硬件私有颜色协议编解码。 |
| **模块 4** | **HSV / HSL 色彩空间** | `getHsv()`, `fromHsv()`, 色相旋转 (Hue Rotate) | 调色板拾色器开发、互补色/类似色配色方案自动化生成。 |
| **模块 5** | **明暗色与自动文字反色** | `lighter()`, `darker()`, 感知亮度公式 ($0.299R + 0.587G + 0.114B$) | 悬浮高亮色/按下暗色自动计算、根据背景明暗自动推荐黑/白文字。 |

## 3. 适用场景
- 图像像素处理与底层渲染（QImage / OpenGL / DirectX）。
- 客户端动态主题换肤、调色板算法与自适应高对比度 UI。
- 工业设备与网络私有协议中的 32 位 RGB/RGBA 颜色字节流编解码。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_color

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_color
```
