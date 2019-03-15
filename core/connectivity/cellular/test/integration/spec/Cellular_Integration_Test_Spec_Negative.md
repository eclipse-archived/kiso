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

##Test Suite 1.4: "Negative Test Cases"
### Description
All negative conditions related to Power control, Network and Socket; and validating the same.
	
#### TC 1.4.1 Check for network connection fail with invalid configuration settings (APN name) 
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

#### TC 1.4.2 Check for network connection fail with invalid configuration settings (APN name- length exceeded) 
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

#### TC 1.4.3 Check for network connection fail with invalid configuration settings (user name) 
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
	
#### TC 1.4.4 Check for network connection fail with invalid configuration settings (user name) 
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

#### TC 1.4.5 Check for network connection fail with invalid configuration settings (Password) 
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

#### TC 1.4.6 Check for network connection fail with invalid configuration settings (Password) 
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

#### TC 1.4.7 Check for network connection fail with invalid configuration settings ( 5 times ) 
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

#### TC 1.4.8 Connect to Cellular network without configuration
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

#### TC 1.4.9 Disconnecting the Cellular network without connection
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

#### TC 1.4.10 Connect the Cellular network twice
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

#### TC 1.4.11 Disconnect the Cellular network twice
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

#### TC 1.4.12 Turn OFF the Cellular modem, without Disconnecting the Cellular network 
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

#### TC 1.4.13: Test for Socket open ( 7 times )
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

#### TC 1.4.14: Test for Socket operation without socket close ( 3 times )
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

#### TC 1.4.15: Send data to opened socket at wrong SOCKET ID for ( 10 times )
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
	
#### TC 1.4.16: Send data to opened socket at wrong IP address for ( 10 times )
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

#### TC 1.4.17: Test for Socket open without network connect
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

#### TC 1.4.18: Test for Socket close without network connect
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

#### TC 1.4.19: Test for Socket send without network connect
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

#### TC 1.4.20: Test for Socket send without socket open ( 3 times )
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

#### TC 1.4.21: Reset modem, after Modem OFF
This test cases tests the reliability of resetting the Cellular modem

1. Prepare for turning on the modem which returns SUCCESS
2. Turn OFF the modem, which returns SUCCESS
2. Reset the Cellular modem by calling "Cellular CONTROL API" 
3. Check for returns FAILURE

#### TC 1.4.22: Test for wrong configuration in connected state, and try connecting again (3 times)
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

#### TC 1.4.23: Test for wrong configuration in connected state and try disconnecting,connecting again(3 times)
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

#### TC 1.4.24: Test for wrong configuration after disconnected state (3 times)
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

#### TC 1.4.25: Test for Socket send after socket close 
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

#### TC 1.4.26: Initialize the Cellular modem when the modem is in Connected state 
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

#### TC 1.4.27: Connect after Disconnection the Cellular network
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

#### SMS Test Case

#### TC 1.4.28: Send SMS to a invalid mobile number length (21 bytes)
This test case tests the reliability of sending a SMS to a invalid mobile number length

###Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initializing/Initialized event

### Teardown
1. Turn off the modem which returns SUCCESS

### Run
1. Send a text message to a invalid mobile number of 21 bytes with message length of 100 bytes and validate for return FAILURE

#### TC 1.4.29: Send SMS with invalid message length (161 bytes)
This test case tests the reliability of sending a SMS with invalid message length (161 bytes)

###Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initializing/Initialized event

### Teardown
1. Turn off the modem which returns SUCCESS

### Run
1. Send a text message to a valid mobile number with message length of 161 bytes and validate for return FAILURE

#### TC 1.4.30: Send SMS with invalid message length (161 bytes) and invalid mobile number length (21 bytes)
This test case tests the reliability of sending a SMS with invalid message length (161 bytes) and invalid mobile number length (21 bytes)

###Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initializing/Initialized event

### Teardown
1. Turn off the modem which returns SUCCESS

### Run
1. Send a text message to a invalid mobile number length of 21 bytes with message length of 161 bytes and validate for return FAILURE

#### TC 1.4.31: Send SMS with valid message length (20 bytes) and invalid mobile number  (14 bytes Alphanumeric)
This test case tests the reliability of sending a SMS with message length (20 bytes) and invalid mobile number  (14 bytes alphanumeric)

###Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initializing/Initialized event

### Teardown
1. Turn off the modem which returns SUCCESS

### Run
1. Send a text message to a invalid mobile number length of 14 bytes of Alphanumeric with message length of 20 bytes and validate for return FAILURE


#### TC 1.4.32: Send SMS to a invalid service center number (14 bytes Alphanumeric)
This test case tests the reliability of sending a SMS to a invalid service center number

###Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initializing/Initialized event

### Teardown
1. Turn off the modem which returns SUCCESS

### Run
1. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
2. Validate the SMS send event, for Successfully send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)
4. Configure the SMS with invalid service center number (14 bytes Alphanumeric) and validate for return Failure
5. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
6. Validate the SMS send event, for Failure 

#### TC 1.4.33: SMS Initialize twice
This test case tests the reliability of Initializing the SMS twice

###Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initializing/Initialized event

### Teardown
1. Turn off the modem which returns SUCCESS

### Run
1. Initialize the SMS and validate for returns FAILURE


#### TC 1.4.34: Send SMS with zero length of Message 
This test case tests the reliability of sending a SMS with no message character

###Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initializing/Initialized event

### Teardown
1. Turn off the modem which returns SUCCESS

### Run
1. Send a text message to a valid mobile number validate for return FAILURE

