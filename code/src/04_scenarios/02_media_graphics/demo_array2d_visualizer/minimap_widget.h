#pragma once

#include <QWidget>
#include <QImage>
#include <QRectF>

class MinimapWidget : public QWidget {
    Q_OBJECT

public:
    explicit MinimapWidget(QWidget *parent = nullptr);

    void setThumbnail(const QImage &image);
    void setViewportRect(const QRectF &normalizedRect); // 归一化相机视口矩形 [0.0, 1.0]

signals:
    void requestCenterOn(double normX, double normY);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void handleMouseInteraction(const QPoint &pos);

    QImage m_thumbnail;
    QRectF m_viewportRect; // 归一化视口 [0, 1] x [0, 1]
    bool m_isDragging{false};
};
