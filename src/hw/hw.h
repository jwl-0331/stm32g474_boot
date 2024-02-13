/*
 * hw.h
 *
 *  Created on: Feb 2, 2024
 *      Author: mm940
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_

#include "hw_def.h"
#include "rtc.h"
#include "led.h"
#include "cdc.h"
#include "usb.h"
#include "uart.h"
#include "button.h"
#include "flash.h"

bool hwInit(void);

#endif /* SRC_HW_HW_H_ */
