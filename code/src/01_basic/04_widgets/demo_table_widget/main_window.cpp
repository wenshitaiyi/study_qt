#include "main_window.h"
#include "ui_main_window.h"

#include <QDateTime>
#include <QMenu>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    // 初始列宽模式：默认均分自适应拉伸
    ui->tableRank->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableRank->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents); // 操作列紧凑

    initSampleHeroLeaderboard();

    // 绑定表格交互控制
    connect(ui->comboSelBehavior, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onSelectionBehaviorChanged);
    connect(ui->comboEditTrigger, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onEditTriggerChanged);
    connect(ui->comboHeaderMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onHeaderModeChanged);
    connect(ui->editFilter, &QLineEdit::textChanged, this, &MainWindow::onFilterTextChanged);

    // 绑定 CRUD 与压测
    connect(ui->btnAddHero, &QPushButton::clicked, this, &MainWindow::onAddHeroClicked);
    connect(ui->btnInsertHero, &QPushButton::clicked, this, &MainWindow::onInsertHeroClicked);
    connect(ui->btnDeleteSelected, &QPushButton::clicked, this, &MainWindow::onDeleteSelectedClicked);
    connect(ui->btnBatchBenchmark, &QPushButton::clicked, this, &MainWindow::onBatchBenchmarkClicked);
    connect(ui->btnResetTable, &QPushButton::clicked, this, &MainWindow::onResetTableClicked);

    // 绑定表格事件与信号
    connect(ui->tableRank, &QTableWidget::cellClicked, this, &MainWindow::onCellClicked);
    connect(ui->tableRank, &QTableWidget::cellDoubleClicked, this, &MainWindow::onCellDoubleClicked);
    connect(ui->tableRank, &QTableWidget::cellChanged, this, &MainWindow::onCellChanged);
    connect(ui->tableRank, &QTableWidget::currentCellChanged, this, &MainWindow::onCurrentCellChanged);
    connect(ui->tableRank, &QTableWidget::itemSelectionChanged, this, &MainWindow::onItemSelectionChanged);
    connect(ui->tableRank->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onHeaderSectionClicked);
    connect(ui->tableRank, &QTableWidget::customContextMenuRequested, this, &MainWindow::onCustomContextMenu);

    connect(ui->btnClearLog, &QPushButton::clicked, ui->textLog, &QPlainTextEdit::clear);

    appendLog(QStringLiteral("系统"), QStringLiteral("全服英雄战力排行榜 QTableWidget 初始化就绪。"));
}

MainWindow::~MainWindow() = default;

void MainWindow::appendLog(const QString &category, const QString &message)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    QString line = QStringLiteral("[%1] [%2] %3").arg(timeStr).arg(category.leftJustified(10, ' ')).arg(message);
    ui->textLog->appendPlainText(line);
}

// ============================================================================
// 单元格嵌入控件 (setCellWidget)
// ============================================================================
QWidget *MainWindow::createWinRateCellWidget(int winRate)
{
    auto *container = new QWidget(this);
    auto *layout = new QHBoxLayout(container);
    layout->setContentsMargins(4, 2, 4, 2);

    auto *pbar = new QProgressBar(container);
    pbar->setRange(0, 100);
    pbar->setValue(winRate);
    pbar->setAlignment(Qt::AlignCenter);
    pbar->setFormat(QStringLiteral("%1 %").arg(winRate));

    // 根据胜率着色
    if (winRate >= 75) {
        pbar->setStyleSheet(QStringLiteral("QProgressBar::chunk { background-color: #e67e22; }")); // 橙金
    } else if (winRate >= 55) {
        pbar->setStyleSheet(QStringLiteral("QProgressBar::chunk { background-color: #2ecc71; }")); // 绿色
    } else {
        pbar->setStyleSheet(QStringLiteral("QProgressBar::chunk { background-color: #95a5a6; }")); // 灰色
    }

    layout->addWidget(pbar);
    return container;
}

QWidget *MainWindow::createActionCellWidget(int row, const QString &heroName)
{
    auto *container = new QWidget(this);
    auto *layout = new QHBoxLayout(container);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(4);

    auto *btnFight = new QPushButton(QStringLiteral("⚔️ 挑战"), container);
    auto *btnInfo  = new QPushButton(QStringLiteral("🔍 查房"), container);
    btnFight->setStyleSheet(QStringLiteral("padding: 2px 6px; font-size: 11px;"));
    btnInfo->setStyleSheet(QStringLiteral("padding: 2px 6px; font-size: 11px;"));

    connect(btnFight, &QPushButton::clicked, this, [this, heroName]() {
        appendLog(QStringLiteral("快捷挑战"), QStringLiteral("向英雄 [%1] 发起天梯对决挑战！").arg(heroName));
        QMessageBox::information(this, QStringLiteral("对决挑战"), QStringLiteral("已向全服天梯英雄【%1】发出战书！").arg(heroName));
    });

    connect(btnInfo, &QPushButton::clicked, this, [this, row, heroName]() {
        appendLog(QStringLiteral("英雄查房"), QStringLiteral("查看第 %1 行英雄 [%2] 详细战报面板").arg(row + 1).arg(heroName));
    });

    layout->addWidget(btnFight);
    layout->addWidget(btnInfo);
    return container;
}

// ============================================================================
// 单行数据构造 (真正的数值科学排序避坑)
// ============================================================================
void MainWindow::insertHeroRow(int row, int rank, const QString &name, const QString &heroClass,
                               int power, int winRate, int level, const QString &guild)
{
    ui->tableRank->insertRow(row);

    // ========================================================================
    // 【核心避坑指南 1：QTableWidgetItem 科学数值排序】
    //  若直接调用 item->setText("1000")，Qt 默认按字符串字典序比较，导致 "100" < "20" < "5"！
    //  解决方案：调用 item->setData(Qt::DisplayRole, intVal)，存入 int 型 QVariant，
    //  QTableWidget 在点击表头排序时即可自动采用真正的数值大小排序！
    // ========================================================================
    
    // 0: 排名 (数值)
    auto *itemRank = new QTableWidgetItem();
    itemRank->setData(Qt::DisplayRole, rank);
    itemRank->setTextAlignment(Qt::AlignCenter);
    if (rank == 1) {
        itemRank->setForeground(QBrush(QColor(230, 126, 34))); // 金牌榜首
        itemRank->setFont(QFont(QStringLiteral("Microsoft YaHei"), 9, QFont::Bold));
    } else if (rank == 2) {
        itemRank->setForeground(QBrush(QColor(149, 165, 166))); // 银牌
    } else if (rank == 3) {
        itemRank->setForeground(QBrush(QColor(211, 84, 0)));   // 铜牌
    }
    ui->tableRank->setItem(row, 0, itemRank);

    // 1: 姓名 (支持双击就地编辑)
    auto *itemName = new QTableWidgetItem(name);
    itemName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
    ui->tableRank->setItem(row, 1, itemName);

    // 2: 职业
    auto *itemClass = new QTableWidgetItem(heroClass);
    itemClass->setTextAlignment(Qt::AlignCenter);
    itemClass->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled); // 只读
    ui->tableRank->setItem(row, 2, itemClass);

    // 3: 战斗力 (数值类型存入 DisplayRole)
    auto *itemPower = new QTableWidgetItem();
    itemPower->setData(Qt::DisplayRole, power);
    itemPower->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    itemPower->setForeground(QBrush(QColor(155, 89, 182))); // 紫色高亮战力
    itemPower->setFont(QFont(QStringLiteral("Consolas"), 9, QFont::Bold));
    ui->tableRank->setItem(row, 3, itemPower);

    // 4: 胜率仪表 (嵌入 QProgressBar 控件)
    // 注意：先放置一个带数值的占位 item 用于排序，再覆盖 cellWidget
    auto *itemWinRate = new QTableWidgetItem();
    itemWinRate->setData(Qt::DisplayRole, winRate);
    ui->tableRank->setItem(row, 4, itemWinRate);
    ui->tableRank->setCellWidget(row, 4, createWinRateCellWidget(winRate));

    // 5: 等级 (数值类型存入 DisplayRole)
    auto *itemLevel = new QTableWidgetItem();
    itemLevel->setData(Qt::DisplayRole, level);
    itemLevel->setTextAlignment(Qt::AlignCenter);
    ui->tableRank->setItem(row, 5, itemLevel);

    // 6: 公会
    auto *itemGuild = new QTableWidgetItem(guild);
    itemGuild->setTextAlignment(Qt::AlignCenter);
    ui->tableRank->setItem(row, 6, itemGuild);

    // 7: 快捷操作 (嵌入按钮组)
    ui->tableRank->setCellWidget(row, 7, createActionCellWidget(row, name));
}

void MainWindow::initSampleHeroLeaderboard()
{
    ui->tableRank->setRowCount(0);

    const struct HeroSample {
        int rank;
        QString name;
        QString heroClass;
        int power;
        int winRate;
        int level;
        QString guild;
    } samples[] = {
        { 1,  QStringLiteral("阿尔萨斯·巫妖王"), QStringLiteral("🗡️ 死亡骑士"), 1280000, 92, 90, QStringLiteral("天灾军团 (Scourge)") },
        { 2,  QStringLiteral("伊利丹·怒风"),     QStringLiteral("🗡️ 恶魔猎手"), 1150000, 88, 89, QStringLiteral("伊利达雷 (Illidari)") },
        { 3,  QStringLiteral("吉安娜·普罗德摩尔"), QStringLiteral("🔮 奥术法皇"), 1080000, 85, 88, QStringLiteral("达拉然议会 (Kirin Tor)") },
        { 4,  QStringLiteral("瓦里安·乌瑞恩"),   QStringLiteral("⚔️ 武器战狂"),  995000, 78, 87, QStringLiteral("联盟至高卫队") },
        { 5,  QStringLiteral("萨尔·世界萨"),     QStringLiteral("⚡ 元素萨满"),  960000, 74, 86, QStringLiteral("大地之环 (Earthen Ring)") },
        { 6,  QStringLiteral("希尔瓦娜斯·风行者"), QStringLiteral("🏹 黑暗游侠"),  920000, 71, 85, QStringLiteral("被遗忘者 (Forsaken)") },
        { 7,  QStringLiteral("玛法里奥·怒风"),   QStringLiteral("🌿 丛林德鲁伊"), 890000, 68, 85, QStringLiteral("塞纳里奥议会") },
        { 8,  QStringLiteral("安度因·乌瑞恩"),   QStringLiteral("🛡️ 神圣戒律牧"), 840000, 65, 84, QStringLiteral("暴风城王室") }
    };

    ui->tableRank->setUpdatesEnabled(false);
    for (int i = 0; i < 8; ++i) {
        const auto &s = samples[i];
        insertHeroRow(i, s.rank, s.name, s.heroClass, s.power, s.winRate, s.level, s.guild);
    }
    ui->tableRank->setUpdatesEnabled(true);
}

// ============================================================================
// 表格交互行为、编辑触发与表头模式控制
// ============================================================================
void MainWindow::onSelectionBehaviorChanged(int index)
{
    switch (index) {
    case 0:
        ui->tableRank->setSelectionBehavior(QAbstractItemView::SelectRows);
        appendLog(QStringLiteral("选择行为"), QStringLiteral("切换为：整行选择 (SelectRows)"));
        break;
    case 1:
        ui->tableRank->setSelectionBehavior(QAbstractItemView::SelectColumns);
        appendLog(QStringLiteral("选择行为"), QStringLiteral("切换为：整列选择 (SelectColumns)"));
        break;
    case 2:
        ui->tableRank->setSelectionBehavior(QAbstractItemView::SelectItems);
        appendLog(QStringLiteral("选择行为"), QStringLiteral("切换为：单个单元格选择 (SelectItems)"));
        break;
    }
}

void MainWindow::onEditTriggerChanged(int index)
{
    switch (index) {
    case 0:
        ui->tableRank->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
        appendLog(QStringLiteral("编辑触发"), QStringLiteral("切换为：双击进入编辑 (DoubleClicked)"));
        break;
    case 1:
        ui->tableRank->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
        appendLog(QStringLiteral("编辑触发"), QStringLiteral("切换为：选中后单击进入编辑 (SelectedClicked)"));
        break;
    case 2:
        ui->tableRank->setEditTriggers(QAbstractItemView::NoEditTriggers);
        appendLog(QStringLiteral("编辑触发"), QStringLiteral("切换为：完全禁止就地编辑 (NoEditTriggers)"));
        break;
    }
}

void MainWindow::onHeaderModeChanged(int index)
{
    switch (index) {
    case 0:
        ui->tableRank->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableRank->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
        appendLog(QStringLiteral("表头模式"), QStringLiteral("列宽模式切换为：自适应均分拉伸 (Stretch)"));
        break;
    case 1:
        ui->tableRank->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        appendLog(QStringLiteral("表头模式"), QStringLiteral("列宽模式切换为：根据内容宽度自适应 (ResizeToContents)"));
        break;
    case 2:
        ui->tableRank->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        appendLog(QStringLiteral("表头模式"), QStringLiteral("列宽模式切换为：鼠标交互自由拖拽 (Interactive)"));
        break;
    }
}

void MainWindow::onFilterTextChanged(const QString &keyword)
{
    QString kw = keyword.trimmed();
    int visibleRows = 0;

    ui->tableRank->setUpdatesEnabled(false);
    for (int r = 0; r < ui->tableRank->rowCount(); ++r) {
        bool match = false;
        if (kw.isEmpty()) {
            match = true;
        } else {
            // 匹配姓名(列1)、职业(列2)、公会(列6)
            for (int col : {1, 2, 6}) {
                auto *item = ui->tableRank->item(r, col);
                if (item && item->text().contains(kw, Qt::CaseInsensitive)) {
                    match = true;
                    break;
                }
            }
        }
        ui->tableRank->setRowHidden(r, !match);
        if (match) ++visibleRows;
    }
    ui->tableRank->setUpdatesEnabled(true);

    appendLog(QStringLiteral("表格检索"), QStringLiteral("检索关键词: [%1] -> 匹配行数: %2 / %3").arg(kw).arg(visibleRows).arg(ui->tableRank->rowCount()));
}

// ============================================================================
// CRUD 与性能优化 (setUpdatesEnabled 防卡顿)
// ============================================================================
void MainWindow::onAddHeroClicked()
{
    QString name = ui->editHeroName->text().trimmed();
    if (name.isEmpty()) return;

    int row = ui->tableRank->rowCount();
    int rank = row + 1;
    QString heroClass = ui->comboHeroClass->currentText();
    int power = ui->spinHeroPower->value();
    int winRate = 60 + (power % 35);
    int level = 80 + (power % 10);

    insertHeroRow(row, rank, name, heroClass, power, winRate, level, QStringLiteral("自由散人军团"));
    ui->tableRank->scrollToBottom();

    appendLog(QStringLiteral("添加英雄"), QStringLiteral("成功追加天梯英雄: %1 (战力: %2, 当前总行数: %3)").arg(name).arg(power).arg(ui->tableRank->rowCount()));
}

void MainWindow::onInsertHeroClicked()
{
    QString name = ui->editHeroName->text().trimmed();
    if (name.isEmpty()) return;

    int currentRow = ui->tableRank->currentRow();
    if (currentRow < 0) currentRow = 0;

    int rank = currentRow + 1;
    insertHeroRow(currentRow, rank, name, ui->comboHeroClass->currentText(),
                  ui->spinHeroPower->value(), 75, 85, QStringLiteral("特邀空降公会"));

    appendLog(QStringLiteral("插入英雄"), QStringLiteral("在第 %1 行插入英雄: %2").arg(currentRow + 1).arg(name));
}

void MainWindow::onDeleteSelectedClicked()
{
    // ========================================================================
    // 【核心避坑指南 2：表格多行删除必须从大行号向小行号倒序删除】
    //  若正序删除，删除前面行会导致后面行的 row 索引动态发生偏移！
    // ========================================================================
    QList<QTableWidgetSelectionRange> ranges = ui->tableRank->selectedRanges();
    if (ranges.isEmpty() && ui->tableRank->currentRow() < 0) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请先选中要删除的英雄行！"));
        return;
    }

    QSet<int> rowsToDelete;
    for (const auto &range : ranges) {
        for (int r = range.topRow(); r <= range.bottomRow(); ++r) {
            rowsToDelete.insert(r);
        }
    }
    if (rowsToDelete.isEmpty() && ui->tableRank->currentRow() >= 0) {
        rowsToDelete.insert(ui->tableRank->currentRow());
    }

    QList<int> sortedRows = rowsToDelete.values();
    std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>()); // 降序

    for (int r : sortedRows) {
        ui->tableRank->removeRow(r);
    }

    appendLog(QStringLiteral("删除行"), QStringLiteral("成功倒序删除 %1 行记录").arg(sortedRows.size()));
}

void MainWindow::onBatchBenchmarkClicked()
{
    // ========================================================================
    // 【核心避坑指南 3：大批量插入防卡顿优化 (setUpdatesEnabled)】
    //  批量插入 100 行时，如果不关闭更新，每插入一行都会触发底层布局与重绘计算；
    //  调用 setUpdatesEnabled(false) 阻断绘制，全部插入后再开启，耗时由数百毫秒骤降至个位数毫秒！
    // ========================================================================
    QElapsedTimer timer;
    timer.start();

    ui->tableRank->setUpdatesEnabled(false);

    int startRow = ui->tableRank->rowCount();
    for (int i = 0; i < 100; ++i) {
        int r = startRow + i;
        QString name = QStringLiteral("测试战将 #%1").arg(m_heroCounter++);
        int power = 500000 + (qrand() % 500000);
        int winRate = 40 + (qrand() % 55);
        int level = 70 + (qrand() % 20);

        insertHeroRow(r, r + 1, name, QStringLiteral("⚔️ 狂战士"), power, winRate, level, QStringLiteral("百战精英团"));
    }

    ui->tableRank->setUpdatesEnabled(true);
    qint64 elapsed = timer.elapsed();

    appendLog(QStringLiteral("批量压测"), QStringLiteral("使用 setUpdatesEnabled(false) 极速插入 100 行数据完成，总耗时: %1 ms (行数: %2)").arg(elapsed).arg(ui->tableRank->rowCount()));
    QMessageBox::information(this, QStringLiteral("压测完成"), QStringLiteral("成功批量插入 100 行数据！\n总耗时仅: %1 毫秒（已启用 setUpdatesEnabled 防卡顿机制）。").arg(elapsed));
}

void MainWindow::onResetTableClicked()
{
    initSampleHeroLeaderboard();
    appendLog(QStringLiteral("重置表格"), QStringLiteral("已恢复初始天梯排行榜 8 位英雄榜单。"));
}

// ============================================================================
// 交互事件监控 (单击、双击、变动、表头点击)
// ============================================================================
void MainWindow::onCellClicked(int row, int col)
{
    auto *item = ui->tableRank->item(row, col);
    QString text = item ? item->text() : QStringLiteral("(嵌控件/无文本)");
    appendLog(QStringLiteral("cellClicked"), QStringLiteral("单击单元格 [行:%1, 列:%2] -> 文本: %3").arg(row + 1).arg(col + 1).arg(text));
}

void MainWindow::onCellDoubleClicked(int row, int col)
{
    auto *item = ui->tableRank->item(row, col);
    QString text = item ? item->text() : QStringLiteral("(无文本)");
    appendLog(QStringLiteral("双击编辑"), QStringLiteral("双击单元格 [行:%1, 列:%2] -> 文本: %3").arg(row + 1).arg(col + 1).arg(text));
}

void MainWindow::onCellChanged(int row, int col)
{
    auto *item = ui->tableRank->item(row, col);
    if (item) {
        appendLog(QStringLiteral("cellChanged"), QStringLiteral("单元格数据被修改 [行:%1, 列:%2] -> 新值: %3").arg(row + 1).arg(col + 1).arg(item->text()));
    }
}

void MainWindow::onCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    appendLog(QStringLiteral("焦点移动"), QStringLiteral("光标焦点切换: [%1, %2] -> [%3, %4]")
              .arg(previousRow + 1).arg(previousColumn + 1)
              .arg(currentRow + 1).arg(currentColumn + 1));
}

void MainWindow::onItemSelectionChanged()
{
    int count = ui->tableRank->selectedItems().size();
    appendLog(QStringLiteral("选中项变更"), QStringLiteral("当前被选中单元格总数: %1").arg(count));
}

void MainWindow::onHeaderSectionClicked(int logicalIndex)
{
    QString colName = ui->tableRank->horizontalHeaderItem(logicalIndex)->text();
    appendLog(QStringLiteral("点击表头"), QStringLiteral("点击第 %1 列表头: [%2] -> 触发排序").arg(logicalIndex + 1).arg(colName));
}

// ============================================================================
// 右键上下文菜单与 Viewport 绝对坐标换算
// ============================================================================
void MainWindow::onCustomContextMenu(const QPoint &pos)
{
    QTableWidgetItem *item = ui->tableRank->itemAt(pos);
    QPoint globalPos = ui->tableRank->viewport()->mapToGlobal(pos);

    if (item) {
        int row = item->row();
        auto *nameItem = ui->tableRank->item(row, 1);
        QString heroName = nameItem ? nameItem->text() : QStringLiteral("未知英雄");

        QMenu menu(this);
        QAction *actFight    = menu.addAction(QStringLiteral("⚔️ 向该英雄发起决斗挑战"));
        QAction *actRename   = menu.addAction(QStringLiteral("✏️ 重命名该英雄 (就地编辑)"));
        QAction *actPowerUp  = menu.addAction(QStringLiteral("✨ 战力突破 (+50000)"));
        menu.addSeparator();
        QAction *actDelete   = menu.addAction(QStringLiteral("🗑 剔除该英雄天梯记录"));

        QAction *sel = menu.exec(globalPos);
        if (sel == actFight) {
            QMessageBox::information(this, QStringLiteral("天梯决斗"), QStringLiteral("已向【%1】发起天梯擂台对决！").arg(heroName));
        } else if (sel == actRename) {
            ui->tableRank->editItem(nameItem);
        } else if (sel == actPowerUp) {
            auto *powerItem = ui->tableRank->item(row, 3);
            if (powerItem) {
                int oldPower = powerItem->data(Qt::DisplayRole).toInt();
                powerItem->setData(Qt::DisplayRole, oldPower + 50000);
                appendLog(QStringLiteral("战力突破"), QStringLiteral("英雄 [%1] 战力提升至: %2").arg(heroName).arg(oldPower + 50000));
            }
        } else if (sel == actDelete) {
            ui->tableRank->removeRow(row);
            appendLog(QStringLiteral("剔除"), QStringLiteral("成功将英雄 [%1] 移出天梯榜单。").arg(heroName));
        }
    } else {
        QMenu menu(this);
        QAction *actAdd     = menu.addAction(QStringLiteral("➕ 追加测试英雄..."));
        QAction *actReset   = menu.addAction(QStringLiteral("🔄 恢复初始天梯榜"));
        QAction *actBench   = menu.addAction(QStringLiteral("⚡ 极速插入100条 (性能压测)"));
        menu.addSeparator();
        QAction *actClear   = menu.addAction(QStringLiteral("🗑 清空天梯榜单"));

        QAction *sel = menu.exec(globalPos);
        if (sel == actAdd) {
            onAddHeroClicked();
        } else if (sel == actReset) {
            onResetTableClicked();
        } else if (sel == actBench) {
            onBatchBenchmarkClicked();
        } else if (sel == actClear) {
            ui->tableRank->setRowCount(0);
            appendLog(QStringLiteral("清空"), QStringLiteral("天梯榜单已一键清空。"));
        }
    }
}
