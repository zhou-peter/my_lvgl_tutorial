/**
 *******************************************************************************
 * @file  lin.c
 * @brief This midware file provides firmware functions to manage the LIN.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-02-28       Hongjh          First version
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
#include "lin.h"

/**
 * @addtogroup HC32F4A0_DDL_Midware
 * @{
 */

/**
 * @defgroup MW_LIN LIN
 * @brief LIN Midware Library
 * @{
 */

#if (MW_LIN_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup LIN_Local_Macros LIN Local Macros
 * @{
 */

/* USART LIN channel */
#define IS_VALID_USART_LIN(x)                                                  \
(   (M4_USART5 == (x)) ||                                                      \
    (M4_USART10 == (x)))

/**
 * @defgroup USART_LIN_Channel_Index USART LIN Channel Index
 * @{
 */
#define USART_LIN_CH5                           (0UL)
#define USART_LIN_CH10                          (1UL)
/**
 * @}
 */

/* BIT operation */
#define BIT(x, pos)                             (((x) >> (pos)) & 0x01U)

/* LIN wakeup/break/sync data definition */
#define LIN_WAKEUP_DATA                         (0x80U)
#define LIN_BREAK_DATA                          (0x00U)
#define LIN_SYNC_DATA                           (0x55U)

/* LIN Timerb capture count/bits definition */
#define LIN_SYNC_CAPTURE_EDGE_CNT               (5U)
#define LIN_SYNC_CAPTURE_BITS                   (8U)

/*  */
/**
 * @defgroup LIN_Frame_PID_Length_Mask LIN frame PID length mask
 * @{
 */
#define LIN_FRAME_PID_LEN_MASK                  (0x30U)
/**
 * @}
 */

/**
 * @defgroup LIN_Delay LIN Delay
 * @{
 */
#define LIN_INTER_FRAME_SPACE                   (10UL) /* Pause (Frame->Frame)   (ca. 10ms) */
#define LIN_FRAME_RESPONSE_SPACE                (2UL)  /* Pause (Header->Data)   (ca.  2ms) */
#define LIN_FRAME_BREAK_DELAY                   (4UL)  /* Pause (Breakfield)     (ca.  4ms) */
#define LIN_FRAME_INTER_BYTE_SPACE              (1UL)  /* Pause (Data->Data)     (ca.  1ms) */
/**
 * @}
 */

/**
 * @defgroup USART_LIN_Interrupt_Source_Number USART LIN Interrupt Source Number
 * @{
 */
#define LIN_RX_INT_SRC(Idx)                                                    \
(   (en_int_src_t)((uint32_t)INT_USART5_RI + Idx * ((uint32_t)(INT_USART10_RI - INT_USART5_RI))))

#define LIN_RXERR_INT_SRC(Idx)                                                 \
(   (en_int_src_t)((uint32_t)INT_USART5_EI + Idx * ((uint32_t)(INT_USART10_EI - INT_USART5_EI))))

#define LIN_BRKWKP_INT_SRC(Idx)                                                 \
(   (en_int_src_t)((uint32_t)INT_USART5_BRKWKPI + Idx * ((uint32_t)(INT_USART10_BRKWKPI - INT_USART5_BRKWKPI))))

#define LIN_TX_INT_SRC(Idx)                                                    \
(   (en_int_src_t)((uint32_t)INT_USART5_TI + Idx * ((uint32_t)(INT_USART10_TI - INT_USART5_TI))))

/**
 * @}
 */


/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void LIN_SendBreak(stc_lin_hanlde_t *pstcLinHandle);
static void LIN_SendChar(stc_lin_hanlde_t *pstcLinHandle, uint8_t u8Char);
static en_result_t LIN_MASTER_SendFrameHeader(stc_lin_hanlde_t *pstcLinHandle,
                                                    stc_lin_frame_t *pstcFrame);
static uint8_t LIN_CalcChecksum(uint8_t u8PID,
                                    const uint8_t au8Data[],
                                    uint8_t u8Len);
static uint8_t LIN_CalcPIDParity(uint8_t u8PID);
static uint8_t LIN_GetFrameDataLenbyPID(uint8_t u8PID);
static void UsartRxIrqCallback(stc_lin_hanlde_t *pstcLinHandle);
static void UsartErrIrqCallback(stc_lin_hanlde_t *pstcLinHandle);
static void UsartBreakWakeupIrqCallback(stc_lin_hanlde_t *pstcLinHandle);
static void Usart5RxIrqCallback(void);
static void Usart10RxIrqCallback(void);
static void Usart5ErrIrqCallback(void);
static void Usart10ErrIrqCallback(void);
static void Usart5BreakWakeupIrqCallback(void);
static void Usart10BreakWakeupIrqCallback(void);
static void InstalIrqHandler(const stc_irq_signin_config_t *pstcConfig,
                                    uint32_t u32Priority);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_lin_hanlde_t *m_apstcLinHandle[2] = {NULL, NULL};

static func_ptr_t m_apfnLinRxIrqCb[2] = {Usart5RxIrqCallback,
                                         Usart10RxIrqCallback};
static func_ptr_t m_apfnLinRxErrIrqCb[2] = {Usart5ErrIrqCallback,
                                            Usart10ErrIrqCallback};
static func_ptr_t m_apfnLinBreakWakeupIrqCb[2] = {Usart5BreakWakeupIrqCallback,
                                                  Usart10BreakWakeupIrqCallback};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup LIN_Global_Functions LIN Global Functions
 * @{
 */

/**
 * @brief  Initialize LIN master function.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_result_t LIN_Init(stc_lin_hanlde_t *pstcLinHandle)
{
    uint32_t u32FcgPeriph;
    uint32_t u32UsartLinIndex;
    stc_irq_signin_config_t stcIrqSigninCfg;
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pstcLinHandle)
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

        if (M4_USART5 == pstcLinHandle->USARTx)
        {
            u32FcgPeriph = PWC_FCG3_USART5;
            u32UsartLinIndex = USART_LIN_CH5;
        }
        else
        {
            u32FcgPeriph = PWC_FCG3_USART10;
            u32UsartLinIndex = USART_LIN_CH10;
        }
        m_apstcLinHandle[u32UsartLinIndex] = pstcLinHandle;

        /* Configure USART RX/TX pin. */
        GPIO_SetFunc(pstcLinHandle->stcPinCfg.u16RxPort, pstcLinHandle->stcPinCfg.u16RxPin, pstcLinHandle->stcPinCfg.u16RxPinFunc, PIN_SUBFUNC_DISABLE);
        GPIO_SetFunc(pstcLinHandle->stcPinCfg.u16TxPort, pstcLinHandle->stcPinCfg.u16TxPin, pstcLinHandle->stcPinCfg.u16TxPinFunc, PIN_SUBFUNC_DISABLE);

        /* Enable peripheral clock */
        PWC_Fcg3PeriphClockCmd(u32FcgPeriph, Enable);

        /* Initialize LIN */
        USART_LinInit(pstcLinHandle->USARTx, &pstcLinHandle->stcLinInit);

        /* Register USART RX IRQ handler && configure NVIC. */
        stcIrqSigninCfg.enIntSrc = LIN_RX_INT_SRC(u32UsartLinIndex);
        stcIrqSigninCfg.pfnCallback = m_apfnLinRxIrqCb[u32UsartLinIndex];
        stcIrqSigninCfg.enIRQn = pstcLinHandle->stcIrqnCfg.RxIntIRQn;
        InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_DEFAULT);

        /* Register USART error IRQ handler && configure NVIC. */
        stcIrqSigninCfg.enIntSrc = LIN_RXERR_INT_SRC(u32UsartLinIndex);
        stcIrqSigninCfg.pfnCallback = m_apfnLinRxErrIrqCb[u32UsartLinIndex];
        stcIrqSigninCfg.enIRQn = pstcLinHandle->stcIrqnCfg.RxErrIntIRQn;
        InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_DEFAULT);

        USART_FuncCmd(pstcLinHandle->USARTx, (USART_RX | USART_INT_RX | USART_TX), Enable);

        if (LIN_SLAVE == pstcLinHandle->u32LinMode)
        {
            /* Register USART LIN break detection interrupt IRQ handler && configure NVIC. */
            stcIrqSigninCfg.pfnCallback = m_apfnLinBreakWakeupIrqCb[u32UsartLinIndex];
            stcIrqSigninCfg.enIntSrc = LIN_BRKWKP_INT_SRC(u32UsartLinIndex);
            stcIrqSigninCfg.enIRQn = pstcLinHandle->stcIrqnCfg.BreakWkupIntIRQn;
            InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_DEFAULT);

            USART_LinFuncCmd(pstcLinHandle->USARTx, USART_LIN_INT_BREAK, Enable);

            if (LinStateSleep == pstcLinHandle->enLinState)
            {
                LIN_Sleep(pstcLinHandle);
            }
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Master send frame.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 */
en_result_t LIN_MASTER_SendFrame(stc_lin_hanlde_t *pstcLinHandle,
                                        stc_lin_frame_t *pstcFrame)
{
    uint8_t u8Checksum;
    en_result_t enRet = Ok;

    if ((NULL != pstcFrame) && (NULL != pstcLinHandle))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

        enRet = LIN_MASTER_SendFrameHeader(pstcLinHandle, pstcFrame);
        if (Ok == enRet)
        {
            DDL_Delay1ms(LIN_FRAME_RESPONSE_SPACE);

            /* Send data */
            for (pstcFrame->u8XferCnt = 0U; pstcFrame->u8XferCnt < pstcFrame->u8Length ; pstcFrame->u8XferCnt++)
            {
                LIN_SendChar(pstcLinHandle, pstcFrame->au8Data[pstcFrame->u8XferCnt]);
                DDL_Delay1ms(LIN_FRAME_INTER_BYTE_SPACE);
            }

            /* Calculate Checksum */
            u8Checksum = LIN_CalcChecksum(pstcFrame->u8PID, pstcFrame->au8Data, pstcFrame->u8Length);

            LIN_SendChar(pstcLinHandle, u8Checksum);
            DDL_Delay1ms(LIN_INTER_FRAME_SPACE);
        }
    }
    return enRet;
}

/**
 * @brief  LIN master receive frame(blocking mode).
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @param  [in] i32Timeout              Timeout ms
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 */
en_result_t LIN_MASTER_RecFrame(stc_lin_hanlde_t *pstcLinHandle,
                                    stc_lin_frame_t *pstcFrame,
                                    int32_t i32Timeout)
{
    uint8_t i;
    uint8_t u8Checksum;
    __IO uint32_t u32Cycle;
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != pstcFrame) && (NULL != pstcLinHandle))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

        /* Clear frame data */
        pstcFrame->u8XferCnt = 0U;
        pstcFrame->u8Checksum = 0U;
        pstcFrame->u8Error = 0U;
        for (i = 0U; i < 8U; i++)
        {
            pstcFrame->au8Data[i] = 0U;
        }

        enRet = LIN_MASTER_SendFrameHeader(pstcLinHandle, pstcFrame);
        if (Ok == enRet)
        {
            u32Cycle = (i32Timeout < 0) ? 0UL : (uint32_t)i32Timeout * (SystemCoreClock / 10000UL);  /* i32Timeout * 1ms */

            /* Wait checksum */
            while (u32Cycle || (i32Timeout < 0))
            {
                u32Cycle--;
                if (LinFrameStateChecksum == pstcFrame->u8State)
                {
                    break;
                }
            }

            if ((0ul == u32Cycle) && (i32Timeout >= 0))
            {
                enRet = ErrorTimeout;
            }
            else
            {
                u8Checksum = LIN_CalcChecksum(pstcFrame->u8PID, pstcFrame->au8Data, pstcFrame->u8Length);

                if (u8Checksum != pstcFrame->u8Checksum)
                {
                    enRet = Error;
                }

                DDL_Delay1ms(LIN_INTER_FRAME_SPACE);
            }
        }
    }

    return enRet;
}

/**
 * @brief  LIN slave receive frame header(blocking mode).
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @param  [in] i32Timeout              Timeout duration (unit: ms)
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 */
en_result_t LIN_SLAVE_RecFrameHeader(stc_lin_hanlde_t *pstcLinHandle,
                                            stc_lin_frame_t *pstcFrame,
                                            int32_t i32Timeout)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint32_t u32Cyc = (i32Timeout < 0) ? 0UL : ((uint32_t)i32Timeout) * (SystemCoreClock / 10000UL);  /* i32Timeout * 1ms */

    if ((NULL != pstcFrame) && (NULL != pstcLinHandle))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

        pstcFrame->u8PID = 0U;
        pstcFrame->u8Error = 0U;
        pstcFrame->u8Length = 0U;
        pstcFrame->u8XferCnt = 0U;
        pstcFrame->u8Checksum = 0U;
        pstcFrame->u8State = LinFrameStateIdle;

        pstcLinHandle->pstcFrame = pstcFrame;

        /* Data state */
        USART_FuncCmd(pstcLinHandle->USARTx, USART_TX | USART_RX | USART_INT_RX, Disable);
        USART_FuncCmd(pstcLinHandle->USARTx, USART_RX | USART_INT_RX, Enable);
        USART_LinFuncCmd(pstcLinHandle->USARTx, USART_LIN_INT_BREAK, Enable);

        while (u32Cyc || (i32Timeout < 0))
        {
            u32Cyc--;
            if (pstcFrame->u8State >= LinFrameStatePID)
            {
                break;
            }
        }

        if ((0UL == u32Cyc) && (i32Timeout >= 0))
        {
            enRet = ErrorTimeout;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  LIN slave receive frame response data(blocking mode).
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @param  [in] i32Timeout              Timeout duration (unit: ms)
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 * @note   Firstly call LIN_SLAVE_RecFrameHeader(), and then call this function.
 */
en_result_t LIN_SLAVE_RecFrameResponse(const stc_lin_hanlde_t *pstcLinHandle,
                                                int32_t i32Timeout)
{
    uint8_t u8Checksum = 0U;
    stc_lin_frame_t *pstcFrame;
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint32_t u32Cyc = (i32Timeout < 0) ? 0UL : ((uint32_t)i32Timeout) * (SystemCoreClock / 10000UL);  /* i32Timeout * 1ms */

    if (NULL != pstcLinHandle)
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));
        DDL_ASSERT(NULL != pstcLinHandle->pstcFrame);
        DDL_ASSERT(LinFrameStatePID == pstcLinHandle->pstcFrame->u8State);

        pstcFrame = pstcLinHandle->pstcFrame;
        while (u32Cyc || (i32Timeout < 0))
        {
            u32Cyc--;

            if (LinFrameStateChecksum == pstcFrame->u8State)
            {
                u8Checksum = LIN_CalcChecksum(pstcFrame->u8PID, pstcFrame->au8Data, pstcFrame->u8Length);
                if (u8Checksum == pstcFrame->u8Checksum)
                {
                    pstcFrame->u8State = LinFrameStateIdle;
                }
                else
                {
                    enRet = Error;
                    pstcFrame->u8Error = LinErrChecksum;
                }

                break;
            }
        }

        if ((0UL == u32Cyc) && (i32Timeout >= 0))
        {
            enRet = ErrorTimeout;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  LIN slave send frame response data.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 * @note   Firstly call LIN_SLAVE_RecFrameHeader(), and then call this function.
 */
en_result_t LIN_SLAVE_SendFrameResponse(stc_lin_hanlde_t *pstcLinHandle)
{
    uint8_t u8Checksum;
    stc_lin_frame_t *pstcFrame;
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != pstcLinHandle) &&
        (NULL != pstcLinHandle->pstcFrame))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));
        DDL_ASSERT(LinFrameStatePID == pstcLinHandle->pstcFrame->u8State);

        pstcFrame = pstcLinHandle->pstcFrame;
        DDL_Delay1ms(LIN_FRAME_RESPONSE_SPACE);

        /* Send data */
        for (pstcFrame->u8XferCnt = 0u; pstcFrame->u8XferCnt < pstcFrame->u8Length ; pstcFrame->u8XferCnt++)
        {
            LIN_SendChar(pstcLinHandle, pstcFrame->au8Data[pstcFrame->u8XferCnt]);
            DDL_Delay1ms(LIN_FRAME_INTER_BYTE_SPACE);
        }

        /* Calculate Checksum */
        u8Checksum = LIN_CalcChecksum(pstcFrame->u8PID, pstcFrame->au8Data, pstcFrame->u8Length);

        LIN_SendChar(pstcLinHandle, u8Checksum);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set LIN state.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] enState                 LIN state
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_result_t LIN_SetState(stc_lin_hanlde_t *pstcLinHandle,
                            en_lin_state_t enState)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcLinHandle != NULL)
    {
        pstcLinHandle->enLinState = enState;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Wait wakeup signal.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_lin_state_t LIN_GetState(const stc_lin_hanlde_t *pstcLinHandle)
{
    DDL_ASSERT(pstcLinHandle != NULL);

    return pstcLinHandle->enLinState;
}

/**
 * @brief  LIN sleep.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_result_t LIN_Sleep(stc_lin_hanlde_t *pstcLinHandle)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pstcLinHandle)
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

        USART_LinFuncCmd(pstcLinHandle->USARTx,
                         (USART_LIN_WKUP | USART_LIN_INT_WKUP),
                         Enable);
        pstcLinHandle->enLinState = LinStateSleep;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  LIN send wakeup signal.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL
 */
en_result_t LIN_SendWakeupSignal(stc_lin_hanlde_t *pstcLinHandle)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pstcLinHandle)
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

        while (!USART_GetFlag(pstcLinHandle->USARTx, USART_FLAG_TXE))
        {
            ;
        }

        USART_SendData(pstcLinHandle->USARTx, LIN_WAKEUP_DATA);

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Master send frame header field.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] pstcFrame               Pointer to a @ref stc_lin_frame_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize success
 *           - ErrorInvalidParameter: pstcLinHandle == NULL or pstcFrame == NULL
 */
static en_result_t LIN_MASTER_SendFrameHeader(stc_lin_hanlde_t *pstcLinHandle,
                                                    stc_lin_frame_t *pstcFrame)
{
    uint8_t u8PID;
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != pstcFrame) && (NULL != pstcLinHandle))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));
        DDL_ASSERT((pstcFrame->u8PID != 0xFFU) && (pstcFrame->u8PID != 0x00U));

        pstcLinHandle->pstcFrame = pstcFrame;

        /* Idle state */
        if (USART_GetFlag(pstcLinHandle->USARTx, USART_FLAG_PE | USART_FLAG_FE | USART_FLAG_ORE))
        {
            enRet = Error;
        }
        else
        {
            /* Send break field */
            LIN_SendBreak(pstcLinHandle);
            pstcFrame->u8State = LinFrameStateBreak;
            DDL_Delay1ms(LIN_FRAME_BREAK_DELAY);

            /* Send sync field */
            LIN_SendChar(pstcLinHandle, LIN_SYNC_DATA);
            pstcFrame->u8State = LinFrameStateSync;
            DDL_Delay1ms(LIN_FRAME_INTER_BYTE_SPACE);

            /* Calculate PID */
            u8PID = LIN_CalcPIDParity(pstcFrame->u8PID);

            /* Send PID */
            LIN_SendChar(pstcLinHandle, u8PID);
            pstcFrame->u8State = LinFrameStatePID;

            pstcFrame->u8Length = LIN_GetFrameDataLenbyPID(pstcFrame->u8PID);
            enRet = Ok;
        }
    }

    return enRet;
}

/**
 * @brief  Master send break field.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @retval None
 */
static void LIN_SendBreak(stc_lin_hanlde_t *pstcLinHandle)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

    while (!USART_GetFlag(pstcLinHandle->USARTx, USART_FLAG_TXE))
    {
        ;
    }

    USART_RequestBreakSending(pstcLinHandle->USARTx);

    if (USART_LIN_SEND_BREAK_MODE_TDR == USART_GetLinBreakMode(pstcLinHandle->USARTx))
    {
        USART_SendData(pstcLinHandle->USARTx, LIN_BREAK_DATA);
    }

    while (USART_GetRequestBreakStatus(pstcLinHandle->USARTx))
    {
        ;
    }
}

/**
 * @brief  Lin send character.
 * @param  [in] pstcLinHandle           Pointer to a @ref stc_lin_hanlde_t structure
 * @param  [in] u8Char                  Character
 * @retval None
 */
static void LIN_SendChar(stc_lin_hanlde_t *pstcLinHandle, uint8_t u8Char)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

    while (!USART_GetFlag(pstcLinHandle->USARTx, USART_FLAG_TXE))
    {
        ;
    }

    USART_SendData(pstcLinHandle->USARTx, (uint16_t)u8Char);
}

/**
 * @brief  Calculate PID && data field checksum.
 * @param  [in] u8PID                   PID number
 * @param  [in] au8Data                 Data buffer
 * @param  [in] u8Len                   Data length
 * @retval Checksum
 */
static uint8_t LIN_CalcChecksum(uint8_t u8PID,
                                    const uint8_t au8Data[],
                                    uint8_t u8Len)
{
    uint8_t i;
    uint16_t u16Checksum;

    if ((u8PID == 0x3CU) || (u8PID == 0x3DU))  /* 0x3C 0x3D Classic Checksum */
    {
        u16Checksum = 0U;
    }
    else /* Enhanced Checksum */
    {
        u16Checksum  = (uint16_t)u8PID;
    }

    for (i = 0U; i < u8Len; i++)
    {
        u16Checksum += au8Data[i];

        if (u16Checksum > 0xFFU)      /* Carry bit */
        {
            u16Checksum -= 0xFFU;
        }
    }

    return (uint8_t)((uint16_t)(~u16Checksum));  /* reversed */
}

/**
 * @brief  Calculate PID field parity.
 * @param  [in] u8PID                   PID number
 * @retval Parity
 */
static uint8_t LIN_CalcPIDParity(uint8_t u8PID)
{
    uint8_t u8P0;
    uint8_t u8P1;
    uint8_t u8Parity;

    u8Parity = u8PID;
    u8P0 = (BIT(u8Parity,0U) ^ BIT(u8Parity,1U) ^ BIT(u8Parity,2U) ^ BIT(u8Parity,4U)) << 6U;
    u8P1 = (!(BIT(u8Parity,1U) ^ BIT(u8Parity,3U) ^ BIT(u8Parity,4U) ^ BIT(u8Parity,5U))) << 7U;
    u8Parity |= (u8P0 | u8P1);

    return u8Parity;
}

/**
 * @brief  Get LIN frame data length by PID.
 * @param  [in] u8PID                   Frame PID
 * @retval Frame data length
 */
static uint8_t LIN_GetFrameDataLenbyPID(uint8_t u8PID)
{
    uint8_t u8DataLen;

    /*------------------------------------+
    | ID5 | ID4 | Frame Data Filed Length |
    |-------------------------------------|
    |  0  |  0  |            2            |
    |-----|-------------------------------|
    |  0  |  1  |            2            |
    |-----|-------------------------------|
    |  1  |  0  |            4            |
    |-----|-------------------------------|
    |  1  |  1  |            8            |
    +-------------------------------------*/

    switch (u8PID & LIN_FRAME_PID_LEN_MASK)
    {
        case LIN_FRAME_DATA_BYTES_4:
            u8DataLen = 4U;
            break;
        case LIN_FRAME_DATA_BYTES_8:
            u8DataLen = 8U;
            break;
        default:
            u8DataLen = 2U;
            break;
    }

    return u8DataLen;
}

/**
 * @brief  USART RX IRQ callback
 * @param  None
 * @retval None
 */
static void UsartRxIrqCallback(stc_lin_hanlde_t *pstcLinHandle)
{
    uint8_t u8Data;
    uint32_t u32Baudrate;

    /* Check paramter */
    DDL_ASSERT(NULL != pstcLinHandle);
    DDL_ASSERT(NULL != pstcLinHandle->pstcFrame);
    DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

    u8Data = (uint8_t)USART_RecData(pstcLinHandle->USARTx);

    switch (pstcLinHandle->pstcFrame->u8State)
    {
        case LinFrameStateBreak:
            u32Baudrate = USART_GetLinMeasureBaudrate(pstcLinHandle->USARTx);
            USART_SetBaudrate(pstcLinHandle->USARTx, u32Baudrate, NULL);
            pstcLinHandle->pstcFrame->u8State = LinFrameStateSync;
            break;
        case LinFrameStateSync:
            if (u8Data == LIN_CalcPIDParity(u8Data & 0x3FU))
            {
                pstcLinHandle->pstcFrame->u8PID = u8Data;
                pstcLinHandle->pstcFrame->u8Length = LIN_GetFrameDataLenbyPID(u8Data);
                pstcLinHandle->pstcFrame->u8State = LinFrameStatePID;
            }
            else
            {
                pstcLinHandle->pstcFrame->u8Error = LinErrPID;
            }
            break;
        case LinFrameStatePID:
            if (pstcLinHandle->pstcFrame->u8XferCnt < pstcLinHandle->pstcFrame->u8Length)
            {
                pstcLinHandle->pstcFrame->au8Data[pstcLinHandle->pstcFrame->u8XferCnt++] = u8Data;
            }

            if (pstcLinHandle->pstcFrame->u8XferCnt == pstcLinHandle->pstcFrame->u8Length)
            {
                pstcLinHandle->pstcFrame->u8State = LinFrameStateData;
            }
            break;
        case LinFrameStateData:
            pstcLinHandle->pstcFrame->u8Checksum = u8Data;
            pstcLinHandle->pstcFrame->u8State = LinFrameStateChecksum;
            break;
        default:
            break;
    }
}

/**
 * @brief  USART error IRQ callback.
 * @param  None
 * @retval None
 */
static void UsartErrIrqCallback(stc_lin_hanlde_t *pstcLinHandle)
{
    uint32_t u32Baudrate;

    /* Check paramter */
    DDL_ASSERT(NULL != pstcLinHandle);
    DDL_ASSERT(NULL != pstcLinHandle->pstcFrame);
    DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

    if (LinFrameStateBreak == pstcLinHandle->pstcFrame->u8State)
    {
        u32Baudrate = USART_GetLinMeasureBaudrate(pstcLinHandle->USARTx);
        USART_SetBaudrate(pstcLinHandle->USARTx, u32Baudrate, NULL);
        pstcLinHandle->pstcFrame->u8State = LinFrameStateSync;
    }

    USART_ClearFlag(pstcLinHandle->USARTx, (USART_CLEAR_FLAG_PE | USART_CLEAR_FLAG_FE | USART_CLEAR_FLAG_ORE));
}

/**
 * @brief  USART LIN break detection && wakeup IRQ callback.
 * @param  None
 * @retval None
 */
static void UsartBreakWakeupIrqCallback(stc_lin_hanlde_t *pstcLinHandle)
{
    /* Check paramter */
    DDL_ASSERT(NULL != pstcLinHandle);
    DDL_ASSERT(IS_VALID_USART_LIN(pstcLinHandle->USARTx));

    if (LIN_SLAVE == pstcLinHandle->u32LinMode)
    {
        if (LinStateSleep == pstcLinHandle->enLinState)
        {
            if (USART_GetFlag(pstcLinHandle->USARTx, USART_FLAG_WKUP))
            {
                USART_LinFuncCmd(pstcLinHandle->USARTx, USART_LIN_WKUP, Disable);
                pstcLinHandle->enLinState = LinStateWakeup;
            }
        }
        else
        {
            if (USART_GetFlag(pstcLinHandle->USARTx, USART_FLAG_LBD))
            {
                USART_LinFuncCmd(pstcLinHandle->USARTx, USART_LIN_INT_BREAK, Disable);
                pstcLinHandle->pstcFrame->u8State = LinFrameStateBreak;
            }
        }
    }

    USART_ClearFlag(pstcLinHandle->USARTx, (USART_CLEAR_FLAG_LBD | \
                                            USART_CLEAR_FLAG_WKUP | \
                                            USART_CLEAR_FLAG_FE | \
                                            USART_CLEAR_FLAG_ORE));
}

/**
 * @brief  USART channel 5 RX IRQ callback
 * @param  None
 * @retval None
 */
static void Usart5RxIrqCallback(void)
{
    UsartRxIrqCallback(m_apstcLinHandle[USART_LIN_CH5]);
}

/**
 * @brief  USART channel 10 RX IRQ callback
 * @param  None
 * @retval None
 */
static void Usart10RxIrqCallback(void)
{
    UsartRxIrqCallback(m_apstcLinHandle[USART_LIN_CH10]);
}

/**
 * @brief  USART channel 5 error IRQ callback.
 * @param  None
 * @retval None
 */
static void Usart5ErrIrqCallback(void)
{
    UsartErrIrqCallback(m_apstcLinHandle[USART_LIN_CH5]);
}

/**
 * @brief  USART channel 10 error IRQ callback.
 * @param  None
 * @retval None
 */
static void Usart10ErrIrqCallback(void)
{
    UsartErrIrqCallback(m_apstcLinHandle[USART_LIN_CH10]);
}

/**
 * @brief  USART channel 5 LIN break detection && wakeup IRQ callback.
 * @param  None
 * @retval None
 */
static void Usart5BreakWakeupIrqCallback(void)
{
    UsartBreakWakeupIrqCallback(m_apstcLinHandle[USART_LIN_CH5]);
}

/**
 * @brief  USART channel 10 LIN break detection && wakeup IRQ callback.
 * @param  None
 * @retval None
 */
static void Usart10BreakWakeupIrqCallback(void)
{
    UsartBreakWakeupIrqCallback(m_apstcLinHandle[USART_LIN_CH10]);
}

/**
 * @brief  Instal IRQ handler.
 * @param  [in] pstcConfig      Pointer to struct @ref stc_irq_signin_config_t
 * @param  [in] Priority        Interrupt priority
 * @retval None
 */
static void InstalIrqHandler(const stc_irq_signin_config_t *pstcConfig,
                                    uint32_t u32Priority)
{
    if (NULL != pstcConfig)
    {
        INTC_IrqSignIn(pstcConfig);
        NVIC_ClearPendingIRQ(pstcConfig->enIRQn);
        NVIC_SetPriority(pstcConfig->enIRQn, u32Priority);
        NVIC_EnableIRQ(pstcConfig->enIRQn);
    }
}

/**
 * @}
 */

#endif /* MW_LIN_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/