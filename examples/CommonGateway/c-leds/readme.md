# Example Project: C-LEDS for CommonGateway Development Kit

## Goal:
This example is the hello world example for running Kiso on the CommonGateway development kit.


## Brief:
The example will flash the blue LED. It will also show how to enable the logging module.

### Components Required:

#### Software Components
Please refer to the getting started [quick start](http://kiso.rempler.de:1313/user-guide/quick_start.html) guide for Eclipse Kiso. 

#### Hardware Components
You will need CommonGateway board.If you want to flash, you need an external debugger,please refer to the vendors reference manual.


##  Initial Setup
Configure the project to be compiled for the CommonGateway Board which is located in boards/CommonGateway

## Execution Flow
```
cd kiso
cmake . -Bbuild -DKISO_BOARD_NAME=CommonGateway
cmake --build build
```
Now as you have built the project you should have an application binary ready to flash.
For you own convenience you can use the cmake build system also to flash the binary with the external debugger.
```
cmake --build build --target flash
```


## Code Walkthrough

The essence of this example can be seen in the *blink_led* function, which is toggling the LED and is re-enqueuing itself into the command processor queue.

```c
void blink_led(void *param1, uint32_t param2)
{
    (void)param2;  
    BSP_LED_Switch(COMMONGATEWAY_LED_BLUE_ID, COMMONGATEWAY_LED_COMMAND_TOGGLE);
    LOG_DEBUG("Led switch");
    vTaskDelay(500);
    CmdProcessor_Enqueue((CmdProcessor_T *)param1, blink_led, param1, 0);
}
```

## Expected Output

Once you have flashed the example you should see the blue onboard LED. 
If you have your device connected to the serial port you will see the output of the Kiso logging module as below.
```
0 D 0 MainCmdProcessor   [kiso/examples/CommonGateway/c-leds/source/blinky_led.c:55]        Logging was started successfully
```

## Troubleshooting

If you don't find a conclusion using the [documentation](http://kiso.rempler.de:1313), please message us on [mattermost](mattermost.eclipse.org/eclipse/channels/kiso).
