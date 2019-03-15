import sys
import socket
import threading
from time import sleep

class UDPCommunicator:
        
        def __init__(self):
                # create Datagram (udp) socket
                self.udpsocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                self.udpsocket.settimeout(0.01)
                print('Socket created')
                self.recvDataEvent = threading.Event()
                self.RecvThread = threading.Thread(target=self.receivedata);
            
        def open(self, host, port):
                ''' Open the port.'''
                print('Socket open')
                self.host = host
                self.port = port
                try:
                        self.udpsocket.bind((self.host, self.port))
                except socket.error:
                        print("Socket bind failed")
                        return False
                        
                # Start Receiving thread
                self.RecvThread.start()
                return True


        def close(self):
                ''' Close the socket port.'''
                print('Socket close')
                self.udpsocket.close()
                self.recvDataEvent.set()
                return
                
        def setReceiveCallback(self, receiveCallback):
                self.receiveCallback = receiveCallback
                

        def senddata(self, DestinationIP, DestinationPort, packet):
                print('Packet sent: ' + packet )
                self.udpsocket.sendto(packet.encode(), (DestinationIP, DestinationPort))

        def receivedata(self):
                while (True):
                        if(True == self.recvDataEvent.is_set()):
                                print("returning from receive thread....")
                                return
                        else:
                                try:
                                        d = self.udpsocket.recvfrom(1024)
                                except socket.timeout:
                                        #Waiting here for minimum time
                                        sleep(0.1)
                                        continue
                                except socket.error:
                                        print("Socket error occurred. Exiting from receive thread")
                                        return
                                        
                                if d:
                                    data = d[0]
                                    addr = d[1]
                                        
                                if data:
                                    reply = data.decode("latin-1")
                                    print ('Received[' + addr[0] + ':' + str(addr[1]) + '] - ' + reply.strip())
                                    self.receiveCallback(reply, addr[0], addr[1])

