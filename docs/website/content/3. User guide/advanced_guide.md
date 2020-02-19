---
title: "Advanced guide"
weight: 2
---

This guide goes into more details about the build system and folder structure. You will learn how to:

- Build unit tests and generate coverage reports
- Integrate a custom board into the Kiso build system
- Structure a user-defined app and build it using the Kiso build system

**Kiso allows configuration through a few CMake arguments:**

- `ENABLE_TESTING` - Enables unit testing build (default is `OFF`).
- `ENABLE_COVERAGE` - Enables generation of coverage reports from unit tests (default is `ON`; `ENABLE_TESTING` must be
  set to `ON` for this option to have an effect)
- `KISO_BOARD_PATH` - Configures the path to BSP layer. Might be one of the directories in `<kiso root>/boards` or an
  absolute external path. The folder pointed to must contain these CMake scripts:
  - `CMakeLists.txt` - Declares the targets needed to build a board implementation (BSP).
  - `board_config.cmake` - Sets up compiler and linker flags for the given target hardware. Also defines list of
    external/vendor libs to link into the BSP.
- `KISO_BOARD_NAME` - Represents the name of the board being built for. If `KISO_BOARD_PATH` is not set, this value will
  be used to determine which predefined board implementation to build. Will be set automatically to basename of
  `KISO_BOARD_PATH` if said variable was declared.
- `KISO_OS_LIB` - Configures OS library for Kiso. Currently supported options:
  - `freertos`
- `KISO_APPLICATION_PATH` - Configures which application code to compile and link with the platform. Must point to a
  folder containing a `CMakeLists.txt` declaring the targets needed to build your application. Possible values for this
  variable are all the directory names under the `/examples` subdirectory or absolute external path.
- `KISO_APPLICATION_NAME` - Represents the name of the application to build. If `KISO_APPLICATION_PATH` is not set, this
  value will be used to determine which example app to build. Will be set automatically to basename of
  `KISO_APPLICATION_PATH` if said variable was declared.

## Building a Custom Board ##

Users can define their own implementations (BSP) for their personal boards. To define a new board, start by creating a
folder with the name of your board somewhere in your project space. When compiling Kiso use the command line argument
`KISO_BOARD_PATH` to refer to your custom board folder (e.g.
`cmake <path-to-kiso-root> -B build -D KISO_BOARD_PATH=<path-to-my-board>`). Inside this folder create a
`CMakeLists.txt` and `board_config.cmake`. The `CMakeLists.txt` defines targets needed to compile and link your BSP,
while `board_config.cmake` sets up compiler flags and declares a list of vendor libs to link with. Your custom
`CMakesLists.txt` **must** contain a target called `bsp`. The `bsp` target must be a static-library containing the
board-specific implementation of the `Essentials/bsp` API. To compile against the BSP API your `bsp` target should at
minimum link against Kiso's `essentials` target.

The following snippet may be a starting point for your a custom boards' `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.6)

project("MyBoard" C ASM)

## Only compile BSP when actually compiling for embedded.
if(${CMAKE_CROSSCOMPILING})
  # List of source files that implement and facilitate the BSP API.
  file(GLOB BSP_SOURCES
    bsp/source/*.c
    bsp/source/*.s
    bsp/source/*.S
    bsp/source/protected/*.c
  )
  # Declare the required bsp target.
  add_library(bsp STATIC ${BSP_SOURCES})
  target_include_directories(bsp
  PUBLIC
    bsp/include
  PRIVATE
    bsp/source/protected
  )
  # List of additional libs from board_config.cmake
  target_link_libraries(bsp PUBLIC essentials ${KISO_BOARD_LIBS})
endif(${CMAKE_CROSSCOMPILING})
```

The corresponding `board_config.cmake` may look like this:

```cmake
## List of vendor libs the BSP implementation need. The list here references
## CMake targets representing those third-party libraries.
set(KISO_BOARD_LIBS <some-vendor-lib>)

## GCC portmacro to use in FreeRTOS (we imagine a Cortex-M4F in this example).
set(KISO_MCU_CORE ARM_CM4F)

## Set the location where we can find the Essentials and vendor-lib config
## headers.
set(BOARD_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/bsp/config)

## Common GCC compiler/linker settings needed to build for the given hardware.
set(BOARD_COMMON_OPTIONS
  -march=armv7e-m
  -mtune=cortex-m4
  -mfloat-abi=softfp
  -mfpu=fpv4-sp-d16
  -mthumb
  -mno-thumb-interwork
)

## Long list of compiler flags needed to compile for the given target hardware.
add_compile_options(
  ${BOARD_COMMON_OPTIONS}
  -ffunction-sections
  -fdata-sections
  -Wall
  -Wextra
  -Wmissing-prototypes
  -Wstrict-prototypes
  -O0
  -g
)

set(BOARD_LINKER_FLAGS
  ${BOARD_COMMON_OPTIONS}
  --specs=nosys.specs
  -T<path-to-linker-script>.ld
  -Xlinker
  -Map=out.map
  -Wl,--gc-sections
  -Wl,--start-group
  -lgcc
  -lc
  -lnosys
  -lm
  -Wl,--end-group
  -Wl,-u,_printf_float
)

## Join the items on the list separated by a space and write the result in
## CMAKE_EXE_LINKER_FLAGS
string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS "${BOARD_LINKER_FLAGS}")
```

## Building a Custom Application ##

User-defined applications -- just like examples, are built through the Kiso build-chain. We can use
`KISO_APPLICATION_PATH` to tell Kiso where to find our custom app.

Start by creating a new folder with the name of your application somewhere in your project space. When compiling Kiso
use the command line argument `KISO_APPLICATION_PATH` to refer to your custom board folder (e.g.
`cmake <path-to-kiso-root> -B build -D KISO_APPLICATION_PATH=<path-to-my-app>`). Inside this folder create a
`CMakeLists.txt`. This file defines targets needed to compile and link your application. Your custom `CMakesLists.txt`
**must** contain a target called `application`. The `application` target must be an executable, compiling your apps'
source files linking with required Kiso library and BSP targets.

The following shows an example set-up for custom application.

```tree
.
├── app
│   ├── CMakeLists.txt  # Defines targets needed to build app
│   ├── inc             # App internal includes
│   └── src
│       └── main.c      # App source files
├── cfg                 # App/Kiso config headers
├── build.sh            # Custom build script (can be as simple as invoking cmake)
└── lib
    └── kiso            # Kiso repository as hard-copy, symbolic link, or perhaps git-submodule
```

The above declared `app/CMakeLists.txt` may contain a minimal set up like this:

```cmake
cmake_minimum_required(VERSION 3.6)

project("MyApp" C)

if(${CMAKE_CROSSCOMPILING})
   add_executable(application
      src/main.c
   )
   target_include_directories(application PRIVATE source)
   target_link_libraries(application bsp essentials utils)

   include(FlashTarget)
   CREATE_FLASH_TARGET_JLINK(application)
endif()
```

With a setup like this you can tell build your app by telling Kiso where to find your application directory:

```sh
# This could be part of your build.sh
cmake <kiso_root> -B build -D KISO_BOARD_PATH=<path_to_bsp> -D KISO_APPLICATION_PATH=<path_to_my_app>
cmake --build build
# OR equivalent:
mkdir build && cd build
cmake <kiso_root> -D KISO_BOARD_PATH=<path_to_bsp> -D KISO_APPLICATION_PATH=<path_to_my_app>
cmake --build .
```

To learn **how to write application code** using Kiso, have a look at the code examples in `<kiso-root>/examples/...`.

**_NOTE:_** Currently the tested CMake generators are "Ninja" and "Unix Makefiles". For UNIX-like host systems, GNU Make
is the default and the option can be omitted, while for Windows CMake defaults to "Visual Studio", so make sure to
specify a generator explicitly.

## Flashing an Application ##

Depending on the hardware platform, a SWG or JTAG Debugger is the preferred tool. In the current example ST-link or a
SEGGER J-link can be used. Kiso's build tools currently support automated flashing with JLink software. The final
application is linked in the chosen example's CMake file. Let's take a look at an example for CommonGateway board:

```cmake
add_executable(application
  source/main.c
)
target_link_libraries(application bsp essentials utils ${KISO_OS_LIB} ${KISO_BOARD_LIBS})

CREATE_BINARY_TARGET(application)

CREATE_FLASH_TARGET_JLINK(application)
```

Here `add_executable` creates an ELF format file (application) with debug symbols. This file can be used in supported
IDEs (e.g. VS Code with Cortex-Debug plugin, Eclipse, etc.) to flash and debug your application with a target device.

Additional target is created - `application.bin` that contains the stripped and minimized binary that can be directly
flashed into the target's memory. Important note here is that CommonGateway devices' application code should be flashed
at address `0x08000000` and the .bin format does not have padding, so the address has to be taken into account by the
flasher. The proper address for different devices can be found in their respective link scripts.

Finally, if we are using a JLink debugger, we can call `CREATE_FLASH_TARGET_JLINK(application)`. This creates additional
target from the build system, called 'flash'. We can execute it like this: `cmake --build . --target flash`.

CMake then takes care to find our installed JLink Commander executable (jlink.exe in Windows/WSL or JLinkExe in Linux),
prepare a command script and execute it. The script will automatically find the connected device, flash and verify the
binary at the proper address and reset the device.
