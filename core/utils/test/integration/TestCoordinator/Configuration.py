import sys
import os

# put here the path to the source files of the framework
sys.path.append(os.getcwd() + '/../../../../../bcds_shared/Testing/TestCoordinator/source')
sys.path.append(os.getcwd() + '/../../../../../bcds_shared/Testing/TestCoordinator/config')
sys.path.append(os.getcwd() + '/../../../../../bcds_shared/Testing/TestCoordinator/libraries')
sys.path.append(os.getcwd())

sys.path.append(os.getcwd() + '//source//Tests')

# from Config import *

# flashing configuration
JLINK_EXE = os.environ.get('BCDS_DEVELOPMENT_TOOLS') +('/JLinkDebugger/V5.10l/JLink.exe')
DEVICE = 'EFM32GG390F1024'
SPEED = '12000'

# flashing useful variables
testing_binary = os.environ.get('TEST_APP_PATH') + ('/release/IntegrationTestApp.bin')

start_address_stm32 = '0x08000000'
JLink_serial_number_stm32 = os.environ.get('BSE_JLINK_DEVICE')

start_address_efm32 = '0x00000000'
JLink_serial_number_efm32 = os.environ.get('XDK_JLINK_DEVICE')

# Test participant names
EMBEDDED_TEST_PARTICIPANT = "Testling"

# Coordination Channel configuration
COM_PORT = "COM1"
JIP = "127.0.0.1"
JPORT = 1111
BIP = "192.168.7.2"
BPORT = 2222

TIMEOUT = 10

LOGGER_NAME = "testing_logger"




