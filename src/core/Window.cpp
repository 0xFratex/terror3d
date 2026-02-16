#include "core/Window.h"
#include "core/Logger.h"
#include <stdexcept>

namespace ExperimentRedbear {

Window::Window() : m_window(nullptr), m_monitor(nullptr) {}

Window::~Window() {
    shutdown();
}

bool Window::initialize(const WindowSettings& settings) {
    if (!glfwInit()) {
        LOG_FATAL("Failed to initialize GLFW");
        return false;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, settings.samples);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, settings.refreshRate);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    // Get monitor for fullscreen
    m_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);

    // Create window
    if (settings.fullscreen && m_monitor) {
        m_window = glfwCreateWindow(mode->width, mode->height, 
                                     settings.title.c_str(), m_monitor, nullptr);
        m_width = mode->width;
        m_height = mode->height;
        m_fullscreen = true;
    } else {
        m_window = glfwCreateWindow(settings.width, settings.height,
                                     settings.title.c_str(), nullptr, nullptr);
        m_width = settings.width;
        m_height = settings.height;
        m_fullscreen = false;
    }

    if (!m_window) {
        LOG_FATAL("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(settings.vsync ? 1 : 0);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        LOG_FATAL(std::string("Failed to initialize GLEW: ") + 
                  reinterpret_cast<const char*>(glewGetErrorString(glewError)));
        return false;
    }

    // Set callbacks
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetCursorPosCallback(m_window, cursorPosCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetScrollCallback(m_window, scrollCallback);

    // Enable MSAA
    glEnable(GL_MULTISAMPLE);

    // Center window if not fullscreen
    if (!m_fullscreen) {
        centerWindow();
    }

    LOG_INFO("Window created successfully: " + std::to_string(m_width) + "x" + std::to_string(m_height));

    return true;
}

void Window::shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
    LOG_INFO("Window shut down");
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::setShouldClose(bool close) {
    glfwSetWindowShouldClose(m_window, close);
}

void Window::setTitle(const std::string& title) {
    glfwSetWindowTitle(m_window, title.c_str());
}

void Window::setIcon(const std::string& iconPath) {
    // TODO: Load and set window icon
}

void Window::setResizeCallback(std::function<void(int, int)> callback) {
    m_resizeCallback = callback;
}

void Window::setKeyCallback(std::function<void(int, int, int, int)> callback) {
    m_keyCallback = callback;
}

void Window::setMouseCallback(std::function<void(double, double)> callback) {
    m_mouseCallback = callback;
}

void Window::setMouseButtonCallback(std::function<void(int, int, int)> callback) {
    m_mouseButtonCallback = callback;
}

void Window::setScrollCallback(std::function<void(double, double)> callback) {
    m_scrollCallback = callback;
}

void Window::setFullscreen(bool fullscreen) {
    if (m_fullscreen == fullscreen) return;

    const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);

    if (fullscreen) {
        glfwSetWindowMonitor(m_window, m_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        m_width = mode->width;
        m_height = mode->height;
    } else {
        glfwSetWindowMonitor(m_window, nullptr, 100, 100, 1920, 1080, mode->refreshRate);
        m_width = 1920;
        m_height = 1080;
        centerWindow();
    }

    m_fullscreen = fullscreen;
    glViewport(0, 0, m_width, m_height);
}

void Window::centerWindow() {
    const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
    int xpos = (mode->width - m_width) / 2;
    int ypos = (mode->height - m_height) / 2;
    glfwSetWindowPos(m_window, xpos, ypos);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_width = width;
    win->m_height = height;
    glViewport(0, 0, width, height);
    if (win->m_resizeCallback) {
        win->m_resizeCallback(width, height);
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win->m_keyCallback) {
        win->m_keyCallback(key, scancode, action, mods);
    }
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win->m_mouseCallback) {
        win->m_mouseCallback(xpos, ypos);
    }
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win->m_mouseButtonCallback) {
        win->m_mouseButtonCallback(button, action, mods);
    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win->m_scrollCallback) {
        win->m_scrollCallback(xoffset, yoffset);
    }
}

} // namespace ExperimentRedbear
