Cellular Integration Test Specification Cellular Driver
==========================================

# Test Section 1: "Cellular Driver basic Manual Testing"
## Description
This test section comprises of test cases to check the basic functionality of the Cellular Driver which cannot be fully automated (Python will run the test cases, but there will be manual interventions)

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

## Manual Test cases

## Test Suite 1.12: "Manual test cases"
  
### Description
The manual test cases related Power control (modem On,Off), Network(Init,Connect,Disconnect), Socket(open,send,close). 

### Test Cases

#### TC 1.12.1: Cellular status - initializing
This test cases tests the check for the Cellular initializing status
##### Setup
1. Remove the SIM card from the DUT
2. Turn on the modem which returns SUCCESS

##### Run
1. Initialize the network which returns SUCCESS
2. After initializing event has been received, Call the query information API to get the Cellular status
3. Return status of the API ,must be SUCCESS
4. Check the Cellular status to be initializing ,since there is no SIM

##### TearDown
1. Turn off the modem which returns SUCCESS

#### TC 1.12.2: Network information
This test cases tests the network information and validates the same

##### Setup
1. Remove the SIM card from the DUT
2. Turn on the modem which returns SUCCESS

##### Run
1. Initialize the network which returns SUCCESS
2. After initializing event has been received,Call the query information API to get the network information
3. Return status of the API ,must be FAILURE
4. The API must not return any network informations

#### TC 1.12.3: Disconnected event after SIM card removal
This test cases tests the network disconnection event after sim card removal and validating the same

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
6. Remove the SIM card from the device under test.
7. Disconnection event must be received.

#### TC 1.12.4: Socket close event after SIM card removal
This test cases tests the socket close event after sim card removal and validating the same

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
6. Open the socket which returns SUCCESS for 3 times
7. And the return containing SocketId must be >= 0 from modem
6. Remove the SIM card from the device under test.
7. Disconnection event must be received.
8. Followed by Socket close event for each opened socket.

#### TC 1.12.5: AT command in initializing state
This test cases tests the initializing event in AT command mode and validating the same

##### Setup
1. Remove the SIM card from the DUT
2. Turn on the modem which returns SUCCESS

##### TearDown
1. Turn off the modem which returns SUCCESS

##### Run
1. Initialize the network which returns SUCCESS
2. Then wait for callback event " Initializing" 
3. Check whether the Initializing event is received every 10s.
4. Then sent any AT command
5. Check for the expected AT response
6. Initializing event must be stopped. 

#### TC 1.12.6: Socket send validation by removing the SIM card form DUT
This test case validates socket send by removing the SIM card form DUT during socket send operation
##### Run
1. Initialize the network which returns SUCCESS with event validation check
2. Set modem configuration (APN name, User name, Password) which returns SUCCESS
3. Connect GPRS which returns SUCCESS with connected event validation check
4. Open Socket with 'SOCKET OPEN API' which returns SUCCESS
5. Send data using Socket send API and remove the SIM card during sending,verify the status
	* Check for Disconnect and Socket close(which are opened) events
6. After events received, insert SIM card again
7. Run steps 1 to 4 which return SUCCESS
8. Run socket send API,which returns SUCCESS 

##### Teardown
1. close the socket which returns SUCCESS
2. Disconnect the modem which returns SUCCESS with Disconnect event validation check.
3. Turn OFF the Cellular modem

#### TC 1.12.7 Send SMS during modem initializing

### Setup
1. Remove the SIM card from modem.

### Run
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initializing/Initialized event.
3. Send a text message to a valid mobile number validate for return FAILURE.
4. Validate manually the SMS not received on recipient mobile 
  
### Teardown
1. Turn off the modem which returns SUCCESS

#### TC 1.12.8 Send SMS during/after receiving the message

### Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initialized event.

### Run
1. Send a text message to a valid mobile number validate for return SUCCESS.
2. Validate manually the SMS is received on recipient mobile.
3. Send a text message from valid mobile number to SIM card under test.
4. Send a text message to a valid mobile number, validate for return SUCCESS.
5. Validate manually, the SMS is received on recipient mobile. 
  
### Teardown
1. Turn off the modem which returns SUCCESS

#### TC 1.12.9 Send SMS during calling to SIM under test

### Setup
1. Turn on the modem and validate for return SUCCESS
2. Validate for the Initialized event.

### Run
1. Send a text message to a valid mobile number validate for return SUCCESS.
2. Validate manually the SMS is received on recipient mobile.
3. Make a call from a valid mobile number to SIM card under test.
4. Send a text message to a valid mobile number, validate for return SUCCESS.
5. Validate manually, the SMS is received on recipient mobile. 
6. Disconnect the call.
7. Send a text message to a valid mobile number, validate for return SUCCESS.
8. Validate manually, the SMS is received on recipient mobile. 
  
### Teardown
1. Turn off the modem which returns SUCCESS

#### TC 1.12.10 Send SMS with dummy SIM card(not activated)

### Setup
1. Insert dummy SIM card(not activated)
2. Turn on the modem. Modem will keep on trying to initialize(Since SIM is not activated)

### Run
1. Send a text message to a valid mobile number validate for return FAILURE.
  
### Teardown
1. Turn off the modem which returns SUCCESS