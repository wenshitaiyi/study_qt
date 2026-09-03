#include "noise_generator.h"

#include <numeric>
#include <random>
#include <algorithm>

NoiseGenerator::NoiseGenerator()
{
    initPermutation();
}

NoiseGenerator::NoiseGenerator(int seed)
    : m_seed(seed)
{
    initPermutation();
}

void NoiseGenerator::setSeed(int seed)
{
    m_seed = seed;
    initPermutation();
}

void NoiseGenerator::initPermutation()
{
    m_perm.resize(512);
    std::vector<int> p(256);
    std::iota(p.begin(), p.end(), 0);

    std::mt19937 g(static_cast<unsigned int>(m_seed));
    std::shuffle(p.begin(), p.end(), g);

    for (int i = 0; i < 256; ++i) {
        m_perm[i] = p[i];
        m_perm[256 + i] = p[i];
    }
}

float NoiseGenerator::fade(float t)
{
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float NoiseGenerator::lerp(float t, float a, float b)
{
    return a + t * (b - a);
}

float NoiseGenerator::grad(int hash, float x, float y)
{
    // 8 组二维单位梯度向量
    switch (hash & 7) {
        case 0: return  x + y;
        case 1: return -x + y;
        case 2: return  x - y;
        case 3: return -x - y;
        case 4: return  x;
        case 5: return -x;
        case 6: return  y;
        case 7: return -y;
        default: return 0.0f;
    }
}

float NoiseGenerator::perlin2D(float x, float y) const
{
    int xi = static_cast<int>(std::floor(x)) & 255;
    int yi = static_cast<int>(std::floor(y)) & 255;

    float xf = x - std::floor(x);
    float yf = y - std::floor(y);

    float u = fade(xf);
    float v = fade(yf);

    int aa = m_perm[m_perm[xi] + yi];
    int ab = m_perm[m_perm[xi] + yi + 1];
    int ba = m_perm[m_perm[xi + 1] + yi];
    int bb = m_perm[m_perm[xi + 1] + yi + 1];

    float x1 = lerp(u, grad(aa, xf, yf), grad(ba, xf - 1.0f, yf));
    float x2 = lerp(u, grad(ab, xf, yf - 1.0f), grad(bb, xf - 1.0f, yf - 1.0f));

    // 输出范围为 [-1.0, 1.0]，映射为 [0.0, 1.0]
    float raw = lerp(v, x1, x2);
    return std::clamp((raw + 1.0f) * 0.5f, 0.0f, 1.0f);
}

float NoiseGenerator::fbm2D(float x, float y, int octaves, float persistence, float lacunarity) const
{
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        // 使用原始 perlin 计算 [-1, 1] 累加
        int xi = static_cast<int>(std::floor(x * frequency)) & 255;
        int yi = static_cast<int>(std::floor(y * frequency)) & 255;
        float xf = x * frequency - std::floor(x * frequency);
        float yf = y * frequency - std::floor(y * frequency);

        float u = fade(xf);
        float v = fade(yf);

        int aa = m_perm[m_perm[xi] + yi];
        int ab = m_perm[m_perm[xi] + yi + 1];
        int ba = m_perm[m_perm[xi + 1] + yi];
        int bb = m_perm[m_perm[xi + 1] + yi + 1];

        float x1 = lerp(u, grad(aa, xf, yf), grad(ba, xf - 1.0f, yf));
        float x2 = lerp(u, grad(ab, xf, yf - 1.0f), grad(bb, xf - 1.0f, yf - 1.0f));
        float n = lerp(v, x1, x2);

        total += n * amplitude;
        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= lacunarity;
    }

    float norm = (maxValue > 1e-6f) ? total / maxValue : 0.0f;
    return std::clamp((norm + 1.0f) * 0.5f, 0.0f, 1.0f);
}

float NoiseGenerator::worley2D(float x, float y) const
{
    int xi = static_cast<int>(std::floor(x));
    int yi = static_cast<int>(std::floor(y));

    float minDist = 1e9f;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int cx = (xi + dx) & 255;
            int cy = (yi + dy) & 255;

            // 基于哈希生成该晶胞内的特征点位置
            int h1 = m_perm[(m_perm[cx] + cy) & 511];
            int h2 = m_perm[(m_perm[cy] + cx) & 511];

            float px = (xi + dx) + static_cast<float>(h1) / 255.0f;
            float py = (yi + dy) + static_cast<float>(h2) / 255.0f;

            float dist = std::sqrt((x - px) * (x - px) + (y - py) * (y - py));
            if (dist < minDist) {
                minDist = dist;
            }
        }
    }

    // 距离一般在 [0.0, 1.414] 之间，归一化并反转增强细胞感
    float val = std::clamp(minDist / 1.2f, 0.0f, 1.0f);
    return 1.0f - val;
}

float NoiseGenerator::harmonicWaves2D(float x, float y) const
{
    // 多向正余弦交错干涉波场
    float w1 = std::sin(x * 1.5f + y * 0.8f);
    float w2 = std::cos(x * 0.9f - y * 1.3f);
    float w3 = std::sin(std::sqrt(x * x + y * y) * 2.0f);
    float w4 = std::cos(x * 2.5f + y * 2.2f) * 0.5f;

    float combined = (w1 + w2 + w3 + w4) / 3.5f;
    return std::clamp((combined + 1.0f) * 0.5f, 0.0f, 1.0f);
}

float NoiseGenerator::thermalPlume2D(float x, float y) const
{
    // 中心高斯热源扩散 + 边缘弱扰动
    float r2 = x * x + y * y;
    float heat = std::exp(-r2 * 0.35f);

    float turbulence = perlin2D(x * 2.0f, y * 2.0f) * 0.3f;
    return std::clamp(heat + turbulence, 0.0f, 1.0f);
}

float NoiseGenerator::sample(NoiseType type, float x, float y, float frequency,
                             int octaves, float persistence, float lacunarity) const
{
    float fx = x * frequency;
    float fy = y * frequency;

    switch (type) {
        case NoiseType::Perlin:
            return perlin2D(fx, fy);
        case NoiseType::Fbm:
            return fbm2D(fx, fy, octaves, persistence, lacunarity);
        case NoiseType::WorleyCellular:
            return worley2D(fx, fy);
        case NoiseType::HarmonicWaves:
            return harmonicWaves2D(fx, fy);
        case NoiseType::ThermalPlume:
            return thermalPlume2D(fx, fy);
        default:
            return perlin2D(fx, fy);
    }
}
