#pragma once

#include <QString>
#include <QVector>
#include <QList>
#include <QMap>
#include <QHash>
#include <QSet>

// 演示前向声明：在头文件中只需前向声明类，无需包含其完整头文件
class TaskItem;
class ForwardDeclaredItem; // 【前向声明不完整类型】

// 宿主管理类：演示头文件中持有 QVector<ForwardDeclaredItem> 值类型容器
class TaskManagerWithIncompleteType
{
public:
    TaskManagerWithIncompleteType();
    ~TaskManagerWithIncompleteType(); // 关键点：析构函数必须在 .cpp 中实现！

    void addItem(int id, const QString &name);
    int count() const;
    void printAll() const;

private:
    // 即使是值类型 QVector，由于 Qt 容器底层基于 d-pointer 共享数据指针，
    // 其对象自身尺寸固定（sizeof(void*) = 8字节），完全支持不完整类型前向声明！
    QVector<ForwardDeclaredItem> m_items;
};

class ContainerDemo
{
public:
    // 1. QVector 与 QList 连续内存、预分配 reserve 性能对比
    static void testSequenceContainers();

    // 2. QMap (红黑树有序) vs QHash (哈希表无序) 性能与自定义 Key
    static void testAssociativeContainers();

    // 3. QSet 集合运算 (交集/并集/差集) 与高速去重
    static void testSetOperations();

    // 4. 迭代器全解析 (STL 风格 vs Java 风格 vs 范围 for)
    static void testIterators();

    // 5. 隐式数据共享 (Implicit Sharing / Copy-On-Write) 与 Detach 避坑
    static void testImplicitSharing();

    // 6. for 循环的 6 种经典写法对比与选型指南
    static void testForLoopStyles();

    // 7. qDeleteAll 与常用 Qt 容器算法/宏实战
    static void testQtContainerMacrosAndAlgorithms();

    // 8. 容器类型与元素类的前向声明 (Forward Declaration) 规范与避坑
    static void testForwardDeclarations();
};
