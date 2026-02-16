#include "utils/Utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <cmath>

namespace ExperimentRedbear {
namespace Utils {

// String utilities
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    while (std::getline(ss, part, delimiter)) {
        parts.push_back(part);
    }
    return parts;
}

std::string join(const std::vector<std::string>& parts, const std::string& delimiter) {
    std::string result;
    for (size_t i = 0; i < parts.size(); i++) {
        if (i > 0) result += delimiter;
        result += parts[i];
    }
    return result;
}

// File utilities
bool fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string getFileExtension(const std::string& path) {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";
    return path.substr(pos + 1);
}

std::string getFileName(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

std::string getDirectoryPath(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return "";
    return path.substr(0, pos);
}

// Math utilities
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t) {
    return a + (b - a) * t;
}

float clamp(float value, float min, float max) {
    return std::max(min, std::min(max, value));
}

int clamp(int value, int min, int max) {
    return std::max(min, std::min(max, value));
}

float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

float distance2D(const glm::vec2& a, const glm::vec2& b) {
    return glm::length(b - a);
}

float distance3D(const glm::vec3& a, const glm::vec3& b) {
    return glm::length(b - a);
}

float magnitude(const glm::vec3& v) {
    return glm::length(v);
}

glm::vec3 normalize(const glm::vec3& v) {
    return glm::normalize(v);
}

float dot(const glm::vec3& a, const glm::vec3& b) {
    return glm::dot(a, b);
}

glm::vec3 cross(const glm::vec3& a, const glm::vec3& b) {
    return glm::cross(a, b);
}

float radians(float degrees) {
    return glm::radians(degrees);
}

float degrees(float radians) {
    return glm::degrees(radians);
}

// Random utilities
std::mt19937 g_randomEngine;

void randomSeed(unsigned int seed) {
    g_randomEngine.seed(seed);
}

float randomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(g_randomEngine);
}

int randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(g_randomEngine);
}

glm::vec3 randomVec3(const glm::vec3& min, const glm::vec3& max) {
    return glm::vec3(
        randomFloat(min.x, max.x),
        randomFloat(min.y, max.y),
        randomFloat(min.z, max.z)
    );
}

// Collision utilities
bool pointInBox(const glm::vec3& point, const glm::vec3& boxMin, const glm::vec3& boxMax) {
    return point.x >= boxMin.x && point.x <= boxMax.x &&
           point.y >= boxMin.y && point.y <= boxMax.y &&
           point.z >= boxMin.z && point.z <= boxMax.z;
}

bool boxIntersectsBox(const glm::vec3& min1, const glm::vec3& max1,
                       const glm::vec3& min2, const glm::vec3& max2) {
    return min1.x <= max2.x && max1.x >= min2.x &&
           min1.y <= max2.y && max1.y >= min2.y &&
           min1.z <= max2.z && max1.z >= min2.z;
}

bool sphereIntersectsBox(const glm::vec3& center, float radius,
                          const glm::vec3& boxMin, const glm::vec3& boxMax) {
    glm::vec3 closest = glm::clamp(center, boxMin, boxMax);
    float distance = glm::length(center - closest);
    return distance < radius;
}

bool rayIntersectsBox(const glm::vec3& origin, const glm::vec3& direction,
                       const glm::vec3& boxMin, const glm::vec3& boxMax,
                       float& outDistance) {
    glm::vec3 invDir = 1.0f / direction;
    glm::vec3 t1 = (boxMin - origin) * invDir;
    glm::vec3 t2 = (boxMax - origin) * invDir;

    glm::vec3 tMin = glm::min(t1, t2);
    glm::vec3 tMax = glm::max(t1, t2);

    float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
    float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

    if (tNear <= tFar && tNear > 0.0f) {
        outDistance = tNear;
        return true;
    }

    return false;
}

// Easing functions
float easeInQuad(float t) {
    return t * t;
}

float easeOutQuad(float t) {
    return t * (2.0f - t);
}

float easeInOutQuad(float t) {
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

float easeInCubic(float t) {
    return t * t * t;
}

float easeOutCubic(float t) {
    float t1 = t - 1.0f;
    return t1 * t1 * t1 + 1.0f;
}

float easeInOutCubic(float t) {
    return t < 0.5f ? 4.0f * t * t * t : (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1.0f;
}

// Time formatting
std::string formatTime(float seconds) {
    int mins = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    std::ostringstream oss;
    oss << mins << ":" << std::setfill('0') << std::setw(2) << secs;
    return oss.str();
}

} // namespace Utils
} // namespace ExperimentRedbear
