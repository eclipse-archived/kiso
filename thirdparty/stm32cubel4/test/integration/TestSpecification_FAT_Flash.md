Integration Test Specification for FAT FS
==

# Test Section 1: "Test Specification for FAT FS"


This section aims at testing the functionalities of the FAT File System Functionality  for the external Flash (via SPI) used in the BSE Project
* The test setup has the following
 1. BSE R0 sample is mounted on the JIG and screwed tightly
 2. J-LINK lite is connected to the JIG via the JTAG interface (for flashing the h/w) and the other end is connected to PC
 3. 12V power is supplied to the JIG through VEXT and GND (From power supply or 12V adapter or eBike Battery)
 4. A uUSB is connected to the USB to UART converter P5 connector on the jig.
 5. The USB to UART converter is connected to UART3 of R0 sample for UART communication between jenkins PC and R0 sample.
 5. The wiki link contains the test setup pictures [https://bcds01.de.bosch.com/wiki/display/BSE/Testing](https://bcds01.de.bosch.com/wiki/display/BSE/Testing)
* Before running the test cases,the FAT flash test code has to be flashed.


**Section Setup:**

* As per integration test frame work, the TLV frame for Drivers section setup is TBD.
* The TLV frame is sent to the DUT via Coordination channel.

**Section Tear Down:**

* The TLV frame for miscDev section tear down is TBD.
* The TLV frame is sent to the DUT via Coordination channel.

## Test Suite 1: Flash
**Description:**

* This suite only aims at testing the initialisation of the flash as prerequisite.

**Suite Setup:**

* As per integration test frame work, the TLV frame for Flash suite setup is TBD.
* The TLV frame for test suite setup is sent to the DUT via Coordination channel.

**Suite Tear Down:**

* The TLV frame for Button suite tear down is TBD.
* The TLV frame is sent to the DUT via Coordination channel.


### TC001: Testcase_ FLASH_W25Q256FV_init
initialize Flash by initializing Spi and configure (at least) to 4-byte mode.
Checked by register value of flash.
* Test case Setup
	set flash (back) to factory default
* Test Case Run
	* Parameter passed: NIL
	* Run FLASH_W25Q256FV_init API
* Expected behaviour:
	* Expected return value from the API: TBD
	* Read status register 1 - bit ADS must be set.
	* Read status register 3 - bit HOLD/RESET must be set.
* Test case Tear Down
	* No required separate Test Case Tear Down for this example.

## Test Suite 2: FAT
**Description:**

* This suite aims at testing the initialisation of flash, writing and reading in the file system, creating folders and deleting those items.

**Suite Setup:**

* As per integration test frame work, the TLV frame for FAT suite setup is TBD.
* The TLV frame for test suite setup is sent to the DUT via Coordination channel.

**Suite Tear Down:**

* The TLV frame for Button suite tear down is TBD.
* The TLV frame is sent to the DUT via Coordination channel.


### TC001: Testcase_ disk_initializeFat
initialize FAT is tested. the FAT driver is linked to the SPI.

* Test case Setup
	set flash (back) to factory default

* Test Case Run
	* Parameter passed: NIL
	* Run disk_initializeFat API

* Expected behaviour:
	* Expected return value from the API: RETCODE_SUCCESS.
	* FATFS_GetAttachedDriversNbr must return 1.

* Test case Tear Down
	* No required separate Test Case Tear Down for this example.

### TC002: Testcase_ f_mkfs_and_f_mkdir
create a FAT volume on  the flash and create a subdirectory.
The creation of the subdirectory should validate, that both worked correctly. 

* Test case Setup
	ensure SPI is initialized
	ensure FAT is initialized

* Test Case Run
	* Parameter passed: fs, "", 1 (force mount)
	* Run f_mount API
	* Parameter passed: Logical drive number, 0, size
	* Run f_mkfs API
	* Parameter passed: "testdir",null
	* Run f_mkdir API
	* Parameter passed: "testdir",null
	* Run f_stat API
* Expected behaviour:
	* Expected return value of f_stat from the API: FR_OK.

* Test case Tear Down
	* No required separate Test Case Tear Down for this example.

### TC003: Testcase_ f_open_anf_f_write
create a FAT volume on  the flash and create a subdirectory.
The creation of the subdirectory should validate, that both worked correctly. 

* Test case Setup
	ensure SPI is initialized
	ensure FAT is initialized
	ensure FAT volume exists (see TC001)
	create file pointer _fil_ and byteswritten pointer _bw_

* Test Case Run
	* Parameter passed: &fil, "hello.txt", FA_CREATE_NEW | FA_WRITE
	* Run f_open API
	* Parameter passed: &fil, "Hello, World!\r\n", 15, &bw
	* Run f_write API
	* Parameter passed: "hello.txt",null
	* Run f_stat API
	* Parameter passed: &fil
	* Run f_close API;
* Expected behaviour:
	* Expected return value of f_stat from the API: FR_OK.

* Test case Tear Down
	* No required separate Test Case Tear Down for this example.

### TC004: Testcase_ f_open_anf_f_write
create a FAT volume on  the flash and create a subdirectory.
The creation of the subdirectory should validate, that both worked correctly. 

* Test case Setup
	ensure SPI is initialized
	ensure FAT is initialized
	ensure FAT volume exists (see TC001)
	create file pointer _fil_ and byteswritten pointer _bw_

* Test Case Run
	* Parameter passed: &fil, "hello.txt", FA_CREATE_NEW | FA_WRITE
	* Run f_open API
	* Parameter passed: &fil, "Hello, World!\r\n", 15, &bw
	* Run f_write API
	* Parameter passed: "hello.txt",null
	* Run f_stat API
	* Parameter passed: &fil
	* Run f_close API;
* Expected behaviour:
	* Expected return value of f_stat from the API: FR_OK.

* Test case Tear Down
	* No required separate Test Case Tear Down for this example.

### TC005: Testcase_ f_open_anf_f_gets
create a FAT volume on  the flash and create a subdirectory.
The creation of the subdirectory should validate, that both worked correctly. 

* Test case Setup
	ensure SPI is initialized
	ensure FAT is initialized
	ensure FAT volume exists (see TC001)
	ensure File "hello.txt" exists as defined in TC003.
	create file pointer _fil_ and line buffer (char array) _line_ with size >= 20
* Test Case Run
	* Parameter passed: &fil, "hello.txt", FA_CREATE_NEW | FA_WRITE
	* Run f_open API
	* Parameter passed: &fil, "Hello, World!\r\n", 15, &bw
	* Read String by running f_gets API
	* Run f_close API;

* Expected behaviour:
	* check if "Hello, World!\r\n" is the output

* Test case Tear Down
	* No required separate Test Case Tear Down for this example.

### TC006: Testcase_ f_unlink
create a FAT volume on  the flash and create a subdirectory.
The creation of the subdirectory should validate, that both worked correctly. 

* Test case Setup
	ensure SPI is initialized
	ensure FAT is initialized
	ensure FAT volume exists (see TC001)
	ensure File "hello.txt" exists as defined in TC003.

* Test Case Run
	* Parameter passed: "hello.txt"
	* Run f_unlink API
	* Parameter passed: "hello.txt",null
	* Run f_stat API

* Expected behaviour:
	* Expected return value of f_stat from the API: FR_NO_FILE.

* Test case Tear Down
	* No required separate Test Case Tear Down for this example.

### TC007: Testcase_ f_unlink_dir
create a FAT volume on  the flash and create a subdirectory.
The creation of the subdirectory should validate, that both worked correctly. 

* Test case Setup
	ensure SPI is initialized
	ensure FAT is initialized
	ensure FAT volume exists (see TC001)
	ensure directory "testdir" exists as defined in TC002.

* Test Case Run
	* Parameter passed: "testdir"
	* Run f_unlink API
	* Parameter passed: "testdir",null
	* Run f_stat API

* Expected behaviour:
	* Expected return value of f_stat from the API: FR_NO_PATH / FR_NO_FILE (TBD).

* Test case Tear Down
	* No required separate Test Case Tear Down for this example.
