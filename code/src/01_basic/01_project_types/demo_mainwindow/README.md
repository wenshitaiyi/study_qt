# demo_mainwindow (标准主窗口工程)

## 1. 简介与功能
本 Demo 演示基于 `QMainWindow` 构建标准桌面主窗口应用程序，涵盖菜单栏、工具栏、状态栏、中心部件及停靠窗口的协同交互工作流程。

## 2. 核心技术点
- **`QMainWindow` 五大要素**：
  1. **`QMenuBar` (菜单栏)**：定义文件、视图与帮助菜单，配合快捷键（如 `Ctrl+N`, `Ctrl+O`, `Ctrl+S`, `Ctrl+Q`）。
  2. **`QToolBar` (工具栏)**：与菜单共用 `QAction`，支持拖拽浮动与停靠区域切换。
  3. **`CentralWidget` (中心工作区)**：基于 `QPlainTextEdit` 实现多行文本输入与实时统计。
  4. **`QStatusBar` (状态栏)**：演示临时消息（`showMessage`）与永久小部件（`addPermanentWidget` 显示行列数/字符数/UTF-8 编码）。
  5. **`QDockWidget` (停靠面板)**：可移动、可浮动、可关闭的侧边资源导航列表，并通过视图菜单的 `toggleViewAction()` 实现显隐控制。
- **文件对话框与流读写**：使用 `QFileDialog`、`QFile`、`QTextStream` 实现 UTF-8 文本文件的打开与保存。
- **消息弹窗**：使用 `QMessageBox::about` 与 `QMessageBox::aboutQt` 展示标准帮助弹窗。

## 3. 适用场景
- IDE 代码/文本编辑器框架。
- 专业桌面软件（如数据分析客户端、工业监控控制台、设计器）。
- 多面板协同交互的桌面端系统底座。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_mainwindow

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_mainwindow
```
