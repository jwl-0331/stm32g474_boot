/*
 * hw.c
 *
 *  Created on: Feb 2, 2024
 *      Author: mm940
 */
#include "hw.h"

bool hwInit(void)
{
  bool ret = true;

  ret &= bspInit();
  ret &= rtcInit();

  ret &= uartInit();
  ret &= uartOpen(_DEF_UART1, 115200);
#ifdef _USE_HW_RESET
  ret &= resetInit();  // 500 ms delay
#endif

  uartPrintf(_DEF_UART1, "Reset Count : %d\n", resetGetCount());
#ifdef _USE_HW_RESET
  if (resetGetCount() == 2)
  {
    // Jump To SystemBootloader
    resetToSystemBoot();
  }
#endif
  ret &= ledInit();
  /* USB DFU MODE */
  //ret &= usbInit();
  //ret &= usbBegin(USB_DFU_MODE);
  ret &= flashInit();

  return ret;
}

