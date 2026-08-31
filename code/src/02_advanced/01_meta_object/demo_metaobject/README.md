# demo_metaobject (Qt 元对象系统与反射机制深度实战)

## 1. 简介与功能
本 Demo 是进阶篇 **2.1 元对象系统与对象模型** 的首个专属控制台实战工程。结合 **RPG 游戏英雄角色（GameHero）领域模型**，系统化探索与演示 Qt 区别于标准 C++ 的核心基础设施——**MOC（Meta-Object Compiler）元对象编译器与动态反射机制**。

## 2. 核心技术点与反射能力对照表

| 核心特性 / 宏 | 相关 API | 解决的问题与应用场景 |
| :--- | :--- | :--- |
| **类元内省** | `className()`, `superClass()`, `inherits()`, `Q_CLASSINFO` | 获取运行时真实类名、判断继承关系、读取类级作者/版本/元数据。 |
| **静态属性系统** | `Q_PROPERTY`, `QMetaProperty`, `property()`, `setProperty()` | 声明可读可写可通知属性，支持反射遍历、按属性名无类型耦合读写。广泛用于低代码引擎与 QML 绑定。 |
| **动态属性系统** | `setProperty()`, `property()`, `dynamicPropertyNames()` | 运行时无需改动 C++ 类结构即可动态附加任意键值属性，广泛用于 QSS 动态状态选择器（如 `[isVip="true"]`）。 |
| **枚举双向反射** | `Q_ENUM`, `QMetaEnum::fromType<T>()`, `keyToValue()`, `valueToKey()` | 字符串与枚举常量的双向高可靠无缝转换，彻底杜绝大量 `switch-case` 或手写 `std::map`。 |
| **方法与信号槽内省** | `QMetaMethod`, `methodSignature()`, `methodType()` | 运行时提取类的所有方法、信号、槽函数签名、返回值类型与形参列表。 |
| **动态方法调用** | `QMetaObject::invokeMethod()`, `Q_ARG()`, `Q_RETURN_ARG()` | 按字符串方法名动态调用带参/带返回值的 `Q_INVOKABLE` 函数或槽函数，支持指定连接类型（Direct / Queued），常用于插件系统与脚本引擎绑定。 |

## 3. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_metaobject

# 运行控制台输出
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_metaobject
```
