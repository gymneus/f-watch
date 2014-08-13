/**************************************************************************//**
 * @file main.c
 * @brief USB CDC Serial Port adapter example project.
 * @version 3.20.5
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "em_device.h"
#include "em_cmu.h"
#include "em_dma.h"
#include "em_dma.h"
#include "em_gpio.h"
#include "em_int.h"
#include "dmactrl.h"
#include "em_usb.h"
#include "em_leuart.h"
#include "bsp.h"
#include "bsp_trace.h"

/**************************************************************************//**
 *
 * This example shows how a CDC based USB to Serial port adapter can be
 * implemented.
 *
 * Use the file EFM32-Cdc.inf to install a USB serial port device driver
 * on the host PC.
 *
 * This implementation uses DMA to transfer data between LEUART0 and memory
 * buffers.
 *
 *****************************************************************************/

/*** Typedef's and defines. ***/

/* Define USB endpoint addresses */
#define EP_DATA_OUT       0x01  /* Endpoint for USB data reception.       */
#define EP_DATA_IN        0x81  /* Endpoint for USB data transmission.    */
#define EP_NOTIFY         0x82  /* The notification endpoint (not used).  */

#define BULK_EP_SIZE     USB_MAX_EP_SIZE  /* This is the max. ep size.    */
#define USB_RX_BUF_SIZ   BULK_EP_SIZE /* Packet size when receiving on USB*/
#define USB_TX_BUF_SIZ   127    /* Packet size when transmitting on USB.  */

/* Calculate a timeout in ms corresponding to 5 char times on current     */
/* baudrate. Minimum timeout is set to 10 ms.                             */
#define RX_TIMEOUT    EFM32_MAX(10U, 50000 / (cdcLineCoding.dwDTERate))

#define LED_SET 0x11


/* The serial port LINE CODING data structure, used to carry information  */
/* about serial port baudrate, parity etc. between host and device.       */
EFM32_PACK_START(1)
typedef struct
{
  uint32_t dwDTERate;               /** Baudrate                            */
  uint8_t  bCharFormat;             /** Stop bits, 0=1 1=1.5 2=2            */
  uint8_t  bParityType;             /** 0=None 1=Odd 2=Even 3=Mark 4=Space  */
  uint8_t  bDataBits;               /** 5, 6, 7, 8 or 16                    */
  uint8_t  dummy;                   /** To ensure size is a multiple of 4 bytes.*/
} __attribute__ ((packed)) cdcLineCoding_TypeDef;
EFM32_PACK_END()


/*** Function prototypes. ***/

static int  UsbDataReceived(USB_Status_TypeDef status, uint32_t xferred,
                            uint32_t remaining);
static void DmaSetup(void);
static int  LineCodingReceived(USB_Status_TypeDef status,
                               uint32_t xferred,
                               uint32_t remaining);
static void SerialPortInit(void);
static int  SetupCmd(const USB_Setup_TypeDef *setup);
static void StateChange(USBD_State_TypeDef oldState,
                        USBD_State_TypeDef newState);
static void UartRxTimeout(void);

/*** Include device descriptor definitions. ***/

#include "descriptors.h"


/*** Variables ***/

/*
 * The LineCoding variable must be 4-byte aligned as it is used as USB
 * transmit and receive buffer
 */
EFM32_ALIGN(4)
EFM32_PACK_START(1)
static cdcLineCoding_TypeDef __attribute__ ((aligned(4))) cdcLineCoding =
{
  4800, 2, 0, 8, 0
};
EFM32_PACK_END()

STATIC_UBUF(usbRxBuffer0, USB_RX_BUF_SIZ);    /* USB receive buffers.   */
STATIC_UBUF(usbRxBuffer1, USB_RX_BUF_SIZ);
STATIC_UBUF(uartRxBuffer0, USB_TX_BUF_SIZ);   /* UART receive buffers.  */
STATIC_UBUF(uartRxBuffer1, USB_TX_BUF_SIZ);

static const uint8_t  *usbRxBuffer[  2 ] = { usbRxBuffer0, usbRxBuffer1 };
static const uint8_t  *uartRxBuffer[ 2 ] = { uartRxBuffer0, uartRxBuffer1 };

static int            usbRxIndex, usbBytesReceived;
static int            uartRxIndex, uartRxCount;
static int            LastUsbTxCnt;

static bool           dmaRxCompleted;
static bool           usbRxActive, dmaTxActive;
static bool           usbTxActive, dmaRxActive;

static DMA_CB_TypeDef DmaTxCallBack;    /** DMA callback structures */
static DMA_CB_TypeDef DmaRxCallBack;

/* Counts 1ms timeTicks */
volatile uint32_t msTicks;

void SysTick_Handler(void)
{
    msTicks++; /* increment counter necessary in Delay()*/
}

void delay(uint32_t dlyTicks)
{
    uint32_t curTicks;

    curTicks = msTicks;
    while ((msTicks - curTicks) < dlyTicks);
}


/**************************************************************************//**
 * @brief main - the entrypoint after reset.
 *****************************************************************************/
int main(void)
{
//  BSP_Init(BSP_INIT_DEFAULT);   /* Initialize DK board register access */
//
//  /* If first word of user data page is non-zero, enable eA Profiler trace */
//  BSP_TraceProfilerSetup();

  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

  GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);;
  GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 1);;

  SerialPortInit();
  DmaSetup();
  USBD_Init(&initstruct);

  /*
   * When using a debugger it is practical to uncomment the following three
   * lines to force host to re-enumerate the device.
   */
//  USBD_Disconnect();
//  USBTIMER_DelayMs(1000);
//  USBD_Connect();

  for (;;)
  {
        GPIO_PinOutToggle(gpioPortE, 11);
        GPIO_PinOutToggle(gpioPortE, 12);
        delay(1000);
  }
}

/**************************************************************************//**
 * @brief Callback function called whenever a new packet with data is received
 *        on USB.
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK.
 *****************************************************************************/
static int UsbDataReceived(USB_Status_TypeDef status,
                           uint32_t xferred,
                           uint32_t remaining)
{
  (void) remaining;            /* Unused parameter */

  if ((status == USB_STATUS_OK) && (xferred > 0))
  {
    usbRxIndex ^= 1;

    if (!dmaTxActive)
    {
      /* dmaTxActive = false means that a new UART Tx DMA can be started. */
      dmaTxActive = true;
      DMA_ActivateBasic(0, true, false,
                        (void *) &(LEUART0->TXDATA),
                        (void *) usbRxBuffer[ usbRxIndex ^ 1 ],
                        xferred - 1);

      /* Start a new USB receive transfer. */
      USBD_Read(EP_DATA_OUT, (void*) usbRxBuffer[ usbRxIndex ],
                USB_RX_BUF_SIZ, UsbDataReceived);
    }
    else
    {
      /* The UART transmit DMA callback function will start a new DMA. */
      usbRxActive      = false;
      usbBytesReceived = xferred;
    }
  }
  return USB_STATUS_OK;
}

/**************************************************************************//**
 * @brief Callback function called whenever a UART transmit DMA has completed.
 *
 * @param[in] channel DMA channel number.
 * @param[in] primary True if this is the primary DMA channel.
 * @param[in] user    Optional user supplied parameter.
 *****************************************************************************/
static void DmaTxComplete(unsigned int channel, bool primary, void *user)
{
  (void) channel;              /* Unused parameter */
  (void) primary;              /* Unused parameter */
  (void) user;                 /* Unused parameter */

  /*
   * As nested interrupts may occur and we rely on variables usbRxActive
   * and dmaTxActive etc, we must handle this function as a critical region.
   */
  INT_Disable();

  if (!usbRxActive)
  {
    /* usbRxActive = false means that an USB receive packet has been received.*/
    DMA_ActivateBasic(0, true, false,
                      (void *) &(LEUART0->TXDATA),
                      (void *) usbRxBuffer[ usbRxIndex ^ 1 ],
                      usbBytesReceived - 1);

    /* Start a new USB receive transfer. */
    usbRxActive = true;
    USBD_Read(EP_DATA_OUT, (void*) usbRxBuffer[ usbRxIndex ],
              USB_RX_BUF_SIZ, UsbDataReceived);
  }
  else
  {
    /* The USB receive complete callback function will start a new DMA. */
    dmaTxActive = false;
  }

  INT_Enable();
}

/**************************************************************************//**
 * @brief Callback function called whenever a packet with data has been
 *        transmitted on USB
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK.
 *****************************************************************************/
static int UsbDataTransmitted(USB_Status_TypeDef status,
                              uint32_t xferred,
                              uint32_t remaining)
{
  (void) xferred;              /* Unused parameter */
  (void) remaining;            /* Unused parameter */
  char tmp[7] = {'0', '1', '2', '3', '4', '5', '\n'};

  if (status == USB_STATUS_OK)
  {
        delay(100);
        USBD_Write(0, (void*) tmp, 7, UsbDataTransmitted);
//    if (!dmaRxActive)
//    {
//      /* dmaRxActive = false means that a new UART Rx DMA can be started. */
//
//      USBD_Write(EP_DATA_IN, (void*) uartRxBuffer[ uartRxIndex ^ 1],
//                 uartRxCount, UsbDataTransmitted);
//      LastUsbTxCnt = uartRxCount;
//
//      dmaRxActive    = true;
//      dmaRxCompleted = true;
//      DMA_ActivateBasic(1, true, false,
//                        (void *) uartRxBuffer[ uartRxIndex ],
//                        (void *) &(LEUART0->RXDATA),
//                        USB_TX_BUF_SIZ - 1);
//      uartRxCount = 0;
//      USBTIMER_Start(0, RX_TIMEOUT, UartRxTimeout);
//    }
//    else
//    {
//      /* The UART receive DMA callback function will start a new DMA. */
//      usbTxActive = false;
//    }
  }
  return USB_STATUS_OK;
}

/**************************************************************************//**
 * @brief Callback function called whenever a UART receive DMA has completed.
 *
 * @param[in] channel DMA channel number.
 * @param[in] primary True if this is the primary DMA channel.
 * @param[in] user    Optional user supplied parameter.
 *****************************************************************************/
static void DmaRxComplete(unsigned int channel, bool primary, void *user)
{
  (void) channel;              /* Unused parameter */
  (void) primary;              /* Unused parameter */
  (void) user;                 /* Unused parameter */

  /*
   * As nested interrupts may occur and we rely on variables usbTxActive
   * and dmaRxActive etc, we must handle this function as a critical region.
   */
  INT_Disable();

  uartRxIndex ^= 1;

  if (dmaRxCompleted)
  {
    uartRxCount = USB_TX_BUF_SIZ;
  }
  else
  {
    uartRxCount = USB_TX_BUF_SIZ - 1 -
                  ((dmaControlBlock[ 1 ].CTRL & _DMA_CTRL_N_MINUS_1_MASK)
                   >> _DMA_CTRL_N_MINUS_1_SHIFT);
  }

  if (!usbTxActive)
  {
    /* usbTxActive = false means that a new USB packet can be transferred. */
    usbTxActive = true;
    USBD_Write(EP_DATA_IN, (void*) uartRxBuffer[ uartRxIndex ^ 1],
               uartRxCount, UsbDataTransmitted);
    LastUsbTxCnt = uartRxCount;

    /* Start a new UART receive DMA. */
    dmaRxCompleted = true;
    DMA_ActivateBasic(1, true, false,
                      (void *) uartRxBuffer[ uartRxIndex ],
                      (void *) &(LEUART0->RXDATA),
                      USB_TX_BUF_SIZ - 1);
    uartRxCount = 0;
    USBTIMER_Start(0, RX_TIMEOUT, UartRxTimeout);
  }
  else
  {
    /* The USB transmit complete callback function will start a new DMA. */
    dmaRxActive = false;
    USBTIMER_Stop(0);
  }

  INT_Enable();
}

/**************************************************************************//**
 * @brief
 *   Called each time UART Rx timeout period elapses.
 *   Implements UART Rx rate monitoring, i.e. we must behave differently when
 *   UART Rx rate is slow e.g. when a person is typing characters, and when UART
 *   Rx rate is maximum.
 *****************************************************************************/
static void UartRxTimeout(void)
{
  int      cnt;
  uint32_t dmaCtrl;

  dmaCtrl = dmaControlBlock[ 1 ].CTRL;

  /* Has the DMA just completed ? */
  if ((dmaCtrl & _DMA_CTRL_CYCLE_CTRL_MASK) == DMA_CTRL_CYCLE_CTRL_INVALID)
  {
    return;
  }

  cnt = USB_TX_BUF_SIZ - 1 -
        ((dmaCtrl & _DMA_CTRL_N_MINUS_1_MASK) >> _DMA_CTRL_N_MINUS_1_SHIFT);

  if ((cnt == 0) && (LastUsbTxCnt == BULK_EP_SIZE))
  {
    /*
     * No activity on UART Rx, send a ZERO length USB package if last USB
     * USB package sent was BULK_EP_SIZE (max. EP size) long.
     */
    DMA->CHENC     = 2;               /* Stop DMA channel 1 (2 = 1 << 1). */
    dmaRxCompleted = false;
    DmaRxComplete(1, true, NULL);     /* Call DMA completion callback.    */
    return;
  }

  if ((cnt > 0) && (cnt == uartRxCount))
  {
    /*
     * There is curently no activity on UART Rx but some chars have been
     * received. Stop DMA and transmit the chars we have got so far on USB.
     */
    DMA->CHENC     = 2;               /* Stop DMA channel 1 (2 = 1 << 1). */
    dmaRxCompleted = false;
    DmaRxComplete(1, true, NULL);     /* Call DMA completion callback.    */
    return;
  }

  /* Restart timer to continue monitoring. */
  uartRxCount = cnt;
  USBTIMER_Start(0, RX_TIMEOUT, UartRxTimeout);
}

/**************************************************************************//**
 * @brief
 *   Callback function called each time the USB device state is changed.
 *   Starts CDC operation when device has been configured by USB host.
 *
 * @param[in] oldState The device state the device has just left.
 * @param[in] newState The new device state.
 *****************************************************************************/
static void StateChange(USBD_State_TypeDef oldState,
                        USBD_State_TypeDef newState)
{
  if (newState == USBD_STATE_CONFIGURED)
  {
    /* We have been configured, start CDC functionality ! */

    if (oldState == USBD_STATE_SUSPENDED)   /* Resume ?   */
    {
    }

    /* Start receiving data from USB host. */
    usbRxIndex  = 0;
    usbRxActive = true;
    dmaTxActive = false;
    USBD_Read(EP_DATA_OUT, (void*) usbRxBuffer[ usbRxIndex ],
              USB_RX_BUF_SIZ, UsbDataReceived);

    /* Start receiving data on UART. */
    uartRxIndex    = 0;
    LastUsbTxCnt   = 0;
    uartRxCount    = 0;
    dmaRxActive    = true;
    usbTxActive    = false;
    dmaRxCompleted = true;
    DMA_ActivateBasic(1, true, false,
                      (void *) uartRxBuffer[ uartRxIndex ],
                      (void *) &(LEUART0->RXDATA),
                      USB_TX_BUF_SIZ - 1);
    USBTIMER_Start(0, RX_TIMEOUT, UartRxTimeout);
  }

  else if ((oldState == USBD_STATE_CONFIGURED) &&
           (newState != USBD_STATE_SUSPENDED))
  {
    /* We have been de-configured, stop CDC functionality */
    USBTIMER_Stop(0);
    DMA->CHENC = 3;     /* Stop DMA channels 0 and 1. */
  }

  else if (newState == USBD_STATE_SUSPENDED)
  {
    /* We have been suspended, stop CDC functionality */
    /* Reduce current consumption to below 2.5 mA.    */
    USBTIMER_Stop(0);
    DMA->CHENC = 3;     /* Stop DMA channels 0 and 1. */
  }
}

/**************************************************************************//**
 * @brief
 *   Handle USB setup commands. Implements CDC class specific commands.
 *
 * @param[in] setup Pointer to the setup packet received.
 *
 * @return USB_STATUS_OK if command accepted.
 *         USB_STATUS_REQ_UNHANDLED when command is unknown, the USB device
 *         stack will handle the request.
 *****************************************************************************/
static int SetupCmd(const USB_Setup_TypeDef *setup)
{
  int r = USB_STATUS_REQ_UNHANDLED;

  if ((setup->Type == USB_SETUP_TYPE_CLASS) &&
      (setup->Recipient == USB_SETUP_RECIPIENT_INTERFACE))
  {
    switch (setup->bRequest)
    {
    case USB_CDC_GETLINECODING:
      /********************/
      if ((setup->wValue == 0) &&
          (setup->wIndex == 0) &&               /* Interface no.            */
          (setup->wLength == 7) &&              /* Length of cdcLineCoding  */
          (setup->Direction == USB_SETUP_DIR_IN))
      {
        /* Send current settings to USB host. */
        USBD_Write(0, (void*) &cdcLineCoding, 7, NULL);
        r = USB_STATUS_OK;
      }
      break;

    case USB_CDC_SETLINECODING:
      /********************/
      if ((setup->wValue == 0) &&
          (setup->wIndex == 0) &&               /* Interface no.            */
          (setup->wLength == 7) &&              /* Length of cdcLineCoding  */
          (setup->Direction != USB_SETUP_DIR_IN))
      {
        /* Get new settings from USB host. */
        USBD_Read(0, (void*) &cdcLineCoding, 7, LineCodingReceived);
        r = USB_STATUS_OK;
      }
      break;

    case USB_CDC_SETCTRLLINESTATE:
      /********************/
      if ((setup->wIndex == 0) &&               /* Interface no.  */
          (setup->wLength == 0))                /* No data        */
      {
        /* Do nothing ( Non compliant behaviour !! ) */
        r = USB_STATUS_OK;
      }
      break;
    }
  }

  return r;
}

/**************************************************************************//**
 * @brief
 *   Callback function called when the data stage of a CDC_SET_LINECODING
 *   setup command has completed.
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK if data accepted.
 *         USB_STATUS_REQ_ERR if data calls for modes we can not support.
 *****************************************************************************/
static int LineCodingReceived(USB_Status_TypeDef status,
                              uint32_t xferred,
                              uint32_t remaining)
{
  uint32_t frame = 0;
  (void) remaining;

  /* We have received new serial port communication settings from USB host */
  if ((status == USB_STATUS_OK) && (xferred == 7))
  {
    /* Check bDataBits, valid values are: 5, 6, 7, 8 or 16 bits */
//    if (cdcLineCoding.bDataBits == 5)
//      frame |= UART_FRAME_DATABITS_FIVE;
//
//    else if (cdcLineCoding.bDataBits == 6)
//      frame |= UART_FRAME_DATABITS_SIX;
//
//    else if (cdcLineCoding.bDataBits == 7)
//      frame |= UART_FRAME_DATABITS_SEVEN;

    if (cdcLineCoding.bDataBits == 8)
      frame |= LEUART_CTRL_DATABITS_EIGHT;

    else if (cdcLineCoding.bDataBits == 9)
      frame |= LEUART_CTRL_DATABITS_NINE;

    else
      return USB_STATUS_REQ_ERR;

    /* Check bParityType, valid values are: 0=None 1=Odd 2=Even 3=Mark 4=Space  */
    if (cdcLineCoding.bParityType == 0)
      frame |= LEUART_CTRL_PARITY_NONE;

    else if (cdcLineCoding.bParityType == 1)
      frame |= LEUART_CTRL_PARITY_ODD;

    else if (cdcLineCoding.bParityType == 2)
      frame |= LEUART_CTRL_PARITY_EVEN;

    else if (cdcLineCoding.bParityType == 3)
      return USB_STATUS_REQ_ERR;

    else if (cdcLineCoding.bParityType == 4)
      return USB_STATUS_REQ_ERR;

    else
      return USB_STATUS_REQ_ERR;

    /* Check bCharFormat, valid values are: 0=1 1=1.5 2=2 stop bits */
    if (cdcLineCoding.bCharFormat == 0)
      frame |= LEUART_CTRL_STOPBITS_ONE;

    else if (cdcLineCoding.bCharFormat == 1)
      frame |= LEUART_CTRL_STOPBITS_TWO;

//    else if (cdcLineCoding.bCharFormat == 2)
//      frame |= UART_FRAME_STOPBITS_TWO;

    else
      return USB_STATUS_REQ_ERR;

    /* Program new UART baudrate etc. */
    LEUART0->CTRL = frame;
    LEUART_BaudrateSet(LEUART0, 0, cdcLineCoding.dwDTERate);

    return USB_STATUS_OK;
  }
  return USB_STATUS_REQ_ERR;
}

/**************************************************************************//**
 * @brief Initialize the DMA peripheral.
 *****************************************************************************/
static void DmaSetup(void)
{
  /* DMA configuration structs */
  DMA_Init_TypeDef       dmaInit;
  DMA_CfgChannel_TypeDef chnlCfgTx, chnlCfgRx;
  DMA_CfgDescr_TypeDef   descrCfgTx, descrCfgRx;

  /* Initialize the DMA */
  dmaInit.hprot        = 0;
  dmaInit.controlBlock = dmaControlBlock;
  DMA_Init(&dmaInit);

  /*---------- Configure DMA channel 0 for UART Tx. ----------*/

  /* Setup the interrupt callback routine */
  DmaTxCallBack.cbFunc  = DmaTxComplete;
  DmaTxCallBack.userPtr = NULL;

  /* Setup the channel */
  chnlCfgTx.highPri   = false;    /* Can't use with peripherals */
  chnlCfgTx.enableInt = true;     /* Interrupt needed when buffers are used */
  chnlCfgTx.select    = DMAREQ_LEUART0_TXBL;
  chnlCfgTx.cb        = &DmaTxCallBack;
  DMA_CfgChannel(0, &chnlCfgTx);

  /* Setup channel descriptor */
  /* Destination is UART Tx data register and doesn't move */
  descrCfgTx.dstInc = dmaDataIncNone;
  descrCfgTx.srcInc = dmaDataInc1;
  descrCfgTx.size   = dmaDataSize1;

  /* We have time to arbitrate again for each sample */
  descrCfgTx.arbRate = dmaArbitrate1;
  descrCfgTx.hprot   = 0;

  /* Configure primary descriptor. */
  DMA_CfgDescr(0, true, &descrCfgTx);

  /*---------- Configure DMA channel 1 for UART Rx. ----------*/

  /* Setup the interrupt callback routine */
  DmaRxCallBack.cbFunc  = DmaRxComplete;
  DmaRxCallBack.userPtr = NULL;

  /* Setup the channel */
  chnlCfgRx.highPri   = false;    /* Can't use with peripherals */
  chnlCfgRx.enableInt = true;     /* Interrupt needed when buffers are used */
  chnlCfgRx.select    = DMAREQ_LEUART0_RXDATAV;
  chnlCfgRx.cb        = &DmaRxCallBack;
  DMA_CfgChannel(1, &chnlCfgRx);

  /* Setup channel descriptor */
  /* Source is UART Rx data register and doesn't move */
  descrCfgRx.dstInc = dmaDataInc1;
  descrCfgRx.srcInc = dmaDataIncNone;
  descrCfgRx.size   = dmaDataSize1;

  /* We have time to arbitrate again for each sample */
  descrCfgRx.arbRate = dmaArbitrate1;
  descrCfgRx.hprot   = 0;

  /* Configure primary descriptor. */
  DMA_CfgDescr(1, true, &descrCfgRx);
}

/**************************************************************************//**
 * @brief Initialize the UART peripheral.
 *****************************************************************************/
static void SerialPortInit(void)
{
  LEUART_TypeDef      *leuart = LEUART0;
  LEUART_Init_TypeDef init    = LEUART_INIT_DEFAULT;

  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(gpioPortE, 14, gpioModePushPull, 1);
  GPIO_PinModeSet(gpioPortE, 15, gpioModeInput, 0);

  /* Enable CORE LE clock in order to access LE modules */
  CMU_ClockEnable(cmuClock_CORELE, true);

  /* Select LFXO for LEUARTs (and wait for it to stabilize) */
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);

  /* Enable LEUART0 clock */
  CMU_ClockEnable(cmuClock_LEUART0, true);

  CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1);

  /* Configure UART for basic async operation */
  init.enable = leuartDisable;
  init.baudrate = 4800;
  init.databits = leuartDatabits8;
  init.stopbits = leuartStopbits2;

  LEUART_Init(leuart, &init);

//  delay(1000);

  /* Enable pins at LEUART0 location #2 */
  leuart->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_ROUTE_LOCATION_LOC2;

//  /* Clear previous RX interrupts */
//  LEUART_IntClear(leuart, LEUART_IF_RXDATAV);
//  NVIC_ClearPendingIRQ(LEUART0_IRQn);
//
//  /* Enable RX interrupts */
//  LEUART_IntEnable(leuart, LEUART_IF_RXDATAV);
//  NVIC_EnableIRQ(LEUART0_IRQn);

  /* Finally enable it */
  LEUART_Enable(leuart, leuartEnable);
}
