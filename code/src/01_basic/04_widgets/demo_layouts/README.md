# demo_layouts (Qt 五大核心布局管理器使用指南)

## 1. 简介与定位
本 Demo 是一个纯粹的、面向教学与查阅参考的 Qt 布局管理器实战工程。按照**标准控件罗列 + 核心 API 演示 + 详细使用规范与注意事项**的形式组织，帮助开发者快速定位每种布局的典型用法。

## 2. 核心布局分类与 API 对照表

| 标签页 | 布局管理器 | 核心 API / 知识点 | 解决的问题 / 注意事项 |
| :--- | :--- | :--- | :--- |
| **Tab 1** | **QHBoxLayout / QVBoxLayout** | `addStretch(stretch)`, `setSpacing(px)`, `setContentsMargins()`, `setAlignment(w, align)`, `setStretch(idx, factor)` | 水平/垂直线性排列；利用 Stretch 控制子控件拉伸比例；弹簧占位顶开对齐。注意：Fixed 策略控件不受 Stretch 放大影响。 |
| **Tab 2** | **QGridLayout** | `addWidget(w, row, col, rowSpan, colSpan)`, `setRowStretch(row, s)`, `setColumnStretch(col, s)` | 跨行跨列单元格合并。注意：被跨越覆盖的网格坐标严禁重复添加控件，否则重叠渲染冲突。 |
| **Tab 3** | **QFormLayout** | `addRow(label, field)`, `addRow(singleWidget)`, `setRowWrapPolicy()`, `setLabelAlignment()` | 标准两列键值表单；平台原生标签与输入框自适应对齐。推荐在登录/注册/配置界面替代 QGridLayout。 |
| **Tab 4** | **QStackedWidget** | `addWidget(w)`, `setCurrentIndex(int)`, `setCurrentWidget(w)`, `currentChanged(int)` | 向导式多步骤界面分步切换；仅当前选中的页面处于可见状态（Visible），其余页面自动隐藏。 |
| **Tab 5** | **动态调参实验室** | `setContentsMargins(l,t,r,b)`, `setSpacing(px)`, `setStretch(idx, s)` | 运行时动态修改布局参数，观察窗口即时重新计算几何与重绘（Invalidate & Relayout）。 |

## 3. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_layouts

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_layouts
```
