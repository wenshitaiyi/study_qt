#pragma once

#include <QMainWindow>
#include <QListWidgetItem>
#include <memory>

namespace Ui {
class MainWindow;
}

/**
 * @brief QListWidget 深度实战与避坑指南演示窗口
 * 
 * 核心功能与避坑点演示：
 * 1. CRUD 增删改查：addItem, insertItem, takeItem (内存安全释放), clear
 * 2. Item 视觉与属性定制：双击就地编辑 (Qt::ItemIsEditable)、复选框勾选 (Qt::ItemIsUserCheckable)、品质高亮背景、64x64 装备图标加载、隐藏元数据绑定 (Qt::UserRole)
 * 3. 视图模式切换：ListMode (常规列表) 与 IconMode (大图标网格)
 * 4. 实时模糊搜索过滤：遍历动态调用 setHidden(!matched)
 * 5. 右键上下文菜单与 Viewport 坐标避坑：
 *    - 点击 Item：弹出装备专属操作菜单（查看属性、重命名、强化、单件出售、销毁）
 *    - 点击空白处：弹出全局背包管理菜单（新增装备、重置背包、全选/全不选、按价值整理、一键清空）
 * 6. 信号死循环防递归：blockSignals(true/false)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // CRUD 槽函数
    void onAddItemClicked();
    void onInsertItemClicked();
    void onDeleteSelectedClicked();
    void onBatchSellClicked();
    void onClearAllClicked();
    void onResetSampleItemsClicked();

    // 过滤、视图与排序
    void onFilterTextChanged(const QString &keyword);
    void onViewModeChanged(int index);
    void onSortAscClicked();
    void onSortDescClicked();
    void onSortByGoldValue();

    // 勾选操作
    void onCheckAllItems(bool check);

    // QListWidget 交互信号与右键菜单
    void onItemClicked(QListWidgetItem *item);
    void onItemDoubleClicked(QListWidgetItem *item);
    void onItemChanged(QListWidgetItem *item);
    void onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void onCustomContextMenu(const QPoint &pos);

private:
    void initSampleItems();
    QListWidgetItem *createInventoryItem(const QString &name, const QString &rarity, int gold, int iconIndex = 1);
    void appendLog(const QString &category, const QString &message);

    std::unique_ptr<Ui::MainWindow> ui;
    int m_itemAutoId{1001};
    int m_iconCounter{1};
};
