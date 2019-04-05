### Build the Cellular library ###

# Get headers
include_dirs = [
	'include',
	'source/protected',
	'../../essentials/include',
	'../../essentials/include/mcu',
	'../../essentials/include/bsp',
	'../../utils/include']

### Section: Add here includes / dependencies to the vendor libs (to extend!)
third_party_dirs = []

if 'freertos' == os_to_use
freertos_path = '../../../thirdparty/freertos/3rd-party/FreeRTOS/Source'
third_party_dirs += [
	join_paths(freertos_path, 'include'),
	join_paths(freertos_path, 'portable/GCC/ARM_CM4F')]
endif
### End of section

# Build include object
module_dirs = include_directories(include_dirs + third_party_dirs)

# Get common source files
path = './source/common'
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f]))'.format(path))
common_files = r.stdout().strip().split()

# Get source files for the specific vendor chip used
path = join_paths('./source', cellular_vendor)
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f]))'.format(path))
specific_files = r.stdout().strip().split()

# Build library
cellular_lib = static_library('Cellular',
	common_files + specific_files,
	include_directories : [config_dirs, module_dirs],
	install : true)

# Build library dependency object
cellular_lib_dep = declare_dependency(
	include_directories : [config_dirs, module_dirs],
	link_with : cellular_lib)