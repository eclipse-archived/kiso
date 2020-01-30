---
title: "Configuration"
weight: 2

---

Kiso consists mainly of four major parts - core libraries, BSP layer, third party libraries and application code.
The framework and external libraries need to be configured for a specific application via configuration headers. For ease of use,
we have separated the configuration headers in three levels, allowing for easy swapping between different configurations,
backup and external accessibility. The top level is project-specific configuration directory. You can override this default location using
the `PROJECT_CONFIG_PATH` CMake variable. After that a board-specific configuration is applied - the is the second level. For the board you are using, the board support package provides
additional configuration overrides that are automatically applied on top of the project default configurations. BSPs provide the `BOARD_CONFIG_PATH` variable -
a directory with the same structure as described bellow. Finally the third level is the application-specific configuration.
Applications have the option to also provide a directory with configuration overrides via the variable `APP_CONFIG_PATH`. Take a look at the c-sensors example to see it in action.

## Configuration directory structure

In the aforementioned configuration directory, we separate the headers for each library by the library's name. Naming here is
important since the build system is configured to look for them by name, which is not externally configurable. Our default configuration
is located in '\<root Kiso directory\>/config' and contains configuration for all libraries - utils, freertos, cellular, etc.
We will refer to this location as "PROJECT_CONFIG_PATH" - the default configuration for a project.
The configuration headers for a library are always contained within it's own subdirectory. The same structure must be followed when
external configuration directory is used.

- `${PROJECT_CONFIG_PATH}`
    - core
        - essentials
            - Kiso_BSPConfig.h
            - Kiso_MCUConfig.h
        - utils
            - Kiso_UtilsConfig.h
        - cellular
            - Kiso_CellularConfig.h
    - thirdparty
        - freertos
            - FreeRTOSConfig.h
        - stm32cubel4
            - ...
    - ...
        - ...

## Merged configuration

During CMake's 'configure' step, all configuration headers are merged in the build directory and provided to the libraries that need them.
The base configuration is taken from `PROJECT_CONFIG_PATH`, overridden by the contents of `BOARD_CONFIG_PATH` - the board-specific headers
and optionally if the application has additional overrides, they are taken from `APP_CONFIG_PATH`.
The merged result is determined by CMake and normally the user shouldn't need to bother with it. Yet it's location is revealed by CMake just in case.

![kisoConfig-error](/images/kisoConfig.png)

It should also follow the same directory structure as the project's configuration directory.
When specific application configuration is used, CMake will create an interim directory that contains all configuration headers from the project
default configuration combined and overriden by the application-specific headers. You will see status messages in the CMake configuration step
and any subsequent changes to these files will automatically trigger the configuration step from CMake to propagate the differences:

```txt
------------- KISO CONFIG -------------
...
Project Config Path:   /opt/my_project/kiso_config
------------- KISO CONFIG -------------
-- The C compiler identification is GNU 7.3.1
-- Check for working C compiler: /usr/bin/arm-none-eabi-gcc
...
-- Applying board-specific configuration from /opt/kiso/boards/CommonGateway/bsp/config.
-- Applying application-specific configuration from /opt/kiso/examples/CommonGateway/c-sensors/config.
-- Configuration headers merged in /opt/kiso/builddir/config.
...
-- Configuring done
-- Generating done
```

## Usage for projects

When configuring Kiso's CMake, users can provide the `PROJECT_CONFIG_PATH` variable externally with `cmake <...> -DPROJECT_CONFIG_PATH:PATH="..."`
This variable's value should be a fully-qualified path to a directory with the same structure as described above. The easiest way to use
such external configuration would be to copy Kiso's default configuration directory, make any changes you need, remove the unused headers and
eventually add your own.

If you don't need specific application configuration, this is all you'll have to do to start with Kiso. Moreover, you can use the default
configuration that Kiso uses and ommit the variable entirely.
