/*
 * uart.h
 *
 *  Created on: Feb 5, 2024
 *      Author: mm940
 */

#ifndef SRC_COMMON_INCLUDE_UART_H_
#define SRC_COMMON_INCLUDE_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#ifdef _USE_HW_UART

#define UART_MAX_CH         HW_UART_MAX_CH

#define PC4_USART1_TX_Pin GPIO_PIN_4
#define PC4_USART1_TX_GPIO_Port GPIOC
#define PC5_USART1_RX_Pin GPIO_PIN_5
#define PC5_USART1_RX_GPIO_Port GPIOC

bool     uartInit(void);
bool     uartOpen(uint8_t ch, uint32_t baud);
bool     uartClose(uint8_t ch);
uint32_t uartAvailable(uint8_t ch);
bool     uartFlush(uint8_t ch);
uint8_t  uartRead(uint8_t ch);
uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length);
uint32_t uartPrintf(uint8_t ch, const char *fmt, ...);
uint32_t uartGetBaud(uint8_t ch);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_INCLUDE_UART_H_ */
