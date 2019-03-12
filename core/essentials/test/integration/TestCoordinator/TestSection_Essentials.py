import Configuration
from AbstractTestSection import *
from CChannel.CCUsb import *
from CChannel.CCUart import *
from TestSuite_I2C.TestSuite_I2C import *
from TestSuite_SPI.TestSuite_SPI import *
from TestSuite_UART.TestSuite_UART import *
from TestSuite_Timer.TestSuite_Timer import *
from TestSuite_Sleep.TestSuite_Sleep import *

class TestSection_Essentials(AbstractTestSection):

    #Constructor 
    def __init__(self, testExecutor, variant):
        super(TestSection_Essentials, self).__init__("TestSection_Essentials", testExecutor)
        self.sectionId = 1

        if variant == "stm32":
            self.addTestParticipantProxy(Configuration.EMBEDDED_TEST_PARTICIPANT,
                                         CCUart(Configuration.COM_PORT, 115200))

        else:
            self.addTestParticipantProxy(Configuration.EMBEDDED_TEST_PARTICIPANT, CCUsb(Configuration.COM_PORT))

            self.add_test_suite(TestSuite_I2C, 1)
            # Now SPI isr are defined in Efm32/BSP code itself. Hence this test cases are invalid
            # self.add_test_suite(TestSuite_SPI, 2)
        
            # UART is being disabled because of a hang-up issue.
            # self.add_test_suite(TestSuite_UART, 3)

            self.add_test_suite(TestSuite_Timer, 4)

            # SLEEP test coordinator needs a better design
            #self.add_test_suite(TestSuite_Sleep, 5)
            
    def tearDown(self):
        print("Section Tear down")
        super(TestSection_Essentials, self).tearDown()
