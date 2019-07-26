### Build the bstlib as library ###

# Get headers
include_dirs = [
	'./3rd-party/mbedtls/include' ] # dependency to be removed in a later point.

# Build include object
module_dirs = include_directories(include_dirs)

# Get common source files
path = './3rd-party/mbedtls/source'
r = run_command('python', '-c', 'from os import listdir; print(" ".join(["@0@/" + f for f in listdir("@0@") if ".c" in f or ".s" in f]))'.format(path))
common_files = r.stdout().strip().split()


# Build library
mbedtls_lib = static_library('mbedtls', 
	common_files, 
	include_directories : [config_dirs, module_dirs], 
	install : true)

# Build dependency object
mbedtls_lib_dep = declare_dependency(
	include_directories : [config_dirs, module_dirs], 
	link_with : mbedtls_lib,
	link_whole: mbedtls_lib)
