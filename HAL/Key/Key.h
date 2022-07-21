/*
    From : https://github.com/paiminlin/PM
    From : 
    Author : PaiMin.lin
    Date : 2022.7.21
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef KEY_H_
#define KEY_H_

#define KeyTask_MAXNUM           100         /* 支持最多Task数量 */

#define KeyPressDown_MAXTIMES    1000        /*  */

typedef enum 
{
    Key_PressDown_Status        = 0,
    Key_Release_Status          = 1,
    Key_Invalid_Status          = 2,
}Key_Status;

typedef enum 
{
    Key_Short_PressStatus       = 0,
    Key_Long_PressStatus        = 1,
    Key_Invalid_PressStatus     = 2,
}Key_PressStatus;

typedef Key_Status (*KeyGetStatus_Fun)(void);

typedef int (*KeyHandle_Fun)(Key_PressStatus enKeyPressStatus);

typedef struct Key_Attribute
{
    int ShortPressTimes;                /* 维持时间 ShortPressTimes * Key_Run */
    int LongPressTimes;                 /* 维持时间 LongPressTimes * Key_Run */
} Key_Attribute;

typedef struct KeyTask_Info
{
    Key_Attribute stKeyAttribute;
    KeyGetStatus_Fun KeyGetStatusFun;
    KeyHandle_Fun KeyHandleFun;
} KeyTask_Info;

int Key_Run(void);

int Key_Init(void);

int Key_DeInit(void);

int Key_CreatTask(KeyTask_Info * pstKeyTaskInfo);

int Key_DestroyTask(int TaskNum, KeyTask_Info * pstKeyTaskInfo);

#endif /* KEY_H_ */

#ifdef __cplusplus
}
#endif

