# Enxel Engine

Enxel is a voxel game engine currently in development.

## Build

This project uses CMake and Visual Studio. It should build and run easily, but it hasn’t been extensively tested yet.

### Requirements

- Vulkan SDK installed  
- CMake installed

See [CMake Downloads](https://cmake.org/download/)  
Follow instructions on [Vulkan Tutorial](https://vulkan-tutorial.com/Development_environment) to set up Vulkan.

### Installation

1. Clone the repo:
    ```bash
    git clone git@github.com:Roqum/EnxelEngine.git
    ```
2. Open a terminal and navigate to the project directory.
3. Create and enter a build directory:
    ```bash
    mkdir build
    cd build
    ```
4. Run CMake to generate the build system:
    ```bash
    cmake ..
    ```
5. Open the generated Visual Studio solution from the `build` folder.
6. Build and run the project from Visual Studio.
