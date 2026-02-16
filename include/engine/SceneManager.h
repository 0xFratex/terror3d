#pragma once

#include <string>
#include <memory>
#include <unordered_map>

namespace ExperimentRedbear {

class Scene {
public:
    Scene(const std::string& name);
    virtual ~Scene();

    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onPause() {}
    virtual void onResume() {}

    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;

    const std::string& getName() const { return m_name; }
    bool isActive() const { return m_active; }

private:
    std::string m_name;
    bool m_active = false;

    friend class SceneManager;
};

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    void addScene(const std::string& name, std::unique_ptr<Scene> scene);
    void removeScene(const std::string& name);

    void changeScene(const std::string& name);
    void pushScene(const std::string& name);
    void popScene();

    Scene* getCurrentScene() const;
    Scene* getScene(const std::string& name) const;

    void update(float deltaTime);
    void render();

private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;
    std::vector<Scene*> m_sceneStack;
};

} // namespace ExperimentRedbear
