# demo_containers (通用容器与迭代器全体系实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 Qt 容器库（`QVector`, `QList`, `QMap`, `QHash`, `QSet`）的核心机制、性能差异对比、6 种 for 循环写法选型、STL 与 Java 风格迭代器、`qDeleteAll` 批量安全释放机制、容器与元素类的前向声明（Forward Declaration）规范，以及 Qt 隐式数据共享（Implicit Sharing / Copy-On-Write）与 Detach 避坑准则。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **QVector 序列容器与性能** | `reserve()`, `append()`, `std::sort`, `std::accumulate` | 预分配连续内存减少动态扩容开销，比对海量插入性能。 |
| **模块 2** | **QMap (红黑树) vs QHash (哈希表)** | `QMap` (有序 $O(\log N)$) vs `QHash` (无序 $O(1)$), `qHash()` 注入 | 根据数据检索频率与有序性要求选择容器，支持自定义结构体作为 Key。 |
| **模块 3** | **QSet 集合运算与去重** | `intersect()` (交集), `unite()` (并集), `subtract()` (差集) | 复杂权限集合运算、多集合差异对比与海量数据高速去重。 |
| **模块 4** | **迭代器全解析** | `cbegin()/cend()`, `QVectorIterator`, `QMutableVectorIterator`, `qAsConst` | 掌握 STL 风格与 Java 风格迭代器，演示遍历中安全原地删除（`remove()`）。 |
| **模块 5** | **隐式数据共享 (COW)** | 浅拷贝 (引用计数+1), 写时分离 (Detach), `qAsConst()` | 避免在 range-for 循环中意外触发 Detach 深拷贝带来的性能损耗。 |
| **模块 6** | **for 循环 6 种经典写法对比** | 下标索引 / STL迭代器 / `Q_FOREACH` / 范围 for / `qAsConst` / `std::for_each` | 掌握各遍历语法的场景选型与只读性能避坑。 |
| **模块 7** | **qDeleteAll 批量内存安全释放** | `qDeleteAll(list)`, `list.clear()` | 批量安全析构容器内堆指针对象，杜绝悬挂指针与内存泄漏。 |
| **模块 8** | **容器与元素前向声明规范** | 前向声明 `class Item;` vs 包含头文件，`QT_FORWARD_DECLARE_CLASS` | 指针容器与值容器的前向声明差异，降低大型项目头文件编译依赖。 |

## 3. 适用场景
- 高性能高频数据缓存与字典查找。
- 业务系统中的用户集合求交/求并运算。
- 深入理解 Qt 容器底层内存架构与编写高性能、低耦合的 C++ / Qt 代码。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_containers

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_containers
```
