# Test Section 3: "Essentials"

## Test Suite 3.3: I2C_Selfcontained_Test
### Description

* Purpose
  + Test the I2C transfer functionality

* Test participants:
  - Device under Test (DUT): Stm32 - Bike Sensor Board (BSE)
  - Test Coordinator: PC

### Test Setup
The test setup consists of the test coordinator and 1 test participant

* Test Coordinator (on PC)
* The DUT is connected to the PC via UART which is the Test Coordination Channel. The embedded C testling code is flashed onto the DUT.
* The DUT is connected to a 12 V power source.
* For this selfcontained test, no external wires are necessary and only BSE-internal Sensors are polled via I2C.

On the BSE testling, the initialization sequence setups the I2C Clock and the I2C GPIOs:

* I2C SCL line: GPIO_PIN_14, Bank GPIOG, mode GPIO_PULLUP, GPIO_SPEED_LOW
* I2C SDA line: GPIO_PIN_13, Bank GPIOG, mode GPIO_PULLUP, GPIO_SPEED_LOW
* Alternative function: GPIO_AF4_I2C1
* I2C Clock: PCLK1
* enable GPIO bank G clock
* power up GPIO bank G (PWR_CR2_IOSV register)
* IRQs I2Cx_ER_IRQn and I2Cx_EV_IRQn are enabled

### Teardown

No special teardown

### Test Cases

#### TC 3.3.1 TestCase_I2C_initialize
##### Setup
No separate Test Case Setup required

##### Run
1. For all present I2C Interfaces: BCDS_I2C1, BCDS_I2C2

	* Call API: I2C_getVersion(), expect version
	* Call API: I2C_getCapabilities(), expect only "address10bit" capability.
	* Call API: I2C_initialize(), expect RETCODE_OK
	* Call API: I2C_powerControl(PERIPHERALS_POWER_STATE_FULL), expect RETCODE_OK
	* Call API: I2C_powerControl(PERIPHERALS_POWER_STATE_OFF), expect RETCODE_OK
	* Call API: I2C_powerControl(PERIPHERALS_POWER_STATE_FULL), expect RETCODE_OK
	* Call API: I2C_control(BCDS_I2C_BUS_SPEED,BCDS_I2C_BUS_SPEED_STANDARD), expect RETCODE_OK
	* Call API: I2C_control(BCDS_I2C_BUS_SPEED,BCDS_I2C_BUS_SPEED_FAST), expect RETCODE_OK
	* Call API: I2C_control(BCDS_I2C_BUS_SPEED,BCDS_I2C_BUS_SPEED_FAST_PLUS), expect RETCODE_OK
	* Call API: I2C_control(BCDS_I2C_BUS_SPEED,BCDS_I2C_OWN_ADDRESS,0x42), expect RETCODE_OK
	* Call API: I2C_getStatus(), expect only flag "Initialized"
	* Call API: I2C_uninitialize(), expect RETCODE_OK

2. If all as expected, return Success.

##### TearDown
No separate Test Case Tear Down required

#### TC 3.3.2 TestCase_I2C_master_BMI160

On the BSE boardThe inertial measurement unit BMI160 is visible via BCDS_I2C1 on address 0x68.
To read a register, the register ID is first written via I2C to BMI160.
Maximum supported clock frequency is 1000khz, i.e., BCDS_I2C_BUS_SPEED_FAST_PLUS.

##### Setup
Call API: I2C_initialize(), expect RETCODE_OK

##### Run
1. Transmit 1 byte value 0 to BMI160 to access the CHIPID register:

	* Call API: I2C_masterTransmit(BCDS_I2C1 ,0x68, 0, 1, 0);
	* Expect RETCODE_OK;
	* Expect "TransferDone" Callback.
2. Read 1 byte CHIPID value from BMI160:

	* Call API: I2C_masterReceive(BCDS_I2C1 ,0x68, &chipid, 1, 0);
	* Expect RETCODE_OK;
	* Expect "TransferDone" Callback.
	* Expect CHIPID to be ( 0b11010001 = 0xD1 )
3. Transmit 1 byte value 0x04 to BMI160 to access the DATA registers:

	* Call API: I2C_masterTransmit(BCDS_I2C1 ,0x68, 0x04, 1, 0);
	* Expect RETCODE_OK;
	* Expect "TransferDone" Callback.
4. Read 20 byte DATA register values from BMI160:

	* Call API: I2C_masterReceive(BCDS_I2C1 ,0x68, datareg, 20, 0);
	* Expect RETCODE_OK;
	* Expect "TransferDone" Callback.
	* Expect at least one DATA registers values to contain values different from 0
5. If all as expected, return Success.

##### TearDown
Call API: I2C_uninitialize(), expect RETCODE_OK

## Test Suite 3.4: I2C_Arduino_Test
### Description

1. Purpose:

	* Test the I2C transfer functionality with an Arduino counterpart that allows precise master AND slave communication tests.

2. Test participants:

	* Test Coordinator: PC
	* Device under Test (DUT): Stm32 - Bike Sensor Board (BSE)
	* Arduino Due - the arduino code is contained in the Peripherals/test/integration/Testling/Tests/testSuiteI2c/Arduino_I2C_final_Testing_Code folder.

### Setup
The test setup consists of the test coordinator and 2 test participants

1.	Test Coordinator (on PC)
2.	The DUT is connected to the PC via UART which is the Test Coordination Channel. The embedded C testling code is flashed onto the DUT.
3.	The DUT is connected to a 12 V power source.
4.	The Arduino is connected to the PC via UART to flash and power the Arduino board.
5.	The Arduino testling code is flashed onto the Arduino.
6.	The I2C bus I2C1 of the BSE board is led out and connected to the Arduino Due board such that:

	* BSE-PG14/C6 SCL line is connected to Arduino GPIO pin 21
	* BSE-PG13/C7 SDA line is connected to Arduino GPIO pin 20
	* a common ground line between the BSE board and the Arduino board is connected

On the BSE testling, the initialization sequence setups the I2C Clock and the I2C GPIOs:

7.	I2C SCL line: GPIO_PIN_14, Bank GPIOG, mode GPIO_PULLUP, GPIO_SPEED_LOW
8.	I2C SDA line: GPIO_PIN_13, Bank GPIOG, mode GPIO_PULLUP, GPIO_SPEED_LOW
9.	Alternative function: GPIO_AF4_I2C1
10.	I2C Clock: PCLK1
11.	Enable GPIO bank G clock
12.	Power up GPIO bank G (PWR_CR2_IOSV register)
13.	IRQs I2Cx_ER_IRQn and I2Cx_EV_IRQn are enabled

### Teardown
No special teardown

### Test Cases

#### TC 3.4.1 TestCase_I2C_master

The arduino wire library has a maximum i2c buffer size of 32 bytes - so larger data streams cannot be tested.

##### Setup
1.	Call API: I2C_initialize(), expect RETCODE_OK
2.	Send testcase ID (1) to arduino and expect acknowledge byte (1)

	* Call API: I2C_masterTransmit(BCDS_I2C1, ARDUINO_I2C_ADDRESS, 1, 1, 0);
	* Call API: I2C_masterRecieve (BCDS_I2C1, ARDUINO_I2C_ADDRESS, &arduinoAcknowledges, 1, 0);
	* Expect arduinoAcknowledges == 1
3.	Call API: I2C_uninitialize(), expect RETCODE_OK
4.	Call API: I2C_initialize(), expect RETCODE_OK

##### Run
1.	In Master mode send 1 byte to Arduino and receive it back:

	* Call API: I2C_masterTransmit(BCDS_I2C1, ARDUINO_I2C_ADDRESS, TESTBYTE, 1, 0);
		* Expect RETCODE_OK;
		* Expect "TransferDone" Callback.
	* Call API: I2C_masterReceive(BCDS_I2C1, ARDUINO_I2C_ADDRESS, &receiveBuffer, 1, 0);
		* Expect RETCODE_OK;
		* Expect "TransferDone" Callback.
	* Verify (receiveBuffer == TESTBYTE)
2.	In Master mode send 32 bytes to Arduino and receive them back

	* uint8_t TEST32BYTES[] = {42, 43, ..., 73 };
	* Call API: I2C_masterTransmit(BCDS_I2C1, ARDUINO_I2C_ADDRESS, TEST32BYTES, 32, 0);
	* Expect RETCODE_OK;
	* Expect "TransferDone" Callback.
3.	Call API: I2C_masterReceive(BCDS_I2C1, ARDUINO_I2C_ADDRESS, receiveBuffer32, 32, 0);

	* Expect RETCODE_OK;
	* Expect "TransferDone" Callback.
4.	Verify (receiveBuffer32 identical to TEST32BYTES)
5.	Return Success.

##### TearDown
Call API: I2C_uninitialize(), expect RETCODE_OK

#### TC 3.4.2 TestCase_I2C_slave_woCallback

##### Setup
1.	Call API: I2C_initialize(), expect RETCODE_OK
2.	Send testcase ID (2) to arduino and expect acknowledge byte (1)

	* Byte to call API: I2C_masterTransmit(BCDS_I2C1, ARDUINO_I2C_ADDRESS, 2, 1, 0);
	* Byte to call API: I2C_masterRecieve (BCDS_I2C1, ARDUINO_I2C_ADDRESS, &arduinoAcknowledges, 1, 0);
	* Expect arduinoAcknowledges == 1
3.	Call API: I2C_uninitialize(), expect RETCODE_OK
4.	Call API: I2C_initialize(), expect RETCODE_OK
5.	Set own slave address to 0x42

##### Run

1.	In Slave mode, wait for Arduino to request 1 byte, then wait until Arduino sends it back

	* Call API: I2C_slaveTransmit(BCDS_I2C1, TESTBYTE, 1);
		* expect RETCODE_OK;
		* expect "TransferDone" Callback.
	* Call API: I2C_slaveReceive(BCDS_I2C1, &receiveBuffer, 1);
		* expect RETCODE_OK;
		* expect "TransferDone" Callback.
	* Verify (receiveBuffer == TESTBYTE)
2.	In Slave mode send 32 bytes to Arduino and receive them back

	* uint8_t TEST32BYTES[] = {42, 43, ..., 73 };
	* Call API: I2C_slaveTransmit(BCDS_I2C1, ARDUINO_I2C_ADDRESS, TEST32BYTES, 32);
		* expect RETCODE_OK;
		* expect "TransferDone" Callback.
	* Call API: I2C_slaveReceive(BCDS_I2C1, ARDUINO_I2C_ADDRESS, receiveBuffer32, 32);
		* expect RETCODE_OK;
		* expect "TransferDone" Callback.
	* Verify (receiveBuffer32 identical to TEST32BYTES)
3.	Return Success.

##### TearDown

Call API: I2C_uninitialize(), expect RETCODE_OK

#### TC 3.4.3 TestCase_I2C_slave_callbackDriven

##### Setup
1.	Call API: I2C_initialize(), expect RETCODE_OK
2.	Send testcase ID (3) to arduino and expect acknowledge byte (1)

	* Call API: I2C_masterTransmit(BCDS_I2C1, ARDUINO_I2C_ADDRESS, 3, 1, 0);
	* Call API: I2C_masterRecieve (BCDS_I2C1, ARDUINO_I2C_ADDRESS, &arduinoAcknowledges, 1, 0);
	* Expect arduinoAcknowledges == 1
3.	Call API: I2C_uninitialize(), expect RETCODE_OK

##### Run
1.	Define uint8_t TEST32BYTES[] = {42, 43, ..., 73 };
2.	Call API: I2C_initialize(), expect RETCODE_OK
3.	Set own slave address to 0x42
4.	Register new callback event handler that:

	* Answers a SlaveTransmit request with: I2C_slaveTransmit(BCDS_I2C1, TEST32BYTES, 32);
		* expect RETCODE_OK;
	* Answers a SlaveReceive request with: I2C_slaveReceive(BCDS_I2C1, receiveBuffer32, 32);
		* expect RETCODE_OK;
	* Updates 4-state-variable from waitForSlaveTransmit(0) to waitForSlaveReceive(1) to done(2) or alternatively error(3)
5.	Wait with timeout until 4-state-variable goes through the states

	* WaitForSlaveTransmit(0)
	* WaitForSlaveReceive(1)
	* Done(2)
6.	Make sure error(3) is not reached or return error-report.
7.	When state done(2) is reached, verify (receiveBuffer32 identical to TEST32BYTES)
8.	Return Success.

##### TearDown

Call API: I2C_uninitialize(), expect RETCODE_OK

#### TC 3.4.4 TestCase_I2C_higherSpeeds

##### Setup
1.	Call API: I2C_initialize(), expect RETCODE_OK
2.	Send testcase ID (4) to arduino and expect acknowledge byte (1)

	* Call API: I2C_masterTransmit(BCDS_I2C1, ARDUINO_I2C_ADDRESS, 4, 1, 0);
	* Call API: I2C_masterRecieve (BCDS_I2C1, ARDUINO_I2C_ADDRESS, &arduinoAcknowledges, 1, 0);
	* Expect arduinoAcknowledges == 1
3.	Call API: I2C_uninitialize(), expect RETCODE_OK
4.	Call API: I2C_initialize(), expect RETCODE_OK

##### Run
1.	uint8_t TEST32BYTES[] = {42, 43, ..., 73 };
2.	Set the bus speeds [Standard Speed (100kHz), Fast Speed (400kHz),  Fast+ Speed  (1MHz)] try:

	* In Master mode send 32 bytes to Arduino and receive them back
		* call API: I2C_masterTransmit(BCDS_I2C1, ARDUINO_I2C_ADDRESS, TEST32BYTES, 32, 0);
			* expect RETCODE_OK;
			* expect "TransferDone" Callback.
	* Call API: I2C_masterReceive(BCDS_I2C1, ARDUINO_I2C_ADDRESS, receiveBuffer32, 32, 0);
		* expect RETCODE_OK;
		* expect "TransferDone" Callback.
	* Verify (receiveBuffer32 identical to TEST32BYTES)
3.	Return Success.

##### TearDown
Call API: I2C_uninitialize(), expect RETCODE_OK
