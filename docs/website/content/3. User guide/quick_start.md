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
cmake <kiso_root> -Bbuilddir -DKISO_BOARD_NAME=<selected_board>
cmake --build builddir
# OR equivalent:
mkdir builddir && cd builddir
cmake <kiso_root> -DKISO_BOARD_NAME=<selected_board>
cmake --build .
```
Currently the tested CMake generators are "Ninja" and "Unix Makefiles". For UNIX-like host systems, GNU Make is the default and the option can be omitted, while for Windows CMake defaults to "Visual Studio", so make sure to specify a generator explicitly.

Which generator to use is specified with an argument to CMake:
```bash
cmake <kiso_root> -G"Unix Makefiles" <...>
```

**Clean**:

- To clean the built libraries and executables:
  ```bash
  cmake --build builddir --target clean
  ```
  Or using the tool directly:
  ```bash
  ninja clean
  # or
  make clean
  ```

- To clean everything, including the CMake cache:
  ```bash
  rm -r builddir
  ```

**Additional notes**:

- `KISO_BOARD_NAME` is a CMake variable that instructs for what board you are building. It's value is the same as the directory name, containing your boards configuration under the boards subdirectory.

- We have `toolchain_arm_gcc.cmake` file that finds the ARM toolchain from your PATH and uses it.

  Should you need to explicitly specify your own toolchain, you could pass your own toolchain file to CMake like this:
  ```bash
  cmake <...> -DCMAKE_TOOLCHAIN_FILE:PATH=<your_toolchain_file>
  ```
  OR you can use the provided toolchain file, but specify one or both of these variables:
  - `ARM_TOOLCHAIN_DIR` - the directory where your toolchain executables are located
  - `ARM_TOOLCHAIN_PREFIX` - prefix for the executables. E.g. **`arm-none-eabi-`gcc**. Also applies to the automatic finding of toolchain in case toolchain file or directory are not provided.

- To build the unit tests and execute them instead of building an application, you can pass to CMake:
  ```bash
  cmake <...> -DENABLE_TESTING=1
  ```
By default this will use your native compiler from your PATH instead of the ARM toolchain. Unit tests are executed on the Host machine:
```bash
cmake <kiso_root> -B build_tests -DKISO_BOARD_NAME=CommonGateway -DENABLE_TESTING=1 # Generate for testing
cmake --build build_tests # Build unit tests
cd build_tests && ctest # Execute tests
```
