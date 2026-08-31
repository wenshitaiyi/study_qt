# demo_layouts (Qt 五大核心布局管理器与游戏界面实战工程)

## 1. 简介与功能
本 Demo 是一个基于 Qt Widgets 的全景布局实战应用程序。采用 **RPG 游戏开发业务场景（英雄 HUD 状态栏、背包物品网格矩阵、新角色属性注册表单、职业二转觉醒向导、动态布局实验室）**，通过 5 个专属 Tab 标签页全面剖析 Qt 的五大核心布局管理器（`QHBoxLayout`, `QVBoxLayout`, `QGridLayout`, `QFormLayout`, `QStackedLayout / QStackedWidget`）。

## 2. 核心技术点与模块划分

| 标签页 | 布局类型与专题 | 核心 API / 知识点 | 游戏化实战场景 |
| :--- | :--- | :--- | :--- |
| **Tab 1** | **QHBoxLayout & QVBoxLayout** | `addStretch()`, `setSpacing()`, `setAlignment()`, `setContentsMargins()` | 顶部英雄 HUD 状态栏（头像、血条/法力条、右对齐金币/钻石钱包）与底部等宽技能动作栏。 |
| **Tab 2** | **QGridLayout 网格矩阵** | `addWidget(w, row, col, rowSpan, colSpan)`, `setRowStretch()`, `setColumnStretch()` | 英雄立绘（跨 3行 x 2列 占据大单元格）与右侧 4x4 物品背包网格自适应对齐。 |
| **Tab 3** | **QFormLayout 结构化表单** | `setRowWrapPolicy(DontWrapRows)`, `setFieldGrowthPolicy()`, `labelAlignment()` | 角色昵称、职业选择、力量/敏捷/智力加点微调框与英雄背景传记自动左右对齐。 |
| **Tab 4** | **QStackedLayout / QStackedWidget** | `setCurrentIndex()`, `count()`, 向导步骤切换与状态校验 | 职业二转觉醒向导：资格考核评估 $\to$ 专精方向选择 $\to$ 誓约缔结完成。 |
| **Tab 5** | **动态布局调参实验室** | `setContentsMargins(val)`, `setSpacing(val)`, `setStretch(index, ratio)` | 实时滑块调节外边距、间距与 `1:1:1`、`1:2:1`、`1:3:2` 阶梯拉伸比效果。 |

## 3. 适用场景
- 复杂桌面软件多分辨率窗口自适应布局。
- 表单注册/配置设置页面标签与输入控件对齐。
- 引导安装、分步向导流程（Wizard）界面开发。
- 游戏背包网格、工具栏与状态栏弹性拉伸排版。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_layouts

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_layouts
```
