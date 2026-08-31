#pragma once

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <memory>

namespace Ui {
class MainWindow;
}

/**
 * @brief QTreeWidget 深度实战与避坑指南演示窗口
 * 
 * 核心功能与避坑点演示：
 * 1. 多级树形结构与 CRUD 增删改查：addTopLevelItem, addChild, takeChild (内存安全释放), clear
 * 2. 父子级联智能三态勾选 (Qt::PartiallyChecked) 与信号防死循环递归 (blockSignals)
 * 3. 动态多级右键菜单 (Top-Level 分支菜单 vs Leaf 技能菜单 vs 空白处全局菜单)
 * 4. 树形结构递归模糊过滤与命中分支自动展开 (setExpanded & setHidden)
 * 5. Viewport 坐标换算避坑：treeWidget->viewport()->mapToGlobal(pos)
 * 6. 隐藏元数据绑定 (Qt::UserRole) 与多列样式定制
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // CRUD 管理
    void onAddTopLevelClicked();
    void onAddChildClicked();
    void onUpgradeClicked();
    void onDeleteNodeClicked();

    // 过滤与折叠展开
    void onFilterTextChanged(const QString &keyword);
    void onExpandAllClicked();
    void onCollapseAllClicked();
    void onSortNameClicked();

    // 信号交互
    void onItemChanged(QTreeWidgetItem *item, int column);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void onCustomContextMenu(const QPoint &pos);

private:
    void initSampleTalentTree();
    QTreeWidgetItem *createSkillNode(const QString &name, const QString &type, int rank, int maxRank,
                                     const QString &cost, const QString &status, const QString &iconText);

    // 父子级联勾选核心辅助函数
    void updateChildrenCheckState(QTreeWidgetItem *parentItem, Qt::CheckState state);
    void updateParentCheckState(QTreeWidgetItem *childItem);

    // 递归过滤辅助函数
    bool filterNodeRecursive(QTreeWidgetItem *item, const QString &keyword);

    void appendLog(const QString &category, const QString &message);

    std::unique_ptr<Ui::MainWindow> ui;
    bool m_isUpdatingCheckState{false}; // 标志位辅助防递归
    bool m_sortAsc{true};
};
