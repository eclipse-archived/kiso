
# Get module headers
include_dirs = [
	'']

# Get other gtest module headers
include_dirs += [
	'']

### Section: Add here includes / dependencies to the vendor libs (to extend!)
third_party_dirs = []

if 'freertos' == os_to_use
freertos_path = '../../../../../thirdparty/freertos/3rd-party/FreeRTOS/Source'
third_party_dirs += [
	join_paths(freertos_path, 'include'),
	join_paths(freertos_path, 'portable/GCC/ARM_CM4F'),
	'../../../../../thirdparty/freertos/test/unit/include']
endif
### End of section

# Build include object
module_dirs = include_directories(include_dirs + third_party_dirs)

# Get common source files
path = join_paths('./source', 'common')
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f]))'.format(path))
common_files = r.stdout().strip().split()

# Get specific MCU source files
path = join_paths('./source', 'ublox')
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f]))'.format(path))
specific_files = r.stdout().strip().split()

# Execute the tests
foreach p : (common_files + specific_files) # To do
#e = executable(p.split('/')[-1], 
#	p, 
#	include_directories : [config_dirs, module_dirs],
#	dependencies : gtest_dep)
#test(p.split('/')[-1], e)
endforeach


