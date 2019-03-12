/*****************************************************************************/
/*
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *
 * ****************************************************************************/

/**
 * @ingroup UTILS
 *
 * @defgroup LEANB2CAP LeanB2CAP
 * @{
 *
 * @brief LeanB2CAP API for CoSP
 *
 * @details The LeanB2CAP is a binary framing protocol to transmit payload data
 * over a serial transport layer. The protocol is mainly designed to be used
 * together with stateless Client - Server architectures.
 *
 * The goal for the serial transport layer is to find bit errors and to be
 * able to resend corrupted frames. To achieve this goal LeanB2CAP has the
 * following frame classifications.
 *
 * Start Delimiter | Length  | Command Type | Check Sum 1 | Payload         | Check Sum 2
 * --------------- | ------- | ------------ | ----------- | --------------- | -----------
 * 1 byte          | 2 bytes | 1 byte       | 1 byte      | 1 - 65533 bytes | 1 byte
 *
 * The LeanB2CAP frame format is explained below.
 *
 * Start Delimiter is an unique byte identifying the start of a new LeanB2CAP frame.
 * The fixed value is 0x55.
 *
 * Length indicates the number of bytes to follow. It includes the sum of the following.
 *      Length of Command Type
 *      Length of Check Sum 1
 *      Length of Payload
 *      Length of Check Sum 2
 *
 * Command Type is any one of the protocol supported commands.
 * It is used to categorise and handle a mixture of important / unimportant data packets.
 *
 * Check Sum 1 is 8-bit CRC (polynomial value - 2) of the the following.
 *      Start Delimiter
 *      Length
 *      Command Type
 *
 * Payload is the actual data to be sent across the serial transport layer.
 * It must be at-least a byte long. And a maximum of 65533 bytes.
 *
 * Check Sum 2 is 8-bit CRC (polynomial value - 2) of the the following.
 *      Check Sum 1
 *      Payload
 *
 * A typical use of these API set would start with initializing the LeanB2CAP
 * instance. Then, a frame can be build to be sent in the serial line using
 * the builder. The received data's shall be sent to the receive processor
 * to trigger events based on data integrity.
 *
 *
 * @code{.c}
 * void LeanB2CAP_EventCallback(LeanB2CAP_Event_T Event, void * Data)
 * {
 *     switch(Event)
 *	   {
 *	   case LEAN_B2CAP_EVENT_FRAME_RX_COMPLETE:
 *	       LeanB2CAP_RawData_T * PayloadInfo = (LeanB2CAP_RawData_T *) Data;
 *	       // Payload information is available in PayloadInfo
 *	       break;
 *	   case LEAN_B2CAP_EVENT_FRAME_RX_ERROR:
 *	       Retcode_T * ErrorInfo = (Retcode_T *) Data;
 *	       // Error information is available in ErrorInfo
 *	       break;
 *	       // ...
 *	   }
 * }
 *
 * void LeanB2CAP_UsageExample()
 * {
 *     Retcode_T Rc = (Retcode_T) RETCODE_FAILURE;
 *     LeanB2CAP_HandlePtr_T LeanB2CAPHandle;
 *
 *     // Lean B2CAP handle instantiation
 *     Rc = LeanB2CAP_Initialize(&LeanB2CAPHandle, &LeanB2CAPEventCallback);
 *
 *     // Lean B2CAP data serialization
 *     LeanB2CAP_RawData_T RawData;
 *
 *     RawData.CommandType = LEAN_B2CAP_CMD_WRITE;
 *     RawData.DataPointer = PtrToDataForSerialization;
 *     RawData.DataSize = LengthOfDataToSerialization;
 *
 *     LeanB2CAP_FrameData_T OutputData;
 *     OutputData.FrameDataPointer = LeanB2CAPTransmitBuffer;
 *     Rc = LeanB2CAP_Builder(&RawData,&OutputData);
 *     UartSend(OutputData.FrameDataPointer, OutputData.FrameDataSize);
 *
 *     // Lean B2CAP data reception
 *     LeanB2CAP_FrameData_T FrameData;
 *     FrameData.FrameDataPointer = &LeanB2CAPReceiveBuffer;
 *     FrameData.FrameDataSize = UartReceive(LeanB2CAPReceiveBuffer, LEAN_B2CAP_RX_BUFFER_LEN);
 *
 *     // This would internally call the Lean B2CAP Event callback function
 *     LeanB2CAP_RxDataProcessor(&LeanB2CAPHandle,&FrameData);
 * }
 * @endcode
 *
 * @file
 */

#ifndef BCDS_LEANB2CAP_H_
#define BCDS_LEANB2CAP_H_

/* interface dependency checks */
#include "BCDS_UtilsConfig.h"

#if BCDS_FEATURE_LEANB2CAP

#include "BCDS_Retcode.h"

#define LEANB2CAP_HEADER_OVERHEAD           (5UL)               /**< LeanB2CAP header size */
#define LEANB2CAP_FOOTER_OVERHEAD           (1UL)               /**< LeanB2CAP footer size */

/**
 * @brief   Enum to represent the LeanB2CAP events.
 */
enum LeanB2CAP_Event_E
{
	LEAN_B2CAP_EVENT_FRAME_RX_COMPLETE,	    /**< A valid LeanB2CAP frame is received */
	LEAN_B2CAP_EVENT_FRAME_RX_ERROR,		/**< An invalid LeanB2CAP frame is encountered */
};

/**
 * @brief   Type definition to represent the LeanB2CAP event.
 */
typedef enum LeanB2CAP_Event_E LeanB2CAP_Event_T;

/**
 * @brief   Structure to represent any common data frames.
 */
struct LeanB2CAP_FrameData_S
{
    uint8_t* FrameDataPointer;	/**< Frame data start pointer */
    uint16_t FrameDataSize;		/**< Frame data start size */
};

/**
 * @brief   Type definition to represent any common data frames.
 */
typedef struct LeanB2CAP_FrameData_S LeanB2CAP_FrameData_T;

/**
 * @brief   Enum to represent the LeanB2CAP command types.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_READ
 * Read command.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_READ_LONG
 * Long read command.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_WRITE
 * Write command.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_WRITE_WITH_CONFIRMATION
 * Write with confirmation command.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_WRITE_LONG
 * Long write command.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_WRITE_LONG_WITH_CONFIRMATION
 * Long write with confirmation command.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_CONFIRMATION
 * Confirmation command.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_INFO_CHANNEL0
 * Information command from Channel 0.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_INFO_CHANNEL1
 * Information command from Channel 1.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_INFO_CHANNEL2
 * Information command from Channel 2.
 *
 * @var LeanB2CAP_CommandType_E::LEAN_B2CAP_CMD_INFO_CHANNEL3
 * Information command from Channel 3.
 */
enum LeanB2CAP_CommandType_E
{
    LEAN_B2CAP_CMD_READ = 'R', /* 52h */
    LEAN_B2CAP_CMD_READ_LONG = 'R' + 'L', /* 9Eh */
    LEAN_B2CAP_CMD_WRITE = 'W', /* 57h */
    LEAN_B2CAP_CMD_WRITE_WITH_CONFIRMATION = 'W' + 'C', /* 9Ah */
    LEAN_B2CAP_CMD_WRITE_LONG = 'W' + 'L', /* A3h */
    LEAN_B2CAP_CMD_WRITE_LONG_WITH_CONFIRMATION = 'W' + 'L' + 'C', /* E6h */
    LEAN_B2CAP_CMD_CONFIRMATION = 'C', /* 43h */
    LEAN_B2CAP_CMD_INFO_CHANNEL0 = 'I' + '0', /* 79h */
    LEAN_B2CAP_CMD_INFO_CHANNEL1 = 'I' + '1', /* 7Ah */
    LEAN_B2CAP_CMD_INFO_CHANNEL2 = 'I' + '2', /* 7Bh */
    LEAN_B2CAP_CMD_INFO_CHANNEL3 = 'I' + '3', /* 7Ch */
};

/**
 * @brief   Type definition to represent the LeanB2CAP command type.
 */
typedef enum LeanB2CAP_CommandType_E LeanB2CAP_CommandType_T;

/**
 * @brief   Structure to represent the raw data for LeanB2CAP frame building.
 */
struct LeanB2CAP_RawData_S
{
	LeanB2CAP_CommandType_T CommandType;	/**< Command type */
    uint8_t* DataPointer;					/**< Pointer to the data start point */
    uint16_t DataSize;						/**< size of the data */
};

/**
 * @brief   Type definition to represent the raw data for LeanB2CAP frame building.
 */
typedef struct LeanB2CAP_RawData_S LeanB2CAP_RawData_T;


/**
 * @brief   Structure to represent LeanB2CAP payload overhead (header and footer).
 */
struct LeanB2CAP_Overhead_S
{
    uint8_t* HeaderPointer;                 /**< Pointer to the header data */
    uint8_t* FooterPointer;                 /**< Pointer to the footer data */
    uint16_t HeaderLength;                  /**< Header data length */
    uint16_t FooterLength;                  /**< Footer data length */
};

/**
 * @brief   Type definition to represent LeanB2CAP payload overhead (header and footer).
 */
typedef struct LeanB2CAP_Overhead_S LeanB2CAP_Overhead_T;

/**
 * @brief   This is the data type for the LeanB2CAP event callback function.
 *
 * @details The callback is registered at the initialization. It is invoked by the
 *          LeanB2CAP received data processing API to notify the application about
 *          any LeanB2CAP events.
 *
 * @param[in] Event The event information.
 *
 * @param[in] Data The data based on the event.
 *                  This shall be cast to appropriate data type to retrieve valid information.
 *
 * @note Below are the Event to corresponding Data cast mapping.
 * 	  Event								    | Data
 * 	 -------------------------------------- | -------------------------------
 * 	  LEAN_B2CAP_EVENT_FRAME_RX_COMPLETE	| LeanB2CAP_RawData_T
 * 	  LEAN_B2CAP_EVENT_FRAME_RX_ERROR		| Retcode_T
 *
 * @see #LeanB2CAP_Event_E, #LeanB2CAP_FrameData_S, #Retcode_T, #Utils_Retcode_E
 */
typedef void (*LeanB2CAPCallback)(LeanB2CAP_Event_T Event, void * Data);

/**
 * @brief   Typedef for the LeanB2CAP handle.
 *
 * @details The actual handle context informations are hidden
 *          from the interface user.
 */
typedef struct LeanB2CAP_HandleDescriptor_S * LeanB2CAP_HandlePtr_T;


/**
 * @brief   Initializes an LeanB2CAP instance.
 *
 * @param[out]  Handle
 * LeanB2CAP handle. Use a void pointer to save the handle context address
 * as the Handle descriptor is hidden from the user. this shall be used
 * for further reference.
 *
 * @param[in] ValidOutputDataStorageBufferInfo
 * Valid output data/payload storage buffer information.
 * Must not be NULL.
 *
 * @param[in] Callback
 * Callback to intimate any LeanB2CAP Receive Process event.
 * Must not be NULL.
 *
 * @retval  #RETCODE_OK: if successful
 * @retval  #RETCODE_OUT_OF_RESOURCES: if no free handle is available
 * @retval  #RETCODE_NULL_POINTER: if NULL has been passed instead of a valid Callback
 */
Retcode_T LeanB2CAP_Initialize(LeanB2CAP_HandlePtr_T * Handle, LeanB2CAP_FrameData_T * ValidOutputDataStorageBufferInfo, LeanB2CAPCallback Callback);


/**
 * @brief   Handles the LeanB2CAP received data processing.
 *
 * @details The processed data results are intimated to the user in the
 *          same processing context via the registered callback.
 *
 * @note	The handle must be initialized prior to this by
 * making use of the LeanB2CAP_Initialize API.
 *
 * @param[in]  Handle
 * LeanB2CAP handle.
 *
 * @param[in] PayLoad
 * Payload information consisting of the received serial line
 * data pointer and its size.
 *
 * @retval	#RETCODE_OK: if successful
 * @retval 	#RETCODE_LEANB2CAP_RX_PROCESSOR_FAIL: unknown/un-handled application logic was encountered
 * @retval 	#RETCODE_FAILURE: if internal CRC calculation failed in the last byte processed
 * @retval  #RETCODE_LEANB2CAP_SD_ERROR: if invalid start delimiter error was encountered in the last byte processed
 * @retval  #RETCODE_LEANB2CAP_LENGTH_ERROR: if invalid length was encountered in the last byte processed
 * @retval  #RETCODE_LEANB2CAP_CMD_TYPE_ERROR: if invalid command type was encountered in the last byte processed
 * @retval  #RETCODE_LEANB2CAP_CRC_ERROR: if invalid CRC was encountered in the last byte processed
 * @retval  #RETCODE_LEANB2CAP_FRAME_ERROR: unknown/un-handled frame state was encountered
 * @retval  #RETCODE_LEANB2CAP_INVALID_CALLBACK: Callback for Lean B2CAP is NULL
 * @retval  #RETCODE_INVALID_PARAM: if Unknown Handle is passed instead of a valid Handle
 * @retval  #RETCODE_NULL_POINTER: NULL has been passed instead of a valid Handle
 */
Retcode_T LeanB2CAP_RxDataProcessor(LeanB2CAP_HandlePtr_T * Handle, LeanB2CAP_FrameData_T * PayLoad);


/**
 * @brief   Builds the LeanB2CAP payload.
 *
 * @note	This is completely independent of the handle.
 * Also, the input and output data buffers must be provided
 * by the interface user.
 *
 * @param[in,out] LeanB2CAPBuildOuput
 * LeanB2CAP payload that is built based on the input data
 * information. Internally the output buffer size is validated
 * against the input payload based on Lean B2CAP overhead.
 *
 * @param[in]  RawDataInput
 * The input data information based on which the resultant
 * LeanB2CAP payload is to be built.
 *
 * @retval	#RETCODE_OK: if successful
 * @retval 	#RETCODE_FAILURE: if internal CRC calculation failed
 * @retval  #RETCODE_LEANB2CAP_CMD_TYPE_ERROR: if invalid command type was provided
 * @retval  #RETCODE_LEANB2CAP_LENGTH_ERROR: if invalid length was provided
 * @retval  #RETCODE_NULL_POINTER: NULL has been passed instead of a valid input / output payload pointer
 */
Retcode_T LeanB2CAP_Builder(LeanB2CAP_FrameData_T * LeanB2CAPBuildOuput, LeanB2CAP_RawData_T * RawDataInput);

/**
 * @brief   Builds the LeanB2CAP payload overhead (header and footer).
 *
 * @note    This is completely independent of the handle.
 * Also, the input and output data buffers must be provided
 * by the interface user.
 *
 * @param[in/out] OutputOverhead
 * LeanB2CAP overhead that is created based on the input data
 * information. The HeaderPointer pointer must point to buffer of
 * size equalent to atleast LEANB2CAP_HEADER_OVERHEAD. Similarly,
 * the FooterPointer pointer must point to buffer of size equalent
 * to atleast LEANB2CAP_FOOTER_OVERHEAD.
 *
 * @param[in]  RawDataInput
 * The input data information based on which the resultant
 * LeanB2CAP payload is to be built.
 *
 * @retval  #RETCODE_OK: if successful
 * @retval  #RETCODE_FAILURE: if internal CRC calculation failed
 * @retval  #RETCODE_LEANB2CAP_CMD_TYPE_ERROR: if invalid command type was provided
 * @retval  #RETCODE_NULL_POINTER: NULL has been passed instead of a valid input / output payload pointer
 * @retval  #RETCODE_INVALID_PARAM: if invalid output buffer sizes or unsupported raw data length is provided
 */
Retcode_T LeanB2CAP_OverheadBuilder(LeanB2CAP_Overhead_T * OutputOverhead, LeanB2CAP_RawData_T * RawDataInput);

/**
 * @brief   Resets the LeanB2CAP received data process informations.
 *
 * @note	The handle must be initialized prior to this by
 * making use of the LeanB2CAP_Initialize API.
 *
 * @param[in]  Handle
 * LeanB2CAP handle.
 *
 * @retval	#RETCODE_OK: if successful
 * @retval  #RETCODE_INVALID_PARAM: if Unknown Handle is passed instead of a valid Handle
 * @retval  #RETCODE_NULL_POINTER: NULL has been passed instead of a valid Handle
 */
Retcode_T LeanB2CAP_ResetRxDataProcessor(LeanB2CAP_HandlePtr_T * Handle);


/**
 * @brief   Deinitializes an LeanB2CAP instance.
 *
 * @note	The handle must be initialized prior to this by
 * making use of the LeanB2CAP_Initialize API.
 *
 * @param[in]  Handle
 * LeanB2CAP handle.
 *
 * @retval	#RETCODE_OK: if successful
 * @retval  #RETCODE_INVALID_PARAM: if Unknown Handle is passed instead of a valid Handle
 * @retval  #RETCODE_NULL_POINTER: NULL has been passed instead of a valid Handle
 */
Retcode_T LeanB2CAP_Deinitialize(LeanB2CAP_HandlePtr_T * Handle);

#endif /* if BCDS_FEATURE_LEANB2CAP */

#endif /* BCDS_LEANB2CAP_H_ */
/**@} */
