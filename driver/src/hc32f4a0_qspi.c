/**
 *******************************************************************************
 * @file  hc32f4a0_qspi.c
 * @brief This file provides firmware functions to manage the QSPI.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-03-10       Wuze            First version
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
#include "hc32f4a0_qspi.h"
#include "hc32f4a0_utility.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_QSPI QSPI
 * @brief QSPI Driver Library
 * @{
 */

#if (DDL_QSPI_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup QSPI_Local_Macros QSPI Local Macros
 * @{
 */

/**
 * @defgroup QSPI_Configuration_Bit_Mask QSPI Configuration Bit Mask
 * @{
 */
#define QSPI_FLAG_CLR_MSK                   (QSPI_FLAG_ROM_ACCESS_ERR)
/**
 * @}
 */

/**
 * @defgroup QSPI_Common_Instruction QSPI Common Instruction
 * @{
 */
#define QSPI_INS_ENTER_4BA                  (0xB7U)
#define QSPI_INS_EXIT_4BA                   (0xE9U)
/**
 * @}
 */

/**
 * @defgroup QSPI_Check_Parameters_Validity QSPI check parameters validity
 * @{
 */
#define IS_QSPI_CLK_DIV(x)                                                     \
(   ((x) != 0U) && (((x) | QSPI_CR_DIV) == QSPI_CR_DIV))

#define IS_QSPI_CS_SETUP_TIMING(x)                                             \
(   ((x) == QSPI_CS_SETUP_BEFORE_0P5_CYCLE)             ||                     \
    ((x) == QSPI_CS_SETUP_BEFORE_1P5_CYCLE))

#define IS_QSPI_CS_RELEASE_TIMING(x)                                           \
(   ((x) == QSPI_CS_RELEASE_AFTER_0P5_CYCLE)            ||                     \
    ((x) == QSPI_CS_RELEASE_AFTER_1P5_CYCLE))

#define IS_QSPI_CS_IDLE_TIME(x)                                                \
(   (x) <= QSPI_CS_IDLE_16_CYCLE)

#define IS_QSPI_CS_EXTEND_TIME(x)                                              \
(   ((x) == QSPI_CS_EXTEND_0_CYCLE)                     ||                     \
    ((x) == QSPI_CS_EXTEND_32_CYCLE)                    ||                     \
    ((x) == QSPI_CS_EXTEND_128_CYCLE)                   ||                     \
    ((x) == QSPI_CS_EXTEND_INFINITE))

#define IS_QSPI_SPI_MODE(x)                                                    \
(   ((x) == QSPI_SPI_MODE_0)                            ||                     \
    ((x) == QSPI_SPI_MODE_3))

#define IS_QSPI_PREFETCH_STOP_POSITION(x)                                      \
(   ((x) == QSPI_PREFETCH_STOP_BYTE_EDGE)               ||                     \
    ((x) == QSPI_PREFETCH_STOP_IMMED))

#define IS_QSPI_READ_MODE(x)                                                   \
(   ((x) == QSPI_READ_STANDARD_READ)                    ||                     \
    ((x) == QSPI_READ_FAST_READ)                        ||                     \
    ((x) == QSPI_READ_FAST_READ_DUAL_OUTPUT)            ||                     \
    ((x) == QSPI_READ_FAST_READ_DUAL_IO)                ||                     \
    ((x) == QSPI_READ_FAST_READ_QUAD_OUTPUT)            ||                     \
    ((x) == QSPI_READ_FAST_READ_QUAD_IO)                ||                     \
    ((x) == QSPI_READ_CUSTOM_STANDARD_READ)             ||                     \
    ((x) == QSPI_READ_CUSTOM_FAST_READ))

#define IS_QSPI_COMM_MODE(x)                                                   \
(   ((x) == QSPI_COMM_ROM_ACCESS)                       ||                     \
    ((x) == QSPI_COMM_DIRECT_COMM))

#define IS_QSPI_ADDR_WIDTH(x)                                                  \
(   ((x) == QSPI_ADDR_WIDTH_1_BYTE)                     ||                     \
    ((x) == QSPI_ADDR_WIDTH_2_BYTE)                     ||                     \
    ((x) == QSPI_ADDR_WIDTH_3_BYTE)                     ||                     \
    ((x) == QSPI_ADDR_WIDTH_4_BYTE))

#define IS_QSPI_DUMMY_CYCLES(x)                                                \
(   ((x) >> QSPI_FCR_DMCYCN_POS) <= 15U)

#define IS_QSPI_INSTR_MODE(x)                                                  \
(   ((x) == QSPI_INSTR_1_LINE)                          ||                     \
    ((x) == QSPI_INSTR_2_LINE)                          ||                     \
    ((x) == QSPI_INSTR_4_LINE))

#define IS_QSPI_ADDR_MODE(x)                                                   \
(   ((x) == QSPI_ADDR_1_LINE)                           ||                     \
    ((x) == QSPI_ADDR_2_LINE)                           ||                     \
    ((x) == QSPI_ADDR_4_LINE))

#define IS_QSPI_DATA_MODE(x)                                                   \
(   ((x) == QSPI_DATA_1_LINE)                           ||                     \
    ((x) == QSPI_DATA_2_LINE)                           ||                     \
    ((x) == QSPI_DATA_4_LINE))

#define IS_QSPI_BLOCK_NUM(x)                                                   \
(   (x) <= 62U)

#define IS_QSPI_PREFETCH_CMD(x)                                                \
(   ((x) == QSPI_PREFETCH_ENABLE)                       ||                     \
    ((x) == QSPI_PREFETCH_DISABLE))

#define IS_QSPI_WP_LEVLE(x)                                                    \
(   ((x) == QSPI_WP_LOW)                                ||                     \
    ((x) == QSPI_WP_HIGH))

/**
 * @}
 */

/**
 * @defgroup QSPI_Miscellaneous_Macros QSPI Miscellaneous Macros
 * @{
 */
#define QSPI_WORD_TO_BYTE(__word__, __au8__)                  \
        do {                                                  \
                (__au8__)[0U] = (uint8_t)(__word__);          \
                (__au8__)[1U] = (uint8_t)((__word__) >> 8U);  \
                (__au8__)[2U] = (uint8_t)((__word__) >> 16U); \
                (__au8__)[3U] = (uint8_t)((__word__) >> 24U); \
        } while (0U)

#define QSPI_ROM_MAP_BASE                   (0x98000000UL)

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

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup QSPI_Global_Functions QSPI Global Functions
 * @{
 */

/**
 * @brief  Initializes QSPI peripheral according to the specified parameters \
 *         in the structure stc_qspi_init_t
 * @param  [in]  pstcInit               Pointer to a stc_qspi_init_t structure value that \
 *                                      contains the configuration information for QSPI.
 * @retval An en_result_t enumeration type value.
 *   @arg  Ok:                          No error occurred.
 *   @arg  ErrorInvalidParameter:       pstcInit == NULL.
 */
en_result_t QSPI_Init(const stc_qspi_init_t *pstcInit)
{
    uint32_t u32DutyCorrection = 0U;
    en_result_t enRet = ErrorInvalidParameter;

    DDL_ASSERT(IS_QSPI_CLK_DIV(pstcInit->u32ClkDiv));
    DDL_ASSERT(IS_QSPI_CS_SETUP_TIMING(pstcInit->u32CSSetupTiming));
    DDL_ASSERT(IS_QSPI_CS_RELEASE_TIMING(pstcInit->u32CSReleaseTiming));
    DDL_ASSERT(IS_QSPI_CS_IDLE_TIME(pstcInit->u32CSIdleTime));
    DDL_ASSERT(IS_QSPI_CS_EXTEND_TIME(pstcInit->u32CSExtendTime));
    DDL_ASSERT(IS_QSPI_SPI_MODE(pstcInit->u32SPIMode));
    DDL_ASSERT(IS_QSPI_PREFETCH_STOP_POSITION(pstcInit->u32PrefetchStopPos));
    DDL_ASSERT(IS_QSPI_PREFETCH_CMD(pstcInit->u32PrefetchCmd));
    DDL_ASSERT(IS_QSPI_WP_LEVLE(pstcInit->u32WPLevel));
    DDL_ASSERT(IS_QSPI_READ_MODE(pstcInit->u32ReadMode));
    DDL_ASSERT(IS_QSPI_COMM_MODE(pstcInit->u32CommMode));
    DDL_ASSERT(IS_QSPI_ADDR_WIDTH(pstcInit->u32AddrWidth));
    DDL_ASSERT(IS_QSPI_DUMMY_CYCLES(pstcInit->u32DummyCycles));
    DDL_ASSERT(IS_QSPI_INSTR_MODE(pstcInit->u32InstrMode));
    DDL_ASSERT(IS_QSPI_ADDR_MODE(pstcInit->u32AddrMode));
    DDL_ASSERT(IS_QSPI_DATA_MODE(pstcInit->u32DataMode));

    if (pstcInit != NULL)
    {
        if ((pstcInit->u32ClkDiv != 0U) && (pstcInit->u32ClkDiv & QSPI_CR_DIV_0) == 0U)
        {
            u32DutyCorrection = QSPI_FCR_DUTY;
        }

        WRITE_REG32(M4_QSPI->CR, (pstcInit->u32ClkDiv          | \
                                  pstcInit->u32SPIMode         | \
                                  pstcInit->u32PrefetchStopPos | \
                                  pstcInit->u32PrefetchCmd     | \
                                  pstcInit->u32ReadMode        | \
                                  pstcInit->u32CommMode        | \
                                  pstcInit->u32InstrMode       | \
                                  pstcInit->u32AddrMode        | \
                                  pstcInit->u32DataMode));

        WRITE_REG32(M4_QSPI->CSCR, (pstcInit->u32CSExtendTime  | \
                                    pstcInit->u32CSIdleTime));

        WRITE_REG32(M4_QSPI->FCR, (pstcInit->u32CSSetupTiming   | \
                                   pstcInit->u32CSReleaseTiming | \
                                   pstcInit->u32AddrWidth       | \
                                   pstcInit->u32DummyCycles     | \
                                   pstcInit->u32WPLevel         | \
                                   u32DutyCorrection));

        WRITE_REG32(M4_QSPI->CCMD, pstcInit->u32RomAccessInstr);

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-initializes QSPI peripheral. Reset the registers of QSPI.
 * @param  None
 * @retval None
 */
void QSPI_DeInit(void)
{
    WRITE_REG32(M4_QSPI->CR, 0x3F0000UL);
    WRITE_REG32(M4_QSPI->CSCR, 0xFUL);
    WRITE_REG32(M4_QSPI->FCR, 0x80B3UL);
    WRITE_REG32(M4_QSPI->SR, 0x8000UL);
    WRITE_REG32(M4_QSPI->CCMD, 0x0UL);
    WRITE_REG32(M4_QSPI->XCMD, 0xFFUL);
    WRITE_REG32(M4_QSPI->SR2, QSPI_FLAG_ROM_ACCESS_ERR);
    WRITE_REG32(M4_QSPI->EXAR, 0x0UL);
}

/**
 * @brief  Set a default value for QSPI initialization structure.
 * @param  [in]  pstcInit               Pointer to a stc_qspi_init_t structure value that \
 *                                      contains the configuration information QSPI.
 * @retval An en_result_t enumeration type value.
 *   @arg  Ok:                          No error occurred.
 *   @arg  ErrorInvalidParameter:       pstcInit == NULL.
 */
en_result_t QSPI_StructInit(stc_qspi_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcInit != NULL)
    {
        pstcInit->u32ClkDiv          = QSPI_CLK_DIV_2;
        pstcInit->u32CSSetupTiming   = QSPI_CS_SETUP_BEFORE_0P5_CYCLE;
        pstcInit->u32CSReleaseTiming = QSPI_CS_RELEASE_AFTER_0P5_CYCLE;
        pstcInit->u32CSIdleTime      = QSPI_CS_IDLE_4_CYCLE;
        pstcInit->u32CSExtendTime    = QSPI_CS_EXTEND_0_CYCLE;
        pstcInit->u32SPIMode         = QSPI_SPI_MODE_0;
        pstcInit->u32PrefetchStopPos = QSPI_PREFETCH_STOP_BYTE_EDGE;
        pstcInit->u32PrefetchCmd     = QSPI_PREFETCH_ENABLE;
        pstcInit->u32WPLevel         = QSPI_WP_HIGH;

        pstcInit->u32ReadMode        = QSPI_READ_STANDARD_READ;
        pstcInit->u32CommMode        = QSPI_COMM_ROM_ACCESS;
        pstcInit->u32AddrWidth       = QSPI_ADDR_WIDTH_3_BYTE;
        pstcInit->u32RomAccessInstr  = 0x0UL;
        pstcInit->u32DummyCycles     = QSPI_DUMMY_CYCLE_3;
        pstcInit->u32InstrMode       = QSPI_INSTR_1_LINE;
        pstcInit->u32AddrMode        = QSPI_ADDR_1_LINE;
        pstcInit->u32DataMode        = QSPI_DATA_1_LINE;

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  QSPI write data.
 * @param  [in]  u32Instr               Instruction.
 * @param  [in]  u32Address             Address.
 * @param  [in]  pu8Src                 Pointer to an array that stores the data to be written.
 * @param  [in]  u32SrcSize             Size of the data to be written.
 * @retval An en_result_t enumeration type value.
 *   @arg  Ok:                          No error occurred.
 *   @arg  ErrorInvalidParameter:       pu8Src == NULL or u32SrcSize == 0U
 */
en_result_t QSPI_WriteData(uint32_t u32Instr, uint32_t u32Address, \
                           const uint8_t pu8Src[], uint32_t u32SrcSize)
{
    uint32_t i;
    uint8_t au8Address[4U];
    uint32_t u32AddrWidth;
    en_result_t enRet = ErrorInvalidParameter;

    if ((pu8Src != NULL) && (u32SrcSize > 0U))
    {
        QSPI_WORD_TO_BYTE(u32Address, au8Address);
        u32AddrWidth = (READ_REG32_BIT(M4_QSPI->FCR, QSPI_FCR_AWSL) >> QSPI_FCR_AWSL_POS) + 1U;

        /* Enter direct communication mode. */
        QSPI_EnterDirectCommMode();
        /* Send instruction. */
        WRITE_REG32(M4_QSPI->DCOM, u32Instr);
        /* Send ROM address. */
        for (i=0U; i<u32AddrWidth; i++)
        {
            WRITE_REG32(M4_QSPI->DCOM, au8Address[i]);
        }
        /* Write data at last. */
        for (i=0U; i<u32SrcSize; i++)
        {
            WRITE_REG32(M4_QSPI->DCOM, pu8Src[i]);
        }
        /* Exit direct communication mode. */
        CLEAR_REG32_BIT(M4_QSPI->CR, QSPI_CR_DCOME);

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  QSPI write data.
 * @param  [in]  u32ReadMode            QSPI read mode.
 * @param  [in]  u32Address             Address.
 * @param  [in]  pu8Dest                Pointer to an array that used to store the read data.
 * @param  [in]  u32DestSize            Size of the data to be read.
 * @retval An en_result_t enumeration type value.
 *   @arg  Ok:                          No error occurred.
 *   @arg  ErrorInvalidParameter:       pu8Dest == NULL or u32DestSize == 0U
 */
en_result_t QSPI_ReadData(uint32_t u32ReadMode, uint32_t u32Address, \
                          uint8_t pu8Dest[], uint32_t u32DestSize)
{
	uint32_t i = 0U;
    en_result_t enRet = ErrorInvalidParameter;

    if ((pu8Dest != NULL) && (u32DestSize > 0U))
    {
        MODIFY_REG32(M4_QSPI->CR, QSPI_CR_MDSEL, u32ReadMode);
        /* Send instruction. */
        u32Address += QSPI_ROM_MAP_BASE;
        while (u32DestSize-- != 0U)
        {
            pu8Dest[i++] = *(uint8_t *)(u32Address++);
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Specifies read mode.
 * @param  [in]  u32ReadMode                QSPI read mode.
 *                                          This parameter can be a value of @ref QSPI_Read_Mode
 *   @arg  QSPI_READ_STANDARD_READ:         Standard read mode (no dummy cycles).
 *   @arg  QSPI_READ_FAST_READ:             Fast read mode (dummy cycles between address and data).
 *   @arg  QSPI_READ_FAST_READ_DUAL_OUTPUT: Fast read dual output mode (data on 2 lines).
 *   @arg  QSPI_READ_FAST_READ_DUAL_IO:     Fast read dual I/O mode (address and data on 2 lines).
 *   @arg  QSPI_READ_FAST_READ_QUAD_OUTPUT: Fast read quad output mode (data on 4 lines).
 *   @arg  QSPI_READ_FAST_READ_QUAD_IO:     Fast read quad I/O mode (address and data on 4 lines).
 *   @arg  QSPI_READ_CUSTOM_STANDARD_READ:  Custom standard read mode.
 *   @arg  QSPI_READ_CUSTOM_FAST_READ:      Custom fast read mode.
 * @retval None
 */
void QSPI_SetReadMode(uint32_t u32ReadMode)
{
    DDL_ASSERT(IS_QSPI_READ_MODE(u32ReadMode));
    MODIFY_REG32(M4_QSPI->CR, QSPI_CR_MDSEL, u32ReadMode);
}

/**
 * @brief  Enable or disable duty correction.
 * @param  [in]  enNewState             An en_functional_state_t enumeration type value.
 *   @arg Enable:                       Enable duty correction.
 *   @arg Disable:                      Disable duty correction.
 * @retval None
 */
void QSPI_DutyCorrectCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    if (enNewState == Enable)
    {
        SET_REG32_BIT(M4_QSPI->FCR, QSPI_FCR_DUTY);
    }
    else
    {
        CLEAR_REG32_BIT(M4_QSPI->FCR, QSPI_FCR_DUTY);
    }
}

/**
 * @brief  Enable or disable prefetch function.
 * @param  [in]  enNewState             An en_functional_state_t enumeration type value.
 *   @arg Enable:                       Enable prefetch function.
 *   @arg Disable:                      Disable prefetch function.
 * @retval None
 */
void QSPI_PrefetchCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    if (enNewState == Enable)
    {
        SET_REG32_BIT(M4_QSPI->CR, QSPI_CR_PFE);
    }
    else
    {
        CLEAR_REG32_BIT(M4_QSPI->CR, QSPI_CR_PFE);
    }
}

/**
 * @brief  Enable or disable XIP mode.
 * @param  [in]  enNewState             An en_functional_state_t enumeration type value.
 *   @arg Enable:                       Enable XIP mode.
 *   @arg Disable:                      Disable XIP mode.
 * @retval None
 */
void QSPI_XIPModeCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    if (enNewState == Enable)
    {
        SET_REG32_BIT(M4_QSPI->CR, QSPI_CR_XIPE);
    }
    else
    {
        CLEAR_REG32_BIT(M4_QSPI->CR, QSPI_CR_XIPE);
    }
}

/**
 * @brief  Specifies the level of WP.
 * @param  [in]  u32Level               The level of WP.
 *                                      This parameter can be a value of @ref QSPI_WP_Level
 *   @arg QSPI_WP_LOW:                  Set WP output low.
 *   @arg QSPI_WP_HIGH:                 Set WP output high.
 * @retval None
 */
void QSPI_SetWPPinLevel(uint32_t u32Level)
{
    DDL_ASSERT(IS_QSPI_WP_LEVLE(u32Level));
    MODIFY_REG32(M4_QSPI->FCR, QSPI_FCR_WPOL, u32Level);
}

/**
 * @brief  Enable or disable 4-byte address mode.
 * @param  [in]  enNewState             An en_functional_state_t enumeration type value.
 *   @arg Enable:                       Enable 4-byte address mode.
 *   @arg Disable:                      Disable 4-byte address mode.
 * @retval None
 */
void QSPI_4ByteAddrModeCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    if (enNewState == Enable)
    {
        SET_REG32_BIT(M4_QSPI->FCR, QSPI_FCR_FOUR_BIC);
    }
    else
    {
        CLEAR_REG32_BIT(M4_QSPI->FCR, QSPI_FCR_FOUR_BIC);
    }
}

/**
 * @brief  Selects the block to access.
 * @param  [in]  u32Block               External block. A number between 0U and 62U, inclusive.
 * @retval None
 */
void QSPI_SelectBlock(uint32_t u32Block)
{
    DDL_ASSERT(IS_QSPI_BLOCK_NUM(u32Block));
    WRITE_REG32(M4_QSPI->EXAR, ((u32Block << QSPI_EXAR_EXADR_POS) & QSPI_EXAR_EXADR));
}

/**
 * @brief  Get the size of prefetched data.
 * @param  None
 * @retval A number between 0U and 18U, inclusive.
 */
uint32_t QSPI_GetPrefetchedSize(void)
{
    return READ_REG32_BIT(M4_QSPI->SR, QSPI_SR_PFNUM) >> QSPI_SR_PFNUM_POS;
}

/**
 * @brief  Get the status of the specified QSPI flags.
 * @param  [in]  u32Flag                The status flags of QSPI.
 *                                      This parameter can be values of @ref QSPI_Status_Flag
 *   @arg  QSPI_FLAG_DIRECT_COMM_BUSY:  Serial transfer being processed.
 *   @arg  QSPI_FLAG_XIP_MODE:          XIP mode.
 *   @arg  QSPI_FLAG_ROM_ACCESS_ERR:    ROM access detection status in direct communication mode.
 *   @arg  QSPI_FLAG_PREFETCH_BUF_FULL: Prefetch buffer is full.
 *   @arg  QSPI_FLAG_PREFETCH_ACTIVE:   Prefetch function operating.
 * @retval An en_flag_status_t enumeration type value.
 *   @arg Set:                          At least one of the specified flags is set.
 *   @arg Reset:                        None of the specified flags is set.
 */
en_flag_status_t QSPI_GetStatus(uint32_t u32Flag)
{
    en_flag_status_t enFlag = Reset;

    if (READ_REG32_BIT(M4_QSPI->SR, (u32Flag & QSPI_FLAG_ALL)))
    {
        enFlag = Set;
    }

    return enFlag;
}

/**
 * @brief  Clear the specified flag.
 * @param  [in]  u32Flag                The specified flags to be cleared.
 *                                      This parameter can only be the following:
 *   @arg  QSPI_FLAG_ROM_ACCESS_ERR:    ROM access detection status in direct communication mode.
 * @retval None
 */
void QSPI_ClrStatus(uint32_t u32Flag)
{
    SET_REG32_BIT(M4_QSPI->SR2, (u32Flag & QSPI_FLAG_CLR_MSK));
}

/**
 * @}
 */

#endif /* DDL_QSPI_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
