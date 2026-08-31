# demo_containers_ui (Qt 常用容器控件使用指南 - 主UI + 多子UI组合架构)

## 1. 简介与架构设计
本 Demo 采用**主 UI + 多子 UI 组合架构（Main UI + Sub UIs）**，以顶部 `QComboBox` 下拉导航作为多视图调度器，中央 `QStackedWidget` 承载 5 个独立的子页面（每个页面拥有独立的 `.ui`、`.h`、`.cpp`），系统化罗列并演示 Qt 中最常用的 5 大容器控件。

## 2. 核心容器与子 UI 对照表

| 子页面 | 容器类型 | 核心 API / 知识点 | 解决的问题 / 注意事项 |
| :--- | :--- | :--- | :--- |
| **Page 1** | **QGroupBox** | `setCheckable(bool)`, `setChecked()`, `toggled(bool)`, `setFlat(bool)`, `setAlignment()` | 标准分组框、扁平无边框分组、自锁复选框分组。注意：Checkable GroupBox 取消勾选时，其内部所有子控件自动被批量置灰禁用（setEnabled(false)）。 |
| **Page 2** | **QTabWidget** | `setTabPosition()`, `setTabsClosable(bool)`, `setMovable(bool)`, `setCornerWidget()`, `tabCloseRequested` | 标签栏上/下/左/右方位切换；拖拽排序；右上角挂载设置角标。注意：触发 tabCloseRequested 时必须显式调用 removeTab 并释放控件内存。 |
| **Page 3** | **QToolBox** | `addItem(w, text)`, `insertItem()`, `removeItem()`, `setCurrentIndex()`, `currentChanged` | 抽屉式手风琴（Accordion）垂直折叠面板，适用于 QQ 风格联系人列表、多层级分类筛选与工具箱。 |
| **Page 4** | **QScrollArea** | `setWidget()`, `setWidgetResizable(bool)`, `setHorizontalScrollBarPolicy()`, `ensureWidgetVisible()` | 大画布/海量卡片滚动视口；尺寸自适应缩放与滚动条策略（AsNeeded/AlwaysOn/AlwaysOff）。 |
| **Page 5** | **QSplitter** | `setOrientation()`, `setStretchFactor(idx, factor)`, `setCollapsible(idx, bool)`, `setSizes()` | 水平/垂直动态可拖拽分割条；按比例分配面板拉伸权重；禁止收缩至 0 像素；实时监听 `splitterMoved` 信号。 |

## 3. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_containers_ui

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_containers_ui
```
