
# Example Project: C-LEDS for Nucleo F767ZI Development Kit

## Goal:
This example is the hello world example for running Kiso on the Nucleo F767ZI development kit.


## Brief:
The example will cycle through all three LEDs (red, green and blue). It will also show how to enable the logging module.

### Components Required:

#### Software Components
Please refer to the getting started [quick start](http://http://kiso.rempler.de:1313/user-guide/quick_start.html) guide for Eclipse Kiso. 

#### Hardware Components
You will need a STM NucleoF767ZI board. The board needs the J-Link debugger firmware flashed, to be easily flashed using Kiso's build target.
If you consider using your own debugger, or connect an external J-Link debugger, please refer to the vendors reference manual.


##  Initial Setup
Configure the project to be compiled for the NucleoF767 Board which is located in boards/NucleoF767

## Execution Flow
```
cd kiso
cmake . -Bbuild -DKISO_BOARD_NAME=NucleoF767
cmake --build build
```
Now as you have built the project you should have an application binary ready to flash.
For you own convenience you can use the cmake build system also to flash the binary with a J-Link debugger (either on-board or externally).
```
cmake --build build --target flash
```


## Code Walkthrough

The essence of this example can be seen in the *blink_led* function, which is toggling the LEDs and is re-enqueuing itself into the command processor queue.

```c
void blink_led(void *param1, uint32_t param2)
{
    static const enum NucleoF767_LED_Ids_E ledIDs[] = {NUCLEOF767_LED_GREEN_ID, NUCLEOF767_LED_BLUE_ID, NUCLEOF767_LED_RED_ID};

    uint32_t lastLed = param2;

    BSP_LED_Switch(ledIDs[lastLed], NUCLEOF767_LED_COMMAND_TOGGLE);

    lastLed = (lastLed + 1) % (sizeof(ledIDs) / sizeof(ledIDs[0]));

    vTaskDelay(500);

    CmdProcessor_Enqueue((CmdProcessor_T *)param1, blink_led, param1, lastLed);
}
```

## Expected Output

Once you have flashed the example you should see the three onboard LEDs red, green and blue cycling through. 
If you have your device connected to the serial port you will see the output of the Kiso logging module as below.
```
0 D 0 MainCmdProcesso   [kiso/examples/NucleoF767/c-leds/source/blinky_led.c:56]        Logging was started successfully
```

## Troubleshooting

If you don't find a conclusion using the [documentation](http://http://kiso.rempler.de:1313), please message us on [mattermost](mattermost.eclipse.org/eclipse/channels/kiso).