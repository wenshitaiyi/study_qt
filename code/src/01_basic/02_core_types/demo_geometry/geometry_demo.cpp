#include "geometry_demo.h"

#include <QLineF>
#include <iostream>
#include <cmath>

static void printTitle(const QString &title)
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  " << title.toUtf8().constData() << std::endl;
    std::cout << "============================================================" << std::endl;
}

static void printLine(const QString &title, const QString &content)
{
    std::cout << QStringLiteral("  • %1 : %2")
                    .arg(title.leftJustified(28, ' '))
                    .arg(content)
                    .toUtf8().constData()
              << std::endl;
}

static QString formatRect(const QRect &r)
{
    return QStringLiteral("Rect(x:%1, y:%2, w:%3, h:%4) [居中:(%5, %6)]")
        .arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height())
        .arg(r.center().x()).arg(r.center().y());
}

void GeometryDemo::testPointsAndDistances()
{
    printTitle(QStringLiteral("[模块 1] QPoint 与 QPointF 坐标点与空间距离计算"));

    QPoint p1(30, 40);
    QPoint p2(150, 200);

    // 1. 点的位移与向量相减
    QPoint delta = p2 - p1;
    printLine(QStringLiteral("点 P1 坐标"), QStringLiteral("(%1, %2)").arg(p1.x()).arg(p1.y()));
    printLine(QStringLiteral("点 P2 坐标"), QStringLiteral("(%1, %2)").arg(p2.x()).arg(p2.y()));
    printLine(QStringLiteral("位移向量 (P2 - P1)"), QStringLiteral("(dx:%1, dy:%2)").arg(delta.x()).arg(delta.y()));

    // 2. 曼哈顿距离 (|dx| + |dy|，常用于鼠标拖拽判断 QApplication::startDragDistance())
    int manhattanDist = delta.manhattanLength();
    printLine(QStringLiteral("曼哈顿距离 (Manhattan)"), QStringLiteral("%1 像素").arg(manhattanDist));

    // 3. 欧几里得空间直线距离 (sqrt(dx^2 + dy^2))
    double euclideanDist = QLineF(p1, p2).length();
    printLine(QStringLiteral("欧氏空间距离 (Euclidean)"), QStringLiteral("%1 像素").arg(euclideanDist, 0, 'f', 2));
}

void GeometryDemo::testSizesAndScaling()
{
    printTitle(QStringLiteral("[模块 2] QSize 与 QSizeF 尺寸缩放与自适应算法"));

    // 模拟一张 16:9 的高清原始图像尺寸
    QSize imageSize(1920, 1080);
    // 模拟一个 1:1 正方形的 UI 视口控件尺寸
    QSize viewSize(400, 400);

    printLine(QStringLiteral("原始图像尺寸 (16:9)"), QStringLiteral("%1 x %2").arg(imageSize.width()).arg(imageSize.height()));
    printLine(QStringLiteral("目标展示视口 (1:1)"), QStringLiteral("%1 x %2").arg(viewSize.width()).arg(viewSize.height()));

    // 1. 保持纵横比缩放 (KeepAspectRatio：适应显示，留黑边)
    QSize fitSize = imageSize.scaled(viewSize, Qt::KeepAspectRatio);
    printLine(QStringLiteral("KeepAspectRatio (适应)"), QStringLiteral("%1 x %2 (完整展现)").arg(fitSize.width()).arg(fitSize.height()));

    // 2. 保持纵横比铺满 (KeepAspectRatioByExpanding：填满裁剪)
    QSize fillSize = imageSize.scaled(viewSize, Qt::KeepAspectRatioByExpanding);
    printLine(QStringLiteral("KeepAspectRatioByExpanding"), QStringLiteral("%1 x %2 (填满视口)").arg(fillSize.width()).arg(fillSize.height()));

    // 3. 自由拉伸忽略纵横比 (IgnoreAspectRatio)
    QSize stretchSize = imageSize.scaled(viewSize, Qt::IgnoreAspectRatio);
    printLine(QStringLiteral("IgnoreAspectRatio (拉伸)"), QStringLiteral("%1 x %2 (比例变形)").arg(stretchSize.width()).arg(stretchSize.height()));
}

void GeometryDemo::testRectanglesAndCollisions()
{
    printTitle(QStringLiteral("[模块 3] QRect 矩形几何、碰撞检测与锚点居中"));

    QRect rectA(50, 50, 200, 100);
    QRect rectB(150, 80, 150, 120);
    QRect rectC(400, 400, 50, 50);

    printLine(QStringLiteral("矩形 A"), formatRect(rectA));
    printLine(QStringLiteral("矩形 B"), formatRect(rectB));
    printLine(QStringLiteral("矩形 C"), formatRect(rectC));

    // 1. 碰撞相交检测 (intersects)
    printLine(QStringLiteral("矩形 A 与 B 是否相交碰撞"), rectA.intersects(rectB) ? QStringLiteral("相交 (True)") : QStringLiteral("不相交 (False)"));
    printLine(QStringLiteral("矩形 A 与 C 是否相交碰撞"), rectA.intersects(rectC) ? QStringLiteral("相交 (True)") : QStringLiteral("不相交 (False)"));

    // 2. 求交集区域 (intersected)
    QRect intersectionRect = rectA.intersected(rectB);
    printLine(QStringLiteral("A 与 B 重叠交集区域"), formatRect(intersectionRect));

    // 3. 求最小外接并集 (united)
    QRect unionRect = rectA.united(rectB);
    printLine(QStringLiteral("A 与 B 最小外接包围盒"), formatRect(unionRect));

    // 4. 点在矩形内判定 (contains)
    QPoint testPoint(100, 80);
    printLine(QStringLiteral("测试点 (100, 80) 在 A 内?"), rectA.contains(testPoint) ? QStringLiteral("包含 (Inside)") : QStringLiteral("外部 (Outside)"));

    // 5. 优雅的居中对齐算法 (moveCenter)
    QRect container(0, 0, 800, 600);
    QRect dialog(0, 0, 300, 200);
    dialog.moveCenter(container.center()); // 将弹窗中心对齐到容器中心
    printLine(QStringLiteral("大容器 (800x600) 居中弹窗"), formatRect(dialog));
}

void GeometryDemo::testMarginsAndPadding()
{
    printTitle(QStringLiteral("[模块 4] QMargins 外边距扩展与内边距收缩计算"));

    QRect windowRect(0, 0, 600, 400);
    QMargins padding(20, 40, 20, 30); // 左:20, 上:40 (标题栏), 右:20, 下:30 (状态栏)

    printLine(QStringLiteral("窗口整体物理区域"), formatRect(windowRect));
    printLine(QStringLiteral("内边距 Padding 配置"), QStringLiteral("左:%1, 上:%2, 右:%3, 下:%4")
        .arg(padding.left()).arg(padding.top()).arg(padding.right()).arg(padding.bottom()));

    // 1. 去除边距得到内容可用区域 (marginsRemoved)
    QRect contentRect = windowRect.marginsRemoved(padding);
    printLine(QStringLiteral("内容可用区域 (ContentRect)"), formatRect(contentRect));

    // 2. 加上边距还原外层区域 (marginsAdded)
    QRect restoredRect = contentRect.marginsAdded(padding);
    printLine(QStringLiteral("加上边距还原外层区域"), formatRect(restoredRect));
}

void GeometryDemo::testPolygonAndSelection()
{
    printTitle(QStringLiteral("[模块 5] QPolygon 多边形几何与点包含判定"));

    // 构造一个五边形顶点数组
    QPolygon polygon;
    polygon << QPoint(100, 50)
            << QPoint(200, 80)
            << QPoint(180, 220)
            << QPoint(80, 200)
            << QPoint(40, 100);

    printLine(QStringLiteral("多边形顶点数量"), QStringLiteral("%1 个顶点").arg(polygon.count()));

    // 1. 获取多边形外接包围盒 (boundingRect)
    QRect bound = polygon.boundingRect();
    printLine(QStringLiteral("多边形外接矩形 (Bound)"), formatRect(bound));

    // 2. 判定任意鼠标点击点是否落在多边形内部 (containsPoint)
    QPoint insidePoint(120, 120);
    QPoint outsidePoint(20, 20);

    printLine(QStringLiteral("点击点 (120, 120) 判定"), polygon.containsPoint(insidePoint, Qt::OddEvenFill) ? QStringLiteral("命中多边形内部 (Hit)") : QStringLiteral("未命中 (Miss)"));
    printLine(QStringLiteral("点击点 (20, 20)   判定"), polygon.containsPoint(outsidePoint, Qt::OddEvenFill) ? QStringLiteral("命中多边形内部 (Hit)") : QStringLiteral("未命中 (Miss)"));
}
