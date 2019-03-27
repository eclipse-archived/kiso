
# Get headers
include_dirs = [
	'include',
	'include/mcu',
	join_paths('include/mcu', device_type),
	join_paths('include/mcu', device_type, mcu_family),
	'include/bsp']

### Add here includes / dependencies to the vendor libs (to extend!)

if 'stm32l4' == mcu_family
stn32cube_path = '../../thirdparty/stm32cubel4'
stn32cubedriver_path = '../../thirdparty/stm32cubel4/3rd-party/stm32cube/Drivers'
third_party_dirs = [
	join_paths(stn32cube_path, 'include'),
	join_paths(stn32cubedriver_path, 'CMSIS/Include'),
	join_paths(stn32cubedriver_path, 'CMSIS/Device/ST', mcu_family.to_upper() + 'xx/Include'),
	join_paths(stn32cubedriver_path, mcu_family.to_upper() + 'xx_HAL_Driver/Inc'),
	join_paths(stn32cubedriver_path, mcu_family.to_upper() + 'xx_HAL_Driver/Inc/Legacy')]
endif

### End of section

# Get common source files
path = './source'
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f]))'.format(path))
common_files = r.stdout().strip().split()
message(r.stdout().strip())

# Get specific MCU source files
path = join_paths('./source/mcu', device_type, mcu_family)
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f]))'.format(path))
specific_files = r.stdout().strip().split()

message(' '.join(config_dirs + include_dirs + third_party_dirs))
# Build lib
essentials_lib = static_library('Essential', 
  common_files + specific_files, 
  include_directories : include_directories(config_dirs + include_dirs + third_party_dirs), 
  install : true)

essentials_lib_dep = declare_dependency(include_directories : include_directories(config_dirs + include_dirs + third_party_dirs), 
  link_with : essentials_lib)