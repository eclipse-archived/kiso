### Board specific configuration needed for whatever application that want to run on it ###

# Meson macro types: (influence the meson paths)
device_type = 'stm32'
mcu_family = 'stm32l4'
os_to_use = 'freertos'
kiso_type = 'Carbon'
compiler_type = 'GCC'

cellular_vendor = 'ublox'

# Build macro types: (influence the meson build)
add_global_arguments([
	'-DBCDS_PACKAGE_ID=0',
	'-Dstm32',
	'-Dstm32l4',
	'-DSTM32L486xx',
	'-DSTM32L486VG'], language : 'c')

add_project_arguments([
	'-DAPPLICATION_ENTRY_ADDRESS=0x08000000',
	'-DAPPLICATION_SECTION_SIZE=0x80000'], language : 'c')

add_project_link_arguments([
	'-DAPPLICATION_ENTRY_ADDRESS=0x08000000',
	'-DAPPLICATION_SECTION_SIZE=0x80000',
	'-T@0@/Application_STM32L4xx_FLASH.ld'.format(meson.current_source_dir())], language : 'c')

# Get configurations for Kiso modules:
config_dirs = include_directories([
	'kiso_config',
	'kiso_config/Essentials',
	'kiso_config/Utils',
	'kiso_config/Cellular'])


# Build the BSP:
subdir('bsp')