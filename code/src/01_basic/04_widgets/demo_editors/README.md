# demo_editors (Qt 全能输入与编辑组件实战工程)

## 1. 简介与功能
本 Demo 是一个基于 Qt Widgets 的全功能输入与编辑组件实战工程。结合 **RPG 游戏管理与玩家交互控制中心（玩家登录、CDK 兑换码、神装模糊补全、数值微调、富文本活动公告排版、GM 脚本指令终端、音频与视角表盘）** 业务场景，深入剖析 Qt 提供的各类文本与数值输入组件。

## 2. 核心技术点与模块划分

| 标签页 | 控件类型与专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **Tab 1** | **QLineEdit 文本与校验** | `EchoMode`, `setInputMask()`, `QRegularExpressionValidator`, `QCompleter` | 账号密码掩码、CDK 格式约束、中英文角色名正则校验与神装模糊搜索补全。 |
| **Tab 2** | **数值微调与下拉选择** | `QSpinBox`, `QDoubleSpinBox`, `QComboBox`, `QFontComboBox`, `QKeySequenceEdit`, `QDateTimeEdit` | 英雄等级前缀/后缀、暴击率与折扣步长控制、装备品质 UserData 映射、技能按键重映射与限时活动日历。 |
| **Tab 3** | **富文本公告与 GM 脚本** | `QTextEdit`, `QTextCursor`, `QTextCharFormat`, `QPlainTextEdit` | 全服公告富文本加粗/变色排版与高性能开发者 GM 批处理脚本逐行解析执行。 |
| **Tab 4** | **音频滑块与视角罗盘** | `QSlider`, `QDial`, `setWrapping(true)`, `setNotchesVisible(true)` | 主音量/BGM 水平滑块与 360 度视角罗盘方向自动换算（正东/东南/正南等）。 |

## 3. 适用场景
- 客户端登录注册、激活码校验与敏感密码安全回显。
- 复杂数值参数微调、游戏快捷键绑定与全局日期时间配置。
- 运营富文本内容排版与工业/开发者控制台脚本输入。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_editors

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_editors
```
