/**
 *******************************************************************************
 * @file  timer4/timer4_oco_link_double_ch/source/main.c
 * @brief This example demonstrates how to use the link double channel of Timer4
 *        OCO function.
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
 * @addtogroup TIMER4_OCO_Link_Double_Channels
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Wave I/O Port/Pin definition */
#define WAVE_IO_PORT                    (GPIO_PORT_E)
#define WAVE_IO_PIN                     (GPIO_PIN_09)
#define WAVE_IO_TOGGLE()                do {        \
    GPIO_TogglePins(WAVE_IO_PORT, WAVE_IO_PIN);     \
} while(0)

/* Timer4 unit& fcg & counter period value definition */
#define TMR4_UNIT                       (M4_TMR4_1)
#define TMR4_FUNCTION_CLK_GATE          (PWC_FCG2_TMR4_1)
#define TMR4_CNT_CYCLE_VAL(div)         ((uint16_t)(Tmr4PclkFreq() /  (1UL << (uint32_t)(div)))) /* Period_Value(1s) */

/* Timer4 OCO Channel definition */
#define TMR4_OCO_HIGH_CH                (TMR4_OCO_UH)    /* only TMR4_OCO_UH  TMR4_OCO_VH  TMR4_OCO_WH */
#define TMR4_OCO_LOW_CH                 (TMR4_OCO_UL)    /* only TMR4_OCO_UL  TMR4_OCO_VL  TMR4_OCO_WL */

/* Timer4 OCO get interrupt source number definition */
#define TMR4_1_OCO_INT_SRC(x)           ((en_int_src_t)((uint32_t)INT_TMR4_1_GCMUH + ((uint32_t)((x) - TMR4_OCO_UH))))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void WaveIoConfig(void);
static uint32_t Tmr4PclkFreq(void);
static void TMR4_OcoMatch_IrqCallback(void);
static void InstalIrqHandler(const stc_irq_signin_config_t *pstcConfig,
                                 uint32_t Priority);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Configure Wave I/O.
 * @param  None
 * @retval None
 */
static void WaveIoConfig(void)
{
    stc_gpio_init_t stcGpioInit;

    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinState = PIN_STATE_RESET;
    GPIO_Init(WAVE_IO_PORT, WAVE_IO_PIN, &stcGpioInit);
}

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
    static uint16_t m_u16OcoOp = TMR4_OCO_INVAILD_OP_LOW;
    uint16_t u16OcoOp = TMR4_OCO_GetOutputPolarity(TMR4_UNIT, TMR4_OCO_LOW_CH);

    if (m_u16OcoOp != u16OcoOp)
    {
        WAVE_IO_TOGGLE();
        m_u16OcoOp = u16OcoOp;
    }

    TMR4_OCO_ClearFlag(TMR4_UNIT, TMR4_OCO_LOW_CH, TMR4_OCO_FLAG_MATCH);
    TMR4_OCO_ClearFlag(TMR4_UNIT, TMR4_OCO_LOW_CH - 1UL, TMR4_OCO_FLAG_MATCH);
}

/**
 * @brief  Instal IRQ handler.
 * @param  [in] pstcConfig      Pointer to struct @ref stc_irq_signin_config_t
 * @param  [in] Priority        Interrupt priority
 * @retval None
 */
static void InstalIrqHandler(const stc_irq_signin_config_t *pstcConfig,
                                 uint32_t Priority)
{
    if (NULL != pstcConfig)
    {
        INTC_IrqSignIn(pstcConfig);
        NVIC_ClearPendingIRQ(pstcConfig->enIRQn);
        NVIC_SetPriority(pstcConfig->enIRQn, Priority);
        NVIC_EnableIRQ(pstcConfig->enIRQn);
    }
}

/**
 * @brief  Main function of TIMER4 OCO link double channels
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_signin_config_t stcIrqSigninCfg;
    stc_tmr4_cnt_init_t stcTmr4CntInit;
    stc_tmr4_oco_init_t stcTmr4OcoInit;
    stc_oco_high_ch_compare_mode_t stcHighChCmpMode;
    stc_oco_low_ch_compare_mode_t stcLowChCmpMode;

    /* Initialize clock. */
    BSP_CLK_Init();
    CLK_ClkDiv(CLK_CATE_ALL, (CLK_PCLK0_DIV32 | CLK_PCLK1_DIV32 | \
                              CLK_PCLK2_DIV4  | CLK_PCLK3_DIV32 | \
                              CLK_PCLK4_DIV2  | CLK_EXCLK_DIV2  | CLK_HCLK_DIV1));

    /* Configure Wave I/O. */
    WaveIoConfig();

    /* Enable peripheral clock */
    PWC_Fcg2PeriphClockCmd(TMR4_FUNCTION_CLK_GATE, Enable);

    /* Initialize TIMER4 Counter */
    TMR4_CNT_StructInit(&stcTmr4CntInit);
    stcTmr4CntInit.u16ClkDiv = TMR4_CNT_CLK_DIV512;
    stcTmr4CntInit.u16CycleVal = TMR4_CNT_CYCLE_VAL(stcTmr4CntInit.u16ClkDiv);
    TMR4_CNT_Init(TMR4_UNIT, &stcTmr4CntInit);

    TMR4_OCO_StructInit(&stcTmr4OcoInit);

    /* Initialize TIMER4 OCO high channel */
    stcTmr4OcoInit.u16CompareVal = stcTmr4CntInit.u16CycleVal / 4U;
    TMR4_OCO_Init(TMR4_UNIT, TMR4_OCO_HIGH_CH, &stcTmr4OcoInit);
    TMR4_OCO_IntCmd(TMR4_UNIT, TMR4_OCO_HIGH_CH, TMR4_OCO_INT_MATCH, Enable);

    /* Initialize TIMER4 OCO low channel */
    stcTmr4OcoInit.u16CompareVal = (stcTmr4CntInit.u16CycleVal / 4U) * 3U;
    TMR4_OCO_Init(TMR4_UNIT, TMR4_OCO_LOW_CH, &stcTmr4OcoInit);
    TMR4_OCO_IntCmd(TMR4_UNIT, TMR4_OCO_LOW_CH, TMR4_OCO_INT_MATCH, Enable);

    if (!(TMR4_OCO_HIGH_CH%2UL))
    {
        /* OCMR[15:0] = 0x000F = b 0000 0000 0000 1111 */
        stcHighChCmpMode.OCMRx_f.OCFDCH = TMR4_OCO_OCF_SET;   /* bit[0] 1 */
        stcHighChCmpMode.OCMRx_f.OCFPKH = TMR4_OCO_OCF_SET;   /* bit[1] 1 */
        stcHighChCmpMode.OCMRx_f.OCFUCH = TMR4_OCO_OCF_SET;   /* bit[2] 1 */
        stcHighChCmpMode.OCMRx_f.OCFZRH = TMR4_OCO_OCF_SET;   /* bit[3] 1 */

        stcHighChCmpMode.OCMRx_f.OPDCH  = TMR4_OCO_OP_HOLD;   /* Bit[5:4]    00 */
        stcHighChCmpMode.OCMRx_f.OPPKH  = TMR4_OCO_OP_HOLD;   /* Bit[7:6]    00 */
        stcHighChCmpMode.OCMRx_f.OPUCH  = TMR4_OCO_OP_HOLD;   /* Bit[9:8]    00 */
        stcHighChCmpMode.OCMRx_f.OPZRH  = TMR4_OCO_OP_HOLD;   /* Bit[11:10]  00 */
        stcHighChCmpMode.OCMRx_f.OPNPKH = TMR4_OCO_OP_HOLD;   /* Bit[13:12]  00 */
        stcHighChCmpMode.OCMRx_f.OPNZRH = TMR4_OCO_OP_HOLD;   /* Bit[15:14]  00 */

        stcHighChCmpMode.u16ExtendMatch = TMR4_OCO_EXTEND_MATCH_DISABLE;

        TMR4_OCO_SetHighChCompareMode(TMR4_UNIT, TMR4_OCO_HIGH_CH, &stcHighChCmpMode);  /* Set OCO high channel compare mode */
    }

    if (TMR4_OCO_LOW_CH%2UL)
    {
        /* OCMR[31:0] Ox FFFF 0FFF = b 1111 1111 1111 1111   0000 1111 1111 1111 */
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
        stcLowChCmpMode.OCMRx_f.EOPNDCL = TMR4_OCO_OP_INVERT; /* bit[17:16]  00 */
        stcLowChCmpMode.OCMRx_f.EOPNUCL = TMR4_OCO_OP_INVERT; /* bit[19:18]  00 */
        stcLowChCmpMode.OCMRx_f.EOPDCL = TMR4_OCO_OP_INVERT;  /* bit[21:20]  11 */
        stcLowChCmpMode.OCMRx_f.EOPPKL = TMR4_OCO_OP_INVERT;  /* bit[23:22]  11 */
        stcLowChCmpMode.OCMRx_f.EOPUCL = TMR4_OCO_OP_INVERT;  /* bit[25:24]  11 */
        stcLowChCmpMode.OCMRx_f.EOPZRL = TMR4_OCO_OP_INVERT;  /* bit[27:26]  11 */
        stcLowChCmpMode.OCMRx_f.EOPNPKL = TMR4_OCO_OP_INVERT; /* bit[29:28]  11 */
        stcLowChCmpMode.OCMRx_f.EOPNZRL = TMR4_OCO_OP_INVERT; /* bit[31:30]  11 */

        stcLowChCmpMode.u16ExtendMatch = TMR4_OCO_EXTEND_MATCH_DISABLE;

        TMR4_OCO_SetLowChCompareMode(TMR4_UNIT, TMR4_OCO_LOW_CH, &stcLowChCmpMode);  /* Set OCO low channel compare mode */
    }

    /* Register IRQ handler && configure NVIC. */
    stcIrqSigninCfg.enIRQn = Int000_IRQn;
    stcIrqSigninCfg.enIntSrc = TMR4_1_OCO_INT_SRC(TMR4_OCO_HIGH_CH);
    stcIrqSigninCfg.pfnCallback = &TMR4_OcoMatch_IrqCallback;
    InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_DEFAULT);

    stcIrqSigninCfg.enIRQn = Int001_IRQn;
    stcIrqSigninCfg.enIntSrc = TMR4_1_OCO_INT_SRC(TMR4_OCO_LOW_CH);
    stcIrqSigninCfg.pfnCallback = &TMR4_OcoMatch_IrqCallback;
    InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_DEFAULT);

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