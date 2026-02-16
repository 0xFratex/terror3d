/**
 * Experiment Redbear
 * A 3D Horror First-Person Game
 * 
 * Created with OpenGL 4.5, GLFW, and GLEW
 * 
 * Copyright (c) 2024
 */

#include "engine/Game.h"
#include "core/Logger.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[]) {
#ifdef _WIN32
    // Show console on Windows for debug builds
    #ifdef DEBUG
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    #endif
#endif

    try {
        // Initialize logger
        auto& logger = ExperimentRedbear::Logger::getInstance();
        logger.setLogLevel(ExperimentRedbear::LogLevel::TRACE);
        logger.setConsoleOutput(true);
        logger.setLogFile("experiment_redbear.log");

        LOG_INFO("========================================");
        LOG_INFO("    EXPERIMENT REDBEAR");
        LOG_INFO("    A Horror First-Person Experience");
        LOG_INFO("========================================");
        LOG_INFO("");

        // Get game instance
        auto& game = ExperimentRedbear::Game::getInstance();

        // Initialize game
        if (!game.initialize()) {
            LOG_FATAL("Failed to initialize game!");
            return -1;
        }

        // Run game loop
        game.run();

        // Game will shutdown automatically
        LOG_INFO("Game exited successfully");

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred!" << std::endl;
        return -1;
    }

    return 0;
}

// Platform-specific entry points

#ifdef _WIN32
// Windows entry point for release builds
#ifndef DEBUG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif
#endif
