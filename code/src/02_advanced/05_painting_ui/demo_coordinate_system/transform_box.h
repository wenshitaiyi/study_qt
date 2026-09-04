#ifndef TRANSFORM_BOX_H
#define TRANSFORM_BOX_H

#include <QMap>
#include <QPainter>
#include <QPointF>
#include <QPolygonF>
#include <QRectF>
#include <QSizeF>
#include <QTransform>

/**
 * @brief 定向包围盒控制点手柄类型
 */
enum class BoxHandle {
    None,
    TopLeft,
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left
};

/**
 * @brief 定向包围盒（OBB, Oriented Bounding Box）及其仿射变换数学模型
 */
class TransformBox {
public:
    TransformBox();

    // 基础几何属性
    void setCenter(const QPointF &c) { m_center = c; }
    QPointF center() const { return m_center; }

    void setSize(const QSizeF &s);
    QSizeF size() const { return m_size; }

    void setRotation(qreal deg);
    qreal rotation() const { return m_rotation; }

    void setVisible(bool v) { m_visible = v; }
    bool isVisible() const { return m_visible; }

    qreal aspectRatio() const { return m_aspectRatio; }
    void setAspectRatio(qreal ar) { if (ar > 0.01) m_aspectRatio = ar; }

    // 变换矩阵生成
    QTransform transform() const;
    QTransform invertedTransform() const;

    // 坐标映射
    QPointF mapToWorld(const QPointF &localPt) const;
    QPointF mapFromWorld(const QPointF &worldPt) const;
    QPolygonF worldPolygon() const;
    QRectF localRect() const;

    // 命中测试
    bool containsPoint(const QPointF &worldPt) const;
    BoxHandle hitTestHandle(const QPointF &worldPt, qreal tolerance = 8.0) const;

    // 8 个手柄的世界坐标表
    QMap<BoxHandle, QPointF> handleWorldPositions() const;
    static BoxHandle oppositeHandle(BoxHandle handle);

    // 交互操作变换
    void translate(const QPointF &delta);
    void rotateAround(const QPointF &pivot, qreal deltaAngle);
    void scaleAround(const QPointF &pivot, qreal scaleFactor);
    void stretchHandle(BoxHandle handle, const QPointF &worldMousePos);

    // 重置定位（保持长宽比不变，角度摆正为0）
    void resetRotation();
    void fitInRect(const QRectF &targetRect, qreal marginRatio = 0.85);
    void fitWidthCenter(const QRectF &targetRect, qreal marginRatio = 0.85);
    void fitHeightCenter(const QRectF &targetRect, qreal marginRatio = 0.85);

    // 绘制定向包围盒及手柄
    void draw(QPainter &painter, BoxHandle hoveredHandle = BoxHandle::None) const;

private:
    QPointF m_center{400.0, 300.0};
    QSizeF m_size{260.0, 180.0};
    qreal m_rotation{0.0};          // 顺时针旋转角度 (0~360)
    qreal m_aspectRatio{260.0 / 180.0}; // 固有纵横比
    bool m_visible{true};           // 包围盒与控制点是否可见
};

#endif // TRANSFORM_BOX_H
