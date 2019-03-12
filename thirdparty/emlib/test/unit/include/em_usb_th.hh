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
 * @brief Unit Test Mock-up Header
 *
 * This unit test header substitutes "em_usb.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */
#ifndef BCDS_EM_USB_TH_HH_
#define BCDS_EM_USB_TH_HH_

/* include system header files */
#include "em_device_th.hh"
#include "em_assert_th.hh"
#include "em_int_th.hh"
#include <stdint.h>

#if defined(USB_HOST)
#error "The test header specifies to provide the USB_DEVICE operation mode related interfaces.\
    But the user defined USB_HOST mode."
#endif
/*wrap the real em_usb.h interface header */
#define __EM_USB_H
#define EFM32_ALIGN(X)

#define STATIC_CONST_STRING_DESC_LANGID( _name, x, y )      \
typedef struct                                              \
{                                                           \
  uint8_t len;                                              \
  uint8_t type;                                             \
  uint8_t name[ 2 ];                                        \
} __attribute__ ((packed)) _##_name;                        \
static const _##_name _name __attribute__ ((aligned(4)))=   \
{                                                           \
  .len = 4,                                                 \
  .type = USB_STRING_DESCRIPTOR,                            \
  .name = { y, x }                                          \
}                                                           \

#define STATIC_CONST_STRING_DESC( _name, ... )                  \
typedef struct                                                  \
{                                                               \
  uint8_t  len;                                                 \
  uint8_t  type;                                                \
  uint8_t name[ 1 +sizeof( (uint8_t[]){__VA_ARGS__}) ];  \
} __attribute__ ((packed)) _##_name;                            \
static const _##_name _name __attribute__ ((aligned(4)))=       \
{                                                               \
	sizeof( _##_name ) - 2,                               \
	 USB_STRING_DESCRIPTOR,                                \
	{__VA_ARGS__},                                        \
}                                                               \


#define USB_SETUP_DIR_OUT       0               /**< Setup request data stage OUT direction value.     */
#define USB_SETUP_DIR_IN        1               /**< Setup request data stage IN direction value.      */
#define USB_SETUP_DIR_MASK      0x80            /**< Setup request data stage direction mask.          */
#define USB_SETUP_DIR_D2H       0x80            /**< Setup request data stage IN direction mask.       */
#define USB_SETUP_DIR_H2D       0x00            /**< Setup request data stage OUT direction mask.      */

/* SETUP request type */
#define USB_SETUP_TYPE_STANDARD       0         /**< Standard setup request value.                     */
#define USB_SETUP_TYPE_CLASS          1         /**< Class setup request value.                        */
#define USB_SETUP_TYPE_VENDOR         2         /**< Vendor setup request value.                       */
#define USB_SETUP_TYPE_STANDARD_MASK  0x00      /**< Standard setup request mask.                      */
#define USB_SETUP_TYPE_CLASS_MASK     0x20      /**< Class setup request mask.                         */
#define USB_SETUP_TYPE_VENDOR_MASK    0x40      /**< Vendor setup request mask.                        */

/* SETUP request recipient */
#define USB_SETUP_RECIPIENT_DEVICE    0         /**< Setup request device recipient value.             */
#define USB_SETUP_RECIPIENT_INTERFACE 1         /**< Setup request interface recipient value.          */
#define USB_SETUP_RECIPIENT_ENDPOINT  2         /**< Setup request endpoint recipient value.           */
#define USB_SETUP_RECIPIENT_OTHER     3         /**< Setup request other recipient value.              */

/* SETUP standard request codes for Full Speed devices */
#define GET_STATUS                0             /**< Standard setup request GET_STATUS.                */
#define CLEAR_FEATURE             1             /**< Standard setup request CLEAR_FEATURE.             */
#define SET_FEATURE               3             /**< Standard setup request SET_FEATURE.               */
#define SET_ADDRESS               5             /**< Standard setup request SET_ADDRESS.               */
#define GET_DESCRIPTOR            6             /**< Standard setup request GET_DESCRIPTOR.            */
#define SET_DESCRIPTOR            7             /**< Standard setup request SET_DESCRIPTOR.            */
#define GET_CONFIGURATION         8             /**< Standard setup request GET_CONFIGURATION.         */
#define SET_CONFIGURATION         9             /**< Standard setup request SET_CONFIGURATION.         */
#define GET_INTERFACE             10            /**< Standard setup request GET_INTERFACE.             */
#define SET_INTERFACE             11            /**< Standard setup request SET_INTERFACE.             */
#define SYNCH_FRAME               12            /**< Standard setup request SYNCH_FRAME.               */

/* SETUP class request codes */
#define USB_HID_GET_REPORT        0x01          /**< HID class setup request GET_REPORT.               */
#define USB_HID_GET_IDLE          0x02          /**< HID class setup request GET_IDLE.                 */
#define USB_HID_SET_REPORT        0x09          /**< HID class setup request SET_REPORT.               */
#define USB_HID_SET_IDLE          0x0A          /**< HID class setup request SET_IDLE.                 */
#define USB_HID_SET_PROTOCOL      0x0B          /**< HID class setup request SET_PROTOCOL.             */
#define USB_CDC_SETLINECODING     0x20          /**< CDC class setup request SET_LINE_CODING.          */
#define USB_CDC_GETLINECODING     0x21          /**< CDC class setup request GET_LINE_CODING.          */
#define USB_CDC_SETCTRLLINESTATE  0x22          /**< CDC class setup request SET_CONTROL_LINE_STATE.   */
#define USB_MSD_BOTRESET          0xFF          /**< MSD class setup request Bulk only transfer reset. */
#define USB_MSD_GETMAXLUN         0xFE          /**< MSD class setup request Get Max LUN.              */

/* SETUP command GET/SET_DESCRIPTOR decriptor types */
#define USB_DEVICE_DESCRIPTOR             1     /**< DEVICE descriptor value.                          */
#define USB_CONFIG_DESCRIPTOR             2     /**< CONFIGURATION descriptor value.                   */
#define USB_STRING_DESCRIPTOR             3     /**< STRING descriptor value.                          */
#define USB_INTERFACE_DESCRIPTOR          4     /**< INTERFACE descriptor value.                       */
#define USB_ENDPOINT_DESCRIPTOR           5     /**< ENDPOINT descriptor value.                        */
#define USB_DEVICE_QUALIFIER_DESCRIPTOR   6     /**< DEVICE_QUALIFIER descriptor value.                */
#define USB_OTHER_SPEED_CONFIG_DESCRIPTOR 7     /**< OTHER_SPEED_CONFIGURATION descriptor value.       */
#define USB_INTERFACE_POWER_DESCRIPTOR    8     /**< INTERFACE_POWER descriptor value.                 */
#define USB_INTERFACE_ASSOCIATION_DESCRIPTOR 11 /**< INTERFACE_ASSOCIATION descriptor value.           */
#define USB_HUB_DESCRIPTOR                0x29  /**< HUB descriptor value.                             */
#define USB_HID_DESCRIPTOR                0x21  /**< HID descriptor value.                             */
#define USB_HID_REPORT_DESCRIPTOR         0x22  /**< HID REPORT descriptor value.                      */
#define USB_CS_INTERFACE_DESCRIPTOR       0x24  /**< Audio Class-specific Descriptor Type.             */
#define USB_SMARTCARD_DESCRIPTOR          0x21  /**< Smartcard usb-ccid-specific Descriptor Type.      */

#define USB_DEVICE_DESCSIZE               18    /**< Device descriptor size.                           */
#define USB_CONFIG_DESCSIZE               9     /**< Configuration descriptor size.                    */
#define USB_INTERFACE_DESCSIZE            9     /**< Interface descriptor size.                        */
#define USB_ENDPOINT_DESCSIZE             7     /**< Endpoint descriptor size.                         */
#define USB_DEVICE_QUALIFIER_DESCSIZE     10    /**< Device qualifier descriptor size.                 */
#define USB_OTHER_SPEED_CONFIG_DESCSIZE   9     /**< Device other speed configuration descriptor size. */
#define USB_HID_DESCSIZE                  9     /**< HID descriptor size.                              */
#define USB_CDC_HEADER_FND_DESCSIZE       5     /**< CDC Header functional descriptor size.            */
#define USB_CDC_CALLMNG_FND_DESCSIZE      5     /**< CDC Call Management functional descriptor size.   */
#define USB_CDC_ACM_FND_DESCSIZE          4     /**< CDC Abstract Control Management functional descriptor size.*/
#define USB_INTERFACE_ASSOCIATION_DESCSIZE 8    /**< INTERFACE_ASSOCIATION descriptor size.            */
#define USB_SMARTCARD_DESCSIZE            54    /**< CCID descriptor size.                             */

/* Misc. USB definitions */
#define USB_EP0_SIZE           64               /**< The size of endpoint 0.                           */
#define USB_MAX_EP_SIZE        64               /**< The max size of any full speed endpoint.          */
#define USB_EPTYPE_CTRL        0                /**< Endpoint type control.                            */
#define USB_EPTYPE_ISOC        1                /**< Endpoint type isochron.                           */
#define USB_EPTYPE_BULK        2                /**< Endpoint type bulk.                               */
#define USB_EPTYPE_INTR        3                /**< Endpoint type interrupt.                          */
#define USB_EP_DIR_IN          0x80             /**< Endpoint direction mask.                          */
#define USB_SETUP_PKT_SIZE     8                /**< Setup request packet size.                        */
#define USB_EPNUM_MASK         0x0F             /**< Endpoint number mask.                             */
#define USB_LANGID_ENUS        0x0409           /**< English-United States language id.                */
#define USB_MAX_DEVICE_ADDRESS 127              /**< Maximum allowable device address.                 */

#define CONFIG_DESC_BM_REMOTEWAKEUP 0x20        /**< Configuration descriptor attribute macro.         */
#define CONFIG_DESC_BM_SELFPOWERED  0x40        /**< Configuration descriptor attribute macro.         */
#define CONFIG_DESC_BM_RESERVED_D7  0x80        /**< Configuration descriptor attribute macro.         */
#define CONFIG_DESC_BM_TRANSFERTYPE 0x03        /**< Configuration descriptor transfer type bitmask.   */
#define CONFIG_DESC_MAXPOWER_mA(x)  (((x)+1)/2) /**< Configuration descriptor power macro.             */

#define DEVICE_IS_SELFPOWERED       0x0001      /**< Standard request GET_STATUS bitmask.              */
#define REMOTE_WAKEUP_ENABLED       0x0002      /**< Standard request GET_STATUS bitmask.              */
#define USB_FEATURE_ENDPOINT_HALT         0     /**< Standard request CLEAR/SET_FEATURE bitmask.       */
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP  1     /**< Standard request CLEAR/SET_FEATURE bitmask.       */

#define HUB_FEATURE_PORT_RESET            4     /**< HUB class request CLEAR/SET_PORT_FEATURE feature selector. */
#define HUB_FEATURE_PORT_POWER            8     /**< HUB class request CLEAR/SET_PORT_FEATURE feature selector. */
#define HUB_FEATURE_C_PORT_CONNECTION     16    /**< HUB class request CLEAR/SET_PORT_FEATURE feature selector. */
#define HUB_FEATURE_C_PORT_RESET          20    /**< HUB class request CLEAR/SET_PORT_FEATURE feature selector. */
#define HUB_FEATURE_PORT_INDICATOR        22    /**< HUB class request CLEAR/SET_PORT_FEATURE feature selector. */

#define USB_CLASS_CDC                     2     /**< CDC device/interface class code.                    */
#define USB_CLASS_CDC_DATA                0x0A  /**< CDC Data interface class code.                      */
#define USB_CLASS_CDC_ACM                 2     /**< CDC Abstract Control Model interface subclass code. */
#define USB_CLASS_CDC_HFN                 0     /**< CDC class Header Functional Descriptor subtype.     */
#define USB_CLASS_CDC_CMNGFN              1     /**< CDC class Call Management Functional Descriptor subtype.*/
#define USB_CLASS_CDC_ACMFN               2     /**< CDC class Abstract Control Management Functional Descriptor subtype.*/
#define USB_CLASS_CDC_UNIONFN             6     /**< CDC class Union Functional Descriptor subtype.      */

#define USB_CLASS_HID                     3     /**< HID device/interface class code.                    */
#define USB_CLASS_HID_KEYBOARD            1     /**< HID keyboard interface protocol code.               */
#define USB_CLASS_HID_MOUSE               2     /**< HID mouse interface protocol code.                  */

#define USB_CLASS_HUB                     9     /**< HUB device/interface class code.                    */

#define USB_CLASS_MSD                     8     /**< MSD device/interface class code.                    */
#define USB_CLASS_MSD_BOT_TRANSPORT       0x50  /**< MSD Bulk Only Transport protocol.                   */
#define USB_CLASS_MSD_SCSI_CMDSET         6     /**< MSD Subclass SCSI transparent command set.          */
#define USB_CLASS_MSD_CSW_CMDPASSED       0     /**< MSD BOT Command status wrapper command passed code. */
#define USB_CLASS_MSD_CSW_CMDFAILED       1     /**< MSD BOT Command status wrapper command failed code. */
#define USB_CLASS_MSD_CSW_PHASEERROR      2     /**< MSD BOT Command status wrapper cmd phase error code.*/

/*** Triplet for the device descriptor of a composite device using IAD descriptors. ***/
#define USB_CLASS_MISCELLANEOUS           0xEF  /**< MISCELLANEOUS device class code.                    */
#define USB_CLASS_MISC_COMMON_SUBCLASS    2     /**< MISCELLANEOUS Common sub class code.                */
#define USB_CLASS_MISC_IAD_PROTOCOL       1     /**< MISCELLANEOUS Interface Association Descriptor protocol code. */

#define PORT_FULL_SPEED                   1     /**< Full speed return value for USBH_GetPortSpeed(). */
#define PORT_LOW_SPEED                    2     /**< Low speed return value for USBH_GetPortSpeed().  */


/* faked variables needs to be initialized by the test fixture */
typedef enum
{
    USBD_STATE_NONE = 0, /**< Device state is undefined/unknown.                */
    USBD_STATE_ATTACHED = 1, /**< Device state is ATTACHED.                         */
    USBD_STATE_POWERED = 2, /**< Device state is POWERED.                          */
    USBD_STATE_DEFAULT = 3, /**< Device state is DEFAULT.                          */
    USBD_STATE_ADDRESSED = 4, /**< Device state is ADDRESSED.                        */
    USBD_STATE_CONFIGURED = 5, /**< Device state is CONFIGURED.                       */
    USBD_STATE_SUSPENDED = 6, /**< Device state is SUSPENDED.                        */
    USBD_STATE_LASTMARKER = 7, /**< Device state enum end marker.                     */
} USBD_State_TypeDef;

typedef struct
{
    union
    {
        struct
        {
            union
            {
                struct
                {
                    uint8_t Recipient :5; /**< Request recipient (device, interface, endpoint or other).*/
                    uint8_t Type :2; /**< Request type (standard, class or vendor).         */
                    uint8_t Direction :1; /**< Transfer direction of SETUP data phase.           */
                };
                uint8_t bmRequestType; /**< Request characteristics.                          */
            };
            uint8_t bRequest; /**< Request code.                                     */
            uint16_t wValue; /**< Varies according to request.                      */
            uint16_t wIndex; /**< Index or offset, varies according to request.     */
            uint16_t wLength; /**< Number of bytes to transfer if there is a data stage.*/
        };
        uint32_t dw[2];
    };
}__attribute__ ((packed)) USB_Setup_TypeDef;
/** @brief USB transfer status enumerator. */
typedef enum
{
    /* NOTE: Please keep in sync with table errMsg[] in em_usbhal.c */
    USB_STATUS_OK = 0,         /**< No errors detected. */
    USB_STATUS_REQ_ERR = -1,    /**< Setup request error. */
    USB_STATUS_EP_BUSY = -2,    /**< Endpoint is busy. */
    USB_STATUS_REQ_UNHANDLED = -3, /**< Setup request not handled. */
    USB_STATUS_ILLEGAL = -4,    /**< Illegal operation attempted. */
    USB_STATUS_EP_STALLED = -5, /**< Endpoint is stalled.  */
    USB_STATUS_EP_ABORTED = -6, /**< Endpoint transfer was aborted.  */
    USB_STATUS_EP_ERROR = -7,   /**< Endpoint transfer error. */
    USB_STATUS_EP_NAK = -8,     /**< Endpoint NAK'ed transfer request. */
    USB_STATUS_DEVICE_UNCONFIGURED = -9, /**< Device is unconfigured.  */
    USB_STATUS_DEVICE_SUSPENDED = -10, /**< Device is suspended.*/
    USB_STATUS_DEVICE_RESET = -11, /**< Device is/was reset.*/
    USB_STATUS_TIMEOUT = -12,    /**< Transfer timeout.*/
    USB_STATUS_DEVICE_REMOVED = -13,     /**< Device was removed.*/
    USB_STATUS_HC_BUSY = -14,   /**< Host channel is busy. */
    USB_STATUS_DEVICE_MALFUNCTION = -15, /**< Malfunctioning device attached. */
    USB_STATUS_PORT_OVERCURRENT = -16, /**< VBUS shortcircuit/overcurrent failure. */
} USB_Status_TypeDef;
/** @} (end addtogroup USB_COMMON) */
typedef int  (*USBD_SetupCmdCb_TypeDef)( const USB_Setup_TypeDef *setup );
typedef void (*USBD_UsbResetCb_TypeDef)( void );
typedef void (*USBD_DeviceStateChangeCb_TypeDef)( USBD_State_TypeDef oldState, USBD_State_TypeDef newState );
typedef bool (*USBD_IsSelfPoweredCb_TypeDef)( void );
typedef void (*USBD_SofIntCb_TypeDef)( uint16_t sofNr );

typedef struct USBD_Callbacks_TypeDef
{
  const USBD_UsbResetCb_TypeDef           usbReset;               /**< Called whenever USB reset signalling is detected
                                                                       on the USB port.                                */
  const USBD_DeviceStateChangeCb_TypeDef  usbStateChange;         /**< Called whenever the device change state.        */
  const USBD_SetupCmdCb_TypeDef           setupCmd;               /**< Called on each setup request received from host.*/
  const USBD_IsSelfPoweredCb_TypeDef      isSelfPowered;          /**< Called whenever the device stack needs to query
                                                                       if the device is currently self- or bus-powered.
                                                                       Applies to devices which can operate in both modes.*/
  const USBD_SofIntCb_TypeDef             sofInt;                 /**< Called at each SOF interrupt. If NULL, the device
                                                                       stack will not enable the SOF interrupt.        */
} USBD_Callbacks_TypeDef;

typedef struct USBD_Callbacks_TypeDef const *USBD_Callbacks_TypeDef_Pointer;
typedef struct
{
    uint8_t bLength;         /**< Size of this descriptor in bytes                  */
    uint8_t bDescriptorType; /**< Constant DEVICE Descriptor Type                   */
    uint16_t bcdUSB;        /**< USB Specification Release Number in Binary-Coded
     Decimal                                           */
    uint8_t bDeviceClass; /**< Class code (assigned by the USB-IF) */
    uint8_t bDeviceSubClass; /**< Subclass code (assigned by the USB-IF) */
    uint8_t bDeviceProtocol; /**< Protocol code (assigned by the USB-IF)*/
    uint8_t bMaxPacketSize0; /**< Maximum packet size for endpoint zero*/
    uint16_t idVendor;      /**< Vendor ID (assigned by the USB-IF)*/
    uint16_t idProduct;     /**< Product ID (assigned by the manufacturer)*/
    uint16_t bcdDevice;     /**< Device release number in binary-coded decimal*/
    uint8_t iManufacturer; /**< Index of string descriptor describing manufacturer*/
    uint8_t iProduct;       /**< Index of string descriptor describing product     */
    uint8_t iSerialNumber; /**< Index of string descriptor describing the device
     serialnumber                                      */
    uint8_t bNumConfigurations; /**< Number of possible configurations*/
}__attribute__ ((packed)) USB_DeviceDescriptor_TypeDef;
typedef struct
{
    const USB_DeviceDescriptor_TypeDef *deviceDescriptor; /**< Pointer to a device descriptor.*/
    const uint8_t *configDescriptor; /**< Pointer to a configuration descriptor.         */
    const void * const *stringDescriptors; /**< Pointer to an array of string descriptor pointers.*/
    const uint8_t numberOfStrings; /**< Number of strings in string descriptor array.  */
    const uint8_t *bufferingMultiplier; /**< Pointer to an array defining the size of the
                                                 endpoint buffers. The size is given in
                                                 multiples of endpoint size. Generally a value
                                                                       of 1 (single) or 2 (double) buffering should be
                                                                       used.                                          */
    USBD_Callbacks_TypeDef_Pointer callbacks; /**< Pointer to struct with callbacks
                                                                       (@ref USBD_Callbacks_TypeDef). These callbacks
                                             are used by the device stack to signal events
                                                 to or query the application.  */
    const uint32_t reserved;             /**< Reserved for future use.  */
} USBD_Init_TypeDef;
typedef int (*USB_XferCompleteCb_TypeDef)(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
#define USB_SETUP_TYPE_CLASS          1         /**< Class setup request value.                        */
#define USB_SETUP_RECIPIENT_INTERFACE 1         /**< Setup request interface recipient value.          */
#define USB_CDC_GETLINECODING     0x21          /**< CDC class setup request GET_LINE_CODING.          */
#define USB_SETUP_DIR_IN        1               /**< Setup request data stage IN direction value.      */
#define USB_CDC_SETCTRLLINESTATE  0x22          /**< CDC class setup request SET_CONTROL_LINE_STATE.   */
/* SETUP request type */
#define USB_SETUP_TYPE_STANDARD       0         /**< Standard setup request value.                     */
#define USB_SETUP_TYPE_CLASS          1         /**< Class setup request value.                        */
#define USB_SETUP_TYPE_VENDOR         2         /**< Vendor setup request value.                       */
#define USB_SETUP_TYPE_STANDARD_MASK  0x00      /**< Standard setup request mask.                      */
#define USB_SETUP_TYPE_CLASS_MASK     0x20      /**< Class setup request mask.                         */
#define USB_SETUP_TYPE_VENDOR_MASK    0x40      /**< Vendor setup request mask.                        */
/* SETUP class request codes */
#define USB_HID_GET_REPORT        0x01          /**< HID class setup request GET_REPORT.               */
#define USB_HID_GET_IDLE          0x02          /**< HID class setup request GET_IDLE.                 */
#define USB_HID_SET_REPORT        0x09          /**< HID class setup request SET_REPORT.               */
#define USB_HID_SET_IDLE          0x0A          /**< HID class setup request SET_IDLE.                 */
#define USB_HID_SET_PROTOCOL      0x0B          /**< HID class setup request SET_PROTOCOL.             */
#define USB_CDC_SETLINECODING     0x20          /**< CDC class setup request SET_LINE_CODING.          */
#define USB_CDC_GETLINECODING     0x21          /**< CDC class setup request GET_LINE_CODING.          */
#define USB_CDC_SETCTRLLINESTATE  0x22          /**< CDC class setup request SET_CONTROL_LINE_STATE.   */
#define USB_MSD_BOTRESET          0xFF          /**< MSD class setup request Bulk only transfer reset. */
#define USB_MSD_GETMAXLUN         0xFE          /**< MSD class setup request Get Max LUN.              */
#define USB_CDC_SETLINECODING     0x20          /**< CDC class setup request SET_LINE_CODING.          */
#define USB_PRINTF(...)
#if !defined(__GNUC__)
#define        UBUF( x, y ) EFM32_ALIGN( 4 )        uint8_t x[((y)+3)&~3]
#define STATIC_UBUF( x, y ) EFM32_ALIGN( 4 ) static uint8_t x[((y)+3)&~3]
#else
#define        UBUF( x, y ) uint8_t x[((y)+3)&~3] __attribute__ ((aligned(4)))

/** Macro for creating WORD (4 byte) aligned static uint8_t arrays with size which
 *  is a multiple of WORD size.
 *  @n Example: @n STATIC_UBUF( rxBuffer, 37 );  =>  static uint8_t rxBuffer[ 40 ];
 */
#define STATIC_UBUF( x, y ) static uint8_t x[((y)+3)&~3] __attribute__ ((aligned(4)))
#endif

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC (USBTIMER_Init);
FAKE_VOID_FUNC(USBTIMER_DelayMs,uint32_t);
FAKE_VALUE_FUNC(int,USBD_Init,const USBD_Init_TypeDef *);
FAKE_VALUE_FUNC(int,USBD_Write,int , void *, int , USB_XferCompleteCb_TypeDef );
FAKE_VALUE_FUNC(int,USBD_Read,int , void *, int , USB_XferCompleteCb_TypeDef );
FAKE_VALUE_FUNC(USBD_State_TypeDef,USBD_GetUsbState);
FAKE_VOID_FUNC (USBD_Disconnect);
FAKE_VOID_FUNC (USBD_Stop);

#endif /* BCDS_EM_USB_TH_HH_ */

/** ************************************************************************* */
