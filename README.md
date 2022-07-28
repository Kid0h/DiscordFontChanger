# DiscordFontChanger 📜
A tiny tool for quickly changing Discord's font!

# Usage
To use **DiscordFontChanger** you'll just need to download latest binary from the [releases](https://github.com/Kid0h/DiscordFontChanger/releases) (or build from source).
**DiscordFontChanger** Has no setup/installation steps and is built with simplicity in mind.

Changing fonts is as easy as follows:
```bash
DiscordFontChanger.exe "Ubuntu Mono"
```
If you have different Discord clients, you can target a specific one using the `-c` or `--client` option:
```bash
DiscordFontChanger.exe --client stable "Ubuntu Mono"
DiscordFontChanger.exe --client canary "Ubuntu Mono"
DiscordFontChanger.exe --client ptb "Ubuntu Mono"
```

# Building from source
**DiscordFontChanger** has no dependencies and could be compiled "out of the box" with C++-17 (needed for <filesystem>) enabled.
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
