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
* Download and use [HTerm](https://www.heise.de/download/product/hterm-53283/download) or another uart-console.
* In case the uart-driver does not work, please download [here](https://www.ftdichip.com/Drivers/VCP.htm) the right driver for your machine

**Note**: python, arm-none-eabi, doxygen need to be accessible via cmd-line. Therefore, you may need to modify the environment variable *PATH* on windows if it is not done in the installation.

### Extension tooling ###
* ```pip3 install meson```
* ```pip3 install ninja```

## Build HelloWorld with your supported hardware ##

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

## Small summary ##
In the meson.build, you always need:

- the project name
- to load the board specific information via `subdir('boards/<folder_of_supported_board>')`
- to load the example you want to use via `subdir('examples/HelloWorld/<example_folder>')`

To utilize your board with Kiso, please take a look at [the next step]({{< ref "advance_guide.md" >}})
