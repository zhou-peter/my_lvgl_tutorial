/**
 *******************************************************************************
 * @file  hc32f4a0_spi.c
 * @brief This file provides firmware functions to manage the Serial Peripheral
 *        Interface(SPI).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-03-11       Wangmin         First version
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
#include "hc32f4a0_spi.h"
#include "hc32f4a0_utility.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_SPI SPI
 * @brief Serial Peripheral Interface Driver Library
 * @{
 */

#if (DDL_SPI_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup SPI_Local_Macros SPI Local Macros
 * @{
 */

#define SPI_CFG1_DEFAULT 0x00000010UL
#define SPI_CFG2_DEFAULT 0x00000F1DUL
#define SPI_SR_DEFAULT   0x00000020UL

#define SPI_FLAG_CLR_ALL            (SPI_FLAG_OVERLOAD          |              \
                                     SPI_FLAG_IDLE              |              \
                                     SPI_FLAG_MODE_FAULT        |              \
                                     SPI_FLAG_PARITY_ERROR      |              \
                                     SPI_FLAG_UNDERLOAD)

/**
 * @defgroup SPI_Check_Parameters_Validity SPI check parameters validity
 * @{
 */

/* Parameter valid check for SPI peripheral */
#define IS_VALID_SPI_UNIT(x)                                                   \
(   (M4_SPI1 == (x))                            ||                             \
    (M4_SPI2 == (x))                            ||                             \
    (M4_SPI3 == (x))                            ||                             \
    (M4_SPI4 == (x))                            ||                             \
    (M4_SPI5 == (x))                            ||                             \
    (M4_SPI6 == (x)))

/* Parameter valid check for SPI wire mode */
#define IS_SPI_WIRE_MODE(x)                                                    \
(   ((x) == SPI_WIRE_4)                     ||                                 \
    ((x) == SPI_WIRE_3))

/* Parameter valid check for SPI transfer mode */
#define IS_SPI_TRANS_MODE(x)                                                   \
(   ((x) == SPI_FULL_DUPLEX)                ||                                 \
    ((x) == SPI_SEND_ONLY))

/* Parameter valid check for SPI master slave mode */
#define IS_SPI_MASTER_SLAVE(x)                                                 \
(   ((x) == SPI_SLAVE)                      ||                                 \
    ((x) == SPI_MASTER))

/* Parameter valid check for SPI loopback mode */
#define IS_SPI_SPLPBK(x)                                                       \
(   ((x) == SPI_SPLPBK_INVALID)             ||                                 \
    ((x) == SPI_SPLPBK_MOSI_INVERT)         ||                                 \
    ((x) == SPI_SPLPBK_MOSI))

/* Parameter valid check for SPI communication suspend function status */
#define IS_SPI_SUSP_MODE_STD(x)                                                \
(   ((x) == SPI_COM_SUSP_FUNC_OFF)          ||                                 \
    ((x) == SPI_COM_SUSP_FUNC_ON))

/* Parameter valid check for SPI fault dectet function status */
#define IS_SPI_MODFE_CMD(x)                                                    \
(   ((x) == SPI_MODFE_DISABLE)              ||                                 \
    ((x) == SPI_MODFE_ENABLE))

/* Parameter valid check for SPI parity check mode */
#define IS_SPI_PARITY_CHECK(x)                                                 \
(   ((x) == SPI_PARITY_INVALID)             ||                                 \
    ((x) == SPI_PARITY_EVEN)                ||                                 \
    ((x) == SPI_PARITY_ODD))

/* Parameter valid check for SPI interval time delay */
#define IS_SPI_INTERVAL_DELAY(x)                                               \
(   ((x) == SPI_INTERVAL_TIME_1SCK_2PCLK1)  ||                                 \
    ((x) == SPI_INTERVAL_TIME_2SCK_2PCLK1)  ||                                 \
    ((x) == SPI_INTERVAL_TIME_3SCK_2PCLK1)  ||                                 \
    ((x) == SPI_INTERVAL_TIME_4SCK_2PCLK1)  ||                                 \
    ((x) == SPI_INTERVAL_TIME_5SCK_2PCLK1)  ||                                 \
    ((x) == SPI_INTERVAL_TIME_6SCK_2PCLK1)  ||                                 \
    ((x) == SPI_INTERVAL_TIME_7SCK_2PCLK1)  ||                                 \
    ((x) == SPI_INTERVAL_TIME_8SCK_2PCLK1))

/* Parameter valid check for SPI release time delay */
#define IS_SPI_RELEASE_DELAY(x)                                                \
(   ((x) == SPI_RELEASE_TIME_1SCK)          ||                                 \
    ((x) == SPI_RELEASE_TIME_2SCK)          ||                                 \
    ((x) == SPI_RELEASE_TIME_3SCK)          ||                                 \
    ((x) == SPI_RELEASE_TIME_4SCK)          ||                                 \
    ((x) == SPI_RELEASE_TIME_5SCK)          ||                                 \
    ((x) == SPI_RELEASE_TIME_6SCK)          ||                                 \
    ((x) == SPI_RELEASE_TIME_7SCK)          ||                                 \
    ((x) == SPI_RELEASE_TIME_8SCK))

/* Parameter valid check for SPI Setup time delay delay */
#define IS_SPI_SETUP_DELAY(x)                                                  \
(   ((x) == SPI_SETUP_TIME_1SCK)            ||                                 \
    ((x) == SPI_SETUP_TIME_2SCK)            ||                                 \
    ((x) == SPI_SETUP_TIME_3SCK)            ||                                 \
    ((x) == SPI_SETUP_TIME_4SCK)            ||                                 \
    ((x) == SPI_SETUP_TIME_5SCK)            ||                                 \
    ((x) == SPI_SETUP_TIME_6SCK)            ||                                 \
    ((x) == SPI_SETUP_TIME_7SCK)            ||                                 \
    ((x) == SPI_SETUP_TIME_8SCK))

/* Parameter valid check for SPI SS active level */
#define IS_SPI_SS_ACTIVE_LEVEL(x)                                              \
(   ((x) == SPI_SS_ACTIVE_LOW)              ||                                 \
    ((x) == SPI_SS_ACTIVE_HIGH))

/* Parameter valid check for SPI read data register target buffer */
#define IS_SPI_RD_TARGET_BUFF(x)                                               \
(   ((x) == SPI_RD_TARGET_RD_BUF)           ||                                 \
    ((x) == SPI_RD_TARGET_WR_BUF))

/* Parameter valid check for SPI mode */
#define IS_SPI_SPI_MODE(x)                                                     \
(   ((x) == SPI_MODE_0)                     ||                                 \
    ((x) == SPI_MODE_1)                     ||                                 \
    ((x) == SPI_MODE_2)                     ||                                 \
    ((x) == SPI_MODE_3))

/* Parameter valid check for SPI SS signal */
#define IS_SPI_SS_SIGNAL(x)                                                     \
(   ((x) == SPI_SS0_VALID)                  ||                                 \
    ((x) == SPI_SS1_VALID)                  ||                                 \
    ((x) == SPI_SS2_VALID)                  ||                                 \
    ((x) == SPI_SS3_VALID))

/* Parameter valid check for SPI baudrate prescaler */
#define IS_SPI_BIT_RATE_DIV(x)                                                 \
(   ((x) == SPI_BR_DIV_2)                   ||                                 \
    ((x) == SPI_BR_DIV_4)                   ||                                 \
    ((x) == SPI_BR_DIV_8)                   ||                                 \
    ((x) == SPI_BR_DIV_16)                  ||                                 \
    ((x) == SPI_BR_DIV_32)                  ||                                 \
    ((x) == SPI_BR_DIV_64)                  ||                                 \
    ((x) == SPI_BR_DIV_128)                 ||                                 \
    ((x) == SPI_BR_DIV_256))

/* Parameter valid check for SPI data bits */
#define IS_SPI_DATA_SIZE(x)                                                    \
(   ((x) == SPI_DATA_SIZE_4BIT)             ||                                 \
    ((x) == SPI_DATA_SIZE_5BIT)             ||                                 \
    ((x) == SPI_DATA_SIZE_6BIT)             ||                                 \
    ((x) == SPI_DATA_SIZE_7BIT)             ||                                 \
    ((x) == SPI_DATA_SIZE_8BIT)             ||                                 \
    ((x) == SPI_DATA_SIZE_9BIT)             ||                                 \
    ((x) == SPI_DATA_SIZE_10BIT)            ||                                 \
    ((x) == SPI_DATA_SIZE_11BIT)            ||                                 \
    ((x) == SPI_DATA_SIZE_12BIT)            ||                                 \
    ((x) == SPI_DATA_SIZE_13BIT)            ||                                 \
    ((x) == SPI_DATA_SIZE_14BIT)            ||                                 \
    ((x) == SPI_DATA_SIZE_15BIT)            ||                                 \
    ((x) == SPI_DATA_SIZE_16BIT)            ||                                 \
    ((x) == SPI_DATA_SIZE_20BIT)            ||                                 \
    ((x) == SPI_DATA_SIZE_24BIT)            ||                                 \
    ((x) == SPI_DATA_SIZE_32BIT))

/* Parameter valid check for SPI LSB MSB mode */
#define IS_SPI_FIRST_BIT(x)                                                    \
(   ((x) == SPI_FIRST_MSB)                  ||                                 \
    ((x) == SPI_FIRST_LSB))

/* Parameter valid check for interrupt flag */
#define IS_SPI_IRQ_FLAG(x)                                                     \
(   ((x) & ~( SPI_INT_ERROR                                                    \
            | SPI_INT_TX_BUFFER_EMPTY                                          \
            | SPI_INT_RX_BUFFER_FULL                                           \
            | SPI_INT_IDLE )) == 0UL)

/* Parameter valid check for SPI status flag */
#define IS_SPI_STD_FLAG(x)                                                     \
(   ((x) == SPI_FLAG_OVERLOAD)              ||                                 \
    ((x) == SPI_FLAG_IDLE)                  ||                                 \
    ((x) == SPI_FLAG_MODE_FAULT)            ||                                 \
    ((x) == SPI_FLAG_PARITY_ERROR)          ||                                 \
    ((x) == SPI_FLAG_UNDERLOAD)             ||                                 \
    ((x) == SPI_FLAG_TX_BUFFER_EMPTY)       ||                                 \
    ((x) == SPI_FLAG_RX_BUFFER_FULL))

/* Parameter valid check for SPI status flag for clear */
#define IS_SPI_CLR_STD_FLAG(x)                                                 \
(   ((x) & ~( SPI_FLAG_OVERLOAD                                                \
            | SPI_FLAG_MODE_FAULT                                              \
            | SPI_FLAG_PARITY_ERROR                                            \
            | SPI_FLAG_UNDERLOAD )) == 0UL)

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
 * @defgroup SPI_Local_Functions SPI Local Functions
 * @{
 */


/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup SPI_Local_Variables SPI Local Variables
 * @{
 */


/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup SPI_Global_Functions SPI Global Functions
 * @{
 */

/**
 * @brief  Initializes the SPI peripheral according to the specified parameters
 *         in the structure stc_spi_init.
 * @param  [in]  SPIx               SPI unit
 * @param  [in]  pstcInit           Pointer to a stc_spi_init_t structure that contains
 *                                  the configuration information for the SPI.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 *   @arg  ErrorInvalidParameter:   pstcInit == NULL or configuration parameter error.
 */
en_result_t SPI_Init(M4_SPI_TypeDef *SPIx, const stc_spi_init_t *pstcInit)
{
    en_result_t enRet = Ok;
    uint32_t u32Frame = 0UL;

    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    if (NULL != pstcInit)
    {
        DDL_ASSERT(IS_SPI_WIRE_MODE(pstcInit->u32WireMode));
        DDL_ASSERT(IS_SPI_TRANS_MODE(pstcInit->u32TransMode));
        DDL_ASSERT(IS_SPI_MASTER_SLAVE(pstcInit->u32MasterSlave));
        DDL_ASSERT(IS_SPI_SUSP_MODE_STD(pstcInit->u32SuspMode));
        DDL_ASSERT(IS_SPI_MODFE_CMD(pstcInit->u32Modfe));
        DDL_ASSERT(IS_SPI_PARITY_CHECK(pstcInit->u32Parity));
        DDL_ASSERT(IS_SPI_SPI_MODE(pstcInit->u32SpiMode));
        DDL_ASSERT(IS_SPI_BIT_RATE_DIV(pstcInit->u32BaudRatePrescaler));
        DDL_ASSERT(IS_SPI_DATA_SIZE(pstcInit->u32DataBits));
        DDL_ASSERT(IS_SPI_FIRST_BIT(pstcInit->u32FirstBit));

        /* Configuration parameter check */
        if((SPI_MASTER == pstcInit->u32MasterSlave)&&(SPI_MODFE_ENABLE == pstcInit->u32Modfe))
        {
            /* pstcInit->u32Modfe can not be SPI_MODFE_ENABLE in master mode */
            enRet = ErrorInvalidParameter;
        }
        else if((SPI_WIRE_3 == pstcInit->u32WireMode)
                 &&((SPI_MODE_0 == pstcInit->u32SpiMode)||(SPI_MODE_2 == pstcInit->u32SpiMode)))
        {
            /* SPI_WIRE_3 can not support SPI_MODE_0 and SPI_MODE_2 */
            enRet = ErrorInvalidParameter;
        }
        else
        {
            /* Calculate the frame number */
            if(pstcInit->u32DataBits <= SPI_DATA_SIZE_8BIT)
            {
                u32Frame = SPI_FRAME_1;
            }
            else if(pstcInit->u32DataBits <= SPI_DATA_SIZE_16BIT)
            {
                u32Frame = SPI_FRAME_2;
            }
            else if(pstcInit->u32DataBits <= SPI_DATA_SIZE_24BIT)
            {
                u32Frame = SPI_FRAME_3;
            }
            else
            {
                u32Frame = SPI_FRAME_4;
            }

            WRITE_REG32(SPIx->CR1, pstcInit->u32WireMode
                                   | pstcInit->u32TransMode
                                   | pstcInit->u32MasterSlave
                                   | pstcInit->u32SuspMode
                                   | pstcInit->u32Modfe
                                   | pstcInit->u32Parity);

            MODIFY_REG32(SPIx->CFG2, SPI_CFG1_FTHLV, u32Frame);

            WRITE_REG32(SPIx->CFG2, pstcInit->u32SpiMode
                                    | pstcInit->u32BaudRatePrescaler
                                    | pstcInit->u32DataBits
                                    | pstcInit->u32FirstBit);
        }
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  De-initializes the SPI peripheral.
 * @param  [in]  SPIx               SPI unit
 * @retval None
 */
void SPI_DeInit(M4_SPI_TypeDef *SPIx)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    SPI_FunctionCmd(SPIx, Disable);

    WRITE_REG32(SPIx->CR1, 0x00000000UL);
    WRITE_REG32(SPIx->CFG1, SPI_CFG1_DEFAULT);
    WRITE_REG32(SPIx->CFG2, SPI_CFG2_DEFAULT);
    WRITE_REG32(SPIx->SR, SPI_SR_DEFAULT);
}

/**
 * @brief  Set a default value for the SPI initialization structure.
 * @param  [in]  pstcInit           Pointer to a stc_spi_init_t structure that
 *                                  contains configuration information.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pstcInit == NULL.
 */
en_result_t SPI_StructInit(stc_spi_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pstcInit)
    {
        pstcInit->u32WireMode          = SPI_WIRE_4;
        pstcInit->u32TransMode         = SPI_FULL_DUPLEX;
        pstcInit->u32MasterSlave       = SPI_MASTER;
        pstcInit->u32SuspMode          = SPI_COM_SUSP_FUNC_OFF;
        pstcInit->u32Modfe             = SPI_MODFE_DISABLE;
        pstcInit->u32Parity            = SPI_PARITY_INVALID;
        pstcInit->u32SpiMode           = SPI_MODE_0;
        pstcInit->u32BaudRatePrescaler = SPI_BR_DIV_8;
        pstcInit->u32DataBits          = SPI_DATA_SIZE_8BIT;
        pstcInit->u32FirstBit          = SPI_FIRST_MSB;

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Enable or disable SPI interrupt.
 * @param  [in]  SPIx               SPI unit
 * @param  [in]  u32IntType         SPI interrupt type. Can be one or any
 *                                  combination of the parameter @ref SPI_Interrupt_Type_Define
 *   @arg  SPI_INT_ERROR
 *   @arg  SPI_INT_TX_BUFFER_EMPTY
 *   @arg  SPI_INT_RX_BUFFER_FULL
 *   @arg  SPI_INT_IDLE
 * @param  [in]  enNewState         An en_functional_state_t enumeration value.
 *   @arg  Enable:                  Enable the specified interrupt of SPI.
 *   @arg  Disable:                 Disable the specified interrupt of SPI.
 * @retval None
 */
void SPI_IntCmd(M4_SPI_TypeDef *SPIx, uint32_t u32IntType, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_SPI_IRQ_FLAG(u32IntType));

    if (enNewState == Enable)
    {
        SET_REG32_BIT(SPIx->CR1, u32IntType);
    }
    else
    {
        CLEAR_REG32_BIT(SPIx->CR1, u32IntType);
    }
}

/**
 * @brief  SPI function enable or disable.
 * @param  [in]  SPIx               SPI unit
 * @param  [in]  enNewState         An en_functional_state_t enumeration value.
 *   @arg  Enable:                  Enable SPI function.
 *   @arg  Disable:                 Disable SPI function.
 * @retval None
 */
void SPI_FunctionCmd(M4_SPI_TypeDef *SPIx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_REG32_BIT(SPIx->CR1, SPI_CR1_SPE);
    }
    else
    {
        CLEAR_REG32_BIT(SPIx->CR1, SPI_CR1_SPE);
    }
}

/**
 * @brief  Write SPI data register.
 * @param  [in]  SPIx           SPI unit
 * @param  [in]  u32Data        The data will be written to the data register.
 * @retval None.
 */
void SPI_WriteDataReg(M4_SPI_TypeDef *SPIx, uint32_t u32Data)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    WRITE_REG32(SPIx->DR, u32Data);
}

/**
 * @brief  Read SPI data register.
 * @param  [in]  SPIx           SPI unit
 * @retval A 32-bit data of SPI data register.
 */
uint32_t SPI_ReadDataReg(const M4_SPI_TypeDef *SPIx)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    return READ_REG32(SPIx->DR);
}


/**
 * @brief  SPI get state flag.
 * @param  [in]  SPIx           SPI unit
 * @param  [in]  u32Flag        SPI state flag.
 *                              This parameter can be a value of @ref SPI_State_Flag_Define
 *   @arg  SPI_FLAG_OVERLOAD
 *   @arg  SPI_FLAG_IDLE
 *   @arg  SPI_FLAG_MODE_FAULT
 *   @arg  SPI_FLAG_PARITY_ERROR
 *   @arg  SPI_FLAG_UNDERLOAD
 *   @arg  SPI_FLAG_TX_BUFFER_EMPTY
 *   @arg  SPI_FLAG_RX_BUFFER_FULL
 * @retval An en_flag_status_t enumeration.
 *   @arg  Set: The specified flag has set.
 *   @arg  Reset: The specified flag has not set.
 */
en_flag_status_t SPI_GetFlag(const M4_SPI_TypeDef *SPIx, uint32_t u32Flag)
{
    en_flag_status_t enFlag = Reset;
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_SPI_STD_FLAG(u32Flag));

    if(0U != READ_REG32_BIT(SPIx->SR, u32Flag))
    {
        enFlag = Set;
    }

    return enFlag;
}

/**
 * @brief  SPI clear state flag.
 * @param  [in]  SPIx           SPI unit
 * @param  [in]  u32Flag        SPI state flag.
 *                              Can be one or any combination of the parameter below
 *   @arg  SPI_FLAG_OVERLOAD
 *   @arg  SPI_FLAG_MODE_FAULT
 *   @arg  SPI_FLAG_PARITY_ERROR
 *   @arg  SPI_FLAG_UNDERLOAD
 * @retval None
 */
void SPI_ClearFlag(M4_SPI_TypeDef *SPIx, uint32_t u32Flag)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_SPI_CLR_STD_FLAG(u32Flag));

    CLEAR_REG32_BIT(SPIx->SR, u32Flag);
}

/**
 * @brief  SPI loopback function configuration.
 * @param  [in]  SPIx           SPI unit
 * @param  [in]  u32Mode        Loopback mode.
 *                              Can be one parameter @ref SPI_Loopback_Selection_Define
 *   @arg  SPI_SPLPBK_INVALID
 *   @arg  SPI_SPLPBK_MOSI_INVERT
 *   @arg  SPI_SPLPBK_MOSI
 * @retval None
 */
void SPI_LoopbackModeCfg(M4_SPI_TypeDef *SPIx, uint32_t u32Mode)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_SPI_SPLPBK(u32Mode));

    MODIFY_REG32(SPIx->CR1, SPI_CR1_SPLPBK | SPI_CR1_SPLPBK2, u32Mode);
}

/**
 * @brief  SPI parity check error self diagnosis function enable or disable.
 * @param  [in]  SPIx               SPI unit
 * @param  [in]  enNewState         An en_functional_state_t enumeration value.
 *   @arg  Enable:                  Enable function.
 *   @arg  Disable:                 Disable function.
 * @retval None
 */
void SPI_PateCmd(M4_SPI_TypeDef *SPIx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_REG32_BIT(SPIx->CR1, SPI_CR1_PATE);
    }
    else
    {
        CLEAR_REG32_BIT(SPIx->CR1, SPI_CR1_PATE);
    }
}

/**
 * @brief  SPI signals delay time configuration
 * @param  [in]  SPIx               SPI unit
 * @param  [in]  pstcDelayCfg       Pointer to a stc_spi_delay_t structure that contains
 *                                  the configuration information for the SPI delay time.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 *   @arg  ErrorInvalidParameter:   pstcDelayCfg == NULL
 */
en_result_t SPI_DelayTimeCfg(M4_SPI_TypeDef *SPIx, const stc_spi_delay_t *pstcDelayCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    if (NULL != pstcDelayCfg)
    {
        DDL_ASSERT(IS_SPI_INTERVAL_DELAY(pstcDelayCfg->u32IntervalDelay));
        DDL_ASSERT(IS_SPI_RELEASE_DELAY(pstcDelayCfg->u32ReleaseDelay));
        DDL_ASSERT(IS_SPI_SETUP_DELAY(pstcDelayCfg->u32SetupDelay));

        /* Interval delay */
        if(SPI_INTERVAL_TIME_1SCK_2PCLK1 == pstcDelayCfg->u32IntervalDelay)
        {
            CLEAR_REG32_BIT(SPIx->CFG2, SPI_CFG2_MIDIE);
            CLEAR_REG32_BIT(SPIx->CFG1, SPI_CFG1_MIDI);
        }
        else
        {
            MODIFY_REG32(SPIx->CFG1, SPI_CFG1_MIDI, pstcDelayCfg->u32IntervalDelay);
            SET_REG32_BIT(SPIx->CFG2, SPI_CFG2_MIDIE);
        }

        /* SCK release delay */
        if(SPI_RELEASE_TIME_1SCK == pstcDelayCfg->u32ReleaseDelay)
        {
            CLEAR_REG32_BIT(SPIx->CFG2, SPI_CFG2_MSSDLE);
            CLEAR_REG32_BIT(SPIx->CFG1, SPI_CFG1_MSSDL);
        }
        else
        {
            SET_REG32_BIT(SPIx->CFG2, SPI_CFG2_MSSDLE);
            MODIFY_REG32(SPIx->CFG1, SPI_CFG1_MSSDL, pstcDelayCfg->u32ReleaseDelay);
        }

        /* Setup delay */
        if(SPI_SETUP_TIME_1SCK == pstcDelayCfg->u32SetupDelay)
        {
            CLEAR_REG32_BIT(SPIx->CFG2, SPI_CFG2_MSSIE);
            CLEAR_REG32_BIT(SPIx->CFG1, SPI_CFG1_MSSI);
        }
        else
        {
            SET_REG32_BIT(SPIx->CFG2, SPI_CFG2_MSSIE);
            MODIFY_REG32(SPIx->CFG1, SPI_CFG1_MSSI, pstcDelayCfg->u32SetupDelay);
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set a default value for the SPI delay time configuration structure.
 * @param  [in]  pstcDelayCfg       Pointer to a stc_spi_delay_t structure that
 *                                  contains configuration information.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pstcDelayCfg == NULL.
 */
en_result_t SPI_DelayStructInit(stc_spi_delay_t *pstcDelayCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pstcDelayCfg)
    {
        pstcDelayCfg->u32IntervalDelay = SPI_INTERVAL_TIME_1SCK_2PCLK1;
        pstcDelayCfg->u32ReleaseDelay = SPI_RELEASE_TIME_1SCK;
        pstcDelayCfg->u32SetupDelay = SPI_SETUP_TIME_1SCK;
        enRet = Ok;
    }

    return enRet;
}
/**
 * @brief  Set a default value for the SPI delay time configuration structure.
 * @param  [in]  pstcSSValidLevelCfg  Pointer to a stc_spi_ss_valid_level_t
 *                                    structure that contains configuration information.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                      No errors occurred.
 *   @arg  ErrorInvalidParameter:   pstcInit == NULL.
 */
en_result_t SPI_SSValidLevelStructInit(stc_spi_ss_valid_level_t *pstcSSValidLevelCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pstcSSValidLevelCfg)
    {
        pstcSSValidLevelCfg->u32ValidLevelSS0 = SPI_SS_ACTIVE_LOW;
        pstcSSValidLevelCfg->u32ValidLevelSS1 = SPI_SS_ACTIVE_LOW;
        pstcSSValidLevelCfg->u32ValidLevelSS2 = SPI_SS_ACTIVE_LOW;
        pstcSSValidLevelCfg->u32ValidLevelSS3 = SPI_SS_ACTIVE_LOW;

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  SPI SS signal valid level configuration
 * @param  [in]  SPIx                  SPI unit
 * @param  [in]  pstcSSValidLevelCfg   Pointer to a stc_spi_ss_valid_level_t structure that contains
 *                                     the configuration information for the valid level for SS signals.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 *   @arg  ErrorInvalidParameter:   pstcSSValidLevelCfg == NULL
 */
en_result_t SPI_SSValidLevelCfg(M4_SPI_TypeDef *SPIx, const stc_spi_ss_valid_level_t *pstcSSValidLevelCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    if (NULL != pstcSSValidLevelCfg)
    {
        DDL_ASSERT(IS_SPI_SS_ACTIVE_LEVEL(pstcSSValidLevelCfg->u32ValidLevelSS0));
        DDL_ASSERT(IS_SPI_SS_ACTIVE_LEVEL(pstcSSValidLevelCfg->u32ValidLevelSS1));
        DDL_ASSERT(IS_SPI_SS_ACTIVE_LEVEL(pstcSSValidLevelCfg->u32ValidLevelSS2));
        DDL_ASSERT(IS_SPI_SS_ACTIVE_LEVEL(pstcSSValidLevelCfg->u32ValidLevelSS3));

        MODIFY_REG32(SPIx->CFG1,
                     SPI_CFG1_SS0PV | SPI_CFG1_SS1PV | SPI_CFG1_SS2PV | SPI_CFG1_SS3PV,
                     pstcSSValidLevelCfg->u32ValidLevelSS0 << SPI_CFG1_SS0PV_POS
                     | pstcSSValidLevelCfg->u32ValidLevelSS1 << SPI_CFG1_SS1PV_POS
                     | pstcSSValidLevelCfg->u32ValidLevelSS2 << SPI_CFG1_SS2PV_POS
                     | pstcSSValidLevelCfg->u32ValidLevelSS3 << SPI_CFG1_SS3PV_POS);
        enRet = Ok;
    }
    return enRet;
}

/**
 * @brief  SPI valid SS signal configuration
 * @param  [in]  SPIx               SPI unit
 * @param  [in]  u32ValidSS         Valid SS signal
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 */
en_result_t SPI_ValidSSCfg(M4_SPI_TypeDef *SPIx, uint32_t u32ValidSS)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_SPI_SS_SIGNAL(u32ValidSS));

    MODIFY_REG32(SPIx->CFG2, SPI_CFG2_SSA, u32ValidSS);

    return enRet;
}


/**
 * @brief  SPI read buffer configuration
 * @param  [in]  SPIx               SPI unit
 * @param  [in]  u32ReadBuf         Target buffer for read operation @ref SPI_Read_Target_Buffer_Define
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 */
en_result_t SPI_ReadBufCfg(M4_SPI_TypeDef *SPIx, uint32_t u32ReadBuf)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_SPI_RD_TARGET_BUFF(u32ReadBuf));

    MODIFY_REG32(SPIx->CFG1, SPI_CFG1_SPRDTD, u32ReadBuf);

    return enRet;
}



/**
 * @}
 */



/**
 * @addtogroup SPI_Local_Functions SPI Local Functions
 * @{
 */

/**
 * @}
 */

#endif /* DDL_SPI_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/