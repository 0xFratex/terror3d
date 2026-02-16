#pragma once

#include <vector>
#include <cmath>

namespace ExperimentRedbear {

class PerlinNoise {
public:
    PerlinNoise();
    PerlinNoise(unsigned int seed);

    void setSeed(unsigned int seed);

    float noise(float x) const;
    float noise(float x, float y) const;
    float noise(float x, float y, float z) const;

    float octaveNoise(float x, int octaves, float persistence = 0.5f) const;
    float octaveNoise(float x, float y, int octaves, float persistence = 0.5f) const;
    float octaveNoise(float x, float y, float z, int octaves, float persistence = 0.5f) const;

    float noise01(float x) const;
    float noise01(float x, float y) const;
    float noise01(float x, float y, float z) const;

private:
    float fade(float t) const;
    float lerp(float t, float a, float b) const;
    float grad(int hash, float x) const;
    float grad(int hash, float x, float y) const;
    float grad(int hash, float x, float y, float z) const;

    std::vector<int> m_permutation;
};

} // namespace ExperimentRedbear
