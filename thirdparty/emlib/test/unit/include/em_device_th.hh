 /*******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
 /**************************************************************************
 * Modified by Bosch Connected Devices and Solutions GmbH. Modifications are
 *		-	Fake functions are created for unit testing
 * ****************************************************************************/
/* header definition ******************************************************** */
#ifndef EM_DEVICE_TH_HH_
#define EM_DEVICE_TH_HH_

/* include system header files */

/* ban or wrap the real em_device.h interface header */
#define EM_DEVICE_H
#define __STATIC_INLINE  static inline
#define __INLINE inline
void WDG_enableWdgTimer(void);

/* faked variables needs to be initialized by the test fixture */
#define     __IO    volatile
#define   __I     volatile
#define   __O     volatile

#define USART_PRESENT
#define USART_COUNT    3
#define GPIO_PRESENT

/** Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers *******************************************/
  NonMaskableInt_IRQn   = -14,              /*!< 2 Non Maskable Interrupt                 */
  HardFault_IRQn        = -13,              /*!< 3 Cortex-M3 Hard Fault Interrupt         */
  MemoryManagement_IRQn = -12,              /*!< 4 Cortex-M3 Memory Management Interrupt  */
  BusFault_IRQn         = -11,              /*!< 5 Cortex-M3 Bus Fault Interrupt          */
  UsageFault_IRQn       = -10,              /*!< 6 Cortex-M3 Usage Fault Interrupt        */
  SVCall_IRQn           = -5,               /*!< 11 Cortex-M3 SV Call Interrupt           */
  DebugMonitor_IRQn     = -4,               /*!< 12 Cortex-M3 Debug Monitor Interrupt     */
  PendSV_IRQn           = -2,               /*!< 14 Cortex-M3 Pend SV Interrupt           */
  SysTick_IRQn          = -1,               /*!< 15 Cortex-M3 System Tick Interrupt       */

/******  EFM32LG Peripheral Interrupt Numbers *********************************************/
  DMA_IRQn              = 0,  /*!< 16+0 EFM32 DMA Interrupt */
  GPIO_EVEN_IRQn        = 1,  /*!< 16+1 EFM32 GPIO_EVEN Interrupt */
  TIMER0_IRQn           = 2,  /*!< 16+2 EFM32 TIMER0 Interrupt */
  USART0_RX_IRQn        = 3,  /*!< 16+3 EFM32 USART0_RX Interrupt */
  USART0_TX_IRQn        = 4,  /*!< 16+4 EFM32 USART0_TX Interrupt */
  USB_IRQn              = 5,  /*!< 16+5 EFM32 USB Interrupt */
  ACMP0_IRQn            = 6,  /*!< 16+6 EFM32 ACMP0 Interrupt */
  ADC0_IRQn             = 7,  /*!< 16+7 EFM32 ADC0 Interrupt */
  DAC0_IRQn             = 8,  /*!< 16+8 EFM32 DAC0 Interrupt */
  I2C0_IRQn             = 9,  /*!< 16+9 EFM32 I2C0 Interrupt */
  I2C1_IRQn             = 10, /*!< 16+10 EFM32 I2C1 Interrupt */
  GPIO_ODD_IRQn         = 11, /*!< 16+11 EFM32 GPIO_ODD Interrupt */
  TIMER1_IRQn           = 12, /*!< 16+12 EFM32 TIMER1 Interrupt */
  TIMER2_IRQn           = 13, /*!< 16+13 EFM32 TIMER2 Interrupt */
  TIMER3_IRQn           = 14, /*!< 16+14 EFM32 TIMER3 Interrupt */
  USART1_RX_IRQn        = 15, /*!< 16+15 EFM32 USART1_RX Interrupt */
  USART1_TX_IRQn        = 16, /*!< 16+16 EFM32 USART1_TX Interrupt */
  LESENSE_IRQn          = 17, /*!< 16+17 EFM32 LESENSE Interrupt */
  USART2_RX_IRQn        = 18, /*!< 16+18 EFM32 USART2_RX Interrupt */
  USART2_TX_IRQn        = 19, /*!< 16+19 EFM32 USART2_TX Interrupt */
  UART0_RX_IRQn         = 20, /*!< 16+20 EFM32 UART0_RX Interrupt */
  UART0_TX_IRQn         = 21, /*!< 16+21 EFM32 UART0_TX Interrupt */
  UART1_RX_IRQn         = 22, /*!< 16+22 EFM32 UART1_RX Interrupt */
  UART1_TX_IRQn         = 23, /*!< 16+23 EFM32 UART1_TX Interrupt */
  LEUART0_IRQn          = 24, /*!< 16+24 EFM32 LEUART0 Interrupt */
  LEUART1_IRQn          = 25, /*!< 16+25 EFM32 LEUART1 Interrupt */
  LETIMER0_IRQn         = 26, /*!< 16+26 EFM32 LETIMER0 Interrupt */
  PCNT0_IRQn            = 27, /*!< 16+27 EFM32 PCNT0 Interrupt */
  PCNT1_IRQn            = 28, /*!< 16+28 EFM32 PCNT1 Interrupt */
  PCNT2_IRQn            = 29, /*!< 16+29 EFM32 PCNT2 Interrupt */
  RTC_IRQn              = 30, /*!< 16+30 EFM32 RTC Interrupt */
  BURTC_IRQn            = 31, /*!< 16+31 EFM32 BURTC Interrupt */
  CMU_IRQn              = 32, /*!< 16+32 EFM32 CMU Interrupt */
  VCMP_IRQn             = 33, /*!< 16+33 EFM32 VCMP Interrupt */
  LCD_IRQn              = 34, /*!< 16+34 EFM32 LCD Interrupt */
  MSC_IRQn              = 35, /*!< 16+35 EFM32 MSC Interrupt */
  AES_IRQn              = 36, /*!< 16+36 EFM32 AES Interrupt */
  EBI_IRQn              = 37, /*!< 16+37 EFM32 EBI Interrupt */
  EMU_IRQn              = 38, /*!< 16+38 EFM32 EMU Interrupt */
} IRQn_Type;


/**************************************************************************//**
 * EFM32G210F128_CMU Register Declaration
 *****************************************************************************/
typedef struct
{
  __IO uint32_t CTRL;         /**< CMU Control Register  */
  __IO uint32_t HFCORECLKDIV; /**< High Frequency Core Clock Division Register  */
  __IO uint32_t HFPERCLKDIV;  /**< High Frequency Peripheral Clock Division Register  */
  __IO uint32_t HFRCOCTRL;    /**< HFRCO Control Register  */
  __IO uint32_t LFRCOCTRL;    /**< LFRCO Control Register  */
  __IO uint32_t AUXHFRCOCTRL; /**< AUXHFRCO Control Register  */
  __IO uint32_t CALCTRL;      /**< Calibration Control Register  */
  __IO uint32_t CALCNT;       /**< Calibration Counter Register  */
  __IO uint32_t OSCENCMD;     /**< Oscillator Enable/Disable Command Register  */
  __IO uint32_t CMD;          /**< Command Register  */
  __IO uint32_t LFCLKSEL;     /**< Low Frequency Clock Select Register  */
  __I uint32_t  STATUS;       /**< Status Register  */
  __I uint32_t  IF;           /**< Interrupt Flag Register  */
  __IO uint32_t IFS;          /**< Interrupt Flag Set Register  */
  __IO uint32_t IFC;          /**< Interrupt Flag Clear Register  */
  __IO uint32_t IEN;          /**< Interrupt Enable Register  */
  __IO uint32_t HFCORECLKEN0; /**< High Frequency Core Clock Enable Register 0  */
  __IO uint32_t HFPERCLKEN0;  /**< High Frequency Peripheral Clock Enable Register 0  */
  uint32_t      RESERVED0[2]; /**< Reserved for future use **/
  __I uint32_t  SYNCBUSY;     /**< Synchronization Busy Register  */
  __IO uint32_t FREEZE;       /**< Freeze Register  */
  __IO uint32_t LFACLKEN0;    /**< Low Frequency A Clock Enable Register 0  (Async Reg)  */
  uint32_t      RESERVED1[1]; /**< Reserved for future use **/
  __IO uint32_t LFBCLKEN0;    /**< Low Frequency B Clock Enable Register 0 (Async Reg)  */
  uint32_t      RESERVED2[1]; /**< Reserved for future use **/
  __IO uint32_t LFAPRESC0;    /**< Low Frequency A Prescaler Register 0 (Async Reg)  */
  uint32_t      RESERVED3[1]; /**< Reserved for future use **/
  __IO uint32_t LFBPRESC0;    /**< Low Frequency B Prescaler Register 0  (Async Reg)  */
  uint32_t      RESERVED4[1]; /**< Reserved for future use **/
  __IO uint32_t PCNTCTRL;     /**< PCNT Control Register  */

  uint32_t      RESERVED5[1]; /**< Reserved for future use **/
  __IO uint32_t ROUTE;        /**< I/O Routing Register  */
  __IO uint32_t LOCK;         /**< Configuration Lock Register  */
} CMU_TypeDef;


typedef struct
{
  __IO uint32_t CTRL;         /**< Control Register  */
  __IO uint32_t CMD;          /**< Command Register  */
  __I uint32_t  STATUS;       /**< Status Register  */
  __I uint32_t  CNT;          /**< Counter Value Register  */
  __IO uint32_t COMP0;        /**< Compare Value Register 0  */
  __IO uint32_t COMP1;        /**< Compare Value Register 1  */
  __IO uint32_t REP0;         /**< Repeat Counter Register 0  */
  __IO uint32_t REP1;         /**< Repeat Counter Register 1  */
  __I uint32_t  IF;           /**< Interrupt Flag Register  */
  __IO uint32_t IFS;          /**< Interrupt Flag Set Register  */
  __IO uint32_t IFC;          /**< Interrupt Flag Clear Register  */
  __IO uint32_t IEN;          /**< Interrupt Enable Register  */

  __IO uint32_t FREEZE;       /**< Freeze Register  */
  __I uint32_t  SYNCBUSY;     /**< Synchronization Busy Register  */

  uint32_t      RESERVED0[2]; /**< Reserved for future use **/
  __IO uint32_t ROUTE;        /**< I/O Routing Register  */
} LETIMER_TypeDef;


/**************************************************************************//**
 * GPIO_P EFM32G GPIO P
 *****************************************************************************/
typedef struct
{
  __IO uint32_t CTRL;     /**< Port Control Register  */
  __IO uint32_t MODEL;    /**< Port Pin Mode Low Register  */
  __IO uint32_t MODEH;    /**< Port Pin Mode High Register  */
  __IO uint32_t DOUT;     /**< Port Data Out Register  */
  __O uint32_t  DOUTSET;  /**< Port Data Out Set Register  */
  __O uint32_t  DOUTCLR;  /**< Port Data Out Clear Register  */
  __O uint32_t  DOUTTGL;  /**< Port Data Out Toggle Register  */
  __I uint32_t  DIN;      /**< Port Data In Register  */
  __IO uint32_t PINLOCKN; /**< Port Unlocked Pins Register  */
} GPIO_P_TypeDef;


typedef struct
{
  GPIO_P_TypeDef P[6];          /**< Port configuration bits */

  uint32_t       RESERVED0[10]; /**< Reserved for future use **/
  __IO uint32_t  EXTIPSELL;     /**< External Interrupt Port Select Low Register  */
  __IO uint32_t  EXTIPSELH;     /**< External Interrupt Port Select High Register  */
  __IO uint32_t  EXTIRISE;      /**< External Interrupt Rising Edge Trigger Register  */
  __IO uint32_t  EXTIFALL;      /**< External Interrupt Falling Edge Trigger Register  */
  __IO uint32_t  IEN;           /**< Interrupt Enable Register  */
  __I uint32_t   IF;            /**< Interrupt Flag Register  */
  __IO uint32_t  IFS;           /**< Interrupt Flag Set Register  */
  __IO uint32_t  IFC;           /**< Interrupt Flag Clear Register  */

  __IO uint32_t  ROUTE;         /**< I/O Routing Register  */
  __IO uint32_t  INSENSE;       /**< Input Sense Register  */
  __IO uint32_t  LOCK;          /**< Configuration Lock Register  */
  __IO uint32_t  CTRL;          /**< GPIO Control Register  */
  __IO uint32_t  CMD;           /**< GPIO Command Register  */
  __IO uint32_t  EM4WUEN;       /**< EM4 Wake-up Enable Register  */
  __IO uint32_t  EM4WUPOL;      /**< EM4 Wake-up Polarity Register  */
  __I uint32_t   EM4WUCAUSE;    /**< EM4 Wake-up Cause Register  */
} GPIO_TypeDef;                 /** @} */
/**************************************************************************//**
 * EFM32GG_DEVINFO
 *****************************************************************************/
typedef struct
{
  __I uint32_t CAL;          /**< Calibration temperature and checksum */
  __I uint32_t ADC0CAL0;     /**< ADC0 Calibration register 0 */
  __I uint32_t ADC0CAL1;     /**< ADC0 Calibration register 1 */
  __I uint32_t ADC0CAL2;     /**< ADC0 Calibration register 2 */
  uint32_t     RESERVED0[2]; /**< Reserved */
  __I uint32_t DAC0CAL0;     /**< DAC calibrartion register 0 */
  __I uint32_t DAC0CAL1;     /**< DAC calibrartion register 1 */
  __I uint32_t DAC0CAL2;     /**< DAC calibrartion register 2 */
  __I uint32_t AUXHFRCOCAL0; /**< AUXHFRCO calibration register 0 */
  __I uint32_t AUXHFRCOCAL1; /**< AUXHFRCO calibration register 1 */
  __I uint32_t HFRCOCAL0;    /**< HFRCO calibration register 0 */
  __I uint32_t HFRCOCAL1;    /**< HFRCO calibration register 1 */
  __I uint32_t MEMINFO;      /**< Memory information */
  uint32_t     RESERVED2[2]; /**< Reserved */
  __I uint32_t UNIQUEL;      /**< Low 32 bits of device unique number */
  __I uint32_t UNIQUEH;      /**< High 32 bits of device unique number */
  __I uint32_t MSIZE;        /**< Flash and SRAM Memory size in KiloBytes */
  __I uint32_t PART;         /**< Part description */
} DEVINFO_TypeDef;           /** @} */

/** \brief  Structure type to access the System Control Block (SCB).
 */
typedef struct
{
  __I  uint32_t CPUID;                   /*!< Offset: 0x000 (R/ )  CPUID Base Register                                   */
  __IO uint32_t ICSR;                    /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register                  */
  __IO uint32_t VTOR;                    /*!< Offset: 0x008 (R/W)  Vector Table Offset Register                          */
  __IO uint32_t AIRCR;                   /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register      */
  __IO uint32_t SCR;                     /*!< Offset: 0x010 (R/W)  System Control Register                               */
  __IO uint32_t CCR;                     /*!< Offset: 0x014 (R/W)  Configuration Control Register                        */
  __IO uint8_t  SHP[12];                 /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
  __IO uint32_t SHCSR;                   /*!< Offset: 0x024 (R/W)  System Handler Control and State Register             */
  __IO uint32_t CFSR;                    /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register                    */
  __IO uint32_t HFSR;                    /*!< Offset: 0x02C (R/W)  HardFault Status Register                             */
  __IO uint32_t DFSR;                    /*!< Offset: 0x030 (R/W)  Debug Fault Status Register                           */
  __IO uint32_t MMFAR;                   /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register                      */
  __IO uint32_t BFAR;                    /*!< Offset: 0x038 (R/W)  BusFault Address Register                             */
  __IO uint32_t AFSR;                    /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register                       */
  __I  uint32_t PFR[2];                  /*!< Offset: 0x040 (R/ )  Processor Feature Register                            */
  __I  uint32_t DFR;                     /*!< Offset: 0x048 (R/ )  Debug Feature Register                                */
  __I  uint32_t ADR;                     /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register                            */
  __I  uint32_t MMFR[4];                 /*!< Offset: 0x050 (R/ )  Memory Model Feature Register                         */
  __I  uint32_t ISAR[5];                 /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register                   */
       uint32_t RESERVED0[5];
  __IO uint32_t CPACR;                   /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register                   */
} SCB_Type;


#define GPIO_BASE         								  (0x40006000UL) 				/**< GPIO base address  */
#define GPIO         									  ((GPIO_TypeDef *) GPIO_BASE)  /**< GPIO base pointer */
#define _GPIO_ROUTE_SWCLKPEN_SHIFT                        0
#define _GPIO_ROUTE_SWDIOPEN_SHIFT                        1
#define _GPIO_LOCK_LOCKKEY_LOCK                           0x00000000UL
#define _GPIO_P_DIN_DIN_MASK                              0xFFFFUL
#define _GPIO_P_DIN_DIN_MASK                              0xFFFFUL
#define GPIO_LOCK_LOCKKEY_LOCK                            (_GPIO_LOCK_LOCKKEY_LOCK << 0)
#define _GPIO_ROUTE_SWOPEN_SHIFT                          2
#define _GPIO_P_DOUTCLR_DOUTCLR_MASK                      0xFFFFUL
#define _GPIO_P_DOUT_DOUT_MASK                            0xFFFFUL
#define _GPIO_P_DOUTSET_DOUTSET_MASK                      0xFFFFUL
#define GPIO_LOCK_LOCKKEY_UNLOCK                          (_GPIO_LOCK_LOCKKEY_UNLOCK << 0)
#define _GPIO_LOCK_LOCKKEY_UNLOCK                         0x0000A534UL
#define _GPIO_P_DOUTTGL_DOUTTGL_MASK                      0xFFFFUL


/* Bit fields for GPIO P_CTRL */
#define _GPIO_P_CTRL_RESETVALUE                           0x00000000UL                           /**< Default value for GPIO_P_CTRL */
#define _GPIO_P_CTRL_MASK                                 0x00000003UL                           /**< Mask for GPIO_P_CTRL */
#define _GPIO_P_CTRL_DRIVEMODE_SHIFT                      0                                      /**< Shift value for GPIO_DRIVEMODE */
#define _GPIO_P_CTRL_DRIVEMODE_MASK                       0x3UL                                  /**< Bit mask for GPIO_DRIVEMODE */
#define _GPIO_P_CTRL_DRIVEMODE_DEFAULT                    0x00000000UL                           /**< Mode DEFAULT for GPIO_P_CTRL */
#define _GPIO_P_CTRL_DRIVEMODE_STANDARD                   0x00000000UL                           /**< Mode STANDARD for GPIO_P_CTRL */
#define _GPIO_P_CTRL_DRIVEMODE_LOWEST                     0x00000001UL                           /**< Mode LOWEST for GPIO_P_CTRL */
#define _GPIO_P_CTRL_DRIVEMODE_HIGH                       0x00000002UL                           /**< Mode HIGH for GPIO_P_CTRL */
#define _GPIO_P_CTRL_DRIVEMODE_LOW                        0x00000003UL                           /**< Mode LOW for GPIO_P_CTRL */
#define GPIO_P_CTRL_DRIVEMODE_DEFAULT                     (_GPIO_P_CTRL_DRIVEMODE_DEFAULT << 0)  /**< Shifted mode DEFAULT for GPIO_P_CTRL */
#define GPIO_P_CTRL_DRIVEMODE_STANDARD                    (_GPIO_P_CTRL_DRIVEMODE_STANDARD << 0) /**< Shifted mode STANDARD for GPIO_P_CTRL */
#define GPIO_P_CTRL_DRIVEMODE_LOWEST                      (_GPIO_P_CTRL_DRIVEMODE_LOWEST << 0)   /**< Shifted mode LOWEST for GPIO_P_CTRL */
#define GPIO_P_CTRL_DRIVEMODE_HIGH                        (_GPIO_P_CTRL_DRIVEMODE_HIGH << 0)     /**< Shifted mode HIGH for GPIO_P_CTRL */
#define GPIO_P_CTRL_DRIVEMODE_LOW                         (_GPIO_P_CTRL_DRIVEMODE_LOW << 0)      /**< Shifted mode LOW for GPIO_P_CTRL */


/* Bit fields for GPIO P_MODEL */
#define _GPIO_P_MODEL_RESETVALUE                          0x00000000UL                                          /**< Default value for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MASK                                0xFFFFFFFFUL                                          /**< Mask for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_SHIFT                         0                                                     /**< Shift value for GPIO_MODE0 */
#define _GPIO_P_MODEL_MODE0_MASK                          0xFUL                                                 /**< Bit mask for GPIO_MODE0 */
#define _GPIO_P_MODEL_MODE0_DEFAULT                       0x00000000UL                                          /**< Mode DEFAULT for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_DISABLED                      0x00000000UL                                          /**< Mode DISABLED for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_INPUT                         0x00000001UL                                          /**< Mode INPUT for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_INPUTPULL                     0x00000002UL                                          /**< Mode INPUTPULL for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_INPUTPULLFILTER               0x00000003UL                                          /**< Mode INPUTPULLFILTER for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_PUSHPULL                      0x00000004UL                                          /**< Mode PUSHPULL for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_PUSHPULLDRIVE                 0x00000005UL                                          /**< Mode PUSHPULLDRIVE for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_WIREDOR                       0x00000006UL                                          /**< Mode WIREDOR for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_WIREDORPULLDOWN               0x00000007UL                                          /**< Mode WIREDORPULLDOWN for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_WIREDAND                      0x00000008UL                                          /**< Mode WIREDAND for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_WIREDANDFILTER                0x00000009UL                                          /**< Mode WIREDANDFILTER for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_WIREDANDPULLUP                0x0000000AUL                                          /**< Mode WIREDANDPULLUP for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_WIREDANDPULLUPFILTER          0x0000000BUL                                          /**< Mode WIREDANDPULLUPFILTER for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_WIREDANDDRIVE                 0x0000000CUL                                          /**< Mode WIREDANDDRIVE for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_WIREDANDDRIVEFILTER           0x0000000DUL                                          /**< Mode WIREDANDDRIVEFILTER for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_WIREDANDDRIVEPULLUP           0x0000000EUL                                          /**< Mode WIREDANDDRIVEPULLUP for GPIO_P_MODEL */
#define _GPIO_P_MODEL_MODE0_WIREDANDDRIVEPULLUPFILTER     0x0000000FUL                                          /**< Mode WIREDANDDRIVEPULLUPFILTER for GPIO_P_MODEL */

#define CMU_HFCORECLKEN0_DMA                       (0x1UL << 1)                         /**< Direct Memory Access Controller Clock Enable */
uint32_t cmuAddr;
#define CMU_BASE         						   (&cmuAddr) 						/**< CMU base address  */
#define CMU          							   ((CMU_TypeDef*)CMU_BASE)             /**< CMU base pointer */
#define _CMU_LOCK_LOCKKEY_LOCK                     0x00000000UL
#define CMU_LOCK_LOCKKEY_LOCK                      (_CMU_LOCK_LOCKKEY_LOCK << 0)
#define _CMU_LOCK_LOCKKEY_UNLOCKED                 0x00000000UL
#define CMU_LOCK_LOCKKEY_UNLOCKED                  (_CMU_LOCK_LOCKKEY_UNLOCKED << 0)
#define _CMU_LOCK_LOCKKEY_UNLOCK                   0x0000580EUL
#define CMU_LOCK_LOCKKEY_UNLOCK                    (_CMU_LOCK_LOCKKEY_UNLOCK << 0)
#define CMU_STATUS_CALBSY                          (0x1UL << 14)

#define CMU_CMD_CALSTART                           (0x1UL << 3)

/** Bit banding area */
#define BITBAND_PER_BASE     					   ((uint32_t) 0x42000000UL)  /**< Peripheral Address Space bit-band area */
#define BITBAND_RAM_BASE     					   ((uint32_t) 0x22000000UL)  /**< SRAM Address Space bit-band area */
#define PER_MEM_BASE         					   ((uint32_t) 0x40000000UL)  /**< PER base address  */
#define RAM_MEM_BASE         					   ((uint32_t) 0x20000000UL)  /**< RAM base address  */

#define _CMU_HFRCOCTRL_BAND_1MHZ                   0x00000000UL                           /**< Mode 1MHZ for CMU_HFRCOCTRL */
#define _CMU_HFRCOCTRL_BAND_7MHZ                   0x00000001UL                           /**< Mode 7MHZ for CMU_HFRCOCTRL */
#define _CMU_HFRCOCTRL_BAND_11MHZ                  0x00000002UL                           /**< Mode 11MHZ for CMU_HFRCOCTRL */
#define _CMU_HFRCOCTRL_BAND_DEFAULT                0x00000003UL                           /**< Mode DEFAULT for CMU_HFRCOCTRL */
#define _CMU_HFRCOCTRL_BAND_14MHZ                  0x00000003UL                           /**< Mode 14MHZ for CMU_HFRCOCTRL */
#define _CMU_HFRCOCTRL_BAND_21MHZ                  0x00000004UL                           /**< Mode 21MHZ for CMU_HFRCOCTRL */
#define _CMU_HFRCOCTRL_BAND_28MHZ                  0x00000005UL                           /**< Mode 28MHZ for CMU_HFRCOCTRL */


#define _CMU_HFPERCLKDIV_HFPERCLKEN_SHIFT          8                                            /**< Shift value for CMU_HFPERCLKEN */
#define _CMU_HFCORECLKEN0_LE_SHIFT                 2                                    /**< Shift value for CMU_LE */
#define _CMU_LFACLKEN0_RTC_SHIFT                   0


#define LETIMER0_BASE     						   (0x40082000UL) 			  /**< LETIMER0 base address  *//**< Shift value for CMU_RTC */
#define LETIMER_STATUS_RUNNING               	   (0x1UL << 0)
#define LETIMER_IEN_UF                       	   (0x1UL << 2)
#define LETIMER_IEN_COMP1                    	   (0x1UL << 1)
#define LETIMER_IF_COMP0                     	   (0x1UL << 0)


#define _CMU_LFACLKEN0_LETIMER0_MASK
#define _CMU_LFACLKEN0_LETIMER0_SHIFT              1


#define CMU_NODIV_REG              0
#define CMU_DIV_REG_POS            4
#define CMU_HFCLKSEL_REG           1
#define CMU_SEL_REG_POS            0
#define CMU_NO_EN_REG              0
#define CMU_EN_REG_POS             8
#define CMU_EN_BIT_POS             12
/* Clock branch bitfield position, for internal use */
#define CMU_HF_CLK_BRANCH          0
#define CMU_HFPER_CLK_BRANCH       1
#define CMU_HFCORE_CLK_BRANCH      2
#define CMU_LFA_CLK_BRANCH         3
#define CMU_RTC_CLK_BRANCH         4
#define CMU_LETIMER_CLK_BRANCH     5
#define CMU_LCDPRE_CLK_BRANCH      6
#define CMU_LCD_CLK_BRANCH         7
#define CMU_LESENSE_CLK_BRANCH     8
#define CMU_LFB_CLK_BRANCH         9
#define CMU_LEUART0_CLK_BRANCH     10
#define CMU_LEUART1_CLK_BRANCH     11
#define CMU_DBG_CLK_BRANCH         12
#define CMU_AUX_CLK_BRANCH         13
#define CMU_USBC_CLK_BRANCH        14

#define CMU_CLK_BRANCH_POS         17
#define CMU_CLK_BRANCH_MASK        0x1f

#define CMU_DBGCLKSEL_REG          4

/* Select register ids, for internal use */
#define CMU_NOSEL_REG              0
#define CMU_HFCLKSEL_REG           1
#define CMU_LFACLKSEL_REG          2
#define CMU_LFBCLKSEL_REG          3
#define CMU_DBGCLKSEL_REG          4
#if defined (_EFM32_GIANT_FAMILY) || defined(_EFM32_WONDER_FAMILY)
#define CMU_USBCCLKSEL_REG         5
#endif

#define CMU_SEL_REG_POS            0
#define CMU_SEL_REG_MASK           0xf

/* Divisor register ids, for internal use */
#define CMU_NODIV_REG              0
#define CMU_HFPERCLKDIV_REG        1
#define CMU_HFCORECLKDIV_REG       2
#define CMU_LFAPRESC0_REG          3
#define CMU_LFBPRESC0_REG          4
#if defined (_EFM32_GIANT_FAMILY) || defined(_EFM32_WONDER_FAMILY)
#define CMU_HFCLKDIV_REG           5
#endif
#define CMU_DIV_REG_POS            4
#define CMU_DIV_REG_MASK           0xf


/* Enable register ids, for internal use */
#define CMU_NO_EN_REG              0
#define CMU_HFPERCLKDIV_EN_REG     1
#define CMU_HFPERCLKEN0_EN_REG     2
#define CMU_HFCORECLKEN0_EN_REG    3
#define CMU_LFACLKEN0_EN_REG       4
#define CMU_LFBCLKEN0_EN_REG       5
#define CMU_PCNT_EN_REG            6

#define CMU_EN_REG_POS             8
#define CMU_EN_REG_MASK            0xf

/*LEUART LOC3 MOCKUP*/
#define _LEUART_ROUTE_LOCATION_LOC3              0x00000003UL                          /**< Mode LOC3 for LEUART_ROUTE */
#define LEUART_ROUTE_LOCATION_LOC3               (_LEUART_ROUTE_LOCATION_LOC3 << 8)    /**< Shifted mode LOC3 for LEUART_ROUTE */

/* Hardware Dependent Usage Register Maps Faked with Fixed Memory */
DEVINFO_TypeDef *DEVINFO  =(DEVINFO_TypeDef *)malloc(sizeof(DEVINFO_TypeDef));      /**< DEVINFO base pointer */
SCB_Type        * SCB     =(SCB_Type       *)malloc(sizeof(SCB_Type));   /*!< SCB configuration struct  */

/** \ingroup    CMSIS_core_register
    \defgroup   CMSIS_NVIC  Nested Vectored Interrupt Controller (NVIC)
    \brief      Type definitions for the NVIC Registers
  @{
 */

/** \brief  Structure type to access the Nested Vectored Interrupt Controller (NVIC).
 */
typedef struct
{
  __IO uint32_t ISER[8];                 /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register           */
       uint32_t RESERVED0[24];
  __IO uint32_t ICER[8];                 /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register         */
       uint32_t RSERVED1[24];
  __IO uint32_t ISPR[8];                 /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register          */
       uint32_t RESERVED2[24];
  __IO uint32_t ICPR[8];                 /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register        */
       uint32_t RESERVED3[24];
  __IO uint32_t IABR[8];                 /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register           */
       uint32_t RESERVED4[56];
  __IO uint8_t  IP[240];                 /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
       uint32_t RESERVED5[644];
  __O  uint32_t STIR;                    /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register     */
}  NVIC_Type;

NVIC_Type *  NVIC = (NVIC_Type *)malloc(sizeof(NVIC_Type));

uint32_t * FakeSystemCoreClockPointer = (uint32_t *) malloc(sizeof(uint32_t));
#define SystemCoreClock (*FakeSystemCoreClockPointer)
/*FAKE_VAR(uint32_t, SystemCoreClock);*/


/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC(SystemCoreClockUpdate);
FAKE_VALUE_FUNC(uint32_t,SysTick_Config,uint32_t);
FAKE_VOID_FUNC(  NVIC_EnableIRQ, IRQn_Type);
FAKE_VOID_FUNC( NVIC_ClearPendingIRQ, IRQn_Type );
FAKE_VOID_FUNC(NVIC_SetPriorityGrouping,uint32_t);
FAKE_VOID_FUNC(NVIC_SetPriority,IRQn_Type, uint32_t);
FAKE_VOID_FUNC(NVIC_SystemReset);

#endif /* EM_DEVICE_TH_HH_ */

/** ************************************************************************* */
