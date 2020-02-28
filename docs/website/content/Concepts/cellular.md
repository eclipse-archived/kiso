---
title: "Cellular Developer's Guide"
weight: 2
---

## Status

| Modem         | Hardware Support   | SocketService | HttpService | DnsService   |
|---------------|--------------------|---------------|-------------|--------------|
| Ublox SARA-R4 | [x]                | [x]           | [x]         | [x]          |

## Introduction

The **Cellular** package provides support for AT based communication with an via serial connected modem device for
mobile networks. Additionally, as most modems implement their own data communication services, cellular adds support for
standard protocols like HTTP and Socket communication (TCP/UDP). More features are likely to come, such as support for
additional on-chip network stacks.

{{<figure src="../../images/Kiso_Cellular_Technical_Context.png" title="Figure 1: Cellular context">}}

Hardware-wise, the typical setup for Cellular involves of a primary µC running Kiso-enabled firmware and a modem
connected via UART. Hardware details are abstracted via Essentials BSP/MCU API. The modem-side is expected to run an AT
command interface -- in cellular network terminology a **TA** (Terminal Adaptor), while the Kiso Cellular package acts
as AT command user -- or **TE** (Terminal Emulator).

## Building blocks

The cellular building blocks consist of mainly two parts which together realize different services for cellular
operations.

Vendor and model specific differences in AT command interfaces, are unified through a command high level API.
Consequently, vendor specific implementations are separated into different folders. Parts of the AT command interface
and internal driver infrastructure are common across all variants and are collected in a common folder.

Cellular's common layer provides all necessary features to provide serial data communication with the modem. AT commands
used to talk to the modem are passed through the Engine to the Hardware layer.

Received serial data from the modem is passed through the AT Response Parser, which tokenizes the incoming data-stream
and formulates events -- similar to a Java SAX parser. The parsed tokens are stored in a AT Response Queue. This queue
implements a blocking-read behavior, enabling further interpretation stages to wait for AT tokens in sequential order.
Interpretation is done either by generic or vendor specific AT response handlers. The generic handlers implement a
subset of the [3GPP TS 27.007 V15.4.0 (2018-12)](https://portal.3gpp.org/desktopmodules/Specifications/SpecificationDetails.aspx?specificationId=1512) commands, while vendor specific handlers follow the command reference of their respective datasheet.
{{<figure src="../../images/Kiso_Cellular_BuildingBlockView.png" title="Figure 2: Cellular building blocks view">}}

Currently there are two OS tasks realising data parsing and handling of Unsolicited Result Codes (URC). AT commands are
sent out directly from the caller's task context.

## Porting Guide

### Porting to a new board

In order to port the Cellular driver to another board you just need to define and implement a basic set of BSP functions.
Cellular is expecting to have the following APIs implemented on BSP level:

{{< highlight c "linenos=table" >}}
Retcode_T CELLULAR_BSP_CONNECT(void);
Retcode_T CELLULAR_BSP_ENABLE(void);
Retcode_T CELLULAR_BSP_DISABLE(void);
Retcode_T CELLULAR_BSP_DISCONNECT(void);
HWHandle_T CELLULAR_BSP_GET_UART_HANDLE(void);
Retcode_T CELLULAR_BSP_RESET(void);
{{< / highlight >}}

This is basically following the general BSP startup and shutdown sequence which is common for all Kiso HW components and
peripherals.

* **`_CONNECT`** has to ensure that the physical connection between the MCU and the cellular modem is established.
* **`_ENABLE`** needs to make the component ready to communicate, ensure that clocks are setup properly, power is turned
  on, etc.
* **`_GET_UART`** shall return an instance of the assigned UART port, as Kiso_Cellular is designed to work directly with
  Kiso essentials MCU HAL.
* **`_DISABLE`** will shutdown the component, disable power, what ever is required to turn it off
* **`_DISCONNECT`** is the counter part of CONNECT, so the device will be physically disconnected, pins may get
  tri-stated
* **`_RESET`** performs a device reset, depends on hardware support, some device use dedicated pins for example


### Porting to a new cellular modem

Porting Celluar to a new modem will require to:

1. provide a custom BSP for that modem
2. realize the necessary interface implementation for `Kiso_Cellular.h`
3. additionally add the required services (`Kiso_CellularXXXService`: Socket, DNS, HTTP)

**Please note that most vendors have their own AT command sets for data contexts and their IP stacks.**

{{< highlight txt>}}
The current design maybe subject to change as it is not fully aligned with all quality goals. The u-blox reference
implementation could be taken as basis to port to a new modem. We will investigate the requirements needed to generalize
the common command sets and will change the driver accordingly

Next versions will rather use OS porting layers than having a direct interface to FreeRTOS. Thils will reduce
inter-dependencies while adding new modems. The same is valid for hardware connections provided via the essentials
package. Moving all UART dependencies directly to the common::Hardware module will improve portability towards other
peripheral libraries.
{{< /highlight>}}
