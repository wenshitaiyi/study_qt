# demo_variant (QVariant 游戏数据多态与元类型实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，**借用经典 RPG 游戏角色、背包系统、装备道具与副本掉落等概念**，深入浅出地演示 Qt 万能类型 `QVariant`、`QVariantMap`、`QVariantList` 的强大数据多态容器能力，以及如何利用 `Q_DECLARE_METATYPE` 和 `qRegisterMetaType` 将自定义 C++ 结构体无缝打包到 `QVariant` 中跨模块/跨信号槽安全流转。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **英雄属性字典 (QVariantMap)** | `QVariantMap`, `toString()`, `toInt()`, `toDouble()`, `toBool()` | 通用动态键值对属性存储，支持异构数据格式统一映射。 |
| **模块 2** | **异构游戏背包 (QVariantList)** | `QVariantList`, `canConvert<T>()`, `value<T>()`, `type()` | 单一容器槽位混合容纳整型（金币）、字符串（药水）、浮点数（倍率）与自定义装备结构体。 |
| **模块 3** | **自定义结构体包装** | `Q_DECLARE_METATYPE`, `qRegisterMetaType<T>()`, `fromValue()`, `qvariant_cast<T>()` | 注册自定义复杂 C++ 结构体（`GameItem`, `SkillBuff`）使其具备元对象识别能力，支持无缝装载与类型还原。 |
| **模块 4** | **复杂嵌套数据配置** | Map 嵌套 List 嵌套自定义结构体 | 解析如 Boss 战利品掉落表、复杂任务奖励等层级化树状数据结构。 |
| **模块 5** | **类型安全判定与避坑** | `canConvert()`, `isValid()`, `isNull()`, `userType()`, `toInt(&ok)` | 运行时类型推导与类型转换失败优雅降级，杜绝直接强制转换导致的程序崩溃。 |

## 3. 适用场景
- 复杂业务对象（如游戏角色档案、配置中心、通用事件总线 Payload）的数据包装。
- Qt Model/View 体系中 `QAbstractItemModel::data(index, role)` 多角色多类型数据返回。
- 跨线程信号槽中传递自定义结构体数据。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_variant

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_variant
```
