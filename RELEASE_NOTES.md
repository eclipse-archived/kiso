## Release date and name ## 
* 2020-01-16: ICD_UPDATE (pre-release)
* 2019-10-24: ICD (pre-release)

## Disclaimer ##
See [DISCLAIMER.md](https://github.com/Bosch-AE-SW/cddk-oss/blob/master/DISCLAIMER.md)

## Features ##
* ICD_UPDATE (2019-10-24): This is not a release, but an update to the first initial code drop, see below. It contains the delta that the development team worked on since then.
  * Reference HW: NucleoF767
  * Improved unittest coverage
  * Improved documentation
* ICD (2019-10-24): This is not a release, but the first initial code drop to Eclipse Foundation.
  * Reference HW: Bosch Common Gateway (Bosch internally available)
  * SW module: Essentials
    * STM32L4 support
    * I2C, SPI, UART
  * SW module: Utils (cross-cutting concepts & co)
    * FreeRTOS support
    * Command processor
    * Guarded task
    * Event hub
    * Pipe and filter
    * Task monitoring
    * Command line debugger
    * Logging (sync recorder & UART appender)
    * ErrorLogger
    * CRC
    * X-protocol
    * Ring (circular) buffer
    * UART transceiver
    * I2C transceiver
  * SW module: Cellular
    * Ublox SARA R4 (Cat-M1, NB-IoT)
    * DNS
    * HTTP
    * TCP/UDP Sockets
  * Continuous integration:
    * clang-format (code format check)
    * clang-tidy (static code check)
    * Unit tests
    * Doxygen (API documtation)
    * Hugo (user manual documentation)
  
  ## Known issues (work in progress) ##
  * NucleoF767 examples broken until SW module IP clearance: thirdparty/stm32cubef7
  * No multi-OS support
  * No HW integration tests
  * Limited Cellular (SW Module) documentation
  * Limited configuration documentation
  * Limited code snipped documentation
  * Limited static code check (no complexity metrics)
