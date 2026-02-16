#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <random>

namespace ExperimentRedbear {

namespace Utils {

// String utilities
std::string toLower(const std::string& str);
std::string toUpper(const std::string& str);
std::string trim(const std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);
std::string join(const std::vector<std::string>& parts, const std::string& delimiter);

// File utilities
bool fileExists(const std::string& path);
std::string readFile(const std::string& path);
std::string getFileExtension(const std::string& path);
std::string getFileName(const std::string& path);
std::string getDirectoryPath(const std::string& path);

// Math utilities
float lerp(float a, float b, float t);
glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t);
float clamp(float value, float min, float max);
int clamp(int value, int min, int max);
float smoothstep(float edge0, float edge1, float x);
float distance2D(const glm::vec2& a, const glm::vec2& b);
float distance3D(const glm::vec3& a, const glm::vec3& b);
float magnitude(const glm::vec3& v);
glm::vec3 normalize(const glm::vec3& v);
float dot(const glm::vec3& a, const glm::vec3& b);
glm::vec3 cross(const glm::vec3& a, const glm::vec3& b);
float radians(float degrees);
float degrees(float radians);

// Random utilities
void randomSeed(unsigned int seed);
float randomFloat(float min = 0.0f, float max = 1.0f);
int randomInt(int min, int max);
glm::vec3 randomVec3(const glm::vec3& min, const glm::vec3& max);

// Collision utilities
bool pointInBox(const glm::vec3& point, const glm::vec3& boxMin, const glm::vec3& boxMax);
bool boxIntersectsBox(const glm::vec3& min1, const glm::vec3& max1, 
                       const glm::vec3& min2, const glm::vec3& max2);
bool sphereIntersectsBox(const glm::vec3& center, float radius,
                          const glm::vec3& boxMin, const glm::vec3& boxMax);
bool rayIntersectsBox(const glm::vec3& origin, const glm::vec3& direction,
                       const glm::vec3& boxMin, const glm::vec3& boxMax,
                       float& outDistance);

// Interpolation
float easeInQuad(float t);
float easeOutQuad(float t);
float easeInOutQuad(float t);
float easeInCubic(float t);
float easeOutCubic(float t);
float easeInOutCubic(float t);

// Time formatting
std::string formatTime(float seconds);

} // namespace Utils

} // namespace ExperimentRedbear
