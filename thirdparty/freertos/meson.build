### Build the FreeRTOS as library ###

# Get headers
include_dirs = [
	'./3rd-party/FreeRTOS/Source/include',
	join_paths('./3rd-party/FreeRTOS/Source/portable', compiler_type, 'ARM_CM4F'),
	'./3rd-party/FreeRTOS/Source/portable/MemMang',
	'../../core/essentials/include'] # dependency to be removed in a later point.

# Build include object
module_dirs = include_directories(include_dirs)

# Get common source files
common_files = [
	'./source/FreeRTOSConfigCompatibilityCheck.c',
	'./3rd-party/FreeRTOS/Source/event_groups.c',
	'./3rd-party/FreeRTOS/Source/list.c',
	'./3rd-party/FreeRTOS/Source/queue.c',
	'./3rd-party/FreeRTOS/Source/stream_buffer.c',
	'./3rd-party/FreeRTOS/Source/tasks.c',
	'./3rd-party/FreeRTOS/Source/timers.c',
	'./3rd-party/FreeRTOS/Source/portable/MemMang/heap_4.c',
	join_paths('./3rd-party/FreeRTOS/Source/portable', compiler_type, 'ARM_CM4F/port.c')]

# Build library
freertos_lib = static_library('FreeRTOS', 
	common_files, 
	include_directories : [config_dirs, module_dirs], 
	install : true)

# Build dependency object
freertos_lib_dep = declare_dependency(
	include_directories : [config_dirs, module_dirs], 
	link_with : freertos_lib)