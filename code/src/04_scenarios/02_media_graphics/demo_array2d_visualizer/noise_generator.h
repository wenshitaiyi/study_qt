#pragma once

#include <vector>
#include <cmath>

enum class NoiseType {
    Perlin = 0,         // Perlin 连续梯度噪声
    Fbm = 1,            // 分形布朗运动（多阶叠加）
    WorleyCellular = 2, // Worley 细胞/晶格距离噪声
    HarmonicWaves = 3,  // 调和正余弦波干涉场
    ThermalPlume = 4    // 高斯热源扩散与湍流羽流
};

class NoiseGenerator {
public:
    NoiseGenerator();
    explicit NoiseGenerator(int seed);

    void setSeed(int seed);
    int seed() const { return m_seed; }

    // 采样单点噪声数值，输出归一化到约 [0.0, 1.0]
    float sample(NoiseType type, float x, float y, float frequency = 1.0f,
                 int octaves = 4, float persistence = 0.5f, float lacunarity = 2.0f) const;

    // 单独的算法采样接口
    float perlin2D(float x, float y) const;
    float fbm2D(float x, float y, int octaves, float persistence, float lacunarity) const;
    float worley2D(float x, float y) const;
    float harmonicWaves2D(float x, float y) const;
    float thermalPlume2D(float x, float y) const;

private:
    void initPermutation();
    static float fade(float t);
    static float lerp(float t, float a, float b);
    static float grad(int hash, float x, float y);

    int m_seed{1337};
    std::vector<int> m_perm;
};
