---
title: "Cellular Developer's Guide"
weight: 2
---

## Status
| Modem         | Hardware Support   | SocketService | HttpService | DnsService   |
|---------------|--------------------|---------------|-------------|--------------|
| Ublox SARA-R4 | [x]                | [x]           | [x]         | [x]          |


## Introduction

The **Cellular** package provides support for **AT3GPP** based AT command controlled modem devices for mobile networks. Additionally, as most modem implement their own data communication services, cellular adds support for standard protocols like HTTP and Socket communication. More features are likely to come or to be added through 3rd party libraries via TCP/UDP Socket communication.

{{<figure src="../../images/Kiso_Cellular_Technical_Context.png" title="Figure 1: Cellular context">}}

The main components of the cellular system consists of a micro controller including UART peripherals on one side and a cellular modem connected on the other side.
All modem specific implementation shall be located in a vendor specific folder. 

## Building blocks

The cellular building blocks consist of mainly two parts which together realize different services for cellular operations. 

Cellular's common layer provides all necessary features to provide serial data communication with the modem.
Data for sending AT commands and for data contexts are send through the Engine and is passed to the Hardware Layer.

As received data is internally processed and sent either to the AT Response parser or handled though data context parsers.
{{<figure src="../../images/Kiso_Cellular_BuildingBlockView.png" title="Figure 2: Cellular building blocks view">}}

Currently there are two OS tasks realising data parsing and handling of Unsolicited Result Codes (URC).
At Commands are sent out directly from the caller's task context.
For convenience the common modules already provide a standard AT command set according to 3GPP TS 27.007 V15.4.0 (2018-12).

## Porting Guide
### Porting to a new board

In order to port the Cellular driver to another board you just need to define and implement a basic set of BSP functions.
Cellular is expecting to have the following APIs implemented on BSP level:
<p></p>

{{< highlight c "linenos=table" >}}
void CELLULAR_BSP_CONNECT(void); 
void CELLULAR_BSP_ENABLE(void); 
void CELLULAR_BSP_DISABLE(void); 
void CELLULAR_BSP_DISCONNECT(void) 
void CELLULAR_BSP_GET_UART_HANDLE(void) 
void CELLULAR_BSP_RESET(void);
{{< / highlight >}}

This is basically following the general BSP startup and shutdown sequence which is common for all Kiso HW components and peripherals.

**_CONNECT** has to ensure that the physical connection between the MCU and the cellular modem is established.

**_ENABLE** needs to make the component ready to communicate, ensure that clocks are setup properly, power is turned on, etc. 

**_GET_UART** shall return an instance of the assigned UART port, as Kiso_Cellular is designed to work directly with Kiso essentials MCU HAL. 

**_DISABLE** will shutdown the component, disable power, what ever is required to turn it off

**_DISCONNECT** is the counter part of CONNECT, so the device will be physically disconnected, pins may get tri-stated

**_RESET** performs a device reset, depends on hardware support, some device use dedicated pins for example


### Porting to a new cellular modem

Porting celluar to a new modem will require to:

1. provide a custom BSP for that modem
2. realize the necessary interface implementation for Kiso_Cellular
3. additionally add the required services (Kiso_CellularXXXService: Socket, DNS,HTTP)

**Please note that most vendors have their own AT command sets for data contexts and their IP stacks.**

{{< highlight txt>}}
The current design maybe subject to change as it is not fully aligned with all quality goals.
The u-blox reference implementation could be taken as basis to port to a new modem. 
We will investigate the requirements needed to generalize the common command sets 
and will change the driver accordingly

Next versions will rather use OS porting layers than having a direct interface to freeRTOS.
Thils will reduce inter dependencies while adding new modems. 
The same is valid for hardware connections provided via the essentials package. 
Moving all UART dependencies directly to the common::Hardware module will improve portability towards other peripheral libraries.
{{< /highlight>}}












