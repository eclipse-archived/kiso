# Test Entry 1: Essentials

## Test Suite 1.1: UART

### Description

This suite aims at testing the basic functionality of the MCU UART APIs.

### Setup

The test setup consists of the test coordinator and one (1) test participant.

* Test Coordinator (on PC)
* The DUT is connected to the PC via UART which is the Test Coordination Channel. The embedded C testling code is flashed onto the DUT.

### Teardown

No special teardown

### Test Cases

#### TC 1.1.1: Functional Test

##### Setup

1. Get device handle of BSP initialized generic UART
    * UART must be physically connected in loopback mode, i.e. jumper wire between UART Tx <-> Rx.
2. Allocate OS signal semaphores used as signal from IRQ
3. Connect the generic UART BSP
4. Initialize UART MCU with UART handle
5. Enable generic UART BSP

##### Run

1. Initiate UART receive process
2. Start send process dummy data of a few bytes size
3. Wait for send to completed
    * Maximum timeout depends on baud-rate and data length
4. Expect to receive echo

##### Teardown

1. Deinitialize UART MCU, deactivating IRQs
2. Disable generic UART BSP
3. Disconnect generic UART BSP
4. Free OS signal semaphores
