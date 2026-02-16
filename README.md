# Experiment Redbear

**A 3D Horror First-Person Game**

![Experiment Redbear Banner](assets/banner.png)

## 🎮 About The Game

Experiment Redbear is a terrifying first-person horror experience set in an abandoned house deep within a snowy forest. You awaken in a dark bedroom with nothing but a flashlight, unsure of how you got there or what lurks in the shadows.

### Features

- **Immersive Horror Atmosphere**: Dark environments, fog effects, and eerie ambience
- **First-Person Exploration**: Navigate through a mysterious house and surrounding forest
- **Dynamic Lighting**: Realistic flashlight with battery management
- **Interactive Environment**: Open doors, collect items, discover secrets
- **Procedural Forest**: Endless snowy forest with particle-based snowfall
- **Professional UI/UX**: Full menu system with graphics, audio, and gameplay settings

## 🏠 The Map

### The House
- **Master Bedroom**: Your starting location, contains the flashlight
- **Hallway**: Connects all rooms on the ground floor
- **Bathroom**: A small, confined space
- **Living Room**: Contains the fireplace and clues
- **Kitchen**: Connected to the back exit
- **Attic**: Second floor with mysterious artifacts

### The Forest
- **Procedurally Generated**: Trees placed using Perlin noise
- **Snow Particles**: Dynamic snowfall that follows the player
- **Fog Effects**: Dense fog that limits visibility

## 🎯 Gameplay

### Controls

| Action | Key |
|--------|-----|
| Move Forward | W |
| Move Backward | S |
| Move Left | A |
| Move Right | D |
| Run | Left Shift |
| Crouch | Left Ctrl |
| Jump | Space |
| Interact | E |
| Toggle Flashlight | F |
| Pause | Escape |
| Toggle FPS | F3 |
| Fullscreen | F11 |

### Interaction System
- Approach objects to see interaction prompts
- Open/close doors with the E key
- Pick up items like batteries, keys, and documents
- Flashlight battery depletes over time

## 🛠️ Technical Details

### Engine Features
- **OpenGL 4.5** rendering with modern shaders
- **Physically-Based Lighting** with multiple light types
- **Deferred Rendering** ready architecture
- **Post-Processing** effects (bloom, vignette, film grain)
- **Frustum Culling** for optimized rendering
- **Fixed Timestep** physics simulation

### System Architecture
```
src/
├── core/           # Core engine systems
│   ├── Window.cpp      # GLFW window management
│   ├── Input.cpp       # Keyboard, mouse input
│   ├── Timer.cpp       # Game loop timing
│   ├── Logger.cpp      # Logging system
│   ├── Config.cpp      # Settings management
│   └── ResourceManager.cpp
├── engine/         # Game engine
│   ├── Game.cpp        # Main game class
│   └── SceneManager.cpp
├── graphics/       # Rendering system
│   ├── Renderer.cpp    # Main renderer
│   ├── Shader.cpp      # Shader compilation
│   ├── Camera.cpp      # First-person camera
│   ├── Model.cpp       # 3D model loading
│   ├── Texture.cpp     # Texture management
│   └── Light.cpp       # Dynamic lighting
├── game/           # Game logic
│   ├── Player.cpp      # Player controller
│   ├── World.cpp       # Game world
│   ├── Door.cpp        # Interactive doors
│   ├── Item.cpp        # Collectible items
│   ├── Flashlight.cpp  # Flashlight system
│   ├── HouseGenerator.cpp
│   └── ForestGenerator.cpp
├── ui/             # User interface
│   ├── UIManager.cpp
│   ├── Menu.cpp        # Main/pause menu
│   ├── HUD.cpp         # In-game HUD
│   └── TextRenderer.cpp
├── audio/          # Sound system
│   ├── AudioManager.cpp
│   └── Sound.cpp
├── physics/        # Physics system
│   └── Collision.cpp
└── utils/          # Utility functions
    ├── Utils.cpp
    └── PerlinNoise.cpp
```

## 🔧 Building

### Prerequisites
- CMake 3.16 or higher
- C++17 compatible compiler
- OpenGL 4.5 support

### Dependencies
- **GLFW** - Window and input management
- **GLEW** - OpenGL extension loading
- **GLM** - Mathematics library
- **FreeType** - Font rendering
- **OpenAL** - Audio playback
- **Assimp** - 3D model loading
- **stb_image** - Image loading
- **stb_vorbis** - OGG audio loading

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/experiment-redbear.git
cd experiment-redbear

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build . --config Release

# Run
./bin/ExperimentRedbear
```

### Linux
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt install libglfw3-dev libglew-dev libglm-dev libfreetype6-dev \
                 libopenal-dev libassimp-dev

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Windows
```bash
# Use vcpkg for dependencies
vcpkg install glfw3 glew glm freetype openal-soft assimp

# Configure with Visual Studio
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build . --config Release
```

### macOS
```bash
# Install dependencies with Homebrew
brew install glfw glew glm freetype openal-soft assimp

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
```

## ⚙️ Configuration

Edit `config.cfg` to customize settings:

```ini
# Graphics
screen_width=1920
screen_height=1080
fullscreen=false
vsync=true
shadow_quality=2
msaa_samples=4

# Audio
master_volume=1.0
music_volume=0.7
sfx_volume=0.8

# Gameplay
mouse_sensitivity=0.1
fov=75.0
```

## 📁 Assets Structure

```
assets/
├── textures/       # Texture files (.png, .jpg)
├── models/         # 3D models (.obj, .fbx, .gltf)
├── sounds/         # Audio files (.ogg)
│   ├── ambient/
│   ├── sfx/
│   └── music/
└── fonts/          # TrueType fonts (.ttf)
```

## 🎨 Creating Custom Content

### Adding Textures
1. Place texture files in `assets/textures/`
2. Supported formats: PNG, JPG, BMP, TGA
3. Use power-of-two dimensions (512x512, 1024x1024, etc.)

### Adding 3D Models
1. Place model files in `assets/models/`
2. Supported formats: OBJ, FBX, glTF
3. Include normal maps and specular maps for best quality

### Adding Sounds
1. Place audio files in `assets/sounds/`
2. Use OGG Vorbis format for best compatibility
3. Mono for 3D positional audio, stereo for music/ambient

## 🐛 Debug Features

- Press **F3** to toggle FPS counter
- Check `experiment_redbear.log` for detailed logs
- Use debug build for additional validation

## 📝 License

This project is for educational purposes. See LICENSE file for details.

## 🤝 Credits

- **Engine**: Custom C++ OpenGL Engine
- **Graphics**: OpenGL 4.5 with GLSL shaders
- **Audio**: OpenAL Soft
- **Libraries**: GLFW, GLEW, GLM, FreeType, Assimp, stb libraries

---

*"In the darkness, something stirs. The experiment continues..."*
