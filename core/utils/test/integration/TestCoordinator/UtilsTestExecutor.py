import time
from Configuration import *
import os
import re
import importlib
import random
from AbstractTestExecutor import *
# Import the module for flashing code into the node
from Flashing.JLINK_flashingTool import *
# Import system and system handle modules
import getopt
import sys
import subprocess
from TestSection_Errorlogger import *

class UtilsTestExecutor(AbstractTestExecutor):
    def __init__(self, variant):
        super(UtilsTestExecutor, self).__init__()
        self.addTestSection(TestSection_Errorlogger(self, variant))

if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hc:v:")
    except getopt.GetoptError:
        print(
            __file__ + " -c <comport> -v <self.variant>")
        sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print(
                __file__ + " -c <comport> -v <self.variant>")
            sys.exit()
        elif opt == "-c":
            Configuration.COM_PORT = arg
        elif opt == "-v":
             variant = arg
        else:
            print(
                __file__ + " -c <comport> -v <self.variant>")
            sys.exit(2)

    if variant == "stm32":
        start_address = start_address_stm32
        JLink_serial_number = JLink_serial_number_stm32
    else:
        Configuration.DEVICE = 'EFM32GG390F1024'
        Configuration.SPEED = '12000'
        start_address = start_address_efm32
        JLink_serial_number = JLink_serial_number_efm32

    flashing_tool = JLINK_flashingTool()
    result = flashing_tool.flash_binary(testing_binary, start_address, JLink_serial_number)

    if result != 0:
        print("Flashing failed with return code: " + str(result))
        sys.exit(2)

    # wait for all TP to get started
    time.sleep(10)
    testExecutor = UtilsTestExecutor(variant)
    testExecutor.execute()

