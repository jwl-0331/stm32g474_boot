/*
 * log.h
 *
 *  Created on: Feb 7, 2024
 *      Author: mm940
 */

#ifndef SRC_COMMON_INCLUDE_LOG_H_
#define SRC_COMMON_INCLUDE_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#ifdef _USE_HW_LOG

#define LOG_CH            HW_LOG_CH


bool logInit(void);
void logPrintf(const char *fmt, ...);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_INCLUDE_LOG_H_ */
