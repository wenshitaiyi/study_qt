#ifndef CUSTOM_VIEWPORT_H
#define CUSTOM_VIEWPORT_H

#include <QMap>
#include <QPainter>
#include <QPointF>
#include <QRectF>

/**
 * @brief 外部视口调整句柄类型
 */
enum class ViewportHandle {
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
 * @brief 自定义外部视口（Custom External Viewport）
 *        支持独立拉伸、拖拽、边框显隐控制，并对画布提供裁剪区域
 */
class CustomViewport {
public:
    CustomViewport();

    void setEnabled(bool e) { m_enabled = e; }
    bool isEnabled() const { return m_enabled; }

    void setRect(const QRectF &r);
    QRectF rect() const { return m_rect; }

    void setBorderVisible(bool v) { m_borderVisible = v; }
    bool isBorderVisible() const { return m_borderVisible; }

    // 命中测试
    bool contains(const QPointF &pt) const { return m_enabled && m_rect.contains(pt); }
    ViewportHandle hitTestHandle(const QPointF &pt, qreal tolerance = 7.0) const;
    bool hitTestBorder(const QPointF &pt, qreal tolerance = 6.0) const;

    // 交互操作
    void translate(const QPointF &delta);
    void stretchHandle(ViewportHandle handle, const QPointF &mousePos);

    // 获取 8 个调整句柄的坐标
    QMap<ViewportHandle, QPointF> handlePositions() const;

    // 绘制视口蒙版、边框与手柄
    void drawMask(QPainter &painter, const QRectF &widgetRect) const;
    void drawBorder(QPainter &painter, ViewportHandle hoveredHandle = ViewportHandle::None) const;

private:
    bool m_enabled{false};          // 视口功能是否开启
    QRectF m_rect{80.0, 60.0, 640.0, 460.0}; // 视口有效几何矩形
    bool m_borderVisible{true};     // 视口边界线框是否可见
};

#endif // CUSTOM_VIEWPORT_H
