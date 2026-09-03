#pragma once

#include <QString>
#include <QColor>
#include <QImage>
#include <vector>

struct ColourStop {
    double position{0.0}; // 归一化区间 [0.0, 1.0]
    int r{0};
    int g{0};
    int b{0};
    int a{255};
};

class ColourMap {
public:
    ColourMap();
    explicit ColourMap(const QString &name);

    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    bool loadFromFile(const QString &filePath);
    void addStop(double position, int r, int g, int b, int a = 255);
    void clear();

    const std::vector<ColourStop>& stops() const { return m_stops; }
    bool isValid() const { return !m_stops.empty(); }

    // 在归一化区间 [0.0, 1.0] 连续采样插值颜色
    QColor sample(double normalizedValue) const;

    // 预计算 256 级 QRgb 查找表（LUT），用于百万级点阵纳秒级取色
    std::vector<QRgb> generateLut256() const;

    // 生成色带渐变预览条图片
    QImage generatePreviewBar(int width, int height, bool horizontal = true) const;

private:
    QString m_name;
    std::vector<ColourStop> m_stops;
};
