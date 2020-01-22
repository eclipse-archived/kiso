## Release date and name ## 
2019-10-24: ICD (pre-release)

## Disclaimer ##
See [DISCLAIMER.md](https://github.com/Bosch-AE-SW/cddk-oss/blob/master/DISCLAIMER.md)

## Features ##
* ICD (2019-10-24)
  This is not a release, but the first initial code drop to Eclipse Foundation.
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
  * No multi-plattform support
  * No multi-OS support
  * No HW integration tests
  * Limited Cellular (SW Module) documentation
  * Limited configuration documentation
  * Limited code snipped documentation
  * Limited static code check (no complexity metrics)
