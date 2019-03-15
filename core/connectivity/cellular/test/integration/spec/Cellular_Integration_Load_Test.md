Cellular Integration Load Test Specification Cellular Driver
==================================================

# Test Section 1: "Load Testing for Cellular Driver"
## Description
This test section comprises the test cases of Load testing to verify that the Cellular Driver should not hang or crash under the expected threshold value. It is validated by calling the API and expecting the response(success or failure) within the threshold time limit.

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

## Test Suite 1.6: "Load Test Cases for Cellular Driver "
  
### Description
The load test cases related to Cellular Driver (ON, OFF, Reset, N/W initialization, N/W configuration, N/W connection, N/W disconnection, Socket Open, Socket Send and Socket Close) are validated.

### Test Cases

#### TC 1.6.1 Loading Cellular driver with Power ON API
To verify that after calling the Power ON API the Event has received within Threshold Limit.

### Setup
Turn On the Modem and ensure it is turned On.

### Teardown
Turn Off the Modem and ensure it is turned Off.  

##### Run
 Repeat the Step 1-3 for 100 times 

1. Call API for Power ON and receive RETCODE_OK in 20 Seconds
2. Wait for Initialization Event(either Initializing or Initialized)
3. Verify the Event response is received within 10 seconds.


#### TC 1.6.2 Loading Cellular driver with Reset API 
To verify that after calling the Reset API the Event has received within Threshold Limit.

### Setup
Turn On the Modem and ensure it is turned On.

### Teardown
Turn Off the Modem and ensure it is turned Off.

##### Run
 Repeat the Step 1-3 for 100 times 

1. Call API for Reset and receive RETCODE_OK in 7 seconds
2. Wait for Initialization Event(either Initializing or Initialized)
3. Verify the Event response is received within 10 seconds.

#### TC 1.6.3 Loading Cellular driver with Init API 
To verify that after calling the Modem Init API the Event has received within Threshold Limit.

### Setup
Turn On the Modem and ensure it is turned On.

### Teardown
Turn Off the Modem and ensure it is turned Off. 

##### Run
 Repeat the Step 1-3 for 100 times 

1. Call API for Modem Init and receive RETCODE_OK.
2. Wait for Initialization Event(either Initializing or Initialized)
3. Verify the Event response is received within 10 seconds.

#### TC 1.6.4 Loading Cellular driver with Configure API 
To verify that after calling the Modem Configure API the Event has received within Threshold Limit. 

### Setup
Turn On the Modem and ensure it is turned On.

### Teardown
Turn Off the Modem and ensure it is turned Off.

##### Run
 Repeat the Step 1-4 for 100 times 

1. Initialize the Modem and ensure it is initialized.
2. Call API for Modem Configure.
3. Wait for response and it can be success or failure. 
4. Verify the response is received within 5 seconds.

#### TC 1.6.5 Loading Cellular driver with Connect and Disconnect
To verify that after calling the Modem Connect and Disconnect APIs the Event has received within Threshold Limits.

### Setup
1. Turn On the Modem and ensure it is turned On.
2. Initialize the Modem and ensure it is initialized.
3. Configure the Modem with valid parameters and ensure it is configured.

### Teardown
Turn Off the Modem and ensure it is turned Off.  

##### Run
 Repeat the Step 1-6 for 100 times 

1. Initialize the Modem and ensure it is initialized.
2. Configure the Modem with valid parameters and ensure it is configured.
3. Call API for Network Connect.
4. Wait for Connect Event(either connected or error)
5. Verify the Event response is received within 180 seconds.
6. Call API for Network Disconnect.
7. Verify the Event response is received within 180 seconds.

#### TC 1.6.6 Loading Cellular driver with Socket Open and close API 
To verify that after calling the Socket Open and Socket Close API, it returns response within an expected threshold time limit.

##### Setup 
1. Turn On the Modem and ensure it is turned On.
2. Initialize the Modem and ensure it is initialized.
3. Configure the Modem with valid parameters and ensure it is configured.
4. Connect the Modem with network and ensure it is connected.

##### TearDown
1. Disconnect the Modem from Network
2. Turn Off the Modem and ensure it is turned Off.

##### Run
Repeat the Step 1-6 for 100 times

1. Call API for Socket Open.
2. Wait for the return value of called API and it can be success or failure. 
3. Verify the response is received within 5 seconds.
4. Close the socket.
5. Wait for response and it can be success or failure. 
6. Verify the response is received within 5 seconds.

#### TC 1.6.7 Loading Cellular driver with Socket Open,Send and Close API 
To verify that after calling the Socket Open,Send and Close API, it returns response within an expected threshold time limit.

##### Setup 
1. Turn On the Modem and ensure it is turned On.
2. Initialize the Modem and ensure it is initialized.
3. Configure the Modem with valid parameters and ensure it is configured.
4. Connect the Modem with network and ensure it is connected.

##### TearDown
1. Disconnect the Modem from Network
2. Turn Off the Modem and ensure it is turned Off.

##### Run
Repeat the Step 1-10 for 100 times

1. Call API for Socket Open.
2. Wait for the return value of called API and it can be success or failure. 
3. Verify the response is received within 5 seconds.
4. Call API for Socket send with 185 bytes of data.
5. Wait for response and it can be success or failure.
6. Verify the response is received within 5 seconds 
7. Wait for an Data received from Echo Server.
8. Close the socket.
9. Wait for response and it can be success or failure. 
10. Verify the response is received within 5 seconds.

#### TC 1.6.8 Loading Cellular driver with Socket Open and close API for 3 Socket 
To verify that after calling the Socket Open and Socket Close API, it returns response within an expected threshold time limit.

##### Setup 
1. Turn On the Modem and ensure it is turned On.
2. Initialize the Modem and ensure it is initialized.
3. Configure the Modem with valid parameters and ensure it is configured.
4. Connect the Modem with network and ensure it is connected.

##### TearDown
1. Disconnect the Modem from Network
2. Turn Off the Modem and ensure it is turned Off.

##### Run
1. Repeat the Step 2-9 for 100 times
2. Repeat the steps (3-5) for 3 times.	
3. Call API for Socket Open
4. Wait for the return value of called API and it can be success or failure. 
5. Verify the response is received within 5 seconds.
6. Repeat the steps (7-9) for 3 times.
7. Close the socket.
8. Wait for response and it can be success or failure. 
9. Verify the response is received within 5 seconds.

#### TC 1.6.9 Loading Cellular driver with Socket Continuous Send API 
To verify that after calling the Socket Send API, it returns response within an expected threshold time limit.

##### Setup 
1. Turn On the Modem and ensure it is turned On.
2. Initialize the Modem and ensure it is initialized.
3. Configure the Modem with valid parameters and ensure it is configured.
4. Connect the Modem with network and ensure it is connected.
5. Open the Socket and ensure it is opened.

##### TearDown
1. Close the socket.
2. Disconnect the Modem from Network
3. Turn Off the Modem and ensure it is turned Off.

##### Run
1. Call API for Socket send with 185 bytes of data.
2. Wait for the return value of called API and it can be success or failure. 
3. Verify the response is received within 5 seconds.
4. Repeat the Step 1-3 for 100000 times


#### TC 1.6.10 Loading Cellular driver with continuous Socket Send and receive API 
To verify that after Opening the Socket Send and receive the Data for Multiple Time for all the Valid three Socket

##### Setup 
1. Power ON the Modem
1. Initialize the Modem and ensure it is initialized.
2. Configure the Modem with valid parameters and ensure it is configured.
3. Connect the Modem with network and ensure it is connected.

##### TearDown
1. Disconnect the Modem from Network
2. Power Off The Modem

##### Run
Repeat the Step 1-19 for 100 times

1. Open the Socket 1 
2. Call API for Socket send with 185 bytes of data.
3. Wait for response and it can be success or failure.
4. Verify the response is received within 5 seconds 
5. Wait for an Data received from Echo Server.
6. Repeat the Step 2-5 for 1000 Times.
7. Open the Socket 2 
8. Call API for Socket send with 185 bytes of data.
9. Wait for response and it can be success or failure.
10. Verify the response is received within 5 seconds 
11. Wait for an Data received from Echo Server.
12. Repeat the Step 8-11 for 1000 Times.
13. Open the Socket 3 
14. Call API for Socket send with 185 bytes of data.
15. Wait for response and it can be success or failure.
16. Verify the response is received within 5 seconds 
17. Wait for an Data received from Echo Server.
18. Repeat the Step 14-17 for 1000 Times.
19. Close the socket_1,Socket_2 and Socket_3.


#### TC 1.6.11 Loading Cellular driver with continuous Socket Send and receive API 
To verify that after Opening the Socket Send and receive the Data for Multiple Time for all the Valid three Socket

##### Setup 
1. Power ON the Modem
1. Initialize the Modem and ensure it is initialized.
2. Configure the Modem with valid parameters and ensure it is configured.
3. Connect the Modem with network and ensure it is connected.

##### TearDown
1. Disconnect the Modem from Network
2. Power Off The Modem

##### Run
Repeat the Step 1-19 for 100 times

1. Open the Socket 1 
2. Open the Socket 2 
3. Open the Socket 3
4. Call API for Socket send to Socket1 with 185 bytes of data.
5. Wait for response and it can be success or failure.
6. Verify the response is received within 5 seconds 
7. Wait for an Data received from Echo Server.
8. Call API for Socket send  to Socket2 with 185 bytes of data.
9. Wait for response and it can be success or failure.
10. Verify the response is received within 5 seconds 
11. Wait for an Data received from Echo Server.
12. Repeat the Step 8-11 for 1000 Times.
13. Call API for Socket send to Socket3  with 185 bytes of data.
14. Wait for response and it can be success or failure.
15. Verify the response is received within 5 seconds 
16. Wait for an Data received from Echo Server.
17. Repeat the Step 14-17 for 1000 Times.
19. Close the socket_1,Socket_2 and Socket_3.



#### TC 1.6.12 Loading Cellular driver with Init,Config,Connect,Socket Open,Send Data,Close,Disconnect And Poweroff API 
To verify Init,Config,Connect,Socket Open,Send Data,Close,Disconnect and Poweroff

##### Run
Repeat the Step 1-12 for 100 times

1. Power ON the Modem
2. Initialize the Modem and ensure it is initialized.
3. Configure the Modem with valid parameters and ensure it is configured.
4. Connect the Modem with network and ensure it is connected.
5. Open the Socket 1 
6. Call API for Socket send with 185 bytes of data.
7. Wait for response and it can be success or failure.
8. Verify the response is received within 5 seconds 
9. Wait for an Data received from Echo Server.
10. Close the Socket 1 
11. Disconnect the Modem from Network
12. Power Off The Modem

#### TC 1.6.13 Sending UDP data to a opened socket for every 20 seconds once
To verify that, after sending UDP data to a opened socket, for every 20 seconds once, the send and receive are not affected

##### Setup 
1. Power ON the Modem
2. Initialize the Modem and ensure it is initialized.
3. Configure the Modem with valid parameters and ensure it is configured.
4. Connect the Modem with network and ensure it is connected.

##### TearDown
1. Disconnect the Modem from Network
2. Power Off The Modem

##### Run
Repeat the Step 2-5 for 300 times

1. Open the Socket 1 
2. Call API for Socket send with 4 bytes of data.
3. Wait for response and it can be success or failure.
4. Wait for an Data received from the Server.
5. Provide a delay of 20 seconds
6. Close the opened socket

#### SMS test Case

#### TC 1.6.14: Socket Open then Send SMS  and wait for SMS event and Send UDP packet
This test case tests the reliability of sending a SMS between socket send

### Setup
1. Power ON the Modem
2. Initialize the Modem and ensure it is initialized.
3. Set modem configuration (APN name, User name, Password) which returns SUCCESS
4. Connect to the network and check for SUCCESS
5. Validate for connected event
6. Open a socket, which returns SUCCESS

### Teardown
1. Close the opened socket
2. Disconnect the connection
3. Turn off the modem which returns SUCCESS

### Run
Run below step for 10 times

1. Send a text message to a valid mobile number with message length of 160 bytes of alpha numeric char and validate for return SUCCESS
2. Validate the SMS send event, for successful send
3. Send 100 bytes of UDP data to the opened socket and validate the returns SUCCESS
4. Validate for the received event and data.
5. Validate manually the SMS is received on mobile and that the message content is the same as the sent one.(Not consider in jenkins report)
