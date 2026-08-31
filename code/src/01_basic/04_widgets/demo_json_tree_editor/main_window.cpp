#include "main_window.h"
#include "ui_main_window.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>
#include <QClipboard>
#include <QApplication>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    // 分栏比例设置 (左侧树 60%, 右侧源码 40%)
    ui->splitterMain->setStretchFactor(0, 3);
    ui->splitterMain->setStretchFactor(1, 2);

    // 列宽比例设置
    ui->treeJson->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeJson->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeJson->header()->setSectionResizeMode(2, QHeaderView::Stretch);

    // 绑定工具栏按钮
    connect(ui->btnOpenFile, &QPushButton::clicked, this, &MainWindow::onOpenFileClicked);
    connect(ui->btnSaveFile, &QPushButton::clicked, this, &MainWindow::onSaveFileClicked);
    connect(ui->btnLoadSample, &QPushButton::clicked, this, &MainWindow::onLoadSampleClicked);
    connect(ui->btnSyncTextToTree, &QPushButton::clicked, this, &MainWindow::onSyncTextToTreeClicked);
    connect(ui->btnExpandAll, &QPushButton::clicked, this, &MainWindow::onExpandAllClicked);
    connect(ui->btnCollapseAll, &QPushButton::clicked, this, &MainWindow::onCollapseAllClicked);
    connect(ui->editFilter, &QLineEdit::textChanged, this, &MainWindow::onFilterTextChanged);

    // 绑定树形交互信号
    connect(ui->treeJson, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onTreeItemDoubleClicked);
    connect(ui->treeJson, &QTreeWidget::itemChanged, this, &MainWindow::onTreeItemChanged);
    connect(ui->treeJson, &QTreeWidget::customContextMenuRequested, this, &MainWindow::onCustomContextMenu);

    // 全局美化 QSS：彻底解决 QGroupBox 标题高度截断/显示不完整问题，并提升现代感
    setStyleSheet(QStringLiteral(
        "QGroupBox {"
        "   font-size: 12px;"
        "   font-weight: bold;"
        "   color: #2c3e50;"
        "   border: 1px solid #dcdde1;"
        "   border-radius: 6px;"
        "   margin-top: 14px;"
        "   padding-top: 16px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   subcontrol-position: top left;"
        "   left: 10px;"
        "   padding: 0 6px;"
        "   background-color: transparent;"
        "}"
        "QPushButton {"
        "   padding: 4px 10px;"
        "   border: 1px solid #bdc3c7;"
        "   border-radius: 4px;"
        "   background-color: #ecf0f1;"
        "}"
        "QPushButton:hover {"
        "   background-color: #dfe6e9;"
        "   border-color: #95a5a6;"
        "}"
    ));

    // 默认载入游戏样例配置
    onLoadSampleClicked();
}

MainWindow::~MainWindow() = default;

// ============================================================================
// 节点视觉样式与外观修饰
// ============================================================================
void MainWindow::setNodeVisualAppearance(QTreeWidgetItem *item, QJsonValue::Type type)
{
    // 将 Type 存入 UserRole
    item->setData(1, Qt::UserRole, static_cast<int>(type));

    switch (type) {
    case QJsonValue::Object:
        item->setText(1, QStringLiteral("{ } Object"));
        item->setForeground(1, QBrush(QColor(155, 89, 182))); // 紫色
        item->setFont(0, QFont(QStringLiteral("Consolas"), 9, QFont::Bold));
        break;
    case QJsonValue::Array:
        item->setText(1, QStringLiteral("[ ] Array"));
        item->setForeground(1, QBrush(QColor(52, 152, 219)));  // 蓝色
        item->setFont(0, QFont(QStringLiteral("Consolas"), 9, QFont::Bold));
        break;
    case QJsonValue::String:
        item->setText(1, QStringLiteral("String"));
        item->setForeground(1, QBrush(QColor(46, 204, 113)));  // 绿色
        item->setForeground(2, QBrush(QColor(39, 174, 96)));
        break;
    case QJsonValue::Double:
        item->setText(1, QStringLiteral("Number"));
        item->setForeground(1, QBrush(QColor(230, 126, 34))); // 橙色
        item->setForeground(2, QBrush(QColor(211, 84, 0)));
        break;
    case QJsonValue::Bool:
        item->setText(1, QStringLiteral("Boolean"));
        item->setForeground(1, QBrush(QColor(231, 76, 60)));  // 红色
        item->setForeground(2, QBrush(QColor(192, 57, 43)));
        break;
    case QJsonValue::Null:
    default:
        item->setText(1, QStringLiteral("Null"));
        item->setForeground(1, QBrush(QColor(149, 165, 166))); // 灰色
        break;
    }
}

QTreeWidgetItem *MainWindow::createTreeItem(const QString &key, QJsonValue::Type type, const QJsonValue &val)
{
    auto *item = new QTreeWidgetItem();
    item->setText(0, key);

    // 默认允许选中、启用与就地双击编辑
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);

    setNodeVisualAppearance(item, type);

    if (type == QJsonValue::String) {
        item->setText(2, val.toString());
    } else if (type == QJsonValue::Double) {
        item->setText(2, QString::number(val.toDouble()));
    } else if (type == QJsonValue::Bool) {
        item->setText(2, val.toBool() ? QStringLiteral("true") : QStringLiteral("false"));
    } else if (type == QJsonValue::Null) {
        item->setText(2, QStringLiteral("null"));
    } else if (type == QJsonValue::Object) {
        item->setText(2, QStringLiteral("{ %1 个字段 }").arg(val.toObject().size()));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable); // Object 只允许编辑 Key
    } else if (type == QJsonValue::Array) {
        item->setText(2, QStringLiteral("[ %1 个元素 ]").arg(val.toArray().size()));
    }

    return item;
}

// ============================================================================
// JSON 递归解析并填充树形结构
// ============================================================================
void MainWindow::populateObject(const QJsonObject &obj, QTreeWidgetItem *parent)
{
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QString key = it.key();
        QJsonValue val = it.value();

        auto *item = createTreeItem(key, val.type(), val);
        if (parent) {
            parent->addChild(item);
        } else {
            ui->treeJson->addTopLevelItem(item);
        }

        if (val.isObject()) {
            populateObject(val.toObject(), item);
        } else if (val.isArray()) {
            populateArray(val.toArray(), item);
        }
    }
}

void MainWindow::populateArray(const QJsonArray &arr, QTreeWidgetItem *parent)
{
    for (int i = 0; i < arr.size(); ++i) {
        QString key = QStringLiteral("[%1]").arg(i);
        QJsonValue val = arr.at(i);

        auto *item = createTreeItem(key, val.type(), val);
        if (parent) {
            parent->addChild(item);
        } else {
            ui->treeJson->addTopLevelItem(item);
        }

        if (val.isObject()) {
            populateObject(val.toObject(), item);
        } else if (val.isArray()) {
            populateArray(val.toArray(), item);
        }
    }
}

void MainWindow::loadJsonDocument(const QJsonDocument &doc)
{
    m_isUpdating = true;
    ui->treeJson->blockSignals(true);
    ui->treeJson->clear();

    if (doc.isObject()) {
        populateObject(doc.object(), nullptr);
    } else if (doc.isArray()) {
        populateArray(doc.array(), nullptr);
    }

    ui->treeJson->expandAll();
    ui->treeJson->blockSignals(false);
    m_isUpdating = false;

    // 同步到右侧文本源码视图
    ui->textJsonRaw->setPlainText(QString::fromUtf8(doc.toJson(QJsonDocument::Indented)));
    ui->labelStatus->setText(QStringLiteral("✅ JSON 解析加载成功 - 顶层节点数: %1").arg(ui->treeJson->topLevelItemCount()));
}

// ============================================================================
// 树形结构逆向序列化为 JSON Document
// ============================================================================
QJsonObject MainWindow::buildJsonObjectFromItem(QTreeWidgetItem *item)
{
    QJsonObject obj;
    int count = item ? item->childCount() : ui->treeJson->topLevelItemCount();

    for (int i = 0; i < count; ++i) {
        QTreeWidgetItem *child = item ? item->child(i) : ui->treeJson->topLevelItem(i);
        QString key = child->text(0);
        QJsonValue val = buildJsonValueFromItem(child);
        obj.insert(key, val);
    }
    return obj;
}

QJsonArray MainWindow::buildJsonArrayFromItem(QTreeWidgetItem *item)
{
    QJsonArray arr;
    int count = item ? item->childCount() : ui->treeJson->topLevelItemCount();

    for (int i = 0; i < count; ++i) {
        QTreeWidgetItem *child = item ? item->child(i) : ui->treeJson->topLevelItem(i);
        arr.append(buildJsonValueFromItem(child));
    }
    return arr;
}

QJsonValue MainWindow::buildJsonValueFromItem(QTreeWidgetItem *item)
{
    int typeInt = item->data(1, Qt::UserRole).toInt();
    auto type = static_cast<QJsonValue::Type>(typeInt);

    if (type == QJsonValue::Object) {
        return buildJsonObjectFromItem(item);
    } else if (type == QJsonValue::Array) {
        return buildJsonArrayFromItem(item);
    } else if (type == QJsonValue::String) {
        return QJsonValue(item->text(2));
    } else if (type == QJsonValue::Double) {
        return QJsonValue(item->text(2).toDouble());
    } else if (type == QJsonValue::Bool) {
        return QJsonValue(item->text(2).compare(QStringLiteral("true"), Qt::CaseInsensitive) == 0);
    } else {
        return QJsonValue(QJsonValue::Null);
    }
}

QJsonDocument MainWindow::buildJsonDocumentFromTree()
{
    if (ui->treeJson->topLevelItemCount() == 0) {
        return QJsonDocument();
    }

    // 判断根层是 Object 还是 Array (如果第一个顶层节点的 key 是 [0] 则按 Array 序列化)
    QTreeWidgetItem *first = ui->treeJson->topLevelItem(0);
    if (first && first->text(0).startsWith(QLatin1Char('[')) && first->text(0).endsWith(QLatin1Char(']'))) {
        return QJsonDocument(buildJsonArrayFromItem(nullptr));
    } else {
        return QJsonDocument(buildJsonObjectFromItem(nullptr));
    }
}

void MainWindow::syncTreeToRawText()
{
    if (m_isUpdating) return;
    QJsonDocument doc = buildJsonDocumentFromTree();
    ui->textJsonRaw->setPlainText(QString::fromUtf8(doc.toJson(QJsonDocument::Indented)));
    ui->labelStatus->setText(QStringLiteral("🔄 树形结构变动已实时双向同步至右侧 JSON 源码。"));
}

// ============================================================================
// 文件 I/O 与测试样例
// ============================================================================
void MainWindow::onOpenFileClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("打开 JSON 配置文件"),
                                                    QString(), QStringLiteral("JSON 配置文件 (*.json);;所有文件 (*.*)"));
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, QStringLiteral("打开失败"), QStringLiteral("无法读取指定文件: %1").arg(filePath));
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, QStringLiteral("JSON 语法解析错误"),
                              QStringLiteral("文件解析失败！\n错误原因: %1\n字符偏移位置: %2").arg(err.errorString()).arg(err.offset));
        return;
    }

    m_currentFilePath = filePath;
    loadJsonDocument(doc);
    ui->labelStatus->setText(QStringLiteral("📂 成功打开文件: %1").arg(filePath));
}

void MainWindow::onSaveFileClicked()
{
    QString filePath = m_currentFilePath;
    if (filePath.isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, QStringLiteral("保存 JSON 配置文件"),
                                                QStringLiteral("game_config.json"),
                                                QStringLiteral("JSON 配置文件 (*.json);;所有文件 (*.*)"));
    }
    if (filePath.isEmpty()) return;

    QJsonDocument doc = buildJsonDocumentFromTree();
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::critical(this, QStringLiteral("保存失败"), QStringLiteral("无法写入目标文件: %1").arg(filePath));
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    m_currentFilePath = filePath;
    ui->labelStatus->setText(QStringLiteral("💾 成功回写保存 JSON 到磁盘: %1").arg(filePath));
    QMessageBox::information(this, QStringLiteral("保存成功"), QStringLiteral("已成功将可视化树形配置同步持久化回写至磁盘！\n文件路径：%1").arg(filePath));
}

void MainWindow::onLoadSampleClicked()
{
    const char *sampleJson = R"json({
    "game_title": "艾泽拉斯战纪 (Chronicles of Azeroth)",
    "version": "1.5.0",
    "server_port": 8080,
    "is_pvp_enabled": true,
    "player_character": {
        "name": "灰烬裁决者·乌瑟尔",
        "class": "圣骑士 (Paladin)",
        "level": 85,
        "combat_power": 985000,
        "attributes": {
            "health": 125000,
            "mana": 45000,
            "armor": 8500,
            "crit_rate": 0.355
        },
        "inventory": [
            {
                "item_id": 1001,
                "name": "灰烬使者 (Ashbringer)",
                "rarity": "Legendary",
                "damage": 3850
            },
            {
                "item_id": 1002,
                "name": "埃辛诺斯壁垒 (Bulwark)",
                "rarity": "Epic",
                "defense": 2200
            }
        ],
        "skills": [
            "神圣风暴",
            "圣盾术",
            "十字军打击",
            "复仇之怒"
        ]
    }
})json";

    QJsonDocument doc = QJsonDocument::fromJson(QByteArray(sampleJson));
    loadJsonDocument(doc);
    ui->labelStatus->setText(QStringLiteral("🎮 成功载入预置 RPG 游戏配置样例数据。"));
}

void MainWindow::onSyncTextToTreeClicked()
{
    QString rawText = ui->textJsonRaw->toPlainText();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(rawText.toUtf8(), &err);

    if (err.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, QStringLiteral("JSON 语法错误"),
                              QStringLiteral("右侧源码存在语法错误，无法同步到树形！\n错误: %1 (位置: %2)").arg(err.errorString()).arg(err.offset));
        return;
    }

    loadJsonDocument(doc);
    ui->labelStatus->setText(QStringLiteral("⬅️ 成功从右侧源码逆向解析并同步刷新树形视图！"));
}

void MainWindow::onExpandAllClicked()
{
    ui->treeJson->expandAll();
}

void MainWindow::onCollapseAllClicked()
{
    ui->treeJson->collapseAll();
}

// ============================================================================
// 树节点模糊过滤检索
// ============================================================================
bool MainWindow::filterNodeRecursive(QTreeWidgetItem *item, const QString &keyword)
{
    bool matchSelf = item->text(0).contains(keyword, Qt::CaseInsensitive) ||
                     item->text(2).contains(keyword, Qt::CaseInsensitive);

    bool matchChild = false;
    for (int i = 0; i < item->childCount(); ++i) {
        if (filterNodeRecursive(item->child(i), keyword)) {
            matchChild = true;
        }
    }

    bool visible = keyword.isEmpty() || matchSelf || matchChild;
    item->setHidden(!visible);
    if (!keyword.isEmpty() && matchChild) {
        item->setExpanded(true);
    }
    return visible;
}

void MainWindow::onFilterTextChanged(const QString &keyword)
{
    QString kw = keyword.trimmed();
    ui->treeJson->setUpdatesEnabled(false);
    for (int i = 0; i < ui->treeJson->topLevelItemCount(); ++i) {
        filterNodeRecursive(ui->treeJson->topLevelItem(i), kw);
    }
    ui->treeJson->setUpdatesEnabled(true);
}

// ============================================================================
// 树交互信号 (就地双击编辑与修改监听)
// ============================================================================
void MainWindow::onTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (!item) return;
    int typeInt = item->data(1, Qt::UserRole).toInt();
    auto type = static_cast<QJsonValue::Type>(typeInt);

    if (column == 0) {
        // 双击 Key 列：直接进入重命名编辑
        ui->treeJson->editItem(item, 0);
    } else if (column == 2) {
        // 双击 Value 列：如果是简单标量类型，进入编辑；如果是 Object/Array 则展开/折叠
        if (type == QJsonValue::Object || type == QJsonValue::Array) {
            item->setExpanded(!item->isExpanded());
        } else {
            ui->treeJson->editItem(item, 2);
        }
    }
}

void MainWindow::onTreeItemChanged(QTreeWidgetItem *item, int column)
{
    if (m_isUpdating || !item) return;

    int typeInt = item->data(1, Qt::UserRole).toInt();
    auto type = static_cast<QJsonValue::Type>(typeInt);

    // 如果修改了 Value 列 (列 2)，进行类型格式校验
    if (column == 2) {
        if (type == QJsonValue::Double) {
            bool ok = false;
            item->text(2).toDouble(&ok);
            if (!ok) {
                QMessageBox::warning(this, QStringLiteral("格式错误"), QStringLiteral("当前节点类型为 Number，请输入合法的数字！"));
            }
        } else if (type == QJsonValue::Bool) {
            QString val = item->text(2).trimmed().toLower();
            if (val != QStringLiteral("true") && val != QStringLiteral("false")) {
                QMessageBox::warning(this, QStringLiteral("格式错误"), QStringLiteral("当前节点类型为 Boolean，取值只能为 true 或 false！"));
            }
        }
    }

    syncTreeToRawText();
}

// ============================================================================
// 全功能右键上下文菜单 (CRUD 增删改查与类型转换)
// ============================================================================
void MainWindow::onCustomContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeJson->itemAt(pos);
    QPoint globalPos = ui->treeJson->viewport()->mapToGlobal(pos);

    if (!item) {
        // --------------------------------------------------------------------
        // 场景 A：点击在空白处 -> 顶层节点操作
        // --------------------------------------------------------------------
        QMenu menu(this);
        QAction *actAddTopObject = menu.addAction(QStringLiteral("➕ 新增顶层 Object 节点..."));
        QAction *actAddTopArray  = menu.addAction(QStringLiteral("➕ 新增顶层 Array 节点..."));
        menu.addSeparator();
        QAction *actExpand       = menu.addAction(QStringLiteral("📂 全部展开 (expandAll)"));
        QAction *actCollapse     = menu.addAction(QStringLiteral("📁 全部折叠 (collapseAll)"));
        menu.addSeparator();
        QAction *actResetSample  = menu.addAction(QStringLiteral("🎮 重置为游戏样例配置"));

        QAction *sel = menu.exec(globalPos);
        if (sel == actAddTopObject) {
            bool ok = false;
            QString key = QInputDialog::getText(this, QStringLiteral("新增 Object 节点"),
                                                QStringLiteral("请输入字段键名 (Key)："),
                                                QLineEdit::Normal, QStringLiteral("new_module"), &ok);
            if (ok && !key.trimmed().isEmpty()) {
                auto *newItem = createTreeItem(key.trimmed(), QJsonValue::Object, QJsonObject());
                ui->treeJson->addTopLevelItem(newItem);
                syncTreeToRawText();
            }
        } else if (sel == actAddTopArray) {
            bool ok = false;
            QString key = QInputDialog::getText(this, QStringLiteral("新增 Array 节点"),
                                                QStringLiteral("请输入字段键名 (Key)："),
                                                QLineEdit::Normal, QStringLiteral("new_list"), &ok);
            if (ok && !key.trimmed().isEmpty()) {
                auto *newItem = createTreeItem(key.trimmed(), QJsonValue::Array, QJsonArray());
                ui->treeJson->addTopLevelItem(newItem);
                syncTreeToRawText();
            }
        } else if (sel == actExpand) {
            ui->treeJson->expandAll();
        } else if (sel == actCollapse) {
            ui->treeJson->collapseAll();
        } else if (sel == actResetSample) {
            onLoadSampleClicked();
        }
        return;
    }

    int typeInt = item->data(1, Qt::UserRole).toInt();
    auto type = static_cast<QJsonValue::Type>(typeInt);

    QMenu menu(this);

    // ------------------------------------------------------------------------
    // 场景 B：点击在 Object 节点上 (添加各类子字段)
    // ------------------------------------------------------------------------
    if (type == QJsonValue::Object) {
        QMenu *menuAddChild = menu.addMenu(QStringLiteral("➕ 添加子字段..."));
        QAction *actAddString = menuAddChild->addAction(QStringLiteral("🔤 String (字符串)"));
        QAction *actAddNumber = menuAddChild->addAction(QStringLiteral("🔢 Number (数字)"));
        QAction *actAddBool   = menuAddChild->addAction(QStringLiteral("🔘 Boolean (布尔值)"));
        QAction *actAddObject = menuAddChild->addAction(QStringLiteral("{ } Object (子对象)"));
        QAction *actAddArray  = menuAddChild->addAction(QStringLiteral("[ ] Array (子数组)"));

        menu.addSeparator();
        QAction *actRename = menu.addAction(QStringLiteral("✏️ 重命名键名 (Key)"));
        QAction *actCopyJson = menu.addAction(QStringLiteral("📋 复制该节点 JSON"));
        menu.addSeparator();
        QAction *actDelete = menu.addAction(QStringLiteral("🗑 删除该节点"));

        QAction *sel = menu.exec(globalPos);

        auto handleAdd = [this, item](QJsonValue::Type childType, const QJsonValue &defaultVal) {
            bool ok = false;
            QString key = QInputDialog::getText(this, QStringLiteral("新增子字段"),
                                                QStringLiteral("请输入子字段键名 (Key)："),
                                                QLineEdit::Normal, QStringLiteral("new_key"), &ok);
            if (ok && !key.trimmed().isEmpty()) {
                auto *child = createTreeItem(key.trimmed(), childType, defaultVal);
                item->addChild(child);
                item->setText(2, QStringLiteral("{ %1 个字段 }").arg(item->childCount()));
                item->setExpanded(true);
                syncTreeToRawText();
            }
        };

        if (sel == actAddString) handleAdd(QJsonValue::String, QStringLiteral("default_value"));
        else if (sel == actAddNumber) handleAdd(QJsonValue::Double, 100);
        else if (sel == actAddBool) handleAdd(QJsonValue::Bool, true);
        else if (sel == actAddObject) handleAdd(QJsonValue::Object, QJsonObject());
        else if (sel == actAddArray) handleAdd(QJsonValue::Array, QJsonArray());
        else if (sel == actRename) ui->treeJson->editItem(item, 0);
        else if (sel == actCopyJson) {
            QJsonObject obj = buildJsonObjectFromItem(item);
            QApplication::clipboard()->setText(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
            ui->labelStatus->setText(QStringLiteral("📋 节点 JSON 已成功复制到系统剪贴板。"));
        } else if (sel == actDelete) {
            QTreeWidgetItem *parent = item->parent();
            if (parent) parent->removeChild(item);
            else ui->treeJson->takeTopLevelItem(ui->treeJson->indexOfTopLevelItem(item));
            delete item;
            syncTreeToRawText();
        }
    }
    // ------------------------------------------------------------------------
    // 场景 C：点击在 Array 节点上 (追加各类元素)
    // ------------------------------------------------------------------------
    else if (type == QJsonValue::Array) {
        QMenu *menuAddElem = menu.addMenu(QStringLiteral("➕ 追加数组元素..."));
        QAction *actAddString = menuAddElem->addAction(QStringLiteral("🔤 String 元素"));
        QAction *actAddNumber = menuAddElem->addAction(QStringLiteral("🔢 Number 元素"));
        QAction *actAddBool   = menuAddElem->addAction(QStringLiteral("🔘 Boolean 元素"));
        QAction *actAddObject = menuAddElem->addAction(QStringLiteral("{ } Object 元素"));
        QAction *actAddArray  = menuAddElem->addAction(QStringLiteral("[ ] Array 元素"));

        menu.addSeparator();
        QAction *actCopyJson = menu.addAction(QStringLiteral("📋 复制该数组 JSON"));
        menu.addSeparator();
        QAction *actDelete = menu.addAction(QStringLiteral("🗑 删除该数组"));

        QAction *sel = menu.exec(globalPos);

        auto handleAppend = [this, item](QJsonValue::Type elemType, const QJsonValue &defaultVal) {
            int newIdx = item->childCount();
            QString key = QStringLiteral("[%1]").arg(newIdx);
            auto *child = createTreeItem(key, elemType, defaultVal);
            item->addChild(child);
            item->setText(2, QStringLiteral("[ %1 个元素 ]").arg(item->childCount()));
            item->setExpanded(true);
            syncTreeToRawText();
        };

        if (sel == actAddString) handleAppend(QJsonValue::String, QStringLiteral("新元素"));
        else if (sel == actAddNumber) handleAppend(QJsonValue::Double, 0);
        else if (sel == actAddBool) handleAppend(QJsonValue::Bool, true);
        else if (sel == actAddObject) handleAppend(QJsonValue::Object, QJsonObject());
        else if (sel == actAddArray) handleAppend(QJsonValue::Array, QJsonArray());
        else if (sel == actCopyJson) {
            QJsonArray arr = buildJsonArrayFromItem(item);
            QApplication::clipboard()->setText(QString::fromUtf8(QJsonDocument(arr).toJson(QJsonDocument::Indented)));
            ui->labelStatus->setText(QStringLiteral("📋 数组 JSON 已成功复制到系统剪贴板。"));
        } else if (sel == actDelete) {
            QTreeWidgetItem *parent = item->parent();
            if (parent) parent->removeChild(item);
            else ui->treeJson->takeTopLevelItem(ui->treeJson->indexOfTopLevelItem(item));
            delete item;
            syncTreeToRawText();
        }
    }
    // ------------------------------------------------------------------------
    // 场景 D：点击在 Value 标量节点上 (就地编辑与类型转换)
    // ------------------------------------------------------------------------
    else {
        QAction *actEditVal = menu.addAction(QStringLiteral("✏️ 就地编辑值 (Value)"));
        QMenu *menuChangeType = menu.addMenu(QStringLiteral("🔀 转换数据类型为..."));
        QAction *actToTypeString = menuChangeType->addAction(QStringLiteral("🔤 String (字符串)"));
        QAction *actToTypeNumber = menuChangeType->addAction(QStringLiteral("🔢 Number (数字)"));
        QAction *actToTypeBool   = menuChangeType->addAction(QStringLiteral("🔘 Boolean (布尔值)"));
        QAction *actToTypeNull   = menuChangeType->addAction(QStringLiteral("⚪ Null (空值)"));

        menu.addSeparator();
        QAction *actCopyVal = menu.addAction(QStringLiteral("📋 复制键值 (Key: Value)"));
        menu.addSeparator();
        QAction *actDelete = menu.addAction(QStringLiteral("🗑 删除该字段"));

        QAction *sel = menu.exec(globalPos);

        if (sel == actEditVal) {
            ui->treeJson->editItem(item, 2);
        } else if (sel == actToTypeString) {
            setNodeVisualAppearance(item, QJsonValue::String);
            syncTreeToRawText();
        } else if (sel == actToTypeNumber) {
            setNodeVisualAppearance(item, QJsonValue::Double);
            bool ok = false;
            item->text(2).toDouble(&ok);
            if (!ok) item->setText(2, QStringLiteral("0"));
            syncTreeToRawText();
        } else if (sel == actToTypeBool) {
            setNodeVisualAppearance(item, QJsonValue::Bool);
            item->setText(2, QStringLiteral("true"));
            syncTreeToRawText();
        } else if (sel == actToTypeNull) {
            setNodeVisualAppearance(item, QJsonValue::Null);
            item->setText(2, QStringLiteral("null"));
            syncTreeToRawText();
        } else if (sel == actCopyVal) {
            QApplication::clipboard()->setText(QStringLiteral("\"%1\": \"%2\"").arg(item->text(0)).arg(item->text(2)));
            ui->labelStatus->setText(QStringLiteral("📋 键值对已成功复制到剪贴板。"));
        } else if (sel == actDelete) {
            QTreeWidgetItem *parent = item->parent();
            if (parent) parent->removeChild(item);
            else ui->treeJson->takeTopLevelItem(ui->treeJson->indexOfTopLevelItem(item));
            delete item;
            syncTreeToRawText();
        }
    }
}
