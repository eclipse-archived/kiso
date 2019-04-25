#Example Application built

# Define project name
project('SensGate', 'c', default_options: meson.get_cross_property('project_config'))

# Get board & configuration
subdir('boards/SensGate')

# Get 3rd party libraries
subdir('thirdparty/freertos')
subdir('thirdparty/stm32cubel4')
subdir('thirdparty/SeggerRTT')

# Get kiso libraries
subdir('core/essentials')
subdir('core/utils')
#subdir('core/connectivity/cellular')

# Get application
subdir('examples/SensGate/application1')