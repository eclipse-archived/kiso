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

/**
 * @file
 *
 * @brief The engine of the Cellular driver.
 *
 * @details This module manages the AtResponseParser-task and CellularDriver-task and forwards data that is received by
 * the cellular communications channel to the AtResponseParser.
 *
 * The CellularDriver-task takes care of dispatching the AtResponse-queue and the CellularRequest-queue, it therefore manages
 * these two queues internally.
 *
 * The AtResponseParser-task is activated by incoming traffic from the BspCellular-rxCallbackas. When the
 * comm-channel holds sufficient data, the task will start execution and consume the rxBuffers' content. The data
 * is parsed by the AtResponseParser-module and results are stored into the AtResponse-queue. From there they may
 * be consumed by various modules inside the Cellular-driver.
 */

#ifndef CELLULAR_ENGINE_H
#define CELLULAR_ENGINE_H

/* Include all headers which are needed by this file. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "BCDS_MCU_UART.h"
#include "CellularAtResponseParser.h"
#include "BCDS_Cellular.h"

/**
 * @brief Cellular modem AT command footer
 */
#define ATCMD_AT_CRLF   "\r\n"

/**
 * @brief Bit-field to indicate modem initialization states.
 */
typedef struct
{
    uint32_t MeInitSuccessful :1;
    uint32_t MeAllFuncAvailable :1;
    uint32_t SimCardPinReady :1;
    uint32_t UseUsimCard :1;
    uint32_t SmsInitDone :1;
    uint32_t PhonebookInitDone :1;
    uint32_t MePoweredDown :1;
} CellularInitStatus_T;

extern CellularInitStatus_T CellularInitStatus;

/**
 * @brief Initializes the CellularEngine. Allocates necessary RTOS resources and starts the CellularDriver- and
 * AtResponseParser-task. It also initializes the request- and response-queue.
 *
 * @param [in]  EventCallback           Callback to be called in case of any event notification.
 *
 * @retval RETCODE_OK                   If initialization was successful.
 * @retval RETCODE_OUT_OF_RESOURCES     If necessary resources could not be allocated.
 * @retval RETCODE_INVALID_PARAM        If EventCallback is NULL
 */
Retcode_T CellularEngine_Init(Cellular_CallbackFunction_T EventCallback);

/**
 * @brief Deinitializes the CellularEngine. Disallocates necessary RTOS resources and stops the CellularDriver- and
 * AtResponseParser-task. It also uninitializes the request- and response-queue.
 *
 * @retval RETCODE_OK                   If deinitialization was successful.
 * @retval RETCODE_UNINITIALIZED        If necessary resources could not be uninitialized.
 */
Retcode_T CellularEngine_Deinit(void);

/**
 * @brief Enables or disables the fluke character filter. "Fluke characters" are all chars/bytes
 * 		  less than 0x20 and greater than 0x7E, except 0x0A and 0x0C (CR, LF). Such characters
 * 		  come to existence during the cellular startup, probably when the modem turns on the level
 * 		  shifter side.
 *
 * @warning Beware that the fluke filter might interfere with the modems regular operation and should
 * 			only be used during the modem startup phase.
 *
 * @param[in] FlukeFilterEnabled TRUE if the fluke filter is to be enabled, FALSE otherwise.
 */
void CellularEngine_SetFlukeCharFilterEnabled(bool FlukeFilterEnabled);

/**
 * @brief Sends a command to the modem
 *
 * @param[in] buffer The command to send
 * @param[in] BufferLength The length of the command to send
 *
 * @retval RETCODE_OK The command was sent
 * @retval RETCODE_UNITIALIZED The serial device was not initialized
 * @retval RETCODE_CELLULAR_BUSY If the modem kept sending and we didn't get a free slot
 */
Retcode_T CellularEngine_SendAtCommand(const uint8_t *buffer, uint32_t BufferLength);

/**
 * @brief Sends a command to the modem and wait for its echo.
 *
 * @param[in] timeout The time to wait for a command echo in milliseconds
 * @param[in] str The command to send
 *
 * @retval RETCODE_OK The command was sent
 * @retval RETCODE_UNITIALIZED The serial device was not initialized
 * @retval RETCODE_CELLULAR_BUSY If the modem kept sending and we didn't get a free slot
 */
Retcode_T CellularEngine_SendAtCmdWaitEcho(uint32_t timeout, const char *str);

/**
 * @brief   Get device status
 *
 * @return  status
 *          CELLULAR_DEVICESTATUS_IDLE
 *          CELLULAR_DEVICESTATUS_INIT
 *          CELLULAR_DEVICESTATUS_CONNECTED
 *          CELLULAR_DEVICESTATUS_DISCONNECTED
 */
Cellular_DeviceStatus_T CellularEngine_GetDeviceStatus(void);

/**
 * @brief Set device status and notify a subscriber with an event
 *
 * @param[in] status The status of the modem
 * @param[in] event The event to be sent to the subscriber
 * @param[in] info The data to be passed to the subscriber
 *
 */
void CellularEngine_SetDeviceStatus(Cellular_DeviceStatus_T status, Cellular_CallbackEvent_T event, void *info);

/**
 * @brief   Get device error
 *
 * @return  device error
 *
 */
Retcode_T CellularEngine_GetDeviceError(void);

/**
 * @brief   Set device error
 *
 * @param[in] error The error code to be set
 *
 */
void CellularEngine_SetDeviceError(Retcode_T error);

typedef Retcode_T (*CellularRequest_CallableFunction_T)(void *parameter, uint32_t ParameterLength);

/**
 * @brief Enqueues the given item for execution by the CellularDriver-task. The caller can wait for the queue to become
 * available (if full), by setting the parameter timeout to x > 0. If a timeout of 0 is passed, the caller will not
 * wait for the queue to be available and perhaps return RETCODE_CELLULAR_DRIVER_BUSY if the queue is full.
 *
 * @param timeout The max. timeout in ticks that the caller should be waiting for the availability of dispatcher.
 * @param function A valid pointer to a CellularRequest_CallableFunction_T.
 * @param parameter An optional pointer to some user-defined structure to be passed to the CellularRequest_CallableFunction_T-execution (can be NULL).
 * @param ParameterLength An optional length to be passed to the CellularRequest_CallableFunction_T-execution.
 *
 * @retval RETCODE_OK                   If the given item was successfully executed by the CellularDriver.
 * @retval RETCODE_CELLULAR_DRIVER_BUSY    If the item could not be executed before the end of the given timeout.
 * @retval RETCODE_INVALID_PARAM        If one or more of the parameters is invalid
 */
Retcode_T CellularRequest_Dispatch(uint32_t timeout, CellularRequest_CallableFunction_T function, void* parameter, uint32_t ParameterLength);

/**
 * @brief Type to pass necessary parameters to custom response parser.
 */
typedef struct
{
    SemaphoreHandle_t Semaphore;
    UART_T Device;
} AtResponseParserArgument_T;

typedef Retcode_T (*AtResponseParserCallback_T)(AtResponseParserArgument_T *);

/**
 * @brief Set custom AT response parser to override the default one.
 *
 * @param callback A pointer to callback function to be used as custom AT response parser.
 */
void CellularEngine_SetCustomAtResponseParser(AtResponseParserCallback_T callback);

/**
 * @brief Handles response queue events, removes unhandled.
 *
 * @retval RETCODE_OK                       If any event has been handled by the URC handler.
 * @retval RETCODE_CELLULAR_URC_NOT_PRESENT    If an unhandled event has been received.
 *
 */
Retcode_T CellularEngine_HandleEvents(void);

/**
 * @brief Read incoming data from UART
 *
 * @param[out] data         Pointer to the buffer where the data has to be copied
 *
 * @param[in] dataLength    Number of bytes to read
 *
 * @param[out] dataRead     Number of bytes read
 *
 * @retval RETCODE_OK               If no errors occur during the read
 * @retval RETCODE_NULL_POINTER     If one of the pointer is NULL
 *
 */
Retcode_T Cellular_ReadData(uint8_t* data, uint32_t dataLength, uint32_t* dataRead);

#endif /* CELLULAR_ENGINE_H */
