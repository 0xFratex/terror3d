#pragma once

#include <string>
#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ExperimentRedbear {

struct WindowSettings {
    int width = 1920;
    int height = 1080;
    std::string title = "Experiment Redbear";
    bool fullscreen = false;
    bool vsync = true;
    int samples = 4;
    int refreshRate = 60;
};

class Window {
public:
    Window();
    ~Window();

    bool initialize(const WindowSettings& settings);
    void shutdown();

    void pollEvents();
    void swapBuffers();

    bool shouldClose() const;
    void setShouldClose(bool close);

    void setTitle(const std::string& title);
    void setIcon(const std::string& iconPath);

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    float getAspectRatio() const { return static_cast<float>(m_width) / m_height; }

    GLFWwindow* getNativeWindow() const { return m_window; }

    void setResizeCallback(std::function<void(int, int)> callback);
    void setKeyCallback(std::function<void(int, int, int, int)> callback);
    void setMouseCallback(std::function<void(double, double)> callback);
    void setMouseButtonCallback(std::function<void(int, int, int)> callback);
    void setScrollCallback(std::function<void(double, double)> callback);

    void setFullscreen(bool fullscreen);
    bool isFullscreen() const { return m_fullscreen; }

    void centerWindow();

private:
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    GLFWwindow* m_window = nullptr;
    GLFWmonitor* m_monitor = nullptr;
    int m_width = 0;
    int m_height = 0;
    bool m_fullscreen = false;

    std::function<void(int, int)> m_resizeCallback;
    std::function<void(int, int, int, int)> m_keyCallback;
    std::function<void(double, double)> m_mouseCallback;
    std::function<void(int, int, int)> m_mouseButtonCallback;
    std::function<void(double, double)> m_scrollCallback;
};

} // namespace ExperimentRedbear
