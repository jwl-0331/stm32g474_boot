/*
 * ap.c
 *
 *  Created on: Feb 2, 2024
 *      Author: mm940
 */
#include "ap.h"
/* for duration blink*/
/*
static uint32_t g_LEDOnTime = 1000;
static uint32_t g_LEDOffTime = 200;
static uint32_t nextmill = 0;
*/
void apInit(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
}

void apMain(void)
{


  uint32_t pre_time1 = millis();
  while(1)
  {
    /* LED Non blocking toggle*/
    /* Using Reg to toggle */

    if(millis()-pre_time1 >= 100)
    {
      pre_time1 = millis();
      ledToggle(_DEF_LED1);
    }
  }
}

