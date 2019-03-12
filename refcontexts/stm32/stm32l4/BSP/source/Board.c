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
 * @defgroup IF_FUNCTIONS Interface Functions
 * @ingroup BSP_BOARD
 * @defgroup PV_FUNCTIONS Private Functions
 * @ingroup BSP_BOARD
 * @{
 * @file
 * @brief  Implementation of board BSP functions
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_BOARD

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_Board.h"

/* All headers which are needed by this file in the following order:
 * Own public header
 * Own protected header
 * Own private header
 * System header (if necessary)
 * Other headers
 */

#include "BoardSettings.h"
#include "BSP_BoardType.h"

#include "SystemClock.h"
#include "LDO_tlv70233.h"
#include "BSP_ModuleState.h"

#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_hal_flash.h"

#include "Charger_bq2407x.h"

#if BCDS_FEATURE_BSP_BT_EM9301
#include "BCDS_MCU_UART_Handle.h"
#endif /* BCDS_FEATURE_BSP_BT_EM9301 */

#define MCU_REV_ID_REV4 UINT16_C(0x1007)
#define MCU_DEV_ID UINT16_C(0x415)

uint8_t BSP_Board_BoardRevision = UINT8_C(0xFF);
uint16_t BSP_Board_McuRevisionID;
uint16_t BSP_Board_McuDeviceID;

static enum BSP_WakeupEvent_E BSP_WakeupEvent = BSP_WAKE_EVENT_NONE;

/* Function declarations */
/**
 * @ingroup IF_FUNCTIONS
 * @{
 */

/**@}*//* @ingroup IF_FUNCTIONS */

/**
 * @ingroup PV_FUNCTIONS
 * @{
 */

/**
 * @brief Initialize the Board GPIOs.
 * @details This function initializes the GPIOs necessary for the board operation such as
 * SYSOFF. and it puts the unused GPIO pins into analog mode to reduce power
 * consumption.
 * It configures board SPECIAL GPIO pins like:
 *  - Analog
 *  - Input
 *  - Output
 *  -EVENT_OUT
 *  -EXTI
 * Free pins are configured as analog.
 */
static void Board_GPIO_Init(void);

/**
 * @brief Save board revision ID for later use.
 * @details @see BSP_Board_GetBoardRevision().
 */
static void Board_PCB_RevisionId_Init(void);

#if BCDS_FEATURE_BSP_BT_EM9301
/**
 * @brief Enter stop1 mode
 * @details
 */
static Retcode_T EnterStop1Mode(void);
#endif /* BCDS_FEATURE_BSP_BT_EM9301 */

/**
 * @brief Enter stop2 mode
 * @details
 */
static Retcode_T EnterStop2Mode(void);

/**@}*//* @ingroup PV_FUNCTIONS */

/**
 * @brief Configure all pin to the analog state to achieve the lowest possible power consumption
 * @details
 * *
 */
void Board_ConfigureAllPinToAnalogState(void);

/**
 * @brief Configure The pin to allow the wakeup capability from shutdown
 * @details
 * *
 */
void Board_ConfigurePinForShutdownCapability(void);

/**
 * @ingroup IF_FUNCTIONS
 * @{
 *
 * BSE: The API initializes internal BSP data structures, configures interrupt
 * and exception priorities, the system clock and puts board peripherals into
 * low power idle state.
 * As prerequisite, the user must call Retcode_initialize(), Assert_initialize()
 * and BSP_Board_OSTickInitialize(). On unrecoverable hardware configuration
 * errors the API raises errors.
 *
 * @param[in] param1
 *      Board specific parameter. In case of BSE the parameter is unused.
 *
 * @param[in] param2
 *      Board specific parameter. In case of BSE the parameter is unused.
 */
Retcode_T BSP_Board_Initialize(uint32_t param1, void* param2)
{
    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    Retcode_T retcode = RETCODE_OK;

    /* Enable the irq vector */
    __enable_irq();
    /* Reset of all peripherals, Initializes the Flash interface and the Systick */
    if (HAL_OK != HAL_Init())
    {
        return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    /* Enable the System configuration controller clock */
    __SYSCFG_CLK_ENABLE();
    /* Configure the interrupt priority grouping with no sub-priority usage. */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* Workaround for STM32L4x6 REV3 & 4 chip erratum 2.1.8 from DocID026121 Rev 3 */
    {
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };

        OSC_GPIO_CLOCK_ENABLE();

        GPIO_InitStruct.Pin = OSC_IN_PIN | OSC_OUT_PIN;
        GPIO_InitStruct.Mode = OSC_MODE;
        HAL_GPIO_Init(OSC_PORT, &GPIO_InitStruct);
    }

    /* Configure the system clock */
    retcode = SystemClock_Config();
    /* Configure the board GPIO pins */
    if (RETCODE_OK == retcode)
    {
        Board_GPIO_Init();
    }

    /* Read hardware variant ID */
    Board_PCB_RevisionId_Init();

    /* Initialize the watchdog */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_IWDG_Init(void);
        retcode = Board_IWDG_Init();
    }
    /* Initialize the I2C for the sensors */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_I2C1_Init(void);
        retcode = Board_I2C1_Init();
    }

    /* Initialize the I2C for LM2755 */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_I2C2_Init(void);
        retcode = Board_I2C2_Init();
    }

    /* Initialize the ADC for the board power control */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_ADC1_Init(void);
        retcode = Board_ADC1_Init();
    }
    /* Initialize the TIM3 timer for the buttons */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_TIM3_Init(void);
        retcode = Board_TIM3_Init();
    }
    /* Initialize the timer for the Charger */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_ChargerTIM_Init(void);
        retcode = Board_ChargerTIM_Init();
    }
    /* Initialize the UART interface for the GSM */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_USART2_UART_Init(void);
        retcode = Board_USART2_UART_Init();
    }
    /* Initialize the low power UART for the GNSS */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_LPUART1_UART_Init(void);
        retcode = Board_LPUART1_UART_Init();
    }
    /* Initialize the SPI for the external memory */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_SPI1_Init(void);
        retcode = Board_SPI1_Init();
    }
    /* Initialize the UART for the Bluetooth */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_USART1_UART_Init(void);
        retcode = Board_USART1_UART_Init();
    }

    /* Initialize UART for the test interface */
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_USART3_UART_Init(void);
        retcode = Board_USART3_UART_Init();
    }

#if BCDS_FEATURE_BSP_PWM_BUZZER
    if (RETCODE_OK == retcode)
    {
        extern Retcode_T Board_TIM1_Init(void);
        retcode = Board_TIM1_Init();
    }
#endif /* BCDS_FEATURE_BSP_PWM_BUZZER*/

    if (RETCODE_OK == retcode)
    {
        retcode = BSP_LDO_TLV70233_Connect();
    }

    return retcode;
}
/**@}*//* @ingroup IF_FUNCTIONS */

/**
 * @ingroup IF_FUNCTIONS
 * @{
 */
uint32_t BSP_Board_GetBoardRevision(void)
{
    assert(BSP_Board_BoardRevision < HARDWARE_VARIANT_ID_MAX);

    return BSP_Board_BoardRevision;
}

void Board_ConfigureAllPinToAnalogState()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_All);
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_All);
	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_All);
	HAL_GPIO_DeInit(GPIOE, GPIO_PIN_All);
	HAL_GPIO_DeInit(GPIOF, GPIO_PIN_All);
	HAL_GPIO_DeInit(GPIOG, GPIO_PIN_All);
	HAL_GPIO_DeInit(GPIOH, GPIO_PIN_All);

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOE_CLK_DISABLE();
    __HAL_RCC_GPIOF_CLK_DISABLE();
    __HAL_RCC_GPIOG_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
}

void Board_ConfigurePinForShutdownCapability(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* power good : mandatory because there is no pull Up*/
	memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
	GPIO_InitStruct.Pin = CHARGING_PWR_GOOD_PIN;
	GPIO_InitStruct.Mode = CHARGING_PWR_GOOD_MODE;
	GPIO_InitStruct.Pull = CHARGING_PWR_GOOD_PULL;
	HAL_GPIO_Init(CHARGING_PWR_GOOD_PORT, &GPIO_InitStruct);

	__HAL_RCC_GPIOC_CLK_DISABLE();

	/* Enable PWR clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	/* defined pull up/down for shutdown and standby mode */
	if(HAL_OK != HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_C, PWR_GPIO_BIT_5))
	{
	    Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE));
	}
	HAL_PWREx_EnablePullUpPullDownConfig();

	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN5_LOW);
}
/**@}*//* @ingroup IF_FUNCTIONS */
/**
 * @ingroup IF_FUNCTIONS
 * @{
 */
Retcode_T BSP_Board_SwitchPowerMode(uint32_t PowerMode)
{
    Retcode_T retcode = RETCODE_OK;

    /** @todo 2016-11-22, BCDS/ENG1: After clarification with ENG2 remove the interface as
     *  BSP shall implement auto power management and the application (driver) layers
     *  shall only be allowed to set clock or power domain constraints and dependencies
     *  applicable for the given MCU family.
     *
     *  Requesting direct entry to low power modes is a questionable practice knowing
     *  that the BSP is unaware of how the application uses the provided MCU/BSP
     *  resources.
     */

    switch(PowerMode)
    {
    case BSP_MODE_STANDBY:
    	HAL_PWREx_DisableVddIO2();
    	Board_ConfigureAllPinToAnalogState();
    	/* Disable all used wakeup sources: WKUP pin */
    	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
    	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
    	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN3);
    	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN4);
    	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN5);

    	//__disable_irq();
    	for(uint32_t i = 0; i <= 81 ; i++ )
    	{
            HAL_NVIC_DisableIRQ((IRQn_Type) i);
            HAL_NVIC_ClearPendingIRQ((IRQn_Type) i);
    	}

    	/* Clear wake up Flag */
    	/*lint -e{506} The function is provided by STM32Cube library*/
    	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);
    	/*lint -e{506} The function is provided by STM32Cube library*/
    	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
    	/*lint -e{506} The function is provided by STM32Cube library*/
    	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF3);
    	/*lint -e{506} The function is provided by STM32Cube library*/
    	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF4);
    	/*lint -e{506} The function is provided by STM32Cube library*/
    	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF5);
    	RCC->CIER = 0x00000000;
    	Board_ConfigurePinForShutdownCapability();

    	MODIFY_REG(PWR->CR1, PWR_CR1_LPMS, PWR_CR1_LPMS_STANDBY);
    	/* Set SLEEPDEEP bit of Cortex System Control Register */
    	SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

    	/* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
    	__force_stores();
#endif
    	/* Request Wait For Interrupt */
    	__WFI();
    	break;
    case BSP_MODE_TRANSPORTATION:
        HAL_PWREx_DisableVddIO2();
        Board_ConfigureAllPinToAnalogState();
        /* Disable all used wakeup sources: WKUP pin */
        HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
        HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
        HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN3);
        HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN4);
        HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN5);

        //__disable_irq();
        for(uint32_t i = 0; i <= 81 ; i++ )
        {
        	HAL_NVIC_DisableIRQ((IRQn_Type) i);
        	HAL_NVIC_ClearPendingIRQ((IRQn_Type) i);
        }

        /* Clear wake up Flag */
        /*lint -e{506} The function is provided by STM32Cube library*/
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
        RCC->CIER = 0x00000000;
        Board_ConfigurePinForShutdownCapability();

        HAL_PWREx_EnterSHUTDOWNMode();
    	break;
    case BSP_MODE_SHORT_TERM_STORAGE:
#if BCDS_FEATURE_BSP_BT_EM9301
        retcode = EnterStop1Mode();
#endif /* BCDS_FEATURE_BSP_BT_EM9301 */
        break;
    case BSP_MODE_LONG_TERM_STORAGE:
        retcode = EnterStop2Mode();
        break;
    default:
    	retcode = RETCODE(RETCODE_SEVERITY_NONE, RETCODE_NOT_SUPPORTED);
    	break;
    }

    return retcode;
}


#if BCDS_FEATURE_BSP_BT_EM9301
static Retcode_T EnterStop1Mode(void)
{
    struct MCU_UART_S * BSP_UART1Handle;
    Retcode_T retcode = RETCODE_OK;


    /* Ensure that MSI is wake-up system clock */
    HAL_RCCEx_WakeUpStopCLKConfig(RCC_STOP_WAKEUPCLOCK_HSI);

    /* Enable PWR clock enable */
    __HAL_RCC_PWR_CLK_ENABLE();

    if (HAL_OK != HAL_PWREx_DisableLowPowerRunMode())
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

#if (PREFETCH_ENABLE != 0)
    /* Disable Prefetch Buffer */
    __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

    extern HWHandle_T BSP_BT_EM9301_GetUARTHandle(void);

    BSP_UART1Handle = BSP_BT_EM9301_GetUARTHandle();
    assert(NULL != BSP_UART1Handle);
    if(NULL != BSP_UART1Handle)
    { /* Dummy if branch to suppress lint warning */
        /* make sure that no UART transfer is on-going */
        while(__HAL_UART_GET_FLAG(&(BSP_UART1Handle->huart), USART_ISR_BUSY) == (int)SET);
        /* make sure that UART is ready to receive
        * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
        while(__HAL_UART_GET_FLAG(&(BSP_UART1Handle->huart), USART_ISR_REACK) == (int)RESET);

        /* Suspend HAL tick interrupt */
        HAL_SuspendTick();

        /* Suspend RTOS Systick */
        CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

        /* Disable system peripheral clocks */
        GNSS_UART_CLOCK_DISABLE();
        GSM_UART_CLOCK_DISABLE();
        SENSOR_NODE_I2C_CLOCK_DISABLE();
        TEST_UART_CLOCK_DISABLE();
        LED_LM2755_I2C_CLOCK_DISABLE();
        __HAL_RCC_RNG_CLK_DISABLE();

        /* Clear the interrupt source WUF */
        SET_BIT(BT_SERIAL_PORT->ICR, USART_ICR_WUCF);

        /* Enable the UART Wake UP from STOP1 mode Interrupt */
        /*lint -e{506} The function is provided by STM32Cube library */
        __HAL_UART_ENABLE_IT(&(BSP_UART1Handle->huart), UART_IT_WUF);

        /* Enable UART clock source to run always in stop1 mode */
        SET_BIT(BT_SERIAL_PORT->CR3, (0x1U << (23U))); /* UCESM */

        /* Disable BT_UART DMA */
        extern DMA_HandleTypeDef BSP_UART1DMARxHandle;
        __HAL_DMA_DISABLE(&BSP_UART1DMARxHandle);

        /* enable MCU wake-up by UART */
        if(HAL_OK != HAL_UARTEx_EnableStopMode(&(BSP_UART1Handle->huart)))
        {
            retcode = RETCODE(RETCODE_SEVERITY_NONE, RETCODE_OUT_OF_RESOURCES);
        }


        if(RETCODE_OK == retcode)
        {
            /* Enter in Low power Stop 1 */
            HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);

            /* Re-enable BT_UART DMA */
            __HAL_DMA_ENABLE(&BSP_UART1DMARxHandle);

            /* Re-configure system clocks */
            retcode = SystemClock_Config();
        }

        if(RETCODE_OK == retcode)
        {
            /* Wake Up based on RXNE flag successful */
            if(HAL_OK != HAL_UARTEx_DisableStopMode(&(BSP_UART1Handle->huart)))
            {
                retcode = RETCODE(RETCODE_SEVERITY_NONE, RETCODE_OUT_OF_RESOURCES);
            }

            /* Enable system peripheral clocks */
            GNSS_UART_CLOCK_ENABLE();
            GSM_UART_CLOCK_ENABLE();
            SENSOR_NODE_I2C_CLOCK_ENABLE();
            TEST_UART_CLOCK_ENABLE();
            LED_LM2755_I2C_CLOCK_ENABLE();
            __HAL_RCC_RNG_CLK_ENABLE();

            /* Re-enable RTOS Systick */
            SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
            HAL_ResumeTick();
        }

    }

    return retcode;
}
#endif /* BCDS_FEATURE_BSP_BT_EM9301 */


static Retcode_T EnterStop2Mode(void)
{
    /* Ensure that MSI is wake-up system clock */
    HAL_RCCEx_WakeUpStopCLKConfig(RCC_STOP_WAKEUPCLOCK_MSI);

    /* Enable PWR clock enable */
    __HAL_RCC_PWR_CLK_ENABLE();

    if (HAL_OK != HAL_PWREx_DisableLowPowerRunMode())
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

#if (PREFETCH_ENABLE != 0)
    /* Disable Prefetch Buffer */
    __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

    /* Suspend HAL tick interrupt */
    HAL_SuspendTick();

    /* Suspend RTOS Systick */
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

    /* Disable system peripheral clocks */
    BT_UART_CLOCK_DISABLE();
    GNSS_UART_CLOCK_DISABLE();
    GSM_UART_CLOCK_DISABLE();
    SENSOR_NODE_I2C_CLOCK_DISABLE();
    TEST_UART_CLOCK_DISABLE();
    LED_LM2755_I2C_CLOCK_DISABLE();
    __HAL_RCC_RNG_CLK_DISABLE();

    /* tri-state all pins except E12 and F14 */
    BOARD_GPIO_CLOCK_ENABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_All);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_All);
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_All);
    HAL_GPIO_DeInit(GPIOE, 0xEFFF);
    HAL_GPIO_DeInit(GPIOF, 0xBFFF);
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_All);
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_All);

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOE_CLK_DISABLE();
    /* Not disabling GPIOF CLK in order to keep F14 IRQ clock on */
    __HAL_RCC_GPIOG_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();

    /* Enter in Low power Stop 2 */
    HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
    /* Reset system on wake-up event */
    NVIC_SystemReset();

    return RETCODE_OK;
}

uint32_t BSP_Board_GetWakeupSource(void)
{
    /** @todo 2016-11-29, BCDS/ENG1: Implement wake-up source tracking */
    return (uint32_t) BSP_WakeupEvent;
}

/**@}*//* @ingroup IF_FUNCTIONS */
/**
 * @ingroup IF_FUNCTIONS
 * @{
 */

uint32_t BSP_Board_DetectPowerSupply(void)
{
    BSP_PowerSupply_T PowerSupply = POWER_SUPPLY_UNKNOWN;

    /** @todo 2017-02-17, BCDS/ENG1: Move control of CHARGING_PWR_GOOD_PORT into Charger module
     *  and make it sure that Charger is in Connected (Enabled) state otherwise this readout
     *  would always report LOW level.
     */
    enum BSP_ModuleState_E moduleState_Charger_bq2407x = BSP_MODULE_STATE_UNINITIALIZED;

    moduleState_Charger_bq2407x = Charger_bq2407x_getState();
    if(BSP_MODULE_STATE_UNINITIALIZED == moduleState_Charger_bq2407x
            || BSP_MODULE_STATE_DISCONNECTED == moduleState_Charger_bq2407x)
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        __HAL_RCC_GPIOC_CLK_ENABLE();
        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = CHARGING_PWR_GOOD_PIN;
        GPIO_InitStruct.Mode = CHARGING_PWR_GOOD_MODE;
        GPIO_InitStruct.Pull = CHARGING_PWR_GOOD_PULL;
        HAL_GPIO_Init(CHARGING_PWR_GOOD_PORT, &GPIO_InitStruct);
    }

    if (BSP_IO_PIN_LOW == HAL_GPIO_ReadPin(CHARGING_PWR_GOOD_PORT, CHARGING_PWR_GOOD_PIN))
    {
        PowerSupply = POWER_SUPPLY_EXTERNAL;
    }
    else
    {
        PowerSupply = POWER_SUPPLY_BATTERY;
    }

    if(BSP_MODULE_STATE_UNINITIALIZED == moduleState_Charger_bq2407x
            || BSP_MODULE_STATE_DISCONNECTED == moduleState_Charger_bq2407x)
    {
        HAL_GPIO_DeInit(CHARGING_PWR_GOOD_PORT, CHARGING_PWR_GOOD_PIN);
    }

    return (uint32_t) PowerSupply;
}
/**@}*//* @ingroup IF_FUNCTIONS */

/**
 * @ingroup IF_FUNCTIONS
 * @{
 */

Retcode_T BSP_Board_ControlPowerRail(uint32_t PowerRail)
{
    BCDS_UNUSED(PowerRail);

    /** @todo 2016-11-29, BCDS/ENG1: Implement power rail control functionality
     *
     *        Existing implementation is invalid as it does not allow to ENABLE
     *        or DISABLE the various power rails, instead it disables the only
     *        power rail available and it does not allow the power rail to be
     *        set back on.
     *        On the other hand the interface is dangerous as it allows the
     *        application to disable power rails while the IO interfaces are
     *        still enabled which could permanently damage the BT and LED driver
     *        ICs to name a selected few.
     */
    return RETCODE(RETCODE_SEVERITY_NONE, RETCODE_NOT_SUPPORTED);
}

/**@}*//* @ingroup IF_FUNCTIONS */

/* Private functions implementation */

void Board_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    BOARD_GPIO_CLOCK_ENABLE();

    /* Enable the power for GPIOG */
    HAL_PWREx_EnableVddIO2();
    /* Configure Board GPIO PINS */
    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));

    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
    GPIO_InitStruct.Pin = VEXT_DCDC_PWR_GOOD_PIN;
    GPIO_InitStruct.Mode = VEXT_DCDC_PWR_GOOD_MODE;
    GPIO_InitStruct.Pull = VEXT_DCDC_PWR_GOOD_PULL;
    HAL_GPIO_Init(VEXT_DCDC_PWR_GOOD_PORT, &GPIO_InitStruct);

    /* Configure unused pins to analog mode to lower the power consumption */
    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;

    /* GPIOA unused pins*/
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* GPIOB unused pins *//* Reset configuration of PB3 is JTDO-TRACESWO */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* GPIOC unused pins */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* GPIOD unused pins */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* GPIOE unused pins */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* GPIOF unused pins */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* GPIOG unused pins */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_12 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void Board_PCB_RevisionId_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Read MCU Revision and Device IDs */
    BSP_Board_McuRevisionID = LL_DBGMCU_GetRevisionID();
    BSP_Board_McuDeviceID = LL_DBGMCU_GetDeviceID();

    assert((MCU_DEV_ID == BSP_Board_McuDeviceID) && (MCU_REV_ID_REV4 == BSP_Board_McuRevisionID));

    /* Enable pins */
    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
    GPIO_InitStruct.Pin = R5010_PIN;
    GPIO_InitStruct.Mode = R5010_MODE;
    GPIO_InitStruct.Pull = R5010_PULL;
    GPIO_InitStruct.Speed = R5010_SPEED;
    HAL_GPIO_Init(R5010_PORT, &GPIO_InitStruct);

    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
    GPIO_InitStruct.Pin = R5011_PIN;
    GPIO_InitStruct.Mode = R5011_MODE;
    GPIO_InitStruct.Pull = R5011_PULL;
    GPIO_InitStruct.Speed = R5011_SPEED;
    HAL_GPIO_Init(R5011_PORT, &GPIO_InitStruct);

    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
    GPIO_InitStruct.Pin = R5012_PIN;
    GPIO_InitStruct.Mode = R5012_MODE;
    GPIO_InitStruct.Pull = R5012_PULL;
    GPIO_InitStruct.Speed = R5012_SPEED;
    HAL_GPIO_Init(R5012_PORT, &GPIO_InitStruct);

    /* Read pins */
    BSP_Board_BoardRevision = ((GPIO_PIN_RESET == HAL_GPIO_ReadPin(R5012_PORT, R5012_PIN) ? 0 : 1) +
            ((GPIO_PIN_RESET == HAL_GPIO_ReadPin(R5011_PORT, R5011_PIN) ? 0 : 1) << 1) +
            ((GPIO_PIN_RESET == HAL_GPIO_ReadPin(R5010_PORT, R5010_PIN) ? 0 : 1) << 2));

    /* Disable pins */
    HAL_GPIO_DeInit(R5010_PORT, R5010_PIN);
    HAL_GPIO_DeInit(R5011_PORT, R5011_PIN);
    HAL_GPIO_DeInit(R5012_PORT, R5012_PIN);
}

void BSP_Board_OSTickInitialize(BSP_Systick_Callback_T PreCallback, BSP_Systick_Callback_T PostCallback)
{
    BSP_SysTickOsPreProcessing = PreCallback;
    BSP_SysTickOsPostProcessing = PostCallback;
}

void BSP_Board_Delay(uint32_t delay)
{
    /** @todo 2016-11-29, BCDS/ENG1: Analyze whether low power delay can be implemented */
    HAL_Delay(delay);
}

/**@}*//* @ingroup BSP_BOARD */
