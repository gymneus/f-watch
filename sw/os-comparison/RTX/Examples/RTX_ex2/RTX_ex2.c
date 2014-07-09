/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    RTX_EX2.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1999-2009 KEIL, 2009-2012 ARM Germany GmbH
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM  nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "em_gpio.h"
#include "em_device.h"
#include "em_cmu.h" 

volatile uint16_t counter;            /* counter for main thread             */
volatile uint16_t counter1;           /* counter for thread 1                */
volatile uint16_t counter2;           /* counter for thread 2                */
volatile uint16_t counter3;           /* counter for thread 3                */

/* Thread IDs */
osThreadId thread1_id;
osThreadId thread2_id;
osThreadId thread3_id;
osThreadId thread4_id;

/* Forward reference */
void job1 (void const *argument);
void job2 (void const *argument);
void job3 (void const *argument);
void job4 (void const *argument);

#define STKSIZE 150

/* Thread definitions */
osThreadDef(job1, osPriorityAboveNormal, 1, STKSIZE);
osThreadDef(job2, osPriorityNormal, 1, STKSIZE);
osThreadDef(job3, osPriorityNormal, 1, STKSIZE);
osThreadDef(job4, osPriorityNormal, 1, STKSIZE);

/*----------------------------------------------------------------------------
 *   Thread 1: 'job1'
 *---------------------------------------------------------------------------*/
void job1 (void const *argument) {    /* higher priority to preempt job2     */
  while (1) {                         /* endless loop                        */
    GPIO_PinOutToggle(gpioPortD, 3);
    osDelay(125);                      /* wait for timeout: 10ms              */
  }
}

/*----------------------------------------------------------------------------
 *   Thread 2: 'job2'
 *---------------------------------------------------------------------------*/
void job2 (void const *argument) {
  while (1)  {                        /* endless loop                        */
      GPIO_PinOutToggle(gpioPortD, 2);
      osDelay(250);
  }
}

/*----------------------------------------------------------------------------
 *   Thread 3: 'job3'
 *---------------------------------------------------------------------------*/
void job3 (void const *argument) {
  while (1) {                         /* endless loop                        */
    GPIO_PinOutToggle(gpioPortD, 1);
    osDelay(500);
  }
}

void job4(void const *argument) {
  while (1) {                         /* endless loop                        */
    osDelay(62);                      /* wait for timeout: 50m               */
    GPIO_PinOutToggle(gpioPortD, 4);
  }
}

/*----------------------------------------------------------------------------
 *   Main Thread
 *---------------------------------------------------------------------------*/
int main (void) {                     /* program execution starts here       */
  int i;

  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  for (i = 1; i < 5; ++i) {
      GPIO_PinModeSet(gpioPortD, i, gpioModePushPull, 0);
  }

  /* Set higher priority of main thread to preempt job2                      */
  osThreadSetPriority(osThreadGetId(), osPriorityAboveNormal);

  thread1_id = osThreadCreate(osThread(job1),NULL);  /* create thread1       */
  thread2_id = osThreadCreate(osThread(job2),NULL);  /* create thread2       */
  thread3_id = osThreadCreate(osThread(job3),NULL);  /* create thread3       */
  thread4_id = osThreadCreate(osThread(job4),NULL);  /* create thread3       */
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
