import sys
import os

#put here the path to the source files of the framework
sys.path.append(os.getcwd() + '/../../../../Testing/TestCoordinator/source')
sys.path.append(os.getcwd() + '/../../../../Testing/TestCoordinator/config')
sys.path.append(os.getcwd() + '/../../../../Testing/TestCoordinator/libraries')

JLINK_EXE = os.environ.get('BCDS_DEVELOPMENT_TOOLS') + '/JLinkDebugger/v5.10l/JLink.exe'
DEVICE = 'EFM32GG390F1024'
SPEED = '2000'

#flashing useful variables
testing_binary = os.environ.get('TEST_APP_PATH') + '/release/IntegrationTestApp.bin'
start_address  = '0x00000000'

# Maximum wait time for acknowledgement from target
TIMEOUT = 90

# Variables that can be set by input arguments for python command
COM_PORT = ''
JLINK_SERIAL = ''
TESTSUITE = 'Default'
CONFIG_APN = '4g.tele2.se'
TARGET_PLATFORM = ''

# Configurable parameters for network configuration
CONFIG_USERNAME = ''
CONFIG_PASSWORD = ''
UDP_SEND_DATA = 'Dojo'
COUNTRY_CODE = 0
NETWORK_CODE = 0
LOCATION_AREA_CODE = 0
CELL_ID = 0
UDP_SEND_DATA_LENGTH = '4'