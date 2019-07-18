#Example Application built

# Define project name
project('LibForChosenBoard', 'c', 'cpp', default_options: meson.get_cross_property('project_config'))

# Get board configuration
subdir(meson.get_cross_property('default_board_config'))


# Build binaries
if meson.get_cross_property('build_type') == 'binary'

# Get board bsp
subdir(meson.get_cross_property('default_board_config') + '/bsp')

# Get 3rd party libraries
foreach p : meson.get_cross_property('default_third_party_lib')
	subdir(p)
endforeach # Add on the next line other lib you would like to link

# Get kiso libraries
foreach p : meson.get_cross_property('default_core_lib')
	subdir(p)
endforeach # Add on the next line other lib you would like to link

# Get application
subdir(meson.get_cross_property('default_application')) # Modify it with another app you want to use


# Build unittest
elif meson.get_cross_property('build_type') == 'unittest'

# Build gtest library
subdir('thirdparty/Gtest/v2.0.0-0.2.0')
# Run tests
foreach p : meson.get_cross_property('default_core_lib')
	subdir(p + '/test/unit')
endforeach

endif


# Generate doxygen (always working)
subdir('docs/doxygen')