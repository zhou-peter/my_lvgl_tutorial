/**
 *******************************************************************************
 * @file    hc32f4a0_icg.h
 * @brief   This file contains all the Macro Definitions of the ICG driver
 *          library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-01-15       Yangjp          First version
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
#ifndef __HC32F4A0_ICG_H__
#define __HC32F4A0_ICG_H__

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
 * @addtogroup DDL_ICG
 * @{
 */

#if (DDL_ICG_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup ICG_Global_Macros ICG Global Macros
 * @{
 */

/**
 * @defgroup ICG_SWDT_Reset_State ICG SWDT Reset State
 * @{
 */
#define ICG_SWDT_RESET_AUTOSTART                (0x00000000UL)        /*!< SWDT Auto Start after reset */
#define ICG_SWDT_RESET_STOP                     (ICG_ICG0_SWDTAUTS)   /*!< SWDT stop after reset       */
/**
 * @}
 */

/**
 * @defgroup ICG_SWDT_Trigger_Type ICG SWDT Trigger Type
 * @{
 */
#define ICG_SWDT_TRIG_INT                       (0x00000000UL)        /*!< SWDT trigger interrupt */
#define ICG_SWDT_TRIG_RESET                     (ICG_ICG0_SWDTITS)    /*!< SWDT trigger reset     */
/**
 * @}
 */

/**
 * @defgroup ICG_SWDT_Counter_Cycle ICG SWDT Counter Cycle
 * @{
 */
#define ICG_SWDT_COUNTER_CYCLE_256              (0x00000000UL)          /*!< 256 clock cycle   */
#define ICG_SWDT_COUNTER_CYCLE_4096             (ICG_ICG0_SWDTPERI_0)   /*!< 4096 clock cycle  */
#define ICG_SWDT_COUNTER_CYCLE_16384            (ICG_ICG0_SWDTPERI_1)   /*!< 16384 clock cycle */
#define ICG_SWDT_COUNTER_CYCLE_65536            (ICG_ICG0_SWDTPERI)     /*!< 65536 clock cycle */
/**
 * @}
 */

/**
 * @defgroup ICG_SWDT_Clock_Division ICG SWDT Clock Division
 * @{
 */
#define ICG_SWDT_CLOCK_DIV1                     (0x00000000UL)                                                  /*!< SWDTCLK      */
#define ICG_SWDT_CLOCK_DIV16                    (ICG_ICG0_SWDTCKS_2)                                            /*!< SWDTCLK/16   */
#define ICG_SWDT_CLOCK_DIV32                    (ICG_ICG0_SWDTCKS_2 | ICG_ICG0_SWDTCKS_0)                       /*!< SWDTCLK/32   */
#define ICG_SWDT_CLOCK_DIV64                    (ICG_ICG0_SWDTCKS_2 | ICG_ICG0_SWDTCKS_1)                       /*!< SWDTCLK/64   */
#define ICG_SWDT_CLOCK_DIV128                   (ICG_ICG0_SWDTCKS_2 | ICG_ICG0_SWDTCKS_1 | ICG_ICG0_SWDTCKS_0)  /*!< SWDTCLK/128  */
#define ICG_SWDT_CLOCK_DIV256                   (ICG_ICG0_SWDTCKS_3)                                            /*!< SWDTCLK/256  */
#define ICG_SWDT_CLOCK_DIV2048                  (ICG_ICG0_SWDTCKS_3 | ICG_ICG0_SWDTCKS_1 | ICG_ICG0_SWDTCKS_0)  /*!< SWDTCLK/2048 */
/**
 * @}
 */

/**
 * @defgroup ICG_SWDT_Refresh_Range ICG SWDT Refresh Range
 * @{
 */
#define ICG_SWDT_RANGE_100PCT                   (0x00000000UL)                                                      /*!< 100%               */
#define ICG_SWDT_RANGE_0TO25PCT                 (ICG_ICG0_SWDTWDPT_0)                                               /*!< 0%~25%             */
#define ICG_SWDT_RANGE_25TO50PCT                (ICG_ICG0_SWDTWDPT_1)                                               /*!< 25%~50%            */
#define ICG_SWDT_RANGE_0TO50PCT                 (ICG_ICG0_SWDTWDPT_1 | ICG_ICG0_SWDTWDPT_0))                        /*!< 0%~50%             */
#define ICG_SWDT_RANGE_50TO75PCT                (ICG_ICG0_SWDTWDPT_2)                                               /*!< 50%~75%            */
#define ICG_SWDT_RANGE_0TO25PCT_50TO75PCT       (ICG_ICG0_SWDTWDPT_2 | ICG_ICG0_SWDTWDPT_0))                        /*!< 0%~25% & 50%~75%   */
#define ICG_SWDT_RANGE_25TO75PCT                (ICG_ICG0_SWDTWDPT_2 | ICG_ICG0_SWDTWDPT_1))                        /*!< 25%~75%            */
#define ICG_SWDT_RANGE_0TO75PCT                 (ICG_ICG0_SWDTWDPT_2 | ICG_ICG0_SWDTWDPT_1 | ICG_ICG0_SWDTWDPT_0))  /*!< 0%~75%             */
#define ICG_SWDT_RANGE_75TO100PCT               (ICG_ICG0_SWDTWDPT_3)                                               /*!< 75%~100%           */
#define ICG_SWDT_RANGE_0TO25PCT_75TO100PCT      (ICG_ICG0_SWDTWDPT_3 | ICG_ICG0_SWDTWDPT_0))                        /*!< 0%~25% & 75%~100%  */
#define ICG_SWDT_RANGE_25TO50PCT_75TO100PCT     (ICG_ICG0_SWDTWDPT_3 | ICG_ICG0_SWDTWDPT_1))                        /*!< 25%~50% & 75%~100% */
#define ICG_SWDT_RANGE_0TO50PCT_75TO100PCT      (ICG_ICG0_SWDTWDPT_3 | ICG_ICG0_SWDTWDPT_1 | ICG_ICG0_SWDTWDPT_0))  /*!< 0%~50% & 75%~100%  */
#define ICG_SWDT_RANGE_50TO100PCT               (ICG_ICG0_SWDTWDPT_3 | ICG_ICG0_SWDTWDPT_2))                        /*!< 50%~100%           */
#define ICG_SWDT_RANGE_0TO25PCT_50TO100PCT      (ICG_ICG0_SWDTWDPT_3 | ICG_ICG0_SWDTWDPT_2 | ICG_ICG0_SWDTWDPT_0))  /*!< 0%~25% & 50%~100%  */
#define ICG_SWDT_RANGE_25TO100PCT               (ICG_ICG0_SWDTWDPT_3 | ICG_ICG0_SWDTWDPT_2 | ICG_ICG0_SWDTWDPT_1))  /*!< 25%~100%           */
#define ICG_SWDT_RANGE_0TO100PCT                (ICG_ICG0_SWDTWDPT)                                                 /*!< 0%~100%            */
/**
 * @}
 */

/**
 * @defgroup ICG_SWDT_LPM_Count ICG SWDT Low Power Mode Count
 * @brief SWDT count control in the sleep/stop mode
 * @{
 */
#define ICG_SWDT_LPM_COUNT_CONTINUE             (0x00000000UL)          /*!< SWDT count continue in the sleep/stop mode */
#define ICG_SWDT_LPM_COUNT_STOP                 (ICG_ICG0_SWDTSLPOFF)   /*!< SWDT count stop in the sleep/stop mode     */
/**
 * @}
 */

/**
 * @defgroup ICG_WDT_Reset_State ICG WDT Reset State
 * @{
 */
#define ICG_WDT_RESET_AUTOSTART                 (0x00000000UL)      /*!< WDT Auto Start after reset */
#define ICG_WDT_RESET_STOP                      (ICG_ICG0_WDTAUTS)  /*!< WDT stop after reset       */
/**
 * @}
 */

/**
 * @defgroup ICG_WDT_Trigger_Type ICG WDT Trigger Type
 * @{
 */
#define ICG_WDT_TRIG_INT                        (0x00000000UL)      /*!< WDT trigger interrupt */
#define ICG_WDT_TRIG_RESET                      (ICG_ICG0_WDTITS)   /*!< WDT trigger reset     */
/**
 * @}
 */

/**
 * @defgroup ICG_WDT_Counter_Cycle ICG WDT Counter Cycle
 * @{
 */
#define ICG_WDT_COUNTER_CYCLE_256               (0x00000000UL)        /*!< 256 clock cycle   */
#define ICG_WDT_COUNTER_CYCLE_4096              (ICG_ICG0_WDTPERI_0)  /*!< 4096 clock cycle  */
#define ICG_WDT_COUNTER_CYCLE_16384             (ICG_ICG0_WDTPERI_1)  /*!< 16384 clock cycle */
#define ICG_WDT_COUNTER_CYCLE_65536             (ICG_ICG0_WDTPERI)    /*!< 65536 clock cycle */
/**
 * @}
 */

/**
 * @defgroup ICG_WDT_Clock_Division ICG WDT Clock Division
 * @{
 */
#define ICG_WDT_CLOCK_DIV4                      (CG_ICG0_WDTCKS_1)                                            /*!< PLCK3/4    */
#define ICG_WDT_CLOCK_DIV64                     (ICG_ICG0_WDTCKS_2 | ICG_ICG0_WDTCKS_1)                       /*!< PLCK3/64   */
#define ICG_WDT_CLOCK_DIV128                    (ICG_ICG0_WDTCKS_2 | ICG_ICG0_WDTCKS_1 | ICG_ICG0_WDTCKS_0)   /*!< PLCK3/128  */
#define ICG_WDT_CLOCK_DIV256                    (CG_ICG0_WDTCKS_3)                                            /*!< PLCK3/256  */
#define ICG_WDT_CLOCK_DIV512                    (ICG_ICG0_WDTCKS_3 | ICG_ICG0_WDTCKS_0)                       /*!< PLCK3/512  */
#define ICG_WDT_CLOCK_DIV1024                   (ICG_ICG0_WDTCKS_3 | ICG_ICG0_WDTCKS_1)                       /*!< PLCK3/1024 */
#define ICG_WDT_CLOCK_DIV2028                   (ICG_ICG0_WDTCKS_3 | ICG_ICG0_WDTCKS_1 | ICG_ICG0_WDTCKS_0)   /*!< PLCK3/2048 */
#define ICG_WDT_CLOCK_DIV8192                   (ICG_ICG0_WDTCKS_3 | ICG_ICG0_WDTCKS_2 | ICG_ICG0_WDTCKS_0)   /*!< PLCK3/8192 */
/**
 * @}
 */

/**
 * @defgroup ICG_WDT_Refresh_Range ICG WDT Refresh Range
 * @{
 */
#define ICG_WDT_RANGE_100PCT                    (0x00000000UL)                                                  /*!< 100%               */
#define ICG_WDT_RANGE_0TO25PCT                  (ICG_ICG0_WDTWDPT_0)                                            /*!< 0%~25%             */
#define ICG_WDT_RANGE_25TO50PCT                 (ICG_ICG0_WDTWDPT_1)                                            /*!< 25%~50%            */
#define ICG_WDT_RANGE_0TO50PCT                  (ICG_ICG0_WDTWDPT_1 | ICG_ICG0_WDTWDPT_0)                       /*!< 0%~50%             */
#define ICG_WDT_RANGE_50TO75PCT                 (ICG_ICG0_WDTWDPT_2)                                            /*!< 50%~75%            */
#define ICG_WDT_RANGE_0TO25PCT_50TO75PCT        (ICG_ICG0_WDTWDPT_2 | ICG_ICG0_WDTWDPT_0)                       /*!< 0%~25% & 50%~75%   */
#define ICG_WDT_RANGE_25TO75PCT                 (ICG_ICG0_WDTWDPT_2 | ICG_ICG0_WDTWDPT_1)                       /*!< 25%~75%            */
#define ICG_WDT_RANGE_0TO75PCT                  (ICG_ICG0_WDTWDPT_2 | ICG_ICG0_WDTWDPT_1 | ICG_ICG0_WDTWDPT_0)  /*!< 0%~75%             */
#define ICG_WDT_RANGE_75TO100PCT                (ICG_ICG0_WDTWDPT_3)                                            /*!< 75%~100%           */
#define ICG_WDT_RANGE_0TO25PCT_75TO100PCT       (ICG_ICG0_WDTWDPT_3 | ICG_ICG0_WDTWDPT_0)                       /*!< 0%~25% & 75%~100%  */
#define ICG_WDT_RANGE_25TO50PCT_75TO100PCT      (ICG_ICG0_WDTWDPT_3 | ICG_ICG0_WDTWDPT_1)                       /*!< 25%~50% & 75%~100% */
#define ICG_WDT_RANGE_0TO50PCT_75TO100PCT       (ICG_ICG0_WDTWDPT_3 | ICG_ICG0_WDTWDPT_1 | ICG_ICG0_WDTWDPT_0)  /*!< 0%~50% & 75%~100%  */
#define ICG_WDT_RANGE_50TO100PCT                (ICG_ICG0_WDTWDPT_3 | ICG_ICG0_WDTWDPT_2)                       /*!< 50%~100%           */
#define ICG_WDT_RANGE_0TO25PCT_50TO100PCT       (ICG_ICG0_WDTWDPT_3 | ICG_ICG0_WDTWDPT_2 | ICG_ICG0_WDTWDPT_0)  /*!< 0%~25% & 50%~100%  */
#define ICG_WDT_RANGE_25TO100PCT                (ICG_ICG0_WDTWDPT_3 | ICG_ICG0_WDTWDPT_2 | ICG_ICG0_WDTWDPT_1)  /*!< 25%~100%           */
#define ICG_WDT_RANGE_0TO100PCT                 (ICG_ICG0_WDTWDPT)                                              /*!< 0%~100%            */
/**
 * @}
 */

/**
 * @defgroup ICG_WDT_LPM_Count ICG WDT Low Power Mode Count
 * @brief WDT count control in the sleep mode
 * @{
 */
#define ICG_WDT_LPM_COUNT_CONTINUE              (0x00000000UL)        /*!< WDT count continue in the sleep mode */
#define ICG_WDT_LPM_COUNT_STOP                  (ICG_ICG0_WDTSLPOFF)  /*!< WDT count stop in the sleep mode     */
/**
 * @}
 */

/**
 * @defgroup ICG_HRC_Frequency_Select ICG HRC Frequency Select
 * @{
 */
#define ICG_HRC_FREQ_20MHZ                      (0x00000000UL)          /*!< HRC frequency 20MHZ */
#define ICG_HRC_FREQ_16MHZ                      (ICG_ICG1_HRCFREQSEL)   /*!< HRC frequency 16MHZ */
/**
 * @}
 */

/**
 * @defgroup ICG_HRC_Reset_State ICG HRC Reset State
 * @{
 */
#define ICG_HRC_RESET_OSCILLATION               (0x00000000UL)      /*!< HRC Oscillation after reset */
#define ICG_HRC_RESET_STOP                      (ICG_ICG1_HRCSTOP)  /*!< HRC stop after reset        */
/**
 * @}
 */

/**
 * @defgroup ICG_BOR_Voltage_Threshold ICG BOR Voltage Threshold
 * @{
 */
#define ICG_BOR_VOL_THRESHOLD_1P9               (0x00000000UL)        /*!< BOR voltage threshold 1.9V */
#define ICG_BOR_VOL_THRESHOLD_2P0               (ICG_ICG1_BOR_LEV_0)  /*!< BOR voltage threshold 2.0V */
#define ICG_BOR_VOL_THRESHOLD_2P1               (ICG_ICG1_BOR_LEV_1)  /*!< BOR voltage threshold 2.1V */
#define ICG_BOR_VOL_THRESHOLD_2P3               (ICG_ICG1_BOR_LEV)    /*!< BOR voltage threshold 2.3V */
/**
 * @}
 */

/**
 * @defgroup ICG_BOR_Reset_State ICG BOR Reset State
 * @{
 */
#define ICG_BOR_RESET_ENABLE                    (0x00000000UL)      /*!< Enable BOR voltage detection after reset  */
#define ICG_BOR_RESET_DISABLE                   (ICG_ICG1_BORDIS)   /*!< Disable BOR voltage detection after reset */
/**
 * @}
 */

/**
 * @defgroup ICG_FLASH_Bank_Reset_State ICG FLASH Bank Reset State
 * @{
 */
#define ICG_FLASH_BANK_1                        (0xFFFFFFFFUL)  /*!< After reset, 1MBytes flash are provided by bank0                              */
#define ICG_FLASH_BANK_12                       (0x004B4B4BUL)  /*!< After reset, 1MBytes flash are provided by bank0 and bank1 with 512Bytes each */
/**
 * @}
 */

/**
 * @defgroup ICG_FLASH_Protect_Reset_State ICG FLASH Protect Reset State
 * @brief Enable or disable D-BUS read protection for addresses 0x00000000 - 0x0001FFFF
 * @{
 */
#define ICG_FLASH_PROTECT_RESET_ENABLE          (0x00004450UL)  /*!< Enable D-BUS read protection after reset  */
#define ICG_FLASH_PROTECT_RESET_DISABLE         (0xFFFFFFFFUL)  /*!< Disable D-BUS read protection after reset */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @defgroup ICG_Register_Configuration ICG Register Configuration
 * @{
 */

/**
 *******************************************************************************
 * @defgroup ICG_SWDT_Preload_Configuration ICG SWDT Preload Configuration
 * @{
 *******************************************************************************
 */
/* SWDT register config */
#define ICG0_RB_SWDT_AUTS                       (ICG_SWDT_RESET_STOP)
#define ICG0_RB_SWDT_ITS                        (ICG_SWDT_TRIG_INT)
#define ICG0_RB_SWDT_PERI                       (ICG_SWDT_COUNTER_CYCLE_16384)
#define ICG0_RB_SWDT_CKS                        (ICG_SWDT_CLOCK_DIV2048)
#define ICG0_RB_SWDT_WDPT                       (ICG_SWDT_RANGE_100PCT)
#define ICG0_RB_SWDT_SLTPOFF                    (ICG_SWDT_LPM_COUNT_CONTINUE)

/* SWDT register value */
#define ICG0_RB_SWDT_CONFIG                     (ICG0_RB_SWDT_AUTS | ICG0_RB_SWDT_ITS | \
                                                 ICG0_RB_SWDT_PERI | ICG0_RB_SWDT_CKS | \
                                                 ICG0_RB_SWDT_WDPT | ICG0_RB_SWDT_SLTPOFF)
/**
 * @}
 */

/**
 *******************************************************************************
 * @defgroup ICG_WDT_Preload_Configuration ICG WDT Preload Configuration
 * @{
 *******************************************************************************
 */
/* WDT register config */
#define ICG0_RB_WDT_AUTS                        (ICG_WDT_RESET_STOP)
#define ICG0_RB_WDT_ITS                         (ICG_WDT_TRIG_INT)
#define ICG0_RB_WDT_PERI                        (ICG_WDT_COUNTER_CYCLE_16384)
#define ICG0_RB_WDT_CKS                         (ICG_WDT_CLOCK_DIV8192)
#define ICG0_RB_WDT_WDPT                        (ICG_WDT_RANGE_100PCT)
#define ICG0_RB_WDT_SLTPOFF                     (ICG_WDT_LPM_COUNT_CONTINUE)

/* WDT register value */
#define ICG0_RB_WDT_CONFIG                      (ICG0_RB_WDT_AUTS | ICG0_RB_WDT_ITS | \
                                                 ICG0_RB_WDT_PERI | ICG0_RB_WDT_CKS | \
                                                 ICG0_RB_WDT_WDPT | ICG0_RB_WDT_SLTPOFF)
/**
 * @}
 */

/**
 *******************************************************************************
 * @defgroup ICG_HRC_Preload_Configuration ICG HRC Preload Configuration
 * @{
 *******************************************************************************
 */
/* HRC register config */
#define ICG1_RB_HRC_FREQSEL                     (ICG_HRC_FREQ_16MHZ)
#define ICG1_RB_HRC_STOP                        (ICG_HRC_RESET_STOP)

/* HRC register value */
#define ICG1_RB_HRC_CONFIG                      (ICG1_RB_HRC_FREQSEL | ICG1_RB_HRC_STOP)
/**
 * @}
 */

/**
 *******************************************************************************
 * @defgroup ICG_BOR_Preload_Configuration ICG BOR Preload Configuration
 * @{
 *******************************************************************************
 */
/* BOR register config */
#define ICG1_RB_BOR_LEV                         (ICG_BOR_VOL_THRESHOLD_2P3)
#define ICG1_RB_BOR_DIS                         (ICG_BOR_RESET_DISABLE)

/* BOR register value */
#define ICG1_RB_BOR_CONFIG                      (ICG1_RB_BOR_LEV | ICG1_RB_BOR_DIS)
/**
 * @}
 */

/**
 *******************************************************************************
 * @defgroup ICG_FLASH_Bank_Preload_Configuration ICG FLASH Bank Preload Configuration
 * @{
 *******************************************************************************
 */
/* FLASH Bank register value */
#define ICG2_RB_FLASH_BANK_CONFIG               (ICG_FLASH_BANK_1)
/**
 * @}
 */

/**
 *******************************************************************************
 * @defgroup ICG_FLASH_Protect_Preload_Configuration ICG FLASH Protect Preload Configuration
 * @{
 *******************************************************************************
 */
/* FLASH Read Protect register value */
#define ICG3_RB_FLASH_PROTECT_CONFIG            (ICG_FLASH_PROTECT_RESET_DISABLE)
/**
 * @}
 */

/**
 * @}
 */

/**
 * @defgroup ICG_Register_Value ICG Register Value
 * @{
 */
/* ICG register value */
#define ICG_REG_CFG0_CONSTANT                   (ICG0_RB_WDT_CONFIG | ICG0_RB_SWDT_CONFIG)
#define ICG_REG_CFG1_CONSTANT                   (ICG1_RB_BOR_CONFIG | ICG1_RB_HRC_CONFIG)
#define ICG_REG_CFG2_CONSTANT                   (ICG2_RB_FLASH_BANK_CONFIG)
#define ICG_REG_CFG3_CONSTANT                   (ICG3_RB_FLASH_PROTECT_CONFIG)
/* ICG reserved value */
#define ICG_RESERVED_CONSTANT                   (0xFFFFFFFFUL)

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/

#endif /* DDL_ICG_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_ICG_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/