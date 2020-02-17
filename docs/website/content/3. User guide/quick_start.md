---
title: "Quick start"
weight: 1
---

## First steps with Windows 10 ##
If you're using a Windows PC it is recommended to use the Microsoft Windows subsystem for Linux. Alternatively, you can also use, e.g., Cygwin to provide functionality similar to a Linux distribution on Windows. All the below listed commands are of course also valid if you are directly using a Linux PC. \

### Step 1: Windows Subsystem Linux (WSL) ###
* Installation guide: https://docs.microsoft.com/en-us/windows/wsl/install-win10
* Install Ubuntu 18.04 (bionic) for Windows via Windows Store. Alternatively, upgrade Ubuntu 16.04 to 18.04: https://help.ubuntu.com/lts/serverguide/installing-upgrading.html

### Step 2: Optional proxy ###
If you're running your Windows computer behind a company proxy install a local proxy server that allows applications to authenticate through an NTLM or Windows Kerberos authenticated proxy server. \

* Installation guide: https://pypi.org/project/px-proxy/ \
  After the installation your local proxy should be accessible at `127.0.0.1:3128`. Please also use the optional proxy settings from next step. \
  **Note**: Consider to add px to your Windows autostart.

### Step 3: Setup WSL Ubuntu ###
Start the Windows app Ubuntu and use the following bash commands to install the basic tooling.\

* Optional for proxy: proxy setting for the command line \
  This environmental variables are valid for all command line tools that don't offer a dedicated proxy setting option. \
  **Note**: The environmental variables here are set in your login file `~/.profile`. \
  ```bash
  printf "export http_proxy=\"http://127.0.0.1:3128\"\nexport https_proxy=\"http://127.0.0.1:3128\"\n" >> ~/.profile && source ~/.profile`
    ```

* Optional for proxy: proxy setting for apt \
  Required if you need to download and install the basic tooling via apt behind a proxy. \
  **Note**: This setting here is added to your global apt config.
  ```bash
  printf "Acquire::http::proxy \"http://127.0.0.1:3128/\";\nAcquire::https::proxy \"http://127.0.0.1:3128/\";\n" \| sudo tee /etc/apt/apt.conf.d/80proxy`
  ```

* Optional for proxy: proxy setting for git \
  Required if you want to clone and work with Kiso's source code repository behind a proxy instead of downloading a zip file of the repo.
  **Note**: This setting here is added to your global apt config `~/.gitconfig`.
  ```bash
  git config --global http.proxy http://:@localhost:3128
  ```

* Install basic tools \
  Install the minimum basic tooling to compile Kiso. \
  ```bash
  sudo apt install git
  sudo apt install cmake
  sudo apt install gcc-arm-none-eabi
  ```

* Optional for proxy: GitHub token instead of passwort \
  This is just a convenience option if you don't want to insert you GitHub credentials every time you interact with Kiso's GitHub repo. \
  Installation guide: https://help.github.com/en/github/authenticating-to-github/creating-a-personal-access-token-for-the-command-line

### Step 4: Clone and compile ###
After installing the basic tools from previous step, you can now clone and compile Kiso's default example. \

* Clone Kiso repo \
  ```bash
  git clone https://github.com/eclipse/kiso.git
  ```

* First default compile \
  ```bash
  cd kiso
  cmake . -Bbuild
  cmake --build build
  ```

## Tooling ##
### Basic ###
* Download and install git
* Download and install CMake
* Download and install arm-none-eabi

### Optional additional tooling ###
  * Download and install Hugo
  * Download and install Docker
  * Download and install Anaconda version [Anaconda3-2019.03](https://repo.continuum.io/archive/)
  * Download and install Doxygen version [v1.8.8](https://sourceforge.net/projects/doxygen/files/rel-1.8.8/)
  * Download and use [HTerm](https://www.heise.de/download/product/hterm-53283/download) or another uart-console.
  * In case the uart-driver does not work, please download [here](https://www.ftdichip.com/Drivers/VCP.htm) the right driver for your machine

**Note**: python, arm-none-eabi, doxygen need to be accessible via cmd-line. Therefore, you may need to modify the environment variable *PATH* on windows if it is not done in the installation.

## Build HelloWorld with your supported hardware (CMake build) ##

Make sure to set `KISO_APPLICATION_PATH` variable to the directory containing your application.

**Build**:
```bash
# You can execute these steps from anywhere
cmake <kiso_root> -Bbuilddir -DKISO_BOARD_PATH=<selected_board>
cmake --build builddir
# OR equivalent:
mkdir builddir && cd builddir
cmake <kiso_root> -DKISO_BOARD_PATH=<selected_board>
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

- `KISO_BOARD_PATH` is a CMake variable that instructs where the directory containing board options and BSP code is located. Could be one of the directories in `<kiso root>/boards` or external absolute path.

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
cmake <kiso_root> -B build_tests -DKISO_BOARD_PATH=boards/CommonGateway -DENABLE_TESTING=1 # Generate for testing
cmake --build build_tests # Build unit tests
cd build_tests && ctest # Execute tests
```
