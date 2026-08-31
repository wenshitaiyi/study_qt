#include "container_demo.h"

#include <QElapsedTimer>
#include <QDebug>
#include <iostream>
#include <algorithm>
#include <numeric>

// 演示前向声明对应的完整类定义
class TaskItem
{
public:
    TaskItem(int id, const QString &name)
        : m_id(id)
        , m_name(name)
    {
        std::cout << "    [构造] TaskItem #" << m_id << " (" << m_name.toUtf8().constData() << ")" << std::endl;
    }

    ~TaskItem()
    {
        std::cout << "    [析构] TaskItem #" << m_id << " (" << m_name.toUtf8().constData() << ") 已安全释放!" << std::endl;
    }

    int id() const { return m_id; }
    QString name() const { return m_name; }

private:
    int m_id;
    QString m_name;
};

// 头文件中仅前向声明的元素类，在此处提供完整定义
class ForwardDeclaredItem
{
public:
    ForwardDeclaredItem(int id, const QString &name)
        : m_id(id)
        , m_name(name)
    {
    }

    int id() const { return m_id; }
    QString name() const { return m_name; }

private:
    int m_id;
    QString m_name;
};

// TaskManagerWithIncompleteType 的成员方法实现
TaskManagerWithIncompleteType::TaskManagerWithIncompleteType() = default;
TaskManagerWithIncompleteType::~TaskManagerWithIncompleteType() = default; // 必须在已知完整定义的 .cpp 中析构！

void TaskManagerWithIncompleteType::addItem(int id, const QString &name)
{
    m_items.append(ForwardDeclaredItem(id, name));
}

int TaskManagerWithIncompleteType::count() const
{
    return m_items.size();
}

void TaskManagerWithIncompleteType::printAll() const
{
    for (const auto &item : qAsConst(m_items)) {
        std::cout << QStringLiteral("     • 任务项 #%1: %2").arg(item.id()).arg(item.name()).toUtf8().constData() << std::endl;
    }
}

// 自定义类型作为 QHash 键结构体
struct DeviceNode
{
    int deviceId{0};
    QString deviceName;

    bool operator==(const DeviceNode &other) const noexcept
    {
        return deviceId == other.deviceId && deviceName == other.deviceName;
    }
};

// 为自定义类型注入 qHash 全局重载
inline uint qHash(const DeviceNode &node, uint seed = 0) noexcept
{
    return qHash(node.deviceId, seed) ^ qHash(node.deviceName, seed);
}

static void printTitle(const QString &title)
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  " << title.toUtf8().constData() << std::endl;
    std::cout << "============================================================" << std::endl;
}

static void printLine(const QString &title, const QString &content)
{
    std::cout << QStringLiteral("  • %1 : %2")
                    .arg(title.leftJustified(26, ' '))
                    .arg(content)
                    .toUtf8().constData()
              << std::endl;
}

void ContainerDemo::testSequenceContainers()
{
    printTitle(QStringLiteral("[模块 1] QVector 与 QList 序列容器性能与算法"));

    const int TOTAL_ELEMENTS = 300000;

    // 1. 无预分配内存插入耗时
    QElapsedTimer timer;
    timer.start();
    QVector<int> vecNoReserve;
    for (int i = 0; i < TOTAL_ELEMENTS; ++i) {
        vecNoReserve.append(i);
    }
    qint64 timeNoReserve = timer.nsecsElapsed() / 1000; // 微秒

    // 2. 有 reserve 预分配内存插入耗时
    timer.restart();
    QVector<int> vecWithReserve;
    vecWithReserve.reserve(TOTAL_ELEMENTS);
    for (int i = 0; i < TOTAL_ELEMENTS; ++i) {
        vecWithReserve.append(i);
    }
    qint64 timeWithReserve = timer.nsecsElapsed() / 1000; // 微秒

    printLine(QStringLiteral("插入规模"), QStringLiteral("%1 个整型元素").arg(TOTAL_ELEMENTS));
    printLine(QStringLiteral("无 reserve 插入耗时"), QStringLiteral("%1 μs (微秒)").arg(timeNoReserve));
    printLine(QStringLiteral("有 reserve 预分配耗时"), QStringLiteral("%1 μs (微秒)").arg(timeWithReserve));
    printLine(QStringLiteral("性能提升比率"), QStringLiteral("预分配提速约 %1%").arg((timeNoReserve - timeWithReserve) * 100.0 / timeNoReserve, 0, 'f', 1));

    // 3. 标准算法支持 (std::sort, std::accumulate)
    QVector<int> sampleList = {65, 23, 89, 12, 45, 99, 34};
    std::sort(sampleList.begin(), sampleList.end());
    int sum = std::accumulate(sampleList.begin(), sampleList.end(), 0);

    QStringList sortedStrList;
    for (int n : sampleList) sortedStrList << QString::number(n);

    printLine(QStringLiteral("std::sort 升序排序"), sortedStrList.join(QStringLiteral(", ")));
    printLine(QStringLiteral("std::accumulate 求和"), QString::number(sum));
}

void ContainerDemo::testAssociativeContainers()
{
    printTitle(QStringLiteral("[模块 2] QMap (红黑树有序) vs QHash (哈希表无序)"));

    // 1. QMap 自动按 Key 升序排序
    QMap<QString, int> portMap;
    portMap.insert(QStringLiteral("HTTP"), 80);
    portMap.insert(QStringLiteral("HTTPS"), 443);
    portMap.insert(QStringLiteral("SSH"), 22);
    portMap.insert(QStringLiteral("FTP"), 21);
    portMap.insert(QStringLiteral("MySQL"), 3306);

    std::cout << "  QMap 红黑树自动排序遍历 (Key 从小到大有序) :" << std::endl;
    for (auto it = portMap.cbegin(); it != portMap.cend(); ++it) {
        std::cout << QStringLiteral("    • 协议: %1 -> 端口: %2")
                        .arg(it.key().leftJustified(10, ' '))
                        .arg(it.value())
                        .toUtf8().constData()
                  << std::endl;
    }

    // 2. QHash 高频查找性能对比
    const int SCALE = 100000;
    QMap<int, QString> bigMap;
    QHash<int, QString> bigHash;

    for (int i = 0; i < SCALE; ++i) {
        QString val = QStringLiteral("Node_%1").arg(i);
        bigMap.insert(i, val);
        bigHash.insert(i, val);
    }

    QElapsedTimer timer;
    timer.start();
    for (int i = 0; i < SCALE; ++i) {
        auto val = bigMap.value(i);
        Q_UNUSED(val);
    }
    qint64 mapLookupTime = timer.nsecsElapsed() / 1000;

    timer.restart();
    for (int i = 0; i < SCALE; ++i) {
        auto val = bigHash.value(i);
        Q_UNUSED(val);
    }
    qint64 hashLookupTime = timer.nsecsElapsed() / 1000;

    printLine(QStringLiteral("10万次查找 QMap 耗时"), QStringLiteral("%1 μs (微秒)").arg(mapLookupTime));
    printLine(QStringLiteral("10万次查找 QHash 耗时"), QStringLiteral("%1 μs (微秒)").arg(hashLookupTime));

    // 3. 自定义结构体作为 QHash Key
    QHash<DeviceNode, QString> deviceStatusHash;
    DeviceNode dev1{101, QStringLiteral("PLC_Main_Station")};
    DeviceNode dev2{102, QStringLiteral("Sensor_Array_A")};

    deviceStatusHash.insert(dev1, QStringLiteral("运行中 (Running)"));
    deviceStatusHash.insert(dev2, QStringLiteral("待机 (Standby)"));

    printLine(QStringLiteral("自定义 Key QHash 查找"), QStringLiteral("设备 [%1] 状态: %2").arg(dev1.deviceName, deviceStatusHash.value(dev1)));
}

void ContainerDemo::testSetOperations()
{
    printTitle(QStringLiteral("[模块 3] QSet 集合运算 (交/并/差) 与高效去重"));

    QSet<QString> setA = {QStringLiteral("Qt"), QStringLiteral("C++"), QStringLiteral("Python"), QStringLiteral("CMake")};
    QSet<QString> setB = {QStringLiteral("C++"), QStringLiteral("Rust"), QStringLiteral("Go"), QStringLiteral("Qt")};

    printLine(QStringLiteral("集合 A"), QStringList(setA.values()).join(QStringLiteral(", ")));
    printLine(QStringLiteral("集合 B"), QStringList(setB.values()).join(QStringLiteral(", ")));

    // 交集 &
    QSet<QString> intersectSet = setA;
    intersectSet.intersect(setB);
    printLine(QStringLiteral("交集 (setA & setB)"), QStringList(intersectSet.values()).join(QStringLiteral(", ")));

    // 并集 |
    QSet<QString> unionSet = setA;
    unionSet.unite(setB);
    printLine(QStringLiteral("并集 (setA | setB)"), QStringList(unionSet.values()).join(QStringLiteral(", ")));

    // 差集 -
    QSet<QString> diffSet = setA;
    diffSet.subtract(setB);
    printLine(QStringLiteral("差集 (setA - setB)"), QStringList(diffSet.values()).join(QStringLiteral(", ")));

    // 去重操作
    QVector<int> duplicateData = {1, 2, 2, 3, 4, 4, 4, 5, 1, 6, 7, 7};
    QSet<int> dedupSet(duplicateData.begin(), duplicateData.end());
    QVector<int> uniqueList(dedupSet.begin(), dedupSet.end());
    std::sort(uniqueList.begin(), uniqueList.end());

    QStringList resultStrList;
    for (int v : uniqueList) resultStrList << QString::number(v);
    printLine(QStringLiteral("QSet 快速去重结果"), resultStrList.join(QStringLiteral(", ")));
}

void ContainerDemo::testIterators()
{
    printTitle(QStringLiteral("[模块 4] 迭代器全解析 (STL 风格 vs Java 风格 vs 范围 for)"));

    QVector<QString> modules = {QStringLiteral("Core"), QStringLiteral("Gui"), QStringLiteral("Widgets"), QStringLiteral("Network"), QStringLiteral("Sql")};

    // 1. STL 风格迭代器
    std::cout << "  1. STL 风格 const_iterator 遍历 :" << std::endl;
    for (auto it = modules.cbegin(); it != modules.cend(); ++it) {
        std::cout << "     • " << it->toUtf8().constData() << std::endl;
    }

    // 2. Java 风格只读迭代器 QVectorIterator
    std::cout << "  2. Java 风格只读迭代器 (QVectorIterator) :" << std::endl;
    QVectorIterator<QString> javaIt(modules);
    while (javaIt.hasNext()) {
        std::cout << "     • " << javaIt.next().toUtf8().constData() << std::endl;
    }

    // 3. Java 风格可变迭代器 QMutableVectorIterator (安全原地删除)
    QVector<int> numbers = {10, 15, 20, 25, 30, 35};
    QMutableVectorIterator<int> mutIt(numbers);
    while (mutIt.hasNext()) {
        if (mutIt.next() % 2 != 0) {
            mutIt.remove(); // 安全删除奇数项
        }
    }

    QStringList evenStrs;
    for (int n : numbers) evenStrs << QString::number(n);
    printLine(QStringLiteral("Java可变迭代器原地移除奇数"), evenStrs.join(QStringLiteral(", ")));

    // 4. C++17 范围 for + qAsConst (避免意外深拷贝)
    std::cout << "  4. C++ 范围 for + qAsConst 现代只读遍历 :" << std::endl;
    for (const auto &item : qAsConst(modules)) {
        std::cout << "     • " << item.toUtf8().constData() << std::endl;
    }
}

void ContainerDemo::testImplicitSharing()
{
    printTitle(QStringLiteral("[模块 5] 隐式数据共享 (Copy-On-Write) 与 Detach 避坑机制"));

    // 1. 浅拷贝：共享同一底层内存指针
    QVector<int> originalVec = {100, 200, 300, 400};
    QVector<int> copyVec = originalVec; // 引用计数 +1，零数据拷贝

    printLine(QStringLiteral("originalVec 内存地址"), QStringLiteral("0x%1").arg(reinterpret_cast<quintptr>(originalVec.constData()), 0, 16));
    printLine(QStringLiteral("copyVec     内存地址"), QStringLiteral("0x%1").arg(reinterpret_cast<quintptr>(copyVec.constData()), 0, 16));
    printLine(QStringLiteral("浅拷贝后地址是否相同"), (originalVec.constData() == copyVec.constData()) ? QStringLiteral("完全相同 (共享底层数据)") : QStringLiteral("不同"));

    // 2. 写时复制 (Detach)：触发写操作时才真正进行深拷贝分配
    copyVec[0] = 999; // 触发写时分离 detach!

    printLine(QStringLiteral("修改后 copyVec 地址"), QStringLiteral("0x%1 (已分离)").arg(reinterpret_cast<quintptr>(copyVec.constData()), 0, 16));
    printLine(QStringLiteral("修改后 originalVec[0]"), QString::number(originalVec[0]));
    printLine(QStringLiteral("修改后 copyVec[0]"), QString::number(copyVec[0]));

    // 3. 避坑指南：只读场景使用 qAsConst
    printLine(QStringLiteral("黄金实践准则"), QStringLiteral("在 range-for 循环只读遍历容器时，务必使用 qAsConst(container) 或 cbegin()/cend()，防止调用非 const 成员隐式触发 detach 性能损失。"));
}

void ContainerDemo::testForLoopStyles()
{
    printTitle(QStringLiteral("[模块 6] Qt / C++ for 循环的 6 种经典写法对比"));

    QVector<QString> frameworks = {QStringLiteral("QtWidgets"), QStringLiteral("QtQuick"), QStringLiteral("QtNetwork"), QStringLiteral("QtSql")};

    // 方式 1: 传统索引下标遍历 (适合随机访问容器，支持基于索引操作)
    std::cout << "  1. 传统下标索引遍历 (for int i=0; i<size; ++i) :" << std::endl;
    for (int i = 0; i < frameworks.size(); ++i) {
        std::cout << "     [" << i << "] " << frameworks[i].toUtf8().constData() << std::endl;
    }

    // 方式 2: STL 风格迭代器遍历 (通用性强，无索引开销)
    std::cout << "  2. STL 风格迭代器遍历 (for auto it=cbegin; it!=cend; ++it) :" << std::endl;
    for (auto it = frameworks.cbegin(); it != frameworks.cend(); ++it) {
        std::cout << "     • " << it->toUtf8().constData() << std::endl;
    }

    // 方式 3: Qt 经典宏 Q_FOREACH / foreach (Qt 专属，自动浅拷贝容器快照，防遍历中被修改)
    std::cout << "  3. Qt 经典宏 Q_FOREACH / foreach :" << std::endl;
    foreach (const QString &item, frameworks) {
        std::cout << "     • " << item.toUtf8().constData() << std::endl;
    }

    // 方式 4: 现代 C++ 范围 for 引用修改 (for auto &item : list)
    QVector<int> scores = {80, 90, 85};
    for (auto &s : scores) {
        s += 5; // 原地自增5分
    }
    QStringList scoreStrs;
    for (int s : scores) scoreStrs << QString::number(s);
    printLine(QStringLiteral("范围 for 引用原地修改"), scoreStrs.join(QStringLiteral(", ")));

    // 方式 5: 现代 C++ 范围 for + qAsConst (Qt 官方推荐只读最佳实践，彻底避免 Detach)
    std::cout << "  5. 范围 for + qAsConst (黄金推荐) :" << std::endl;
    for (const auto &item : qAsConst(frameworks)) {
        std::cout << "     • " << item.toUtf8().constData() << std::endl;
    }

    // 方式 6: std::for_each + Lambda 表达式 (函数式编程风格)
    std::cout << "  6. std::for_each + Lambda 函数式遍历 :" << std::endl;
    std::for_each(frameworks.cbegin(), frameworks.cend(), [](const QString &name) {
        std::cout << "     -> " << name.toUtf8().constData() << std::endl;
    });
}

void ContainerDemo::testQtContainerMacrosAndAlgorithms()
{
    printTitle(QStringLiteral("[模块 7] qDeleteAll 与 Qt 常用容器宏 / 算法实战"));

    // 1. qDeleteAll 针对指针容器批量释放堆内存
    std::cout << "  1. 创建包含堆指针对象的 QVector<TaskItem*> :" << std::endl;
    QVector<TaskItem*> taskList;
    taskList.append(new TaskItem(1, QStringLiteral("初始化网络连接")));
    taskList.append(new TaskItem(2, QStringLiteral("加载数据库缓存")));
    taskList.append(new TaskItem(3, QStringLiteral("渲染主窗口视图")));

    printLine(QStringLiteral("释放前容器大小"), QStringLiteral("%1 个指针元素").arg(taskList.size()));

    std::cout << "  2. 执行 qDeleteAll(taskList) 批量析构堆对象 :" << std::endl;
    qDeleteAll(taskList); // 逐个调用 delete

    // 【重要安全准则】：qDeleteAll 只负责 delete 指针指向的堆内存，
    // 但容器内仍保留已失效的悬挂指针地址！因此必须立即调用 clear() 清空指针！
    taskList.clear();
    printLine(QStringLiteral("qDeleteAll + clear() 后大小"), QStringLiteral("%1 个元素 (已杜绝悬挂指针)").arg(taskList.size()));

    // 2. QMap 搭配 qDeleteAll
    std::cout << "  3. QMap<QString, TaskItem*> 搭配 qDeleteAll 实战 :" << std::endl;
    QMap<QString, TaskItem*> taskMap;
    taskMap.insert(QStringLiteral("Task_A"), new TaskItem(10, QStringLiteral("后台日志轮转")));
    taskMap.insert(QStringLiteral("Task_B"), new TaskItem(20, QStringLiteral("心跳检测包发送")));

    qDeleteAll(taskMap);
    taskMap.clear();
    printLine(QStringLiteral("taskMap 已完成 qDeleteAll + clear"), QStringLiteral("全部释放完毕"));
}

#include "pipeline_service.h"

// 实现基于 qcontainerfwd.h 接口的流水线服务实现类
class PipelineServiceImpl : public IPipelineService
{
public:
    void processBatch(const QVector<ForwardDeclaredItem> &items) override
    {
        std::cout << QStringLiteral("     [PipelineService] 正在处理 %1 个前向声明数据项...").arg(items.size()).toUtf8().constData() << std::endl;
    }

    void setFilterTags(const QStringList &tags) override
    {
        std::cout << QStringLiteral("     [PipelineService] 过滤标签配置: %1").arg(tags.join(QStringLiteral(", "))).toUtf8().constData() << std::endl;
    }

    void collectMetrics(QMap<QString, int> &outMetrics) const override
    {
        outMetrics.insert(QStringLiteral("ProcessedCount"), 100);
        outMetrics.insert(QStringLiteral("SuccessRate"), 99);
    }
};

void ContainerDemo::testForwardDeclarations()
{
    printTitle(QStringLiteral("[模块 8] QVector<T> 值容器与 qcontainerfwd.h 前向声明机制"));

    std::cout << "  【QVector<IncompleteType> 前向声明底层原理】:\n"
              << "  • Qt 容器（QVector, QList）基于隐式共享架构，对象本体仅持有一个指向内部管理块的 d-pointer (即 sizeof(QVector<T>) == sizeof(void*))。\n"
              << "  • 因此即便是【值类型容器】 QVector<ForwardDeclaredItem>，在头文件声明类成员时，也完全支持元素类型仅做前向声明 (class ForwardDeclaredItem;)。\n"
              << "\n"
              << "  【强迫症福音：容器自身的前向声明头文件 <QtCore/qcontainerfwd.h>】:\n"
              << "  • Qt 官方提供了专用的轻量前向声明头文件：#include <QtCore/qcontainerfwd.h>\n"
              << "  • 包含该头文件后，QVector, QList, QMap, QHash, QSet, QStringList 等全部获得模板前向声明，\n"
              << "    在接口类（如 IPipelineService）的参数引用/指针中完全无需引入具体容器的实现头文件！\n"
              << "\n"
              << "  【核心编译避坑规则 (黄金铁律)】:\n"
              << "  1. 宿主类的构造函数与析构函数【不能在头文件中内联/默认实现 (~TaskManager() = default;)】。\n"
              << "  2. 必须将宿主类的析构函数放在 .cpp 实现文件中，因为在析构 QVector<T> 时需要调用 T 的完整析构函数！\n"
              << std::endl;

    // 1. 运行实战验证：值容器前向声明宿主类
    TaskManagerWithIncompleteType manager;
    manager.addItem(101, QStringLiteral("解析前向声明值容器配置"));
    manager.addItem(102, QStringLiteral("构建依赖倒置组件流水线"));
    manager.addItem(103, QStringLiteral("平稳析构释放任务"));

    printLine(QStringLiteral("管理器容纳元素数"), QStringLiteral("%1 个").arg(manager.count()));
    std::cout << "  值容器遍历输出验证 :" << std::endl;
    manager.printAll();

    // 2. 运行实战验证：基于 qcontainerfwd.h 纯前向声明的抽象接口
    std::cout << "\n  基于 <QtCore/qcontainerfwd.h> 纯前向声明接口调用测试 :" << std::endl;
    PipelineServiceImpl service;
    service.setFilterTags({QStringLiteral("Production"), QStringLiteral("FastPath")});
    QMap<QString, int> metrics;
    service.collectMetrics(metrics);
    printLine(QStringLiteral("接口导出指标 Processed"), QString::number(metrics.value(QStringLiteral("ProcessedCount"))));
}
