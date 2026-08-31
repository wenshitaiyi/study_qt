# demo_table_widget (QTableWidget 深度实战与避坑指南)

## 1. 简介与功能
本 Demo 是针对 `QTableWidget` 二维表格控件的专属深度实战工程。结合 **RPG 游戏全服英雄战力天梯排行榜与属性矩阵** 业务场景，系统化演示 QTableWidget 的全部核心交互、行选/列选/单元格选行为、表头列宽模式、单元格嵌入复杂控件、真正科学数值排序与关键开发避坑点。

## 2. 核心技术点与避坑对照表

| 功能模块 | 核心 API / 知识点 | 解决的问题与避坑指南 (Gotchas) |
| :--- | :--- | :--- |
| **选择行为与模式** | `setSelectionBehavior(SelectRows / SelectColumns / SelectItems)`, `setSelectionMode()` | 支持整行选择、整列选择与单个单元格选择动态切换；支持单选与 Ctrl/Shift 扩展多选。 |
| **编辑触发策略** | `setEditTriggers(DoubleClicked / SelectedClicked / NoEditTriggers)` | 支持双击编辑、选中后单击编辑或完全只读锁定。部分列可通过 `flags` 单独配置只读。 |
| **表头与列宽模式** | `horizontalHeader()->setSectionResizeMode(Stretch / ResizeToContents / Interactive)` | 支持自适应拉伸铺满、按内容紧凑自适应或用户自由拖拽表头分界线调节宽度。 |
| **数值科学排序避坑** | `item->setData(Qt::DisplayRole, intVal)` | **避坑重点**：若直接调用 `item->setText("1000")`，点击表头排序时 Qt 会按字符串字典序比较（导致 `"100"` 排在 `"20"` 前面）。必须存入整型 `QVariant`，实现真正的数值大小排序。 |
| **单元格嵌入富控件** | `setCellWidget(row, col, QWidget*)` | 单元格嵌入交互式操作按钮组（“⚔️ 挑战”、“🔍 查房”）与胜率进度条（`QProgressBar`）。**注意**：海量数据（>1000行）建议采用 Model/View + `QStyledItemDelegate`。 |
| **批量插入防卡顿刷新** | `setUpdatesEnabled(false / true)` | **性能优化重点**：大批量插入行（如 100 行）前关闭刷新，插入完成后恢复开启，消除 UI 重绘卡死与白屏闪烁，耗时由数百毫秒骤降至个位数毫秒。 |
| **右键菜单与坐标换算** | `customContextMenuRequested`, `viewport()->mapToGlobal(pos)` | **避坑重点**：右键菜单传入的 `pos` 位于视口坐标系，必须使用 `viewport()->mapToGlobal(pos)` 换算屏幕绝对坐标。点击行项弹出英雄专属菜单，点击空白弹出全局天梯操作菜单。 |

## 3. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_table_widget

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_table_widget
```
