/*
 * usb.c
 *
 *  Created on: Feb 5, 2024
 *      Author: mm940
 */

#include "usb.h"
#include "cdc.h"

#ifdef _USE_HW_CLI
#include "cli.h"
#endif

#ifdef _USE_HW_USB
#include "usbd_core.h"

#if HW_USE_CDC == 1
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#endif

#if HW_USE_MSC == 1
#include "usbd_msc.h"
#include "usbd_storage_if.h"
#endif


static bool is_init = false;
static UsbMode is_usb_mode = USB_NON_MODE;

extern USBD_HandleTypeDef hUsbDeviceFS;

extern USBD_DescriptorsTypeDef CDC_Desc;
extern USBD_DescriptorsTypeDef MSC_Desc;
extern PCD_HandleTypeDef hpcd_USB_FS;


#ifdef _USE_HW_CLI
static void cliCmd(cli_args_t *args);
#endif



bool usbInit(void)
{
  bool ret = true;

  /* USB - RESET 시 장치관리자 해제 연결 안될 시 */

  GPIO_InitTypeDef GPIO_InitStructure = {0, };
  // PA12(USB_DP) 출력 설정
  GPIO_InitStructure.Pin = GPIO_PIN_12;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  //회로에 PULL UP 이 걸려있다면 - 내리고
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
  delay(200);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);

  // INPUT 으로 다시 변경
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  MX_USB_Device_Init();

#ifdef _USE_HW_CLI
  cliAdd("usb", cliCmd);
#endif
  return ret;
}

void usbDeInit(void)
{
  if (is_init == true)
  {
    USBD_DeInit(&hUsbDeviceFS);
  }
}

bool usbIsOpen(void)
{
  return cdcIsConnect();
}

bool usbIsConnect(void)
{
  if (hUsbDeviceFS.pClassData == NULL)
  {
    return false;
  }
  if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED)
  {
    return false;
  }
  if (hUsbDeviceFS.dev_config == 0)
  {
    return false;
  }

  return true;
}


UsbMode usbGetMode(void)
{
  return is_usb_mode;
}

bool usbBegin(UsbMode usb_mode)
{
  bool ret = false;


#if HW_USE_CDC == 1

  if (usb_mode == USB_CDC_MODE)
  {
    if (USBD_Init(&hUsbDeviceFS, &CDC_Desc, DEVICE_FS) != USBD_OK)
    {
      return false;
    }
    if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
    {
      return false;
    }
    if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK)
    {
      return false;
    }
    if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
    {
      return false;
    }


    cdcInit();

    is_usb_mode = USB_CDC_MODE;
    ret = true;
  }
#endif

#if HW_USE_MSC == 1

  if (usb_mode == USB_MSC_MODE)
  {
    if (USBD_Init(&hUsbDeviceFS, &MSC_Desc, DEVICE_FS) != USBD_OK)
    {
      return false;
    }
    if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC) != USBD_OK)
    {
      return false;
    }
    if (USBD_MSC_RegisterStorage(&hUsbDeviceFS, &USBD_Storage_Interface_fops_FS) != USBD_OK)
    {
      return false;
    }
    if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
    {
      return false;
    }

    is_usb_mode = USB_MSC_MODE;
    ret = true;
  }
#endif


  is_init = ret;

  return ret;
}


#ifdef _USE_HW_CLI
void cliCmd(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    while(cliKeepLoop())
    {
      cliPrintf("USB Connect : %d\n", usbIsConnect());
      cliPrintf("USB Open    : %d\n", usbIsOpen());
      cliPrintf("\x1B[%dA", 2);
      delay(100);
    }
    cliPrintf("\x1B[%dB", 2);

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "tx") == true)
  {
    uint32_t pre_time;
    uint32_t tx_cnt = 0;

    while(cliKeepLoop())
    {
      if (millis()-pre_time >= 1000)
      {
        pre_time = millis();
        logPrintf("tx : %d KB/s\n", tx_cnt/1024);
        tx_cnt = 0;
      }
      cdcWrite((uint8_t *)"123456789012345678901234567890\n", 31);
      tx_cnt += 31;
    }
    cliPrintf("\x1B[%dB", 2);

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "rx") == true)
  {
    uint32_t pre_time;
    uint32_t rx_cnt = 0;
    uint32_t rx_len;

    while(cliKeepLoop())
    {
      if (millis()-pre_time >= 1000)
      {
        pre_time = millis();
        logPrintf("rx : %d KB/s\n", rx_cnt/1024);
        rx_cnt = 0;
      }

      rx_len = cdcAvailable();

      for (int i=0; i<rx_len; i++)
      {
        cdcRead();
      }

      rx_cnt += rx_len;
    }
    cliPrintf("\x1B[%dB", 2);

    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("usb info\n");
    cliPrintf("usb tx\n");
    cliPrintf("usb rx\n");
  }
}
#endif


#endif
