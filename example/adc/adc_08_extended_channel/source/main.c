/**
 *******************************************************************************
 * @file  adc/adc_08_extended_channel/source/main.c
 * @brief Main program ADC extended channel for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wuze            First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 *******************************************************************************
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup ADC_Extended_Channel
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*
 * ADC unit instance for this example.
 * Marco 'APP_ADC_UNIT' can be defined as M4_ADCx(x=1, 2, 3).
 * Definition of 'APP_PERIP_CLK_ADC' depends on 'APP_ADC_UNIT'.
 */
#define APP_ADC_UNIT                        (M4_ADC1)
#define APP_ADC_PERIP_CLK                   (PWC_FCG3_ADC1)

/* 
 * ADC channels definition for this example.
 * 'ADC_EXT_CH'(extended channel, channel 15) must be enabled.
 * NOTE!!! When channel 15 is used as the sampling internal analog input source, \
 *         its corresponding external input pin does not need to be set to analog input mode, \
 *         unless you use it in other ADC units. See definitions of 'APP_ADC_SA_CH' and 'APP_ADC_PIN_ANALOG_CH'.
 *         The pins which is corresponding the channels that included in 'APP_ADC_PIN_ANALOG_CH' must be
 *         set to analog input mode.
 */
#define APP_ADC_SA_CH                       (ADC_CH0 | ADC_EXT_CH)
#define APP_ADC_SA_CH_COUNT                 (2U)
#define APP_ADC_PIN_ANALOG_CH               (ADC_CH0)

/* ADC channel sampling time.               ADC_CH0   ADC_EXT_CH */
#define APP_ADC_SA_SAMPLE_TIME              { 30,        25 }

/* ADC reference voltage. The voltage of pin VREFH. */
#define APP_ADC_VREF                        (3.2867f)

/* ADC accuracy(according to the resolution of ADC). */
#define APP_ADC_ACCURACY                    (1UL << 12U)

/* Calculate the voltage. */
#define APP_CAL_VOL(adcVal)                 ((float)(adcVal) * APP_ADC_VREF) / ((float)APP_ADC_ACCURACY)

/* Timeout value definitions. */
#define APP_TIMEOUT_MS                      (10U)

/* Debug printing definition. */
#if (DDL_PRINT_ENABLE == DDL_ON)
#define DBG         printf
#else
#define DBG(...)
#endif

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);

static void AdcConfig(void);
static void AdcClockConfig(void);
static void AdcInitConfig(void);
static void AdcChannelConfig(void);
static void AdcExtChSrcConfig(void);

static void AdcSetChannelPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint32_t u32Channel);
static void AdcSetPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint8_t u8PinNum);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t m_au16AdcSaVal[APP_ADC_SA_CH_COUNT];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of adc_08_extended_channel project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* The default system clock source is MRC(8MHz). */

    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_PrintfInit();
#endif /* #if (DDL_PRINT_ENABLE == DDL_ON) */
    /* Configures ADC. */
    AdcConfig();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /***************** Configuration end, application start **************/

    while (1U)
    {
        /* Get the internal reference voltage. */
    	PWC_Unlock(PWC_UNLOCK_CODE_1);
        PWC_VBAT_MonitorCmd(Disable);
        PWC_VBAT_MeasureVolCmd(Disable);
        PWC_AdcInternVolSel(PWC_AD_INTERN_REF);
        PWC_Lock(PWC_UNLOCK_CODE_1);
        /* Delay 50us is needed. */
        DDL_DelayUS(100U);
        ADC_PollingSA(APP_ADC_UNIT, m_au16AdcSaVal, APP_ADC_SA_CH_COUNT, APP_TIMEOUT_MS);
        DBG("Internal reference voltage: adc value is %d, voltage is %.3f.\n", m_au16AdcSaVal[1U], APP_CAL_VOL(m_au16AdcSaVal[1U]));

        /* Get the half voltage of VBAT. */
        PWC_Unlock(PWC_UNLOCK_CODE_1);
        PWC_VBAT_MonitorCmd(Enable);
        PWC_VBAT_MeasureVolCmd(Enable);
        PWC_AdcInternVolSel(PWC_AD_VBAT_DIV2);
        PWC_Lock(PWC_UNLOCK_CODE_1);
        /* Delay 50us is needed. */
        DDL_DelayUS(100U);
        ADC_PollingSA(APP_ADC_UNIT, m_au16AdcSaVal, APP_ADC_SA_CH_COUNT, APP_TIMEOUT_MS);
        DBG("VBAT/2: adc value is %d, voltage is %.3f.\n", m_au16AdcSaVal[1U], APP_CAL_VOL(m_au16AdcSaVal[1U]));
    }
}

/**
 * @brief  MCU Peripheral registers write unprotected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static void Peripheral_WE(void)
{
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    GPIO_Unlock();
    /* Unlock PWC register: FCG0 */
    // PWC_FCG0_Unlock();
    /* Unlock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Unlock SRAM register: WTCR */
    // SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    // SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    // EFM_Unlock();
    /* Unlock EFM register: FWMC */
    // EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
    // EFM_OTP_WP_Unlock();
}

/**
 * @brief  MCU Peripheral registers write protected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static void Peripheral_WP(void)
{
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    GPIO_Lock();
    /* Lock PWC register: FCG0 */
    // PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Lock SRAM register: WTCR */
    // SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    // SRAM_CKCR_Lock();
    /* Lock all EFM registers */
    // EFM_Lock();
    /* Lock EFM OTP write protect registers */
    // EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    // EFM_FWMC_Lock();
}

/**
 * @brief  ADC configuration, including clock configuration, initial configuration
 *         and channel configuration.
 * @param  None
 * @retval None
 */
static void AdcConfig(void)
{
    AdcClockConfig();
    AdcInitConfig();
    AdcExtChSrcConfig();
    AdcChannelConfig();
}

/**
 * @brief  Configures ADC clock.
 * @param  None
 * @retval None
 */
static void AdcClockConfig(void)
{
    /*
     * 1. Configures the clock divider of PCLK2 and PCLK4 here or in the function of configuring the system clock.
     *    In this example, the default system clock is MRC(8MHz).
     *    PCLK4 is the digital interface clock, and PCLK2 is the analog circuit clock.
     *    PCLK4 : PCLK2 = 1:1, 2:1, 4:1, 8:1, 1:2, 1:4.
     *    PCLK2 is in range [1MHz, 60MHz].
     */
    CLK_ClkDiv((CLK_CATE_PCLK2 | CLK_CATE_PCLK4), (CLK_PCLK2_DIV4 | CLK_PCLK4_DIV2));

    /* 2. Specify the clock source of ADC. Only 'CLK_PERI_CLK_PCLK' can be used in this example. */
    CLK_PERI_ClkConfig(CLK_PERI_CLK_PCLK);
}

/**
 * @brief  Initializes ADC.
 * @param  None
 * @retval None
 */
static void AdcInitConfig(void)
{
    stc_adc_init_t stcInit;

    /* Set a default value. */
    ADC_StructInit(&stcInit);

    /* 1. Modify the default value depends on the application. It is not needed in this example. */

    /* 2. Enable ADC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(APP_ADC_PERIP_CLK, Enable);

    /* 3. Initializes ADC. */
    ADC_Init(APP_ADC_UNIT, &stcInit);
}

/**
 * @brief  Configures ADC channel(s).
 * @param  None
 * @retval None
 */
static void AdcChannelConfig(void)
{
    uint8_t au8AdcSASplTime[] = APP_ADC_SA_SAMPLE_TIME;

    /* 1. Set the ADC pin to analog input mode. */
    AdcSetChannelPinAnalogMode(APP_ADC_UNIT, APP_ADC_PIN_ANALOG_CH);

    /* 2. Enable the ADC channels. */
    ADC_ChannelCmd(APP_ADC_UNIT, ADC_SEQ_A, \
                   APP_ADC_SA_CH, au8AdcSASplTime, \
                   Enable);
}

/**
 * @brief  Configures the analog input source for the extended channel.
 * @param  None
 * @retval None
 */
static void AdcExtChSrcConfig(void)
{
    PWC_AdcBufCmd(Enable);
    ADC_SetExChannelSrc(APP_ADC_UNIT, ADC_EXCH_SRC_INTERNAL);
}

/**
 * @brief  Set the pin(s) corresponding to the specified channel(s) to analog mode.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  M4_ADC1:                     ADC unit 1 instance register base.
 *   @arg  M4_ADC2:                     ADC unit 2 instance register base.
 *   @arg  M4_ADC3:                     ADC unit 3 instance register base.
 * @param  [in]  u32Channel             The specified channel(s).
 * @retval None
 */
static void AdcSetChannelPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint32_t u32Channel)
{
    uint8_t u8PinNum;

    u8PinNum = 0U;
    while (u32Channel != 0U)
    {
        if (u32Channel & 0x1U)
        {
            AdcSetPinAnalogMode(ADCx, u8PinNum);
        }

        u32Channel >>= 1U;
        u8PinNum++;
    }
}

/**
 * @brief  Set specified ADC pin to analog mode.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  M4_ADC1:                     ADC unit 1 instance register base.
 *   @arg  M4_ADC2:                     ADC unit 2 instance register base.
 *   @arg  M4_ADC3:                     ADC unit 3 instance register base.
 * @param  [in]  u8PinNum               The ADC pin number.
 *                                      This parameter can be a value of @ref ADC_Pin_Number
 * @retval None
 */
static void AdcSetPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint8_t u8PinNum)
{
    typedef struct
    {
        uint8_t  u8Port;
        uint16_t u16Pin;
    } stc_adc_pin_t;

    stc_gpio_init_t stcGpioInit;

    stc_adc_pin_t astcADC12[ADC1_CH_COUNT] = { \
            {GPIO_PORT_A, GPIO_PIN_00}, {GPIO_PORT_A, GPIO_PIN_01}, \
            {GPIO_PORT_A, GPIO_PIN_02}, {GPIO_PORT_A, GPIO_PIN_03}, \
            {GPIO_PORT_A, GPIO_PIN_04}, {GPIO_PORT_A, GPIO_PIN_05}, \
            {GPIO_PORT_A, GPIO_PIN_06}, {GPIO_PORT_A, GPIO_PIN_07}, \
            {GPIO_PORT_B, GPIO_PIN_00}, {GPIO_PORT_B, GPIO_PIN_01}, \
            {GPIO_PORT_C, GPIO_PIN_00}, {GPIO_PORT_C, GPIO_PIN_01}, \
            {GPIO_PORT_C, GPIO_PIN_02}, {GPIO_PORT_C, GPIO_PIN_03}, \
            {GPIO_PORT_C, GPIO_PIN_04}, {GPIO_PORT_C, GPIO_PIN_05}, \
    };
    stc_adc_pin_t astcADC3[ADC3_CH_COUNT] = { \
            {GPIO_PORT_A, GPIO_PIN_00}, {GPIO_PORT_A, GPIO_PIN_01}, \
            {GPIO_PORT_A, GPIO_PIN_02}, {GPIO_PORT_A, GPIO_PIN_03}, \
            {GPIO_PORT_F, GPIO_PIN_06}, {GPIO_PORT_F, GPIO_PIN_07}, \
            {GPIO_PORT_F, GPIO_PIN_08}, {GPIO_PORT_F, GPIO_PIN_09}, \
            {GPIO_PORT_F, GPIO_PIN_10}, {GPIO_PORT_F, GPIO_PIN_03}, \
            {GPIO_PORT_C, GPIO_PIN_00}, {GPIO_PORT_C, GPIO_PIN_01}, \
            {GPIO_PORT_C, GPIO_PIN_02}, {GPIO_PORT_C, GPIO_PIN_03}, \
            {GPIO_PORT_F, GPIO_PIN_04}, {GPIO_PORT_F, GPIO_PIN_05}, \
            {GPIO_PORT_H, GPIO_PIN_02}, {GPIO_PORT_H, GPIO_PIN_03}, \
            {GPIO_PORT_H, GPIO_PIN_04}, {GPIO_PORT_H, GPIO_PIN_05}, \
    };

    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;

    if ((ADCx == M4_ADC1) || (ADCx == M4_ADC2))
    {
        GPIO_Init(astcADC12[u8PinNum].u8Port, astcADC12[u8PinNum].u16Pin, &stcGpioInit);
    }
    else
    {
        GPIO_Init(astcADC3[u8PinNum].u8Port, astcADC3[u8PinNum].u16Pin, &stcGpioInit);
    }
}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
