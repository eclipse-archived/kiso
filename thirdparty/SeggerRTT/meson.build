### Build the SeggerRTT as library ###

# Get headers
include_dirs = [
	'./3rd-party/SEGGER_RTT/RTT',
	'../freertos/3rd-party/FreeRTOS/Source/include',
	join_paths('../freertos/3rd-party/FreeRTOS/Source/portable', compiler_type, 'ARM_CM4F'),
	'../freertos/3rd-party/FreeRTOS/Source/portable/MemMang',
	'../../core/essentials/include'] # dependency to be removed in a later point.

# Build include object
module_dirs = include_directories(include_dirs)

# Get common source files
common_files = [
	'./3rd-party/SEGGER_RTT/RTT/SEGGER_RTT.c',
	'./3rd-party/SEGGER_RTT/RTT/SEGGER_RTT_printf.c']

# Build library
seggerrtt_lib = static_library('SeggerRTT', 
	common_files, 
	include_directories : [config_dirs, module_dirs], 
	install : true)

# Build dependency object
seggerrtt_lib_dep = declare_dependency(
	include_directories : [config_dirs, module_dirs], 
	link_with : seggerrtt_lib,
	link_whole: seggerrtt_lib)