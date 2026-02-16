#pragma once

#include <memory>
#include <functional>
#include "core/Window.h"
#include "core/Input.h"
#include "core/Timer.h"
#include "core/Config.h"
#include "engine/SceneManager.h"
#include "game/Player.h"
#include "game/World.h"
#include "ui/Menu.h"
#include "ui/HUD.h"

namespace ExperimentRedbear {

enum class GameState {
    UNINITIALIZED,
    MAIN_MENU,
    LOADING,
    PLAYING,
    PAUSED,
    CUTSCENE,
    GAME_OVER,
    SHUTTING_DOWN
};

class Game {
public:
    static Game& getInstance();

    bool initialize();
    void run();
    void shutdown();

    // State management
    void setState(GameState state);
    GameState getState() const { return m_state; }

    // Game flow
    void newGame();
    void continueGame();
    void pauseGame();
    void resumeGame();
    void quitGame();
    void gameOver();

    // Accessors
    Window& getWindow() { return m_window; }
    Input& getInput() { return m_input; }
    Timer& getTimer() { return m_timer; }
    Config& getConfig() { return m_config; }
    Player& getPlayer() { return m_player; }
    World& getWorld() { return m_world; }
    Menu& getMenu() { return m_menu; }
    HUD& getHUD() { return m_hud; }
    SceneManager& getSceneManager() { return m_sceneManager; }

    // Callbacks
    void setOnGameStart(std::function<void()> callback) { m_onGameStart = callback; }
    void setOnGameEnd(std::function<void()> callback) { m_onGameEnd = callback; }

private:
    Game() = default;
    ~Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void mainLoop();
    void handleEvents();
    void update(float deltaTime);
    void fixedUpdate(float fixedDeltaTime);
    void render();
    void renderUI();

    void loadAssets();
    void initializeWorld();

    GameState m_state = GameState::UNINITIALIZED;

    Window m_window;
    Input& m_input = Input::getInstance();
    Timer m_timer;
    Config& m_config = Config::getInstance();
    SceneManager m_sceneManager;

    Player m_player;
    World m_world;
    Menu m_menu;
    HUD m_hud;

    // Fixed timestep
    float m_fixedTimeStep = 1.0f / 60.0f;
    float m_accumulator = 0.0f;

    // Callbacks
    std::function<void()> m_onGameStart;
    std::function<void()> m_onGameEnd;

    bool m_initialized = false;
    bool m_running = false;
};

} // namespace ExperimentRedbear
