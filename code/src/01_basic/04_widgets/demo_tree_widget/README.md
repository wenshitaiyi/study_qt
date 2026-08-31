# demo_tree_widget (QTreeWidget 深度实战与避坑指南)

## 1. 简介与功能
本 Demo 是针对 `QTreeWidget` 树形层级控件的专属深度实战工程。结合 **RPG 游戏技能天赋树与多级转职专精** 业务场景，系统化演示 QTreeWidget 的全部核心交互、CRUD 操作、父子智能级联勾选与关键开发避坑点。

## 2. 核心技术点与避坑对照表

| 功能模块 | 核心 API / 知识点 | 解决的问题与避坑指南 (Gotchas) |
| :--- | :--- | :--- |
| **多级树 CRUD** | `addTopLevelItem()`, `addChild()`, `takeChild()`, `takeTopLevelItem()`, `clear()` | **避坑重点**：`takeChild(index)` 仅从父节点脱离关系，**不会自动 delete 内存**！必须显式 `delete item`，否则导致内存泄漏。 |
| **父子智能级联勾选** | `Qt::Checked`, `Qt::Unchecked`, `Qt::PartiallyChecked` (三态半选) | **避坑重点**：父节点更新子节点、子节点逆向更新父节点时，必须通过 `blockSignals(true/false)` 或标志位严格阻断递归，防止 `itemChanged` 引发死循环风暴崩溃。 |
| **树形递归模糊过滤** | 深度递归遍历 `setHidden(!visible)` 与自动展开 `setExpanded(true)` | **重点机制**：当叶子子节点命中搜索词时，除自身显示外，必须递归确保其所有祖先父节点保持可见，并调用 `setExpanded(true)` 自动展开分支让用户直观看到结果。 |
| **多级分流右键菜单** | `customContextMenuRequested`, `viewport()->mapToGlobal(pos)` | **避坑重点**：右键菜单传入的 `pos` 位于视口坐标系，必须使用 `viewport()->mapToGlobal(pos)` 换算屏幕绝对坐标。同时根据点击的是【空白区】、【顶层分支】还是【叶子技能节点】智能弹出不同的专属菜单。 |
| **就地编辑与元数据** | `Qt::ItemIsEditable`, `setData(Qt::UserRole)` | 双击任意列可直接重命名与就地编辑；通过 `UserRole` 绑定技能等级与上限数值。 |

## 3. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_tree_widget

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_tree_widget
```
