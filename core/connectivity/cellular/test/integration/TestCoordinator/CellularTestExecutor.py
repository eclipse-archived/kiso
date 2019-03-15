import random
import time
import Configuration
from AbstractTestExecutor import *
from CellularTestSection import *
from Flashing.JLINK_flashingTool import *
import getopt
import sys

class CellularTestExecutor(AbstractTestExecutor):

    def __init__(self, indexInSequence):
        super(CellularTestExecutor, self).__init__()
        self.addTestSection(TestSection_Cellular(self))

        self.mySequence = [x for s in [10, 100, 1000] for x in range(0, 10*s, s) if x != 0]
        self.indexInSequence = indexInSequence % len(self.mySequence)

    def getNextTestDuration(self):
        duration = self.mySequence[self.indexInSequence]
        self.indexInSequence = (self.indexInSequence + 1) % len(self.mySequence)
        return duration

    def getNext3TestDurations(self):
        duration1 = self.mySequence[self.indexInSequence]
        self.indexInSequence = (self.indexInSequence + 1) % len(self.mySequence)
        duration2 = self.mySequence[self.indexInSequence]
        self.indexInSequence = (self.indexInSequence + 1) % len(self.mySequence)
        duration3 = self.mySequence[self.indexInSequence]
        self.indexInSequence = (self.indexInSequence + 1) % len(self.mySequence)

        durations = [duration1, duration2, duration3]
        random.shuffle(durations)
        return durations

    def getRandomTestDuration(self):
        index = random.randint(0, len(self.mySequence) - 1)
        return self.mySequence[index]

def XDK_run(opts):
    numberOfIteration = 1

    for opt, arg in opts:
        if opt == "-n":
            numberOfIteration = int(arg)
        elif opt == "-c":
            Configuration.COM_PORT = arg
        elif opt == "-j":
            Configuration.JLINK_SERIAL = arg
        elif opt == '-p':
            Configuration.TARGET_PLATFORM = arg
        elif opt == "-a":
            Configuration.CONFIG_APN = arg
        elif opt == "-t":
            Configuration.TESTSUITE = arg
        else:
            print(
                __file__ + " accepts these arguments: -n <numberOfIteration> -c <comport> -j <JLink serial number> -p <target platform> -a <APN> -t <test suite>")
            sys.exit(2)

    flashing_tool = JLINK_flashingTool()
    result = flashing_tool.flash_binary(Configuration.testing_binary, Configuration.start_address,
                                        Configuration.JLINK_SERIAL)
    if result != 0:
        print("Flashing failed with return code: " + str(result))
        sys.exit(2)
    time.sleep(10)

    for i in range(numberOfIteration):
        print("iteration number " + str(i))
        testExecutor = CellularTestExecutor(i)
        testExecutor.execute()
        time.sleep(2)

if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hn:c:j:p:a:t:")
    except getopt.GetoptError:
        print(__file__ + " accepts these arguments: -n <numberOfIteration> -c <comport> -j <JLink serial number> -p <target platform> -a <APN> -t <test suite>")
        print("Test suite: (UDP_TEST, SMOKE_TEST, LOAD_STRESS_TEST)")
        sys.exit(2)

    XDK_run(opts)
