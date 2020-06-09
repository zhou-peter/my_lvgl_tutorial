/**
 *******************************************************************************
 * @file  hc32f4a0_crc.c
 * @brief This file provides firmware functions to manage the Cyclic Redundancy
 *        Check(CRC).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-05-27       Heqb            First version
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
#include "hc32f4a0_crc.h"
#include "hc32f4a0_utility.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_CRC CRC
 * @brief Cyclic Redundancy Check Driver Library
 * @{
 */

#if (DDL_CRC_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup CRC_Local_Macros CRC Local Macros
 * @{
 */

/**
 * @defgroup CRC_Check_Parameters_Validity CRC check parameters validity
 * @{
 */
#define IS_CRC_PROCOTOL(x)                                                     \
(   ((x) == CRC_CRC16)                      ||                                 \
    ((x) == CRC_CRC32))

#define IS_CRC_BIT_WIDTH(x)                                                    \
(   ((x) == CRC_BW_8)                       ||                                 \
    ((x) == CRC_BW_16)                      ||                                 \
    ((x) == CRC_BW_32))

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
/** 
 * @defgroup CRC_Local_Functions CRC Local Functions
 * @{
 */
static void CRC_8BitWrite(const uint8_t au8Data[], uint32_t u32Length);
static void CRC_16BitWrite(const uint16_t au16Data[], uint32_t u32Length);
static void CRC_32BitWrite(const uint32_t au32Data[], uint32_t u32Length);
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/** 
 * @defgroup CRC_Global_Functions CRC Global Functions
 * @{
 */

/**
 * @brief  CRC16 calculation.
 * @param  [in] u32CrcProtocol        CRC protocol control bit.
 *                                    This parameter can be a value of @ref CRC_Protocol_Control_Bit
 * @param  [in] pvData                Pointer to the buffer containing the data to be computed.
 * @param  [in] u32InitVal            Initialize the CRC calculation. 
 * @param  [in] u32Length             The length(countted in bytes or half word or word, depending on
 *                                    the bit width) of the data to be computed.
 * @param  [in] u8BitWidth            Bit width of the data.
 *   @arg  CRC_BW_8:                  8  Bit.
 *   @arg  CRC_BW_16:                 16 Bit.
 *   @arg  CRC_BW_32:                 32 Bit.
 * @retval CRC checksum.
 */
uint32_t CRC_Calculate(uint32_t u32CrcProtocol,
                       const void *pvData,
                       uint32_t u32InitVal,
                       uint32_t u32Length,
                       uint8_t u8BitWidth)
{
    uint32_t u32CheckSum = 0u;

    if ((pvData != NULL) && (u32Length != 0u))
    {
        DDL_ASSERT(IS_CRC_PROCOTOL(u32CrcProtocol));
        DDL_ASSERT(IS_CRC_BIT_WIDTH(u8BitWidth));
        /* Set operation mode (CRC16 or CRC32) */
        WRITE_REG32(M4_CRC->CR, u32CrcProtocol);
        /* Set initial value */
        if (u32CrcProtocol == CRC_CRC32)
        {
            WRITE_REG32(M4_CRC->RESLT, u32InitVal);
        }
        else
        {
            WRITE_REG16(M4_CRC->RESLT, u32InitVal);
        }
        /* Write data */
        if (u8BitWidth == CRC_BW_8)
        {
            CRC_8BitWrite((const void *)pvData, u32Length);
        }
        else if (u8BitWidth == CRC_BW_16)
        {
            CRC_16BitWrite((const void *)pvData, u32Length);
        }
        else
        {
            CRC_32BitWrite((const void *)pvData, u32Length);
        }
        /* Get checksum */
        if (u32CrcProtocol == CRC_CRC32)
        {
            u32CheckSum = READ_REG32(M4_CRC->RESLT);
        }
        else
        {
            u32CheckSum = READ_REG16(M4_CRC->RESLT);
        }

    }

      return u32CheckSum;
}

/**
 * @brief  CRC check.
 * @param  [in] u32CrcProtocol        CRC protocol control bit.
 *                                    This parameter can be a value of @ref CRC_Protocol_Control_Bit
 * @param  [in] u32CheckSum           The checksum of the data pointed by pointer pvData.
 * @param  [in] pvData                Pointer to the buffer containing the data to be checked.
 * @param  [in] u32InitVal            Initialize the CRC calculation.    
 * @param  [in] u32Length             The length(countted in bytes or half word or word, depending on
 *                                    the bit width) of the data to be computed.
 * @param  [in] u8BitWidth            Bit width of the data.
 *   @arg  CRC_BW_8:                  8  Bit.
 *   @arg  CRC_BW_16:                 16 Bit.
 *   @arg  CRC_BW_32:                 32 Bit.
 * @retval A en_flag_status_t value.
 *   @arg  Set:                    CRC checks successfully.
 *   @arg  Reset:                  CRC checks unsuccessfully.
 */
en_flag_status_t CRC_Check(uint32_t u32CrcProtocol,
                           uint32_t u32CheckSum,
                           const void *pvData,
                           uint32_t u32InitVal,
                           uint32_t u32Length,
                           uint8_t u8BitWidth)
{
    en_flag_status_t enFlag = Reset;
    DDL_ASSERT(IS_CRC_PROCOTOL(u32CrcProtocol));
    DDL_ASSERT(IS_CRC_BIT_WIDTH(u8BitWidth));
    if ((pvData != NULL) && (u32Length != 0u))
    {
        /* Set operation mode (CRC16 or CRC32) */
        WRITE_REG32(M4_CRC->CR, u32CrcProtocol);
        /* Set initial value */
        if (u32CrcProtocol == CRC_CRC32)
        {
            WRITE_REG32(M4_CRC->DAT0, u32InitVal);
        }
        else
        {
            WRITE_REG16(M4_CRC->RESLT, u32InitVal);
        }
        /* Write data */
        if (u8BitWidth == CRC_BW_8)
        {
            CRC_8BitWrite(pvData, u32Length);
        }
        else if (u8BitWidth == CRC_BW_16)
        {
            CRC_16BitWrite(pvData, u32Length);
        }
        else
        {
            CRC_32BitWrite(pvData, u32Length);
        }
        /* Write checksum */
        if (u32CrcProtocol == CRC_CRC32)
        {
            WRITE_REG32(M4_CRC->DAT0, u32CheckSum);
        }
        else
        {
            WRITE_REG16(M4_CRC->DAT0, u32CheckSum);
        }
        /* Get flag */
        if (READ_REG32_BIT(M4_CRC->CR, CRC_CR_FLAG) != 0UL)
        {
            enFlag = Set;
        }

    }

    return enFlag;
}

/**
 * @}
 */

/**
 * @addtogroup CRC_Local_Functions CRC Local Functions
 * @{
 */

/**
 * @brief  Write CRC data register in bytes.
 * @param  [in]  au8Data            The buffer for writing.
 * @param  [in]  u32Length          The length of data the in bytes.
 * @retval None
 */
static void CRC_8BitWrite(const uint8_t au8Data[], uint32_t u32Length)
{
    uint32_t i;

    for (i = 0UL; i < u32Length; i++)
    {
        RW_MEM8(&M4_CRC->DAT0) = au8Data[i];

    }
}

/**
 * @brief  Write CRC data register in half words.
 * @param  [in]  au16Data           The buffer for writing.
 * @param  [in]  u32Length          The length of the data in half words.
 * @retval None
 */
static void CRC_16BitWrite(const uint16_t au16Data[], uint32_t u32Length)
{
    uint32_t i;

    for (i = 0UL; i < u32Length; i++)
    {
        RW_MEM16((&M4_CRC->DAT0)) = au16Data[i];
    }
}

/**
 * @brief  Write CRC data register in words.
 * @param  [in]  au32Data           The buffer for writing.
 * @param  [in]  u32Length          The length of the data in words.
 * @retval None
 */
static void CRC_32BitWrite(const uint32_t au32Data[], uint32_t u32Length)
{
    uint32_t i;

    for (i = 0UL; i < u32Length; i++)
    {
        RW_MEM32((&M4_CRC->DAT0)) = au32Data[i];
    }
}

/**
 * @}
 */

#endif /* DDL_CRC_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
