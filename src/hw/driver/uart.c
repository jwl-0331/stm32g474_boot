/*
 * uart.c
 *
 *  Created on: Feb 5, 2024
 *      Author: mm940
 */


#include "uart.h"
#include "cdc.h"
#include "qbuffer.h"

// 외부에서 사용 불가능 static, 에러 방지 uart 에서만 사용
static bool is_open[UART_MAX_CH];

static qbuffer_t qbuffer[UART_MAX_CH]; // UART 갯수 만큼
static uint8_t rx_buf[256]; // 수신 버퍼
static uint8_t rx_data[UART_MAX_CH]; // 수신용 데이터

UART_HandleTypeDef huart1;



bool uartInit(void)
{
  for(int i = 0; i <UART_MAX_CH; i++)
  {
    is_open[i] = false;
  }

  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret =false;

  switch(ch)
  {
    case _DEF_UART1:
      huart1.Instance = USART1;
      huart1.Init.BaudRate = baud;
      huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
      huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
      huart1.Init.Mode = UART_MODE_TX_RX;
      huart1.Init.OverSampling = UART_OVERSAMPLING_16;
      huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
      huart1.Init.Parity = UART_PARITY_NONE;
      huart1.Init.StopBits = UART_STOPBITS_1;
      huart1.Init.WordLength = UART_WORDLENGTH_8B;
      huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
      qbufferCreate(&qbuffer[ch],&rx_buf[ch], 256); // 수신 버퍼 생성
      if (HAL_UART_Init(&huart1) != HAL_OK)
      {
        ret = false;
      }
      else
      {
        ret = true;
        is_open[ch] = true;
        // 수신용 데이터 버퍼 (rx_data)에 size 만큼 받아서 HAL_UART_RxCpltCallback 함수 호출
        if(HAL_UART_Receive_IT(&huart1, (uint8_t*)&rx_data[ch], 1) != HAL_OK)
        {
         ret = false;
         is_open[ch] = false;
        }
      }
      if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
      {
        ret = false;
        is_open[ch] = false;
      }
      if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
      {
        ret = false;
        is_open[ch] = false;
      }
      if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
      {
        ret = false;
        is_open[ch] = false;
      }
      break;
  }


  return ret;
}

bool uartClose(uint8_t ch)
{
  return true;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = qbufferAvailable(&qbuffer[ch]);
      break;
  }

  return ret;
}

bool uartFlush(uint8_t ch)
{
  uint32_t pre_time;

  pre_time = millis();
  while(uartAvailable(ch))
  {
    if (millis()-pre_time >= 10)
    {
      break;
    }
    uartRead(ch);
  }

  return true;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret = 0;
  switch(ch)
  {
    case _DEF_UART1:
      qbufferRead(&qbuffer[ch],&ret,1);
      break;
  }

  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  HAL_StatusTypeDef status;
  switch(ch)
  {
    case _DEF_UART1:
      status = HAL_UART_Transmit(&huart1, p_data, length, 100);
      if(status == HAL_OK)
      {
        ret = length;
      }
      break;
  }

  return ret;
}

uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  char buf[256];
  va_list args;
  int len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);

  ret = uartWrite(ch, (uint8_t *)buf, len);

  va_end(args);


  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;


  switch(ch)
  {
    case _DEF_UART1:
      ret = huart1.Init.BaudRate;
      break;
  }

  return ret;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart)
{
  if(huart->Instance == USART1)
  {

  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {
    // 빠르게 송신시 1 바이트 사용시 인터럽트가 많이 걸려서 처리가 늦어져 overrun (덮어쓰기 에러) , 데이터 손실 에러 발생 가능
    // 고속의 송수신 - DMA 방식을 사용
    qbufferWrite(&qbuffer[_DEF_UART1], &rx_data[_DEF_UART1], 1);

    HAL_UART_Receive_IT(&huart1, (uint8_t*)&rx_data[_DEF_UART1], 1);
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = PC4_USART1_TX_Pin|PC5_USART1_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOC, PC4_USART1_TX_Pin|PC5_USART1_RX_Pin);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}
