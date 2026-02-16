#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

namespace ExperimentRedbear {

template<typename T>
class ResourceCache {
public:
    std::shared_ptr<T> get(const std::string& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_resources.find(key);
        if (it != m_resources.end()) {
            return it->second;
        }
        return nullptr;
    }

    void put(const std::string& key, std::shared_ptr<T> resource) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_resources[key] = resource;
    }

    bool has(const std::string& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_resources.find(key) != m_resources.end();
    }

    void remove(const std::string& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_resources.erase(key);
    }

    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_resources.clear();
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_resources.size();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<T>> m_resources;
    std::mutex m_mutex;
};

} // namespace ExperimentRedbear
