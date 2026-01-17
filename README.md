# Hyper Boing

![Hyper Boing Gameplay](docs/images/hyper_boing.gif)

## 🎮 About

**Hyper Boing** is a Super Pang-inspired arcade game originally developed in **2001** as a university computer graphics class project by **Miguel Martinez Rodilla**. The game was initially built using DirectX and has been recently modernized and ported to **SDL 2** in **2025**, bringing this classic project back to life with cross-platform compatibility.

## 📜 License

This project is licensed under the **MIT License** - feel free to use, modify, and distribute it as you see fit.

## 🛠️ Building the Project

### Prerequisites

To compile Hyper Boing, you need to set up the following **environment variables** for SDL 2:

1. **`SDL_ROOT`** - Path to your SDL2 installation directory  
   Example: `C:\Libraries\SDL2-2.28.0`

2. **`SDL_IMAGE_ROOT`** - Path to your SDL2_image installation directory  
   Example: `C:\Libraries\SDL2_image-2.6.3`

3. **`SDL_MIXER_ROOT`** - Path to your SDL2_mixer installation directory  
   Example: `C:\Libraries\SDL2_mixer-2.6.3`

4. **`SDL_TTF_ROOT`** - Path to your SDL2_ttf installation directory  
   Example: `C:\Libraries\SDL2_ttf-2.20.2`

### Compilation

Once the environment variables are set, open the solution in Visual Studio and build the project.

## 🎯 How to Play

### Default Controls

#### Player 1
- **Arrow Left** - Move left
- **Arrow Right** - Move right
- **Arrow Up** - Shoot

#### Player 2 (if enabled)
- **A** - Move left
- **D** - Move right
- **W** - Shoot

### Debug Features

- **F1** - Toggle debug console
- **Backtick (`)** or **F9** - Open AppConsole
- Type `/help` in the console to see available commands

### Game Objective

Pop all the bouncing balls by shooting your harpoon! Each ball splits into smaller balls when hit, until they're finally destroyed. Avoid touching the balls or you'll lose a life. Complete each stage before time runs out!

## 🎨 Features

- Classic arcade-style gameplay
- Multiple stages with increasing difficulty
- 1-2 player support
- Time-based challenges
- Power-ups and special items
- Score tracking
- Debug console with real-time game state inspection
- Smooth SDL 2 rendering
- Custom bitmap font system (`.fnt` format) for retro-styled text rendering

## 🔧 Technical Details

- **Language**: C++14
- **Graphics**: SDL 2
- **Audio**: SDL_mixer
- **Image Loading**: SDL_image
- **Text Rendering**: Custom bitmap fonts (`.fnt` files)
- **Original API**: DirectX (2001)
- **Modern Port**: SDL 2 (2025)

## 🙏 Acknowledgments

Special thanks to the original Super Pang arcade game that inspired this project, and to the SDL community for providing excellent cross-platform libraries that made this port possible.

---

**Enjoy playing Hyper Boing!** 🎈💥
