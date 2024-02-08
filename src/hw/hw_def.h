/*
 * hw_def.h
 *
 *  Created on: Feb 2, 2024
 *      Author: mm940
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "bsp.h"

#define _USE_HW_RESET
#define _USE_HW_RTC

#define _USE_HW_LED
#define   HW_LED_MAX_CH         1

#define _USE_HW_UART
#define   HW_UART_MAX_CH        1


#define _USE_HW_FLASH

/* USB DFU MODE */
/*
#define _USE_HW_USB
#define   HW_USE_CDC            0
#define   HW_USE_MSC            0
#define   HW_USE_DFU            1
*/




#endif /* SRC_HW_HW_DEF_H_ */
