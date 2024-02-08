/*
 * cdc.h
 *
 *  Created on: Feb 5, 2024
 *      Author: mm940
 */

#ifndef SRC_COMMON_INCLUDE_CDC_H_
#define SRC_COMMON_INCLUDE_CDC_H_

#include "hw_def.h"


#ifdef _USE_HW_CDC



bool     cdcInit(void);
bool     cdcIsInit(void);
bool     cdcIsConnect(void);
uint32_t cdcAvailable(void);
uint8_t  cdcRead(void);
uint32_t cdcWrite(uint8_t *p_data, uint32_t length);
uint32_t cdcGetBaud(void);


#endif

#endif /* SRC_COMMON_INCLUDE_CDC_H_ */
