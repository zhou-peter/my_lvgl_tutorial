/**
 *******************************************************************************
 * @file  emb/emb_osc_brake_timer6/source/main.c
 * @brief This example demonstrates how to use OSC failure brake function of 
 *        EMB function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-03-12       Hongjh          First version
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
 * @addtogroup EMB_OSC_Brake_Timer6
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Key definition */
#define USER_KEY                        (BSP_KEY_1)

/* EMB unit & fcg & interrupt number definition */
#define EMB_UNIT                        (M4_EMB0)
#define EMB_FUNCTION_CLK_GATE           (PWC_FCG2_EMB)
#define EMB_INT_SRC                     (INT_EMB_GR0)
#define EMB_INT_IRQn                    (Int000_IRQn)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void XTALClockConfig(void);
static uint32_t Tmr6PclkFreq(void);
static void Tmr6PwmConfig(void);
static void EMB_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Configure XTAL clock
 * @param  None
 * @retval None
 */
static void XTALClockConfig(void)
{
    stc_clk_xtal_init_t stcXtalInit;
    stc_clk_xtalstd_init_t stcXtalstdInit;

    /* Config XTAL and Enable */
    CLK_XtalStrucInit(&stcXtalInit);
    stcXtalInit.u8XtalState = CLK_XTAL_ON;
    stcXtalInit.u8XtalMode = CLK_XTALMODE_OSC;
    stcXtalInit.u8XtalDrv = CLK_XTALDRV_HIGH;
    stcXtalInit.u8XtalStb = CLK_XTALSTB_499US;
    CLK_XtalInit(&stcXtalInit);

    /* Enable XTAL fault dectect and occur reset. */
    stcXtalstdInit.u8XtalStdState = CLK_XTALSTD_ON;
    stcXtalstdInit.u8XtalStdMode = CLK_XTALSTD_MODE_INT;
    stcXtalstdInit.u8XtalStdInt = CLK_XTALSTD_INT_ON;
    stcXtalstdInit.u8XtalStdRst = CLK_XTALSTD_RST_OFF;
    CLK_XtalStdInit(&stcXtalstdInit);
}

/**
 * @brief  Get TIMER6 PCLK frequency.
 * @param  None
 * @retval None
 */
static uint32_t Tmr6PclkFreq(void)
{
    stc_clk_freq_t stcClkFreq;

    CLK_GetClockFreq(&stcClkFreq);
    return stcClkFreq.pclk0Freq;
}

/**
 * @brief  Configure Timer6 PWM
 * @param  None
 * @retval None
 */
static void Tmr6PwmConfig(void)
{
    uint32_t u32Period;
    stc_tmr6_basecnt_cfg_t stcTmr6BaseCntCfg;
    stc_tmr6_port_output_cfg_t stcTmr6PortOutCfg;

    TMR6_BaseCntStructInit(&stcTmr6BaseCntCfg);
    TMR6_PortOutputStructInit(&stcTmr6PortOutCfg);

    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_1, Enable);

    /* Timer6 PWM output port configuration */
    GPIO_Unlock();
    GPIO_SetFunc(GPIO_PORT_B, GPIO_PIN_09, GPIO_FUNC_3_TIM61, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(GPIO_PORT_B, GPIO_PIN_08, GPIO_FUNC_3_TIM61, PIN_SUBFUNC_DISABLE);
    GPIO_Lock();

    /* Timer6 general count function configuration */
    stcTmr6BaseCntCfg.u32CntMode = TMR6_MODE_SAWTOOTH;
    stcTmr6BaseCntCfg.u32CntDir = TMR6_CNT_INCREASE;
    stcTmr6BaseCntCfg.u32CntClkDiv = TMR6_CLK_PCLK0_DIV128;
    stcTmr6BaseCntCfg.u32CntStpAftOvf = TMR6_CNT_CONTINUOUS;
    TMR6_Init(M4_TMR6_1, &stcTmr6BaseCntCfg);

    /* Set Period register/Compare RegisterA/Compare RegisterB Value */
    u32Period = Tmr6PclkFreq() / (1UL << (uint32_t)(stcTmr6BaseCntCfg.u32CntClkDiv >> TMR6_GCONR_CKDIV_POS)); /* Period_Value(500ms) */
    TMR6_SetPeriodReg(M4_TMR6_1, TMR6_PERIOD_REG_A, u32Period);
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_A, u32Period/2UL);
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_B, u32Period/2UL);

     /* Configurate PWM output */
    stcTmr6PortOutCfg.u32PortMode = TMR6_PORT_COMPARE_OUTPUT;
    stcTmr6PortOutCfg.u32NextPeriodForceStd = TMR6_PORT_OUTPUT_STD_LOW;
    stcTmr6PortOutCfg.u32DownCntMatchAnotherCmpRegStd = TMR6_PORT_OUTPUT_STD_HOLD;
    stcTmr6PortOutCfg.u32UpCntMatchAnotherCmpRegStd = TMR6_PORT_OUTPUT_STD_HOLD;
    stcTmr6PortOutCfg.u32DownCntMatchCmpRegStd = TMR6_PORT_OUTPUT_STD_LOW;
    stcTmr6PortOutCfg.u32UpCntMatchCmpRegStd = TMR6_PORT_OUTPUT_STD_HIGH;
    stcTmr6PortOutCfg.u32UnderflowStd = TMR6_PORT_OUTPUT_STD_HOLD;
    stcTmr6PortOutCfg.u32OverflowStd = TMR6_PORT_OUTPUT_STD_LOW;
    stcTmr6PortOutCfg.u32StopStd = TMR6_PORT_OUTPUT_STD_LOW;
    stcTmr6PortOutCfg.u32StartStd = TMR6_PORT_OUTPUT_STD_LOW;
    TMR6_PortOutputConfig(M4_TMR6_1, TMR6_IO_PWMA, &stcTmr6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_1, TMR6_IO_PWMB, &stcTmr6PortOutCfg);

    /* Start Timer6 */
    TMR6_CountCmd(M4_TMR6_1, Enable);
}

/**
 * @brief  EMB IRQ callback function.
 * @param  None
 * @retval None
 */
static void EMB_IrqCallback(void)
{
    if (Set == EMB_GetFlag(EMB_UNIT, EMB_FLAG_OSC))
    {
        /* Wait key release */
        while (Reset == BSP_KEY_GetStatus(USER_KEY))
        {
        }

        CLK_ClearXtalStdFlag();
        EMB_ClearFlag(EMB_UNIT, EMB_FLAG_OSC);  /* Clear OSC Brake flag */
    }
}

/**
 * @brief  Main function of EMB OSC failure brake
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_tmr6_emb_cfg_t stcTmr6EmbCfg;
    stc_emb_tmr6_init_t stcEmbInit;
    stc_irq_signin_config_t stcIrqSigninCfg;

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Initialize IO. */
    BSP_IO_Init();

    /* Initialize key. */
    BSP_KEY_Init();

    /* Configure XTAL clock. */
    XTALClockConfig();

    /* Configure Timer6 PWM. */
    Tmr6PwmConfig();

    /* Configure Timer6 EMB. */
    TMR6_EMBCfgStructInit(&stcTmr6EmbCfg);
    stcTmr6EmbCfg.u32ValidCh = TMR6_EMB_EVENT_VALID_CH0;
    stcTmr6EmbCfg.u32ReleaseMode = TMR6_EMB_RELESE_IMMEDIATE;
    stcTmr6EmbCfg.u32PortStd = TMR6_EMB_PORTSTD_LOW;
    TMR6_EMBConfig(M4_TMR6_1, TMR6_IO_PWMA, &stcTmr6EmbCfg);
    TMR6_EMBConfig(M4_TMR6_1, TMR6_IO_PWMB, &stcTmr6EmbCfg);

    /* Configure EMB. */
    PWC_Fcg2PeriphClockCmd(EMB_FUNCTION_CLK_GATE, Enable);
    EMB_Tmr6StructInit(&stcEmbInit);
    stcEmbInit.u32Osc = EMB_OSC_ENABLE;
    EMB_Tmr6Init(EMB_UNIT, &stcEmbInit);

    EMB_IntCmd(EMB_UNIT, EMB_INT_OSC, Enable);
    EMB_SetReleasePwmMode(EMB_UNIT, EMB_EVENT_OSC, EMB_RELEASE_PWM_SEL_FLAG_ZERO);

    /* Register IRQ handler && configure NVIC. */
    stcIrqSigninCfg.enIRQn = EMB_INT_IRQn;
    stcIrqSigninCfg.enIntSrc = EMB_INT_SRC;
    stcIrqSigninCfg.pfnCallback = &EMB_IrqCallback;
    INTC_IrqSignIn(&stcIrqSigninCfg);
    NVIC_ClearPendingIRQ(stcIrqSigninCfg.enIRQn);
    NVIC_SetPriority(stcIrqSigninCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqSigninCfg.enIRQn);

    while (1)
    {
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
