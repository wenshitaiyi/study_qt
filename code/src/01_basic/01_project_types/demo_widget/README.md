# demo_widget (基础窗口工程)

## 1. 简介与功能
本 Demo 演示基于 `QWidget` 与 Qt Designer 可视化设计文件（`main_widget.ui`）创建基础窗口部件及按钮交互。

## 2. 核心技术点
- 使用 Qt Designer 绘制 `.ui` 界面布局文件（`main_widget.ui`），由 `uic` 自动生成 `ui_main_widget.h`。
- 采用现代 C++ 智能指针 `std::unique_ptr<Ui::MainWidget>` 管理 UI 指针生命周期。
- 在 `main_widget.cpp` 中通过 `m_ui->setupUi(this)` 绑定并初始化组件。
- 基础信号与槽（`m_ui->buttonTest::clicked` -> `MainWidget::onButtonClicked`）连接与状态更新。
- 使用 `QStringLiteral` 避免中文乱码。

## 3. 适用场景
- 轻量级桌面小挂件、悬浮工具条。
- 基于 Qt Designer 可视化快速排版的独立子窗口部件。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_widget

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_widget
```
