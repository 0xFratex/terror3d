#include "utils/PerlinNoise.h"
#include <algorithm>
#include <random>

namespace ExperimentRedbear {

PerlinNoise::PerlinNoise() {
    setSeed(0);
}

PerlinNoise::PerlinNoise(unsigned int seed) {
    setSeed(seed);
}

void PerlinNoise::setSeed(unsigned int seed) {
    m_permutation.resize(256);

    // Fill with values 0-255
    for (int i = 0; i < 256; i++) {
        m_permutation[i] = i;
    }

    // Shuffle using seed
    std::mt19937 engine(seed);
    std::shuffle(m_permutation.begin(), m_permutation.end(), engine);

    // Duplicate for overflow
    m_permutation.insert(m_permutation.end(), m_permutation.begin(), m_permutation.end());
}

float PerlinNoise::noise(float x) const {
    int X = static_cast<int>(std::floor(x)) & 255;
    x -= std::floor(x);

    float u = fade(x);

    return lerp(u,
        grad(m_permutation[X], x),
        grad(m_permutation[X + 1], x - 1)
    );
}

float PerlinNoise::noise(float x, float y) const {
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    float u = fade(x);
    float v = fade(y);

    int A = m_permutation[X] + Y;
    int B = m_permutation[X + 1] + Y;

    return lerp(v,
        lerp(u, grad(m_permutation[A], x, y),
                grad(m_permutation[B], x - 1, y)),
        lerp(u, grad(m_permutation[A + 1], x, y - 1),
                grad(m_permutation[B + 1], x - 1, y - 1))
    );
}

float PerlinNoise::noise(float x, float y, float z) const {
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    int Z = static_cast<int>(std::floor(z)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    int A = m_permutation[X] + Y;
    int AA = m_permutation[A] + Z;
    int AB = m_permutation[A + 1] + Z;
    int B = m_permutation[X + 1] + Y;
    int BA = m_permutation[B] + Z;
    int BB = m_permutation[B + 1] + Z;

    return lerp(w,
        lerp(v,
            lerp(u, grad(m_permutation[AA], x, y, z),
                    grad(m_permutation[BA], x - 1, y, z)),
            lerp(u, grad(m_permutation[AB], x, y - 1, z),
                    grad(m_permutation[BB], x - 1, y - 1, z))
        ),
        lerp(v,
            lerp(u, grad(m_permutation[AA + 1], x, y, z - 1),
                    grad(m_permutation[BA + 1], x - 1, y, z - 1)),
            lerp(u, grad(m_permutation[AB + 1], x, y - 1, z - 1),
                    grad(m_permutation[BB + 1], x - 1, y - 1, z - 1))
        )
    );
}

float PerlinNoise::octaveNoise(float x, int octaves, float persistence) const {
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; i++) {
        total += noise(x * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }

    return total / maxValue;
}

float PerlinNoise::octaveNoise(float x, float y, int octaves, float persistence) const {
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; i++) {
        total += noise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }

    return total / maxValue;
}

float PerlinNoise::octaveNoise(float x, float y, float z, int octaves, float persistence) const {
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; i++) {
        total += noise(x * frequency, y * frequency, z * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }

    return total / maxValue;
}

float PerlinNoise::noise01(float x) const {
    return noise(x) * 0.5f + 0.5f;
}

float PerlinNoise::noise01(float x, float y) const {
    return noise(x, y) * 0.5f + 0.5f;
}

float PerlinNoise::noise01(float x, float y, float z) const {
    return noise(x, y, z) * 0.5f + 0.5f;
}

float PerlinNoise::fade(float t) const {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::lerp(float t, float a, float b) const {
    return a + t * (b - a);
}

float PerlinNoise::grad(int hash, float x) const {
    return (hash & 1) == 0 ? x : -x;
}

float PerlinNoise::grad(int hash, float x, float y) const {
    int h = hash & 3;
    float u = h < 2 ? x : y;
    float v = h < 2 ? y : x;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::grad(int hash, float x, float y, float z) const {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    float w = h < 4 ? z : (h < 8 ? y : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v) + ((h & 4) == 0 ? w : -w);
}

} // namespace ExperimentRedbear
