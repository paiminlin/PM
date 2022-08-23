/*
    From : https://github.com/paiminlin/PM
    From : https://blog.csdn.net/lpaim/article/details/125649860
    Author : PaiMin.lin
    Date : 2022.8.24
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef HAL_LED_H_
#define HAL_LED_H_

#define HAL_LEDTASK_MAXNUM         100      /* 支持最多控制对象数量 */

typedef enum 
{
    HAL_LED_BRIGHT_STATUS = 0,              /* 亮 */
    HAL_LED_DARK_STATUS,                    /* 暗 */
    HAL_LED_INVALID_STATUS,                 /* 无效 */
} Hal_Led_Status;

typedef int (*Hal_LedControl_Fun)(Hal_Led_Status enHalLedStatus);

typedef struct Hal_LedTask_Info
{
    Hal_Led_Status enInitHalLedStatus;      /*  */
    Hal_LedControl_Fun HalLedControlFun;    /*  */
} Hal_LedTask_Info;

typedef enum 
{
    HAL_LED_BRIGHT_MODE = 0,                /* 常亮 */
    HAL_LED_DARK_MODE,                      /* 常暗 */
    HAL_LED_LIMITEDFLASH_MODE,              /* 亮暗闪烁(有限) */
    HAL_LED_UNLIMITEDFLASH_MODE,            /* 亮暗闪烁(无限) */
    HAL_LED_INVALID_MODE,                   /* 无效 */
}Hal_Led_Mode;

typedef struct Hal_LedFlash_Info
{
    int RepeatTimes;                        /* 重复次数 */
    int BrightTimes;                        /* 亮维持时间 * Hal_Led_Run */
    int DarkTimes;                          /* 暗维持时间 * Hal_Led_Run */
} Hal_LedFlash_Info;

int Hal_Led_Run(void);

int Hal_Led_Init(void);

int Hal_Led_DeInit(void);

int Hal_Led_CreatTask(Hal_LedTask_Info * pstHalLedTaskInfo);

int Hal_Led_DestroyTask(int TaskNum, Hal_LedTask_Info * pstHalLedTaskInfo);

int Hal_Led_SetMode(int TaskNum, Hal_Led_Mode enHalLedMode, 
                                Hal_LedFlash_Info stHalLedFlashInfo);

int Hal_Led_GetMode(int TaskNum, Hal_Led_Mode *penHalLedMode, 
                                Hal_LedFlash_Info *pstHalLedFlashInfo);

#endif /* HAL_LED_H_ */

#ifdef __cplusplus
}
#endif

