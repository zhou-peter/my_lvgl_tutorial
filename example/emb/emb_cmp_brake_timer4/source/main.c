/**
 *******************************************************************************
 * @file  emb/emb_cmp_brake_timer4/source/main.c
 * @brief This example demonstrates how to use CMP brake function of EMB 
 *        function.
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
 * @addtogroup EMB_CMP_Brake_Timer4
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* EMB unit & fcg & interrupt number definition */
#define EMB_UNIT                        (M4_EMB4)
#define EMB_FUNCTION_CLK_GATE           (PWC_FCG2_EMB)
#define EMB_INT_SRC                     (INT_EMB_GR4)
#define EMB_INT_IRQn                    (Int000_IRQn)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static uint32_t Tmr4PclkFreq(void);
static void Timer4PwmConfig(void);
static void CmpConfig(void);
static void DacConfig(void);
static void EMB_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Get TIMER4 PCLK frequency.
 * @param  None
 * @retval None
 */
static uint32_t Tmr4PclkFreq(void)
{
    stc_clk_freq_t stcClkFreq;

    CLK_GetClockFreq(&stcClkFreq);
    return stcClkFreq.pclk0Freq;
}

/**
 * @brief  Configure Timer4 PWM
 * @param  None
 * @retval None
 */
static void Timer4PwmConfig(void)
{
    stc_tmr4_cnt_init_t stcTmr4CntInit;
    stc_tmr4_oco_init_t stcTmr4OcoInit;
    stc_tmr4_pwm_init_t stcTmr4PwmInit;
    stc_oco_high_ch_compare_mode_t stcHighChCmpMode;
    stc_oco_low_ch_compare_mode_t stcLowChCmpMode;

    /* Enable peripheral clock */
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR4_1, Enable);

    /* Initialize TIMER4 Counter */
    TMR4_CNT_StructInit(&stcTmr4CntInit);
    stcTmr4CntInit.u16ClkDiv = TMR4_CNT_CLK_DIV512;
    stcTmr4CntInit.u16CycleVal = (uint16_t)(Tmr4PclkFreq() / (2UL * (1UL << (uint32_t)(stcTmr4CntInit.u16ClkDiv)))); /* Period_Value(500ms) */
    TMR4_CNT_Init(M4_TMR4_1, &stcTmr4CntInit);

    /* Initialize TIMER4 OCO high&&low channel */
    TMR4_OCO_StructInit(&stcTmr4OcoInit);
    stcTmr4OcoInit.u16CompareVal = stcTmr4CntInit.u16CycleVal/2U;
    TMR4_OCO_Init(M4_TMR4_1, TMR4_OCO_UH, &stcTmr4OcoInit);
    TMR4_OCO_Init(M4_TMR4_1, TMR4_OCO_UL, &stcTmr4OcoInit);

    /* OCMR[15:0] = 0x0FFF = b 0000 1111 1111 1111 */
    stcHighChCmpMode.OCMRx_f.OCFDCH = TMR4_OCO_OCF_SET;   /* bit[0] 1 */
    stcHighChCmpMode.OCMRx_f.OCFPKH = TMR4_OCO_OCF_SET;   /* bit[1] 1 */
    stcHighChCmpMode.OCMRx_f.OCFUCH = TMR4_OCO_OCF_SET;   /* bit[2] 1 */
    stcHighChCmpMode.OCMRx_f.OCFZRH = TMR4_OCO_OCF_SET;   /* bit[3] 1 */
    stcHighChCmpMode.OCMRx_f.OPDCH = TMR4_OCO_OP_INVERT;  /* Bit[5:4]    11 */
    stcHighChCmpMode.OCMRx_f.OPPKH = TMR4_OCO_OP_INVERT;  /* Bit[7:6]    11 */
    stcHighChCmpMode.OCMRx_f.OPUCH = TMR4_OCO_OP_INVERT;  /* Bit[9:8]    11 */
    stcHighChCmpMode.OCMRx_f.OPZRH = TMR4_OCO_OP_INVERT;  /* Bit[11:10]  11 */
    stcHighChCmpMode.OCMRx_f.OPNPKH = TMR4_OCO_OP_HOLD;   /* Bit[13:12]  00 */
    stcHighChCmpMode.OCMRx_f.OPNZRH = TMR4_OCO_OP_HOLD;   /* Bit[15:14]  00 */
    stcHighChCmpMode.u16ExtendMatch = TMR4_OCO_EXTEND_MATCH_DISABLE;
    TMR4_OCO_SetHighChCompareMode(M4_TMR4_1, TMR4_OCO_UH, &stcHighChCmpMode);  /* Set OCO high channel compare mode */

    /* OCMR[31:0] 0x0FF0 0FFF = b 0000 1111 1111 0000   0000 1111 1111 1111 */
    stcLowChCmpMode.OCMRx_f.OCFDCL = TMR4_OCO_OCF_SET;    /* bit[0] 1 */
    stcLowChCmpMode.OCMRx_f.OCFPKL = TMR4_OCO_OCF_SET;    /* bit[1] 1 */
    stcLowChCmpMode.OCMRx_f.OCFUCL = TMR4_OCO_OCF_SET;    /* bit[2] 1 */
    stcLowChCmpMode.OCMRx_f.OCFZRL = TMR4_OCO_OCF_SET;    /* bit[3] 1 */
    stcLowChCmpMode.OCMRx_f.OPDCL = TMR4_OCO_OP_INVERT;   /* bit[5:4]    11 */
    stcLowChCmpMode.OCMRx_f.OPPKL = TMR4_OCO_OP_INVERT;   /* bit[7:6]    11 */
    stcLowChCmpMode.OCMRx_f.OPUCL = TMR4_OCO_OP_INVERT;   /* bit[9:8]    11 */
    stcLowChCmpMode.OCMRx_f.OPZRL = TMR4_OCO_OP_INVERT;   /* bit[11:10]  11 */
    stcLowChCmpMode.OCMRx_f.OPNPKL = TMR4_OCO_OP_HOLD;    /* bit[13:12]  00 */
    stcLowChCmpMode.OCMRx_f.OPNZRL = TMR4_OCO_OP_HOLD;    /* bit[15:14]  00 */
    stcLowChCmpMode.OCMRx_f.EOPNDCL = TMR4_OCO_OP_HOLD;   /* bit[17:16]  00 */
    stcLowChCmpMode.OCMRx_f.EOPNUCL = TMR4_OCO_OP_HOLD;   /* bit[19:18]  00 */
    stcLowChCmpMode.OCMRx_f.EOPDCL = TMR4_OCO_OP_INVERT;  /* bit[21:20]  11 */
    stcLowChCmpMode.OCMRx_f.EOPPKL = TMR4_OCO_OP_INVERT;  /* bit[23:22]  11 */
    stcLowChCmpMode.OCMRx_f.EOPUCL = TMR4_OCO_OP_INVERT;  /* bit[25:24]  11 */
    stcLowChCmpMode.OCMRx_f.EOPZRL = TMR4_OCO_OP_INVERT;  /* bit[27:26]  11 */
    stcLowChCmpMode.OCMRx_f.EOPNPKL = TMR4_OCO_OP_HOLD;   /* bit[29:28]  00 */
    stcLowChCmpMode.OCMRx_f.EOPNZRL = TMR4_OCO_OP_HOLD;   /* bit[31:30]  00 */
    stcLowChCmpMode.u16ExtendMatch = TMR4_OCO_EXTEND_MATCH_DISABLE;
    TMR4_OCO_SetLowChCompareMode(M4_TMR4_1, TMR4_OCO_UL, &stcLowChCmpMode);  /* Set OCO low channel compare mode */

    /* Initialize PWM I/O */
    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_09, GPIO_FUNC_2, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_08, GPIO_FUNC_2, PIN_SUBFUNC_DISABLE);

    /* Initialize Timer4 PWM */
    TMR4_PWM_StructInit(&stcTmr4PwmInit);
    stcTmr4PwmInit.u32OxHPortOutMode = TMR4_PWM_PORT_OUTPUT_NORMAL;
    stcTmr4PwmInit.u32EmbOxHPortState = TMR4_PWM_EMB_PORT_OUTPUT_LOW;
    stcTmr4PwmInit.u32OxLPortOutMode = TMR4_PWM_PORT_OUTPUT_NORMAL;
    stcTmr4PwmInit.u32EmbOxLPortState = TMR4_PWM_EMB_PORT_OUTPUT_LOW;
    TMR4_PWM_Init(M4_TMR4_1, TMR4_PWM_U, &stcTmr4PwmInit);
    TMR4_PWM_MasterOutputCmd(M4_TMR4_1, Enable);
    TMR4_PWM_AutoOutputCmd(M4_TMR4_1, Enable);

    /* Start TIMER4 counter. */
    TMR4_CNT_Start(M4_TMR4_1);
}

/**
 * @brief  Configure CMP
 * @param  None
 * @retval None
 */
static void CmpConfig(void)
{
    stc_cmp_init_t stcCmpInit;
    stc_gpio_init_t stcGpioInit;

    /* Enable peripheral Clock */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_CMP1, Enable);

    /* Port function configuration for CMP*/
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    GPIO_Init(GPIO_PORT_E, GPIO_PIN_10, &stcGpioInit); /* CMP1_INP3 compare voltage */

    /* Configuration for normal compare function */
    CMP_StructInit(&stcCmpInit);
    stcCmpInit.u8CmpCh = CMP_CVSL_INP3;
    stcCmpInit.u16CmpVol = CMP1_INP3_CMP1_INP3;
    stcCmpInit.u8RefVol = CMP_RVSL_INM1;
    stcCmpInit.u8OutDetectEdges = CMP_DETECT_EDGS_BOTH;
    stcCmpInit.u8OutFilter = CMP_OUT_FILTER_PCLKDIV32;
    stcCmpInit.u8OutPolarity = CMP_OUT_REVERSE_OFF;
    CMP_NormalModeInit(M4_CMP1, &stcCmpInit);

    /* Enable CMP output */
    CMP_OutputCmd(M4_CMP1, Enable);

    /* Enable VCOUT */
    CMP_VCOUTCmd(M4_CMP1, Enable);

}

/**
 * @brief  Configure DAC.
 * @param  None
 * @retval None
 */
static void DacConfig(void)
{
    /* Enable peripheral Clock */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_DAC1, Enable);

    /* Right alignment of data */
    DAC_DataPatternConfig(M4_DAC1, DAC_Align_12b_R);

    /* Write Data :V = (Conversion Data / 4096) * VREFH */
    DAC_SetChannel1Data(M4_DAC1, 4096U/2U);

    /* Output Enable */
    DAC_ChannelAllCmd(M4_DAC1, Enable);

    /* Start Convert */
    DAC_ChannelCmd(M4_DAC1, DAC_Channel_1, Enable);
}

/**
 * @brief  EMB IRQ callback function.
 * @param  None
 * @retval None
 */
static void EMB_IrqCallback(void)
{
    if(Set == EMB_GetFlag(EMB_UNIT, EMB_FLAG_CMP))
    {
        EMB_ClearFlag(EMB_UNIT, EMB_FLAG_CMP);  /* Clear CMP Brake */
    }
}

/**
 * @brief  Main function of EMB CMP brake Timer4
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_emb_tmr4_init_t stcEmbInit;
    stc_irq_signin_config_t stcIrqSigninCfg;

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Configure DAC. */
    DacConfig();

    /* Configure CMP. */
    CmpConfig();

    /* Configure Timer4 PWM. */
    Timer4PwmConfig();

    /* Configure EMB. */
    PWC_Fcg2PeriphClockCmd(EMB_FUNCTION_CLK_GATE, Enable);
    EMB_Timer4StructInit(&stcEmbInit);
    stcEmbInit.u32Cmp1 = EMB_CMP1_ENABLE;
    EMB_Timer4Init(EMB_UNIT, &stcEmbInit);
    EMB_IntCmd(EMB_UNIT, EMB_INT_CMP, Enable);
    EMB_SetReleasePwmMode(EMB_UNIT, EMB_EVENT_CMP, EMB_RELEALSE_PWM_SEL_FLAG_ZERO);

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
