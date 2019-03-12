import Configuration
from AbstractTestSection import *
from CChannel.CCUsb import *
from CChannel.CCUart import *
from CChannel.Message import *
from TestSuite_ErrorLogger.TestSuite_ErrorLogger import *


class TestSection_Errorlogger(AbstractTestSection):
    """ Defined test section.
    """

    def __init__(self, testExecutor, variant):
        super(TestSection_Errorlogger, self).__init__("TestSection_Errorlogger", testExecutor)
        self.sectionId = 1

        if variant == "stm32":
            self.addTestParticipantProxy(Configuration.EMBEDDED_TEST_PARTICIPANT,
                                         CCUart(Configuration.COM_PORT, 115200))

        else:
            self.addTestParticipantProxy(Configuration.EMBEDDED_TEST_PARTICIPANT, CCUsb(Configuration.COM_PORT))

            self.add_test_suite(TestSuite_ErrorLogger, 1)

    def tearDown(self):
        """ Call the tearDown from the mother-class.

        .. note::
            In python, this is not necessary because python go get
            automaticaly the tearDown of the mother-class if it was not
            defined in this class.
        """
        print("Section Tear down")
        super(TestSection_Errorlogger, self).tearDown()
        self.stopAllTestParticipants()
