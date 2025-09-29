# TheEye 👁️

A smart screensaver with a 3D-rendered eye that follows you in real-time using AI and webcam tracking.

![TheEye Demo](link-to-demo-gif-or-image)

## 🚀 Features

- Real-time 3D eye rendering with OpenGL
- Webcam Head Tracking using AI-powered face detection

## 🛠️ Current Status

**✅ Working & Tested:**
- Linux Mint (primary development platform)
- Automated build script
- DEB package generation

**🔧 Known Limitations:**
- Currently Linux-focused (Windows support planned)
- Manual dependency setup required
- Submodules use SSH URLs
- The project generates an executable file, you need to configure your screensaver to launch it.

## 📦 Installation


1. Clone the repository with submodules::

git clone --recursive https://github.com/MickaelOnTheWave/TheEye.git

Configure and build :

cd TheEye
mkdir build
cd build
cmake ../
cmake --build .

Run from the build directory:

ConfigurationEditingTool — GUI configuration editor

TaskTool — background runner


### Prerequisites
```bash
# List actual dependencies here (OpenCV, Qt, etc.)
sudo apt-get install libopencv-core4.5d libeigen3-dev Iconv glfw
