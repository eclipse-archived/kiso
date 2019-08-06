---
title: "Quick start"
description: "How to start using kiso"
weight: 1
draft: false
toc: true
menu:
  main:
    parent: 3. User guide
    identifier: quick_start
    weight: 1
---

# Quick start

## Installation guide (manual) ##
### Basic tooling ###
* Download and install anaconda version [Anaconda3-2019.03](https://repo.continuum.io/archive/)
* Download and install arm-none-eabi version [7-2017-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
* Download and install doxygen version [v1.8.8](https://sourceforge.net/projects/doxygen/files/rel-1.8.8/)
* Download and install the [Latest version of CMake](https://cmake.org/download) (3.15 as of writing this)
* Download and use [HTerm](https://www.heise.de/download/product/hterm-53283/download) or another uart-console.
* In case the uart-driver does not work, please download [here](https://www.ftdichip.com/Drivers/VCP.htm) the right driver for your machine

**Note**: python, arm-none-eabi, doxygen need to be accessible via cmd-line. Therefore, you may need to modify the environment variable *PATH* on windows if it is not done in the installation.

### Extension tooling ###
* ```pip3 install meson```
* ```pip3 install ninja```

## Build HelloWorld with your supported hardware (meson build) ##

**Modify root meson.build**:
```bash
# Define project name
project('Hello World', 'c', default_options: meson.get_cross_property('project_config'))

# Get board & configuration
subdir('boards/<folder_of_supported_board>') # Only one allowed

# Get application
subdir('examples/HelloWorld/hello-world')
```

**Build**:
```bash
meson builddir --cross-file boards/<folder_of_supported_board>/meson_config_<mcu_id>.ini
cd builddir
ninja hex
```
**Clean**:
```bash
cd builddir
ninja clean
```

**Small summary**

In the meson.build, you always need:

- the project name
- to load the board specific information via `subdir('boards/<folder_of_supported_board>')`
- to load the example you want to use via `subdir('examples/HelloWorld/<example_folder>')`

To utilize your board with Kiso, please take a look at [the next step]({{< ref "advance_guide.md" >}})

## Build HelloWorld with your supported hardware (CMake build) ##

**Modify root CMakeLists.txt**:

Find the line which includes the application:
```cmake
add_subdirectory(examples/${KISO_BOARD_NAME}/c-leds)
```
And make sure it points to your application directory.

**Build**:
```bash
# You can execute these steps from anywhere
cmake <path_to_kiso_root> -B builddir -G "Ninja" -D KISO_BOARD_NAME=CommonGateway
cmake --build builddir
```
Currently the tested cmake generators are "Ninja" and "Unix Makefiles" so you should have either Ninja or GNU Make installed.
Refer to CMake documentation for other generators such as MinGW or MSYS Makefiles if you prefer to use them.

**Clean**:

To clean the built libraries and executables:
```bash
ninja clean
# or
make clean
```

To clean everything, including the CMake cache:
```bash
rm -r builddir
```

**Additional notes**:

- `KISO_BOARD_NAME` is a CMake variable that instructs for what board you are building. It's value is the same as the directory name, containing your boards configuration under the boards subdirectory.

- We have a toolchain_arm_gcc.cmake file that finds the ARM toolchain from your PATH and uses it. Should you need to specify your own toolchain for some reason, you could pass it to CMake like this:
```bash
cmake <...> -D CMAKE_TOOLCHAIN_FILE:PATH=<your_toolchain_file>
```

- To build the unit tests and execute them instead of building an application, you can pass to CMake:
```bash
cmake <...> -D ENABLE_TESTING=1
```
By default this will use your native compiler from your PATH instead of the ARM toolchain. Unit tests are executed on the Host machine:
```bash
cmake <kiso_root> -B build_tests -G Ninja -D KISO_BOARD_NAME=CommonGateway -D ENABLE_TESTING=1 # Generate for testing
cmake --build build_tests # Build unit tests
cd build_tests && ctest # Execute tests
```
