#include <QCoreApplication>
#include <iostream>

#include "variant_demo.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    QCoreApplication app(argc, argv);
    app.setApplicationName("demo_variant");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt QVariant 游戏数据多态与元类型实战 (demo_variant)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 5 大 QVariant 测试模块
    VariantDemo::testBasicGameAttributes();
    VariantDemo::testGameInventoryList();
    VariantDemo::testCustomStructPackingAndUnpacking();
    VariantDemo::testNestedQuestRewardConfig();
    VariantDemo::testTypeCheckingAndSafeConversion();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_variant] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
