#ifndef TRANSFORM_CANVAS_H
#define TRANSFORM_CANVAS_H

#include "custom_viewport.h"
#include "primitive_shape.h"
#include "transform_box.h"

#include <QWidget>
#include <memory>

/**
 * @brief 自定义自绘画布组件
 *        负责 paintEvent 双缓冲绘制、矩阵变换调度、鼠标手势与多模式滚轮缩放旋转
 */
class TransformCanvas : public QWidget {
    Q_OBJECT

public:
    explicit TransformCanvas(QWidget *parent = nullptr);
    ~TransformCanvas() override = default;

    // 图元管理
    void setPrimitiveType(PrimitiveType type);
    PrimitiveType primitiveType() const { return m_primitive ? m_primitive->type() : PrimitiveType::Rectangle; }

    // 包围盒控制
    void setBoundingBoxVisible(bool v);
    bool isBoundingBoxVisible() const { return m_box.isVisible(); }

    // 外部视口控制
    void setViewportEnabled(bool e);
    bool isViewportEnabled() const { return m_viewport.isEnabled(); }

    void setViewportBorderVisible(bool v);
    bool isViewportBorderVisible() const { return m_viewport.isBorderVisible(); }

    void setViewportRect(const QRectF &rect);
    QRectF viewportRect() const { return m_viewport.rect(); }

    // 属性直接读写与微调联动
    qreal rotation() const { return m_box.rotation(); }
    void setRotation(qreal deg);

    QSizeF primitiveSize() const { return m_box.size(); }
    void setPrimitiveSize(const QSizeF &s);

    QPointF primitiveCenter() const { return m_box.center(); }
    void setPrimitiveCenter(const QPointF &c);

    const TransformBox &transformBox() const { return m_box; }
    const CustomViewport &customViewport() const { return m_viewport; }

    // 重置定位
    void resetFitInView();
    void resetFitWidthCenter();
    void resetFitHeightCenter();
    void resetRotationOnly();

    // 状态信息导出
    QString statusSummary() const;

signals:
    void statusChanged(const QString &text);
    void transformChanged();
    void viewportChanged();

protected:
    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    enum class DragMode {
        None,
        DraggingPrimitive,
        StretchingPrimitive,
        DraggingViewport,
        StretchingViewport
    };

    void drawBackgroundGrid(QPainter &painter);
    QRectF activeContainerRect() const;
    void updateCursorShape(const QPointF &mousePos);
    void emitStatus();

private:
    std::unique_ptr<PrimitiveShape> m_primitive;
    TransformBox m_box;
    CustomViewport m_viewport;

    DragMode m_dragMode{DragMode::None};
    BoxHandle m_activeBoxHandle{BoxHandle::None};
    BoxHandle m_hoveredBoxHandle{BoxHandle::None};

    ViewportHandle m_activeVpHandle{ViewportHandle::None};
    ViewportHandle m_hoveredVpHandle{ViewportHandle::None};

    QPointF m_lastMousePos;
};

#endif // TRANSFORM_CANVAS_H
