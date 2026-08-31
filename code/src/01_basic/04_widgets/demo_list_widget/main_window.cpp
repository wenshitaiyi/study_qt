#include "main_window.h"
#include "ui_main_window.h"
#include "icon_helper.h"

#include <QDateTime>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    // 确保本地生成 50 个 64x64 图标
    IconHelper::ensureIconsExist();

    initSampleItems();

    // 绑定 CRUD 操作
    connect(ui->btnAddItem, &QPushButton::clicked, this, &MainWindow::onAddItemClicked);
    connect(ui->btnInsertItem, &QPushButton::clicked, this, &MainWindow::onInsertItemClicked);
    connect(ui->btnDeleteSelected, &QPushButton::clicked, this, &MainWindow::onDeleteSelectedClicked);
    connect(ui->btnBatchSell, &QPushButton::clicked, this, &MainWindow::onBatchSellClicked);
    connect(ui->btnClearAll, &QPushButton::clicked, this, &MainWindow::onClearAllClicked);

    // 绑定搜索、视图模式与排序
    connect(ui->editFilter, &QLineEdit::textChanged, this, &MainWindow::onFilterTextChanged);
    connect(ui->comboViewMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onViewModeChanged);
    connect(ui->btnSortAsc, &QPushButton::clicked, this, &MainWindow::onSortAscClicked);
    connect(ui->btnSortDesc, &QPushButton::clicked, this, &MainWindow::onSortDescClicked);

    // 绑定 QListWidget 原生信号
    connect(ui->listInventory, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);
    connect(ui->listInventory, &QListWidget::itemDoubleClicked, this, &MainWindow::onItemDoubleClicked);
    connect(ui->listInventory, &QListWidget::itemChanged, this, &MainWindow::onItemChanged);
    connect(ui->listInventory, &QListWidget::currentItemChanged, this, &MainWindow::onCurrentItemChanged);
    connect(ui->listInventory, &QListWidget::customContextMenuRequested, this, &MainWindow::onCustomContextMenu);

    connect(ui->btnClearLog, &QPushButton::clicked, ui->textLog, &QPlainTextEdit::clear);

    appendLog(QStringLiteral("系统"), QStringLiteral("游戏背包 QListWidget 初始化就绪 (已加载 50 个 64x64 本地图标资源)。"));
}

MainWindow::~MainWindow() = default;

void MainWindow::appendLog(const QString &category, const QString &message)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    QString line = QStringLiteral("[%1] [%2] %3").arg(timeStr).arg(category.leftJustified(10, ' ')).arg(message);
    ui->textLog->appendPlainText(line);
}

// ============================================================================
// Item 构造与元数据绑定 (Flags, Icons, UserRole)
// ============================================================================
QListWidgetItem *MainWindow::createInventoryItem(const QString &name, const QString &rarity, int gold, int iconIndex)
{
    auto *item = new QListWidgetItem();
    item->setText(name);

    // 绑定 64x64 图标
    item->setIcon(IconHelper::getIcon(iconIndex));

    // Item Flags 标志位设置
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable |
                   Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled);

    item->setCheckState(Qt::Unchecked);

    // 绑定隐藏元数据 (Qt::UserRole)
    item->setData(Qt::UserRole + 1, m_itemAutoId);  // Item ID
    item->setData(Qt::UserRole + 2, rarity);        // 品质
    item->setData(Qt::UserRole + 3, gold);          // 金币
    item->setData(Qt::UserRole + 4, iconIndex);     // 图标编号

    // 根据品质配置前景色和背景气泡提示
    QString tooltip = QStringLiteral("【装备 ID: %1】\n品质: %2\n回收价值: %3 金币\n(提示: 双击重命名，右键查看上下文菜单)").arg(m_itemAutoId).arg(rarity).arg(gold);
    item->setToolTip(tooltip);

    if (rarity.contains(QStringLiteral("传说"))) {
        item->setForeground(QBrush(QColor(230, 126, 34))); // 金色
    } else if (rarity.contains(QStringLiteral("史诗"))) {
        item->setForeground(QBrush(QColor(155, 89, 182))); // 紫色
    } else if (rarity.contains(QStringLiteral("精良"))) {
        item->setForeground(QBrush(QColor(52, 152, 219)));  // 蓝色
    } else {
        item->setForeground(QBrush(QColor(46, 204, 113)));  // 绿色
    }

    ++m_itemAutoId;
    return item;
}

void MainWindow::initSampleItems()
{
    ui->listInventory->clear();

    const struct SampleItem {
        QString name;
        QString rarity;
        int gold;
        int iconIdx;
    } samples[] = {
        { QStringLiteral("灰烬使者 (Ashbringer)"),       QStringLiteral("🟡 传说 (Legendary)"), 88888, 1 },
        { QStringLiteral("埃辛诺斯壁垒 (Bulwark)"),      QStringLiteral("🟣 史诗 (Epic)"),       36000, 2 },
        { QStringLiteral("索利达尔·群星之怒"),           QStringLiteral("🟡 传说 (Legendary)"), 99999, 3 },
        { QStringLiteral("达拉然高级传送卷轴"),          QStringLiteral("🔵 精良 (Rare)"),        1200, 9 },
        { QStringLiteral("特效强效生命药水 x20"),        QStringLiteral("🟢 普通 (Common)"),       500, 10 },
        { QStringLiteral("黑暗神殿至尊誓约之戒"),        QStringLiteral("🟣 史诗 (Epic)"),       45000, 7 },
        { QStringLiteral("远古奥术风暴聚焦法球"),        QStringLiteral("🔵 精良 (Rare)"),        8500, 5 },
        { QStringLiteral("迅捷疾风追猎者战靴"),          QStringLiteral("🟢 普通 (Common)"),       800, 12 }
    };

    for (const auto &s : samples) {
        auto *item = createInventoryItem(s.name, s.rarity, s.gold, s.iconIdx);
        ui->listInventory->addItem(item);
    }
}

// ============================================================================
// CRUD 增删改查实现
// ============================================================================
void MainWindow::onAddItemClicked()
{
    QString name = ui->editNewItemName->text().trimmed();
    if (name.isEmpty()) return;

    QString rarity = ui->comboRarity->currentText();
    int gold = ui->spinGold->value();

    m_iconCounter = (m_iconCounter % 50) + 1;
    auto *item = createInventoryItem(name, rarity, gold, m_iconCounter);
    ui->listInventory->addItem(item); // 追加到末尾

    appendLog(QStringLiteral("addItem"), QStringLiteral("成功追加装备: %1 (当前总数: %2)").arg(name).arg(ui->listInventory->count()));
}

void MainWindow::onInsertItemClicked()
{
    QString name = ui->editNewItemName->text().trimmed();
    if (name.isEmpty()) return;

    int currentRow = ui->listInventory->currentRow();
    if (currentRow < 0) currentRow = 0;

    m_iconCounter = (m_iconCounter % 50) + 1;
    auto *item = createInventoryItem(name, ui->comboRarity->currentText(), ui->spinGold->value(), m_iconCounter);
    ui->listInventory->insertItem(currentRow, item); // 插入指定行

    appendLog(QStringLiteral("insertItem"), QStringLiteral("在第 %1 行插入装备: %2").arg(currentRow).arg(name));
}

void MainWindow::onDeleteSelectedClicked()
{
    QList<QListWidgetItem *> selectedItems = ui->listInventory->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请先选中要删除的装备项！"));
        return;
    }

    for (auto *item : selectedItems) {
        int row = ui->listInventory->row(item);
        QListWidgetItem *taken = ui->listInventory->takeItem(row);
        QString itemName = taken->text();
        delete taken; // 显式释放内存！
        appendLog(QStringLiteral("takeItem"), QStringLiteral("安全移除并释放装备项: %1 (原行号: %2)").arg(itemName).arg(row));
    }
}

void MainWindow::onBatchSellClicked()
{
    int totalGold = 0;
    int soldCount = 0;

    for (int i = ui->listInventory->count() - 1; i >= 0; --i) {
        QListWidgetItem *item = ui->listInventory->item(i);
        if (item && item->checkState() == Qt::Checked) {
            int gold = item->data(Qt::UserRole + 3).toInt();
            totalGold += gold;
            ++soldCount;

            QListWidgetItem *taken = ui->listInventory->takeItem(i);
            delete taken;
        }
    }

    if (soldCount > 0) {
        appendLog(QStringLiteral("批量出售"), QStringLiteral("成功批量出售 %1 件已勾选装备，回收金币: +%2").arg(soldCount).arg(totalGold));
        QMessageBox::information(this, QStringLiteral("批量出售成功"),
                                 QStringLiteral("成功出售 %1 件装备，获得金币 +%2！").arg(soldCount).arg(totalGold));
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请先勾选需要批量出售的装备前的复选框！"));
    }
}

void MainWindow::onClearAllClicked()
{
    ui->listInventory->clear();
    appendLog(QStringLiteral("clear"), QStringLiteral("背包已一键清空！"));
}

void MainWindow::onResetSampleItemsClicked()
{
    initSampleItems();
    appendLog(QStringLiteral("重置背包"), QStringLiteral("成功恢复初始测试装备道具列表。"));
}

void MainWindow::onCheckAllItems(bool check)
{
    for (int i = 0; i < ui->listInventory->count(); ++i) {
        QListWidgetItem *item = ui->listInventory->item(i);
        if (item) {
            item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
        }
    }
    appendLog(QStringLiteral("批量勾选"), QStringLiteral("所有装备已全部 %1").arg(check ? QStringLiteral("勾选") : QStringLiteral("取消勾选")));
}

// ============================================================================
// 实时搜索过滤、视图切换与排序
// ============================================================================
void MainWindow::onFilterTextChanged(const QString &keyword)
{
    QString kw = keyword.trimmed();
    int visibleCount = 0;

    for (int i = 0; i < ui->listInventory->count(); ++i) {
        QListWidgetItem *item = ui->listInventory->item(i);
        bool match = kw.isEmpty() || item->text().contains(kw, Qt::CaseInsensitive);
        item->setHidden(!match);
        if (match) ++visibleCount;
    }

    appendLog(QStringLiteral("过滤搜索"), QStringLiteral("关键词: [%1] -> 匹配显示: %2 / %3 项").arg(kw).arg(visibleCount).arg(ui->listInventory->count()));
}

void MainWindow::onViewModeChanged(int index)
{
    if (index == 0) {
        ui->listInventory->setViewMode(QListView::ListMode);
        ui->listInventory->setGridSize(QSize()); // 重置网格
        appendLog(QStringLiteral("视图切换"), QStringLiteral("切换为标准列表模式 (ListMode)"));
    } else {
        ui->listInventory->setViewMode(QListView::IconMode);
        ui->listInventory->setGridSize(QSize(130, 85)); // 设置大图标网格尺寸
        ui->listInventory->setResizeMode(QListView::Adjust);
        appendLog(QStringLiteral("视图切换"), QStringLiteral("切换为大图标网格模式 (IconMode)"));
    }
}

void MainWindow::onSortAscClicked()
{
    ui->listInventory->sortItems(Qt::AscendingOrder);
    appendLog(QStringLiteral("排序"), QStringLiteral("装备列表按名称升序 A->Z 排序"));
}

void MainWindow::onSortDescClicked()
{
    ui->listInventory->sortItems(Qt::DescendingOrder);
    appendLog(QStringLiteral("排序"), QStringLiteral("装备列表按名称降序 Z->A 排序"));
}

void MainWindow::onSortByGoldValue()
{
    // 自定义按金币价值冒泡/插入排序
    int count = ui->listInventory->count();
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            int gold1 = ui->listInventory->item(j)->data(Qt::UserRole + 3).toInt();
            int gold2 = ui->listInventory->item(j + 1)->data(Qt::UserRole + 3).toInt();
            if (gold1 < gold2) { // 降序：高价值排在前面
                QListWidgetItem *taken = ui->listInventory->takeItem(j + 1);
                ui->listInventory->insertItem(j, taken);
            }
        }
    }
    appendLog(QStringLiteral("智能整理"), QStringLiteral("背包已按装备金币回收价值从高到低完成整理排序！"));
}

// ============================================================================
// QListWidget 交互信号与右键菜单 (区分 Item 菜单与空白处全局菜单)
// ============================================================================
void MainWindow::onItemClicked(QListWidgetItem *item)
{
    if (item) {
        int id = item->data(Qt::UserRole + 1).toInt();
        appendLog(QStringLiteral("itemClicked"), QStringLiteral("单击装备: %1 (ID: %2, 勾选状态: %3)")
                  .arg(item->text())
                  .arg(id)
                  .arg(item->checkState() == Qt::Checked ? QStringLiteral("已勾选") : QStringLiteral("未勾选")));
    }
}

void MainWindow::onItemDoubleClicked(QListWidgetItem *item)
{
    if (item) {
        appendLog(QStringLiteral("双击编辑"), QStringLiteral("双击触发就地编辑: %1").arg(item->text()));
    }
}

void MainWindow::onItemChanged(QListWidgetItem *item)
{
    if (item) {
        appendLog(QStringLiteral("itemChanged"), QStringLiteral("装备数据或文本被修改: %1").arg(item->text()));
    }
}

void MainWindow::onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QString currName = current ? current->text() : QStringLiteral("无");
    QString prevName = previous ? previous->text() : QStringLiteral("无");
    appendLog(QStringLiteral("光标移动"), QStringLiteral("焦点项变更: [%1] -> [%2]").arg(prevName).arg(currName));
}

void MainWindow::onCustomContextMenu(const QPoint &pos)
{
    // ========================================================================
    // 【核心避坑与右键菜单完整设计】：
    //  1. 使用 itemAt(pos) 判断右键点击的是具体 Item 还是列表空白区域。
    //  2. 弹出位置必须使用 viewport()->mapToGlobal(pos) 进行绝对坐标映射。
    // ========================================================================
    QListWidgetItem *item = ui->listInventory->itemAt(pos);
    QPoint globalPos = ui->listInventory->viewport()->mapToGlobal(pos);

    if (item) {
        // --------------------------------------------------------------------
        // 场景 A：点击在具体的 Item 装备上，弹出 Item 专属操作菜单
        // --------------------------------------------------------------------
        QMenu menu(this);
        QAction *actDetail   = menu.addAction(QStringLiteral("🔍 查看详细属性"));
        QAction *actRename   = menu.addAction(QStringLiteral("✏️ 重命名道具 (就地编辑)"));
        QAction *actEnhance  = menu.addAction(QStringLiteral("✨ 装备强化 (+500金币)"));
        QAction *actSellOne  = menu.addAction(QStringLiteral("💰 出售此件装备"));
        menu.addSeparator();
        QAction *actDelete   = menu.addAction(QStringLiteral("🗑 销毁丢弃 (takeItem)"));

        QAction *selected = menu.exec(globalPos);
        if (selected == actDetail) {
            int id = item->data(Qt::UserRole + 1).toInt();
            QString rarity = item->data(Qt::UserRole + 2).toString();
            int gold = item->data(Qt::UserRole + 3).toInt();
            QMessageBox::information(this, QStringLiteral("装备详情"),
                                     QStringLiteral("道具名：%1\n装备编号：%2\n品质等级：%3\n回收价值：%4 金币")
                                     .arg(item->text()).arg(id).arg(rarity).arg(gold));
        } else if (selected == actRename) {
            ui->listInventory->editItem(item);
        } else if (selected == actEnhance) {
            int gold = item->data(Qt::UserRole + 3).toInt() + 500;
            item->setData(Qt::UserRole + 3, gold);

            ui->listInventory->blockSignals(true);
            item->setText(item->text() + QStringLiteral(" +1"));
            ui->listInventory->blockSignals(false);

            appendLog(QStringLiteral("强化"), QStringLiteral("装备强化成功: %1 (新价值: %2 金币)").arg(item->text()).arg(gold));
        } else if (selected == actSellOne) {
            int row = ui->listInventory->row(item);
            int gold = item->data(Qt::UserRole + 3).toInt();
            QString name = item->text();
            QListWidgetItem *taken = ui->listInventory->takeItem(row);
            delete taken;
            appendLog(QStringLiteral("出售"), QStringLiteral("成功出售装备: %1，获得金币: +%2").arg(name).arg(gold));
        } else if (selected == actDelete) {
            int row = ui->listInventory->row(item);
            QListWidgetItem *taken = ui->listInventory->takeItem(row);
            delete taken;
            appendLog(QStringLiteral("销毁"), QStringLiteral("通过右键菜单成功丢弃销毁装备。"));
        }
    } else {
        // --------------------------------------------------------------------
        // 场景 B：点击在空白区域，弹出全局背包管理与快捷菜单
        // --------------------------------------------------------------------
        QMenu menu(this);
        QAction *actQuickAdd   = menu.addAction(QStringLiteral("➕ 快速新增装备道具..."));
        QAction *actResetAll   = menu.addAction(QStringLiteral("🔄 重置恢复初始背包"));
        menu.addSeparator();
        QAction *actSortGold   = menu.addAction(QStringLiteral("🧹 整理背包 (按金币价值从高到低)"));
        QAction *actCheckAll   = menu.addAction(QStringLiteral("☑️ 全部勾选"));
        QAction *actUncheckAll = menu.addAction(QStringLiteral("⬜ 全部取消勾选"));
        menu.addSeparator();
        QAction *actClearAll   = menu.addAction(QStringLiteral("🗑 一键清空背包 (clear)"));

        QAction *selected = menu.exec(globalPos);
        if (selected == actQuickAdd) {
            bool ok = false;
            QString text = QInputDialog::getText(this, QStringLiteral("快速新增装备"),
                                                 QStringLiteral("请输入装备名称："),
                                                 QLineEdit::Normal, QStringLiteral("神秘符文之剑"), &ok);
            if (ok && !text.trimmed().isEmpty()) {
                m_iconCounter = (m_iconCounter % 50) + 1;
                auto *newItem = createInventoryItem(text.trimmed(), QStringLiteral("🟣 史诗 (Epic)"), 5000, m_iconCounter);
                ui->listInventory->addItem(newItem);
                appendLog(QStringLiteral("空白右键"), QStringLiteral("快速新增装备: %1").arg(text.trimmed()));
            }
        } else if (selected == actResetAll) {
            onResetSampleItemsClicked();
        } else if (selected == actSortGold) {
            onSortByGoldValue();
        } else if (selected == actCheckAll) {
            onCheckAllItems(true);
        } else if (selected == actUncheckAll) {
            onCheckAllItems(false);
        } else if (selected == actClearAll) {
            onClearAllClicked();
        }
    }
}
