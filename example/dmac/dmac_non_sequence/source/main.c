 /*******************************************************************************
 * @file  dmac/dmac_non_sequence/source/main.c
 * @brief This example demonstrates TIMERA base count function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-04-09       Zhangxl         First version
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
 * @addtogroup DMAC_Non_Sequence
 * @{
 */
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* DMAC */
#define DMA_UNIT        (M4_DMA1)
#define DMA_CH          (DMA_CH3)
#define DMA_MX_CH       (DMA_MX_CH3)
#define DMA_TC          (4UL)
#define DMA_BC          (3UL)
#define DMA_SRC_OFFSET  (3UL)
#define DMA_SRC_NSEQCNT (2UL)
#define DMA_DW          (DMA_DATAWIDTH_32BIT)
#define DMA_INT_SRC     (INT_DMA1_TC3)
#define DMA_IRQn        (Int000_IRQn)


/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint8_t u8CmpRet = 1u;
en_flag_status_t u8DmaTcEnd = Reset;
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static const uint32_t u32SrcBuf[22] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                       11, 12, 13, 14, 15, 16, 17, 18,
                                       19, 20, 21, 22};
static uint32_t u32DestBuf[22] = {0};
static uint32_t u32ExpectDestBufData[22] = {1, 2, 5, 6, 9, 10, 13, 14, 17, 18, 21,
                                           22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  DMA basical and non-sequence function init
 * @param  None
 * @retval en_result_t
 */
en_result_t DmaInit(void)
{
    stc_dma_init_t stcDmaInit;
    stc_dma_nonseq_init_t stcDmaNonSeqInit;

    DMA_SetTrigSrc(DMA_UNIT, DMA_CH, EVT_AOS_STRG);

    DMA_StructInit(&stcDmaInit);

    stcDmaInit.u32IntEn     = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = DMA_BC;
    stcDmaInit.u32TransCnt  = DMA_TC;
    stcDmaInit.u32DataWidth = DMA_DW;
    stcDmaInit.u32DestAddr  = (uint32_t)(&u32DestBuf[0]);
    stcDmaInit.u32SrcAddr   = (uint32_t)(&u32SrcBuf[0]);
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_INC;

    if (Ok != DMA_Init(DMA_UNIT, DMA_CH, &stcDmaInit))
    {
        while (1)
        {}
    }

    DMA_NonSeqStructInit(&stcDmaNonSeqInit);

    stcDmaNonSeqInit.u32SrcNonSeqEn = DMA_SRC_NS_ENABLE;
    stcDmaNonSeqInit.u32DestNonSeqEn= DMA_DEST_NS_DISABLE;
    stcDmaNonSeqInit.u32SrcNonSeqOfs= DMA_SRC_OFFSET;
    stcDmaNonSeqInit.u32SrcNonSeqCnt= DMA_SRC_NSEQCNT;

    return (DMA_NonSeqInit(DMA_UNIT, DMA_CH, &stcDmaNonSeqInit));
}

/**
 * @brief  DMA ch.3 transfer complete IRQ callback
 * @param  None
 * @retval None
 */
void DMA1_CH3_TransEnd_IrqCallback(void)
{
    u8DmaTcEnd = Set;
    DMA_ClearTransIntStatus(DMA_UNIT, DMA_TC_INT3);
}

/**
 * @brief  DMA basical function interrupt init
 * @param  None
 * @retval None
 */
void DmaIntInit(void)
{
    stc_irq_signin_config_t stcIrqSignConfig;

    stcIrqSignConfig.enIntSrc   = DMA_INT_SRC;
    stcIrqSignConfig.enIRQn     = DMA_IRQn;
    stcIrqSignConfig.pfnCallback= &DMA1_CH3_TransEnd_IrqCallback;

    INTC_IrqSignIn(&stcIrqSignConfig);

    DMA_ClearTransIntStatus(DMA_UNIT, DMA_TC_INT3);

    /* NVIC setting */
    NVIC_ClearPendingIRQ(DMA_IRQn);
    NVIC_SetPriority(DMA_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(DMA_IRQn);
}

/**
 * @brief  Main function of DMAC project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    BSP_IO_Init();
    BSP_LED_Init();

    /* DMA/AOS FCG enable */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_DMA1 | PWC_FCG0_PTDIS, Enable);

    /* Config DMA */
    if (Ok != DmaInit()) {
        while (1)
        {}
    }

    /* DMA interrupt config */
    DmaIntInit();

    /* DMA moudle enable */
    DMA_Cmd(DMA_UNIT, Enable);

    /* DMA channel enable */
    DMA_ChannelCmd(DMA_UNIT, DMA_CH, Enable);
    //DMA_MxChannelCmd(DMA_UNIT, DMA_MX_CH, Enable);

    /* 1st trigger for DMA */
    AOS_SW_Trigger();

    while (Reset == u8DmaTcEnd)
    {
        AOS_SW_Trigger();
    }
    if((uint8_t)memcmp(u32DestBuf, u32ExpectDestBufData, sizeof(u32DestBuf)))
    {
        BSP_LED_On(LED_RED);    /* Don't meet the expected */
    }
    else
    {
        BSP_LED_On(LED_BLUE);    /* Meet the expected */
    }

    while(1)
    {
        ;
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