/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

#include "BCDS_TestAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID 0

#include "BCDS_Tests.h"
#include "BCDS_Retcode.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "BCDS_BSP_Board.h"
#include "BSP_BoardSettings.h"
#include "TestSuite_I2C.h"
#include "BSP_BoardType.h"
#include "BCDS_MCU_I2C.h"
#include "BCDS_BSP_SensorNode.h"
#include "BSP_SensorNode.h"

#include <bmm050.h>
#include <stdlib.h>

#define TEST_CASE_1_a_ID   1

#define BMM050_CHIP_ID_VALUE                UINT8_C(0x32)
#define I2C_BUFFER_LEN                      255U
#define DATA_TRANSFER_TIMEOUT_MS            UINT32_C(1000)
#define DELAY_FOR_BMI160_MODE_CHANGE        100UL
#define BMM050_USER_CHIP_ID__REG            (BMM050_CHIP_ID)

static I2C_T BSP_DataCollector_I2C_Handle = 0;
MCU_I2C_Callback_T I2CCallback;
BSP_Sensor_InterruptCallback_T BMI160SensorNodeCallback;
static xSemaphoreHandle I2CBusLock;
struct bmm050_t Bmm150Struct;

static void TestCase_1_a_run(CCMsg_T* ccmsg);

static Retcode_T TestCase_I2C_Setup(CCMsg_T *ccmsg);
static Retcode_T TestCase_I2C_TearDown(CCMsg_T *ccmsg);

static void DataCollector_I2CCallback(I2C_T i2c, struct MCU_I2C_Event_S event);
static void DataCollector_SensorNodeCallback(uint32_t, uint32_t);
static uint32_t MsToTicks(uint32_t miliseconds);
static Retcode_T CreateBinarySemaphore(SemaphoreHandle_t* semaphoreToCreate);
static Retcode_T DeleteSemaphore(SemaphoreHandle_t* semaphoreToDelete);
static s8 BstSensor_I2C_BusRead(u8 Device_addr, u8 Reg_addr, u8 *Reg_data, u8 Count);
static s8 BstSensor_I2C_BusWrite(u8 Device_addr, u8 Reg_addr, u8 *Reg_data, u8 Count);

Retcode_T TestSuite_I2C_initialize(uint8_t testSuiteId)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = Tests_RegisterTestSuite(testSuiteId, TestCase_I2C_Setup, TestCase_I2C_TearDown);

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(testSuiteId, TEST_CASE_1_a_ID, NULL, TestCase_1_a_run, NULL);
    }
    return retcode;
}

static void TestCase_1_a_run(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    uint8_t Buf[I2C_BUFFER_LEN] = { 0 };
    Buf[0] = BMM050_USER_CHIP_ID__REG;

    Retcode_T Rc = RETCODE_OK;

    Rc = MCU_I2C_Send((I2C_T) BSP_DataCollector_I2C_Handle, BMM050_I2C_ADDRESS, Buf, 1);
    if (pdTRUE == xSemaphoreTake(I2CBusLock, (TickType_t)(MsToTicks(DATA_TRANSFER_TIMEOUT_MS))))
    {
        /* We were able to obtain the semaphore and can now access the
         shared resource. */
    }
    else
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        Tests_SendReport(Rc, "Failed to send Byte over I2C");
        return;
    }

    if (RETCODE_OK == Rc)
    {
        Rc = MCU_I2C_Receive((I2C_T) BSP_DataCollector_I2C_Handle, BMM050_I2C_ADDRESS, Buf, 1);
        if (pdTRUE == xSemaphoreTake(I2CBusLock, (TickType_t)(MsToTicks(DATA_TRANSFER_TIMEOUT_MS))))
        {
            /* We were able to obtain the semaphore and can now access the
             shared resource. */
        }
        else
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
            Tests_SendReport(Rc, "Failed to read Byte over I2C");
            return;
        }
        if (BMM050_CHIP_ID_VALUE != Buf[0])
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            Tests_SendReport(Rc, "Failed to read CHIP ID over I2C");
            return;
        }
    }
    else
    {
        Tests_SendReport(Rc, "Failed to send Byte over I2C");
        return;
    }
    if (RETCODE_OK != Rc)
    {
        Tests_SendReport(Rc, "Failed to send Byte over I2C");
        return;
    }
    else
    {
        Tests_SendReport(Rc, "Success I2C Byte send");
        return;
    }
}

static s8 BstSensor_I2C_BusRead(u8 Device_addr, u8 Reg_addr, u8 *Reg_data, u8 Count)
{
    Retcode_T Rc = RETCODE_OK;
    s8 Bmi160_Rc = 0;

    if (NULL == Reg_data || UINT8_C(0) >= Count)
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (RETCODE_OK == Rc)
    {
        if (NULL == I2CBusLock || UINT32_C(0) == BSP_DataCollector_I2C_Handle)
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
        }
    }

    if (RETCODE_OK == Rc)
    {
        Rc = MCU_I2C_ReadRegister((I2C_T) BSP_DataCollector_I2C_Handle, Device_addr, Reg_addr, Reg_data, Count);
        if (pdTRUE == xSemaphoreTake(I2CBusLock, (TickType_t)(MsToTicks(DATA_TRANSFER_TIMEOUT_MS))))
        {
            /* We were able to obtain the semaphore and can now access the
             shared resource. */
        }
        else
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        }
    }

    if (RETCODE_OK != Rc)
    {
        Bmi160_Rc = ERROR;
    }
    return Bmi160_Rc;
}

static uint32_t MsToTicks(uint32_t miliseconds)
{
    uint32_t frequency = HSE_VALUE;
    uint32_t ticks = miliseconds * (frequency / 1000);
    return ticks;
}

/*
 *  @brief : The function is used as I2C bus write
 *  @return : Status of the I2C write
 *  @param Device_addr : The device address of the sensor
 *  @param Reg_addr : Address of the first register,
 *  will data is going to be written
 *  @param Reg_data : It is a value hold in the array,
 *      will be used for write the value into the register
 *  @param Count : The no of byte of data to be write
 */
static s8 BstSensor_I2C_BusWrite(u8 Device_addr, u8 Reg_addr, u8 *Reg_data, u8 Count)
{
    Retcode_T Rc = RETCODE_OK;
    s8 Bmi160_Rc = 0;

    if (NULL == Reg_data || UINT8_C(0) >= Count)
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if (RETCODE_OK == Rc)
    {
        if (NULL == I2CBusLock || UINT32_C(0) == BSP_DataCollector_I2C_Handle)
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
        }
    }

    if (RETCODE_OK == Rc)
    {

        Rc = MCU_I2C_WriteRegister((I2C_T) BSP_DataCollector_I2C_Handle, Device_addr, Reg_addr, Reg_data, Count);

        if (pdTRUE == xSemaphoreTake(I2CBusLock, (TickType_t)(MsToTicks(DATA_TRANSFER_TIMEOUT_MS))))
        {
            /* We were able to obtain the semaphore and can now access the
             shared resource. */
        }
        else
        {
            Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        }
    }

    if (RETCODE_OK != Rc)
    {
        Bmi160_Rc = ERROR;
    }
    return Bmi160_Rc;
}

static void SensorsDelayInMs(u32 msek)
{
    BSP_Board_Delay(msek);
}

static Retcode_T TestCase_I2C_Setup(CCMsg_T *ccmsg)
{
    BCDS_UNUSED(ccmsg);

    Retcode_T Rc = RETCODE_OK;
    s8 SensorReturn = -1;

    I2CCallback = DataCollector_I2CCallback;
    BMI160SensorNodeCallback = DataCollector_SensorNodeCallback;

    Rc = CreateBinarySemaphore(&I2CBusLock);

    if (RETCODE_OK != Rc)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    Rc = BSP_SensorNode_Connect();
    if (RETCODE_OK != Rc)
    {
        return Rc;
    }
    BSP_DataCollector_I2C_Handle = BSP_SensorNode_GetI2CHandle();
    Rc = MCU_I2C_Initialize((I2C_T) BSP_DataCollector_I2C_Handle, I2CCallback);
    if (RETCODE_OK != Rc)
    {
        return Rc;
    }

    Rc = BSP_SensorNode_Enable(BSP_XDK_SENSOR_BMM150, BMI160SensorNodeCallback);
    if (RETCODE_OK != Rc)
    {
        return Rc;
    }

    Bmm150Struct.bus_write = BstSensor_I2C_BusWrite;
    Bmm150Struct.bus_read = BstSensor_I2C_BusRead;
    Bmm150Struct.delay_msec = SensorsDelayInMs;
    Bmm150Struct.dev_addr = BMM050_I2C_ADDRESS;

    SensorsDelayInMs(50);

    /* Initialize bmm150 sensor */
    SensorReturn = bmm050_init(&Bmm150Struct);

    SensorsDelayInMs(DELAY_FOR_BMI160_MODE_CHANGE);

    if (0 == SensorReturn)
    {
        return RETCODE_OK;
    }
    else
    {
        return RETCODE_FAILURE;
    }
    return Rc;
}

static Retcode_T TestCase_I2C_TearDown(CCMsg_T *ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T Rc = RETCODE_FAILURE;

    Rc = DeleteSemaphore(&I2CBusLock);
    if (RETCODE_OK != Rc)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES); // ??? Welcher Retcodes Übergeben ??
    }
    if (RETCODE_OK == Rc)
    {
        Rc = MCU_I2C_Deinitialize((I2C_T) BSP_DataCollector_I2C_Handle);
        if (RETCODE_OK != Rc)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES); // ??? Welcher Retcodes Übergeben ??
        }
        Rc = BSP_SensorNode_Disable(BSP_XDK_SENSOR_BMM150);
        if (RETCODE_OK == Rc)
        {
            BSP_SensorNode_Disconnect();
            return Rc;
        }
        else
        {
            return RETCODE_FAILURE;
        }
    }
    else
    {
        return RETCODE_FAILURE;
    }
}

static void DataCollector_I2CCallback(I2C_T i2c, struct MCU_I2C_Event_S event)
{
    BCDS_UNUSED(i2c);
    Retcode_T Rc = RETCODE_OK;

    if (UINT8_C(1) == event.TxComplete)
    {
        if (RETCODE_OK == Rc)
        {
            BaseType_t higherPriorityTaskWoken = pdFALSE;

            if (NULL != I2CBusLock)
            {
                if (pdTRUE == xSemaphoreGiveFromISR(I2CBusLock, &higherPriorityTaskWoken))
                {
                    portYIELD_FROM_ISR(higherPriorityTaskWoken);
                }
                else
                {
                    /* ignore... semaphore has already been given */
                }
            }
            else
            {
                Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
            }
        }
    }

    if (UINT8_C(1) == event.RxComplete)
    {
        if (RETCODE_OK == Rc)
        {
            BaseType_t higherPriorityTaskWoken = pdFALSE;

            if (NULL != I2CBusLock)
            {
                if (pdTRUE == xSemaphoreGiveFromISR(I2CBusLock, &higherPriorityTaskWoken))
                {
                    portYIELD_FROM_ISR(higherPriorityTaskWoken);
                }
                else
                {
                    /* ignore... semaphore has already been given */
                }
            }
            else
            {
                Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
            }
        }
    }

    if (UINT8_C(1) == event.TransferError)
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    if (RETCODE_OK != Rc)
    {
        Retcode_RaiseErrorFromIsr(Rc);
    }
}

static void DataCollector_SensorNodeCallback(uint32_t var1, uint32_t var2)
{
    /* nothing to do */
    BCDS_UNUSED(var1);
    BCDS_UNUSED(var2);
}

static Retcode_T CreateBinarySemaphore(SemaphoreHandle_t* semaphoreToCreate)
{
    if (NULL != *semaphoreToCreate)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
    }
    else
    {
        *semaphoreToCreate = xSemaphoreCreateBinary();
        if (NULL == semaphoreToCreate)
        {
            return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
        }
        else
        {
            return RETCODE_OK;
        }
    }
}

static Retcode_T DeleteSemaphore(SemaphoreHandle_t* semaphoreToDelete)
{
    Retcode_T Rc = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
    if (NULL != *semaphoreToDelete)
    {
        vSemaphoreDelete(*semaphoreToDelete);
        *semaphoreToDelete = NULL;
        Rc = RETCODE_OK;
    }
    return Rc;
}

