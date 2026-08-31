# demo_json_tree_editor (JSON 树形结构可视化编辑器)

## 1. 简介与功能
本 Demo 结合 `QTreeWidget` 与 `QJsonDocument` 构建了一套专业级 **JSON 树形可视化编辑器**。摒弃多余侧边栏，将所有节点创建、修改、类型转换与删除操作**完全收敛于纯粹的右键上下文菜单（Context Menu）与就地双击编辑**，并与右侧原始 JSON 源码保持毫秒级双向同步。

## 2. 核心架构与操作指南

| 场景 / 节点类型 | 右键菜单操作 | 就地双击行为 |
| :--- | :--- | :--- |
| **{ } Object 节点** | ➕ 添加子字段（String、Number、Bool、Object、Array）、✏️ 重命名 Key、📋 复制该节点 JSON、🗑 删除节点 | 双击 Key 列直接重命名；双击 Value 列展开/折叠子层级 |
| **[ ] Array 节点** | ➕ 追加数组元素（String、Number、Bool、Object、Array）、📋 复制数组 JSON、🗑 删除数组 | 双击 Key 列重命名；双击 Value 列展开/折叠数组项 |
| **标量 Value 节点** | ✏️ 就地编辑值、🔀 转换类型（String ↔ Number ↔ Bool ↔ Null）、📋 复制键值、🗑 删除字段 | 双击 Key 列编辑键名；双击 Value 列就地修改数值 |
| **空白处** | ➕ 新建顶层 Object 节点、➕ 新建顶层 Array 节点、📂 全部展开、📁 全部折叠、🎮 恢复游戏预置配置 | 双击空白无动作 |

## 3. 双向同步与持久化
- **可视化树 -> 源码视图**：树节点任何修改、增删、类型变更，自动触发递归逆向序列化为 `QJsonDocument`，格式化更新至右侧 `QPlainTextEdit`。
- **源码视图 -> 可视化树**：用户在右侧直接粘贴或编辑 JSON 代码，点击 `[⬅️ 源码同步至树形]` 进行语法校验并逆向构建树形节点。
- **磁盘 I/O 回写**：支持打开外部 `.json` 配置文件并在可视化修改后一键持久化保存回写到磁盘。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_json_tree_editor

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_json_tree_editor
```
