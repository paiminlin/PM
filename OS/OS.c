
#include "OS.h"

#ifdef __cplusplus
extern "C"{
#endif

static int s_10MsConunt = 0;
static int s_100MsConunt = 1;       /* +1 100Ms周期首次左偏移 规避10Ms和100Ms调度并发执行*/

static bool s_bOSStart = false;
static OSTask_WakeUp s_enOSTaskWakeUp = OSTask_WakeUp_Invalid;
static OSTask_WakeUp s_enOSTaskGrab = OSTask_Grab_Invalid;

static bool s_bOSTaskStart[OSTask_MAXNUM] = {0};
static OSTask_Info s_stOSTaskInfo[OSTask_MAXNUM] = {0};

int OS_Run()
{
    if(s_bOSStart == false || s_enOSTaskWakeUp == OSTask_WakeUp_Invalid)
        return 0;

    int TaskNum = 0;

    if(s_enOSTaskWakeUp == OSTask_WakeUp_Grab)
    {
        for(TaskNum = 0; TaskNum < OSTask_MAXNUM; TaskNum ++)
        {
            if(s_bOSTaskStart[TaskNum] == true
                && s_stOSTaskInfo[TaskNum].enOSTaskWakeUp == OSTask_WakeUp_Grab
                && s_stOSTaskInfo[TaskNum].OSTaskRunFun != NULL)
                    s_stOSTaskInfo[TaskNum].OSTaskRunFun();

            if(s_bOSStart == false || s_enOSTaskWakeUp == OSTask_WakeUp_Invalid)
                break;
        }
    }
    else if(s_enOSTaskWakeUp == OSTask_WakeUp_1MSTimer)
    {
        s_10MsConunt ++;
        s_100MsConunt ++;
        /*
            定时器首次唤醒 不触发执行任务
        */
        bool b10MS = s_10MsConunt%10 == 0 ? true : false;
        bool b100MS = s_100MsConunt%100 == 0 ? true : false;

        for(TaskNum = 0; TaskNum < OSTask_MAXNUM; TaskNum ++)
        {
            if(s_bOSTaskStart[TaskNum] == true)
            {
                 if(s_stOSTaskInfo[TaskNum].enOSTaskWakeUp == OSTask_WakeUp_1MSTimer
                    && s_stOSTaskInfo[TaskNum].enOSTaskTimer == OSTask_Timer_1MS
                    && s_stOSTaskInfo[TaskNum].OSTaskRunFun != NULL)
                        s_stOSTaskInfo[TaskNum].OSTaskRunFun();

                if(s_bOSStart == false || s_enOSTaskWakeUp == OSTask_WakeUp_Invalid)
                    break;

                if(b10MS && s_stOSTaskInfo[TaskNum].enOSTaskWakeUp == OSTask_WakeUp_1MSTimer
                    && s_stOSTaskInfo[TaskNum].enOSTaskTimer == OSTask_Timer_10MS
                    && s_stOSTaskInfo[TaskNum].OSTaskRunFun != NULL)
                        s_stOSTaskInfo[TaskNum].OSTaskRunFun();

                if(s_bOSStart == false || s_enOSTaskWakeUp == OSTask_WakeUp_Invalid)
                    break;

                if(b100MS && s_stOSTaskInfo[TaskNum].enOSTaskWakeUp == OSTask_WakeUp_1MSTimer
                    && s_stOSTaskInfo[TaskNum].enOSTaskTimer == OSTask_Timer_100MS
                    && s_stOSTaskInfo[TaskNum].OSTaskRunFun != NULL)
                        s_stOSTaskInfo[TaskNum].OSTaskRunFun();

                if(s_bOSStart == false || s_enOSTaskWakeUp == OSTask_WakeUp_Invalid)
                    break;
            }
        }

        if(s_10MsConunt == 10)
            s_10MsConunt = 0;
        if(s_100MsConunt == 100)
            s_100MsConunt = 0;
    }

    OS_Sleep();

    return 0;
}

int OS_Reset()
{
    OS_Sleep();

    OS_Stop();

    int TaskNum = 0;

    for(TaskNum = 0; TaskNum < OSTask_MAXNUM; TaskNum ++)
    {
        OS_StopTask(TaskNum);
    }

    for(TaskNum = 0; TaskNum < OSTask_MAXNUM; TaskNum ++)
    {
        OS_StartTask(TaskNum);
    }

    s_10MsConunt = 0;
    s_100MsConunt = 1;

    OS_Start();

    return 0;
}

int OS_WakeUp(OSTask_WakeUp enOSTaskWakeUp, OSTask_Grab enOSTaskGrab)
{
    s_enOSTaskWakeUp = enOSTaskWakeUp;
    s_enOSTaskGrab = enOSTaskGrab;

    return 0;
}

int OS_Sleep()
{
    s_enOSTaskWakeUp = OSTask_WakeUp_Invalid;
    s_enOSTaskGrab = OSTask_Grab_Invalid;

    return 0;
}

int OS_Start()
{
    s_bOSStart = true;

    return 0;
}

int OS_Stop()
{
    s_bOSStart = false;
    s_enOSTaskWakeUp = OSTask_WakeUp_Invalid;
    s_enOSTaskGrab = OSTask_Grab_Invalid;

    return 0;
}

int OS_Init()
{
    int TaskNum = 0;

    s_10MsConunt = 0;
    s_100MsConunt = 1;
    s_bOSStart = false;
    s_enOSTaskWakeUp = OSTask_WakeUp_Invalid;
    s_enOSTaskGrab = OSTask_Grab_Invalid;

    for(TaskNum = 0; TaskNum < OSTask_MAXNUM; TaskNum ++)
    {
        s_bOSTaskStart[TaskNum] = false;

        s_stOSTaskInfo[TaskNum].enOSTaskWakeUp = OSTask_WakeUp_Invalid;
        s_stOSTaskInfo[TaskNum].enOSTaskTimer = OSTask_Timer_Invalid;
        s_stOSTaskInfo[TaskNum].OSTaskInitFun = NULL;
        s_stOSTaskInfo[TaskNum].OSTaskDeInitFun = NULL;
        s_stOSTaskInfo[TaskNum].OSTaskRunFun = NULL;
    }

    return 0;
}

int OS_DeInit()
{
    int TaskNum = 0;

    if(s_bOSStart == true)
        return -1;

    s_10MsConunt = 0;
    s_100MsConunt = 1;
    s_bOSStart = false;
    s_enOSTaskWakeUp = OSTask_WakeUp_Invalid;
    s_enOSTaskGrab = OSTask_Grab_Invalid;

    for(TaskNum = 0; TaskNum < OSTask_MAXNUM; TaskNum ++)
    {
        s_bOSTaskStart[TaskNum] = false;

        s_stOSTaskInfo[TaskNum].enOSTaskWakeUp = OSTask_WakeUp_Invalid;
        s_stOSTaskInfo[TaskNum].enOSTaskTimer = OSTask_Timer_Invalid;
        s_stOSTaskInfo[TaskNum].OSTaskRunFun = NULL;
        s_stOSTaskInfo[TaskNum].OSTaskInitFun = NULL;
        s_stOSTaskInfo[TaskNum].OSTaskDeInitFun = NULL;
        s_stOSTaskInfo[TaskNum].OSTaskRunFun = NULL;
    }

    return 0;
}

int OS_CreatTask(OSTask_Info * pstOSTaskInfo)
{
    int TaskNum = 0;

    if(pstOSTaskInfo == NULL)
        return -1;

    for(TaskNum = 0; TaskNum < OSTask_MAXNUM; TaskNum ++)
    {
        if(s_stOSTaskInfo[TaskNum].OSTaskInitFun == NULL
            && s_stOSTaskInfo[TaskNum].OSTaskDeInitFun == NULL
            && s_stOSTaskInfo[TaskNum].OSTaskRunFun == NULL)
        {
            s_bOSTaskStart[TaskNum] = false;

            s_stOSTaskInfo[TaskNum].enOSTaskWakeUp = pstOSTaskInfo->enOSTaskWakeUp;
            s_stOSTaskInfo[TaskNum].enOSTaskTimer = pstOSTaskInfo->enOSTaskTimer;
            s_stOSTaskInfo[TaskNum].OSTaskInitFun = pstOSTaskInfo->OSTaskInitFun;
            s_stOSTaskInfo[TaskNum].OSTaskDeInitFun = pstOSTaskInfo->OSTaskDeInitFun;
            s_stOSTaskInfo[TaskNum].OSTaskRunFun = pstOSTaskInfo->OSTaskRunFun;
            return TaskNum;
        }
    }
    return -1;
}

int OS_DestroyTask(OSTask_Info * pstOSTaskInfo)
{
    int TaskNum = 0;

    if(pstOSTaskInfo == NULL)
        return -1;

    for(TaskNum = 0; TaskNum < OSTask_MAXNUM; TaskNum ++)
    {
        if(s_stOSTaskInfo[TaskNum].enOSTaskWakeUp == pstOSTaskInfo->enOSTaskWakeUp
            && s_stOSTaskInfo[TaskNum].enOSTaskTimer == pstOSTaskInfo->enOSTaskTimer
            && s_stOSTaskInfo[TaskNum].OSTaskInitFun == pstOSTaskInfo->OSTaskInitFun
            && s_stOSTaskInfo[TaskNum].OSTaskDeInitFun == pstOSTaskInfo->OSTaskDeInitFun
            && s_stOSTaskInfo[TaskNum].OSTaskRunFun == pstOSTaskInfo->OSTaskRunFun)
        {
            s_bOSTaskStart[TaskNum] = false;

            s_stOSTaskInfo[TaskNum].enOSTaskWakeUp = OSTask_WakeUp_Invalid;
            s_stOSTaskInfo[TaskNum].enOSTaskTimer = OSTask_Timer_Invalid;
            s_stOSTaskInfo[TaskNum].OSTaskInitFun = NULL;
            s_stOSTaskInfo[TaskNum].OSTaskDeInitFun = NULL;
            s_stOSTaskInfo[TaskNum].OSTaskRunFun = NULL;
            return TaskNum;
        }
    }
    return -1;
}

int OS_StartTask(int TaskNum)
{
    s_bOSTaskStart[TaskNum] = true;
    if(s_stOSTaskInfo[TaskNum].OSTaskInitFun != NULL)
        s_stOSTaskInfo[TaskNum].OSTaskInitFun();

    return 0;
}

int OS_StopTask(int TaskNum)
{
    s_bOSTaskStart[TaskNum] = false;
    if(s_stOSTaskInfo[TaskNum].OSTaskDeInitFun != NULL)
        s_stOSTaskInfo[TaskNum].OSTaskDeInitFun();

    return 0;
}

//#define OS_MAIN_DEBUG
#ifdef OS_MAIN_DEBUG
int main()
{
    int TaskNum = 0;

    OS_Init();

    OSTask_Info stOSTaskInfo[2] =
    {
        /*enOSTaskWakeUp            enOSTaskTimer       OSTaskInitFun   OSTaskDeInitFun OSTaskRunFun*/
        { OSTask_WakeUp_Grab,       OSTask_Timer_10MS,  NULL,           NULL,           NULL},
        { OSTask_WakeUp_1MSTimer,   OSTask_Timer_100MS, NULL,           NULL,           NULL},
    };
    for(TaskNum = 0; TaskNum < sizeof(stOSTaskInfo); TaskNum ++)
        OS_CreatTask(&stOSTaskInfo[TaskNum]);

    for(TaskNum = 0; TaskNum < sizeof(stOSTaskInfo); TaskNum ++)
        OS_StartTask(TaskNum);

    OS_Start();

    while(1)
    {
        OS_Run();
    }

    OS_Stop();

    for(TaskNum = 0; TaskNum < sizeof(stOSTaskInfo); TaskNum ++)
        OS_StopTask(TaskNum);

    for(TaskNum = 0; TaskNum < sizeof(stOSTaskInfo); TaskNum ++)
        OS_DestroyTask(&stOSTaskInfo[TaskNum]);

    OS_DeInit();

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

