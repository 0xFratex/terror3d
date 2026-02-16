#include "engine/Game.h"
#include "engine/SceneManager.h"
#include "graphics/Renderer.h"
#include "audio/AudioManager.h"
#include "ui/TextRenderer.h"
#include "ui/UIManager.h"
#include "game/HouseGenerator.h"
#include "game/ForestGenerator.h"
#include "core/Logger.h"
#include <sstream>
#include <GLFW/glfw3.h>

namespace ExperimentRedbear {

Game& Game::getInstance() {
    static Game instance;
    return instance;
}

bool Game::initialize() {
    LOG_INFO("=== Experiment Redbear ===");
    LOG_INFO("Initializing game engine...");

    // Load configuration
    m_config.load("config.cfg");

    // Initialize window
    WindowSettings windowSettings;
    windowSettings.width = m_config.graphics.screenWidth;
    windowSettings.height = m_config.graphics.screenHeight;
    windowSettings.fullscreen = m_config.graphics.fullscreen;
    windowSettings.vsync = m_config.graphics.vsync;
    windowSettings.title = "Experiment Redbear";

    if (!m_window.initialize(windowSettings)) {
        LOG_FATAL("Failed to initialize window");
        return false;
    }

    // Set up window callbacks
    m_window.setKeyCallback([this](int key, int scancode, int action, int mods) {
        m_input.keyCallback(key, scancode, action, mods);
        handleKeyInput(key, action);
    });
    m_window.setMouseCallback([this](double xpos, double ypos) {
        m_input.mouseCallback(xpos, ypos);
    });
    m_window.setMouseButtonCallback([this](int button, int action, int mods) {
        m_input.mouseButtonCallback(button, action, mods);
    });
    m_window.setScrollCallback([this](double xoffset, double yoffset) {
        m_input.scrollCallback(xoffset, yoffset);
    });
    m_window.setResizeCallback([this](int width, int height) {
        handleResize(width, height);
    });

    // Initialize input
    m_input.initialize();

    // Initialize renderer
    auto& renderer = Renderer::getInstance();
    if (!renderer.initialize(m_window.getWidth(), m_window.getHeight())) {
        LOG_FATAL("Failed to initialize renderer");
        return false;
    }

    // Initialize text renderer
    auto& textRenderer = TextRenderer::getInstance();
    if (!textRenderer.initialize()) {
        LOG_FATAL("Failed to initialize text renderer");
        return false;
    }

    // Load fonts
    textRenderer.loadFont("main", "assets/fonts/arial.ttf", 24);
    textRenderer.loadFont("title", "assets/fonts/arial.ttf", 48);
    textRenderer.loadFont("mono", "assets/fonts/arial.ttf", 16);

    // Initialize UI manager
    auto& uiManager = UIManager::getInstance();
    uiManager.initialize(m_window.getWidth(), m_window.getHeight());

    // Initialize audio
    auto& audioManager = AudioManager::getInstance();
    if (!audioManager.initialize()) {
        LOG_WARNING("Failed to initialize audio manager - continuing without audio");
    }

    // Initialize menu
    if (!m_menu.initialize()) {
        LOG_WARNING("Failed to initialize menu system");
    }
    m_menu.setOnNewGame([this]() { newGame(); });
    m_menu.setOnContinue([this]() { continueGame(); });
    m_menu.setOnQuit([this]() { quitGame(); });

    // Initialize HUD
    if (!m_hud.initialize()) {
        LOG_WARNING("Failed to initialize HUD");
    }

    // Initialize player
    m_player.initialize();

    // Set game state
    m_state = GameState::MAIN_MENU;

    // Hide cursor for gameplay, show for menu
    glfwSetInputMode(m_window.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    m_initialized = true;
    m_running = true;

    LOG_INFO("Game engine initialized successfully");
    return true;
}

void Game::run() {
    if (!m_initialized) {
        LOG_FATAL("Game not initialized - cannot run");
        return;
    }

    LOG_INFO("Starting game loop");

    while (m_running && !m_window.shouldClose()) {
        mainLoop();
    }

    shutdown();
}

void Game::shutdown() {
    LOG_INFO("Shutting down game engine...");

    m_running = false;

    // Save configuration
    m_config.save("config.cfg");

    // Shutdown systems
    auto& audioManager = AudioManager::getInstance();
    audioManager.shutdown();

    auto& uiManager = UIManager::getInstance();
    uiManager.shutdown();

    auto& textRenderer = TextRenderer::getInstance();
    textRenderer.shutdown();

    auto& renderer = Renderer::getInstance();
    renderer.shutdown();

    m_window.shutdown();

    m_initialized = false;
    LOG_INFO("Game engine shut down successfully");
}

void Game::mainLoop() {
    // Update timer
    m_timer.update();

    float deltaTime = m_timer.getDeltaTime();
    deltaTime = std::min(deltaTime, 0.1f); // Cap delta time

    // Handle events
    handleEvents();

    // Update input
    m_input.update();

    // Fixed timestep for physics
    m_accumulator += deltaTime;
    while (m_accumulator >= m_fixedTimeStep) {
        fixedUpdate(m_fixedTimeStep);
        m_accumulator -= m_fixedTimeStep;
    }

    // Update
    update(deltaTime);

    // Render
    render();
    renderUI();

    // Swap buffers
    m_window.swapBuffers();
    m_window.pollEvents();
}

void Game::handleEvents() {
    // Handle system events
}

void Game::handleKeyInput(int key, int action) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                if (m_state == GameState::PLAYING) {
                    pauseGame();
                } else if (m_state == GameState::PAUSED) {
                    resumeGame();
                } else if (m_state == GameState::MAIN_MENU) {
                    // Nothing or quit confirmation
                }
                break;

            case GLFW_KEY_F:
                if (m_state == GameState::PLAYING) {
                    m_player.toggleFlashlight();
                }
                break;

            case GLFW_KEY_F3:
                m_config.gameplay.showFPS = !m_config.gameplay.showFPS;
                break;

            case GLFW_KEY_F11:
                m_window.setFullscreen(!m_window.isFullscreen());
                break;
        }
    }
}

void Game::handleResize(int width, int height) {
    auto& renderer = Renderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    renderer.initialize(width, height);
    uiManager.resize(width, height);

    m_player.getCamera().setAspectRatio(static_cast<float>(width) / height);
}

void Game::update(float deltaTime) {
    switch (m_state) {
        case GameState::MAIN_MENU:
            m_menu.update(deltaTime);
            break;

        case GameState::PLAYING:
            updatePlaying(deltaTime);
            break;

        case GameState::PAUSED:
            m_menu.update(deltaTime);
            break;

        case GameState::LOADING:
            // Loading screen update
            break;

        default:
            break;
    }
}

void Game::updatePlaying(float deltaTime) {
    Input& input = Input::getInstance();
    Config& config = Config::getInstance();

    // Lock cursor for gameplay
    static bool cursorLocked = false;
    if (!cursorLocked) {
        glfwSetInputMode(m_window.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        cursorLocked = true;
    }

    // Player movement
    if (input.isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        m_player.run(true);
    } else {
        m_player.run(false);
    }

    if (input.isKeyDown(GLFW_KEY_LEFT_CONTROL)) {
        m_player.crouch(true);
    } else {
        m_player.crouch(false);
    }

    if (input.isKeyPressed(GLFW_KEY_SPACE)) {
        m_player.jump();
    }

    // Update player
    m_player.update(deltaTime);

    // Update flashlight battery for HUD
    m_hud.setFlashlightBattery(m_player.isFlashlightOn() ? 
        (m_player.hasFlashlight() ? 100.0f : 0.0f) : 100.0f);

    // Update world
    m_world.update(deltaTime);

    // Check for interactables
    auto interactable = m_world.getNearestInteractable(
        m_player.getPosition(),
        m_player.getCamera().getForward(),
        2.5f
    );

    if (interactable && interactable->canInteract()) {
        m_hud.showInteractionPrompt(interactable->getInteractionText());

        if (input.isKeyPressed(GLFW_KEY_E)) {
            interactable->interact();
        }
    } else {
        m_hud.hideInteractionPrompt();
    }

    // Update audio listener
    auto& audioManager = AudioManager::getInstance();
    audioManager.setListenerPosition(m_player.getPosition());
    audioManager.setListenerOrientation(
        m_player.getCamera().getForward(),
        m_player.getCamera().getUp()
    );

    // Update HUD
    m_hud.update(deltaTime);

    // Debug info
    if (config.gameplay.showFPS) {
        std::ostringstream debugInfo;
        debugInfo << "FPS: " << std::to_string(static_cast<int>(m_timer.getFPS())) << "\n";
        debugInfo << "Pos: " << m_player.getPosition().x << ", " 
                  << m_player.getPosition().y << ", " 
                  << m_player.getPosition().z << "\n";
        debugInfo << "State: " << static_cast<int>(m_player.getState()) << "\n";
        m_hud.setDebugInfo(debugInfo.str());
    }
    m_hud.setShowDebugInfo(config.gameplay.showFPS);
}

void Game::fixedUpdate(float fixedDeltaTime) {
    if (m_state == GameState::PLAYING) {
        m_player.fixedUpdate(fixedDeltaTime);
    }
}

void Game::render() {
    auto& renderer = Renderer::getInstance();

    renderer.beginFrame();

    if (m_state == GameState::PLAYING || m_state == GameState::PAUSED) {
        renderer.setCamera(&m_player.getCamera());

        // Render world
        m_world.render();

        // Flush render queue
        renderer.flush();
    }

    renderer.endFrame();
}

void Game::renderUI() {
    auto& renderer = Renderer::getInstance();

    switch (m_state) {
        case GameState::MAIN_MENU:
            m_menu.render();
            break;

        case GameState::PLAYING:
            m_hud.render(m_player);
            break;

        case GameState::PAUSED:
            m_hud.render(m_player);
            m_menu.render();
            break;

        case GameState::LOADING:
            // Render loading screen
            break;

        default:
            break;
    }
}

void Game::setState(GameState state) {
    m_state = state;
}

void Game::newGame() {
    LOG_INFO("Starting new game...");

    m_state = GameState::LOADING;

    // Initialize world
    WorldSettings worldSettings;
    worldSettings.name = "Experiment Redbear";
    worldSettings.playerStart = glm::vec3(1.5f, 1.7f, 1.5f);  // Bedroom position
    worldSettings.enableSnow = true;
    worldSettings.enableFog = true;

    m_world.initialize(worldSettings);

    // Generate house
    HouseGenerator houseGen;
    houseGen.generate(&m_world, glm::vec3(0.0f));

    // Generate forest
    ForestGenerator forestGen;
    forestGen.generate(&m_world, glm::vec3(0.0f), 200.0f);

    // Set player position
    m_player.setPosition(worldSettings.playerStart);

    // Initialize renderer lighting
    Renderer::getInstance().setAmbientLight(glm::vec3(0.02f, 0.02f, 0.03f), 1.0f);

    // Show objective
    m_hud.showObjective("Find a way out of the house");

    // Start game
    m_state = GameState::PLAYING;

    // Lock cursor
    glfwSetInputMode(m_window.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (m_onGameStart) {
        m_onGameStart();
    }
}

void Game::continueGame() {
    // Load saved game
    LOG_INFO("Continuing game...");
    newGame();  // For now, just start new game
}

void Game::pauseGame() {
    if (m_state == GameState::PLAYING) {
        m_state = GameState::PAUSED;
        m_menu.setState(MenuState::PAUSED);
        glfwSetInputMode(m_window.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        LOG_INFO("Game paused");
    }
}

void Game::resumeGame() {
    if (m_state == GameState::PAUSED) {
        m_state = GameState::PLAYING;
        glfwSetInputMode(m_window.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        LOG_INFO("Game resumed");
    }
}

void Game::quitGame() {
    m_running = false;
    LOG_INFO("Quitting game...");
}

void Game::gameOver() {
    m_state = GameState::GAME_OVER;
    LOG_INFO("Game over!");
}

} // namespace ExperimentRedbear
