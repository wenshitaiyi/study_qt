#pragma once

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class PageGroupBox;
class PageTabWidget;
class PageToolBox;
class PageScrollArea;
class PageSplitter;

/**
 * @brief Qt 常用容器控件使用指南主窗口 (主 UI + 多子 UI 模块化架构)
 * 
 * 顶层通过 QComboBox 作为多视图导航器，中央使用 QStackedWidget 承载 5 个独立的子 UI 页面：
 * 1. PageGroupBox: QGroupBox (标准、自锁 Checkable、扁平 Flat)
 * 2. PageTabWidget: QTabWidget (方位切换、可关闭、可拖拽排序、角标挂载)
 * 3. PageToolBox: QToolBox (抽屉式手风琴折叠面板)
 * 4. PageScrollArea: QScrollArea (滚动区域、尺寸自适应、滚动条策略)
 * 5. PageSplitter: QSplitter (动态水平/垂直可拖拽拆分条)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onNavigationChanged(int index);
    void onSubPageLogReceived(const QString &category, const QString &message);

private:
    void initSubPages();

    std::unique_ptr<Ui::MainWindow> ui;

    PageGroupBox   *m_pageGroupBox{nullptr};
    PageTabWidget  *m_pageTabWidget{nullptr};
    PageToolBox    *m_pageToolBox{nullptr};
    PageScrollArea *m_pageScrollArea{nullptr};
    PageSplitter   *m_pageSplitter{nullptr};
};
