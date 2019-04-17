### Build the application with its dependencies ###

# Get sources
application_files = [
    'source/main.c',
    'source/blinky_led.c']

# Define application build and its dependencies
exe = executable('Application', 
	application_files,
	dependencies : [bsp_lib_dep, freertos_lib_dep, stm32cubel4_lib_dep, seggerrtt_lib_dep, essentials_lib_dep, utils_lib_dep],
	install : true)

# Build the hex file
run_target(
	'hex',
	command: [
		meson.get_cross_property('objcopy'),
		meson.get_cross_property('objcopy_args'),
		exe.full_path(), exe.full_path() + '.hex'],
	depends: exe)