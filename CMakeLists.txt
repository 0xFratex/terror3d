cmake_minimum_required(VERSION 3.16)
project(ExperimentRedbear VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(CORE_SOURCES
    src/core/Window.cpp
    src/core/Input.cpp
    src/core/Timer.cpp
    src/core/Logger.cpp
    src/core/Config.cpp
    src/core/ResourceManager.cpp
)

set(ENGINE_SOURCES
    src/engine/Game.cpp
    src/engine/SceneManager.cpp
    src/engine/EntityManager.cpp
)

set(GRAPHICS_SOURCES
    src/graphics/Renderer.cpp
    src/graphics/Shader.cpp
    src/graphics/Camera.cpp
    src/graphics/Model.cpp
    src/graphics/Texture.cpp
    src/graphics/Light.cpp
    src/graphics/SkyBox.cpp
    src/graphics/Particle.cpp
    src/graphics/Framebuffer.cpp
)

set(GAME_SOURCES
    src/game/Player.cpp
    src/game/World.cpp
    src/game/Door.cpp
    src/game/Item.cpp
    src/game/Interactable.cpp
    src/game/Flashlight.cpp
    src/game/HouseGenerator.cpp
    src/game/ForestGenerator.cpp
)

set(UI_SOURCES
    src/ui/UIManager.cpp
    src/ui/Menu.cpp
    src/ui/HUD.cpp
    src/ui/TextRenderer.cpp
    src/ui/Button.cpp
    src/ui/Slider.cpp
    src/ui/Panel.cpp
)

set(AUDIO_SOURCES
    src/audio/AudioManager.cpp
    src/audio/Sound.cpp
)

set(PHYSICS_SOURCES
    src/physics/Physics.cpp
    src/physics/Collision.cpp
)

set(UTILS_SOURCES
    src/utils/Utils.cpp
    src/utils/PerlinNoise.cpp
)

# Main source
set(MAIN_SOURCE src/main.cpp)

# All sources combined
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${ENGINE_SOURCES}
    ${GRAPHICS_SOURCES}
    ${GAME_SOURCES}
    ${UI_SOURCES}
    ${AUDIO_SOURCES}
    ${PHYSICS_SOURCES}
    ${UTILS_SOURCES}
    ${MAIN_SOURCE}
)

# Create executable
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    Threads::Threads
    glfw
    glm
    freetype
    openal
    assimp
)

# Platform-specific settings
if(WIN32)
    target_link_libraries(${PROJECT_NAME} gdi32 user32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} dl)
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Cocoa -framework IOKit")
endif()

# Create output directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/assets/textures)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/assets/models)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/assets/sounds/ambient)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/assets/sounds/sfx)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/assets/sounds/music)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/assets/fonts)

# Copy shaders to build directory
if(EXISTS ${CMAKE_SOURCE_DIR}/shaders)
    file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()

# Copy assets if they exist
if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
    file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()

# Copy config file
if(EXISTS ${CMAKE_SOURCE_DIR}/config.cfg)
    file(COPY ${CMAKE_SOURCE_DIR}/config.cfg DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Debug/Release configurations
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

message(STATUS "=== Experiment Redbear Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
