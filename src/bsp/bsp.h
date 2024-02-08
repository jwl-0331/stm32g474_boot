/*
 * bsp.h
 *
 *  Created on: Feb 2, 2024
 *      Author: mm940
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#include "def.h"
#include "stm32g4xx_hal.h"


bool bspInit(void);

void delay(uint32_t ms);
uint32_t millis(void);

void Error_Handler(void);
#endif /* SRC_BSP_BSP_H_ */
