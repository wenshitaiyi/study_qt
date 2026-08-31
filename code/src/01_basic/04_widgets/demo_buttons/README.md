# demo_buttons (RPG 游戏控制台与全能按钮/弹窗实战工程)

## 1. 简介与功能
本 Demo 是一个基于 Qt Widgets 的全能交互控制台应用程序。采用 **RPG 游戏战斗控制台与系统操作** 作为核心业务场景，深度剖析 Qt 按钮族（`QPushButton`, `QToolButton`, `QRadioButton`, `QCheckBox`）与系统消息弹窗族（`QMessageBox` 全家桶）的各项核心特性与高级用法。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **QPushButton 技能与状态控制** | `setEnabled(false)`, `QTimer`, `setCheckable(true)`, `setMenu(QMenu*)` | 技能施放 5s CD 动态倒计时禁用与恢复、狂暴/防御战姿开关切换、下拉连招技能菜单。 |
| **模块 2** | **QToolButton 模式与箭头** | `setPopupMode(MenuButtonPopup / InstantPopup)`, `setArrowType(Left/RightArrow)`, `setAutoRaise(true)` | 地图副本下拉难度选择、全图雷达即时弹窗、大地图左右翻页与悬浮无边框按钮。 |
| **模块 3** | **互斥单选与分组管理** | `QRadioButton`, `QButtonGroup`, `addButton(btn, id)`, `idClicked` | 阵营选择与画质渲染模式的互斥管理，通过整数 ID 极简绑定。 |
| **模块 4** | **双态与三态勾选** | `QCheckBox`, `setTristate(true)`, `Qt::PartiallyChecked`, `blockSignals` | 自动战斗辅助开关，掉落物全选/半选/全不选与子拾取项联动同步。 |
| **模块 5** | **QMessageBox 消息弹窗全家桶** | `information()`, `warning()`, `critical()`, `question()`, `addButton()` 自定义按钮 | 停服公告、体力告警、断网拦截、神装分解确认与战败复活多按钮选择。 |

## 3. 适用场景
- 游戏/工业上位机控制台主面板交互。
- 复杂表单操作工具栏、参数互斥切换与分级联动勾选。
- 系统核心操作二次确认、危险拦截与全局弹窗提示。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_buttons

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_buttons
```
