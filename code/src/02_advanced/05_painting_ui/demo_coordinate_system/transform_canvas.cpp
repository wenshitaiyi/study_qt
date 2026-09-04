#include "transform_canvas.h"

#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QtMath>

TransformCanvas::TransformCanvas(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_OpaquePaintEvent, false);

    // 默认创建矩形图元
    m_primitive = createPrimitive(PrimitiveType::Rectangle);

    // 初始位置设置在画布合适区域
    m_box.setCenter(QPointF(400.0, 300.0));
    m_box.setSize(QSizeF(240.0, 160.0));
}

void TransformCanvas::setPrimitiveType(PrimitiveType type) {
    m_primitive = createPrimitive(type);
    update();
    emitStatus();
    emit transformChanged();
}

void TransformCanvas::setBoundingBoxVisible(bool v) {
    m_box.setVisible(v);
    update();
    emitStatus();
}

void TransformCanvas::setViewportEnabled(bool e) {
    m_viewport.setEnabled(e);
    update();
    emitStatus();
    emit viewportChanged();
}

void TransformCanvas::setViewportBorderVisible(bool v) {
    m_viewport.setBorderVisible(v);
    update();
    emitStatus();
}

void TransformCanvas::setViewportRect(const QRectF &rect) {
    m_viewport.setRect(rect);
    update();
    emitStatus();
    emit viewportChanged();
}

void TransformCanvas::setRotation(qreal deg) {
    m_box.setRotation(deg);
    update();
    emitStatus();
    emit transformChanged();
}

void TransformCanvas::setPrimitiveSize(const QSizeF &s) {
    m_box.setSize(s);
    update();
    emitStatus();
    emit transformChanged();
}

void TransformCanvas::setPrimitiveCenter(const QPointF &c) {
    m_box.setCenter(c);
    update();
    emitStatus();
    emit transformChanged();
}

QRectF TransformCanvas::activeContainerRect() const {
    if (m_viewport.isEnabled()) {
        return m_viewport.rect();
    }
    return rect();
}

void TransformCanvas::resetFitInView() {
    m_box.fitInRect(activeContainerRect(), 0.82);
    update();
    emitStatus();
    emit transformChanged();
}

void TransformCanvas::resetFitWidthCenter() {
    m_box.fitWidthCenter(activeContainerRect(), 0.85);
    update();
    emitStatus();
    emit transformChanged();
}

void TransformCanvas::resetFitHeightCenter() {
    m_box.fitHeightCenter(activeContainerRect(), 0.85);
    update();
    emitStatus();
    emit transformChanged();
}

void TransformCanvas::resetRotationOnly() {
    m_box.resetRotation();
    update();
    emitStatus();
    emit transformChanged();
}

QString TransformCanvas::statusSummary() const {
    QTransform t = m_box.transform();
    return QStringLiteral("【矩阵】m11:%1, m12:%2, m21:%3, m22:%4, dx:%5, dy:%6 | "
                          "【图元】中心:(%7, %8) 尺寸:%9x%10 旋转:%11° | "
                          "【视口】%12")
        .arg(t.m11(), 0, 'f', 2)
        .arg(t.m12(), 0, 'f', 2)
        .arg(t.m21(), 0, 'f', 2)
        .arg(t.m22(), 0, 'f', 2)
        .arg(t.dx(), 0, 'f', 1)
        .arg(t.dy(), 0, 'f', 1)
        .arg(m_box.center().x(), 0, 'f', 1)
        .arg(m_box.center().y(), 0, 'f', 1)
        .arg(m_box.size().width(), 0, 'f', 1)
        .arg(m_box.size().height(), 0, 'f', 1)
        .arg(m_box.rotation(), 0, 'f', 1)
        .arg(m_viewport.isEnabled() ? QStringLiteral("开启 (%1x%2)")
                                          .arg(qRound(m_viewport.rect().width()))
                                          .arg(qRound(m_viewport.rect().height()))
                                    : QStringLiteral("关闭 (全画布)"));
}

void TransformCanvas::emitStatus() {
    emit statusChanged(statusSummary());
}

bool TransformCanvas::event(QEvent *event) {
    // 拦截 Alt 键按键与松开事件，防止 Windows 将焦点夺走去激活窗口系统菜单
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Alt) {
            event->accept();
            return true;
        }
    }
    return QWidget::event(event);
}

void TransformCanvas::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    static bool s_initialized = false;
    if (!s_initialized && width() > 300 && height() > 200) {
        s_initialized = true;
        qreal vw = qMin(640.0, width() * 0.78);
        qreal vh = qMin(460.0, height() * 0.76);
        qreal vx = (width() - vw) / 2.0;
        qreal vy = (height() - vh) / 2.0;
        m_viewport.setRect(QRectF(vx, vy, vw, vh));

        m_box.setCenter(rect().center());
        emitStatus();
        emit transformChanged();
        emit viewportChanged();
    }
}

void TransformCanvas::drawBackgroundGrid(QPainter &painter) {
    painter.save();
    painter.fillRect(rect(), QColor(248, 250, 252));

    QPen minorPen(QColor(226, 232, 240), 1.0);
    QPen majorPen(QColor(203, 213, 225), 1.2);

    int w = width();
    int h = height();

    painter.setPen(minorPen);
    for (int x = 0; x < w; x += 20) {
        painter.drawLine(x, 0, x, h);
    }
    for (int y = 0; y < h; y += 20) {
        painter.drawLine(0, y, w, y);
    }

    painter.setPen(majorPen);
    for (int x = 0; x < w; x += 100) {
        painter.drawLine(x, 0, x, h);
    }
    for (int y = 0; y < h; y += 100) {
        painter.drawLine(0, y, w, y);
    }

    painter.restore();
}

void TransformCanvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 1. 全局工程网格
    drawBackgroundGrid(painter);

    // 2. 外部视口蒙版
    if (m_viewport.isEnabled()) {
        m_viewport.drawMask(painter, rect());
    }

    // 3. 视口裁剪沙箱
    if (m_viewport.isEnabled()) {
        painter.save();
        painter.setClipRect(m_viewport.rect());
    }

    // 4. 应用矩阵并绘制内部图元
    if (m_primitive) {
        painter.save();
        painter.setTransform(m_box.transform(), true);
        m_primitive->draw(painter, m_box.size());
        painter.restore();
    }

    // 5. 绘制定向包围盒（OBB）
    m_box.draw(painter, m_hoveredBoxHandle);

    // 6. 恢复视口上下文
    if (m_viewport.isEnabled()) {
        painter.restore();
    }

    // 7. 绘制视口边框
    if (m_viewport.isEnabled()) {
        m_viewport.drawBorder(painter, m_hoveredVpHandle);
    }
}

void TransformCanvas::updateCursorShape(const QPointF &mousePos) {
    if (m_dragMode != DragMode::None) return;

    if (m_viewport.isEnabled() && m_viewport.isBorderVisible()) {
        ViewportHandle vh = m_viewport.hitTestHandle(mousePos);
        m_hoveredVpHandle = vh;
        if (vh != ViewportHandle::None) {
            switch (vh) {
            case ViewportHandle::TopLeft:
            case ViewportHandle::BottomRight:
                setCursor(Qt::SizeFDiagCursor);
                return;
            case ViewportHandle::TopRight:
            case ViewportHandle::BottomLeft:
                setCursor(Qt::SizeBDiagCursor);
                return;
            case ViewportHandle::Top:
            case ViewportHandle::Bottom:
                setCursor(Qt::SizeVerCursor);
                return;
            case ViewportHandle::Left:
            case ViewportHandle::Right:
                setCursor(Qt::SizeHorCursor);
                return;
            default:
                break;
            }
        }

        if (m_viewport.hitTestBorder(mousePos)) {
            setCursor(Qt::SizeAllCursor);
            return;
        }
    } else {
        m_hoveredVpHandle = ViewportHandle::None;
    }

    bool insideVpIfEnabled = !m_viewport.isEnabled() || m_viewport.contains(mousePos);
    if (insideVpIfEnabled && m_box.isVisible()) {
        BoxHandle bh = m_box.hitTestHandle(mousePos);
        m_hoveredBoxHandle = bh;
        if (bh != BoxHandle::None) {
            setCursor(Qt::CrossCursor);
            return;
        }
    } else {
        m_hoveredBoxHandle = BoxHandle::None;
    }

    if (insideVpIfEnabled && m_box.containsPoint(mousePos)) {
        setCursor(Qt::OpenHandCursor);
        return;
    }

    setCursor(Qt::ArrowCursor);
}

void TransformCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    QPointF pos = event->pos();
    m_lastMousePos = pos;

    if (m_viewport.isEnabled() && m_viewport.isBorderVisible()) {
        ViewportHandle vh = m_viewport.hitTestHandle(pos);
        if (vh != ViewportHandle::None) {
            m_dragMode = DragMode::StretchingViewport;
            m_activeVpHandle = vh;
            return;
        }

        if (m_viewport.hitTestBorder(pos)) {
            m_dragMode = DragMode::DraggingViewport;
            setCursor(Qt::SizeAllCursor);
            return;
        }
    }

    bool insideVp = !m_viewport.isEnabled() || m_viewport.contains(pos);
    if (insideVp && m_box.isVisible()) {
        BoxHandle bh = m_box.hitTestHandle(pos);
        if (bh != BoxHandle::None) {
            m_dragMode = DragMode::StretchingPrimitive;
            m_activeBoxHandle = bh;
            return;
        }
    }

    if (insideVp && m_box.containsPoint(pos)) {
        m_dragMode = DragMode::DraggingPrimitive;
        setCursor(Qt::ClosedHandCursor);
        return;
    }

    m_dragMode = DragMode::None;
}

void TransformCanvas::mouseMoveEvent(QMouseEvent *event) {
    QPointF pos = event->pos();
    QPointF delta = pos - m_lastMousePos;

    if (m_dragMode == DragMode::DraggingPrimitive) {
        m_box.translate(delta);
        m_lastMousePos = pos;
        update();
        emitStatus();
        emit transformChanged();
        return;
    }

    if (m_dragMode == DragMode::StretchingPrimitive) {
        m_box.stretchHandle(m_activeBoxHandle, pos);
        m_lastMousePos = pos;
        update();
        emitStatus();
        emit transformChanged();
        return;
    }

    if (m_dragMode == DragMode::DraggingViewport) {
        m_viewport.translate(delta);
        m_lastMousePos = pos;
        update();
        emitStatus();
        emit viewportChanged();
        return;
    }

    if (m_dragMode == DragMode::StretchingViewport) {
        m_viewport.stretchHandle(m_activeVpHandle, pos);
        m_lastMousePos = pos;
        update();
        emitStatus();
        emit viewportChanged();
        return;
    }

    updateCursorShape(pos);
    update();
}

void TransformCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragMode = DragMode::None;
        m_activeBoxHandle = BoxHandle::None;
        m_activeVpHandle = ViewportHandle::None;
        updateCursorShape(event->pos());
        update();
    }
    QWidget::mouseReleaseEvent(event);
}

void TransformCanvas::wheelEvent(QWheelEvent *event) {
    QPointF mousePos = event->pos();

    // 兼顾 Windows 下 Alt+滚轮 可能产生的纵向或横向滚动增量
    int numDegrees = event->angleDelta().y();
    if (numDegrees == 0) {
        numDegrees = event->angleDelta().x();
    }
    if (numDegrees == 0) {
        event->ignore();
        return;
    }

    if (m_viewport.isEnabled() && !m_viewport.contains(mousePos)) {
        event->ignore();
        return;
    }

    // 获取混合键盘修饰键（从事件及从应用程序全局双重检测，确保 Alt 准确捕获）
    Qt::KeyboardModifiers mods = event->modifiers() | QApplication::keyboardModifiers();
    bool ctrlPressed = (mods & Qt::ControlModifier);
    bool altPressed = (mods & Qt::AltModifier);
    bool shiftPressed = (mods & Qt::ShiftModifier);

    // 1. 优先判定 Alt 旋转：Alt + 滚轮（光标轴心公转），Alt + Shift + 滚轮（图元中心自转）
    if (altPressed) {
        qreal angleStep = (numDegrees > 0) ? 5.0 : -5.0;
        QPointF pivot = shiftPressed ? m_box.center() : mousePos;
        m_box.rotateAround(pivot, angleStep);
        update();
        emitStatus();
        emit transformChanged();
        event->accept();
        return;
    }

    // 2. Ctrl 缩放：Ctrl + 滚轮（光标不动点缩放），Ctrl + Shift + 滚轮（中心不动点缩放）
    if (ctrlPressed) {
        qreal factor = (numDegrees > 0) ? 1.12 : (1.0 / 1.12);
        QPointF pivot = shiftPressed ? m_box.center() : mousePos;
        m_box.scaleAround(pivot, factor);
        update();
        emitStatus();
        emit transformChanged();
        event->accept();
        return;
    }

    event->ignore();
}
