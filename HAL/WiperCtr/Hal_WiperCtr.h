/*
    From : https://github.com/paiminlin/PM
    From : 
    Author : PaiMin.lin
    Date : 2023.3.12
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef HAL_WIPERCTR_H_
#define HAL_WIPERCTR_H_

#define HAL_WIPERCTRTASK_MAXNUM         2           /* 支持最多控制对象数量 */

typedef enum 
{
    HAL_WIPERCTR_OFF_MODE = 0,                      /* 关闭 */
    HAL_WIPERCTR_LOW_MODE,                          /* 低速 */
    HAL_WIPERCTR_HIGH_MODE,                         /* 高速 */
    HAL_WIPERCTR_AUTO_MODE,                         /* 自动 */
    HAL_WIPERCTR_WASH_MODE,                         /* 洗涤 */
    HAL_WIPERCTR_REPAIR_MODE,                       /* 维修 */
    HAL_WIPERCTR_INVALID_MODE,                      /* 无效 */
}Hal_WiperCtr_Mode;

typedef int (*Hal_WiperCtrReset_Fun)(bool bResetChange);

typedef int (*Hal_WiperCtrControl_Fun)(Hal_WiperCtr_Mode enHalWiperCtrMode);

typedef struct Hal_WiperCtrOff_Info
{
    int KeepResetTimes;                             /* 重复次数 */
} Hal_WiperCtrOff_Info;

typedef struct Hal_WiperCtrAuto_Info
{
} Hal_WiperCtrAuto_Info;

typedef struct Hal_WiperCtrWash_Info
{
    int LOW1RepeatTimes;                            /* 重复次数 */
    int OFFTimes;                                   /* 关闭维持时间       */
    int LOW2RepeatTimes;                            /* 重复次数 */
} Hal_WiperCtrWash_Info;

typedef struct Hal_WiperCtrRepair_Info
{
    int LOWKeepTimes;                               /* Low 维持时间 */
} Hal_WiperCtrRepair_Info;

typedef struct Hal_WiperCtrTask_Info
{
    Hal_WiperCtrOff_Info stHalWiperCtrOffInfo;
    Hal_WiperCtrAuto_Info stHalWiperCtrAutoInfo;
    Hal_WiperCtrWash_Info stHal_WiperCtrWashInfo;
    Hal_WiperCtrRepair_Info stHal_WiperCtrRepairInfo;
    Hal_WiperCtrReset_Fun HalWiperCtrResetFun;      /*  */
    Hal_WiperCtrControl_Fun HalWiperCtrControlFun;  /*  */
} Hal_WiperCtrTask_Info;

int Hal_WiperCtr_Run(void);

int Hal_WiperCtr_Init(void);

int Hal_WiperCtr_DeInit(void);

int Hal_WiperCtr_CreatTask(Hal_WiperCtrTask_Info * pstHalWiperCtrTaskInfo);

int Hal_WiperCtr_DestroyTask(int TaskNum, Hal_WiperCtrTask_Info * pstHalWiperCtrTaskInfo);

int Hal_WiperCtr_SetMode(int TaskNum, Hal_WiperCtr_Mode enWiperCtrTaskMode);

int Hal_WiperCtr_GetMode(int TaskNum, Hal_WiperCtr_Mode *penWiperCtrTaskMode);

#endif /* HAL_WIPERCTR_H_ */

#ifdef __cplusplus
}
#endif

