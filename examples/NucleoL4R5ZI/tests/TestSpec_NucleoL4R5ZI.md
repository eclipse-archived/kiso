# Test Section 1: NucleoL4R5ZI Manual Tests

## Test Suite 1.1: Test Led Features

### Description
To verify the Led Features

### Test Cases

### TC 1.1.1: Test Led Blink functionalities

#### Description
To verify the Led Blink functionalities

#### Setup
1. Build Led Application to generate bin file.

#### Run
1. Open the STM32 ST-LINK Utility Software.
2. Select the binary file to flash the software into the Nucleo-L4R5ZI device
3. Click Target option and Select "Program and verify"
4. Download dialog box opens, Select "Reset after programming" option and Click "Start"
5. Close the STM32 ST-LINK Utility Software, Once Download dialog box closes
     * Expected:
     * Every 1 second, Red, Blue and Green Led blinks.


#### TearDown
1. Open the STM32 ST-LINK Utility Software.
2. Click "Full Chip Erase" icon. Select "OK" option in STM32 ST-LINK Utility Dialog box to perform full chip erase.
3. Once the Flash memory erase completed. Make sure that Red, Blue and Green Led blink stops.
----------


## Test Suite 1.2: Test Uart Features

### Description
To verify the Uart Features

### Test Cases

### TC 1.2.1: Test Uart functionalities

#### Description
To verify the Uart functionalities

#### Setup
1. Build Uart Application to generate bin file.
2. Connect PC and Nucleo-L4R5ZI board via Uart.

#### Run
1. Open the STM32 ST-LINK Utility Software.
2. Select the binary file to flash the software into the Nucleo-L4R5ZI device
3. Click Target option and Select "Program and verify"
4. Download dialog box opens, Select "Reset after programming" option and Click "Start"
5. Close the STM32 ST-LINK Utility Software, Once Download dialog box closes
6. Open Hterm Software, Select the comport and Click "Connect". 
     * Expected:
     * Every 1 second, Uart data prints.
7. Send any command in Hterm Software.
     * Expected:
     * Uart data print should stop


#### TearDown
1. Disconnect the comport and Close the hterm software.
----------

## Test Suite 1.3: Test SPI Features

### Description
To verify the SPI Features

### Test Cases

### TC 1.3.1: Test SPI communication between Nucleo-L4R5ZI board and BMA255 Sensor

#### Description
To Verify SPI communication between Nucleo-L4R5ZI board and BMA255 Sensor

#### Setup
1. Connect SPI lines of BMA255 Sensor and Nucleo-L4R5ZI board by Connecting wires.
2. Pinout reference for SPI1.
    * Signal Name	-	STM32 Pin
    * SPI_A_SCK		-	PA5
    * SPI_A_MISO	-	PA6
    * SPI_A_MOSI	-	PA7
    * SPI_A_CS		-	PD14 
3. Build SPI Application(i.e., Read BMA255 Chip id and Send data via UART for every 5second) to generate bin file. 
4. Connect PC and Nucleo-L4R5ZI board via Uart.

#### Run
1. Open the STM32 ST-LINK Utility Software.
2. Select the binary file to flash the software into the Nucleo-L4R5ZI device
3. Click Target option and Select "Program and verify"
4. Download dialog box opens, Select "Reset after programming" option and Click "Start"
5. Close the STM32 ST-LINK Utility Software, Once Download dialog box closes
6. Open Hterm Software, Select the comport and Click "Connect". 
     * Expected:
     * Every 5seconds, BMA255 Chip id (i.e., 0xFA) must be printed in UART.


#### TearDown
1. Disconnect the comport and Close the hterm software.
----------

## Test Suite 1.4: Test I2C Features

### Description
To verify the I2C Features

### Test Cases

### TC 1.4.1: Test I2C communication between Nucleo-L4R5ZI board and BMA280 Sensor

#### Description
To Verify I2C communication between Nucleo-L4R5ZI board and BMA280 Sensor

#### Setup
1. Connect I2C lines of BMA280 Sensor and Nucleo-L4R5ZI board by Connecting wires.
2. Build I2C Application(i.e., Read BMA280 Chip id and Send data via UART for every 5second) to generate bin file. 
3. Connect PC and Nucleo-L4R5ZI board via Uart.

#### Run
1. Open the STM32 ST-LINK Utility Software.
2. Select the binary file to flash the software into the Nucleo-L4R5ZI device
3. Click Target option and Select "Program and verify"
4. Download dialog box opens, Select "Reset after programming" option and Click "Start"
5. Close the STM32 ST-LINK Utility Software, Once Download dialog box closes
6. Open Hterm Software, Select the comport and Click "Connect". 
     * Expected:
     * Every 5seconds, BMA280 Chip id must be printed in UART.


#### TearDown
1. Disconnect the comport and Close the hterm software.