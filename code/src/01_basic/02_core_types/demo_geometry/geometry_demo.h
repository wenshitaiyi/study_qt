#pragma once

#include <QString>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QRect>
#include <QRectF>
#include <QMargins>
#include <QMarginsF>
#include <QPolygon>
#include <QPolygonF>

class GeometryDemo
{
public:
    // 1. QPoint 与 QPointF (坐标点、位移向量与欧氏/曼哈顿距离)
    static void testPointsAndDistances();

    // 2. QSize 与 QSizeF (尺寸缩放算法与纵横比保持)
    static void testSizesAndScaling();

    // 3. QRect 与 QRectF (矩形碰撞检测、包含检测、交集并集与锚点定位)
    static void testRectanglesAndCollisions();

    // 4. QMargins 与 QMarginsF (边距扩张/内缩与布局计算)
    static void testMarginsAndPadding();

    // 5. QPolygon 与 QPolygonF (多边形外接包围盒与点在多边形内判定)
    static void testPolygonAndSelection();
};
