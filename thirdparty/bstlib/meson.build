### Build the bstlib as library ###

# Get headers
include_dirs = [
	'./3rd-party/bstlib/BMA2x2_driver', 
	'./3rd-party/bstlib/BME280_driver',
	'./3rd-party/bstlib/BMG160_driver', 
	'./3rd-party/bstlib/BMI160_driver', 
	'./3rd-party/bstlib/BMM050_driver', 
	'./3rd-party/bstlib/BMP180_driver', 
	'./3rd-party/bstlib/BMP280_driver', 
	'./3rd-party/bstlib/BNO055_driver'
	] # dependency to be removed in a later point.

# Build include object
module_dirs = include_directories(include_dirs)

# Get common source files
common_files = [
	'./3rd-party/bstlib/BMA2x2_driver/bma2x2.c', 
	'./3rd-party/bstlib/BME280_driver/bme280.c',
	'./3rd-party/bstlib/BMG160_driver/bmg160.c', 
	'./3rd-party/bstlib/BMI160_driver/bmi160.c', 
	'./3rd-party/bstlib/BMM050_driver/bmm050.c', 
	'./3rd-party/bstlib/BMP180_driver/bmp180.c', 
	'./3rd-party/bstlib/BMP280_driver/bmp280.c', 
	'./3rd-party/bstlib/BNO055_driver/bno055.c'
	]

# Build library
bst_lib = static_library('bst', 
	common_files, 
	include_directories : [config_dirs, module_dirs], 
	install : true)

# Build dependency object
bst_lib_dep = declare_dependency(
	include_directories : [config_dirs, module_dirs], 
	link_with : bst_lib,
	link_whole: bst_lib)
