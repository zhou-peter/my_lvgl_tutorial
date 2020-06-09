/**
 *******************************************************************************
 * @file  timer4/timer4_pwm_dead_timer_filter_mode/source/main.c
 * @brief This example demonstrates how to use the dead timer filter mode 
 *        function of Timer4 PWM function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-02-19       Hongjh          First version
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
 * @addtogroup TIMER4_PWM_Dead_Timer_Filter_Mode
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* TIM4 PWM Port/Pin definition */
#define TIM4_OXH_PORT                   (GPIO_PORT_E)
#define TIM4_OXH_PIN                    (GPIO_PIN_09)
#define TIM4_OXH_GPIO_FUNC              (GPIO_FUNC_2)

#define TIM4_OXL_PORT                   (GPIO_PORT_E)
#define TIM4_OXL_PIN                    (GPIO_PIN_08)
#define TIM4_OXL_GPIO_FUNC              (GPIO_FUNC_2)

/* Timer4 unit& fcg & counter period value definition */
#define TMR4_UNIT                       (M4_TMR4_1)
#define TMR4_FUNCTION_CLK_GATE          (PWC_FCG2_TMR4_1)
#define TMR4_CNT_CYCLE_VAL(div)         ((uint16_t)(Tmr4PclkFreq() / (2UL * (1UL << (uint32_t)(div))))) /* Period_Value(500ms) */

/* Timer4 OCO Channel definition */
#define TMR4_OCO_LOW_CH                 (TMR4_OCO_UL)    /* only TMR4_OCO_UL  TMR4_OCO_VL  TMR4_OCO_WL */
#define TMR4_OCO_CMP_BUF_SIZE           (4U)

/* Timer4 OCO get interrupt source number definition */
#define TMR4_1_OCO_INT_SRC(x)           ((en_int_src_t)((uint32_t)INT_TMR4_1_GCMUH + ((uint32_t)((x) - TMR4_OCO_UH))))

/* Timer4 PWM get channel by OCO low channel */
#define TMR4_PWM_CH(x)                  (((x) == TMR4_OCO_UL) ? TMR4_PWM_U : \
                                         (((x) == TMR4_OCO_VL) ? TMR4_PWM_V : TMR4_PWM_W))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static uint32_t Tmr4PclkFreq(void);
static void TMR4_OcoMatch_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t m_au16OcoCmpVal[TMR4_OCO_CMP_BUF_SIZE];

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
 * @brief  TIMER4 OCO match interrupt handler callback.
 * @param  None
 * @retval None
 */
static void TMR4_OcoMatch_IrqCallback(void)
{
    static uint8_t u8CntOcoMatchIrq = 0U;

    if (++u8CntOcoMatchIrq >= TMR4_OCO_CMP_BUF_SIZE)
    {
        u8CntOcoMatchIrq = 0U;
    }

    TMR4_OCO_ClearFlag(TMR4_UNIT, TMR4_OCO_LOW_CH);
    TMR4_OCO_SetCompareVal(TMR4_UNIT, TMR4_OCO_LOW_CH, m_au16OcoCmpVal[u8CntOcoMatchIrq]);
}

/**
 * @brief  Main function of TIMER4 PWM dead timer filter mode
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint32_t u32PwmCh;
    stc_irq_signin_config_t stcIrqSigninCfg;
    stc_tmr4_cnt_init_t stcTmr4CntInit;
    stc_tmr4_oco_init_t stcTmr4OcoInit;
    stc_tmr4_pwm_init_t stcTmr4PwmInit;
    stc_oco_low_ch_compare_mode_t stcLowChCmpMode;

    /* Initialize clock. */
    BSP_CLK_Init();
    CLK_ClkDiv(CLK_CATE_ALL, (CLK_PCLK0_DIV32 | CLK_PCLK1_DIV32 | \
                              CLK_PCLK2_DIV4  | CLK_PCLK3_DIV32 | \
                              CLK_PCLK4_DIV2  | CLK_EXCLK_DIV2  | CLK_HCLK_DIV1));

    /* Enable peripheral clock */
    PWC_Fcg2PeriphClockCmd(TMR4_FUNCTION_CLK_GATE, Enable);

    /* Initialize TIMER4 Counter */
    TMR4_CNT_StructInit(&stcTmr4CntInit);
    stcTmr4CntInit.u16ClkDiv = TMR4_CNT_CLK_DIV128;
    stcTmr4CntInit.u16CycleVal = TMR4_CNT_CYCLE_VAL(stcTmr4CntInit.u16ClkDiv);
    TMR4_CNT_Init(TMR4_UNIT, &stcTmr4CntInit);

    /* Initialize TIMER4 OCO high&&low channel */
    m_au16OcoCmpVal[0] = (stcTmr4CntInit.u16CycleVal / 8U);
    m_au16OcoCmpVal[1] = (stcTmr4CntInit.u16CycleVal / 4U);
    m_au16OcoCmpVal[2] = (stcTmr4CntInit.u16CycleVal / 2U);
    m_au16OcoCmpVal[3] = (stcTmr4CntInit.u16CycleVal / 2U);
    TMR4_OCO_StructInit(&stcTmr4OcoInit);
    stcTmr4OcoInit.u16CompareVal = m_au16OcoCmpVal[0];
    TMR4_OCO_Init(TMR4_UNIT, TMR4_OCO_LOW_CH, &stcTmr4OcoInit);
    TMR4_OCO_IntCmd(TMR4_UNIT, TMR4_OCO_LOW_CH, Enable);

    if (TMR4_OCO_LOW_CH % 2UL)
    {
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
        TMR4_OCO_SetLowChCompareMode(TMR4_UNIT, TMR4_OCO_LOW_CH, &stcLowChCmpMode);  /* Set OCO low channel compare mode */
    }

    /* Register IRQ handler && configure NVIC. */
    stcIrqSigninCfg.enIRQn = Int000_IRQn;
    stcIrqSigninCfg.enIntSrc =TMR4_1_OCO_INT_SRC(TMR4_OCO_LOW_CH);
    stcIrqSigninCfg.pfnCallback = &TMR4_OcoMatch_IrqCallback;
    INTC_IrqSignIn(&stcIrqSigninCfg);
    NVIC_ClearPendingIRQ(stcIrqSigninCfg.enIRQn);
    NVIC_SetPriority(stcIrqSigninCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqSigninCfg.enIRQn);

    /* Initialize PWM I/O */
    GPIO_Unlock();
    GPIO_SetFunc(TIM4_OXH_PORT, TIM4_OXH_PIN, TIM4_OXH_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TIM4_OXL_PORT, TIM4_OXL_PIN, TIM4_OXL_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_Lock();

    /* Timer4 PWM: Get pwm couple channel */
    u32PwmCh = TMR4_PWM_CH(TMR4_OCO_LOW_CH);

    /* Initialize Timer4 PWM */
    TMR4_PWM_StructInit(&stcTmr4PwmInit);
    stcTmr4PwmInit.u16ClkDiv = TMR4_PWM_CLK_DIV128;
    stcTmr4PwmInit.u16Mode = TMR4_PWM_DEAD_TIMER_FILTER_MODE;
    TMR4_PWM_Init(TMR4_UNIT, u32PwmCh, &stcTmr4PwmInit);
    TMR4_PWM_SetDeadRegionValue(TMR4_UNIT, u32PwmCh, 1U, 1U);
    TMR4_PWM_SetFilterCountValue(TMR4_UNIT, u32PwmCh, (m_au16OcoCmpVal[1] + m_au16OcoCmpVal[2])/2U);

    /* Start TIMER4 counter. */
    TMR4_CNT_Start(TMR4_UNIT);

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
