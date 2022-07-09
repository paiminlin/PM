
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef ALARM_H_
#define ALARM_H_

#define AlarmTask_MAXNUM                100     /* 支持最多管理的报警对象数量 */

typedef struct Alarm_Info
{
    int Threshold;                              /* 阈值 */
    int duration;                               /* 维持时间 duration * Alarm_Run */
} Alarm_Info;

typedef int (*AlarmGetData_Fun)(void);

typedef int (*AlarmSill_Fun)(void);

typedef int (*AlarmResume_Fun)(void);

typedef struct AlarmTask_Info
{
    Alarm_Info AlarmSill;                       /* 报警阈值/维持时间 */
    Alarm_Info AlarmResume;                     /* 回滞阈值/维持时间 */
    AlarmGetData_Fun AlarmGetDataFun;           /* 获取报警对象 返回报警对象数据 */
    AlarmSill_Fun AlarmSillFun;                 /* 触发报警 */
    AlarmResume_Fun AlarmResumeFun;             /* 触发回滞 */
} AlarmTask_Info;

int Alarm_Run(void);

int Alarm_Start(void);

int Alarm_Stop(void);

bool Alarm_GetStatus(int TaskNum);

int Alarm_Init(void);

int Alarm_DeInit(void);

int Alarm_CreatTask(AlarmTask_Info * pstAlarmTaskInfo);

int Alarm_DestroyTask(int TaskNum, AlarmTask_Info * pstAlarmTaskInfo);

#endif /* ALARM_H_ */

#ifdef __cplusplus
}
#endif

