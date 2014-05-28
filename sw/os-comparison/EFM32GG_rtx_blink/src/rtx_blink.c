/**************************************************************************//**
 * @file
 * @brief Simple LCD blink demo for EFM32GG_STK3700 using CMSIS RTOS
 * @author Energy Micro AS
 * @version 3.20.3
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/


#include <stdint.h>
#include "cmsis_os.h"
#include "bsp_trace.h"
#include "em_chip.h"
#include "bsp.h"
#include "segmentlcd.h"

typedef char lcdText_t[8];

/* Define memory pool */
osPoolDef(mpool, 16, lcdText_t);
osPoolId  mpool;

/* Define message queue */
osMessageQDef(msgBox, 16, lcdText_t);
osMessageQId msgBox;

#define OS_STKSIZE 150

/**************************************************************************//**
 * @brief
 *   Thread 1: Print LCD thread
 *****************************************************************************/
void PrintLcdThread(void const *argument) {
  lcdText_t *rptr;
  osEvent  evt;
  (void)argument;                 /* Unused parameter. */

  while (1)
  {
    /* Wait for message */
    evt = osMessageGet(msgBox, osWaitForever);
    if (evt.status == osEventMessage)
    {
      rptr = evt.value.p;
      SegmentLCD_Write(*rptr);
      /* Free memory allocated for message */
      osPoolFree(mpool,rptr);
    }
  }
}

/* Thread definition */
osThreadDef(PrintLcdThread, osPriorityNormal, 1, 0);


/**************************************************************************//**
 * @brief
 *   Thread 2: Put on LCD
 *****************************************************************************/
void PutOnLcd(void const *arg)
{
	static int count = 0;

	/* Infinite loop */
	while (1)
	{
	    count = (count+1)&0xF;
	    BSP_LedsSet(count);

	    /* Send message to PrintLcdThread */
	    /* Allocate memory for the message */
	    lcdText_t *mptr = osPoolAlloc(mpool);
	    /* Set the message content */
	    (*mptr)[0] = count>=10 ? '1' : '0';
	    (*mptr)[1] = count%10 + '0';
	    (*mptr)[2] = '\0';
	    /* Send message */
	    osMessagePut(msgBox, (uint32_t)mptr, osWaitForever);

	    /* Wait now for half a second */
	    osDelay(500);
	}
}

osThreadDef(PutOnLcd, osPriorityNormal, 1, 0);


/**************************************************************************//**
 * @brief
 *   Main function is a CMSIS RTOS thread in itself
 *
 * @note
 *   This example uses threads, memory pool and message queue to demonstrate the
 *   usage of these CMSIS RTOS features. In this simple example, the same
 *   functionality could more easily be achieved by doing everything in the main
 *   loop.
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();

  osKernelInitialize();

  /* If first word of user data page is non-zero, enable eA Profiler trace */
  BSP_TraceProfilerSetup();

  /* Initialize LED driver */
  BSP_LedsInit();

  /* Initialize the LCD driver */
  SegmentLCD_Init(false);

  /* Initialize CMSIS RTOS structures */
  /* create memory pool */
  mpool = osPoolCreate(osPool(mpool));
  /* create msg queue */
  msgBox = osMessageCreate(osMessageQ(msgBox), NULL);
  /* create thread 1 */
  osThreadCreate(osThread(PutOnLcd), NULL);
  osThreadCreate(osThread(PrintLcdThread), NULL);

  osKernelStart();
}
