import sys
import threading
from time import sleep
from UdpTest.TestCases_Udp.UDPCommunicator import UDPCommunicator
        
class UdpDataValidation:

        def __init__(self):
            self.signalThreadToClose=0
            self.PacketCounter=0
            self.udpcommunicator = UDPCommunicator()
            self.UdpThread = threading.Thread(target = self._threadFunction)
            self.receiveEvent = threading.Event()
            self.udpdata = []
            self.destinationIp=""
            self.destinationPort=0

            
        def open(self, host, port):
            print('Start udp communication')
            if(True == self.udpcommunicator.open(host, port)):
                    self.UdpThread.start()
                    return True
            else:
                    print("Open failed")
                    return False

        def close(self):
            print('Close udp communication')
            self.udpcommunicator.close()
            self.signalThreadToClose = 1
            return              
                
        def _threadFunction(self):
            while (1):
                if(0 == self.signalThreadToClose):
                    self.udpcommunicator.setReceiveCallback(self._receiveCallback)
                else:
                    print("returning in threadFunction")
                    return

        def _receiveCallback(self,data,destip,despport):
            self.PacketCounter += 1
            self.destinationIp = destip
            self.destinationPort = despport
            print ('ReceiveCallback1 : Received[' + destip + ':' + str(despport) + '] - ' + str(self.PacketCounter) + ' ' + data.strip())
            self.udpdata = data
##            print("udp data " + self.udpdata)
            self.receiveEvent.set()
            self.udpcommunicator.senddata(destip,despport,data)

        def validateReceivedData(self,dataToValidate):
            if(dataToValidate == self.udpdata):
                print("data validation pass")
                return 1
            else:
                print("data validation fail")
                return 0

        def isDataReceived(self):
            if(True == self.receiveEvent.is_set()):
                self.receiveEvent.clear()
                return True
            else:
                return False

        def sendUdpData(self,data,count):
            #Todo - Add logic to check for valid IP and Port
            if(self.destinationPort <= 0):
                    print("No valid IP and Port is known to send data")
            else:
                     for i in range(0, count):
                             print("in loop: " + str(i) + ' ' + data)
                             print("Dest IP " + str(self.destinationIp) + "Dest Port " + str(self.destinationPort))
                             sleep(0.5)
                             self.udpcommunicator.senddata(self.destinationIp,self.destinationPort,data)

        def waitAndSendUdpData(self,waitTimeInSec,data):
            #Todo - Add logic to check for valid IP and Port
            if(self.destinationPort <= 0):
                    print("No valid IP and Port is known to send data in waitAndSendUdpData")
            else:
                    print("waiting for " + str(waitTimeInSec) + " seconds before sending udp data")
                    sleep(waitTimeInSec)
                    self.udpcommunicator.senddata(self.destinationIp,self.destinationPort,data)
                
