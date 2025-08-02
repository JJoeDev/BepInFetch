BepInFetch

A C++ 20 application for managing BepInEx mods for Unity games

## Table of Contents

- [About](#about)
- [Features](#features)
- [Installation](#installation)
- [Building](#building)
- [Usage](#usage)
- [Screenshots](#screenshots)
- [Contributing](#contributing)
- [License](#license)

## About

BepInFetch is a C++ 20 application designed for managing BepInEx mods, originally created for the Gorilla Tag community
as an alternative to the community's MMM mod manager.

BepInFetch uses [GLFW](https://www.glfw.org/) for window management and OpenGL 4.4 for rendering. Its graphical user interface
is built with [Dear ImGui](https://github.com/ocornut/imgui). For networking BepInFetch makes use of [cpp-httplib](https://github.com/yhirose/cpp-httplib)
with SSL support from [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html) to request data and download content from
the internet. [nlohmann::json](https://github.com/nlohmann/json) is used for parsing JSON.

## Features

- Easy installation and removal of mods
- Easy updating of mods

## Installation

### Prerequisites
(todo)

## Building

> [!WARNING]
>
> **Platform support notice:** Currently, BepInFetch only supports 64-bit Microsoft Windows systems.
Building and running on Unix-like platforms such as Linux or MacOS is not supported due to platform specific dependencies such as OpenGL 4.4.

### Prerequisites
- CMake (version 3.26 or newer)
- C++ 20 capable compilerfor Windows (MSVC, clang, gcc)
- OpenSSL (Download Windows 64-bit non "lite" version: https://slproweb.com/products/Win32OpenSSL.html)
- - (Optionally) A build system like Ninja

**Step 1** Cloning the repository including submodules for dependencies like [Dear ImGui](https://github.com/ocornut/imgui)
```
  $ git clone --recursive https://github.com/JJoeDev/BepInFetch.git
```

**Step 2** Building BepInFetch. There are two approaches for this step.

- Using CMake and the auto detected compiler
```
  $ cmake .
  $ cmake --build .
```
- Using `.\build.bat` this requires the Ninja build system in order to work
```
  $ .\build.bat
```

Once BepInFetch has compiled a new directory by the name of `bin` should be available in the root of the repository. This is where you will find the `BepInFetch.exe` binary.

If you wish to contribute to BepInFetch, remember to read [contributing](#contributing)

## Usage
(todo)

## Screenshots
(todo)

## Contributing
(todo)

## License
(todo)
