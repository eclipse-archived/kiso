Cellular Integration Test Specification Cellular Driver
==========================================

# Test Section 1: "Cellular Driver Smoke Testing"
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

## Test Suite 1.9: "Smoke Test Cases"
  
### Description
The positive test cases related to Cellular Power control, Network, Socket to check basic functionalities of the Cellular are working fine.

### Test Cases
#### TC 1.9.1 Turn modem ON and OFF
This test cases tests the reliability of turning the Cellular modem ON and turning the Cellular modem OFF

1. Turn on the modem by calling "Cellular POWER CONTROL API" 
2. Check For returns SUCCESS
3. Turn off the modem by calling "Cellular POWER CONTROL API" 
4. Check for returns SUCCESS

#### TC 1.9.2 Reset modem, after Modem ON
This test cases tests the reliability of resetting the Cellular modem

1. Prepare for turning on the modem which returns SUCCESS
2. Reset the Cellular modem by calling "Cellular CONTROL API" 
3. Check for returns SUCCESS

#### TC 1.9.3 Init, Configure, Connect, Disconnect the Cellular network
This test case tests the basic functionality of Cellular network

### Setup
1. Turn on the modem which returns SUCCESS

### Teardown 
1. Turn off the modem which returns SUCCESS

##### TestRun
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

#### TC 1.9.4: Send data to Socket Opened
This test cases tests the reliability of sending the data to the server 

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

##### TestRun
This test cases tests the reliability of sending the data to the opened socket.

1. Open the socket and check for SUCCESS
2. And the return containing SocketId must be >= 0 from modem
3. Send data to the server by calling "Cellular SOCKET SEND DATA API" (SocketId, Ip address, Port no, Data) which return SUCCESS
4. Validate for the received data event 
5. Close the socket with received opened SocketId which returns SUCCESS


#### SMS test Case

#### TC 1.9.5: Send SMS to a valid mobile number
This test case tests the reliability of sending a SMS to a valid mobile number

###Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the "Initialization" event 

### Teardown
1. Turn off the modem which returns SUCCESS

##### Run
1. Send a text message to a valid mobile number with message length of 160 bytes and validate for return SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)
