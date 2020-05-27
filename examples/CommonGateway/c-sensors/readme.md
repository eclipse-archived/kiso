# Example Project: C-sensor for CommonGateway Development Kit

## Goal:
this example is for running the c-sensor in CommonGateway.

## Brief:
this example will show the enviromental measurements and dimenstions.

### Components Required:

#### Software Components
Please refer to the getting started [quick start](http://kiso.rempler.de:1313/user-guide/quick_start.html) guide for Eclipse Kiso.

#### Hardware Components
You will need CommonGateway Board. 
## (*Optional*) Initial Setup
Configure the project to be compiled for theCommonGateway Board which is located in boards/CommonGateway

## (*Optional*) Execution Flow
```
cd kiso
cmake . -Bbuild -DKISO_BOARD_NAME=CommonGateway -DKISO_APPLICATION_NAME=c-sensors -DKISO_APPLICATION_PATH=examples/CommonGateway/c-sensors 
cmake --build build
```
Now as you have built the project you should have an application binary ready to flash.
For you own convenience you can use the cmake build system also to flash the binary with a J-Link debugger (either on-board or externally).
```
cmake --build build --target flash
```

## (*Optional*)Code Walkthrough
The essence of this example can be seen in theappPeriodicTest function, which is taking the data from  SensorEnvironment_Read and make some calculations to give the output understandable and take the data from the SensorAccelerometer_Read and show dimensions and if everything goes correct and green led will flash, otherwise the red led will flash .

```c
void appPeriodicTest(void *CmdProcessorHandle, uint32_t param2)
{
    (void)param2;
    struct bme280_data sensorEnvironmentData = {0};
    struct bma2x2_accel_data sensorAccelerometerData = {0};

    Retcode_T returnValue = RETCODE_OK;

    if (RETCODE_OK == returnValue)
    {
        returnValue = SensorEnvironment_Read(&sensorEnvironmentData);
        if (RETCODE_OK == returnValue)
        {
#ifdef BME280_FLOAT_ENABLE
#error BME280_FLOAT_ENABLE not supported for this example
#else
            /* Finally we need to apply static temperature offset correction.
             * As no two sensors can be the same, this coefficient will be
             * different from sensor to sensor.
             * */
            const int32_t DEFAULT_TEMPERATURE_OFFSET = -210;
            int32_t temperature = sensorEnvironmentData.temperature + DEFAULT_TEMPERATURE_OFFSET;

            //  Unit is in 1024 * % relative humidity
            uint32_t humidity = (sensorEnvironmentData.humidity * 1000) / 1024;
#ifdef BME280_64BIT_ENABLE
            // If macro "BME280_64BIT_ENABLE" is enabled, which it is by default, the unit is 100 * Pascal
            uint32_t pressure = sensorEnvironmentData.pressure;
#else
            // If this macro is disabled, Then the unit is in Pascal
            uint32_t pressure = sensorEnvironmentData.pressure * 100;
#endif
            // clang-format off
            LOG_DEBUG("Temperature: %ld.%02lu;\tPresure: %lu.%03luhPa;\tHumidity: %lu.%03lu%%RH",
                    temperature / 100,
                    temperature % 100,
                    pressure / 10000,
                    (pressure % 10000) / 10,
                    humidity / 1000,
                    humidity % 1000);
            // clang-format on
#endif
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
            // clang-format off
            LOG_DEBUG("X: %ld;\tY: %ld;\tZ: %ld",
                    sensorAccelerometerData.x,
                    sensorAccelerometerData.y,
                    sensorAccelerometerData.z);
            // clang-format on
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
}
```
This code through in two main functions to get the output.
the first one is for the Enviroment Sensor, which go through two functions called SensorEnvironment_Init and SensorEnvironment_Read as you can see [here](https://github.com/eclipse/kiso/blob/master/examples/CommonGateway/c-sensors/source/env_sensor.c) that these two functions give the measurements continuously for the (temperature, pressure and humidity).
the second one is for Accelerometer Sensor, which go through two functions called SensorAccelerometer_Init and SensorAccelerometer_Read as you can see [here](https://github.com/eclipse/kiso/blob/master/examples/CommonGateway/c-sensors/source/accel_sensor.c)that these two functions give the dimantions (x,y,z).

## Expected Output

Specify what the user should obtain as an output of the execution the example. e.g. terminal output screenshots, pictures(flashing LEDs) etc.

## Troubleshooting

If you don't find a conclusion using the [documentation](http://kiso.rempler.de:1313), please message us on [mattermost](mattermost.eclipse.org/eclipse/channels/kiso).