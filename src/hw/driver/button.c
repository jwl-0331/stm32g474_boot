/*
 * button.c
 *
 *  Created on: Feb 6, 2024
 *      Author: mm940
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "button.h"
#include "swtimer.h"
#include "uart.h"
#include "cli.h"

#ifdef _USE_HW_BTN
#ifdef _USE_HW_CLI
static void cliButton(cli_args_t *args);
#endif
#define BTN_EVENT_MAX          8

typedef struct
{
  bool        pressed;
  bool        pressed_event;
  uint16_t    pressed_cnt;
  uint32_t    pressed_start_time;
  uint32_t    pressed_end_time;

  bool        released;
  bool        released_event;
  uint32_t    released_start_time;
  uint32_t    released_end_time;

  bool        repeat_update;
  uint32_t    repeat_cnt;
  uint32_t    repeat_time_detect;
  uint32_t    repeat_time_delay;
  uint32_t    repeat_time;
} button_t;

typedef struct
{
  GPIO_TypeDef *port;
  uint32_t      pin;
  GPIO_PinState on_state;
} button_pin_t;

static const button_pin_t button_pin[BTN_MAX_CH] = {
  {PC13_BLU_SW_GPIO_Port, PC13_BLU_SW_Pin, GPIO_PIN_SET},
};

static const char *button_name[BTN_MAX_CH+1] =
{
  "BLUE_BTN",
  "Unknown"
};

static button_t button_tbl[BTN_MAX_CH];
static bool is_enable = true;
static uint16_t event_cnt = 0;
static uint8_t event_level = 5;
static button_event_t *event_tbl[BTN_EVENT_MAX];

extern void buttonISR(void *arg);
extern bool buttonGetPin(uint8_t ch);


bool buttonInit(void)
{
  bool ret = true;

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();


  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  for(int i=0; i < BTN_MAX_CH; i++)
  {
    GPIO_InitStruct.Pin = button_pin[i].pin;
    HAL_GPIO_Init(PC13_BLU_SW_GPIO_Port, &GPIO_InitStruct);
  }

  for(int i=0; i < BTN_MAX_CH; i++)
  {
    button_tbl[i].pressed = 0;
    button_tbl[i].pressed_cnt = 0;
    button_tbl[i].pressed_event = 0;

    button_tbl[i].released = 0;
    button_tbl[i].released_event = 0;

    button_tbl[i].repeat_cnt = 0;
    button_tbl[i].repeat_update = 0;
  }

  for (int i=0; i<BTN_EVENT_MAX; i++)
  {
   event_tbl[i] = NULL;
  }

  swtimer_handle_t timer_ch;
  timer_ch = swtimerGetHandle();
  if (timer_ch >= 0)
  {
    swtimerSet(timer_ch, 10, LOOP_TIME, buttonISR, NULL);
    swtimerStart(timer_ch);
  }
  else
  {
    uartPrintf(_DEF_UART1,"[NG] buttonInit()\n     swtimerGetHandle()\n");
  }
#ifdef _USE_HW_CLI
  cliAdd("BUTTON", cliButton);
#endif
  return ret;
}

bool  buttonEventInit(button_event_t *p_event, uint8_t level)
{
  bool ret = false;

  if(event_cnt < BTN_EVENT_MAX)
  {
    memset(p_event, 0, sizeof(button_event_t));

    event_tbl[event_cnt] = p_event;
    p_event->level = level;
    p_event->index = event_cnt;
    event_cnt++;

    p_event->is_init = true;
    ret = true;
  }

  return ret;
}

void buttonISR(void* args)
{
  uint32_t repeat_time;

  for (int i=0; i<BTN_MAX_CH; i++)
  {
    // btn pushed
    if (buttonGetPin(i) == true)
    {
      //pre btn state - false
      if (button_tbl[i].pressed == false)
      {
        button_tbl[i].pressed_event = true;
        button_tbl[i].pressed_start_time = HAL_GetTick();

        for (int e_i=0; e_i<event_cnt; e_i++)
        {
          if (event_tbl[e_i]->level <= event_level)
            event_tbl[e_i]->pressed_event[i] = true;
          else
            event_tbl[e_i]->pressed_event[i] = false;
        }
      }

      button_tbl[i].pressed = true;
      button_tbl[i].pressed_cnt++;

      if (button_tbl[i].repeat_cnt == 0)
      {
        repeat_time = button_tbl[i].repeat_time_detect;
      }
      else if (button_tbl[i].repeat_cnt == 1)
      {
        repeat_time = button_tbl[i].repeat_time_delay;
      }
      else
      {
        repeat_time = button_tbl[i].repeat_time;
      }
      if (button_tbl[i].pressed_cnt >= repeat_time)
      {
        button_tbl[i].pressed_cnt = 0;
        button_tbl[i].repeat_cnt++;
        button_tbl[i].repeat_update = true;

        for (int e_i=0; e_i<event_cnt; e_i++)
        {
          if (event_tbl[e_i]->level <= event_level)
            event_tbl[e_i]->repeat_event[i] = true;
          else
            event_tbl[e_i]->repeat_event[i] = false;
        }
      }

      button_tbl[i].pressed_end_time = HAL_GetTick();

      button_tbl[i].released = false;
    }
    else  //btn - release
    {
      // pre btn state - pressed
      if (button_tbl[i].pressed == true)
      {
        button_tbl[i].released_event = true;
        button_tbl[i].released_start_time = HAL_GetTick();

        for (int e_i=0; e_i<event_cnt; e_i++)
        {
          if (event_tbl[e_i]->level <= event_level)
            event_tbl[e_i]->released_event[i] = true;
          else
            event_tbl[e_i]->released_event[i] = false;
        }
      }

      button_tbl[i].pressed  = false;
      button_tbl[i].released = true;
      button_tbl[i].repeat_cnt = 0;
      button_tbl[i].repeat_update = false;

      button_tbl[i].released_end_time = HAL_GetTick();
    }
  }

  uartPrintf("pressed_cnt : %d\nrepeat_cnt : %d\nrepeat_time : %d\n", button_tbl[0].pressed_cnt, button_tbl[0].repeat_cnt, button_tbl[0].repeat_time);
}

// btn - click : TRUE
bool buttonGetPin(uint8_t ch)
{
  bool ret = false;

  if(ch >= BTN_MAX_CH)
  {
    return false;
  }

  // btn - pushed ret true
  if (HAL_GPIO_ReadPin(button_pin[ch].port, button_pin[ch].pin) == button_pin[ch].on_state)
  {
    ret = true;
  }

  return ret;
}

void buttonClear(void)
{
  for (int i=0; i<BTN_MAX_CH; i++)
  {
    button_tbl[i].pressed_start_time    = 0;
    button_tbl[i].pressed_end_time      = 0;
    button_tbl[i].released_start_time   = 0;
    button_tbl[i].released_end_time     = 0;

    button_tbl[i].pressed_event = false;
    button_tbl[i].released_event = false;
  }
}

void buttonEnable(bool enable)
{
  is_enable = enable;
}

// ret btn state
bool buttonGetPressed(uint8_t ch)
{
  if (ch >= BTN_MAX_CH || is_enable == false)
  {
    return false;
  }

  return button_tbl[ch].pressed;
}


uint32_t buttonGetData(void)
{
  uint32_t ret = 0;


  for (int i=0; i<BTN_MAX_CH; i++)
  {
    ret |= (buttonGetPressed(i)<<i);
  }

  return ret;
}

const char *buttonGetName(uint8_t ch)
{
  ch = constrain(ch, 0, BTN_MAX_CH);

  return button_name[ch];
}

uint8_t  buttonGetPressedCount(void)
{
  uint32_t i;
  uint8_t ret = 0;

  for (i=0; i<BTN_MAX_CH; i++)
  {
    if (buttonGetPressed(i) == true)
    {
      ret++;
    }
  }

  return ret;
}

void buttonSetRepeatTime(uint8_t ch, uint32_t detect_ms, uint32_t repeat_delay_ms, uint32_t repeat_ms)
{
  if (ch >= BTN_MAX_CH || is_enable == false) return;

  button_tbl[ch].repeat_update = false;
  button_tbl[ch].repeat_cnt = 0;
  button_tbl[ch].pressed_cnt = 0;

  button_tbl[ch].repeat_time_detect = detect_ms;
  button_tbl[ch].repeat_time_delay  = repeat_delay_ms;
  button_tbl[ch].repeat_time        = repeat_ms;
}

uint32_t buttonGetRepeatCount(uint8_t ch)
{
  volatile uint32_t ret = 0;

  if (ch >= BTN_MAX_CH || is_enable == false) return 0;

  ret = button_tbl[ch].repeat_cnt;

  return ret;
}

uint32_t buttonGetPressedTime(uint8_t ch)
{
  volatile uint32_t ret;


  if (ch >= BTN_MAX_CH || is_enable == false) return 0;


  ret = button_tbl[ch].pressed_end_time - button_tbl[ch].pressed_start_time;

  return ret;
}


bool button_GetReleased(uint8_t ch)
{
  bool ret;


  if (ch >= BTN_MAX_CH || is_enable == false) return false;

  ret = button_tbl[ch].released;

  return ret;
}

uint32_t buttonGetReleasedTime(uint8_t ch)
{
  volatile uint32_t ret;


  if (ch >= BTN_MAX_CH || is_enable == false) return 0;


  ret = button_tbl[ch].released_end_time - button_tbl[ch].released_start_time;

  return ret;
}

bool buttonEventClear(button_event_t *p_event)
{
  bool ret = true;


  if (is_enable == false) return false;
  if (p_event->is_init != true) return false;

  for (int i=0; i<BTN_MAX_CH; i++)
  {
    event_tbl[p_event->index]->released_event[i] = false;
  }
  return ret;
}

bool button_EventGetReleased(button_event_t *p_event, uint8_t ch)
{
  bool ret;


  if (ch >= BTN_MAX_CH || is_enable == false) return false;
  if (p_event->is_init != true) return false;


  ret = event_tbl[p_event->index]->released_event[ch];
  event_tbl[p_event->index]->released_event[ch] = false;

  return ret;
}

bool buttonEventGetPressed(button_event_t *p_event, uint8_t ch)
{
  bool ret;


  if (ch >= BTN_MAX_CH || is_enable == false) return false;
  if (p_event->is_init != true) return false;


  ret = event_tbl[p_event->index]->pressed_event[ch];
  event_tbl[p_event->index]->pressed_event[ch] = false;

  return ret;
}

uint32_t buttonEventGetRepeat(button_event_t *p_event, uint8_t ch)
{
  volatile uint32_t ret = 0;

  if (ch >= BTN_MAX_CH || is_enable == false) return 0;
  if (p_event->is_init != true) return false;

  if (event_tbl[p_event->index]->repeat_event[ch])
  {
    event_tbl[p_event->index]->repeat_event[ch] = false;
    ret = button_tbl[ch].repeat_cnt;
  }

  return ret;
}

#ifdef _USE_HW_CLI
void cliButton(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info"))
  {
    for (int i=0; i<BTN_MAX_CH; i++)
    {
      cliPrintf("%-12s pin %d\n", buttonGetName(i), button_pin[i].pin);
    }
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "show"))
  {
    while(cliKeepLoop())
    {
      for (int i=0; i<BTN_MAX_CH; i++)
      {
        cliPrintf("%d", buttonGetPressed(i));
      }
      delay(50);
      cliPrintf("\r");
    }
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "time"))
  {
    uint8_t ch;

    ch = (uint8_t)args->getData(1);
    ch = constrain(ch, 0, BTN_MAX_CH-1);

    while(cliKeepLoop())
    {
      for (int i=0; i<BTN_MAX_CH; i++)
      {
        if(buttonGetPressed(i))
        {
          cliPrintf("%-12s, Time :  %d ms\n", buttonGetName(i), buttonGetPressedTime(i));
        }
      }
      delay(10);
    }
    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("button info\n");
    cliPrintf("button show\n");
    cliPrintf("button time\n", BTN_MAX_CH);
  }
}
#endif

#endif
#ifdef __cplusplus
extern "C" }
#endif
