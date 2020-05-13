/******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
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
 */
/******************************************************************************/
/** \file usb_bsp_template.c
 **
 ** \brief  This file is responsible to offer board support package and is
 **         configurable by user.
 **
 **   - 2018-12-25  1.0  Wangmin First version for USB custom hid device demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_bsp.h"
#include "hc32_ddl.h"

//#define M020_EVB_BD  1UL

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* KEY0 */
#define  KEY1_PORT       (GPIO_PORT_A)
#define  KEY1_PIN        (GPIO_PIN_00)
#define  KEY1_EXINT_CH   (EXINT_CH00)
#define  KEY1_INT_SRC    (INT_PORT_EIRQ0)
#define  KEY1_IRQn       (Int003_IRQn)

#ifdef USB_OTG_FS_CORE

/* USBF Core*/
#define USB_DP_PORT     (GPIO_PORT_A)
#define USB_DP_PIN      (GPIO_PIN_12)
#define USB_DM_PORT     (GPIO_PORT_A)
#define USB_DM_PIN      (GPIO_PIN_11)
#define USB_VBUS_PORT   (GPIO_PORT_A)
#define USB_VBUS_PIN    (GPIO_PIN_09)
#define USB_SOF_PORT    (GPIO_PORT_A)
#define USB_SOF_PIN     (GPIO_PIN_08)

#else

#ifdef USE_EMBEDDED_PHY
/* USBF Core, embedded PHY */
#define USB_DP_PORT     (GPIO_PORT_B)
#define USB_DP_PIN      (GPIO_PIN_15)
#define USB_DM_PORT     (GPIO_PORT_B)
#define USB_DM_PIN      (GPIO_PIN_14)
#define USB_VBUS_PORT   (GPIO_PORT_B)
#define USB_VBUS_PIN    (GPIO_PIN_13)
#define USB_SOF_PORT    (GPIO_PORT_A)
#define USB_SOF_PIN     (GPIO_PIN_04)

#else

#ifndef M020_EVB_BD
/* ULPI */
#define USBH_ULPI_CLK_PORT      (GPIO_PORT_E)
#define USBH_ULPI_CLK_PIN       (GPIO_PIN_12)
#define USBH_ULPI_DIR_PORT      (GPIO_PORT_C)
#define USBH_ULPI_DIR_PIN       (GPIO_PIN_02)
#define USBH_ULPI_NXT_PORT      (GPIO_PORT_C)
#define USBH_ULPI_NXT_PIN       (GPIO_PIN_03)
#define USBH_ULPI_STP_PORT      (GPIO_PORT_C)
#define USBH_ULPI_STP_PIN       (GPIO_PIN_00)
#define USBH_ULPI_D0_PORT       (GPIO_PORT_E)
#define USBH_ULPI_D0_PIN        (GPIO_PIN_13)
#define USBH_ULPI_D1_PORT       (GPIO_PORT_E)
#define USBH_ULPI_D1_PIN        (GPIO_PIN_14)
#define USBH_ULPI_D2_PORT       (GPIO_PORT_E)
#define USBH_ULPI_D2_PIN        (GPIO_PIN_15)
#define USBH_ULPI_D3_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D3_PIN        (GPIO_PIN_10)
#define USBH_ULPI_D4_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D4_PIN        (GPIO_PIN_11)
#define USBH_ULPI_D5_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D5_PIN        (GPIO_PIN_12)
#define USBH_ULPI_D6_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D6_PIN        (GPIO_PIN_13)
#define USBH_ULPI_D7_PORT       (GPIO_PORT_E)
#define USBH_ULPI_D7_PIN        (GPIO_PIN_11)
/* 3300 reset */
#define USB_3300_RESET_PORT     (EIO_PORT1)
#define USB_3300_RESET_PIN      (EIO_USB3300_RST)
#else

/* Define for M020 EVB board */
#define USBH_ULPI_CLK_PORT      (GPIO_PORT_A)
#define USBH_ULPI_CLK_PIN       (GPIO_PIN_05)
#define USBH_ULPI_DIR_PORT      (GPIO_PORT_C)
#define USBH_ULPI_DIR_PIN       (GPIO_PIN_02)
#define USBH_ULPI_NXT_PORT      (GPIO_PORT_C)
#define USBH_ULPI_NXT_PIN       (GPIO_PIN_03)
#define USBH_ULPI_STP_PORT      (GPIO_PORT_C)
#define USBH_ULPI_STP_PIN       (GPIO_PIN_00)
#define USBH_ULPI_D0_PORT       (GPIO_PORT_A)
#define USBH_ULPI_D0_PIN        (GPIO_PIN_03)
#define USBH_ULPI_D1_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D1_PIN        (GPIO_PIN_00)
#define USBH_ULPI_D2_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D2_PIN        (GPIO_PIN_01)
#define USBH_ULPI_D3_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D3_PIN        (GPIO_PIN_10)
#define USBH_ULPI_D4_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D4_PIN        (GPIO_PIN_11)
#define USBH_ULPI_D5_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D5_PIN        (GPIO_PIN_12)
#define USBH_ULPI_D6_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D6_PIN        (GPIO_PIN_13)
#define USBH_ULPI_D7_PORT       (GPIO_PORT_B)
#define USBH_ULPI_D7_PIN        (GPIO_PIN_05)
/* 3300 reset */
#define USB_3300_RESET_PORT     (GPIO_PORT_A)
#define USB_3300_RESET_PIN      (GPIO_PIN_00)
#endif


#endif

#endif
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint8_t PrevXferDone = 1u;
uint8_t Send_Buffer[2];

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
extern  USB_OTG_CORE_HANDLE      USB_OTG_dev;
extern  uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);

/**
 ******************************************************************************
 ** \brief  Usb interrupt handle
 **
 ** \param  None
 **
 ** \return None
 ******************************************************************************/
void USB_IRQ_Handler(void)
{
    USBD_OTG_ISR_Handler(&USB_OTG_dev);
}

/**
 *******************************************************************************
 ** \brief KEY1_IrqCallback callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void KEY1_IrqCallback(void)
{
    if (Set == EXINT_GetExIntSrc(KEY1_EXINT_CH))
    {
        if ((PrevXferDone) && (USB_OTG_dev.dev.device_status == USB_OTG_CONFIGURED))
        {
            Send_Buf[0] = KEY_REPORT_ID;

            if(Pin_Reset == GPIO_ReadInputPortPin(KEY1_PORT, KEY1_PIN))
            {
                Send_Buf[1] = 0x01u;
            }
            else
            {
                Send_Buf[1] = 0x00u;
            }

            USBD_CUSTOM_HID_SendReport(&USB_OTG_dev, Send_Buf, 2u);
            PrevXferDone = 0u;
        }
    }
}

static void SW2_ExitIntIni(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(KEY1_PORT, KEY1_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = KEY1_EXINT_CH;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEY1_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEY1_IRQn;
    stcIrqSignConfig.pfnCallback = &KEY1_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(KEY1_IRQn);
    NVIC_SetPriority(KEY1_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(KEY1_IRQn);
}

/**
 ******************************************************************************
 ** \brief  Initilizes BSP configurations
 **
 ** \param  None
 ** \retval None
 ******************************************************************************/
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
    stc_gpio_init_t stcGpioCfg;
    /* System clock configurate */
    BSP_CLK_Init();
#ifndef M020_EVB_BD
    BSP_IO_Init();
#endif

    BSP_LED_Init();
    DDL_PrintfInit();

    /* USB clock source configurate */
    CLK_USB_ClkConfig(CLK_USB_CLK_MCLK_DIV5);

    /* KEY SW2 interrupt function initialize */
    SW2_ExitIntIni();

    printf("USBFS start !!\n");

    /* port config */
    /* Disable digital function for DM DP */
    stcGpioCfg.u16PinAttr = PIN_ATTR_ANALOG;
    GPIO_Init(USB_DM_PORT, USB_DM_PIN, &stcGpioCfg);
    GPIO_Init(USB_DP_PORT, USB_DP_PIN, &stcGpioCfg);

    /* GPIO function configurate */
    //GPIO_SetFunc(USB_SOF_PORT, USB_SOF_PIN, Func_UsbF, Disable); //SOF

#ifdef USB_OTG_FS_CORE
    GPIO_SetFunc(USB_VBUS_PORT, USB_VBUS_PIN, GPIO_FUNC_10_USBF_VBUS, PIN_SUBFUNC_DISABLE); //VBUS
#else
    GPIO_SetFunc(USB_VBUS_PORT, USB_VBUS_PIN, GPIO_FUNC_12_USBH_VBUS, PIN_SUBFUNC_DISABLE);
#endif
#else
    GPIO_StructInit(&stcGpioCfg);
    /* High drive capability */
    stcGpioCfg.u16PinDrv = PIN_HIGH_DRV;
    GPIO_Init(USBH_ULPI_D0_PORT, USBH_ULPI_D0_PIN, &stcGpioCfg);
    GPIO_Init(USBH_ULPI_D1_PORT, USBH_ULPI_D1_PIN, &stcGpioCfg);
    GPIO_Init(USBH_ULPI_D2_PORT, USBH_ULPI_D2_PIN, &stcGpioCfg);
    GPIO_Init(USBH_ULPI_D3_PORT, USBH_ULPI_D3_PIN, &stcGpioCfg);
    GPIO_Init(USBH_ULPI_D4_PORT, USBH_ULPI_D4_PIN, &stcGpioCfg);
    GPIO_Init(USBH_ULPI_D5_PORT, USBH_ULPI_D5_PIN, &stcGpioCfg);
    GPIO_Init(USBH_ULPI_D6_PORT, USBH_ULPI_D6_PIN, &stcGpioCfg);
    GPIO_Init(USBH_ULPI_D7_PORT, USBH_ULPI_D7_PIN, &stcGpioCfg);
    GPIO_Init(USBH_ULPI_STP_PORT, USBH_ULPI_STP_PIN, &stcGpioCfg);

    GPIO_SetFunc(USBH_ULPI_CLK_PORT, USBH_ULPI_CLK_PIN, GPIO_FUNC_10_USBH_ULPI_CK, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_DIR_PORT, USBH_ULPI_DIR_PIN, GPIO_FUNC_10_USBH_ULPI_DIR, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_NXT_PORT, USBH_ULPI_NXT_PIN, GPIO_FUNC_10_USBH_ULPI_NXT, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_STP_PORT, USBH_ULPI_STP_PIN, GPIO_FUNC_10_USBH_ULPI_STP, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_D0_PORT, USBH_ULPI_D0_PIN, GPIO_FUNC_10_USBH_ULPI_DATA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_D1_PORT, USBH_ULPI_D1_PIN, GPIO_FUNC_10_USBH_ULPI_DATA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_D2_PORT, USBH_ULPI_D2_PIN, GPIO_FUNC_10_USBH_ULPI_DATA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_D3_PORT, USBH_ULPI_D3_PIN, GPIO_FUNC_10_USBH_ULPI_DATA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_D4_PORT, USBH_ULPI_D4_PIN, GPIO_FUNC_10_USBH_ULPI_DATA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_D5_PORT, USBH_ULPI_D5_PIN, GPIO_FUNC_10_USBH_ULPI_DATA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_D6_PORT, USBH_ULPI_D6_PIN, GPIO_FUNC_10_USBH_ULPI_DATA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USBH_ULPI_D7_PORT, USBH_ULPI_D7_PIN, GPIO_FUNC_10_USBH_ULPI_DATA, PIN_SUBFUNC_DISABLE);
#ifndef M020_EVB_BD
    /* Reset 3300 */
    BSP_IO_WritePortPin(USB_3300_RESET_PORT, USB_3300_RESET_PIN, EIO_PIN_SET);
    BSP_IO_ConfigPortPin(USB_3300_RESET_PORT, USB_3300_RESET_PIN, EIO_DIR_OUT);
    BSP_IO_WritePortPin(USB_3300_RESET_PORT, USB_3300_RESET_PIN, EIO_PIN_RESET);
#else
    GPIO_SetPins(USB_3300_RESET_PORT, USB_3300_RESET_PIN);
    stcGpioCfg.u16PinDir = PIN_DIR_OUT;
    GPIO_Init(USB_3300_RESET_PORT, USB_3300_RESET_PIN, &stcGpioCfg);
    GPIO_ResetPins(USB_3300_RESET_PORT, USB_3300_RESET_PIN);
#endif

#endif

#ifdef USB_OTG_FS_CORE
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_USBFS, Enable);
#else
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_USBHS, Enable);
#endif
}

/**
 *******************************************************************************
 ** \brief   Enabele USB Global interrupt
 ** \param None
 ** \retval None
 ******************************************************************************/
void USB_OTG_BSP_EnableInterrupt(void)
{
    stc_irq_signin_config_t stcIrqRegiConf;
    /* Register INT_USBFS_GLB Int to Vect.No.030 */
    stcIrqRegiConf.enIRQn = Int030_IRQn;
    /* Select INT_USBFS_GLB interrupt function */
#ifdef USB_OTG_FS_CORE
    stcIrqRegiConf.enIntSrc = INT_USBFS_GLB;
#else
    stcIrqRegiConf.enIntSrc = INT_USBHS_GLB;
#endif
    /* Callback function */
    stcIrqRegiConf.pfnCallback = &USB_IRQ_Handler;
    /* Registration IRQ */
    INTC_IrqSignIn(&stcIrqRegiConf);
    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);
    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

/**
 *******************************************************************************
 ** \brief  Drives the Vbus signal through IO
 ** \param  speed : Full, Low
 ** \param  state : VBUS states
 ** \retval None
 ******************************************************************************/
void USB_OTG_BSP_DriveVBUS(uint32_t speed, uint8_t state)
{

}

/**
 *******************************************************************************
 ** \brief  Configures the IO for the Vbus and OverCurrent
 ** \param  speed : Full, Low
 ** \retval None
 ******************************************************************************/
void  USB_OTG_BSP_ConfigVBUS(uint32_t speed)
{

}

/**
 *******************************************************************************
 ** \brief  Initialises delay unit Systick timer /Timer2
 ** \param  None
 ** \retval None
 ******************************************************************************/
void USB_OTG_BSP_TimeInit ( void )
{

}

/**
 *******************************************************************************
 ** \brief  This function provides delay time in micro sec
 ** \param  usec : Value of delay required in micro sec
 ** \retval None
 ******************************************************************************/
#define Fclk    15000000ul
void USB_OTG_BSP_uDelay (const uint32_t t)
{
    uint32_t    i;
    uint32_t    j;
    j=Fclk / 1000000ul * t;
    for(i = 0ul; i < j; i++)
    {
        ;
    }
}

/**
 *******************************************************************************
 ** \brief  This function provides delay time in milli sec
 ** \param  msec : Value of delay required in milli sec
 ** \retval None
 ******************************************************************************/
void USB_OTG_BSP_mDelay (const uint32_t msec)
{
    USB_OTG_BSP_uDelay(msec * 1000ul);
}

/**
 *******************************************************************************
 ** \brief  Time base IRQ
 ** \param  None
 ** \retval None
 ******************************************************************************/
void USB_OTG_BSP_TimerIRQ (void)
{

}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
