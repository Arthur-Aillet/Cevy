# Getting Started
This section will help you get started on your Cevy journey as quilckly as possible. It will walk you through setting up your development environment an writing a simple Cevy app.

# Quick Start
If you want to drive in immediately and already have a working C++ setup, fell free to follow this "quick start" guid. Otherwis, move on to the next page.

## Try the Examples

1. Clone the [Cevy Repository][1]

```bash
git clone git@github.com:Arthur-Aillet/Cevy.git Cevy
```

2. Navigate to the new "Cevy" folder

```bash
cd Cevy
```

3. Switch to the version as you want or the latest version

``` Actually is not possible, we just have one version```

4. Try the exemple in the [R-Type project][2]

## Add Cevy as a Dependency

You can add in your CMakefile or in a specific file a FetchModule included in your CMakefile

```cmake
include(FetchContent)
FetchContent_Declare(
    cevy
    GIT_REPOSITORY https://github.com/Arthur-Aillet/Cevy
    GIT_TAG main (or another the tag version)
)
FetchContent_Populate(cevy)
```


# Setup

## C++ Setup
All Cevy app and engine is written in C++. This means that before we begin, we need to set up our C++ development environment.

You can check that you have installed C++ on our linux computer with this command:

```bash
gcc --version
```

### Installing C++

1. On **Debian**, **Ubuntu**, **Linux Mint**, **Deepin**

```bash
sudo apt-get install gcc
```

2. On **Arch Linux**, **Manjaro**, **ArkOS**, **EndeavourOS**

```bash
sudo pacman -S gcc
```

3. On **RedHat Enterprise Linux**, **Fedora**, **CentOS**, **SUSE**

```bash
sudo dnf install gcc
```


### Install OS dependencies

####    Linux

1. Ubuntu

```bash
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libglfw3-dev
```

2. Fedora

```bash
sudo dnf install alsa-lib-devel xorg-x11-server-devel libXrandr-devel libXi-devel mesa-libGL-devel mesa-libGLU-devel libXcursor-devel libXinerama-devel glfw-devel
```

3. Pacman

```bash
sudo pacman -S alsa-lib xorg-server libxrandr libxi mesa libglu libxcursor xorg-server-common libxinerama glfw-x11
```

####    Windows

- Run the [Visual Studio 2019 build tools installer][3]
- For easy setup, select the `Desktop development with C++` workload in the installer
- For a minimal setup, follow these steps:
  1. In the installer, navigate to `Individual components`
  2. Select the latest `MSVC` for your architecture and version of Windows
  3. Select the latest `Windows SDK` for your version of Windows
  4. Select the `C++ CMake tools` for Windows component
  5. Install the components


# Create a new Cevy
Now we are ready to set up a Cevy project! Cevy is just a normal C++ dependency. You can either add it to an existing C++ project or create a new one. For completeness we will assume you are starting from scratch.

## Create a new C++ executable project
First create a new folder for our project. For exemple, if you want to name your project "my_cevy_game", you can do so with the following command in your terminal:
```bash
mkdir my_cevy_game
cd my_cevy_game
```

In this folder, create a CMakeLists.txt file. This file is used by CMake to understand how to build your project. Here's a basic example for a C++ project:

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name
project(mon_projet_c++)

# add the executable
add_executable(mon_projet_c++ main.cpp)
```
In this example, we've defined a minimum version requirement for CMake, named our project and added an executable to be built from the main.cpp 1 file.

Create a main.cpp file in the same folder. This file will contain your program's main code. To start with, it can simply display "Hello, World!" as follows:


```cpp
#include <iostream>

int main() {
   std::cout << "Hello, World!";
   return 0;
}
```

## Add Cevy as a dependency

You can just modify your CMakeLists.txt at the root of your project.
```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name
project(mon_projet_c++)

# add the executable
add_executable(mon_projet_c++ main.cpp)

include(FetchContent)
FetchContent_Declare(
    cevy
    GIT_REPOSITORY https://github.com/Arthur-Aillet/Cevy
    GIT_TAG main (or another the tag version)
)
FetchContent_Populate(cevy)
```
## Build Cevy
Now you can run your program with these lign

```bash
cmake -S . -B ./build
make --no-print-directory -C build
```


[1]: https://github.com/Arthur-Aillet/Cevy "Title"
[2]: https://github.com/Arthur-Aillet/RType "Title"
[3]: https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16 "Title"
[4]: https://github.com/microsoft/vcpkg "Title"