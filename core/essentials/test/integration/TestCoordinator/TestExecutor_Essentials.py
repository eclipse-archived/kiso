import time
import Configuration
from AbstractTestExecutor import *
from TestSection_Essentials import *
from Flashing.JLINK_flashingTool import *
import getopt
import sys


class TestExecutor_Essentials(AbstractTestExecutor):
    def __init__(self, variant):
        super(TestExecutor_Essentials, self).__init__()
        self.addTestSection(TestSection_Essentials(self, variant))


if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hc:v:")
    except getopt.GetoptError:
        print(
            __file__ + " -c <comport> -v <variant>")
        sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print(
                __file__ + " -c <comport> -v <variant>")
            sys.exit()
        elif opt == "-c":
            Configuration.COM_PORT = arg
        elif opt == "-v":
            variant = arg
        else:
            print(
                __file__ + " -c <comport> -v <variant>")
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

    testExecutor = TestExecutor_Essentials(variant)
    testExecutor.execute()
