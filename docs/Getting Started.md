# Getting Started
This section will help you get started on your Cevy journey as quilckly as possible. It will walk you through setting up your development environment an writing a simple Cevy app.

# Quick Start
If you want to drive in immediately and already have a working C++ setup, fell free to follow this "quick start" guid. Otherwis, move on to the next page.

### Try the Examples

1. Clone the [Cevy Repository][1]

``` git clone git@github.com:Arthur-Aillet/Cevy.git Cevy```

2. Navigate to the new "Cevy" folder

``` cd Cevy```

3. Switch to the version as you want or the latest version

``` Actually is not possible, we just have one version```

4. Try the exemple in the [R-Type project][2]

### Add Cevy as a Dependency

You can add in your CMakefile or in a specific file a FetchModule included in your CMakefile

```
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

``` gcc --version```

### Installing C++

1. On **Debian**, **Ubuntu**, **Linux Mint**, **Deepin**

``` sudo apt-get install gcc```

2. On **Arch Linux**, **Manjaro**, **ArkOS**, **EndeavourOS**

``` sudo pacman -S gcc```

3. On **RedHat Enterprise Linux**, **Fedora**, **CentOS**, **SUSE**

``` sudo dnf install gcc```


### Install OS dependencies

#### Linux

##### Ubuntu

``` sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libglfw3-dev```

##### Fedora

``` sudo dnf install alsa-lib-devel xorg-x11-server-devel libXrandr-devel libXi-devel mesa-libGL-devel mesa-libGLU-devel libXcursor-devel libXinerama-devel glfw-devel```

##### Pacman

``` sudo pacman -S alsa-lib xorg-server libxrandr libxi mesa libglu libxcursor xorg-server-common libxinerama glfw-x11```





[1]: https://github.com/Arthur-Aillet/Cevy "Title"
[2]: https://github.com/Arthur-Aillet/RType "Title"
