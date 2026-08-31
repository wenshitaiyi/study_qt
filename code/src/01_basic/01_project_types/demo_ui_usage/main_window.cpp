#include "main_window.h"
#include "ui_main_window.h"

#include "panel_pointer_member.h"
#include "panel_direct_member.h"
#include "panel_multi_inherit.h"
#include "panel_pure_code.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(std::make_unique<Ui::MainWindow>())
{
    m_ui->setupUi(this);

    initTabs();

    statusBar()->showMessage(QStringLiteral("就绪：欢迎体验 Qt 4 种界面构建与 UI 组织方式"));
}

MainWindow::~MainWindow() = default;

void MainWindow::initTabs()
{
    auto *panel1 = new PanelPointerMember(this);
    auto *panel2 = new PanelDirectMember(this);
    auto *panel3 = new PanelMultiInherit(this);
    auto *panel4 = new PanelPureCode(this);

    m_ui->tabWidget->addTab(panel1, QStringLiteral("1. 指针成员方式 (Pointer Member)"));
    m_ui->tabWidget->addTab(panel2, QStringLiteral("2. 值对象成员方式 (Direct Member)"));
    m_ui->tabWidget->addTab(panel3, QStringLiteral("3. 多重继承方式 (Multiple Inheritance)"));
    m_ui->tabWidget->addTab(panel4, QStringLiteral("4. 纯代码构建 (Pure Code Layout)"));
}
