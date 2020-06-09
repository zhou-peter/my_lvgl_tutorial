/**
 *******************************************************************************
 * @file  hc32f4a0_spi.h
 * @brief This file contains all the functions prototypes of the SPI driver
 *        library.
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
#ifndef __HC32F4A0_SPI_H__
#define __HC32F4A0_SPI_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @addtogroup DDL_SPI
 * @{
 */

#if (DDL_SPI_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup SPI_Global_Types SPI Global Types
 * @{
 */

/**
 * @brief Structure definition of SPI initialization.
 */
typedef struct
{
    uint32_t u32WireMode;           /*!< SPI wire mode, 3 wire mode or 4 wire mode.
                                         This parameter can be a value of @ref SPI_Wire_Mode_Define */
    uint32_t u32TransMode;          /*!< SPI transfer mode, send only or full duplex.
                                         This parameter can be a value of @ref SPI_Transfer_Mode_Define */
    uint32_t u32MasterSlave;        /*!< SPI master/slave mode.
                                         This parameter can be a value of @ref SPI_Master_Slave_Mode_Define */
    uint32_t u32SuspMode;           /*!< SPI communication suspend function.
                                         This parameter can be a value of @ref SPI_Communication_Suspend_Function_Define */
    uint32_t u32Modfe;              /*!< SPI mode fault detect command.
                                         This parameter can be a value of @ref SPI_Mode_Fault_Dectet_Command_Define */
    uint32_t u32Parity;             /*!< SPI parity check selection.
                                         This parameter can be a value of @ref SPI_Parity_Check_Define */
    uint32_t u32SpiMode;            /*!< SPI mode.
                                         This parameter can be a value of @ref SPI_Mode_Define */
    uint32_t u32BaudRatePrescaler;  /*!< SPI baud rate prescaler.
                                         This parameter can be a value of @ref SPI_Baud_Rate_Prescaler_Define */
    uint32_t u32DataBits;           /*!< SPI data bits, 4 bits ~ 32 bits.
                                         This parameter can be a value of @ref SPI_Data_Size_Define */
    uint32_t u32FirstBit;           /*!< MSB first or LSB first.
                                         This parameter can be a value of @ref SPI_First_Bit_Define */
    uint32_t u32FrameLevel;         /*!< SPI frame level, SPI_FRAME_1 ~ SPI_FRAME_4.
                                         This parameter can be a value of @ref SPI_Frame_Level_Define */
} stc_spi_init_t;

/**
 * @brief Structure definition of SPI delay time configuration.
 */
typedef struct
{
    uint32_t u32IntervalDelay;      /*!< SPI interval time delay (Next access delay time)
                                         This parameter can be a value of @ref SPI_Interval_Delay_Time_define */
    uint32_t u32ReleaseDelay;       /*!< SPI release time delay (SCK invalid delay time)
                                         This parameter can be a value of @ref SPI_Release_Delay_Time_define */
    uint32_t u32SetupDelay;         /*!< SPI Setup time delay (SCK valid delay time) define
                                         This parameter can be a value of @ref SPI_Setup_Delay_Time_define */
} stc_spi_delay_t;

/**
 * @brief Structure definition of valid level for SS0/SS1/SS2/SS3 signal.
 */
typedef struct
{
    uint32_t u32ValidLevelSS0;      /*!< Valid level for SS0 signal @ref SPI_SS_Active_Level_Define */
    uint32_t u32ValidLevelSS1;      /*!< Valid level for SS1 signal @ref SPI_SS_Active_Level_Define */
    uint32_t u32ValidLevelSS2;      /*!< Valid level for SS2 signal @ref SPI_SS_Active_Level_Define */
    uint32_t u32ValidLevelSS3;      /*!< Valid level for SS3 signal @ref SPI_SS_Active_Level_Define */
} stc_spi_ss_valid_level_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup SPI_Global_Macros SPI Global Macros
 * @{
 */

/**
 * @defgroup SPI_Wire_Mode_Define SPI wire mode define
 * @{
 */
#define SPI_WIRE_4                  (0UL)
#define SPI_WIRE_3                  (SPI_CR1_SPIMDS)
/**
 * @}
 */

/**
 * @defgroup SPI_Transfer_Mode_Define SPI transfer mode define
 * @{
 */
#define SPI_FULL_DUPLEX             (0UL)               /*!< Full duplex. */
#define SPI_SEND_ONLY               (SPI_CR1_TXMDS)     /*!< Send only. */
/**
 * @}
 */

/**
 * @defgroup SPI_Master_Slave_Mode_Define SPI master slave mode define
 * @{
 */
#define SPI_SLAVE                   (0UL)
#define SPI_MASTER                  (SPI_CR1_MSTR)
/**
 * @}
 */

/**
 * @defgroup SPI_Loopback_Selection_Define SPI loopback selection define
 * @note Loopback mode is mainly used for parity self-diagnosis in 4-wire full-duplex mode.
 * @{
 */
#define SPI_SPLPBK_INVALID          (0UL)
#define SPI_SPLPBK_MOSI_INVERT      (SPI_CR1_SPLPBK)    /*!< MISO data is the inverse of the data output by MOSI. */
#define SPI_SPLPBK_MOSI             (SPI_CR1_SPLPBK2)   /*!< MISO data is the data output by MOSI. */
/**
 * @}
 */

/**
 * @defgroup SPI_Communication_Suspend_Function_Define SPI communication suspend function define
 * @{
 */
#define SPI_COM_SUSP_FUNC_OFF       (0UL)
#define SPI_COM_SUSP_FUNC_ON        (SPI_CR1_CSUSPE)
/**
 * @}
 */

/**
 * @defgroup SPI_Interrupt_Type_Define SPI interrupt type define
 * @{
 */
#define SPI_INT_ERROR               (SPI_CR1_EIE)       /*!< Including overload, underload and parity error. */
#define SPI_INT_TX_BUFFER_EMPTY     (SPI_CR1_TXIE)
#define SPI_INT_RX_BUFFER_FULL      (SPI_CR1_RXIE)
#define SPI_INT_IDLE                (SPI_CR1_IDIE)
/**
 * @}
 */

/**
 * @defgroup SPI_Mode_Fault_Dectet_Command_Define SPI mode fault dectect command define
 * @{
 */
#define SPI_MODFE_DISABLE           (0UL)               /*!< Disable mode fault detection. */
#define SPI_MODFE_ENABLE            (SPI_CR1_MODFE)     /*!< Enable mode fault detection. */
/**
 * @}
 */

/**
 * @defgroup SPI_Parity_Check_Error_Self_Diagnosis_Define SPI parity check error self diagnosis define
 * @{
 */
#define SPI_PATE_DISABLE            (0UL)               /*!< Disable self diagnosis of parity check. */
#define SPI_PATE_ENABLE             (SPI_CR1_PATE)      /*!< Enable self diagnosis of parity check. */
/**
 * @}
 */

/**
 * @defgroup SPI_Parity_Check_Define SPI parity check mode define
 * @{
 */
#define SPI_PARITY_INVALID          (0UL)                           /*!< Parity check invalid. */
#define SPI_PARITY_EVEN             (SPI_CR1_PAE)                   /*!< Parity check selection even parity. */
#define SPI_PARITY_ODD              (SPI_CR1_PAE | SPI_CR1_PAOE)    /*!< Parity check selection odd parity. */
/**
 * @}
 */

/**
 * @defgroup SPI_Interval_Delay_Time_define SPI interval time delay (Next access delay time) define
 * @{
 */
#define SPI_INTERVAL_TIME_1SCK_2PCLK1  (0UL)
#define SPI_INTERVAL_TIME_2SCK_2PCLK1  (SPI_CFG1_MIDI_0)
#define SPI_INTERVAL_TIME_3SCK_2PCLK1  (SPI_CFG1_MIDI_1)
#define SPI_INTERVAL_TIME_4SCK_2PCLK1  (SPI_CFG1_MIDI_1 | SPI_CFG1_MIDI_0)
#define SPI_INTERVAL_TIME_5SCK_2PCLK1  (SPI_CFG1_MIDI_2)
#define SPI_INTERVAL_TIME_6SCK_2PCLK1  (SPI_CFG1_MIDI_2 | SPI_CFG1_MIDI_0)
#define SPI_INTERVAL_TIME_7SCK_2PCLK1  (SPI_CFG1_MIDI_2 | SPI_CFG1_MIDI_1)
#define SPI_INTERVAL_TIME_8SCK_2PCLK1  (SPI_CFG1_MIDI_2 | SPI_CFG1_MIDI_1 | SPI_CFG1_MIDI_0)
/**
 * @}
 */

/**
 * @defgroup SPI_Release_Delay_Time_define SPI release time delay (SCK invalid delay time) define
 * @{
 */
#define SPI_RELEASE_TIME_1SCK         (0UL)
#define SPI_RELEASE_TIME_2SCK         (SPI_CFG1_MSSDL_0)
#define SPI_RELEASE_TIME_3SCK         (SPI_CFG1_MSSDL_1)
#define SPI_RELEASE_TIME_4SCK         (SPI_CFG1_MSSDL_1 | SPI_CFG1_MSSDL_0)
#define SPI_RELEASE_TIME_5SCK         (SPI_CFG1_MSSDL_2)
#define SPI_RELEASE_TIME_6SCK         (SPI_CFG1_MSSDL_2 | SPI_CFG1_MSSDL_0)
#define SPI_RELEASE_TIME_7SCK         (SPI_CFG1_MSSDL_2 | SPI_CFG1_MSSDL_1)
#define SPI_RELEASE_TIME_8SCK         (SPI_CFG1_MSSDL_2 | SPI_CFG1_MSSDL_1 | SPI_CFG1_MSSDL_0)
/**
 * @}
 */

/**
 * @defgroup SPI_Setup_Delay_Time_define SPI Setup time delay (SCK valid delay time) define
 * @{
 */
#define SPI_SETUP_TIME_1SCK         (0UL)
#define SPI_SETUP_TIME_2SCK         (SPI_CFG1_MSSI_0)
#define SPI_SETUP_TIME_3SCK         (SPI_CFG1_MSSI_1)
#define SPI_SETUP_TIME_4SCK         (SPI_CFG1_MSSI_1 | SPI_CFG1_MSSI_0)
#define SPI_SETUP_TIME_5SCK         (SPI_CFG1_MSSI_2)
#define SPI_SETUP_TIME_6SCK         (SPI_CFG1_MSSI_2 | SPI_CFG1_MSSI_0)
#define SPI_SETUP_TIME_7SCK         (SPI_CFG1_MSSI_2 | SPI_CFG1_MSSI_1)
#define SPI_SETUP_TIME_8SCK         (SPI_CFG1_MSSI_2 | SPI_CFG1_MSSI_1 | SPI_CFG1_MSSI_0)
/**
 * @}
 */

/**
 * @defgroup SPI_SS_Active_Level_Define SPI SSx Active Level define
 * @{
 */
#define SPI_SS_ACTIVE_LOW          (0UL)               /*!< SS pin active low. */
#define SPI_SS_ACTIVE_HIGH         (1UL)               /*!< SS pin active high. */
/**
 * @}
 */

/**
 * @defgroup SPI_Read_Target_Buffer_Define SPI read data register target buffer define
 * @{
 */
#define SPI_RD_TARGET_RD_BUF        (0UL)               /*!< Read read buffer. */
#define SPI_RD_TARGET_WR_BUF        (SPI_CFG1_SPRDTD)   /*!< Read write buffer. */
/**
 * @}
 */

/**
 * @defgroup SPI_Frame_Level_Define SPI data frame level define, The Data in the
 *                                   SPI_DR register will be send to TX_BUFF after
 *                                   enough data frame write to the SPI_DR
 * @{
 */
#define SPI_FRAME_1     (0UL)                               /*!< Data 1 frame */
#define SPI_FRAME_2     (SPI_CFG1_FTHLV_0)                  /*!< Data 2 frame.*/
#define SPI_FRAME_3     (SPI_CFG1_FTHLV_1)                  /*!< Data 3 frame.*/
#define SPI_FRAME_4     (SPI_CFG1_FTHLV_0 | SPI_CFG1_FTHLV_1) /*!< Data 4 frame.*/
/**
 * @}
 */

/**
 * @defgroup SPI_Mode_Define SPI Mode define
 * @{
 */
/* SCK pin output low in idle state; MOSI/MISO pin data valid in odd edge , MOSI/MISO pin data change in even edge */
#define SPI_MODE_0                  (0UL)
/* SCK pin output low in idle state; MOSI/MISO pin data valid in even edge , MOSI/MISO pin data change in odd edge */
#define SPI_MODE_1                  (SPI_CFG2_CPHA)
/* SCK pin output high in idle state; MOSI/MISO pin data valid in odd edge , MOSI/MISO pin data change in even edge */
#define SPI_MODE_2                  (SPI_CFG2_CPOL)
/* SCK pin output high in idle state; MOSI/MISO pin data valid in even edge , MOSI/MISO pin data change in odd edge */
#define SPI_MODE_3                  (SPI_CFG2_CPOL | SPI_CFG2_CPHA)
/**
 * @}
 */

/**
 * @defgroup SPI_SS_Singal_Valid_Define SPI SS signal valid define
 * @{
 */
#define SPI_SS0_VALID               (0UL)
#define SPI_SS1_VALID               (SPI_CFG2_SSA_0)
#define SPI_SS2_VALID               (SPI_CFG2_SSA_1)
#define SPI_SS3_VALID               (SPI_CFG2_SSA_0 | SPI_CFG2_SSA_1)
/**
 * @}
 */

/**
 * @defgroup SPI_Baud_Rate_Prescaler_Define SPI baudrate prescaler define
 * @{
 */
#define SPI_BR_DIV_2                (0UL)                               /*!< SPI baud rate is the pclk1 divided by 2. */
#define SPI_BR_DIV_4                (SPI_CFG2_MBR_0)                    /*!< SPI baud rate is the pclk1 clock divided by 4. */
#define SPI_BR_DIV_8                (SPI_CFG2_MBR_1)                    /*!< SPI baud rate is the pclk1 clock divided by 8. */
#define SPI_BR_DIV_16               (SPI_CFG2_MBR_1 | SPI_CFG2_MBR_0)   /*!< SPI baud rate is the pclk1 clock divided by 16. */
#define SPI_BR_DIV_32               (SPI_CFG2_MBR_2)                    /*!< SPI baud rate is the pclk1 clock divided by 32. */
#define SPI_BR_DIV_64               (SPI_CFG2_MBR_2 | SPI_CFG2_MBR_0)   /*!< SPI baud rate is the pclk1 clock divided by 64. */
#define SPI_BR_DIV_128              (SPI_CFG2_MBR_2 | SPI_CFG2_MBR_1)   /*!< SPI baud rate is the pclk1 clock divided by 128. */
#define SPI_BR_DIV_256              (SPI_CFG2_MBR_2 | SPI_CFG2_MBR_1 | SPI_CFG2_MBR_0)  /*!< SPI baud rate is the pclk1 divided by 256. */
/**
 * @}
 */

/**
 * @defgroup SPI_Data_Size_Define SPI data size define
 * @{
 */
#define SPI_DATA_SIZE_4BIT          (0UL)
#define SPI_DATA_SIZE_5BIT          (SPI_CFG2_DSIZE_0)
#define SPI_DATA_SIZE_6BIT          (SPI_CFG2_DSIZE_1)
#define SPI_DATA_SIZE_7BIT          (SPI_CFG2_DSIZE_0 | SPI_CFG2_DSIZE_1)
#define SPI_DATA_SIZE_8BIT          (SPI_CFG2_DSIZE_2)
#define SPI_DATA_SIZE_9BIT          (SPI_CFG2_DSIZE_2 | SPI_CFG2_DSIZE_0)
#define SPI_DATA_SIZE_10BIT         (SPI_CFG2_DSIZE_2 | SPI_CFG2_DSIZE_1)
#define SPI_DATA_SIZE_11BIT         (SPI_CFG2_DSIZE_2 | SPI_CFG2_DSIZE_1 | SPI_CFG2_DSIZE_0)
#define SPI_DATA_SIZE_12BIT         (SPI_CFG2_DSIZE_3)
#define SPI_DATA_SIZE_13BIT         (SPI_CFG2_DSIZE_3 | SPI_CFG2_DSIZE_0)
#define SPI_DATA_SIZE_14BIT         (SPI_CFG2_DSIZE_3 | SPI_CFG2_DSIZE_1)
#define SPI_DATA_SIZE_15BIT         (SPI_CFG2_DSIZE_3 | SPI_CFG2_DSIZE_1 | SPI_CFG2_DSIZE_0)
#define SPI_DATA_SIZE_16BIT         (SPI_CFG2_DSIZE_3 | SPI_CFG2_DSIZE_2)
#define SPI_DATA_SIZE_20BIT         (SPI_CFG2_DSIZE_3 | SPI_CFG2_DSIZE_2 | SPI_CFG2_DSIZE_0)
#define SPI_DATA_SIZE_24BIT         (SPI_CFG2_DSIZE_3 | SPI_CFG2_DSIZE_2 | SPI_CFG2_DSIZE_1)
#define SPI_DATA_SIZE_32BIT         (SPI_CFG2_DSIZE_3 | SPI_CFG2_DSIZE_2 | SPI_CFG2_DSIZE_1 | SPI_CFG2_DSIZE_0)
/**
 * @}
 */

/**
 * @defgroup SPI_First_Bit_Define SPI first bit define
 * @{
 */
#define SPI_FIRST_MSB               (0UL)
#define SPI_FIRST_LSB               (SPI_CFG2_LSBF)
/**
 * @}
 */

/**
 * @defgroup SPI_State_Flag_Define SPI state flag define
 * @{
 */
#define SPI_FLAG_OVERLOAD           (SPI_SR_OVRERF)
#define SPI_FLAG_IDLE               (SPI_SR_IDLNF)
#define SPI_FLAG_MODE_FAULT         (SPI_SR_MODFERF)
#define SPI_FLAG_PARITY_ERROR       (SPI_SR_PERF)
#define SPI_FLAG_UNDERLOAD          (SPI_SR_UDRERF)
#define SPI_FLAG_TX_BUFFER_EMPTY    (SPI_SR_TDEF)       /*!< This flag is set when the data in the data register     \
                                                             is copied into the shift register, but the transmission \
                                                             of the data bit may not have been completed. */
#define SPI_FLAG_RX_BUFFER_FULL     (SPI_SR_RDFF)       /*!< When this flag is set, it indicates that a data was received. */

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup SPI_Global_Functions
 * @{
 */
en_result_t SPI_StructInit(stc_spi_init_t *pstcInit);
en_result_t SPI_DelayStructInit(stc_spi_delay_t *pstcDelayCfg);
en_result_t SPI_SSValidLevelStructInit(stc_spi_ss_valid_level_t *pstcSSValidLevelCfg);

en_result_t SPI_Init(M4_SPI_TypeDef *SPIx, const stc_spi_init_t *pstcInit);
void SPI_DeInit(M4_SPI_TypeDef *SPIx);

void SPI_IntCmd(M4_SPI_TypeDef *SPIx, uint32_t u32IntType, en_functional_state_t enNewState);
void SPI_FunctionCmd(M4_SPI_TypeDef *SPIx, en_functional_state_t enNewState);

void SPI_WriteDataReg(M4_SPI_TypeDef *SPIx, uint32_t u32Data);
uint32_t SPI_ReadDataReg(const M4_SPI_TypeDef *SPIx);

en_flag_status_t SPI_GetStatus(const M4_SPI_TypeDef *SPIx, uint32_t u32Flag);
void SPI_ClearFlag(M4_SPI_TypeDef *SPIx, uint32_t u32Flag);

void SPI_ValidSSCfg(M4_SPI_TypeDef *SPIx, uint32_t u32ValidSS);
void SPI_LoopbackModeCfg(M4_SPI_TypeDef *SPIx, uint32_t u32Mode);
void SPI_PateCmd(M4_SPI_TypeDef *SPIx, en_functional_state_t enNewState);
en_result_t SPI_DelayTimeCfg(M4_SPI_TypeDef *SPIx, const stc_spi_delay_t *pstcDelayCfg);
en_result_t SPI_SSValidLevelCfg(M4_SPI_TypeDef *SPIx, const stc_spi_ss_valid_level_t *pstcSSValidLevelCfg);
void SPI_ReadBufCfg(M4_SPI_TypeDef *SPIx, uint32_t u32ReadBuf);

en_result_t SPI_Transmit(M4_SPI_TypeDef *SPIx, const void *pvTxBuf, uint32_t u32TxLength);
en_result_t SPI_Receive(M4_SPI_TypeDef *SPIx, void *pvRxBuf, uint32_t u32RxLength);
en_result_t SPI_TransmitReceive(M4_SPI_TypeDef *SPIx, const void *pvTxBuf, void *pvRxBuf, uint32_t u32Length);

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

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_SPI_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
