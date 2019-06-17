---
title: "Advance guide"
description: "How to load features specific to your board"
weight: 2
draft: false
toc: true
menu:
  main:
    parent: 3. User guide
    identifier: advance_guide
    weight: 2
---

# Advance guide

Usually, you would like to:

- utilize the features of your board. Such as I2C, SPI,...
- add an RTOS
- add Kiso components and modules

For simplicity, let us use our code with an stm32f4 nucleo board: NUCLEO-L476RG

## Utilize your board ##
It be able to use any Kiso component, you will need to add the basic components: `Essentials` and `Utils`.
To do so, you need to modify the **meson.build** with:
```bash
# Define project name
project('Hello World', 'c', default_options: meson.get_cross_property('project_config'))

# Get board & configuration
subdir('boards/Nucleo-L476RG')

# Missing part

# Get kiso libraries
subdir('core/essentials')
subdir('core/utils')

# Get application
subdir('examples/HelloWorld/hello-world')
```

**This will not fully work**, because you need now the dependencies to **3rd party libraries**. Let us modify a bit more:
```bash
# Define project name
project('Hello World', 'c', default_options: meson.get_cross_property('project_config'))

# Get board & configuration
subdir('boards/Nucleo-L476RG')

# Get 3rd party libraries
subdir('thirdparty/freertos')
subdir('thirdparty/stm32cubel4')

# Get kiso libraries
subdir('core/essentials')
subdir('core/utils')

# Get application
subdir('examples/HelloWorld/hello-world')
```

Let us load a slightly more complex test: led-blinking:
```bash
# Define project name
project('Hello World', 'c', default_options: meson.get_cross_property('project_config'))

# Get board & configuration
subdir('boards/Nucleo-L476RG')

# Get 3rd party libraries
subdir('thirdparty/freertos')
subdir('thirdparty/stm32cubel4')

# Get kiso libraries
subdir('core/essentials')
subdir('core/utils')

# Get application
subdir('examples/HelloWorld/c-leds')
```
## Run application on your board ##
**Build application**:
```bash
meson builddir --cross-file boards/Nucleo-L476RG/meson_config_stm32l4_gcc8.ini
cd builddir
ninja hex
```
**Flash application**:

It dependence on what you are using. Usually it would be for our case an ST-link or a SEGGER J-link.
The code needs to be flashed at the address: 0x<tbd>

## Small summary ##
In the meson.build, you always need:

- the project name
- to load the board specific information via `subdir('boards/<folder_of_supported_board>')`
- to load the 3rd party needed libraries via `subdir('thirdparty/<3rd_party_lib_name>')`
- to load the Kiso components via `subdir('core/<kiso_component_name>')`
- to load the example you want to use via `subdir('examples/HelloWorld/<example_folder>')` or `subdir('examples/<folder_of_supported_board>/<example_folder>')`

