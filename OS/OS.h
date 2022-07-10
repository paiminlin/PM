
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef OS_H_
#define OS_H_

#define OSTask_MAXNUM           100         /* 支持最多Task数量 */

typedef enum 
{
    OSTask_WakeUp_Grab          = 0,        /* 抢占唤醒 */
    OSTask_WakeUp_1MSTimer      = 1,        /* 1MS定时器唤醒 */
    OSTask_WakeUp_Invalid       = 2,
}OSTask_WakeUp;

typedef enum 
{
    OSTask_Grab_Event1          = 0,
    OSTask_Grab_Event2          = 1,
    OSTask_Grab_Event3          = 2,
    OSTask_Grab_Invalid         = 3,
}OSTask_Grab;

typedef enum 
{
    OSTask_Timer_1MS            = 0,
    OSTask_Timer_10MS           = 1,
    OSTask_Timer_100MS          = 2,
    OSTask_Timer_Invalid        = 3,
}OSTask_Timer;

typedef int (*OSTaskInit_Fun)(void);

typedef int (*OSTaskDeInit_Fun)(void);

typedef int (*OSTaskRun_Fun)(void);

typedef struct OSTask_Info
{
    OSTask_WakeUp enOSTaskWakeUp;           /* 唤醒方式 */
    OSTask_Grab enOSTaskGrab;               /* 抢占唤醒 */
    OSTask_Timer enOSTaskTimer;             /* 定时器唤醒周期 */
    OSTaskInit_Fun OSTaskInitFun;           /* Task初始化回调 暂不支持While(1)逻辑 */
    OSTaskDeInit_Fun OSTaskDeInitFun;       /* Task去初始化回调 暂不支持While(1)逻辑 */
    OSTaskRun_Fun OSTaskRunFun;             /* Task任务回调 暂不支持While(1)逻辑 */
} OSTask_Info;

int OS_Run(void);

int OS_Reset(void);

int OS_WakeUp(OSTask_WakeUp enOSTaskWakeUp, OSTask_Grab enOSTaskGrab);

int OS_Sleep(void);

int OS_Start(void);

int OS_Stop(void);

bool OS_GetStatus(void);

int OS_Init(void);

int OS_DeInit(void);

int OS_CreatTask(OSTask_Info * pstOSTaskInfo);

int OS_DestroyTask(int TaskNum, OSTask_Info * pstOSTaskInfo);

int OS_StartTask(int TaskNum);

int OS_StopTask(int TaskNum);

#endif /* OS_H_ */

#ifdef __cplusplus
}
#endif

