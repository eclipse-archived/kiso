# cddk-oss

# Meson installation:
* pip3 install meson (https://mesonbuild.com/Getting-meson.html)
* Also install ninja somewhere

# Meson build Sensgate:
* meson debug --cross-file boards/SensGate/meson_config_stm32l4_gcc8.ini
* cd debug
* ninja
