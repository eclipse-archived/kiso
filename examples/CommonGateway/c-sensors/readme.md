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
Configure the project to be compiled for the CommonGateway Board which is located in [boards/CommonGateway](https://github.com/eclipse/kiso/tree/master/boards/CommonGateway) .

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
This code through in two main functions to get the output.
The first one is for the Enviroment Sensor, which go through two functions called [SensorEnvironment_Init](https://github.com/eclipse/kiso/blob/master/examples/CommonGateway/c-sensors/source/env_sensor.c#L58) and [SensorEnvironment_Read](https://github.com/eclipse/kiso/blob/master/examples/CommonGateway/c-sensors/source/env_sensor.c#L110) as you can see that these two functions give the measurements continuously for the (temperature, pressure and humidity).
The second one is for Accelerometer Sensor, which go through two functions called [SensorAccelerometer_Init](https://github.com/eclipse/kiso/blob/master/examples/CommonGateway/c-sensors/source/accel_sensor.c#L48) and [SensorAccelerometer_Read](https://github.com/eclipse/kiso/blob/master/examples/CommonGateway/c-sensors/source/accel_sensor.c#L76) as you can see that these two functions give the dimensions (x,y,z).

## Expected Output

Specify what the user should obtain as an output of the execution the example. e.g. terminal output screenshots, pictures(flashing LEDs) etc.

## Troubleshooting

If you don't find a conclusion using the [documentation](http://docs.eclipsekiso.de:1313/), please message us on [mattermost](mattermost.eclipse.org/eclipse/channels/kiso).
