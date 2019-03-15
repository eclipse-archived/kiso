Cellular Integration Test Specification Cellular Driver
==========================================

# Test Section 1: "Cellular Driver basic Testing"
## Description
This test section comprises of test cases to check the basic functionality of the Cellular Driver

### Setup
1. Test participant : CoSP device 
2. Test Manager : Running on PC via Python scripts
3. Network : Any subscriber with GPRS activated SIM-Card   
4. The test participant (CoSP device) has to be connected with PC through UART.
5. Setup for Flashing- Manual
	* Flash SIMCOM .cfg file into the SIMCOM modem of CoSP device
	* Flash IntegrationtestApp binary file into CoSP device
	* Once the flashing is done, It automatically reset the device and IntegrationtestApp Boots-up  
6. Initialize the Cellular system which returns SUCCESS

### Teardown 
1. Uninitialize the Cellular system which returns SUCCESS
2. Turn off the test participant(CoSP device)

## Positive Test Cases

## Test Suite 1.1: "Cellular Power control"
  
### Description
The positive condition related to Cellular Power control Module(ON,OFF,Reset) and corresponding response is validated.

### Test Cases
#### TC 1.1.1 Turn modem ON and OFF
This test cases tests the reliability of turning the Cellular modem ON and turning the Cellular modem OFF

1. Turn on the modem by calling "Cellular POWER CONTROL API" 
2. Check For returns SUCCESS
3. Turn off the modem by calling "Cellular POWER CONTROL API" 
4. Check for returns SUCCESS

#### TC 1.1.2 Turn ON, OFF stability (time delay-shorter duration) 
This test cases tests the reliability of turning the Cellular modem ON and turning the Cellular modem OFF

1. Turn on the modem by calling "Cellular POWER CONTROL API" 
2. Check For returns SUCCESS
3. Random time delay of shorter duration (say 15 seconds)
3. Turn off the modem by calling "Cellular POWER CONTROL API" 
4. Check for returns SUCCESS

#### TC 1.1.3 Turn ON, OFF stability (time delay-longer duration) 
This test cases tests the reliability of turning the Cellular modem ON and turning the Cellular modem OFF with time delay

1. Turn on the modem by calling "Cellular POWER CONTROL API" 
2. Check For returns SUCCESS
3. Random time delay of longer duration (say 60 seconds)
3. Turn off the modem by calling "Cellular POWER CONTROL API" 
4. Check for returns SUCCESS

#### TC 1.1.4 Turn modem OFF twice
This test cases tests the reliability of turning the Cellular modem OFF twice

1. Turn on the modem by calling "Cellular POWER CONTROL API" 
2. Check For returns SUCCESS
3. Turn off the modem by calling "Cellular POWER CONTROL API" 
4. Check for returns SUCCESS
5. Turn off the modem by calling "Cellular POWER CONTROL API" 
6. Check for returns SUCCESS

#### TC 1.1.5 Reset modem, after Modem ON
This test cases tests the reliability of resetting the Cellular modem

1. Prepare for turning on the modem which returns SUCCESS
2. Reset the Cellular modem by calling "Cellular CONTROL API" 
3. Check for returns SUCCESS

## Test Suite 1.2: "Cellular network connection"
### Description
The positive condition related to Cellular Module initialization, connection and disconnection; corresponding response is validated.

### Setup
1. Turn on the modem which returns SUCCESS

### Teardown 
1. Turn off the modem which returns SUCCESS

#### TC 1.2.1 Initialization of Cellular network
This test cases tests whether Cellular modem is able to initialize to Cellular network.

1. Initialize the network which returns SUCCESS
2. Then wait for callback event "Initializing | Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 

#### TC 1.2.2 Initialization of Cellular network twice
This test cases tests whether Cellular modem twice is able to connect to Cellular network.

1. Initialize the network which returns SUCCESS
2. Then wait for callback event "Initializing | Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Initialize the network again, which returns SUCCESS
4. Then wait for callback event "Initializing | Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 

#### TC 1.2.3 Init, Configure, Connect, Disconnect the Cellular network
This test case tests the basic functionality of Cellular network

1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Prepare to connect GPRS and check for SUCCESS
5. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
	
6. And Disconnect GPRS connection and check for SUCCESS
7. And wait for callback event "Disconnected" 
	* Get the event data as disconnected within the specified timeout (since disconnecting for data connection from network will take less time only) 

#### TC 1.2.4 Init,Time delay,Configure,Connect,Disconnect the Cellular network
This test cases tests the reliability of Connecting and disconnecting the Cellular network after successful init and configure with some time delay

1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Random time delay (say 120 seconds)
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
7. And Disconnect GPRS connection and check for SUCCESS
8. And wait for callback event "Disconnected" 

#### TC 1.2.5 Init,Configure,Time delay,Connect,Disconnect the Cellular network
This test cases tests the reliability of Connecting and disconnecting the Cellular network after successful init and configure with some time delay

1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Random time delay(say 120 seconds)
5. Prepare to connect GPRS and check for SUCCESS
6. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
7. And Disconnect GPRS connection and check for SUCCESS
8. And wait for callback event "Disconnected" 

#### TC 1.2.6 Init,Configure,Connect,Time delay,Disconnect the Cellular network
This test cases tests the reliability of disconnecting the Cellular network after successful connection with some time delay

1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Prepare to connect GPRS and check for SUCCESS
5. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
6. Random time delay(say 120 seconds)
7. And Disconnect GPRS connection and check for SUCCESS
8. And wait for callback event "Disconnected" 

#### TC 1.2.7 Init, Configure, Connect, Disconnect the Cellular network
This test cases tests the reliability of init,Connecting and disconnecting (multiple times)the Cellular network after successful init and configure.

Repeat the below sequence for 5 times:
1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Prepare to connect GPRS and check for SUCCESS
5. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
6. And Disconnect GPRS connection and check for SUCCESS
7. And wait for callback event "Disconnected" 

##Test Suite 1.3: "Socket Operations"
### Description
All the conditions related to Socket operations (Socket open,send data to server and socket close) and validating the same.

###Setup
1. Prepare for turning on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
	
### Teardown
1. Disconnect GPRS connection which returns SUCCESS
2. And wait for callback event "Disconnected" 
3. Turn off the modem which returns SUCCESS

####Test Cases 

#### TC 1.3.1: Socket open and close
This test cases tests the reliability of the socket open and socket close operations. 

1. Open Cellular socket by calling "Cellular SOCKET OPEN API" which return SUCCESS
2. And return containing SocketId must be >= 0 from modem
3. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId which returns SUCCESS

#### TC 1.3.2: Socket open(3) and close
This test cases tests the reliability of opening 3 socket and closing the opened socket.
Note: Ensure the socketIds to be different,for every new opened socket

1. Repeat the following procedure 3 times
	1. Open the socket and check for SUCCESS 
	2. And the return containing SocketId must be >= 0 from modem
2. Check whether all the 3 socket ids are different
2. Repeat the following procedure 3 times 
	1. Close the socket with received opened SocketId which returns SUCCESS

#### TC 1.3.3: Socket open, time delay , close Socket
This test cases tests the reliability of the socket open and socket close operations with some delay

1. Open Cellular socket by calling "Cellular SOCKET OPEN API" which return SUCCESS
2. And return containing SocketId must be >= 0 from modem
3. Random time delay(say 260 seconds) 
3. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId which returns SUCCESS

#### TC 1.3.4: Send data to Socket Opened
This test cases tests the reliability of sending the data to the opened socket.

1. Open the socket and check for SUCCESS
2. And the return containing SocketId must be >= 0 from modem
3. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return SUCCESS
4. Validate for the received data event 
5. Close the socket with received opened SocketId which returns SUCCESS

#### TC 1.3.5: Send data to 3 Opened sockets
This test cases tests the reliability of sending the data to 3 Opened sockets

1. Repeat the following procedure 3 times
	1. Open the socket and check for SUCCESS 
	2. And the return containing SocketId must be >= 0 from modem
2. Repeat the following procedure 3 times, different data for each of the socket
	* Send different data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return SUCCESS
	* Validate for the received data event
3. Repeat the following procedure 3 times 
	1. Close the socket with received opened SocketId which returns SUCCESS

#### TC 1.3.6: Send data to Socket Open with time delay
This test cases tests the reliability of sending the data to the opened socket with time delay

1. Open the socket and check for SUCCESS
2. And the return containing SocketId must be >= 0 from modem
3. Random time delay(say 260 seconds)
4. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return SUCCESS
5. Validate for the received data event
6. Close the socket with received opened SocketId which returns SUCCESS

#### TC 1.3.7: Send multiple (5) datum to Socket Opened
This test cases tests the reliability of sending the multiple data to the opened socket.

1. Open the socket and check for SUCCESS
2. And the return containing SocketId must be >= 0 from modem
3. Repeat the following procedure 5 times, data must be different each time 
	* Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return SUCCESS
	* Validate for the received data event
4. Close the socket with received opened SocketId which returns SUCCESS

#### TC 1.3.8: Socket open, send data, receive data(time delay) and close
This test cases tests the reliability of the socket send with a time delay

1. Repeat the following procedure 8 times
	1. Open the socket and check for SUCCESS 
	2. And the return containing SocketId must be >= 0 from modem
	3. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return SUCCESS
	4. Random time delay(say 260 seconds)
	6. Validate for the received data event
	6. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId which returns SUCCESS

#### TC 1.3.9: Socket open, send data(time delay), receive data(time delay) and close
This test cases tests the reliability of the socket send with a time delay

1. Repeat the following procedure 8 times
	1. Open the socket and check for SUCCESS 
	2. And the return containing SocketId must be >= 0 from modem
	3. Random time delay(say 260 seconds)
	3. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return SUCCESS
	4. Random time delay(say 260 seconds)
	6. Validate for the received data event
	6. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId which returns SUCCESS
	
##Test Suite 1.4: "Negative Test Cases"
### Description
All negative conditions related to Power control, Network and Socket; and validating the same.

#### TC 1.4.1 Check for network initialization without Modem ON for ( 5 times )
This Negative test cases tests the reliability of the Cellular modem initialization without Modem ON 
 
1. Repeat the following procedure for 5 times
	1. Initialize the network which return FAILURE
	2. Set modem configuration (APN name, User name, Password) which return FAILURE
	
#### TC 1.4.2 Check for network connection fail with invalid configuration settings (APN name) 
This Negative test cases tests the reliability of network connection with invalid configuration settings (APN name) -> Not applicable for India, n/w connection is established for invalid apn setting also
##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. And initialize the network and check for return SUCCESS
2. Then wait for callback event "Initialization Done" 
3. Set modem configuration (APN name -invalid, User name -valid, Password -valid) which returns SUCCESS
4. Prepare to connect GPRS and check for FAILURE

#### TC 1.4.3 Check for network connection fail with invalid configuration settings (APN name- length exceeded) 
This Negative test cases tests the reliability of network connection with invalid configuration settings (APN name) - more than max size 
##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. And initialize the network and check for return SUCCESS
2. Then wait for callback event "Initialization Done" 
3. Set modem configuration (APN name -invalid, User name -valid, Password -valid) which returns FAILURE
4. Prepare to connect GPRS and check for FAILURE

#### TC 1.4.4 Check for network connection fail with invalid configuration settings (user name) 
This Negative test cases tests the reliability of network connection with invalid configuration settings (user name) -> Not applicable for India, n/w connection is established for invalid apn setting also

##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. And initialize the network and check for return SUCCESS
2. Then wait for callback event "Initialization Done" 
3. Set modem configuration (APN name -valid, User name -invalid, Password -valid) which returns SUCCESS
4. Prepare to connect GPRS and check for FAILURE
	
#### TC 1.4.5 Check for network connection fail with invalid configuration settings (user name) 
This Negative test cases tests the reliability of network connection with invalid configuration settings (user name) - more than max size

##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. And initialize the network and check for return SUCCESS
2. Then wait for callback event "Initialization Done" 
3. Set modem configuration (APN name -valid, User name -invalid, Password -valid) which returns FAILURE
4. Prepare to connect GPRS and check for FAILURE

#### TC 1.4.6 Check for network connection fail with invalid configuration settings (Password) 
This Negative test cases tests the reliability of network connection with invalid configuration settings (Password)-> Not applicable for India, n/w connection is established for invalid apn setting also

##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. And initialize the network and check for return SUCCESS
2. Then wait for callback event "Initialization Done" 
3. Set modem configuration (APN name -valid, User name -valid, Password -invalid) which returns SUCCESS
4. Prepare to connect GPRS and check for FAILURE

#### TC 1.4.7 Check for network connection fail with invalid configuration settings (Password) 
This Negative test cases tests the reliability of network connection with invalid configuration settings (Password)- more than max size

##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. And initialize the network and check for return SUCCESS
2. Then wait for callback event "Initialization Done" 
3. Set modem configuration (APN name -valid, User name -valid, Password -invalid) which returns FAILURE
4. Prepare to connect GPRS and check for FAILURE

#### TC 1.4.8 Check for network connection fail with invalid configuration settings ( 5 times ) 
This Negative test cases tests the reliability of network connection with null configuration settings

##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. Repeat the following procedure for 5 times
	1. And initialize the network and check for return SUCCESS
	2. Then wait for callback event "Initialization Done" 
	3. Set modem configuration (APN name "null", User name "null", Password "null") which returns FAILURE
	4. Prepare to connect GPRS and check for FAILURE

#### TC 1.4.9 Connect to Cellular network without configuration
This test cases tests the reliability of Connecting without configuration

##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Prepare to connect GPRS and check for FAILURE

#### TC 1.4.10 Disconnecting the Cellular network without connection
This test cases tests the reliability of disconnecting without connecting
##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Disconnect GPRS connection and check for FAILURE

#### TC 1.4.11 Connect the Cellular network twice
This test cases tests the reliability of Connection with trying to connect twice
##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Prepare to connect GPRS and check for SUCCESS
5. Prepare to connect GPRS and check for FAILURE

#### TC 1.4.12 Disconnect the Cellular network twice
This test cases tests the reliability of disconnection with trying to disconnect twice
##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Prepare to connect GPRS and check for SUCCESS
5. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
6. Disconnect GPRS connection and check for SUCCESS
7. Disconnect GPRS connection and check for FAILURE

#### TC 1.4.13 Turn OFF the Cellular modem, without Disconnecting the Cellular network 
This test cases tests the reliability of disconnection (The outcome of this testCase vary from country to country)
##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Prepare to connect GPRS and check for SUCCESS
5. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
6. Turn off the modem which returns SUCCESS
7. Turn on the modem which returns SUCCESS
8. Repeat steps 1 to 5, connection should be established successfully
9. Disconnect GPRS connection and check for SUCCESS 

#### TC 1.4.14: Test for Socket open ( 7 times )
This Negative test cases tests the reliability of opening the socket for 3 times and after that socket should not open
 
#####Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success

##### Teardown
1. Disconnect the GPRS from Cellular modem which returns SUCCESS
2. And wait for callback event "Disconnected" 
3. Then turn off the modem which returns SUCCESS

##### TestRun
1. Repeat the following procedure for 7 times
	1. Open the socket which returns SUCCESS for first 3 times and for other FAILURE
	2. And the return containing SocketId must be >= 0 from modem for first 3 times and for other no Socket ID
2. Close all the opened sockets

#### TC 1.4.15: Test for Socket operation without socket close ( 3 times )
This Negative test cases tests the reliability of opening the socket for 3 times, without socket close, turn off the modem
 
#####Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success

##### Teardown
1. Disconnect the GPRS from Cellular modem which returns SUCCESS
2. And wait for callback event "Disconnected" 
3. Then turn off the modem which returns SUCCESS

##### TestRun
1. Repeat the following procedure for 3 times
	1. Open the socket which returns SUCCESS for first 3 times and for other FAILURE
	2. And the return containing SocketId must be >= 0 from modem for first 3 times and for other no Socket ID
2. Disconnect the GPRS from Cellular modem which returns FAILURE
3. Then turn off the modem which returns SUCCESS
4. Turn on the modem which returns SUCCESS
5. And initialize the network and check for return SUCCESS
6. Then wait for callback event "Initialization Done" 
7. Set modem configuration (APN name, User name, Password) which returns SUCCESS
8. Prepare to connect GPRS and check for SUCCESS
9. And wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
10. Open the socket and check for SUCCESS
11. And the return containing SocketId must be >= 0 from modem
12. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return SUCCESS
13. Validate for the received data event
15. Close the socket with received opened SocketId which returns SUCCESS

#### TC 1.4.16: Send data to opened socket at wrong SOCKET ID for ( 10 times )
This test cases tests the reliability of the Cellular modem after socket open try to send data to sever for 10 times but no data received at device and and then close the socket.

#####Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	1. Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
	2. And if we may also get disconnect event, in case of connection failure which leads to test case failure
	3. We should get connection success within the specified timeout

##### Teardown
1. Disconnect the GPRS from Cellular modem which returns SUCCESS
2. And wait for callback event "Disconnected" 
3. Then turn off the modem which returns SUCCESS

##### TestRun
1. Open the socket which returns SUCCESS 
2. And the return containing SocketId must be >= 0 from modem
3. Repeat the following procedure for 10 times
	1. Send different data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId (Wrong i.e not match with return of SOCKET OPEN), Ip address, Port no, Data) which return FAILURE
4. Close the socket with Socket ID Returned from Socket Open
5. Check for status returns SUCCESS
	
#### TC 1.4.17: Send data to opened socket at wrong IP address for ( 10 times )
This test cases tests the reliability of the Cellular modem after socket open try to send data to the server(Undesired IP address) for 10 times and and then close the socket.

#####Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	1. Check if we are getting the connected event with valid ip-address, in case of connection success

##### Teardown
1. Disconnect the GPRS from Cellular modem which returns SUCCESS
2. And wait for callback event "Disconnected" 
3. Then turn off the modem which returns SUCCESS

##### TestRun
1. Open the socket which returns SUCCESS 
2. And the return containing SocketId must be >= 0 from modem
3. Repeat the following procedure for 10 times
	1. Send different data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address(Undesired Ip address), Port no, Data) which return SUCCESS
4. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address(correct Ip address), Port no, Data) which return SUCCESS
5. Validate the response from server
4. Close the socket with Socket ID Returned from Socket Open
5. Check for status returns SUCCESS

#### TC 1.4.18: Test for Socket open without network connect
This Negative test cases tests the reliability of opening a socket without connecting the Cellular network
 
#####Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS

##### Teardown
1. Then turn off the modem which returns SUCCESS

##### TestRun
1. Open the socket and check for FAILURE

#### TC 1.4.19: Test for Socket close without network connect
This Negative test cases tests the reliability of closing 3 socket without connecting the Cellular network 
 
#####Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS

##### Teardown
1. Disconnect the GPRS from Cellular modem which returns SUCCESS
2. And wait for callback event "Disconnected" 
3. Then turn off the modem which returns SUCCESS

##### TestRun
1. Repeat the steps for 3 times:
	1. Close the socket with some random Socket ID Returned from Socket Open

#### TC 1.4.20: Test for Socket send without network connect
This Negative test cases tests the reliability of sending socket id to the 3 socket without connecting the Cellular network 
 
#####Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	1. Check if we are getting the connected event with valid ip-address, in case of connection success

##### Teardown
1. Then turn off the modem which returns SUCCESS

##### TestRun
1. Repeat the steps for 3 times:
	1. Open the socket which returns SUCCESS 
	2. And the return containing SocketId must be >= 0 from modem
	3. Disconnect the GPRS from Cellular modem which returns SUCCESS
	4. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId , Ip address, Port no, Data) which return FAILURE
	4. Close the socket with Socket ID Returned from Socket Open, return SUCCESS

#### TC 1.4.21: Test for Socket send without socket open ( 3 times )
This Negative test cases tests the reliability of sending socket data without socket open, repeat for 3 times
 
#####Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
	
##### Teardown
1. Disconnect the GPRS from Cellular modem which returns SUCCESS
2. And wait for callback event "Disconnected" 
3. Then turn off the modem which returns SUCCESS

##### TestRun
1. Repeat the following procedure for 3 times
	1. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId-random id, Ip address, Port no, Data) which return FAILURE
	2. Open the socket and check for SUCCESS
	3. And the return containing SocketId must be >= 0 from modem
	4. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return SUCCESS
	5. Validate the response from server
	6. Close the socket with received opened SocketId which returns SUCCESS

#### TC 1.4.22 Reset modem, after Modem OFF
This test cases tests the reliability of resetting the Cellular modem

1. Prepare for turning on the modem which returns SUCCESS
2. Turn OFF the modem, which returns SUCCESS
2. Reset the Cellular modem by calling "Cellular CONTROL API" 
3. Check for returns FAILURE

#### TC 1.4.23: Test for wrong configuration in connected state, and try connecting again (3 times)
This Negative test cases tests the reliability of changing the configuration in connected state and try connecting again

###Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success

##### Teardown
1. Then turn off the modem which returns SUCCESS

##### TestRun
1. In connected state, set wrong modem configuration (APN name-wrong, User name, Password)
2. Repeat the below step for 3 times,
	* Prepare to connect GPRS and check for FAILURE

#### TC 1.4.24: Test for wrong configuration in connected state and try disconnecting,connecting again(3 times)
This Negative test cases tests the reliability of changing the configuration in connected state and try disconnecting,connecting again(3 times)

###Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
##### Teardown
1. Then turn off the modem which returns SUCCESS

##### TestRun
1. In connected state, set wrong modem configuration (APN name-wrong, User name, Password)
2. Disconnect the GPRS from Cellular modem which returns SUCCESS
3. And wait for callback event "Disconnected" 
4. Repeat the below step for 3 times,
	* Prepare to connect GPRS and check for FAILURE

#### TC 1.4.25: Test for wrong configuration after disconnected state (3 times)
This Negative test cases tests the reliability of changing the configuration in connected state

###Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success

##### Teardown
1. Then turn off the modem which returns SUCCESS

##### TestRun
1. Disconnect the GPRS from Cellular modem which returns SUCCESS
2. And wait for callback event "Disconnected" 
3. Set wrong modem configuration (APN name-wrong, User name, Password)
4. Repeat the data connection for 3 times,
	* Prepare to connect GPRS and check for FAILURE

#### TC 1.4.26: Test for Socket send after socket close 
This Negative test cases tests the reliability of sending data to server after socket close
 
#####Setup
1. Turn on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success

##### Teardown
1. Disconnect the GPRS from Cellular modem which returns SUCCESS
2. And wait for callback event "Disconnected" 
3. Then turn off the modem which returns SUCCESS

##### TestRun
1. Open the socket which returns SUCCESS 
2. And the return containing SocketId must be >= 0 from modem
3. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return SUCCESS. 
4. Close the socket with received opened SocketId which returns SUCCESS
5. Repeat sending data to the closed socket 3 times:
	* Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return FAILURE

#### TC 1.4.27 Initialize the Cellular modem when the modem is in Connected state 
This test cases tests the reliability of connection
##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Prepare to connect GPRS and check for SUCCESS
5. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
6. Try to initialize the modem again
7. Modem should not be initialized and remains in connected state. 

#### TC 1.4.28 Connect after Disconnection the Cellular network
This test cases tests the reliability of Connecting after disconnecting (multiple times)the Cellular network after successful init and configure.
##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. Initialize the network which returns SUCCESS
2. Then wait for callback event "Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Prepare to connect GPRS and check for SUCCESS
5. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
6. And Disconnect GPRS connection and check for SUCCESS
7. And wait for callback event "Disconnected"
8. Repeat connection 5 times:
* Prepare to connect GPRS and check for FAILURE

##Test Suite 1.5: "Query information"
### Description
All the conditions related to network information, error info, cellular status(initialising,initialized,connected,disconnected) and validating the same.
##### Setup
1. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

####Test Cases 

#### TC 1.5.1: Network information
This test cases tests the network information and validates the same

1. Initialize the network which returns SUCCESS
2. Then wait for callback event "Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Call the query information API to get the network information
4. Return status of the API ,must be SUCCESS
5. The API must also return the network information (country code, network code,location area code,cell ID)
6. Check for the total the network information length not to exceed 14 bytes
7. Check for the country code (MCC) to be exactly 3 bytes length
8. Check for the network code (MNC) not to exceed 3 bytes length
9. Check for the location area code (LAC) to be 4 bytes length.
10. Check for the cell ID to be 4 bytes length

#### TC 1.5.2: Error information
This test cases tests the checks the error information in initialized state

1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Call the query information API to get the error information
4. Return status of the API ,must be SUCCESS
5. Check the error field for no error

#### TC 1.5.3: Cellular status - initialized 
This test cases tests the check for the Cellular initialized status

1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Call the query information API to get the Cellular status
4. Return status of the API ,must be SUCCESS
5. Check the Cellular status to be initialized

#### TC 1.5.4: Cellular status - Connected 
This test cases tests the check for the Cellular Connected status

##### Setup
1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Prepare to connect GPRS and check for SUCCESS
5. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. After connected event has been received,Call the query information API to get the Cellular status
2. Return status of the API ,must be SUCCESS
3. Check the Cellular status to be Connected

#### TC 1.5.5: Cellular status - Disconnected 
This test cases tests the check for the Cellular Connected status

##### Setup
1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength. 
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Prepare to connect GPRS and check for SUCCESS
5. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
6. And Disconnect GPRS connection and check for SUCCESS
7. And wait for callback event "Disconnected"

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. After disconnected event has been received,Call the query information API to get the Cellular status
2. Return status of the API ,must be SUCCESS
3. Check the Cellular status to be disconnected


##Test Suite 1.8: "UDP Data Transmission"
### Description
Basic UDP data transmission and reception. The leshan server in Rt is made use for testing the same.

###Setup
1. Prepare for turning on the modem which returns SUCCESS
2. And initialize the network and check for return SUCCESS
3. Then wait for callback event "Initialization Done" 
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. And wait for callback event "Cellular Connected" 
	
### Teardown
1. Disconnect GPRS connection which returns SUCCESS
2. And wait for callback event "Disconnected" 
3. Turn off the modem which returns SUCCESS

####Test Cases 

#### TC 1.8.1: UDP send and receive data using socket
This test case tests the received data from server

###Setup
1. Open the socket and check for SUCCESS 
2. Send UDP data to the server and check for the received data event.

### Run
1. Validate for the received data event and check for the received UDP data("COSP_RX") for 100 times.

### Teardown
1. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId and check for SUCCESS

#### TC 1.8.2: UDP receive data after a delay of 4 min
This test case tests whether the device is able to receive data from server after 4 min of inactivity

###Setup
1. Open the socket and check for SUCCESS 
2. Send UDP data to the server and check for the received data event.

### Run
1. Wait for 4 min and check for the received data event and check for the received UDP data("COSP_RX after delay of 4 minutes").
2. It should return failure.Server communication will be deactivated due to NAT window when data send from server after 4 min.  

### Teardown
1. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId and check for SUCCESS

#### TC 1.8.3: UDP receive data after a delay of 30 sec
This test case tests whether the device is able to receive data from server after 30 sec of inactivity

###Setup
1. Open the socket and check for SUCCESS 
2. Send UDP data to the server and check for the received data event.

### Run
1. Wait for 30 sec and check for the received data event and check for the received UDP data("COSP_RX after delay of 30 sec").

### Teardown
1. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId and check for SUCCESS

#### TC 1.8.4 socket send for 100000 times
This test runs the socket send (with 185 Bytes as data) for 100000 times 

### Setup
1. Open Cellular socket by calling "Cellular SOCKET OPEN API" which return SUCCESS
2. And the return containing SocketId must be >= 0 from modem

### Run
1. Repeat the below steps for 100000 times
	* send data using socket by calling "Cellular SOCKET SEND DATA API" with different data length field, provided the data value should not be an extended ASCII value
2. send data using socket by calling "Cellular SOCKET SEND DATA API"
	* Validate the return status 

### Teardown
1. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId which returns SUCCESS

#### TC 1.8.5 send and receive data with max data limit    
To verify by calling send API continuously and checks the response received within an expected threshold time limit 

### Setup
1. Open Cellular socket by calling "Cellular SOCKET OPEN API" which return SUCCESS
2. And the return containing SocketId must be >= 0 from modem

### Run
1. Call API for Socket send with 185 bytes of data.
2. Wait for the return value of called API and it should be success.
3. Verify the response is received within 5 seconds.
4. Repeat the Step 1-3 for 100000 times

### Teardown
1. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId which returns SUCCESS

#### TC 1.8.6 With three sockets send data continuously to server   
To verify by calling send API continuously and checks the response received within an expected threshold time limit  

### Setup
1. Open three Cellular sockets by calling "Cellular SOCKET OPEN API" which return SUCCESS
2. And the return containing SocketId must be >= 0 from modem for all three sockets

### Run

1. Call API for Socket send to Socket1 with 185 bytes of data.
2. Wait for response and it should be success.
3. Verify the response is received within 5 seconds. 
4. Repeat the Step 1-3 for 100 Times.
5. Call API for Socket send to Socket2 with 185 bytes of data.
6. Wait for response and it should be success.
7. Verify the response is received within 5 seconds 
8. Repeat the Step 5-7 for 100 Times.
9. Call API for Socket send to Socket3 with 185 bytes of data.
10. Wait for response and it should be success.
11. Verify the response is received within 5 seconds 
12. Repeat the Step 9-11 for 100 Times.

### Teardown
1. Close three Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId which returns SUCCESS

#### TC 1.8.7 Send the data with exceeding data length to server   
To verify by calling send API continuously with exceeding data length  

### Setup
1. Open Cellular socket by calling "Cellular SOCKET OPEN API" which return SUCCESS
2. And the return containing SocketId must be >= 0 from modem

### Run
1. Call API for Socket send with 200 bytes of data.
2. Wait for the return value of called API and checks it for failure. 
3. Repeat the Step 1-3 for 100 times

### Teardown
1. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId which returns SUCCESS
