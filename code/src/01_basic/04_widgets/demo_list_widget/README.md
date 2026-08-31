# demo_list_widget (QListWidget 深度实战与避坑指南)

## 1. 简介与功能
本 Demo 是针对 `QListWidget` 这一高频列表控件的专属深度实战工程。结合 **RPG 游戏装备仓库与背包管理** 业务场景，系统化演示 QListWidget 的全部核心交互、CRUD 操作与关键开发避坑点。

## 2. 核心技术点与避坑对照表

| 功能模块 | 核心 API / 知识点 | 解决的问题与避坑指南 (Gotchas) |
| :--- | :--- | :--- |
| **CRUD 操作** | `addItem()`, `insertItem()`, `takeItem()`, `clear()` | **避坑重点**：`takeItem(row)` 仅将 Item 从视图脱离，**不会自动 delete 内存**！必须由调用者显式 `delete item`，否则导致内存泄漏。 |
| **Item 属性定制** | `setFlags()`, `setCheckState()`, `setData(Qt::UserRole)` | 双击就地重命名（`Qt::ItemIsEditable`）、开启多选勾选框（`Qt::ItemIsUserCheckable`）、自定义隐藏元数据绑定。 |
| **视图模式与排序** | `setViewMode(ListMode / IconMode)`, `sortItems()` | 标准列表模式与大图标网格模式动态切换；字典序与升降序排序。 |
| **实时模糊搜索** | 遍历 `setHidden(!matched)` | 搜索框输入关键字时动态隐藏不匹配项，性能高且无需推倒重建整个列表。 |
| **右键菜单与坐标换算** | `customContextMenuRequested`, `viewport()->mapToGlobal(pos)` | **避坑重点**：右键菜单传入的 `pos` 为视口坐标，弹出菜单必须使用 `listWidget->viewport()->mapToGlobal(pos)` 换算，直接使用 `mapToGlobal` 会因边距/表头产生位置偏移。 |
| **信号防递归** | `blockSignals(true / false)` | **避坑重点**：在 `itemChanged` 槽函数内部若再次修改 Item 文本或属性，必须调用 `blockSignals` 阻断信号，避免引发死循环递归调用导致程序崩溃。 |

## 3. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_list_widget

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_list_widget
```
