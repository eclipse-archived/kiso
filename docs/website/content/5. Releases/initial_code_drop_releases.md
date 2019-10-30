---
title: "v0.1.0"
description: "First release"
weight: 1
draft: false
toc: true
menu:
  main:
    parent: 5. Releases
    identifier: initial_code_drop_releases
    weight: 1
---

## v0.1.0
The initial code drop will contain the following packages

**Features**:

* Essentials (Base of Kiso) with stm32l4 support
  * I2C
  * SPI
  * UART
* Utils (cross-cutting concepts & co) with FreeRTOS support
  * Command Processor
  * Guarded Task
  * Event Hub
  * Pipe And Filter
  * Task Monitoring
  * Command Line Debugger
  * Logging (Sync recorder & uart appender)
  * ErrorLogger
  * CRC
  * TLV
  * X-Protocol
  * Ring Buffer
  * UART Transceiver
  * I2C Transceiver
* Cellular with Ublox SARA R2
  * DNS
  * HTTP

**Continuous Integration**:

* build
* clang-format (format check)
* clang-tidy (static check)
* unittests
* doxygen
* hugo (website documentation)

**Limitations**

* Cellular concept missing
* Configuration not simple and not explained
* Integration tests in the continuous integration missing
* Some code snipped missing in the doxygen