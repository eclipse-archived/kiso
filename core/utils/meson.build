
# Get headers
include_dirs = [
	'include',
	'../essentials/include',
	'../essentials/include/mcu']

### Add here includes / dependencies to the vendor libs (to extend!)
third_party_dirs = []

if 'freertos' == os_to_use
freertos_path = '../../thirdparty/freertos/3rd-party/FreeRTOS/Source'
third_party_dirs += [
	join_paths(freertos_path, 'include'),
	join_paths(freertos_path, 'portable/GCC/ARM_CM4F')]
endif

if 'stm32' == device_type
stn32cube_path = '../../thirdparty/stm32cubel4'
stn32cubedriver_path = '../../thirdparty/stm32cubel4/3rd-party/stm32cube/Drivers'
third_party_dirs += [
	join_paths(stn32cube_path, 'include'),
	join_paths(stn32cubedriver_path, 'CMSIS/Include'),
	join_paths(stn32cubedriver_path, 'CMSIS/Device/ST', mcu_family.to_upper() + 'xx/Include'),
	join_paths(stn32cubedriver_path, mcu_family.to_upper() + 'xx_HAL_Driver/Inc')]
endif

### End of section

# Get common source files
path = './source'
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f]))'.format(path))
common_files = r.stdout().strip().split()

# Get specific MCU source files
path = join_paths('./source/Carbon')
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f]))'.format(path))
specific_files = r.stdout().strip().split()

# Build lib
utils_lib = static_library('Utils', 
  common_files + specific_files, 
  include_directories : include_directories(config_dirs + include_dirs + third_party_dirs), 
  install : true)

utils_lib_dep = declare_dependency(include_directories : include_directories(config_dirs + include_dirs + third_party_dirs), 
  link_with : utils_lib)