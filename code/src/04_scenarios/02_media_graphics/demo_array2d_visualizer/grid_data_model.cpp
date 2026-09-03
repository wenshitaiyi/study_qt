#include "grid_data_model.h"

#include <QtConcurrent>
#include <limits>
#include <algorithm>

GridDataModel::GridDataModel(QObject *parent)
    : QObject(parent)
{
    connect(&m_watcher, &QFutureWatcher<void>::finished, this, [this]() {
        emit generationFinished();
        emit dataChanged();
    });
}

GridDataModel::~GridDataModel()
{
    if (m_watcher.isRunning()) {
        m_watcher.waitForFinished();
    }
}

bool GridDataModel::isGenerating() const
{
    return m_watcher.isRunning();
}

void GridDataModel::executeGeneration(const GridParams &params)
{
    NoiseGenerator generator(params.seed);

    int rows = std::max(1, params.rows);
    int cols = std::max(1, params.cols);
    std::vector<float> values(static_cast<size_t>(rows) * cols);

    double dx = (cols > 1) ? (params.xMax - params.xMin) / (cols - 1) : 0.0;
    double dy = (rows > 1) ? (params.yMax - params.yMin) / (rows - 1) : 0.0;

    float minVal = std::numeric_limits<float>::max();
    float maxVal = std::numeric_limits<float>::lowest();

    for (int r = 0; r < rows; ++r) {
        float y = static_cast<float>(params.yMin + r * dy);
        size_t rowOffset = static_cast<size_t>(r) * cols;
        for (int c = 0; c < cols; ++c) {
            float x = static_cast<float>(params.xMin + c * dx);
            float val = generator.sample(params.noiseType, x, y, params.frequency,
                                         params.octaves, params.persistence);
            values[rowOffset + c] = val;
            if (val < minVal) minVal = val;
            if (val > maxVal) maxVal = val;
        }
    }

    m_rows = rows;
    m_cols = cols;
    m_xMin = params.xMin;
    m_xMax = params.xMax;
    m_yMin = params.yMin;
    m_yMax = params.yMax;
    m_minValue = minVal;
    m_maxValue = maxVal;
    m_values = std::move(values);
}

void GridDataModel::generateAsync(const GridParams &params)
{
    if (m_watcher.isRunning()) {
        m_watcher.waitForFinished();
    }

    emit generationStarted();

    // 在后台子线程执行计算
    QFuture<void> future = QtConcurrent::run([this, params]() {
        this->executeGeneration(params);
    });
    m_watcher.setFuture(future);
}

void GridDataModel::generateSync(const GridParams &params)
{
    if (m_watcher.isRunning()) {
        m_watcher.waitForFinished();
    }

    emit generationStarted();
    executeGeneration(params);
    emit generationFinished();
    emit dataChanged();
}
