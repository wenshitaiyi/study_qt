#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include "main_window.h"
#include "colourmap_manager.h"

int main(int argc, char *argv[])
{
    // 启用高 DPI 自适应缩放
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("demo_array2d_visualizer"));
    app.setOrganizationName(QStringLiteral("StudyQt"));

    // 路径预置与探测：必须在 main 函数中统一完成环境与数据路径初始化
    QString appDir = QCoreApplication::applicationDirPath();
    QString colourmapDir;

    // 优先检测打包与开发环境下的 app/data/colourmap
    // 候选 1: 开发目录结构 (app/debug/../data/colourmap -> app/data/colourmap)
    QDir candidate1(appDir + QStringLiteral("/../data/colourmap"));
    // 候选 2: 打包发布结构 (数据与可执行程序平级: <app_dir>/data/colourmap)
    QDir candidate2(appDir + QStringLiteral("/data/colourmap"));
    // 候选 3: 工作目录为工程根目录 (./app/data/colourmap)
    QDir candidate3(QDir::currentPath() + QStringLiteral("/app/data/colourmap"));
    // 候选 4: 工作目录下的 data/colourmap (./data/colourmap)
    QDir candidate4(QDir::currentPath() + QStringLiteral("/data/colourmap"));
    // 候选 5: 项目绝对路径备选
    QDir candidate5(QStringLiteral("D:/zcode/study/study_qt/app/data/colourmap"));

    if (candidate1.exists()) {
        colourmapDir = candidate1.canonicalPath();
    } else if (candidate2.exists()) {
        colourmapDir = candidate2.canonicalPath();
    } else if (candidate3.exists()) {
        colourmapDir = candidate3.canonicalPath();
    } else if (candidate4.exists()) {
        colourmapDir = candidate4.canonicalPath();
    } else if (candidate5.exists()) {
        colourmapDir = candidate5.canonicalPath();
    } else {
        qWarning() << "[main] Could not find app/data/colourmap directory!";
    }

    qInfo() << "[main] Target colourmap directory:" << colourmapDir;

    // 初始化公用色带管理器
    bool ok = ColourMapManager::instance().initialize(colourmapDir);
    if (!ok) {
        qWarning() << "[main] Failed to load colourmaps from disk, will use builtin batlow fallback.";
    }

    MainWindow window;
    window.show();

    return app.exec();
}
