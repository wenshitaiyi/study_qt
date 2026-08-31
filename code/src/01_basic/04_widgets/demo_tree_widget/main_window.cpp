#include "main_window.h"
#include "ui_main_window.h"

#include <QDateTime>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    // 列宽比例自适应配置
    ui->treeSkills->header()->setSectionResizeMode(0, QHeaderView::Stretch); // 技能名称列自适应拉伸
    ui->treeSkills->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeSkills->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeSkills->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeSkills->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    initSampleTalentTree();

    // 绑定 CRUD 操作
    connect(ui->btnAddTopLevel, &QPushButton::clicked, this, &MainWindow::onAddTopLevelClicked);
    connect(ui->btnAddChild, &QPushButton::clicked, this, &MainWindow::onAddChildClicked);
    connect(ui->btnUpgrade, &QPushButton::clicked, this, &MainWindow::onUpgradeClicked);
    connect(ui->btnDeleteNode, &QPushButton::clicked, this, &MainWindow::onDeleteNodeClicked);

    // 绑定搜索与折叠
    connect(ui->editFilter, &QLineEdit::textChanged, this, &MainWindow::onFilterTextChanged);
    connect(ui->btnExpandAll, &QPushButton::clicked, this, &MainWindow::onExpandAllClicked);
    connect(ui->btnCollapseAll, &QPushButton::clicked, this, &MainWindow::onCollapseAllClicked);
    connect(ui->btnSortName, &QPushButton::clicked, this, &MainWindow::onSortNameClicked);

    // 绑定 QTreeWidget 信号
    connect(ui->treeSkills, &QTreeWidget::itemChanged, this, &MainWindow::onItemChanged);
    connect(ui->treeSkills, &QTreeWidget::itemClicked, this, &MainWindow::onItemClicked);
    connect(ui->treeSkills, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onItemDoubleClicked);
    connect(ui->treeSkills, &QTreeWidget::currentItemChanged, this, &MainWindow::onCurrentItemChanged);
    connect(ui->treeSkills, &QTreeWidget::customContextMenuRequested, this, &MainWindow::onCustomContextMenu);

    connect(ui->btnClearLog, &QPushButton::clicked, ui->textLog, &QPlainTextEdit::clear);

    appendLog(QStringLiteral("系统"), QStringLiteral("游戏技能天赋树 QTreeWidget 初始化就绪。"));
}

MainWindow::~MainWindow() = default;

void MainWindow::appendLog(const QString &category, const QString &message)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    QString line = QStringLiteral("[%1] [%2] %3").arg(timeStr).arg(category.leftJustified(10, ' ')).arg(message);
    ui->textLog->appendPlainText(line);
}

// ============================================================================
// 天赋树节点创建与属性设置 (Flags, CheckBox, UserRole)
// ============================================================================
QTreeWidgetItem *MainWindow::createSkillNode(const QString &name, const QString &type, int rank, int maxRank,
                                             const QString &cost, const QString &status, const QString &iconText)
{
    auto *item = new QTreeWidgetItem();
    Q_UNUSED(iconText);

    // 0: 名称 (开启复选框与就地编辑)
    item->setText(0, name);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable |
                   Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
    item->setCheckState(0, Qt::Unchecked);

    // 1: 类型
    item->setText(1, type);

    // 2: 点数等级
    item->setText(2, QStringLiteral("%1 / %2 点").arg(rank).arg(maxRank));
    item->setTextAlignment(2, Qt::AlignCenter);

    // 3: 消耗 / CD
    item->setText(3, cost);
    item->setTextAlignment(3, Qt::AlignCenter);

    // 4: 状态
    item->setText(4, status);
    item->setTextAlignment(4, Qt::AlignCenter);

    // 元数据绑定
    item->setData(0, Qt::UserRole + 1, rank);
    item->setData(0, Qt::UserRole + 2, maxRank);

    // 状态前景色
    if (status.contains(QStringLiteral("已满阶"))) {
        item->setForeground(4, QBrush(QColor(230, 126, 34))); // 金色
    } else if (status.contains(QStringLiteral("已激活"))) {
        item->setForeground(4, QBrush(QColor(46, 204, 113)));  // 绿色
    } else {
        item->setForeground(4, QBrush(QColor(149, 165, 166))); // 灰色
    }

    return item;
}

void MainWindow::initSampleTalentTree()
{
    ui->treeSkills->blockSignals(true);
    ui->treeSkills->clear();

    // ------------------------------------------------------------------------
    // 顶层分支 1：惩戒系专精 (Retribution)
    // ------------------------------------------------------------------------
    auto *rootRet = new QTreeWidgetItem();
    rootRet->setText(0, QStringLiteral("⚔️ 惩戒专精天赋分支 (Retribution)"));
    rootRet->setText(1, QStringLiteral("【主战专精】"));
    rootRet->setText(2, QStringLiteral("9 / 11 点"));
    rootRet->setText(4, QStringLiteral("主修专精"));
    rootRet->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
    rootRet->setCheckState(0, Qt::Unchecked);
    rootRet->setForeground(0, QBrush(QColor(231, 76, 60))); // 红色
    rootRet->setFont(0, QFont(QStringLiteral("Microsoft YaHei"), 9, QFont::Bold));

    auto *skill11 = createSkillNode(QStringLiteral("🔥 灰烬裁决 (Ashen Verdict)"), QStringLiteral("核心主动"), 3, 5, QStringLiteral("45 MP / 8s"), QStringLiteral("已激活"), QStringLiteral("🗡️"));
    auto *skill12 = createSkillNode(QStringLiteral("⚡ 圣光之剑 (Blade of Justice)"), QStringLiteral("核心主动"), 5, 5, QStringLiteral("30 MP / 6s"), QStringLiteral("已满阶"), QStringLiteral("⚡"));

    // 二级子分支：终极奥义
    auto *subBranch = new QTreeWidgetItem();
    subBranch->setText(0, QStringLiteral("👑 狂暴裁决阶层 (Awakening)"));
    subBranch->setText(1, QStringLiteral("【二转进阶】"));
    subBranch->setText(2, QStringLiteral("1 / 1 点"));
    subBranch->setText(4, QStringLiteral("已进阶"));
    subBranch->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
    subBranch->setCheckState(0, Qt::Unchecked);

    auto *skillAwaken = createSkillNode(QStringLiteral("🌟 诸神黄昏·灰烬使者觉醒"), QStringLiteral("终极觉醒"), 1, 1, QStringLiteral("100 MP / 120s"), QStringLiteral("已满阶"), QStringLiteral("👑"));
    subBranch->addChild(skillAwaken);

    rootRet->addChild(skill11);
    rootRet->addChild(skill12);
    rootRet->addChild(subBranch);
    ui->treeSkills->addTopLevelItem(rootRet);

    // ------------------------------------------------------------------------
    // 顶层分支 2：防护系专精 (Protection)
    // ------------------------------------------------------------------------
    auto *rootProt = new QTreeWidgetItem();
    rootProt->setText(0, QStringLiteral("🛡️ 防护专精天赋分支 (Protection)"));
    rootProt->setText(1, QStringLiteral("【防御专精】"));
    rootProt->setText(2, QStringLiteral("7 / 8 点"));
    rootProt->setText(4, QStringLiteral("副修专精"));
    rootProt->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
    rootProt->setCheckState(0, Qt::Unchecked);
    rootProt->setForeground(0, QBrush(QColor(52, 152, 219))); // 蓝色
    rootProt->setFont(0, QFont(QStringLiteral("Microsoft YaHei"), 9, QFont::Bold));

    auto *skill21 = createSkillNode(QStringLiteral("🛡️ 绝对壁垒光环 (Holy Bulwark)"), QStringLiteral("被动强化"), 5, 5, QStringLiteral("0 MP / 被动"), QStringLiteral("已满阶"), QStringLiteral("🛡️"));
    auto *skill22 = createSkillNode(QStringLiteral("✨ 炽热防御者 (Ardent Defender)"), QStringLiteral("减伤救赎"), 2, 3, QStringLiteral("50 MP / 90s"), QStringLiteral("已激活"), QStringLiteral("✨"));

    rootProt->addChild(skill21);
    rootProt->addChild(skill22);
    ui->treeSkills->addTopLevelItem(rootProt);

    // ------------------------------------------------------------------------
    // 顶层分支 3：神圣系专精 (Holy)
    // ------------------------------------------------------------------------
    auto *rootHoly = new QTreeWidgetItem();
    rootHoly->setText(0, QStringLiteral("✨ 神圣治愈天赋分支 (Holy)"));
    rootHoly->setText(1, QStringLiteral("【治愈专精】"));
    rootHoly->setText(2, QStringLiteral("5 / 10 点"));
    rootHoly->setText(4, QStringLiteral("辅助专精"));
    rootHoly->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
    rootHoly->setCheckState(0, Qt::Unchecked);
    rootHoly->setForeground(0, QBrush(QColor(46, 204, 113))); // 绿色
    rootHoly->setFont(0, QFont(QStringLiteral("Microsoft YaHei"), 9, QFont::Bold));

    auto *skill31 = createSkillNode(QStringLiteral("✨ 圣光术 (Holy Light)"), QStringLiteral("单体强效"), 4, 5, QStringLiteral("80 MP / 2.5s"), QStringLiteral("已激活"), QStringLiteral("✨"));
    auto *skill32 = createSkillNode(QStringLiteral("💖 黎明之光 (Light of Dawn)"), QStringLiteral("群体治愈"), 1, 5, QStringLiteral("60 MP / 15s"), QStringLiteral("已激活"), QStringLiteral("💖"));

    rootHoly->addChild(skill31);
    rootHoly->addChild(skill32);
    ui->treeSkills->addTopLevelItem(rootHoly);

    ui->treeSkills->expandAll();
    ui->treeSkills->blockSignals(false);
}

// ============================================================================
// 父子级联勾选核心逻辑 (智能三态半选与信号防递归)
// ============================================================================
void MainWindow::updateChildrenCheckState(QTreeWidgetItem *parentItem, Qt::CheckState state)
{
    // 如果是半选状态，不向下强制覆盖
    if (state == Qt::PartiallyChecked) return;

    for (int i = 0; i < parentItem->childCount(); ++i) {
        QTreeWidgetItem *child = parentItem->child(i);
        child->setCheckState(0, state);
        // 递归处理多级深层子节点
        updateChildrenCheckState(child, state);
    }
}

void MainWindow::updateParentCheckState(QTreeWidgetItem *childItem)
{
    QTreeWidgetItem *parent = childItem->parent();
    if (!parent) return;

    int checkedCount = 0;
    int partiallyCount = 0;
    int totalChildren = parent->childCount();

    for (int i = 0; i < totalChildren; ++i) {
        Qt::CheckState s = parent->child(i)->checkState(0);
        if (s == Qt::Checked) {
            ++checkedCount;
        } else if (s == Qt::PartiallyChecked) {
            ++partiallyCount;
        }
    }

    if (checkedCount == totalChildren) {
        parent->setCheckState(0, Qt::Checked); // 全选
    } else if (checkedCount > 0 || partiallyCount > 0) {
        parent->setCheckState(0, Qt::PartiallyChecked); // 半选
    } else {
        parent->setCheckState(0, Qt::Unchecked); // 全不选
    }

    // 递归向上追溯祖父节点
    updateParentCheckState(parent);
}

void MainWindow::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (column != 0 || m_isUpdatingCheckState) return;

    // ========================================================================
    // 【核心避坑指南 1：父子级联勾选防死循环风暴】
    //  当修改父节点勾选状态会触发子节点修改，子节点修改又反过来触发父节点，
    //  必须使用 m_isUpdatingCheckState 标志位或 blockSignals 严格阻断递归！
    // ========================================================================
    m_isUpdatingCheckState = true;
    ui->treeSkills->blockSignals(true);

    Qt::CheckState currentState = item->checkState(0);

    // 1. 向下递归更新所有子节点
    updateChildrenCheckState(item, currentState);

    // 2. 向上递归更新所有父级节点（智能计算全选/半选/全不选）
    updateParentCheckState(item);

    ui->treeSkills->blockSignals(false);
    m_isUpdatingCheckState = false;

    appendLog(QStringLiteral("勾选级联"), QStringLiteral("节点 [%1] 状态变更为: %2 (已完成父子双向级联同步)")
              .arg(item->text(0))
              .arg(currentState == Qt::Checked ? QStringLiteral("全选") : (currentState == Qt::PartiallyChecked ? QStringLiteral("半选") : QStringLiteral("未勾选"))));
}

// ============================================================================
// CRUD 增删改查实现
// ============================================================================
void MainWindow::onAddTopLevelClicked()
{
    QString name = ui->editNodeName->text().trimmed();
    if (name.isEmpty()) return;

    auto *topLevel = new QTreeWidgetItem();
    topLevel->setText(0, QStringLiteral("🌲 %1 (新分支)").arg(name));
    topLevel->setText(1, QStringLiteral("【自定义分支】"));
    topLevel->setText(2, QStringLiteral("0 / 5 点"));
    topLevel->setText(4, QStringLiteral("新开放"));
    topLevel->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate | Qt::ItemIsEditable);
    topLevel->setCheckState(0, Qt::Unchecked);

    ui->treeSkills->addTopLevelItem(topLevel);
    ui->treeSkills->setCurrentItem(topLevel);

    appendLog(QStringLiteral("addTopLevel"), QStringLiteral("添加顶层天赋分支: %1").arg(name));
}

void MainWindow::onAddChildClicked()
{
    QTreeWidgetItem *current = ui->treeSkills->currentItem();
    if (!current) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请先在左侧树中选中要挂载子技能的父分支节点！"));
        return;
    }

    QString name = ui->editNodeName->text().trimmed();
    if (name.isEmpty()) return;

    QString type = ui->comboNodeType->currentText();
    int rank = ui->spinRank->value();

    auto *child = createSkillNode(name, type, rank, 5, QStringLiteral("35 MP / 10s"), QStringLiteral("已激活"), QStringLiteral("⚔️"));
    current->addChild(child);
    current->setExpanded(true); // 自动展开父节点

    appendLog(QStringLiteral("addChild"), QStringLiteral("在分支 [%1] 下添加子技能: %2").arg(current->text(0)).arg(name));
}

void MainWindow::onUpgradeClicked()
{
    QTreeWidgetItem *current = ui->treeSkills->currentItem();
    if (!current) return;

    int rank = current->data(0, Qt::UserRole + 1).toInt();
    int maxRank = current->data(0, Qt::UserRole + 2).toInt();

    if (maxRank == 0) maxRank = 5;

    if (rank < maxRank) {
        ++rank;
        current->setData(0, Qt::UserRole + 1, rank);
        current->setText(2, QStringLiteral("%1 / %2 点").arg(rank).arg(maxRank));
        if (rank == maxRank) {
            current->setText(4, QStringLiteral("已满阶"));
            current->setForeground(4, QBrush(QColor(230, 126, 34)));
        }
        appendLog(QStringLiteral("技能升级"), QStringLiteral("技能 [%1] 投入点数升级至: %2 / %3 点").arg(current->text(0)).arg(rank).arg(maxRank));
    } else {
        QMessageBox::information(this, QStringLiteral("已达上限"), QStringLiteral("该天赋技能已达到最大阶数 (%1 点)！").arg(maxRank));
    }
}

void MainWindow::onDeleteNodeClicked()
{
    // ========================================================================
    // 【核心避坑指南 2：takeChild 与 takeTopLevelItem 内存安全释放】
    //  必须显式 delete 指针，否则造成内存泄漏！
    // ========================================================================
    QTreeWidgetItem *current = ui->treeSkills->currentItem();
    if (!current) return;

    QString name = current->text(0);
    QTreeWidgetItem *parent = current->parent();

    if (parent) {
        int index = parent->indexOfChild(current);
        QTreeWidgetItem *taken = parent->takeChild(index);
        delete taken; // 安全释放
        // 重新刷新父节点的勾选状态
        updateParentCheckState(parent);
    } else {
        int index = ui->treeSkills->indexOfTopLevelItem(current);
        QTreeWidgetItem *taken = ui->treeSkills->takeTopLevelItem(index);
        delete taken; // 安全释放
    }

    appendLog(QStringLiteral("删除节点"), QStringLiteral("安全移除并释放节点: %1").arg(name));
}

// ============================================================================
// 树形结构递归搜索过滤与自动展开
// ============================================================================
bool MainWindow::filterNodeRecursive(QTreeWidgetItem *item, const QString &keyword)
{
    // 判断自身多列中是否含有关键词
    bool selfMatch = false;
    for (int col = 0; col < 5; ++col) {
        if (item->text(col).contains(keyword, Qt::CaseInsensitive)) {
            selfMatch = true;
            break;
        }
    }

    // 递归判断所有子节点
    bool anyChildMatch = false;
    for (int i = 0; i < item->childCount(); ++i) {
        if (filterNodeRecursive(item->child(i), keyword)) {
            anyChildMatch = true;
        }
    }

    // ========================================================================
    // 【核心知识点 2：树节点显示判定与祖先自动展开】
    //  若自身命中 或 任一子节点命中，则当前节点必须可见 (setHidden(false))。
    //  若子节点命中，父节点必须自动展开 (setExpanded(true))，以便用户看到结果。
    // ========================================================================
    bool visible = keyword.isEmpty() || selfMatch || anyChildMatch;
    item->setHidden(!visible);

    if (!keyword.isEmpty() && anyChildMatch) {
        item->setExpanded(true);
    }

    return visible;
}

void MainWindow::onFilterTextChanged(const QString &keyword)
{
    QString kw = keyword.trimmed();
    ui->treeSkills->setUpdatesEnabled(false); // 抑制刷新防闪烁

    for (int i = 0; i < ui->treeSkills->topLevelItemCount(); ++i) {
        filterNodeRecursive(ui->treeSkills->topLevelItem(i), kw);
    }

    ui->treeSkills->setUpdatesEnabled(true);
    appendLog(QStringLiteral("过滤搜索"), QStringLiteral("技能树递归检索关键词: [%1]").arg(kw));
}

void MainWindow::onExpandAllClicked()
{
    ui->treeSkills->expandAll();
    appendLog(QStringLiteral("展开折叠"), QStringLiteral("调用 expandAll() 展开全部层级分支"));
}

void MainWindow::onCollapseAllClicked()
{
    ui->treeSkills->collapseAll();
    appendLog(QStringLiteral("展开折叠"), QStringLiteral("调用 collapseAll() 折叠全部层级分支"));
}

void MainWindow::onSortNameClicked()
{
    m_sortAsc = !m_sortAsc;
    ui->treeSkills->sortItems(0, m_sortAsc ? Qt::AscendingOrder : Qt::DescendingOrder);
    appendLog(QStringLiteral("排序"), QStringLiteral("按技能名称进行 %1 排序").arg(m_sortAsc ? QStringLiteral("升序 A->Z") : QStringLiteral("降序 Z->A")));
}

// ============================================================================
// 交互信号与右键多级上下文菜单 (Viewport 坐标避坑)
// ============================================================================
void MainWindow::onItemClicked(QTreeWidgetItem *item, int column)
{
    if (item) {
        appendLog(QStringLiteral("itemClicked"), QStringLiteral("点击节点: [%1], 列: %2, 勾选状态: %3")
                  .arg(item->text(0))
                  .arg(column)
                  .arg(item->checkState(0) == Qt::Checked ? QStringLiteral("已勾选") : QStringLiteral("未勾选")));
    }
}

void MainWindow::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item) {
        appendLog(QStringLiteral("双击编辑"), QStringLiteral("双击节点: [%1], 第 %2 列就地编辑").arg(item->text(0)).arg(column));
    }
}

void MainWindow::onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    QString currName = current ? current->text(0) : QStringLiteral("无");
    QString prevName = previous ? previous->text(0) : QStringLiteral("无");
    appendLog(QStringLiteral("光标移动"), QStringLiteral("焦点节点切换: [%1] -> [%2]").arg(prevName).arg(currName));
}

void MainWindow::onCustomContextMenu(const QPoint &pos)
{
    // ========================================================================
    // 【核心知识点 3：多级分流右键菜单与 Viewport 绝对坐标换算】
    //  1. 换算坐标：treeSkills->viewport()->mapToGlobal(pos)
    //  2. 智能分流：
    //     - 空白区域：全局操作菜单
    //     - 顶层分支节点：分支管理菜单
    //     - 叶子技能节点：技能升级/洗点/详情菜单
    // ========================================================================
    QTreeWidgetItem *item = ui->treeSkills->itemAt(pos);
    QPoint globalPos = ui->treeSkills->viewport()->mapToGlobal(pos);

    if (!item) {
        // --------------------------------------------------------------------
        // 场景 A：点击在空白区域 -> 全局天赋树操作
        // --------------------------------------------------------------------
        QMenu menu(this);
        QAction *actAddBranch  = menu.addAction(QStringLiteral("🌲 添加新顶层分支..."));
        QAction *actResetTree  = menu.addAction(QStringLiteral("🔄 重置恢复初始技能树"));
        menu.addSeparator();
        QAction *actExpand     = menu.addAction(QStringLiteral("📂 全部展开 (expandAll)"));
        QAction *actCollapse   = menu.addAction(QStringLiteral("📁 全部折叠 (collapseAll)"));

        QAction *sel = menu.exec(globalPos);
        if (sel == actAddBranch) {
            onAddTopLevelClicked();
        } else if (sel == actResetTree) {
            initSampleTalentTree();
        } else if (sel == actExpand) {
            onExpandAllClicked();
        } else if (sel == actCollapse) {
            onCollapseAllClicked();
        }
    } else if (item->parent() == nullptr) {
        // --------------------------------------------------------------------
        // 场景 B：点击在顶层分支节点 (TopLevel Item)
        // --------------------------------------------------------------------
        QMenu menu(this);
        QAction *actAddChild   = menu.addAction(QStringLiteral("➕ 在此分支下追加子技能..."));
        QAction *actToggleExp  = menu.addAction(item->isExpanded() ? QStringLiteral("📁 折叠本分支") : QStringLiteral("📂 展开本分支"));
        QAction *actCheckBranch = menu.addAction(QStringLiteral("☑️ 激活本分支全部技能"));
        menu.addSeparator();
        QAction *actDeleteBranch = menu.addAction(QStringLiteral("🗑 移除本顶层分支"));

        QAction *sel = menu.exec(globalPos);
        if (sel == actAddChild) {
            ui->treeSkills->setCurrentItem(item);
            onAddChildClicked();
        } else if (sel == actToggleExp) {
            item->setExpanded(!item->isExpanded());
        } else if (sel == actCheckBranch) {
            item->setCheckState(0, Qt::Checked);
        } else if (sel == actDeleteBranch) {
            ui->treeSkills->setCurrentItem(item);
            onDeleteNodeClicked();
        }
    } else {
        // --------------------------------------------------------------------
        // 场景 C：点击在叶子技能节点 (Child Item)
        // --------------------------------------------------------------------
        QMenu menu(this);
        QAction *actUpgrade    = menu.addAction(QStringLiteral("✨ 学习加点升级 (+1)"));
        QAction *actRename     = menu.addAction(QStringLiteral("✏️ 重命名 (就地编辑)"));
        QAction *actDetail     = menu.addAction(QStringLiteral("🔍 查看技能数值面板"));
        menu.addSeparator();
        QAction *actDelete     = menu.addAction(QStringLiteral("🗑 遗忘洗点 (移除该技能)"));

        QAction *sel = menu.exec(globalPos);
        if (sel == actUpgrade) {
            ui->treeSkills->setCurrentItem(item);
            onUpgradeClicked();
        } else if (sel == actRename) {
            ui->treeSkills->editItem(item, 0);
        } else if (sel == actDetail) {
            QMessageBox::information(this, QStringLiteral("技能详情"),
                                     QStringLiteral("技能名称：%1\n类型：%2\n当前点数：%3\n消耗与冷却：%4\n状态：%5")
                                     .arg(item->text(0)).arg(item->text(1)).arg(item->text(2)).arg(item->text(3)).arg(item->text(4)));
        } else if (sel == actDelete) {
            ui->treeSkills->setCurrentItem(item);
            onDeleteNodeClicked();
        }
    }
}
