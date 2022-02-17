# DiscordFontChanger 📜:
### A tiny tool for quickly changing Discord's font!

# Usage
To use **DiscordFontChanger** you'll just need to download latest binary from the [releases](https://github.com/Kid0h/DiscordFontChanger/releases) (or build from source).
**DiscordFontChanger** Has no setup/installation steps and is built with simplicity in mind.

Changing fonts is as easy as follows:
```cmd
DiscordFontChanger.exe "Ubuntu Mono"
```
> Note: this tool currently support Windows only.

# Building from source
**DiscordFontChanger** has no dependencies and could be compiled "out of the box" with C++-17 enabled.
First thing you'll need to do is clone the repository to your local machine:
```sh
git clone https://github.com/Kid0h/DiscordFontChanger
cd DiscordFontChanger
```
Because there are no dependencies, the next step is actually compiling the tool. 
You could use the CMake script (`CMakeLists.txt`) provided with the tool to easily compile it using CMake:
```sh
mkdir build
cd build
cmake ../
cmake --build .
```