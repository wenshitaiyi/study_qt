# demo_smart_pointers (Qt 与 C++ STL 智能指针全体系对比实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，系统对比 Qt 智能指针库（`QPointer`, `QScopedPointer`, `QSharedPointer`, `QWeakPointer`, `QSharedDataPointer`）与现代 C++ 标准库智能指针（`std::unique_ptr`, `std::shared_ptr`, `std::weak_ptr`），涵盖 `QPointer` 针对 `QObject` 自动置空防野指针机制、独占所有权与移动语义、引用计数与循环引用打破、以及手写实现具备隐式数据共享（写时复制 COW）特性的高性能自定义数据类。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **QPointer vs std::weak_ptr** | `QPointer<QObject>`, `isNull()` | `QPointer` 无需依赖 `shared_ptr` 即可直接监控任何 QObject，被销毁时自动归零置 `nullptr`，彻底解决异步回调悬挂野指针。 |
| **模块 2** | **QScopedPointer vs std::unique_ptr** | `QScopedPointer`, `std::unique_ptr`, `std::move`, 自定义 Deleter | 独占所有权 RAII 自动释放对比；`std::unique_ptr` 具备 C++11 移动语义可存入 STL 容器，而 `QScopedPointer` 适合 Qt 内部 PIMPL 模式。 |
| **模块 3** | **QSharedPointer vs std::shared_ptr** | `QSharedPointer::create()`, `std::make_shared`, `QWeakPointer`, `std::weak_ptr` | 强引用计数生命周期共享与通过弱指针打破循环引用导致的内存泄漏死锁。 |
| **模块 4** | **QSharedDataPointer (COW)** | `QSharedData`, `QSharedDataPointer<T>`, `d.detach()` | 亲手打造如 `QString` / `QVector` 般具备“写时复制”特性的高性能值对象类，实现浅拷贝零内存开销与按需深拷贝。 |
| **模块 5** | **智能指针全维度决策矩阵** | Qt 智能指针 vs STL 智能指针对比决策 | 提供完整清晰的 C++ / Qt 架构选型指引。 |

## 3. 适用场景
- 异步跨线程任务中对主窗口 / UI 控件生命周期的安全监听。
- 复杂网络请求与数据库连接池资源的 RAII 自动化清理。
- 设计高并发、高频传递的高性能自定义只读/可读写业务数据实体类。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_smart_pointers

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_smart_pointers
```
