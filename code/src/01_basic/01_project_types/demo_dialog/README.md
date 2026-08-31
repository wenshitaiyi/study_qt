# demo_dialog (对话框全特性工程)

## 1. 简介与功能
本 Demo 全面演示 Qt 对话框体系的核心技术，包括自定义对话框的模态（Modal）与非模态（Modeless）运行模式，以及 5 种最常用的 Qt 内置标准对话框的高级调用。

## 2. 核心技术点
- **模态与非模态对比**：
  - **模态 (`exec()`)**：阻塞当前调用线程事件流，直到对话框关闭，直接返回 `Accepted` / `Rejected` 状态值。
  - **非模态 (`show()`)**：异步显示，主界面与子对话框均可独立接收鼠标与键盘事件，通过自定义信号槽（`configApplied`）传递数据，配置 `Qt::WA_DeleteOnClose` 实现关闭时自动析构防内存泄漏。
- **Qt 标准内置对话框**：
  1. **`QMessageBox`**：信息提示、警告、错误与多按钮（Yes/No/Cancel）询问弹窗。
  2. **`QFileDialog`**：单文件、多文件、保存文件与目录选择。
  3. **`QColorDialog`**：系统颜色选择与 Hex/RGB 转换。
  4. **`QFontDialog`**：系统字体家族、字号、字重视图配置与动态实时应用。
  5. **`QInputDialog`**：字符串、整数、浮点数以及下拉列表（Combobox）快速输入。

## 3. 适用场景
- 登录鉴权弹窗、系统参数设置面板。
- 业务流中的二次确认框、文件导出路径选择器。
- 文本编辑器的字体/颜色格式设置窗。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_dialog

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_dialog
```
