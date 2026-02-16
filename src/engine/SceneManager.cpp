#include "engine/SceneManager.h"

namespace ExperimentRedbear {

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

SceneManager::SceneManager() {}

SceneManager::~SceneManager() = default;

void SceneManager::addScene(const std::string& name, std::unique_ptr<Scene> scene) {
    m_scenes[name] = std::move(scene);
}

void SceneManager::removeScene(const std::string& name) {
    m_scenes.erase(name);
}

void SceneManager::changeScene(const std::string& name) {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end()) {
        if (!m_sceneStack.empty()) {
            m_sceneStack.back()->m_active = false;
            m_sceneStack.back()->onExit();
            m_sceneStack.pop_back();
        }

        m_sceneStack.push_back(it->second.get());
        it->second->m_active = true;
        it->second->onEnter();
    }
}

void SceneManager::pushScene(const std::string& name) {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end()) {
        if (!m_sceneStack.empty()) {
            m_sceneStack.back()->onPause();
        }

        m_sceneStack.push_back(it->second.get());
        it->second->m_active = true;
        it->second->onEnter();
    }
}

void SceneManager::popScene() {
    if (!m_sceneStack.empty()) {
        m_sceneStack.back()->m_active = false;
        m_sceneStack.back()->onExit();
        m_sceneStack.pop_back();

        if (!m_sceneStack.empty()) {
            m_sceneStack.back()->onResume();
        }
    }
}

Scene* SceneManager::getCurrentScene() const {
    return m_sceneStack.empty() ? nullptr : m_sceneStack.back();
}

Scene* SceneManager::getScene(const std::string& name) const {
    auto it = m_scenes.find(name);
    return it != m_scenes.end() ? it->second.get() : nullptr;
}

void SceneManager::update(float deltaTime) {
    if (!m_sceneStack.empty()) {
        m_sceneStack.back()->update(deltaTime);
    }
}

void SceneManager::render() {
    if (!m_sceneStack.empty()) {
        m_sceneStack.back()->render();
    }
}

} // namespace ExperimentRedbear
