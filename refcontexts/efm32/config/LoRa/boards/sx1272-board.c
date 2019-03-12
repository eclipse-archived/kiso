/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: SX1272 driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
 */
/*****************************************************
 * This file has Modified as per the BCDS-APLM requirements
 *****************************************************/
#include "board.h"
#include "sx1272-board.h"
#include "BCDS_CmdProcessor.h"
#include "LoraEngine.h"
#include "BCDS_Assert.h"

/*!
 * \brief DIO 0 IRQ callback
 */
void SX1272OnDio0Irq( void );

/*!
 * \brief DIO 1 IRQ callback
 */
void SX1272OnDio1Irq( void );

/*!
 * \brief DIO 2 IRQ callback
 */
void SX1272OnDio2Irq( void );

/*!
 * Flag used to set the RF switch control pins in low power mode when the radio is not active.
 */
static bool RadioIsActive = false;

/*!
 * Antenna switch GPIO pins objects
 */
Gpio_t AntRx;
Gpio_t AntTx;
Gpio_t AntTxRx;

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
        SX1272Init,
        SX1272GetStatus,
        SX1272SetModem,
        SX1272SetChannel,
        SX1272IsChannelFree,
        SX1272Random,
        SX1272SetRxConfig,
        SX1272SetTxConfig,
        SX1272CheckRfFrequency,
        SX1272GetTimeOnAir,
        SX1272Send,
        SX1272SetSleep,
        SX1272SetStby,
        SX1272SetRx,
        SX1272StartCad,
        SX1272ReadRssi,
        SX1272Write,
        SX1272Read,
        SX1272WriteBuffer,
        SX1272ReadBuffer,
        SX1272SetMaxPayloadLength
};

/*!
 * Radio driver structure initialization
 */
void SX1272IoInit( void )
{
    /* pin setting for chip select of LoRa */
    GpioInit( &SX1272.Spi.Nss, RADIO_NSS, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, SET );

    /* pin setting for External interrupt of Lora DIO*/
    GpioInit( &SX1272.DIO0, RADIO_DIO_0, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, CLEAR );

    /* pin setting for External interrupt of Lora DIO*/
    GpioInit( &SX1272.DIO1, RADIO_DIO_1, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, CLEAR );

    /* pin setting for External interrupt of Lora DIO*/
    GpioInit( &SX1272.DIO2, RADIO_DIO_2, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, CLEAR );

}

/* This API will trigger the SX1272IRQ callback */
static void loraProcessInterfaceDIO0( void *pvParameter1, uint32_t ulParameter2 )
{
    (void)pvParameter1;
    (void)ulParameter2;
    SX1272OnDio0Irq();   /* Triggering DIO0 ISR callback */
}

/* This API will trigger the SX1272IRQ callback */
static void loraProcessInterfaceDIO1( void *pvParameter1, uint32_t ulParameter2 )
{
    (void)pvParameter1;
    (void)ulParameter2;
    SX1272OnDio1Irq();   /* Triggering DIO1 ISR callback */
}

/* This API will trigger the SX1272IRQ callback */
static void loraProcessInterfaceDIO2( void *pvParameter1, uint32_t ulParameter2 )
{
    (void)pvParameter1;
    (void)ulParameter2;
    SX1272OnDio2Irq();   /* Triggering DIO2 ISR callback */
}

/* LoRa DIO0 external interrupt callback  */
void loraDio0Callback(void)
{
    Retcode_T retVal;

    CmdProcessor_T *handleHighPrio = LoRaEngine_getHighPrioCmdProcessor();
    retVal = CmdProcessor_enqueueFromIsr(handleHighPrio, &loraProcessInterfaceDIO0, NULL, (uint32_t)NULL);
    /* Raising Error to handle from  ErrorHandler */
    if(RETCODE_OK != retVal )
    {
        Retcode_RaiseError(retVal);
    }
}

/* LoRa DIO1 external interrupt callback  */
void loraDio1Callback(void)
{
    Retcode_T retVal;
    CmdProcessor_T *handleHighPrio = LoRaEngine_getHighPrioCmdProcessor();
    retVal = CmdProcessor_enqueueFromIsr(handleHighPrio, &loraProcessInterfaceDIO1, NULL, (uint32_t)NULL);
    /* Raising Error to handle from  ErrorHandler */
    if(RETCODE_OK != retVal )
    {
        Retcode_RaiseError(retVal);
    }
}

/* LoRa DIO2 external interrupt callback  */
void loraDio2Callback(void)
{
    Retcode_T retVal;
    CmdProcessor_T *handleLowPrio = LoRaEngine_getHighPrioCmdProcessor();
    retVal = CmdProcessor_enqueueFromIsr(handleLowPrio, &loraProcessInterfaceDIO2, NULL, (uint32_t)NULL);
    /* Raising Error to handle from  ErrorHandler */
    if(RETCODE_OK != retVal )
    {
        Retcode_RaiseError(retVal);
    }
}

/*!
 * Initializes DIO IRQ handlers
 */
void SX1272IoIrqInit( DioIrqHandler **irqHandlers )
{
    (void)irqHandlers;

    /* The interrupt pin is configured in the PTD module for the Lora */
    GpioSetInterrupt( &SX1272.DIO0, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, (PTD_intrCallback)&loraDio0Callback );

    GpioSetInterrupt( &SX1272.DIO1, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, (PTD_intrCallback)&loraDio1Callback );

    GpioSetInterrupt( &SX1272.DIO2, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, (PTD_intrCallback)&loraDio2Callback );

}

/*!
 * De-initializes the radio I/Os pins interface.
 */
void SX1272IoDeInit( void )
{
    /* pin setting for chip select of LoRa */
    GpioInit( &SX1272.Spi.Nss, RADIO_NSS, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, SET );

    /* pin setting for External interrupt of Lora DIO*/
    GpioInit( &SX1272.DIO0, RADIO_DIO_0, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, CLEAR );

    /* pin setting for External interrupt of Lora DIO*/
    GpioInit( &SX1272.DIO1, RADIO_DIO_1, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, CLEAR );

    /* pin setting for External interrupt of Lora DIO*/
    GpioInit( &SX1272.DIO2, RADIO_DIO_2, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, CLEAR );
}

/*!
 * Gets the board PA selection configuration
 */
uint8_t SX1272GetPaSelect( uint32_t channel )
{
    (void)channel;
    return RF_PACONFIG_PASELECT_RFO;
}

/*!
 * Set the RF Switch I/Os pins in Low Power mode
 */
void SX1272SetAntSwLowPower( bool status )
{
    if( RadioIsActive != status )
    {
        RadioIsActive = status;

        if( status == false )
        {
            SX1272AntSwInit( );
        }
        else
        {
            SX1272AntSwDeInit( );
        }
    }
}

/*!
 * Initializes the RF Switch I/Os pins interface
 */
void SX1272AntSwInit( void )
{

}

/*!
 * De-initializes the RF Switch I/Os pins interface
 */
void SX1272AntSwDeInit( void )
{

}

/*!
 * Controls the antena switch if necessary.
 */
void SX1272SetAntSw( uint8_t rxTx )
{
    if( rxTx != 0 )
    {
        GpioWrite( &AntTxRx, SET );
    }
    else
    {
        GpioWrite( &AntTxRx, CLEAR );
    }
}

/*!
 * Checks if the given RF frequency is supported by the hardware
 */
bool SX1272CheckRfFrequency( uint32_t frequency )
{
    (void)frequency;
    return true;
}
