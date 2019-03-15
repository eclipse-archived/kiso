Cellular Integration Test Specification Cellular Driver
==========================================

# Test Section 7: "Cellular Driver stress Testing"
## Description
Stress testing determines the robustness of software by testing beyond the limits of normal operation.Stress test involves in over loading the device with corner case looping(without expecting the response) followed by valid test command(with validation).

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

## Stress Test Cases

## Test Suite 1.7: "Cellular Driver stress Testing"
  
### Description
Stress testing determines the robustness of software by testing beyond the limits of normal operation.Stress test involves in over loading the device with corner case looping(without expecting the response) followed by valid test command(with validation).

NOTE: Cellular UDP related test cases are added in Cellular_Integration_Test_Spec.md file.

### Test Cases

#### TC 1.7.1 Turn ON, OFF
This test case runs turning the Cellular modem ON and turning the Cellular modem OFF for 100 times without expecting response

1. Repeat the below steps for 100 times
	* Turn on the modem by calling "Cellular POWER CONTROL API" 
	* Turn off the modem by calling "Cellular POWER CONTROL API"
2. Turn off the modem by calling "Cellular POWER CONTROL API" 
3. Check for returns SUCCESS

#### TC 1.7.2 Turn ON, RESET
This test cases tests looping of turning the Cellular modem ON and Reset the Cellular modem for 100 times without expecting response

1. Turn on the modem which returns SUCCESS
2. Repeat the below steps for 100 times
	* Turn on the modem by calling "Cellular POWER CONTROL API" 
	* Reset the modem by calling "Cellular CONTROL API"
3. Reset the modem by calling "Cellular CONTROL API" 
4. Check for returns SUCCESS
5. Turn off the modem which returns SUCCESS

#### TC 1.7.3 Cellular network Config settings with maximum parameter length
This test case runs the config settings with APN name 100Bytes for 100 times without expecting response

### Setup
1. Turning on the modem which returns SUCCESS
2. Initialize the network and check for return SUCCESS`
3. Wait for callback event "Initialization Done"
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.

### Run
1. Repeat the below steps for 100 times
	* Config with APN name 100 Bytes, user name 30 bytes,Password 30 bytes
2. Set modem configuration (APN name, User name, Password) which returns SUCCESS

### Teardown
1. Turn off the modem which returns SUCCESS

#### TC 1.7.4 Query network info
This test case requests the network query info for 100 times without expecting response

### Setup
1. Turning on the modem which returns SUCCESS
2. Initialize the network and check for return SUCCESS
3. Wait for callback event "Initialization Done"
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.

### Run
1. Repeat the below steps for 100 times
	* Call the query information API to get the network information
	* Call the query information API to get the Cellular Status
	* Call the query information API to get the error information
2. Call the query information API to get the network information
3. Return status of the API ,must be SUCCESS
4. The API must also return the network information (country code, network code,location area code,cell ID)
5. Check for the total the network information length not to exceed 14 bytes
6. Check for the country code (MCC) to be exactly 3 bytes length
7. Check for the network code (MNC) not to exceed 3 bytes length
8. Check for the location area code (LAC) to be 4 bytes length.
9. Check for the location area code (LAC) to be 4 bytes length
10.Call the query information API to get the error information
11.Check for the network error information
12.Call the query information API to get the Cellular Status 
13.Check for the network status information

### Teardown
1. Turn off the modem which returns SUCCESS


#### TC 1.7.5 Cellular network connection for 100 times
This test runs the Cellular network connection for 100 times 

### Setup
1. Turning on the modem which returns SUCCESS
2. Initialize the network and check for return SUCCESS
3. Wait for callback event "Initialization Done"
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS

### Run
1. Prepare to connect GPRS and check for SUCCESS
2. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address, in case of connection success
3. Repeat the below steps for 100 times
	* Prepare to connect GPRS without waiting for connect event.
4. Disconnect the Cellular network connection and check for SUCCESS
5. Initialize the network and check for return SUCCESS
6. Wait for callback event "Initialization Done"
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.
7. Set modem configuration (APN name, User name, Password) which returns SUCCESS
8. Prepare to connect GPRS and check for SUCCESS
9. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address, in case of connection success

### Teardown
1. Disconnect the Cellular network connection and check for SUCCESS
2. Turn off the modem which returns SUCCESS

#### TC 1.7.6 Disconnect Cellular network for 100 times
This test runs the Cellular network disconnect for 100 times 

### Setup
1. Turning on the modem which returns SUCCESS
2. Initialize the network and check for return SUCCESS
3. Wait for callback event "Initialization Done"
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.

### Run
1. Set modem configuration (APN name, User name, Password) which returns SUCCESS
2. Prepare to connect GPRS and check for SUCCESS
3. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
4. Repeat the below steps for 100 times
	* Prepare to disconnect GPRS.
5. Initialize the network and check for return SUCCESS
6. Wait for callback event "Initialization Done"
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.
7. Set modem configuration (APN name, User name, Password) which returns SUCCESS
8. Prepare to connect GPRS and check for SUCCESS
9. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
10.Disconnect the Cellular network connection and check for SUCCESS

### Teardown 
1. Turn off the modem which returns SUCCESS

#### TC 1.7.7 Open the socket for 100 times
This test runs the socket open for 100 times 

### Setup
1. Turning on the modem which returns SUCCESS
2. Initialize the network and check for return SUCCESS
3. Wait for callback event "Initialization Done"
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success

### Run
1. Repeat the below steps for 100 times
	* Open Cellular socket by calling "Cellular SOCKET OPEN API"
3. Open Cellular socket by calling "Cellular SOCKET OPEN API" which return SUCCESS
4. Return containing SocketId must be >= 0 from modem
5. Close Cellular socket by calling "Cellular SOCKET CLOSE API" with received opened SocketId which returns SUCCESS

### Teardown
1. Disconnect the Cellular network connection and check for SUCCESS
2. Wait for callback event "Disconnected" 
3. Turn off the modem which returns SUCCESS

#### TC 1.7.8 Send config API before init event will appear
This test sends config API before getting Initializing/initialized event

1. Turning on the modem which returns SUCCESS
2. Send config API before getting Initializing/Initialized event
	* Validate the return value for Failure
3. Turn OFF the modem which returns SUCCESS

#### TC 1.7.9 Send Socket open API before connected event will appear
This test sends Socket open API before getting connected event

1. Turning on the modem which returns SUCCESS
2. Initialize the network and check for return SUCCESS
3. Wait for callback event "Initialization Done"
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. Send Socket open API before getting connected event
	* Validate the return value for Failure

### Teardown
1. Turn OFF the modem

#### TC 1.7.10 Send Socket open API before Disconnected event will appear
This test sends Socket open API before getting Disconnected event

1. Turning on the modem which returns SUCCESS
2. Initialize the network and check for return SUCCESS
3. Wait for callback event "Initialization Done"
	* Check for response within the timeout period. Since establishing connection to the Cellular network is based on network signal strength.
4. Set modem configuration (APN name, User name, Password) which returns SUCCESS
5. Prepare to connect GPRS and check for SUCCESS
6. Then wait for callback event "Cellular Connected" 
	* Check if we are getting the connected event with valid ip-address apart from "0.0.0.0", in case of connection success
7. Disconnect the Cellular network connection and check for SUCCESS
8. Send Socket open API before getting disconnected event
	* Validate the return value for Failure
9. Turn OFF the modem

### SMS Test Case

#### TC 1.7.11: Send SMS continuously without waiting for event
This test case tests the reliability of sending a SMS 

###Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the "Initialization" event 

### Teardown
1. Turn off the modem which returns SUCCESS

### Run
1. run step 3 for 10 Times 
2.  Send a text message to a valid mobile number with message length of 160 bytes and validate for return SUCCESS
3.  Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)


#### TC 1.7.12: Send SMS  and then socket open without SMS event and Send UDP packet
This test case tests the reliability of sending a SMS between socket send

### Setup
1. Power ON the Modem
2. Initialize the Modem and ensure it is initialized.
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Connect to the network and check for SUCCESS
5. Validate for connected event

### Teardown
1. Close the opened socket
2. Disconnect the connection
3. Turn off the modem which returns SUCCESS

### Run
1. Send a text message to a valid mobile number with message length of 160 bytes and validate for return SUCCESS
2. Open a socket, which returns SUCCESS
3. Send 100 bytes of UDP data to the opened socket and validate the returns SUCCESS
4. Validate for the received event and data
5. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)



#### TC 1.7.13: Send SMS  and send UDP packet without waiting for SMS send event and UDP packet received event 
This test case tests the reliability of sending a SMS between socket send

### Setup
1. Power ON the Modem
2. Initialize the Modem and ensure it is initialized.
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Connect to the network and check for SUCCESS
5. Validate for connected event
6. Open a socket, which returns SUCCESS

### Teardown
1. Close the opened socket.
2. Disconnect the connection.
3. Turn off the modem which returns SUCCESS

### Run
Run step 1 to 4 for 5 Times 

1. Send 100 bytes of UDP data to the opened socket and validate the returns SUCCESS
2. Send a text message to a valid mobile number with message length of 160 bytes and validate for return SUCCESS
3. Validate for the received event and data
4. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)
5. wait for 10 minutes
6. Send a text message to a valid mobile number with message length of 160 bytes and validate for return SUCCESS
7. Validate the SMS send event, for successful send
