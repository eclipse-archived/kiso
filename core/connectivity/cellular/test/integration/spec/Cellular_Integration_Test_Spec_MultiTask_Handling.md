Cellular Integration MultiTask Handling Test Specification Cellular Driver
===============================================================

# Test Section 1: "MultiTask Handling Test for Cellular Driver"
## Description
This test section comprises the test cases of MultiTask Handling Test to validate the Lean B2CAP frame corruption is not occured in Cellular Driver when Multitask is running.
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

## Test Suite 1.11: "MultiTask Handling Testcases for Cellular Driver"
  
### Description
The MultiTask Handling Testcases related to Cellular Driver are validated.

### Setup
Turn On the Modem and ensure it is turned On.

### Teardown
Turn Off the Modem and ensure it is turned Off. 

### Test Cases

#### TC 1.11.1 MultiTask Handling Test with Config API
To verify that Lean B2CAP frame is not corrupted after calling the Cellular driver APIs by the tasks which are running parallel.  

### Setup
Initialize the Network and ensure the Modem is initialized.

##### Run

1. Create a task with the delay of 2MS and ensure it returns pdPASS and Handle is not NULL.
2. In the created task call Cellular Driver API (Example: Config API) infinitely.
3. If Event error occured or API returns failure then suspend the task.
4. Create another task with the delay of 1MS and ensure it returns pdPASS and Handle is not NULL. 
5. In the created task call Cellular Driver API (Example: Config API) infinitely.
6. If Event error occured or API returns failure then suspend the task.
7. If no error occured then API will be called infinitely. To stop this, test case is configured with the timeout (Example : 10 minutes)
8. This timeout ensures no Lean B2CAP corruption occured till that timeout.
9. Semaphore is used to monitor the timeout and exit from the task when Event error occured or API returns failure
10. Created task is deleted. 


