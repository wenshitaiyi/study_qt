# demo_signalslot (Qt 信号与槽机制深度探索与全景实战)

## 1. 简介与功能
本 Demo 是进阶篇 **2.1 元对象系统与对象模型** 的专属 GUI 实战工程。深度探索与可视化验证 Qt 核心通信基础设施——**信号与槽（Signals & Slots）机制**，全面覆盖各类语法范式、Lambda 安全绑定、信号级联联动、绑定与执行顺序（FIFO）、重复绑定问题与 `Qt::UniqueConnection`、多维度解绑大全、自定义数据类型与 `qRegisterMetaType`、`void*` 通用指针与内存所有权安全、`QVariant` 包装、5 种连接模式在跨线程下的行为表现、`qApp` 跨线程调度与高级避坑技巧。

## 2. 核心技术模块与知识点对照表

| 模块 | 核心 API / 语法 | 解决的问题与避坑指南 (Gotchas) |
| :--- | :--- | :--- |
| **语法范式** | `SIGNAL(...) / SLOT(...)` vs `&Sender::sig, &Receiver::slot` | 对比 Qt4 字符串宏（运行时匹配无类型检查）与 Qt5 函数指针语法（编译期强类型检查与重构支持）。 |
| **重载二义性消除 (3种写法)** | 1. `QOverload<int>::of(&Class::sig)`<br>2. `qOverload<int>(&Class::sig)`<br>3. `static_cast<void (Class::*)(int)>(&Class::sig)` | 解决同名重载信号/槽的编译器二义性匹配问题：对比 Qt 模板辅助类、C++14 辅助模板宏与标准 C++ 成员函数指针类型转换。 |
| **Lambda 表达式** | `connect(sender, &sig, [=](){})` vs `connect(sender, &sig, this, [=](){})` | **避坑重点**：无 Context 的 Lambda 在接收者销毁后触发易导致野指针崩溃；传入 `this` 作为 Context 对象可由 Qt 自动托管生命周期与线程调度。 |
| **信号联动 (Signal to Signal)** | `connect(sig1, sig2)` | 信号三级级联联动与转发（`sigLevel1 -> sigLevel2 -> sigLevel3 -> slot`），实现多层架构数据向上冒泡与模块扁平解耦。 |
| **绑定顺序与执行顺序** | FIFO 先入先出队列 | **核心规则**：当一个信号绑定了多个槽函数时，Qt 在同线程下严格按照 `connect()` 的先后顺序依次同步执行。 |
| **重复绑定问题与防重** | `Qt::UniqueConnection` | **避坑重点**：Qt 默认允许同一对信号槽多次 connect，发射 1 次将触发 N 次！使用 `Qt::UniqueConnection` 确保连接唯一性，第 2/3 次绑定会被自动拒绝。 |
| **多维度解绑全解 (5 种形式)** | 1. `sender->disconnect()`<br>2. `disconnect(s, &sig, 0, 0)`<br>3. `disconnect(s, 0, r, 0)`<br>4. `disconnect(s, &sig, r, &slot)`<br>5. `QObject::disconnect(handle)` | 全面掌握发送者全量解绑、指定信号解绑、指定接收者解绑、单对精准解绑与 Connection 句柄单点断开。 |
| **自定义类型传递与元类型注册** | `Q_DECLARE_METATYPE(T)`, `qRegisterMetaType<T>("T")` | **跨线程重点**：QueuedConnection 必须将参数拷贝到事件队列，自定义结构体必须具备默认构造/拷贝构造/析构函数，并执行 `qRegisterMetaType` 运行时注册。 |
| **`void*` 通用指针与内存所有权** | `void*`, `reinterpret_cast<T*>` | **避坑重点**：跨线程异步传递堆内存 `void*` 时，必须明确所有权协议（由接收方负责 `delete`），防止野指针与内存泄漏。 |
| **QVariant 通用包装解耦** | `QVariant::fromValue(packet)`, `var.value<T>()` | 通用泛型安全传递方案：统一参数为 `const QVariant&`，接收端通过 `canConvert<T>()` 安全解包。 |
| **5 种连接模式** | `AutoConnection`, `DirectConnection`, `QueuedConnection`, `BlockingQueuedConnection`, `UniqueConnection` | **跨线程重点**：展示 Direct（发射线程立即同步执行）与 Queued（投递到目标事件队列安全跨线程）的线程 ID 差异；**禁止在同线程使用 BlockingQueuedConnection 避免死锁**。 |
| **qApp 调度切线程** | `QMetaObject::invokeMethod(qApp, [=](){ ... }, Qt::QueuedConnection)` | 在任意无 Qt 依赖的底层 `std::thread` / 三方库异步回调中无锁切回主 UI 线程安全更新界面。 |
| **自动命名绑定** | `QMetaObject::connectSlotsByName(this)` | 遵循 `on_<objectName>_<signalName>()` 命名规范实现全自动无代码绑定。 |
| **信号阻断与反射探测** | `blockSignals(bool)`, `sender()`, `receivers(SIGNAL(...))` | 批量更新时静音信号、探测当前事件触发源对象与统计槽函数连接数。 |

## 3. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_signalslot

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_signalslot
```
