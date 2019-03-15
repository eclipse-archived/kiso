
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

#### TC 1.1.4 Turn modem ON twice
This test cases tests the reliability of turning the Cellular modem ON twice

1. Turn on the modem by calling "Cellular POWER CONTROL API" 
2. Check For returns SUCCESS
3. Turn on the modem by calling "Cellular POWER CONTROL API" 
4. Check For returns SUCCESS
5. Turn off the modem by calling "Cellular POWER CONTROL API" 
6. Check for returns SUCCESS

#### TC 1.1.5 Turn modem OFF twice
This test cases tests the reliability of turning the Cellular modem OFF twice

1. Turn on the modem by calling "Cellular POWER CONTROL API" 
2. Check For returns SUCCESS
3. Turn off the modem by calling "Cellular POWER CONTROL API" 
4. Check for returns SUCCESS
5. Turn off the modem by calling "Cellular POWER CONTROL API" 
6. Check for returns SUCCESS

#### TC 1.1.6 Reset modem, after Modem ON
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


#### SMS Test Case

## Test Suite 1.10: "Send SMS"
  
### Description
The positive test cases related to SMS send and corresponding response is validated. Using SMS feature, the user can send data of max 160 bytes to a valid mobile number(max 20 bytes) 

#### NOTE
The message which is send, includes alphabets , numbers and Special Character

### Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initializing/Initialized event 

### Teardown
1. Turn off the modem which returns SUCCESS

#### Test cases

#### TC 1.10.1: Sending 5 SMS to a valid mobile number with min data limit(1 byte)
This test case tests the reliability of sending 5 SMS to a valid mobile number with min data limit(1 byte)

### Run
Run steps 1 to 3 for 5 times 

1. Send a text message to a valid mobile number with message length of 1 byte and validate for returns SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)

#### TC 1.10.2: Sending 5 SMS to a valid mobile number with max data limit(160 bytes)
This test case tests the reliability of sending 5 SMS to a valid mobile number


### Run
Run steps 1 to 3 for 5 times

1. Send a text message to a valid mobile number with message length of 160 bytes and validate for returns SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)

#### TC 1.10.3: Sending 5 SMS to a valid mobile number with extended ascii characters
This test case tests the reliability of sending 5 SMS to a valid mobile number with extended ascii characters

### Run
Run steps 1 to 3 for 5 times.

1. Send a text message to a valid mobile number with message of extended ascii characters of 20 bytes length and validate for returns SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)


#### TC 1.10.4: Sending a SMS to 2 different valid mobile number consecutively
This test case tests the reliability of sending a SMS to 2 valid mobile number consecutively


### Run
1. Send a text message to a valid mobile number(say Indian number) with message length of 160 bytes and validate for returns SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)
4. Send a text message to a different valid mobile number(say German number) with message length of 160 bytes and validate for returns SUCCESS
5. Validate the SMS send event, for successful send
6. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)

#### TC 1.10.5: Sending incremental SMS to a valid mobile number 
This test case tests the reliability of sending incremental SMS to a valid mobile number

### Run
Run steps 1 to 3 with initial value of x as 40 bytes and it is incremented by 40, for every time send till max of 160 bytes has been reached

1. Send a text message to a valid mobile number with message length of 40 bytes and validate for returns SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)

#### TC 1.10.6: SMS De-Initialize twice
This test case tests the reliability of De-Initializing the SMS twice

### Teardown
1. Initialize the SMS and validate for returns SUCCESS

##### Run
1. De-Initialize the SMS and validate for returns SUCCESS
2. De-Initialize the SMS and validate for returns SUCCESS

#### TC 1.10.7: Configuring the Service Centre Number twice
This test case tests the reliability of Configuring the Service Centre Number twice

##### Run
1. Get the Service Centre Number for SMS and validate the return SUCCESS
2. Configure the Service Centre Number for SMS and validate the return SUCCESS
3. Configure the Service Centre Number for SMS and validate the return SUCCESS

#### TC 1.10.8: Send SMS in Network connected state
This test case tests the reliability of sending a SMS in Network connected state

### Setup
1. Set modem configuration (APN name, User name, Password) which returns SUCCESS
2. Connect to the network and check for SUCCESS
3. Validate for connected event


### Teardown
1. Disconnect GPRS connection which returns SUCCESS and Validate for disconnected event
2. Turn off the modem which returns SUCCESS

### Run
1. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)

#### TC 1.10.9: Send SMS in Network disconnected state
This test case tests the reliability of sending a SMS in Network disconnected state

### Setup
1. Set modem configuration (APN name, User name, Password) which returns SUCCESS
2. Connect to the network and check for SUCCESS
3. Validate for connected event
4. Disconnect GPRS connection which returns SUCCESS and Validate for disconnected event


### Teardown
1. Turn off the modem which returns SUCCESS

### Run
1. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)

#### TC 1.10.10: Send SMS with socket open
This test case tests the reliability of sending a SMS with socket open

### Setup
1. Set modem configuration (APN name, User name, Password) which returns SUCCESS
2. Connect to the network and check for SUCCESS
3. Validate for connected event
4. Open a socket, which returns SUCCESS


### Teardown
1. Close the opened socket
2. Disconnect the network
3. Turn off the modem which returns SUCCESS

### Run
1. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)

#### TC 1.10.11: Send SMS between socket send
This test case tests the reliability of sending a SMS between socket send

### Setup
1. Set modem configuration (APN name, User name, Password) which returns SUCCESS
2. Connect to the network and check for SUCCESS
3. Validate for connected event
4. Open a socket, which returns SUCCESS


### Teardown
1. Close the opened socket.
2. Disconnect the connection.
3. Turn off the modem which returns SUCCESS

### Run
1. Send 100 bytes of UDP data to the opened socket and validate the returns SUCCESS
2. Validate for the received event and data
3. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
4. Validate the SMS send event, for successful send
5. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)


#### TC 1.10.12: Sending SMS to a valid mobile number after incremental delay 
This test case tests the reliability of sending SMS to a valid mobile number after incremental delay


### Run
Run steps 1 to 4 with initial value of x as 50 seconds and it is incremented by 50 seconds, for every time send till max of 300 seconds delay has been reached

1. Send a text message to a valid mobile number with message length of 100 bytes and validate for returns SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)
4. Provide a time delay of x seconds

#### TC 1.10.13: Power Off the modem without de-initializing the SMS
This test case tests the reliability of powering Off the modem without de-initializing the SMS

##### Run
1. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)
4. Turn off the modem and validate the return SUCCESS
5. Turn On the modem and validate the return SUCCESS
6. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
7. Validate the SMS send event, for successful send
8. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)



#### TC 1.10.14: Get and Configure the Service center Number to validate the SMS send
This test case tests the reliability of Configuring the Service center Number 

##### Run
1. Call the API to get the Service Center Number for SMS and validate the return SUCCESS
2. Configure the Service Center Number for SMS and validate the return SUCCESS
3. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
4. Validate the SMS send event, for successful send
5. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)
 
#### TC 1.10.15: De-initializing the SMS so event should not received 
This test case tests the reliability of powering Off the modem without de-initializing the SMS

### Teardown
1. Initialize the SMS and validate for returns SUCCESS

##### Run
1. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)
4. De-initialize the SMS and validate for returns SUCCESS
5. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
6. Wait for an event for 20 Sec,Event should not received
7. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)

 
#### TC 1.10.16: SMS send with message contain NULL character so the data after NULL character should not received

This test case tests the reliability of message sending with NULL character

##### Run
1. Send a text message to a valid mobile number with message length of 20 bytes  which contains NULL character( 0x00) in between the text and validate for return SUCCESS
2. Validate the SMS send event, for successful send
3. Validate manually the SMS is received on mobile and that the message content after nULL character should not received .(Not consider in jenkins report)

#### TC 1.10.17: SMS send with mobile number of 20 digits and message of 160 byte

This test case tests the reliability of message sending 


##### Run
1. Send a text message to a mobile number  of 20 digits with message length of 160 bytes   validate for return SUCCESS
2. Validate the SMS send event, for Failure.
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)

#### TC 1.10.18: Power Off the modem immediately after sending SMS 
This test case tests the reliability of powering Off the modem immediately after sending the SMS


##### Run
1. Send a text message to a valid mobile number with message length of 20 bytes and validate for return SUCCESS
2. Turn off the modem and validate the return SUCCESS
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report).

#### TC 1.10.19: Sending SMS to a  mobile number of length 6 digit with max data limit(160 bytes)
This test case tests the reliability of sending 5 SMS to a valid mobile number


### Run

1. Send a text message to a  mobile number length of 6 digit with message length of 160 bytes and validate for returns SUCCESS
2. Validate the SMS send event, for failure send
3. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)