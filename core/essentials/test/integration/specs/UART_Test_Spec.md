# Test Entry 3: "Essentials"
## Test Suite 3.1: UART
### Description
* This suite aims at testing the basic functionality of the UART APIs

### Setup
The test setup consists of the test coordinator and 1 test participant

* Test Coordinator (on PC)
* The DUT is connected to the PC via UART which is the Test Coordination Channel. The embedded C testling code is flashed onto the DUT.

### TearDown

No special teardown

### Test Cases

#### TC 3.1.1:TestCase_Uart_getStatus
##### Setup
No special setup

##### Run
1. Invoke the Uart_getStatus API
2. Expected return value from the API: HAL_UART_STATE_READY

##### TearDown 
No special teardown

#### TC 3.1.2:TestCase_Uart_getVersion
##### Setup
No special setup

##### Run
1. Invoke the Uart_getVersion API
2. API should return the version number of the UART

##### TearDown 
No special teardown 

#### TC 3.1.3:TestCase_Uart_getCapabilities
##### Setup
No special setup

##### Run
1. Invoke the Uart_getCapabilities API
2. API should return the capabilities supported by the UART

##### TearDown 
No special teardown


#### TC 3.1.4:TestCase_Uart_initialize
##### Setup
1. Invoke the Uart_control API with the following parameters,

       * BCDS_Uart1
       * BCDS_UART_MODE_ASYNCHRONOUS | BCDS_UART_DATA_BITS_8 | BCDS_UART_PARITY_NONE | BCDS_UART_STOP_BITS_1 | BCDS_UART_FLOW_CONTROL_NONE
       * 115200 (Baud rate)
2. Expected return value from the API: RETCODE_SUCCESS
       
##### Run
1. Invoke the Uart_initialize API
2. Expected return value from the API: RETCODE_SUCCESS

##### TearDown 
1. Invoke the Uart_uninitialize API 
2. Expected return value from the API: RETCODE_SUCCESS

#### TC 3.1.5:TestCase_Uart_control
##### Setup
No special setup

##### Run
1. Invoke the Uart_control API with the following parameters,

       * BCDS_Uart1
       * BCDS_UART_MODE_ASYNCHRONOUS | BCDS_UART_DATA_BITS_8 | BCDS_UART_PARITY_NONE | BCDS_UART_STOP_BITS_1 
       * 115200 (Baud rate)      
2. Expected return value from the API: RETCODE_SUCCESS

##### TearDown 
No special teardown 


#### TC 3.1.6:TestCase_Uart_send
##### Setup
1. Invoke the Uart_control API with the following parameters,

       * BCDS_Uart1
       * BCDS_UART_MODE_ASYNCHRONOUS | BCDS_UART_DATA_BITS_8 | BCDS_UART_PARITY_NONE | BCDS_UART_STOP_BITS_1 | BCDS_UART_FLOW_CONTROL_NONE
       * 115200 (Baud rate)
2. Expected return value from the API: RETCODE_SUCCESS
3. Invoke the Uart_initialize API
4. Expected return value from the API: RETCODE_SUCCESS

##### Run
1. Invoke the Uart_send API and send the data "BikeSensor" 
2. Expected return value from the API: RETCODE_SUCCESS

##### TearDown 
1. Invoke the Uart_uninitialize API
2. Expected return value from the API: RETCODE_SUCCESS

#### TC 3.1.7:TestCase_Uart_receive
##### Setup
1. Invoke the Uart_control API with the following parameters,

       * BCDS_Uart1
       * BCDS_UART_MODE_ASYNCHRONOUS | BCDS_UART_DATA_BITS_8 | BCDS_UART_PARITY_NONE | BCDS_UART_STOP_BITS_1 
       * 115200 (Baud rate)
2. Expected return value from the API: RETCODE_SUCCESS
3. Invoke the Uart_initialize API
4. Expected return value from the API: RETCODE_SUCCESS

##### Run
1. Invoke the Uart_receive API 
2. Expected return value from the API: RETCODE_FAILURE 

/* @todo: Expectation is a failure because no device is sending data to the DUT UART. Beaglebone has to be integrated for validating this test case */

##### TearDown 
1. Invoke the Uart_uninitialize API
2. Expected return value from the API: RETCODE_SUCCESS


#### TC 3.1.8:TestCase_Uart_uninitialize
##### Setup
1. Invoke the Uart_control API with the following parameters

       * BCDS_Uart1
       * BCDS_UART_MODE_ASYNCHRONOUS | BCDS_UART_DATA_BITS_9 | BCDS_UART_PARITY_NONE | BCDS_UART_STOP_BITS_1 
       * 9600 (Baud rate)
2. Expected return value from the API: RETCODE_SUCCESS
3. Invoke the Uart_initialize API 
4. Expected return value from the API: RETCODE_SUCCESS

##### Run
1. Invoke the Uart_uninitialize API
2. Expected return value from the API: RETCODE_SUCCESS

##### TearDown 
No special teardown

#### TC 3.1.9:TestCase_Uart_powerControl_full
##### Setup
No special setup

##### Run
1. Invoke the Uart_powerControl API with the following parameter

       * PERIPHERALS_POWER_STATE_FULL
2. Expected return value from the API: RETCODE_SUCCESS

##### TearDown 
No special teardown

#### TC 3.1.10:TestCase_Uart_powerControl_low
##### Setup
No special setup

##### Run
1. Invoke the Uart_powerControl API with the following parameter

       * PERIPHERALS_POWER_STATE_LOW
2. Expected return value from the API: RETCODE_FAILURE

##### TearDown 
No special teardown

#### TC 3.1.11:Testcase_Uart_initialize_without_control
##### Setup
No special setup

##### Run
1. Invoke the Uart_initialize API
2. Expected return value from the API:RETCODE_SUCCESS

##### TearDown 
1. Invoke the Uart_uninitialize API
2. Expected return value from the API: RETCODE_SUCCESS

#### TC 3.1.12:Testcase_Uart_send_without_initialize
##### Setup
1. Invoke the Uart_control API with the following parameters,

       * BCDS_Uart1
       * BCDS_UART_MODE_ASYNCHRONOUS | BCDS_UART_DATA_BITS_8 | BCDS_UART_PARITY_NONE | BCDS_UART_STOP_BITS_1 | BCDS_UART_FLOW_CONTROL_NONE
       * 115200 (Baud rate)
2. Expected return value from the API: RETCODE_SUCCESS

##### Run
1. Invoke the Uart_send API and send the data "Integration testing"
2. Expected return value from the API: RETCODE_FAILURE

##### TearDown
No special teardown

#### TC 3.1.13:Testcase_Uart_receive_without_initialize
##### Setup
1. Invoke the Uart_control API with the following parameters,

       * BCDS_Uart1
       * BCDS_UART_MODE_ASYNCHRONOUS | BCDS_UART_DATA_BITS_8 | BCDS_UART_PARITY_NONE | BCDS_UART_STOP_BITS_1 | BCDS_UART_FLOW_CONTROL_NONE
       * 115200 (Baud rate)
2. Expected return value from the API: RETCODE_SUCCESS

##### Run
1. Invoke the Uart_receive API 
2. Expected return value from the API: RETCODE_FAILURE

##### TearDown 
No special teardown