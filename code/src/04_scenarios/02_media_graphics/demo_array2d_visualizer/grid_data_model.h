#pragma once

#include "noise_generator.h"
#include <QObject>
#include <QFutureWatcher>
#include <vector>

struct GridParams {
    int rows{150};
    int cols{150};
    double xMin{-5.0};
    double xMax{5.0};
    double yMin{-5.0};
    double yMax{5.0};
    NoiseType noiseType{NoiseType::Perlin};
    int seed{1337};
    float frequency{1.0f};
    int octaves{4};
    float persistence{0.5f};
};

class GridDataModel : public QObject {
    Q_OBJECT

public:
    explicit GridDataModel(QObject *parent = nullptr);
    ~GridDataModel() override;

    int rows() const { return m_rows; }
    int cols() const { return m_cols; }
    int totalCells() const { return m_rows * m_cols; }

    double xMin() const { return m_xMin; }
    double xMax() const { return m_xMax; }
    double yMin() const { return m_yMin; }
    double yMax() const { return m_yMax; }

    float minValue() const { return m_minValue; }
    float maxValue() const { return m_maxValue; }

    inline float valueAt(int r, int c) const {
        if (r < 0 || r >= m_rows || c < 0 || c >= m_cols) {
            return 0.0f;
        }
        return m_values[static_cast<size_t>(r) * m_cols + c];
    }

    // 归一化数值到 [0.0, 1.0]
    inline float normalizedValueAt(int r, int c) const {
        float val = valueAt(r, c);
        float diff = m_maxValue - m_minValue;
        if (diff < 1e-6f) {
            return 0.5f;
        }
        return std::clamp((val - m_minValue) / diff, 0.0f, 1.0f);
    }

    const std::vector<float>& rawValues() const { return m_values; }

    bool isGenerating() const;

public slots:
    void generateAsync(const GridParams &params);
    void generateSync(const GridParams &params);

signals:
    void generationStarted();
    void generationFinished();
    void dataChanged();

private:
    void executeGeneration(const GridParams &params);

    int m_rows{0};
    int m_cols{0};
    double m_xMin{-5.0};
    double m_xMax{5.0};
    double m_yMin{-5.0};
    double m_yMax{5.0};

    float m_minValue{0.0f};
    float m_maxValue{1.0f};
    std::vector<float> m_values;

    QFutureWatcher<void> m_watcher;
};
