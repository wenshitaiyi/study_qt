#pragma once

#include <QObject>
#include <QPointer>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QSharedDataPointer>
#include <QSharedData>
#include <QString>
#include <QVector>

#include <memory> // C++ 标准库智能指针

// ============================================================================
// 演示类 1: 普通 QObject 派生对象 (用于测试 QPointer 与析构追踪)
// ============================================================================
class TrackedNode : public QObject
{
    Q_OBJECT

public:
    explicit TrackedNode(int id, const QString &name, QObject *parent = nullptr);
    ~TrackedNode() override;

    int id() const { return m_id; }
    QString name() const { return m_name; }

private:
    int m_id;
    QString m_name;
};

// ============================================================================
// 演示类 2: 手写具备“隐式数据共享 (写时复制 COW)”的自定义数据类
// ============================================================================
class EmployeeData : public QSharedData
{
public:
    EmployeeData() = default;
    EmployeeData(int id, const QString &name, const QVector<QString> &skills)
        : m_id(id), m_name(name), m_skills(skills)
    {
    }
    EmployeeData(const EmployeeData &other) = default;
    ~EmployeeData() = default;

    int m_id{0};
    QString m_name;
    QVector<QString> m_skills;
};

class Employee
{
public:
    Employee();
    Employee(int id, const QString &name, const QVector<QString> &skills);
    Employee(const Employee &other);
    Employee &operator=(const Employee &other);
    ~Employee();

    // 只读访问器：不触发分离 (detach)
    int id() const;
    QString name() const;
    QVector<QString> skills() const;
    const EmployeeData *internalDataPointer() const;

    // 修改器：调用 d.detach() 自动触发写时复制深拷贝！
    void setId(int id);
    void setName(const QString &name);
    void addSkill(const QString &skill);

private:
    QSharedDataPointer<EmployeeData> d;
};

// ============================================================================
// 演示类 3: 循环引用测试节点
// ============================================================================
class CyclicNode
{
public:
    explicit CyclicNode(const QString &name);
    ~CyclicNode();

    QString m_name;
    QSharedPointer<CyclicNode> m_strongPartner;
    QWeakPointer<CyclicNode> m_weakPartner; // 打破循环引用的弱指针
};

// ============================================================================
// SmartPointersDemo 主测试套件
// ============================================================================
class SmartPointersDemo
{
public:
    // 1. QPointer vs std::weak_ptr: QObject 自动置空弱引用的不可替代性
    static void testQPointerForQObject();

    // 2. QScopedPointer vs std::unique_ptr (独占所有权、移动语义与自定义 Deleter)
    static void testScopedVsUniquePointer();

    // 3. QSharedPointer / QWeakPointer vs std::shared_ptr / std::weak_ptr (共享与循环引用)
    static void testSharedAndWeakPointersComparison();

    // 4. QSharedDataPointer: 亲手实现具备 COW 隐式共享特性的自定义数据类
    static void testCustomImplicitSharingCOW();

    // 5. Qt 智能指针 vs STL 智能指针全维度对比决策矩阵
    static void testSmartPointerSelectionGuide();
};
