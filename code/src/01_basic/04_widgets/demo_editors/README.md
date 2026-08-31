# demo_editors (Qt 常用输入与编辑器组件使用指南)

## 1. 简介与定位
本 Demo 是一个纯粹的、面向教学与查阅参考的 Qt 输入与编辑器组件实战工程。按照**标准控件罗列 + 核心 API 演示 + 详细使用规范与注意事项**的形式组织，帮助开发者快速掌握各类文本、数值与下拉选择组件。

## 2. 核心控件与 API 对照表

| 标签页 | 控件类型 | 核心 API / 知识点 | 解决的问题 / 注意事项 |
| :--- | :--- | :--- | :--- |
| **Tab 1** | **QLineEdit** | `setEchoMode()`, `setInputMask()`, `setValidator()`, `setCompleter()`, `setClearButtonEnabled()` | 明文/密码/短暂可见回显；IP/序列号固定格式掩码；整型与正则输入校验；搜索动态自动补全。 |
| **Tab 2** | **QSpinBox / QDoubleSpinBox** | `setRange()`, `setSingleStep()`, `setPrefix()`, `setSuffix()`, `setWrapping()`, `setSpecialValueText()` | 整数与浮点数值微调；支持前后缀修饰、单步增减控制、到达极值循环滚动以及 0 值特殊文本占位。 |
| **Tab 3** | **QComboBox / QFontComboBox / QKeySequenceEdit / QDateTimeEdit** | `addItem(text, userData)`, `itemData()`, `currentFontChanged()`, `keySequenceChanged()`, `setCalendarPopup()` | 下拉选项绑定后台自定义数据对象；系统字体选择；按键事件快速捕获与映射；日历弹出视图。 |
| **Tab 4** | **QTextEdit / QPlainTextEdit** | `setHtml()`, `toHtml()`, `textCursor().mergeCharFormat()`, `setMaximumBlockCount()` | 富文本编辑与 HTML 排版样式操作；海量日志/代码高性能渲染与最大行数防溢出控制。 |

## 3. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_editors

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_editors
```
