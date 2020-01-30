---
title: "Configuration"
weight: 2

---

Kiso consists mainly of four major parts - core libraries, BSP layer, third party libraries and application code.
The framework and external libraries need to be configured for a specific application via configuration headers. For ease of use,
we have separated all configuration headers in a single location, allowing for easy swapping between different configurations,
backup and external accessibility. Kiso also provides a mechanism for a project building multiple applications that have configuration
differences between them to be handled easily in the build system.

## Configuration directory structure

In the aforementioned configuration directory, we separate the headers for each library by the library's name. Naming here is
important since the build system is configured to look for them by name, which is not externally configurable. Our default configuration
is located in '\<root Kiso directory\>/config' and contains configuration for all libraries - utils, freertos, cellular, etc.
We will refer to this location as "PROJECT_CONFIG_PATH" - the default configuration for a project.
The configuration headers for a library are always contained within it's own subdirectory. The same structure must be followed when
external configuration directory is used.

- `${PROJECT_CONFIG_PATH}`
    - essentials
        - Kiso_HALConfig.h
    - utils
        - Kiso_UtilsConfig.h
    - freertos
        - FreeRTOSConfig.h
    - cellular
        - Kiso_CellularConfig.h
    - ...
        - ...

## Board-specific configuration

TODO - after [#307](https://github.com/Bosch-AE-SW/cddk-oss/issues/307) there will be changes here and in general section.

## Application configuration

The mechanism to specify additional custom configuration per application is available as a CMake function - `USE_CUSTOM_CONFIG(PATH)`.
This function can be used in an application's CMake file to override any configuration headers from the default project configuration path -
PROJECT_CONFIG_PATH. With this function you can instruct CMake that you have a directory with application-specific headers that differ from the
base project configuration. The function takes only one argument - path to the application configuration directory.

![kisoConfig-error](/images/kisoConfig.png)

It should also follow the same directory structure as the project's configuration directory.
When specific application configuration is used, CMake will create an interim directory that contains all configuration headers from the project
default configuration combined and overriden by the application-specific headers. You will see status messages in the CMake configuration step
and any subsequent changes to these files will automatically trigger the configuration step from CMake to propagate the differences:

```txt
------------- KISO CONFIG -------------
...
Project Config Path:   /MyProject/kiso_config
------------- KISO CONFIG -------------
-- The C compiler identification is GNU 7.3.1
-- Check for working C compiler: /usr/bin/arm-none-eabi-gcc
...
-- Using custom application configuration from /MyProject/application1/app_config
-- Intermediate config path: /MyProject/builddir/application1/config
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
Another option would be to have only application-specific configuration with the default Kiso configuration as the base for your project.
In that case, you can only use the `USE_CUSTOM_CONFIG` CMake function in you application's build definitions.
