nFetch

A C++ 20 application for managing BepInEx mods for Unity games

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Building](#building)
- [Usage](#usage)
- [Screenshots](#screenshots)
- [Contributing](#contributing)
- [License](#license)

## Features

- Easy installation and removal of mods
- Easy updating of mods

## Installation

### Prerequisites
(todo)

## Building

**Platform support notice:** Currently, BepInFetch only supports 64-bit Microsoft Windows systems. Building and running on Unix-like platforms such as Linux or MacOS is not supported due to platform specific dependencies such as OpenGL 4.4.

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
