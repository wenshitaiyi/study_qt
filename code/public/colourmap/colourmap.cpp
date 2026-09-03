#include "colourmap.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QPainter>
#include <algorithm>
#include <cmath>

ColourMap::ColourMap()
    : m_name(QStringLiteral("default"))
{
}

ColourMap::ColourMap(const QString &name)
    : m_name(name)
{
}

bool ColourMap::loadFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QFileInfo info(filePath);
    m_name = info.baseName();
    m_stops.clear();

    struct RawNode {
        double idx;
        int r, g, b, a;
    };
    std::vector<RawNode> rawNodes;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith(QLatin1Char('#')) || line.startsWith(QStringLiteral("ColorMap"), Qt::CaseInsensitive)) {
            continue;
        }

        QStringList parts = line.split(QRegExp(QStringLiteral("\\s+")), Qt::SkipEmptyParts);
        if (parts.size() >= 4) {
            // 支持两种格式：
            // 1. 5 个字段: index r g b a
            // 2. 4 个字段: r g b a 或者 index r g b
            if (parts.size() >= 5) {
                double idx = parts[0].toDouble();
                int r = parts[1].toInt();
                int g = parts[2].toInt();
                int b = parts[3].toInt();
                int a = parts[4].toInt();
                rawNodes.push_back({idx, r, g, b, a});
            } else if (parts.size() == 4) {
                // 默认 4 个字段视作 r g b a，index 随序号递增
                int r = parts[0].toInt();
                int g = parts[1].toInt();
                int b = parts[2].toInt();
                int a = parts[3].toInt();
                rawNodes.push_back({static_cast<double>(rawNodes.size()), r, g, b, a});
            }
        }
    }
    file.close();

    if (rawNodes.empty()) {
        return false;
    }

    // 归一化位置到 [0.0, 1.0]
    double minIdx = rawNodes.front().idx;
    double maxIdx = rawNodes.back().idx;
    if (std::abs(maxIdx - minIdx) < 1e-6) {
        maxIdx = minIdx + 1.0;
    }

    m_stops.reserve(rawNodes.size());
    for (size_t i = 0; i < rawNodes.size(); ++i) {
        double pos = 0.0;
        if (rawNodes.size() > 1) {
            pos = static_cast<double>(i) / static_cast<double>(rawNodes.size() - 1);
        }
        addStop(pos, rawNodes[i].r, rawNodes[i].g, rawNodes[i].b, rawNodes[i].a);
    }

    return !m_stops.empty();
}

void ColourMap::addStop(double position, int r, int g, int b, int a)
{
    ColourStop stop;
    stop.position = std::clamp(position, 0.0, 1.0);
    stop.r = std::clamp(r, 0, 255);
    stop.g = std::clamp(g, 0, 255);
    stop.b = std::clamp(b, 0, 255);
    stop.a = std::clamp(a, 0, 255);
    m_stops.push_back(stop);

    // 保持按 position 升序排列
    std::sort(m_stops.begin(), m_stops.end(), [](const ColourStop &s1, const ColourStop &s2) {
        return s1.position < s2.position;
    });
}

void ColourMap::clear()
{
    m_stops.clear();
}

QColor ColourMap::sample(double normalizedValue) const
{
    if (m_stops.empty()) {
        return Qt::black;
    }

    double t = std::clamp(normalizedValue, 0.0, 1.0);
    if (m_stops.size() == 1 || t <= m_stops.front().position) {
        const auto &s = m_stops.front();
        return QColor(s.r, s.g, s.b, s.a);
    }
    if (t >= m_stops.back().position) {
        const auto &s = m_stops.back();
        return QColor(s.r, s.g, s.b, s.a);
    }

    // 二分查找或顺序搜索
    for (size_t i = 0; i < m_stops.size() - 1; ++i) {
        if (t >= m_stops[i].position && t <= m_stops[i + 1].position) {
            double range = m_stops[i + 1].position - m_stops[i].position;
            double factor = (range > 1e-8) ? (t - m_stops[i].position) / range : 0.0;

            int r = static_cast<int>(std::round(m_stops[i].r + factor * (m_stops[i + 1].r - m_stops[i].r)));
            int g = static_cast<int>(std::round(m_stops[i].g + factor * (m_stops[i + 1].g - m_stops[i].g)));
            int b = static_cast<int>(std::round(m_stops[i].b + factor * (m_stops[i + 1].b - m_stops[i].b)));
            int a = static_cast<int>(std::round(m_stops[i].a + factor * (m_stops[i + 1].a - m_stops[i].a)));

            return QColor(std::clamp(r, 0, 255),
                          std::clamp(g, 0, 255),
                          std::clamp(b, 0, 255),
                          std::clamp(a, 0, 255));
        }
    }

    return QColor(m_stops.back().r, m_stops.back().g, m_stops.back().b, m_stops.back().a);
}

std::vector<QRgb> ColourMap::generateLut256() const
{
    std::vector<QRgb> lut(256);
    for (int i = 0; i < 256; ++i) {
        double t = static_cast<double>(i) / 255.0;
        lut[i] = sample(t).rgba();
    }
    return lut;
}

QImage ColourMap::generatePreviewBar(int width, int height, bool horizontal) const
{
    if (width <= 0 || height <= 0) {
        return QImage();
    }

    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
    std::vector<QRgb> lut = generateLut256();

    if (horizontal) {
        for (int x = 0; x < width; ++x) {
            int lutIdx = std::clamp(static_cast<int>((static_cast<double>(x) / (width - 1)) * 255.0), 0, 255);
            QRgb rgb = lut[lutIdx];
            for (int y = 0; y < height; ++y) {
                image.setPixel(x, y, rgb);
            }
        }
    } else {
        for (int y = 0; y < height; ++y) {
            // 纵向通常自下而上表示由低到高
            int lutIdx = std::clamp(static_cast<int>((1.0 - static_cast<double>(y) / (height - 1)) * 255.0), 0, 255);
            QRgb rgb = lut[lutIdx];
            for (int x = 0; x < width; ++x) {
                image.setPixel(x, y, rgb);
            }
        }
    }

    return image;
}
