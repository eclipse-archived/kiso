
# Get module headers
include_dirs = [
	'./include',
	'./include/mcu',
	'./include/bsp',
	join_paths('./source', device_type, 'protected'),
	join_paths('./source', device_type, mcu_family, 'protected'),
	'../../include',
	'../../include/mcu',
	join_paths('../../include/mcu/', device_type),
	join_paths('../../include/mcu/', device_type, mcu_family),
	'../../include/bsp',
	'../../source',
	'../../source/mcu',
	join_paths('../../source/mcu/', device_type, mcu_family)]

### Section: Add here includes / dependencies to the vendor libs (to extend!)
third_party_dirs = []

if 'stm32l4' == mcu_family
stm32cube_path = '../../../../thirdparty/stm32cubel4'
stm32cubedriver_path = '../../../../thirdparty/stm32cubel4/3rd-party/stm32cube/Drivers'
third_party_dirs += [
	join_paths(stm32cube_path, 'include'),
	join_paths(stm32cubedriver_path, 'CMSIS/Include'),
	join_paths(stm32cubedriver_path, 'CMSIS/Device/ST', mcu_family.to_upper() + 'xx/Include'),
	join_paths(stm32cubedriver_path, mcu_family.to_upper() + 'xx_HAL_Driver/Inc'),
	join_paths(stm32cube_path, 'test/unit/include')]
endif
### End of section

# Build include object
module_dirs = include_directories(include_dirs + third_party_dirs)

# Get common source files
path = './source'
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f]))'.format(path))
common_files = r.stdout().strip().split()

# Get specific MCU source files
path = join_paths('./source', device_type, mcu_family)
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f]))'.format(path))
specific_files = r.stdout().strip().split()

# Execute the tests
foreach p : (common_files + specific_files)
e = executable(p.split('/')[-1], 
	p, 
	include_directories : [config_dirs, module_dirs],
	dependencies : gtest_dep)
test(p.split('/')[-1], e)
endforeach
