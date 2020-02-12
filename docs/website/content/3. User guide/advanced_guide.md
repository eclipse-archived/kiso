---
title: "Advanced guide"
weight: 2
---

In this guide more specific configurations will be shown. 
Example cases:

 - Utilize the features of the used board. Peripherals such as I2C, SPI and etc.
 - Add RTOS
 - Add various Kiso components and modules

For simplicity, let us use our code with an STM32L4 based board: `CommonGateway`

### Kiso allows configuration through a couple of CMAKE arguments: ###

 - `ENABLE_TESTING` - Enables Unit testing build (default is `OFF`)
 - `ENABLE_COVERAGE` - Enables Coverage reporting from the Unit tests
 - `ENABLE_ALL_FEATURES` - Enables all platform features (only used for static code analysis and unit-tests; default is `OFF`)
 - `KISO_BOARD_NAME` - Configures which Device will be used with the platform
 - `KISO_OS_LIB` - Configures OS library for Kiso
 - `KISO_EXAMPLE` - Configures which application code to compile and link with the platform (Possible values for this variable are all the directory names under the `/examples` subdirectory.)

## Utilize your board ##
In order to be able to use any Kiso component, you will need to add the basic components: `Essentials` and `Utils`.
To do so, you need to make sure root **CMakeLists.txt** is including:

```cmake
# Get kiso libraries
add_subdirectory(core/essentials)
add_subdirectory(core/utils)
```
Find the line which includes the application:
```cmake
add_subdirectory(examples/${KISO_BOARD_NAME}/${KISO_EXAMPLE})
```
And make sure it points to your application directory.

Configure Project name and verify that the used board config is included:
```cmake
# Define project name
project (Kiso C)

# Get board & configuration
add_subdirectory(boards)

# Get 3rd party libraries
add_subdirectory(thirdparty)

# Get kiso libraries
add_subdirectory(core/essentials)
add_subdirectory(core/utils)
```
The root **CMakeLists.txt** file points to the locations of:

- Application example
- Third-party libraries
- Board configuration
- Kiso specific modules (e.g Core, Utils, Essentials)

**_NOTE:_**  Those locations contain separate CMakeLists.txt configurations.


## Run application on your board ##

**Build application**:
```bash
# You can execute these steps from anywhere
cmake <kiso_root> -Bbuilddir -DKISO_BOARD_NAME=<selected_board>
cmake --build builddir
# OR equivalent:
mkdir builddir && cd builddir
cmake <kiso_root> -DKISO_BOARD_NAME=<selected_board>
cmake --build .
```

**_NOTE:_** Currently the tested CMake generators are "Ninja" and "Unix Makefiles". For UNIX-like host systems, GNU Make is the default and the option can be omitted,
while for Windows CMake defaults to "Visual Studio", so make sure to specify a generator explicitly.

**Flashing application**:

Depending on the hardware platform, a SWG or JTAG Debugger is the preferred tool. In the current example ST-link or a SEGGER J-link can be used. Kiso's build tools currently support automated flashing with JLink software.
The final application is linked in the chosen example's CMake file. Let's take a look at an example for CommonGateway board:
```cmake
add_executable(application
  source/main.c
)
target_link_libraries(application bsp essentials utils ${KISO_OS_LIB} ${KISO_BOARD_LIBS})

add_custom_target(application.bin
  COMMAND ${CMAKE_OBJCOPY} -O binary -R .usrpg $<TARGET_FILE:application> ${CMAKE_CURRENT_BINARY_DIR}/application.bin
  COMMENT "Creating flashable binary ${CMAKE_CURRENT_BINARY_DIR}/application.bin"
)
add_dependencies(application.bin application)

CREATE_FLASH_TARGET_JLINK(application)
```

Here `add_executable` creates an ELF format file (application) with debug symbols. This file can be used in supported IDEs (e.g. VS Code with Cortex-Debug plugin, Eclipse, etc.)
to flash and debug your application with a target device.

Additional target is created - `application.bin` that contains the stripped and minimized binary that can be directly flashed into the target's memory. Important note here is that CommonGateway devices' application code
should be flashed at address `0x08000000` and the .bin format does not have padding, so the address has to be taken into account by the flasher.
The proper address for different devices can be found in their respective link scripts.

Finally, if we are using a JLink debugger, we can call `CREATE_FLASH_TARGET_JLINK(application)`. This creates additional target from the build system, called 'flash'.
We can execute it like this: `cmake --build . --target flash`.

CMake then takes care to find our installed JLink Commander executable (jlink.exe in Windows/WSL or JLinkExe in Linux), prepare a command script and execute it.
The script will automatically find the connected device, flash and verify the binary at the proper address and reset the device.

## Summary ##
In the root **CMakeLists.txt**, you always need to:

- set project name
- load the board specific information `KISO_BOARD_NAME` set to specific board directory
- load the 3rd party needed libraries you just have to place them in `/thirdparty` directory
- load the Kiso components via `add_subdirectory('core/<kiso_component_name>')`
- load the example you want to use via `add_subdirectory('examples/HelloWorld/<example_folder>')` or `add_subdirectory('examples/<folder_of_supported_board>/<example_folder>')`
