# TheEye 👁️

[![Platform](https://img.shields.io/badge/platform-Linux-blue.svg)](https://github.com/MickaelOnTheWave/TheEye)
[![C++](https://img.shields.io/badge/C++-17-00599C.svg)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3+-5586A4.svg)](https://www.opengl.org/)
[![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)](https://opencv.org/)
[![AI](https://img.shields.io/badge/AI-MobileNet%20SSD-FF6F00.svg)](https://github.com/chuanqi305/MobileNet-SSD)

**The screensaver that screensaves back.** An intelligent prank screensaver that lulls viewers into thinking your computer is still awake... until an eye suddenly opens and starts watching them.

![TheEye Demo](link-to-demo-gif-or-image)
*Demo coming soon - but spoilers ruin the surprise!*

## 🎭 The Concept

Picture this: Someone walks by your "idle" computer and sees your normal desktop—maybe an email client, a code editor, whatever was on screen. They might even stop to look at something interesting. They lean in closer...

**And then the eye opens.**

TheEye creates a perfect illusion by capturing a screenshot of your actual desktop and rendering it as a 3D texture. The screen appears completely normal until AI-powered face detection realizes someone has been staring for a few seconds. That's when the "eyelids" (textured with your desktop screenshot) slowly open to reveal a 3D eye that locks onto the viewer's position and follows their every move.

It's the digital equivalent of a jump scare, but with more sophistication and way cooler technology.

## ✨ Features

- **🖼️ Perfect Desktop Camouflage**: Captures and renders your actual screen as a seamless texture
- **🧠 AI-Powered Face Detection**: Uses MobileNet neural network to detect when someone is watching
- **⏱️ Strategic Timing**: Waits for sustained attention before revealing itself
- **👁️ 3D Eye Tracking**: Real-time face position tracking
- **🎨 Custom OpenGL Renderer**: Efficient 3D graphics with texture-mapped eyelids for the reveal effect
- **⚡ Lightweight Performance**: Optimized to run smoothly as a background screensaver

## 🏗️ Technical Architecture

TheEye combines several technologies to create this experience:

**The Illusion Pipeline:**
1. **Screenshot Capture**: Grabs the current desktop state before activating
2. **Texture Mapping**: Projects the screenshot onto 3D geometry (background plane + eyelid surfaces)
3. **Face Detection**: OpenCV + MobileNet SSD continuously monitors for faces
4. **Trigger Logic**: Waits for sustained face presence (several seconds of detected staring)
5. **The Reveal**: Animates eyelids opening, transitioning texture coordinates to expose the 3D eye
6. **Active Tracking**: Eye orientation updates in real-time based on detected face position

**Technology Stack:**
- **Computer Vision**: OpenCV with MobileNet SSD for face detection
- **Graphics Engine**: Custom OpenGL 3.3+ renderer with GLSL shaders
- **Window Management**: GLFW for fullscreen rendering
- **Build System**: CMake with automated packaging

## 🛠️ Current Status

**✅ Working Features:**
- Screenshot capture and texture rendering
- Desktop camouflage mode
- AI-powered face detection with trigger logic
- Smooth eyelid opening animation
- Real-time 3D eye tracking
- Tested on Mint/Cinnamon and Ubuntu/KDE

**🔮 Roadmap:**
- Windows support
- Procedural iris texture with more details
- Better eye animations
- Configuration options (trigger delay, eye appearance)

- Customizable appearance and behavior settings

## 📦 Installation

### Quick Install - Debian Package (Recommended)

Download the latest release from the [Releases page](https://github.com/MickaelOnTheWave/TheEye/releases) and install:

```bash
sudo dpkg -i the-eye_*.deb
```

### Building from Source

#### Prerequisites

Install the required dependencies on Ubuntu/Debian-based systems:

```bash
sudo apt-get install libopencv-dev libeigen3-dev libglfw3-dev cmake build-essential
```

#### Build Instructions

1. Clone the repository with submodules:
```bash
git clone --recursive https://github.com/MickaelOnTheWave/TheEye.git
cd TheEye
```

2. Build the project:
```bash
cd scripts
./buildLinuxBinaries.sh
```

3. (Optional) Create a DEB package:
```bash
./makeDebianPackage.sh
```

The package will be generated at: `TheEye/build/debPackage/the-eye_*.deb`

## 🚀 Usage

After installation, configure your system screensaver to launch TheEye:

```bash
the-eye
```

**What happens:**
1. TheEye captures your current desktop and displays it seamlessly
2. Your webcam activates (but stays invisible to the viewer)
3. When someone stares at the "desktop" for a few seconds...
4. The eye reveals itself and tracks their movement
5. Press any key or move your mouse to exit

**Pro tip:** Don't tell people about this before they experience it. The surprise is half the fun.

## 🎯 Why I Built This

I wanted to create something that would genuinely surprise people who wander by my idle computer—something that combines technical sophistication with a bit of playful mischief. It's also a perfect sandbox for exploring technologies I'm passionate about: real-time 3D graphics, computer vision, and AI integration.

The challenge of creating a convincing illusion (the fake desktop), implementing smooth real-time face tracking, and orchestrating the reveal animation made this an incredibly fun project that showcases modern C++ development, OpenGL rendering techniques, and practical AI applications.

## 🤝 Contributing

This is primarily a portfolio project, but suggestions and bug reports are welcome! Feel free to:
- Open issues for bugs or feature requests  
- Submit pull requests for improvements
- Share your experience (or your victims' reactions) using TheEye

---

*If you like this project, give it a ⭐! And maybe set it up on a colleague's computer.*
