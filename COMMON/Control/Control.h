
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef Control_H_
#define Control_H_

#define ControlTask_MAXNUM         100      /* 支持最多控制对象数量 */

typedef int (*ControlStart_Fun)(void);

typedef int (*ControlStop_Fun)(void);

typedef struct ControTask_Info
{
    ControlStart_Fun ControlStartFun;       /* 亮 */
    ControlStop_Fun ControlStopFun;         /* 灭 */
} ControTask_Info;

typedef enum 
{
    Control_StartMode = 0,              /* 常亮 */
    Control_StopMode,                   /* 常灭 */
    Control_LimitedFlashMode,           /* 亮灭闪烁(有限) */
    Control_UnlimitedFlashMode,         /* 亮灭闪烁(无限) */
    Control_InvalidMode,                /* 无效 */
}Control_Mode;

typedef struct ControlFlash_Info
{
    int RepeatTimes;                        /* 重复次数 */
    int StartTimes;                         /* 亮次数(*Control_Run) */
    int StopTimes;                          /* 灭次数(*Control_Run) */
    int IntermittentTimes;                  /* 间隙(灭)次数(*Control_Run) */
} ControlFlash_Info;

int Control_Run(void);

int Control_SetMode(int TaskNum, Control_Mode enControlMode, 
                                ControlFlash_Info stControlFlashInfo);

int Control_GetMode(int TaskNum, Control_Mode *penControlMode, 
                                ControlFlash_Info *pstControlFlashInfo);

int Control_Init(void);

int Control_DeInit(void);

int Control_CreatTask(ControTask_Info * pstControlTaskInfo);

int Control_DestroyTask(int TaskNum, ControTask_Info * pstControlTaskInfo);

#endif /* Control_H_ */

#ifdef __cplusplus
}
#endif

