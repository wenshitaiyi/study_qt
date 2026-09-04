#ifndef PRIMITIVE_SHAPE_H
#define PRIMITIVE_SHAPE_H

#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QRectF>
#include <memory>

/**
 * @brief 图元类型枚举
 */
enum class PrimitiveType {
    Rectangle,
    Triangle,
    Hexagon,
    Image
};

/**
 * @brief 图元基类：负责在局部坐标系（以 (0,0) 为几何中心）中定义几何外观与自绘逻辑
 */
class PrimitiveShape {
public:
    virtual ~PrimitiveShape() = default;

    virtual PrimitiveType type() const = 0;
    virtual QString name() const = 0;

    // 在局部坐标系下执行绘制
    virtual void draw(QPainter &painter, const QSizeF &size) = 0;

    // 获取在局部坐标系下的封闭路径（用于碰撞或精准轮廓）
    virtual QPainterPath shapePath(const QSizeF &size) const = 0;

    // 基础画笔画刷属性
    void setStrokeColor(const QColor &c) { m_strokeColor = c; }
    QColor strokeColor() const { return m_strokeColor; }

    void setFillColor(const QColor &c) { m_fillColor = c; }
    QColor fillColor() const { return m_fillColor; }

    void setLineWidth(qreal w) { m_lineWidth = w; }
    qreal lineWidth() const { return m_lineWidth; }

protected:
    QColor m_strokeColor{QColor(30, 144, 255)};
    QColor m_fillColor{QColor(30, 144, 255, 60)};
    qreal m_lineWidth{2.5};
};

/**
 * @brief 矩形图元
 */
class RectanglePrimitive : public PrimitiveShape {
public:
    PrimitiveType type() const override { return PrimitiveType::Rectangle; }
    QString name() const override { return QStringLiteral("矩形"); }

    void draw(QPainter &painter, const QSizeF &size) override;
    QPainterPath shapePath(const QSizeF &size) const override;
};

/**
 * @brief 三角形图元
 */
class TrianglePrimitive : public PrimitiveShape {
public:
    PrimitiveType type() const override { return PrimitiveType::Triangle; }
    QString name() const override { return QStringLiteral("三角形"); }

    void draw(QPainter &painter, const QSizeF &size) override;
    QPainterPath shapePath(const QSizeF &size) const override;
};

/**
 * @brief 正六边形图元
 */
class HexagonPrimitive : public PrimitiveShape {
public:
    PrimitiveType type() const override { return PrimitiveType::Hexagon; }
    QString name() const override { return QStringLiteral("正六边形"); }

    void draw(QPainter &painter, const QSizeF &size) override;
    QPainterPath shapePath(const QSizeF &size) const override;
};

/**
 * @brief 图片位图图元
 */
class ImagePrimitive : public PrimitiveShape {
public:
    ImagePrimitive();
    PrimitiveType type() const override { return PrimitiveType::Image; }
    QString name() const override { return QStringLiteral("图片"); }

    void draw(QPainter &painter, const QSizeF &size) override;
    QPainterPath shapePath(const QSizeF &size) const override;

    void setPixmap(const QPixmap &pixmap) { m_pixmap = pixmap; }
    QPixmap pixmap() const { return m_pixmap; }

private:
    void generateDefaultPixmap();
    QPixmap m_pixmap;
};

/**
 * @brief 图元工厂方法
 */
std::unique_ptr<PrimitiveShape> createPrimitive(PrimitiveType type);

#endif // PRIMITIVE_SHAPE_H
