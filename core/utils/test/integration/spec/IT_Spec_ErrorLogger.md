Test Suite Errorlogger Tests
===========================================
#Description
This test is to check the functionality of Errorlogger file API's
# Global requirement
The test requires the test coordinator and only one participant:

* **Test coordinator(TC)** = PC
* **Test participant(TP)** = ErrorLogger Software module

The test participant is connected to the PC via USB as coordination channel for carbon platform.

## Hardware required
* efm32 hardware
* Jig Board + USB (LEUART)
* J-Link lite (debugger)
## Software required
* Embedded test software 
* efm32 Test executor software

#Test Section Name: ErrorLogger

###Test Section ID :1
## Test Section Setup
No special Setup
## Test Section Teardown
No special teardown

# Test Suite

####Test Suite ID:1 ( TC will assign the Suite ID )
## Test Suite Setup
Initialize the errorlogger module.
## Test Suite Teardown
No special teardown

## Test Cases
### TC 1:Logging a single error
    Setup:
    	  No special setup
	Run:
		1. A single error is logged in to the error logger module by calling ErrorLogger_LogError API
		2. Successful writing returns RETCODE_SUCCESS
		3. Error message will be verified by comparing obtained result with the stored data in the respective storage medium.
	Teardown:
    	   No special teardown

#### TC 2: Logging the multiple errors (10)
    Setup:
           Clear all the errors.
	Run:
		1. Multiple errors are logged into error logger module by calling ErrorLogger_LogError API
		2. Successful logging returns RETCODE_SUCCESS
		3. Error message will be verified by comparing obtained result with the stored data in the respective storage medium.
	Teardown:
    	No special teardown

#### TC 3: Clearing All Error Messages
    Setup:
    	   No special setup
	Run:
		1. Multiple Errors are logged into the error logger module by calling ErrorLogger_LogError API
		2. Successful logging returns RETCODE_SUCCESS
		3. Error messages will be verified by comparing obtained result with userapge fieds.
		4. Now, ErrorLogger_ClearAllErrorLogs API is called. 
		5. Error messages are cleared and will be verified by comparing obtained result with userpage. 
	Teardown:
    	   No special teardown

#### TC 4: Get the last Error Log
   	Setup:
    	   No special setup
   	Run:
		1. A single error is logged in to the error logger module by calling ErrorLogger_LogError API
		2. Successful writing returns RETCODE_SUCCESS
		3. Now, ErrorLogger_GetLastErrorLog returns the error which we logged into the error logger module. 
		4. Multiple Errors are logged into the error logger module by calling ErrorLogger_LogError API
		5. Successful writing returns RETCODE_SUCCESS
		6. Now, ErrorLogger_GetLastErrorLog returns the error which we last logged into the error logger module.
 	Teardown:
    	No special teardown 
	

#### TC 5: Check where particular Error is available in log
 	Setup:
    	   No special setup	
	Run:
		1. Errors are cleared by calling ErrorLogger_ClearAllErrorLogs. 
		2. Now check Error a pariticular error in error logger module. 
		3. ErrorLogger_HasError API returns RETCODE_FAILURE. 
		4. A single error say "Periherial Error (say 3456)" is logged into the system. 
		5. Now check again for the logged error in error logger module.
		6. ErrorLogger_HasError API returns RETCODE_OK
	Teardown:
    	No special teardown 

#### TC 6: Count number of error messages
 	Setup:
    	    Errors are cleared by calling ErrorLogger_ClearAllErrorLogs.	
	Run:
		1. Errors are cleared by calling ErrorLogger_ClearAllErrorLogs. 
		2. Now calling ErrorLogger_GetTotalErrors API returns 0
		3. And calling ErrorLogger_GetCurrentErrors API returns 0
		4. A single error is logged in to the error logger module by calling ErrorLogger_LogError API
		5. Now calling ErrorLogger_GetTotalErrors API returns 1
		6. And calling ErrorLogger_GetCurrentErrors API returns 1
		7. Multiple errors are logged (say 5 errors which is half of the Maximum Errors "Error logger module" can hold).
		8. Now calling ErrorLogger_GetTotalErrors API returns 6
		9. And calling ErrorLogger_GetCurrentErrors API returns 6
		10. Multiple errors are logged (say 10 errors)
		11. Now calling ErrorLogger_GetTotalErrors API returns 16
		12. And calling ErrorLogger_GetCurrentErrors API returns 10
	Teardown:
    	No special teardown 

#### TC 7: Get the error at particular index
 	Setup:
    	   Errors are cleared by calling ErrorLogger_ClearAllErrorLogs.
	Run:
		1. Now try to get error at index 1 by calling ErrorLogger_GetErrorAt API
		2. It returns with RETCODE_OUT_OF_RESOURCES code
		3. Now trying to get error at index ERRORLOGGER_MAX_ENTRY + 10. 
		4. It returns with RETCODE_INVALID_PARAM
		4. Now Multiple errors are logged (say 5 errors)
		6. Now try to get the error at index 4 by calling ErrorLogger_GetErrorAt API
		7. It returns with RETCODE_OK and ErrorLogger_LogEntry_T *LogEntry is filled with actual error
		8. Now try to get error at index 8 by calling ErrorLogger_GetErrorAt API
	    9.It returns with RETCODE_OUT_OF_RESOURCES code 
	Teardown:
    	Clear all the errors

#### TC 8 Persistence of error logs during reboot scenario
 	Setup:
    	    Errors are cleared by calling ErrorLogger_ClearAllErrorLogs.
	Run:
		1. Errors are cleared by calling ErrorLogger_ClearAllErrors Logs
		2. Now the systems is rebooted. 
		3. Called ErrorLogger_Init API which returns RETCODE_OK
		4. Now calling ErrorLogger_GetTotalErrors API returns 0
		5. And calling ErrorLogger_GetCurrentErrors API returns 0
		6. A single error is logged in to the error logger module by calling ErrorLogger_LogError API
		7. Now the systems is rebooted
		8. Now calling ErrorLogger_GetTotalErrors API returns 1
	Teardown:
    	Clear all the errors

