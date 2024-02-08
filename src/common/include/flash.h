/*
 * flash.h
 *
 *  Created on: Feb 6, 2024
 *      Author: mm940
 */

#ifndef SRC_COMMON_INCLUDE_FLASH_H_
#define SRC_COMMON_INCLUDE_FLASH_H_

#include "hw_def.h"

bool flashInit(void);
bool flashErase(uint32_t addr, uint32_t length);
bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length);
bool flashRead(uint32_t addr, uint32_t *p_data, uint32_t length);


#endif /* SRC_COMMON_INCLUDE_FLASH_H_ */
