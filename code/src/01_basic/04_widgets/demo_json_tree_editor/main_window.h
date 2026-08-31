#pragma once

#include <QMainWindow>
#include <QTreeWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <memory>

namespace Ui {
class MainWindow;
}

/**
 * @brief 可视化 JSON 树形结构编辑器 (QTreeWidget 与 QJsonDocument 双向同步)
 * 
 * 核心功能：
 * 1. 递归解析与树形呈现：支持 Object、Array、String、Double、Bool、Null 递归渲染为多层级树
 * 2. 全功能右键上下文菜单 (CRUD)：
 *    - 在 Object 节点上右键：添加子键值对 (多种数据类型)、重命名 Key、删除、复制 JSON
 *    - 在 Array 节点上右键：追加数组元素 (多种数据类型)、删除、复制 JSON
 *    - 在 Value 标量节点上右键：就地编辑值、转换数据类型、删除、复制
 *    - 在空白处右键：新建根节点、全部展开/折叠
 * 3. 就地双击编辑：双击 Key 或 Value 列直接就地修改
 * 4. 树形与源码实时双向同步：树修改自动格式化输出到源码视图；修改源码可一键同步解析回写至树
 * 5. 磁盘文件 I/O：打开 JSON 文件与持久化保存回写
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onOpenFileClicked();
    void onSaveFileClicked();
    void onLoadSampleClicked();
    void onSyncTextToTreeClicked();
    void onExpandAllClicked();
    void onCollapseAllClicked();
    void onFilterTextChanged(const QString &keyword);

    // 树交互信号
    void onTreeItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onTreeItemChanged(QTreeWidgetItem *item, int column);
    void onCustomContextMenu(const QPoint &pos);

private:
    // JSON 解析与填充树
    void loadJsonDocument(const QJsonDocument &doc);
    void populateObject(const QJsonObject &obj, QTreeWidgetItem *parent);
    void populateArray(const QJsonArray &arr, QTreeWidgetItem *parent);
    QTreeWidgetItem *createTreeItem(const QString &key, QJsonValue::Type type, const QJsonValue &val);

    // 树逆向序列化为 JSON Document
    QJsonDocument buildJsonDocumentFromTree();
    QJsonValue buildJsonValueFromItem(QTreeWidgetItem *item);
    QJsonObject buildJsonObjectFromItem(QTreeWidgetItem *item);
    QJsonArray buildJsonArrayFromItem(QTreeWidgetItem *item);

    // 同步与更新
    void syncTreeToRawText();
    void setNodeVisualAppearance(QTreeWidgetItem *item, QJsonValue::Type type);
    bool filterNodeRecursive(QTreeWidgetItem *item, const QString &keyword);

    std::unique_ptr<Ui::MainWindow> ui;
    QString m_currentFilePath;
    bool m_isUpdating{false};
};
