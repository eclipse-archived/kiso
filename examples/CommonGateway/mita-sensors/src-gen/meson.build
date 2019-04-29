# Define project name
project('application', 'c')

# Get board
subdir('kiso/boards/CommonGateway')

# Get 3rd party libraries
subdir('kiso/thirdparty/freertos')
subdir('kiso/thirdparty/stm32cubel4')
subdir('kiso/thirdparty/bstlib')

# Get kiso libraries
subdir('kiso/core/essentials')
subdir('kiso/core/utils')
#subdir('kiso/core/connectivity/cellular')


# Get sources
application_files = ['main.c', 'base/MitaTime.c', 'base/ConnectivityLEDLeds.c', 'base/SensorAccelerometer.c', 'base/SensorEnvironment.c', 'base/PlatformCGW.c', 'application.c']

# Define application build and its dependencies
exe = executable('Application.out', 
  application_files,
  dependencies : [bsp_lib_dep, freertos_lib_dep, stm32cubel4_lib_dep, essentials_lib_dep, utils_lib_dep, bst_lib_dep],
  include_directories: ['.', 'base'],
  install : true)

run_target(
	'hex', 
	command: [
		meson.get_cross_property('objcopy'),
		meson.get_cross_property('objcopy_args'),
		exe.full_path(), exe.full_path() + '.bin'], 
	depends: exe)
