/*
    From : https://github.com/paiminlin/PM
    From : 
    Author : PaiMin.lin
    Date : 2022.8.23
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef HAL_KEY_H_
#define HAL_KEY_H_

#define HAL_KEYTASK_MAXNUM          100    /* 支持最多Task数量 */

#define HAL_KEYPRESS_MAXTIMES       50     /* 按键粘滞时长 * Hal_Key_Run */

typedef enum 
{
    HAL_KEY_PRESS_STATUS            = 0,    /* 按键按下或松开的状态 */
    HAL_KEY_RELEASE_STATUS          = 1,
    HAL_KEY_INVALID_STATUS          = 2,
}Hal_Key_Status;

typedef enum 
{
    HAL_KEY_SHORTPRESS_STATUS       = 0,    /* 按键短按或长按的状态 */
    HAL_KEY_LONGPRESS_STATUS        = 1,
    HAL_KEY_INVALIDPRESS_STATUS     = 2,
}Hal_Key_PressStatus;

typedef Hal_Key_Status (*Hal_KeyGetStatus_Fun)(void);

typedef int (*Hal_KeyHandle_Fun)(Hal_Key_PressStatus enHalKeyPressStatus);

typedef struct Hal_Key_Attribute
{
    int ShortPressTimes;                    /* 短按维持时间 * Hal_KeyRun */
    int LongPressTimes;                     /* 长按维持时间 * Hal_KeyRun */
} Hal_Key_Attribute;

typedef struct Hal_KeyTask_Info
{
    Hal_Key_Attribute stHalKeyAttribute;    /* 按键短按和长按的属性 */
    Hal_KeyGetStatus_Fun HalKeyGetStatusFun;/* 获取按键按下或松开的状态 */
    Hal_KeyHandle_Fun HalKeyHandleFun;      /* 触发按键短按或长按的行为 */
} Hal_KeyTask_Info;

int Hal_Key_Run(void);

int Hal_Key_Init(void);

int Hal_Key_DeInit(void);

int Hal_Key_CreatTask(Hal_KeyTask_Info * pstHalKeyTaskInfo);

int Hal_Key_DestroyTask(int TaskNum, Hal_KeyTask_Info * pstHalKeyTaskInfo);

#endif /* HAL_KEY_H_ */

#ifdef __cplusplus
}
#endif

