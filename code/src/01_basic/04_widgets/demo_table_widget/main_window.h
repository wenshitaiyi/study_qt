#pragma once

#include <QMainWindow>
#include <QTableWidgetItem>
#include <memory>

namespace Ui {
class MainWindow;
}

/**
 * @brief QTableWidget 深度实战与避坑指南演示窗口
 * 
 * 核心功能与避坑点演示：
 * 1. 行选 (SelectRows)、列选 (SelectColumns)、单元格选 (SelectItems) 自由切换
 * 2. 编辑触发策略：双击 (DoubleClicked)、单击选中 (SelectedClicked)、禁用编辑 (NoEditTriggers)
 * 3. 表头列宽模式：Stretch, ResizeToContents, Interactive
 * 4. 真正数值科学排序避坑：setData(Qt::DisplayRole, intValue) 避免字典序排序 Bug
 * 5. 单元格嵌入自定义交互控件 (setCellWidget)：进度条 (QProgressBar) 与多功能按钮组
 * 6. 大批量插入性能优化与防卡顿刷新：setUpdatesEnabled(false/true)
 * 7. Viewport 视口坐标与全方位右键菜单 (Row Menu vs Header Menu vs Empty Menu)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // 表格配置控制
    void onSelectionBehaviorChanged(int index);
    void onEditTriggerChanged(int index);
    void onHeaderModeChanged(int index);
    void onFilterTextChanged(const QString &keyword);

    // CRUD 与性能压测
    void onAddHeroClicked();
    void onInsertHeroClicked();
    void onDeleteSelectedClicked();
    void onBatchBenchmarkClicked();
    void onResetTableClicked();

    // 交互事件监控
    void onCellClicked(int row, int col);
    void onCellDoubleClicked(int row, int col);
    void onCellChanged(int row, int col);
    void onCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void onItemSelectionChanged();
    void onHeaderSectionClicked(int logicalIndex);
    void onCustomContextMenu(const QPoint &pos);

private:
    void initSampleHeroLeaderboard();
    void insertHeroRow(int row, int rank, const QString &name, const QString &heroClass,
                       int power, int winRate, int level, const QString &guild);
    QWidget *createActionCellWidget(int row, const QString &heroName);
    QWidget *createWinRateCellWidget(int winRate);

    void appendLog(const QString &category, const QString &message);

    std::unique_ptr<Ui::MainWindow> ui;
    int m_heroCounter{100};
};
