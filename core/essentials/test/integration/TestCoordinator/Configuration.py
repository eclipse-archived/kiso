import sys
import os

# put here the path to the source files of the framework
sys.path.append(os.getcwd() + '/../../../../Testing/TestCoordinator/source')
sys.path.append(os.getcwd() + '/../../../../Testing/TestCoordinator/config')

# from Config import *

# flashing configuration
JLINK_EXE = os.environ.get('BCDS_DEVELOPMENT_TOOLS') + '/JLinkDebugger/v5.10l/JLink.exe'
DEVICE = 'EFM32GG390F1024'
SPEED = '12000'

# flashing useful variables
testing_binary = os.environ.get('TEST_APP_PATH') + '/debug/IntegrationTestApp_debug.bin'
start_address_stm32 = '0x08000000'
JLink_serial_number_stm32 = os.environ.get('BSE_JLINK_DEVICE')

start_address_efm32 = '0x00000000'
JLink_serial_number_efm32 = os.environ.get('XDK_JLINK_DEVICE')

# Test participant names
JAVA_TEST_PARTICIPANT = "JavaTP"
BEAGLEBONE_TEST_PARTICIPANT = "Beaglebone"
EMBEDDED_TEST_PARTICIPANT = "Testling"

# Coordination Channel configuration
COM_PORT = "COM1"
JIP = "127.0.0.1"
JPORT = 1111
BIP = "192.168.7.2"
BPORT = 2222

TIMEOUT = 20

LOGGER_NAME = "testing_logger"

TLV_TYPE_TEST_DURATION = 23
