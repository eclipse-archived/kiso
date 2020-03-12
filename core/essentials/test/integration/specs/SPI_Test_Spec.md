Integration Test Specification for SPI

# Test Suite 2: "Test Specification of SPI"

## Description

* Purpose
	* Test the Peripheral SPI functionality
* Test participants which are involved
	* Test participant :- BSE
* Test environment and setup
	* BSE device be connected to the PC via USB Cable.
	* A PC has a test coordinator. 
* Setup for Flashing-Manual.
 * SPI IntegrationTestApp binary file into BSE device
 * once the flashing is done
 * It automatically reset the device and IntegrationTestApps Boots-up

## Setup
The test setup consists of the test coordinator and 2 test participants:

*	test coordinator	(on PC)
*	The test device BSE with the embedded C test participant

The test device is connected to the test coordinator on the PC via UART as a Test Coordination Channel.


### Parameters
No special parameters

## Teardown 
* Deinitialize the SPI Port with SPI_deinitialize.

## Test Suite 1.2: Non initialized - error test
### Description
* Test the SPI error cases for non initialized case

### Setup
* No required separate Test Suite Setup for this example.

### Teardown 
* Not required separate Test Suite Tear Down for this example.
	
### Test Cases

#### TC 1.1.1 TestCase_SPI_Control_Error_unitialized
* Test case Setup
	* Deinitialize the SPI.
* Test Case Run
	* Parameter passed: valid handle, valid control, valid argument 
	* Run SPI_Control API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.
		
#### TC 1.1.2 TestCase_SPI_Send_Error_unitialized
* Test case Setup
	* Deinitialize the SPI.
* Test Case Run
	* Parameter passed: valid handle, valid sendValue, valid bufferSize
	* Run SPI_Send API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.
		
#### TC 1.1.3 TestCase_SPI_Receive_Error_unitialized
* Test case Setup
	* Deinitialize the SPI.
* Test Case Run
	* Parameter passed: valid handle, valid receiveValue, valid bufferSize
	* Run SPI_Receive API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* No required separate Test Case Tear Down for this example.
		
#### TC 1.1.4 TestCase_SPI_Transfer_Error_unitialized
* Test case Setup
	* Deinitialize the SPI.
* Test Case Run
	* Parameter passed: valid handle, valid sendValue, valid receiveValue, valid bufferSize
	* Run SPI_Transfer API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* No required separate Test Case Tear Down for this example.

##################################################################################################################################	
	

#### TC 1.1.5 TestCase_SPI_Send_Error_invalidPort
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: invalid port, valid sendValue, valid bufferSize
	* Run SPI_Send API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* No required separate Test Case Tear Down for this example.

#### TC 1.1.6 TestCase_SPI_Send_Error_invalidSendValue
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: valid port, invalid sendValue, valid bufferSize
	* Run SPI_Send API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.
	
#### TC 1.2.3 TestCase_SPI_Send_Error_invalidBufferSize
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: valid port, valid sendValue, invalid bufferSize
	* Run SPI_Send API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.	

#### TC 1.2.4 TestCase_SPI_Receive_Error_invalidPort
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: invalid port, valid receiveValue, valid bufferSize
	* Run SPI_Receive API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.
	
#### TC 1.2.5 TestCase_SPI_Receive_Error_invalidReceiveValue
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: valid port, invalid receiveValue, valid bufferSize
	* Run SPI_Receive API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.
	
#### TC 1.2.6 TestCase_SPI_Receive_Error_invalidReceiveValue
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: valid port, invalid receiveValue, valid bufferSize
	* Run SPI_Receive API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.	

#### TC 1.2.7 TestCase_SPI_Transfer_Error_invalidPort
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: invalid port, valid sendValue, valid receiveValue, valid bufferSize
	* Run SPI_Receive API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.	
	
#### TC 1.2.8 TestCase_SPI_Transfer_Error_invalidSendValue
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: valid port, invalid sendValue, valid receiveValue, valid bufferSize
	* Run SPI_Receive API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.	
	
#### TC 1.2.9 TestCase_SPI_Transfer_Error_invalidReceiveValue
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: valid port, valid sendValue, invalid receiveValue, valid bufferSize
	* Run SPI_Receive API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.		
	
#### TC 1.2.10 TestCase_SPI_Transfer_Error_invalidBufferSize
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: valid port, valid sendValue, valid receiveValue, invalid bufferSize
	* Run SPI_Receive API
* Expected return value from the API: RETCODE_FAILURE
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.		

	#### TC 1.2.11 TestCase_SPI_Control_Error_invalidParameter
* Test case Setup
	* Initialize the SPI Port.
* Test Case Run
	* Parameter passed: valid handle, invalid control, invalid argument 
	* Run SPI_Control API
	* Expected return value from the API: RETCODE_NOT_SUPPORTED
* Test case Tear Down
	* Not required separate Test Case Tear Down for this example.
	
##################################################################################################################################	
		
## Test Suite 1.3: functional tests
### Description
* Currently it is not possible to have functional SPI tests, because there is neither implemented bus
partner for testing nor a possibility to test the GPIO pins. Instead, the (positive tests) 
functionality will be tested by testing the external flash which is connected via SPI.  
That means, if flash is working, SPI is working too. But that also means, if flash does not work, 
its unclear if the error is in the flash driver or in the SPI driver.
