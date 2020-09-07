# Example Project: C-sensor for CommonGateway Development Kit

## Goal:
This example is for running the c-sensor in CommonGateway.

## Brief:
This example will show the enviromental measurements and dimensions.

### Components Required:

#### Software Components
Please refer to the getting started [quick start](http://docs.eclipsekiso.de:1313/user-guide/quick_start.html) guide for Eclipse Kiso.

#### Hardware Components
You will need CommonGateway Board. 
## Initial Setup
Configure the project to be compiled for the CommonGateway Board which is located in [boards/CommonGateway](https://github.com/eclipse/kiso/tree/master/boards/CommonGateway). To do this, add the respective `KISO_BOARD_NAME` setting to `cmake` during workspace configuration, as shown in Section [Execution Flow](#Execution-Flow)

## Execution Flow
```
cd kiso
cmake . -Bbuild -DKISO_BOARD_NAME=CommonGateway -DKISO_APPLICATION_NAME=c-sensors -DKISO_APPLICATION_PATH=examples/CommonGateway/c-sensors 
cmake --build build
```
Now as you have built the project you should have an application binary ready to run.
For you own convenience you can use the cmake build system also to flash the binary with the external debugger.
```
cmake --build build --target flash
```

## Code Walkthrough
The main program loop can be seen in `appPeriodicTest()`. It takes data from  `SensorEnvironment_Read()` and `SensorAccelerometer_Read()`, performs some conversions and corrections based on static calibration and finally presents the data via debug log. If everything went OK, the green LED will flash. In case of any errors the red LED will flash.

```c
void appPeriodicTest(void *CmdProcessorHandle, uint32_t param2)
{
    struct bme280_data sensorEnvironmentData = {0};
    struct bma2x2_accel_data sensorAccelerometerData = {0};

    Retcode_T returnValue = RETCODE_OK;

    // [...]

    if (RETCODE_OK == returnValue)
    {
        returnValue = SensorEnvironment_Read(&sensorEnvironmentData);
    
        // [... perform conversion and corrections based on calibration coefficients ...]
    
        if (RETCODE_OK == returnValue)
        {
            LOG_DEBUG("Temperature: %ld.%02lu;\tPresure: %lu.%03luhPa;\tHumidity: %lu.%03lu%%RH",
                    temperature / 100,
                    temperature % 100,
                    pressure / 10000,
                    (pressure % 10000) / 10,
                    humidity / 1000,
                    humidity % 1000);
        }
        else
        {
            LOG_DEBUG("Env sensor read failed!\r\n");
        }
    }

    if (RETCODE_OK == returnValue)
    {
        returnValue = SensorAccelerometer_Read(&sensorAccelerometerData);
        
        if (RETCODE_OK == returnValue)
        {
            LOG_DEBUG("X: %ld;\tY: %ld;\tZ: %ld",
                    sensorAccelerometerData.x,
                    sensorAccelerometerData.y,
                    sensorAccelerometerData.z);
        }
        else
        {
            LOG_DEBUG("Accel sensor read failed!\r\n");
        }
    }

    if (RETCODE_OK == returnValue)
    {
        BSP_LED_Switch(COMMONGATEWAY_LED_GREEN_ID, COMMONGATEWAY_LED_COMMAND_TOGGLE);
    }
    else
    {
        BSP_LED_Switch(COMMONGATEWAY_LED_GREEN_ID, COMMONGATEWAY_LED_COMMAND_OFF);
        BSP_LED_Switch(COMMONGATEWAY_LED_RED_ID, COMMONGATEWAY_LED_COMMAND_ON);
    }

    if (RETCODE_OK == returnValue)
    {
        BSP_Board_Delay(5);
        returnValue = CmdProcessor_Enqueue((CmdProcessor_T *)CmdProcessorHandle, appPeriodicTest, CmdProcessorHandle, UINT32_C(0));
    }

    // [...]
}
```
The app is structured in three modules:

* the main app logic
* environment sensor and
* acceleration sensor logic.

With the main app logic being at top of the hierarchy.

Each module has to be initialized first, before it can be used. The app logic initializes vital system components such as the sensor BSP components, as well as the I2C communication bus they are sit on. Finally each sensor module initializes the `bma2x2.h` and `bme280.h` library modules respectively.

Once initialization is complete, the main application loop is entered. This is done using the main command processor, created in `main()`. For each iteration of the app loop, `appPeriodicTest()` is enqueued at the end of the last iteration. This is repeated endlessly.

Reading acceleration data from `SensorAccelerometer_Read()`, one can directly access the x, y and z components as provided by the sensor. Environmental data however, need some static temperature correction first. This compensation can be changed via the `DEFAULT_TEMPERATURE_OFFSET` constant, with the measured temperature offset at a known temperature.

## Expected Output

During normal operation the green LED should toggle for each app loop cycle every 5 seconds. If an error occurred anywhere within the app loop iteration, the red LED will be set ON and the green LED set to OFF. The app loop cycle continues, and may recover, toggling the green LED, while keeping the red LED on.

At the same time, with each app loop, data is read from the sensors. This data can be viewed via the debug UART port, provided a appropriate host is connected.

## Troubleshooting

If you don't find a conclusion using the [documentation](http://docs.eclipsekiso.de:1313/), please message us on [mattermost](https://mattermost.eclipse.org/eclipse/channels/kiso).
