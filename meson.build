#Example

# Define project name
project('SensGateApp', 'c')

# Define macros to use cross-builds
device_type = 'stm32'
mcu_family = 'stm32l4'
os_to_use = 'freertos'

# Define paths to the configuration files
config_dir = join_paths('../../refcontexts', device_type, mcu_family, 'config') # Relative path from core
config_dirs = [
	config_dir,
	join_paths(config_dir, 'Essentials'),
	join_paths(config_dir, 'Utils')]

# Build library
subdir('core/essentials')
subdir('core/utils')