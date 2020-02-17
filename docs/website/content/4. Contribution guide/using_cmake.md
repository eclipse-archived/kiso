---
title: "Using CMake"
weight: 6
---

# How is CMake Organized

The Kiso project consists of subprojects, all managed by the `CMakeLists.txt` file from the root directory:

- `core`</br>
    Main project components and utilities as libraries
- `boards`</br>
    Supported boards with their own specific configuration and BSP
- `thirdparty`</br>
    Third party libraries
- `examples`</br>
    Basic examples of applications using the core and BSP components
- `docs`</br>
    Documentation

In addition to the CMakeLists.txt files, there are a few `.cmake` configuration files defining different parts of required configuration and variables for building.

Let's take a look at the basic content of the root `CMakeLists.txt` line by line:

```cmake
option(ENABLE_TESTING "Configure a build tree for testing instead of normal application mode" OFF)

include(kiso_defaults.cmake OPTIONAL)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE AND NOT ${ENABLE_TESTING})
   include(cmake/ArmToolchain.cmake)
endif()

project (Kiso C)

include(${KISO_BOARD_PATH}/board_config.cmake)

add_subdirectory(boards)
add_subdirectory(core/essentials)
add_subdirectory(core/utils)
add_subdirectory(core/connectivity)

add_subdirectory(thirdparty)

add_subdirectory(${KISO_APPLICATION_PATH})

add_subdirectory(docs)
```

## Build Variants
CMake could be configured to either generate build directory for unit tests or target libraries and binaries.
Both configuration variants only include in the build what's needed for a specific board.</br>
Switching between build modes is provided by the `ENABLE_TESTING` variable that can be provided to CMake externally or defaults to `OFF`, meaning a normal application build.
Where applicable, each subproject should respect this variable and define the proper libraries, dependencies and build flags for the chosen build variant.

## Default Configuration
For easier configuration and avoidance of extremely long cmake commands, the project has a default configuration providing all the necessary variables.
Users could modify the config or provide multiples so it's easy to swap them quickly.

The default configuration is provided by the `kiso_defaults.cmake` file and should contain at least the minimum set of required variables for CMake configuration so that a simple
```bash
cmake <project_root>
```

is enough to configure a working build tree with a supported generator.

## Cross-compilation Toolchain
So for all of the boards this project supports use ARM-based MCUs so an ARM compiler is required for building an application with Kiso. In order to configure CMake for cross-compilation,
a toolchain file must be provided before the first `project()` call in any CMakeLists. This can happen either externally by calling
```bash
cmake <project_root> -DCMAKE_TOOLCHAIN_FILE=PATH:/path/to/toolchain.cmake
```
or internally by using the project's included `cmake/ArmToolchain.cmake`.

The included toolchain configuration makes sure to find the ARM compiler from the user's PATH and configure CMake for proper cross-compilation.
Some of the required configuration includes the following:
```cmake
# Use the found compiler from PATH
set(CMAKE_C_COMPILER ${ARM_TOOLCHAIN_PREFIX}gcc)

# CMake tries to compile test-programs, instruct it that they will not be executable on the cross-compilation host machine
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Use the ARM sysroot that comes with the compiler toolchain
set(CMAKE_SYSROOT ${ARM_TOOLCHAIN_DIR}/../arm-none-eabi)
```

The default toolchain file searches for ARM toolchain with prefix `arm-none-eabi-`. This prefix can be modified by overriding `ARM_TOOLCHAIN_PREFIX` either externally, in the `kiso_defaults.cmake` or directly by providing your own toolchain file.

## Building Unit Tests
Building and running the unit tests happens on the Host machine, so we are not cross-compiling.
It is recommended that the user uses separate directories if they need to compile unit tests and applications simultaneously, CMake does not bode well with switching compilers in the same build tree and requires cache cleanup and possibly recreating the whole configuration.

Build for testing is configured by providing the `ENABLE_TESTING=ON` flag again either externally or within the configuration file. Possible values that CMake recognizes are `ON`, `1` and `TRUE` for enabling the option, `OFF`, `FALSE` and `0` for disabling it.

In this case the toolchain file is not used and CMake finds the default compiler for the Host automagically.
In case what the compiler is trying to use is not the one you expect, you can either modify your PATH to only have the proper one, or better - define the environment variable `CC` (also possible in config).
CMake respects the `CC` variable and also uses the adjacent CXX compiler with all their proper paths and tools.

## Board Configuration
Each board has a specific configuration, supported hardware, libraries and compile definitions that come with the BSP in most cases. The board configuration is loaded early and used extensively.

We have a separate `board_config.cmake` for each board, which defines mostly compiler flags, definitions and the linker script which is used to link all libraries together with an example application.
This file also defines the following variables:

- `KISO_MCU_FAMILY` [Required]</br>
    The root of the MCU series this board uses.
- `KISO_MCU_SERIES` [Required]</br>
    Specific MCU series (A series is defined through a set of different devices)
- `KISO_MCU_TYPE` [Optional]</br>
    Defines a set of MCU devices sharing the same core, etc, but have different configurations.
- `KISO_MCU_DEVICE` [Optional]</br>
    Defines the exact MCU part number.
- `KISO_MCU_REVISION` [Optional]</br>
    Different revisions of the same part number.
- `KISO_MCU_CORE` [Required]</br>
    CPU core as expected by FreeRTOS to include the correct portmacros. (Make sure you also have the corresponding compile definition for the compiler)
- `KISO_BOARD_LIBS` [Optional]</br>
    A list of thirdparty libraries that should be included with the build. Possible values are directory names from the `/thirdparty` subdirectory.

## Libraries

### Boards
Each board has it's own `CMakeLists.txt` file, which should define a library, called `bsp`. The BSP lib contains specific API implementations of the components supported by the board. It implements the
interfaces from `essentials/include/bsp`.

### Core
Each of the libraries in Core has it's own `CMakeLists.txt` file which handles interface libraries, static library and includes the specific unit tests.
Interface libraries [[3]](#references) are not always necessary, but some libraries have interdependent code and headers.</br>
The libraries are only available in a cross-compilation build so that unit tests can be built on the host machine without CMake trying to create a static library from ARM code.

MCU specific code for each library is located in hierarchical matter according to the following pattern:

`core/*library*/[include|source]/mcu/${KISO_MCU_FAMILY}/${KISO_MCU_SERIES}/${KISO_MCU_TYPE}/${KISO_MCU_DEVICE}`

Always place the newly developed headers and sources in the topmost possible directory so the code works for as many devices as possible.</br>
Currently only `MCU_FAMILY` and `MCU_SERIES` are required so not all boards define the remaining variables. This also means that whenever there directories appear, the CMake files should be checked and missing variables populated.
Make sure to use lowercase letters for directory naming where possible to avoid problems in case-sensitive filesystems.

Tests and mockup test headers go into the `test` subdirectory of each library.
For information on how testing is organized in Kiso, please check the [Unit Testing Guide]({{< relref "unit_testing_guide" >}}).

### Third Party
Some third party libraries are shipped with Kiso and are ready to compile and use out of the box. There are also libraries that cannot be integrated due to licensing terms.
Each thirdparty library that Kiso uses or ships with is under it's own directory in the `/thirdparty` tree. It's important that the directory name and CMake target name for the library match so that CMake is able to build only what's necessary instead of every library in the subtree.
For thirdparty libraries that do not support CMake out of the box, we make sure to create a basic `CMakeLists.txt` file to enable integration of that library with Kiso.

In addition to the `CMakeLists.txt`, each library's subdirectory also contains:

- Instructions or scripts for how to update and/or obtain the library
- a `3rd-party` directory - the actual third party code and licenses
- `source` and `include` directories - custom patches or modifications on the library
- a `test` directory - unit testing mockups

### Examples
Examples are small applications that use the Kiso infrastructure (essentials/utils), BSP and MCU interfaces and show a basic concept for working with the APIs. They are used to demonstrate how the devices on the board are interfaced with, how to setup peripherals, how to read sensor values, etc. Their `CMakeLists.txt` is very simple and almost identical for all example applications. This is the only CMake file that defines building actual executables - one that can be used with a debugger and one that can be directly flashed to the target board.

Choosing which application to build when configuring CMake is done by the `KISO_APPLICATION_PATH` variable and defaults currently to `examples/CommonGateway/c-leds`. Possible values for this variable are all the directory names under the `/examples` subdirectory.

### Documentation
The `docs` directory contains a `Doxyfile.in` file, which is patched by CMake to supply the proper input and output paths. HTML reference documentation is generated and available to use via the `docs` cmake target.

## References
CMake build generator: [cmake.org](https://cmake.org/cmake/help/latest/manual/cmake.1.html)</br>
CMake toolchains: [cmake.org/.../cmake-toolchains](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html)</br>
CMake Interface Libraries: [cmake.org/.../add_library](https://cmake.org/cmake/help/v3.7/command/add_library.html#interface-libraries)</br>

