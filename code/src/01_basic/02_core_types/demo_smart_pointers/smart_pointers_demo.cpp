#include "smart_pointers_demo.h"

#include <iostream>
#include <iomanip>
#include <vector>

static void printTitle(const QString &title)
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  " << title.toUtf8().constData() << std::endl;
    std::cout << "============================================================" << std::endl;
}

static void printLine(const QString &title, const QString &content)
{
    std::cout << QStringLiteral("  • %1 : %2")
                    .arg(title.leftJustified(32, ' '))
                    .arg(content)
                    .toUtf8().constData()
              << std::endl;
}

// ============================================================================
// TrackedNode 实现
// ============================================================================
TrackedNode::TrackedNode(int id, const QString &name, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_name(name)
{
    std::cout << "    [构造 QObject] TrackedNode #" << m_id << " (" << m_name.toUtf8().constData() << ")" << std::endl;
}

TrackedNode::~TrackedNode()
{
    std::cout << "    [析构 QObject] TrackedNode #" << m_id << " (" << m_name.toUtf8().constData() << ") 已安全销毁!" << std::endl;
}

// ============================================================================
// Employee (COW 隐式共享类) 实现
// ============================================================================
Employee::Employee()
    : d(new EmployeeData())
{
}

Employee::Employee(int id, const QString &name, const QVector<QString> &skills)
    : d(new EmployeeData(id, name, skills))
{
}

// 拷贝构造函数：仅仅复制 d 指针，引用计数 +1，不产生任何深拷贝开销！
Employee::Employee(const Employee &other)
    : d(other.d)
{
}

// 赋值运算符：复用 d 指针
Employee &Employee::operator=(const Employee &other)
{
    if (this != &other) {
        d = other.d;
    }
    return *this;
}

Employee::~Employee() = default;

int Employee::id() const { return d->m_id; }
QString Employee::name() const { return d->m_name; }
QVector<QString> Employee::skills() const { return d->m_skills; }
const EmployeeData *Employee::internalDataPointer() const { return d.constData(); }

// 写入操作：非 const 调用会自动触发 d.detach() 分离出独占副本！
void Employee::setId(int id) { d->m_id = id; }
void Employee::setName(const QString &name) { d->m_name = name; }
void Employee::addSkill(const QString &skill) { d->m_skills.append(skill); }

// ============================================================================
// CyclicNode 实现
// ============================================================================
CyclicNode::CyclicNode(const QString &name)
    : m_name(name)
{
    std::cout << "    [构造 CyclicNode] " << m_name.toUtf8().constData() << std::endl;
}

CyclicNode::~CyclicNode()
{
    std::cout << "    [析构 CyclicNode] " << m_name.toUtf8().constData() << " 已释放!" << std::endl;
}

// ============================================================================
// 测试套件具体实现
// ============================================================================

void SmartPointersDemo::testQPointerForQObject()
{
    printTitle(QStringLiteral("[模块 1] QPointer: 针对 QObject 独一无二的自动置空弱引用"));

    std::cout << "  1. 堆上创建普通 QObject 派生对象 (非 shared_ptr 托管) :" << std::endl;
    auto *rawNode = new TrackedNode(101, QStringLiteral("MainDialog_Instance"));

    // 使用 QPointer 监控 QObject 派生对象
    QPointer<TrackedNode> guardedNode = rawNode;

    printLine(QStringLiteral("销毁前 rawNode 内存地址"), QStringLiteral("0x%1").arg(reinterpret_cast<quintptr>(rawNode), 0, 16));
    printLine(QStringLiteral("销毁前 guardedNode.isNull()"), guardedNode.isNull() ? QStringLiteral("True (空)") : QStringLiteral("False (有效)"));
    printLine(QStringLiteral("销毁前通过 QPointer 访问"), guardedNode->name());

    std::cout << "  2. 手动 delete 释放 rawNode (模拟父窗口销毁或对象被外部 deleteLater) :" << std::endl;
    delete rawNode;
    rawNode = nullptr;

    // 观察 QPointer 是否自动归零
    printLine(QStringLiteral("销毁后 guardedNode.isNull()"), guardedNode.isNull() ? QStringLiteral("True (自动归零置 nullptr！)") : QStringLiteral("False"));
    printLine(QStringLiteral("销毁后 guardedNode.data()"), QStringLiteral("0x%1 (安全保护，绝不发生悬挂指针野指针崩盘)").arg(reinterpret_cast<quintptr>(guardedNode.data()), 0, 16));

    std::cout << "  【STL 差异分析】: std::weak_ptr 无法直接监听普通 new/Qt对象树管理的对象，必须强绑定 std::shared_ptr，而 QPointer 是 Qt 对象树天然保护伞！\n" << std::endl;
}

// 自定义内存释放器
struct CustomBufferDeleter
{
    static inline void cleanup(int *pointer)
    {
        if (pointer) {
            std::cout << "    [QScopedPointer 自定义 Deleter] 释放缓冲区..." << std::endl;
            delete[] pointer;
        }
    }
};

void SmartPointersDemo::testScopedVsUniquePointer()
{
    printTitle(QStringLiteral("[模块 2] QScopedPointer vs std::unique_ptr 独占指针全方位对比"));

    // 1. QScopedPointer 对比 std::unique_ptr
    {
        std::cout << "  1. QScopedPointer 独占管理 (RAII 自动析构) :" << std::endl;
        QScopedPointer<TrackedNode> scopedPtr(new TrackedNode(201, QStringLiteral("ScopedWorker")));
        printLine(QStringLiteral("QScopedPointer 访问"), scopedPtr->name());

        // 无法进行拷贝，也不能转移给 std::vector (QScopedPointer 缺乏现代移动语义 move)
    }

    {
        std::cout << "\n  2. std::unique_ptr 独占管理 (现代 C++ 移动语义支持) :" << std::endl;
        auto uniquePtr = std::make_unique<TrackedNode>(202, QStringLiteral("UniqueWorker"));
        printLine(QStringLiteral("std::unique_ptr 访问"), uniquePtr->name());

        // 支持 std::move 转移所有权到 STL 容器中
        std::vector<std::unique_ptr<TrackedNode>> vec;
        vec.push_back(std::move(uniquePtr));
        printLine(QStringLiteral("std::move 转移后 uniquePtr.get()"), QStringLiteral("0x%1 (已转让所有权)").arg(reinterpret_cast<quintptr>(uniquePtr.get()), 0, 16));
        printLine(QStringLiteral("容器中对象访问"), vec.back()->name());
    }

    // 3. 自定义 Deleter 对比
    {
        std::cout << "\n  3. 自定义 Deleter 对比 :" << std::endl;
        QScopedPointer<int, CustomBufferDeleter> qScopedBuf(new int[512]);
        auto stdUniqueBuf = std::unique_ptr<int[], void(*)(int*)>(new int[512], [](int *p) {
            std::cout << "    [std::unique_ptr Lambda Deleter] 释放缓冲区..." << std::endl;
            delete[] p;
        });
    }
}

void SmartPointersDemo::testSharedAndWeakPointersComparison()
{
    printTitle(QStringLiteral("[模块 3] QSharedPointer/QWeakPointer vs std::shared_ptr/std::weak_ptr"));

    // 1. QSharedPointer 与 std::shared_ptr 引用共享机制对比
    std::cout << "  1. QSharedPointer 共享与弱引用提升 :" << std::endl;
    QSharedPointer<TrackedNode> qSp1 = QSharedPointer<TrackedNode>::create(301, QStringLiteral("QSharedSession"));
    printLine(QStringLiteral("qSp1 初始状态 isNull()"), qSp1.isNull() ? QStringLiteral("True") : QStringLiteral("False (持有对象)"));
    {
        QSharedPointer<TrackedNode> qSp2 = qSp1;
        printLine(QStringLiteral("qSp2 共享指向同一对象"), (qSp1.data() == qSp2.data()) ? QStringLiteral("True (共享相同指针)") : QStringLiteral("False"));
        QWeakPointer<TrackedNode> qWp = qSp1;
        printLine(QStringLiteral("QWeakPointer 提升访问"), qWp.toStrongRef()->name());
    }
    printLine(QStringLiteral("离开 qSp2 作用域后 qSp1 状态"), qSp1.isNull() ? QStringLiteral("空") : QStringLiteral("仍有效"));

    std::cout << "\n  2. std::shared_ptr 引用计数演进 :" << std::endl;
    auto stdSp1 = std::make_shared<TrackedNode>(302, QStringLiteral("StdSharedSession"));
    printLine(QStringLiteral("stdSp1 初始引用计数"), QString::number(stdSp1.use_count()));
    {
        auto stdSp2 = stdSp1;
        printLine(QStringLiteral("stdSp2 共享后引用计数"), QString::number(stdSp1.use_count()));
        std::weak_ptr<TrackedNode> stdWp = stdSp1;
        printLine(QStringLiteral("std::weak_ptr lock() 访问"), stdWp.lock()->name());
    }
    printLine(QStringLiteral("离开作用域后引用计数"), QString::number(stdSp1.use_count()));

    // 3. 循环引用打破测试
    std::cout << "\n  3. 弱指针打破相互强引用死锁测试 :" << std::endl;
    {
        auto nodeA = QSharedPointer<CyclicNode>::create(QStringLiteral("Node_A"));
        auto nodeB = QSharedPointer<CyclicNode>::create(QStringLiteral("Node_B"));

        nodeA->m_strongPartner = nodeB;
        nodeB->m_weakPartner = nodeA; // 弱引用防止死锁
        std::cout << "  作用域结束：弱指针正常允许双向关联对象析构 :" << std::endl;
    }
}

void SmartPointersDemo::testCustomImplicitSharingCOW()
{
    printTitle(QStringLiteral("[模块 4] QSharedDataPointer: 亲手实现具备 COW 隐式共享特性的自定义数据类"));

    // 1. 创建原始对象
    Employee emp1(1001, QStringLiteral("Alice"), {QStringLiteral("Qt5"), QStringLiteral("C++17"), QStringLiteral("CMake")});

    std::cout << "  1. 发生拷贝构造: Employee emp2 = emp1 (浅拷贝) :" << std::endl;
    Employee emp2 = emp1; // 仅拷贝指针，引用计数 +1

    printLine(QStringLiteral("emp1 内部底层数据地址"), QStringLiteral("0x%1").arg(reinterpret_cast<quintptr>(emp1.internalDataPointer()), 0, 16));
    printLine(QStringLiteral("emp2 内部底层数据地址"), QStringLiteral("0x%1").arg(reinterpret_cast<quintptr>(emp2.internalDataPointer()), 0, 16));
    printLine(QStringLiteral("浅拷贝后地址是否相同"), (emp1.internalDataPointer() == emp2.internalDataPointer()) ? QStringLiteral("完全相同 (共享内存，零额外开销)") : QStringLiteral("不同"));

    // 2. 触发写时复制 (Detach)
    std::cout << "\n  2. 修改 emp2 数据: emp2.addSkill(\"Python\") (自动触发写时分离 Detach) :" << std::endl;
    emp2.addSkill(QStringLiteral("Python"));
    emp2.setName(QStringLiteral("Alice_Modified"));

    printLine(QStringLiteral("修改后 emp1 底层数据地址"), QStringLiteral("0x%1").arg(reinterpret_cast<quintptr>(emp1.internalDataPointer()), 0, 16));
    printLine(QStringLiteral("修改后 emp2 底层数据地址"), QStringLiteral("0x%1 (已自动独立深拷贝！)").arg(reinterpret_cast<quintptr>(emp2.internalDataPointer()), 0, 16));

    printLine(QStringLiteral("emp1 技能列表 (保持原样)"), QStringList::fromVector(emp1.skills()).join(QStringLiteral(", ")));
    printLine(QStringLiteral("emp2 技能列表 (独立追加)"), QStringList::fromVector(emp2.skills()).join(QStringLiteral(", ")));
}

void SmartPointersDemo::testSmartPointerSelectionGuide()
{
    printTitle(QStringLiteral("[模块 5] Qt 智能指针 vs STL 智能指针全维度决策矩阵"));

    std::cout << "  【Qt 与 C++ STL 智能指针全方位对比决策矩阵】:\n"
              << "  ┌───────────────────────┬───────────────────────────────┬────────────────────────────────────────────────────────┐\n"
              << "  │ 智能指针类型          │ 对应 STL 替代方案             │ 核心优势、特性差异与典型选型场景                       │\n"
              << "  ├───────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤\n"
              << "  │ QPointer<T>           │ 无（STL 无直接对等物）        │ 专属 QObject，对象销毁自动归零为 nullptr，防野指针神器 │\n"
              << "  │ QScopedPointer<T>     │ std::unique_ptr<T> (推荐)     │ std::unique_ptr 具备移动语义，现代 C++ 开发首选        │\n"
              << "  │ QSharedPointer<T>     │ std::shared_ptr<T>            │ 两者功能对等，涉及 QVariant / Qt元对象交互用 Qt 版     │\n"
              << "  │ QWeakPointer<T>       │ std::weak_ptr<T>              │ 弱引用观察者，配合 SharedPointer 打破循环引用死锁      │\n"
              << "  │ QSharedDataPointer<T> │ 无（需手写 COW 逻辑）         │ 打造高性能隐式共享值对象类的唯一官方标准方案           │\n"
              << "  └───────────────────────┴───────────────────────────────┴────────────────────────────────────────────────────────┘\n"
              << std::endl;

    printLine(QStringLiteral("黄金架构决策准则"), QStringLiteral("1. 监视 QObject 生命周期必选 QPointer；2. 独占资源首选 std::unique_ptr；3. 业务值实体类选 QSharedDataPointer。"));
}
