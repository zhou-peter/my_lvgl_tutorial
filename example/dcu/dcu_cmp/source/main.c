/**
 *******************************************************************************
 * @file  dcu/dcu_cmp/source/main.c
 * @brief This example demonstrates DCU cmp function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-02-11       Hongjh          First version
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
 * @addtogroup DCU_Cmp
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* DCU unit && Function clock gate definition */
#define DCU_UNIT                        (M4_DCU1)
#define DCU_FUNCTION_CLK_GATE           (PWC_FCG0_DCU1)

/* DCU unit interrupt definition */
#define DCU_UNIT_INT_SRC                (INT_DCU1)
#define DCU_UNIT_INT_IRQn               (Int000_IRQn)

/* DCU unit interrupt selection */
#define DCU_INT_SELECTION                                                      \
(   DCU_INT_DATA0_LS_DATA2              |                                      \
    DCU_INT_DATA0_EQ_DATA2              |                                      \
    DCU_INT_DATA0_GT_DATA2              |                                      \
    DCU_INT_DATA0_LS_DATA1              |                                      \
    DCU_INT_DATA0_EQ_DATA1              |                                      \
    DCU_INT_DATA0_GT_DATA1)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void DCU_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static en_int_status_t m_enData0LsData1 = Reset;
static en_int_status_t m_enData0LsData2 = Reset;

static en_int_status_t m_enData0EqData1 = Reset;
static en_int_status_t m_enData0EqData2 = Reset;

static en_int_status_t m_enData0GtData1 = Reset;
static en_int_status_t m_enData0GtData2 = Reset;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  DCU irq callback function.
 * @param  None
 * @retval None
 */
static void DCU_IrqCallback(void)
{
    if (Set == DCU_GetFlag(DCU_UNIT, DCU_FLAG_DATA0_LS_DATA2))
    {
        m_enData0LsData2 = Set;
    }

    if (Set == DCU_GetFlag(DCU_UNIT, DCU_FLAG_DATA0_EQ_DATA2))
    {
        m_enData0EqData2 = Set;
    }

    if (Set == DCU_GetFlag(DCU_UNIT, DCU_FLAG_DATA0_GT_DATA2))
    {
        m_enData0GtData2 = Set;
    }

    if (Set == DCU_GetFlag(DCU_UNIT, DCU_FLAG_DATA0_LS_DATA1))
    {
        m_enData0LsData1 = Set;
    }

    if (Set == DCU_GetFlag(DCU_UNIT, DCU_FLAG_DATA0_EQ_DATA1))
    {
        m_enData0EqData1 = Set;
    }

    if (Set == DCU_GetFlag(DCU_UNIT, DCU_FLAG_DATA0_GT_DATA1))
    {
        m_enData0GtData1 = Set;
    }

    DCU_ClearFlag(DCU_UNIT, (DCU_FLAG_DATA0_LS_DATA2 | DCU_FLAG_DATA0_EQ_DATA2 | \
                             DCU_FLAG_DATA0_GT_DATA2 | DCU_FLAG_DATA0_LS_DATA1 | \
                             DCU_FLAG_DATA0_EQ_DATA1 | DCU_FLAG_DATA0_GT_DATA1));
}

/**
 * @brief  Main function of DCU compare project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_dcu_init_t stcDcuInit;
    en_result_t enTestResult = Ok;
    stc_irq_signin_config_t stcIrqSigninCfg;
    uint8_t au8Data0Val[5] = {0x00U, 0x22U, 0x44U, 0x66U, 0x88};
    uint8_t au8Data1Val[5] = {0x00U, 0x11U, 0x55U, 0x77U, 0x77};
    uint8_t au8Data2Val[5] = {0x00U, 0x11U, 0x55U, 0x55U, 0x00};

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Initialize IO. */
    BSP_IO_Init();

    /* Initialize LED. */
    BSP_LED_Init();

    /* Enable peripheral clock */
    PWC_Fcg0PeriphClockCmd(DCU_FUNCTION_CLK_GATE, Enable);

    /* Initialize DCU */
    DCU_StructInit(&stcDcuInit);
    stcDcuInit.u32Mode = DCU_CMP;
    stcDcuInit.u32IntEn = DCU_INT_ENABLE;
    DCU_Init(DCU_UNIT, &stcDcuInit);
    DCU_IntCmd(DCU_UNIT, DCU_INT_SELECTION, Enable);

    /* Set DCU IRQ */
    stcIrqSigninCfg.enIRQn = DCU_UNIT_INT_IRQn;
    stcIrqSigninCfg.enIntSrc = DCU_UNIT_INT_SRC;
    stcIrqSigninCfg.pfnCallback = &DCU_IrqCallback;
    INTC_IrqSignIn(&stcIrqSigninCfg);
    NVIC_SetPriority(stcIrqSigninCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqSigninCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqSigninCfg.enIRQn);

    /* DATA0 = DATA1  &&  DATA0 = DATA2 */
    DCU_WriteReg8Data1(DCU_UNIT, au8Data1Val[0]);
    DCU_WriteReg8Data2(DCU_UNIT, au8Data2Val[0]);
    DCU_WriteReg8Data0(DCU_UNIT, au8Data0Val[0]);
    if ((Set != m_enData0EqData1) || (Set != m_enData0EqData2))
    {
        enTestResult = Error;
    }

    /* DATA0 > DATA1  &&  DATA0 > DATA2 */
    DCU_WriteReg8Data1(DCU_UNIT, au8Data1Val[1]);
    DCU_WriteReg8Data2(DCU_UNIT, au8Data2Val[1]);
    DCU_WriteReg8Data0(DCU_UNIT, au8Data0Val[1]);
    if ((Set != m_enData0GtData1) || (Set != m_enData0GtData2))
    {
        enTestResult = Error;
    }

    /* DATA0 < DATA1  &&  DATA0 < DATA2 */
    DCU_WriteReg8Data1(DCU_UNIT, au8Data1Val[2]);
    DCU_WriteReg8Data2(DCU_UNIT, au8Data2Val[2]);
    DCU_WriteReg8Data0(DCU_UNIT, au8Data0Val[2]);
    if ((Set != m_enData0LsData1) || (Set != m_enData0LsData2))
    {
        enTestResult = Error;
    }

    DCU_IntCmd(DCU_UNIT, DCU_INT_SELECTION, Disable);

    /* Inside window: DATA2 <= DATA0 <= DATA1 */
    m_enData0LsData1 = Reset;
    m_enData0GtData2 = Reset;
    DCU_SetCmpIntMode(DCU_UNIT, DCU_CMP_WINDOW_INT_INSIDE);
    DCU_WriteReg8Data1(DCU_UNIT, au8Data1Val[3]);
    DCU_WriteReg8Data2(DCU_UNIT, au8Data2Val[3]);
    DCU_WriteReg8Data0(DCU_UNIT, au8Data0Val[3]);
    if (!((Set == m_enData0LsData1) && (Set == m_enData0GtData2)))
    {
        enTestResult = Error;
    }

    /* Outside window: DATA0 < DATA2 or DATA0 > DATA1 */
    m_enData0GtData1 = Reset;
    m_enData0LsData2 = Reset;
    DCU_SetCmpIntMode(DCU_UNIT, DCU_CMP_WINDOW_INT_OUTSIDE);
    DCU_WriteReg8Data1(DCU_UNIT, au8Data1Val[4]);
    DCU_WriteReg8Data2(DCU_UNIT, au8Data2Val[4]);
    DCU_WriteReg8Data0(DCU_UNIT, au8Data0Val[4]);
    if (!((Set == m_enData0GtData1) || (Set == m_enData0LsData2)))
    {
        enTestResult = Error;
    }

    if (Ok == enTestResult)
    {
        BSP_LED_On(LED_BLUE);  /* Test pass && meet the expected */
    }
    else
    {
        BSP_LED_On(LED_RED);  /* Test fail && don't meet the expected */
    }

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
