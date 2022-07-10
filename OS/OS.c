
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

int OS_Run(void)
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

int OS_Reset(void)
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

int OS_Sleep(void)
{
    s_enOSTaskWakeUp = OSTask_WakeUp_Invalid;
    s_enOSTaskGrab = OSTask_Grab_Invalid;

    return 0;
}

int OS_Start(void)
{
    s_bOSStart = true;

    return 0;
}

int OS_Stop(void)
{
    s_bOSStart = false;
    s_enOSTaskWakeUp = OSTask_WakeUp_Invalid;
    s_enOSTaskGrab = OSTask_Grab_Invalid;

    return 0;
}

bool OS_GetStatus(void)
{
    return s_bOSStart;
}

int OS_Init(void)
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

int OS_DeInit(void)
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

int OS_DestroyTask(int TaskNum, OSTask_Info * pstOSTaskInfo)
{
    if(TaskNum < 0 || TaskNum >= OSTask_MAXNUM 
        || pstOSTaskInfo == NULL)
        return -1;

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
        return 0;
    }
    return -1;
}

int OS_StartTask(int TaskNum)
{
    if(TaskNum < 0 || TaskNum >= OSTask_MAXNUM)
        return -1;

    s_bOSTaskStart[TaskNum] = true;
    if(s_stOSTaskInfo[TaskNum].OSTaskInitFun != NULL)
        s_stOSTaskInfo[TaskNum].OSTaskInitFun();

    return 0;
}

int OS_StopTask(int TaskNum)
{
    if(TaskNum < 0 || TaskNum >= OSTask_MAXNUM)
        return -1;

    s_bOSTaskStart[TaskNum] = false;
    if(s_stOSTaskInfo[TaskNum].OSTaskDeInitFun != NULL)
        s_stOSTaskInfo[TaskNum].OSTaskDeInitFun();

    return 0;
}

//#define OS_MAIN_DEBUG
#ifdef OS_MAIN_DEBUG

#include <time.h>

int OSTaskInitFunEvent1(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}
int OSTaskDeInitFunEvent1(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}
int OSTaskRunFunEvent1(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    OS_Stop();
    return 0;
}

int OSTaskInitFun1MS(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}
int OSTaskDeInitFun1MS(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}
int OSTaskRunFun1MS(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}

int OSTaskInitFun10MS(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}
int OSTaskDeInitFun10MS(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}
int OSTaskRunFun10MS(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}

int OSTaskInitFun100MS(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}
int OSTaskDeInitFun100MS(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}
int OSTaskRunFun100MS(void)
{
    printf("%s-%d\n",__func__, __LINE__);
    return 0;
}

int main()
{
    int TaskNum = 0;

    OS_Init();

    OSTask_Info stOSTaskInfo[4] =
    {
        /*enOSTaskWakeUp          enOSTaskGrab         enOSTaskTimer         OSTaskInitFun        OSTaskDeInitFun        OSTaskRunFun*/
        { OSTask_WakeUp_Grab,     OSTask_Grab_Event1,  OSTask_Timer_Invalid, OSTaskInitFunEvent1, OSTaskDeInitFunEvent1, OSTaskRunFunEvent1},
        { OSTask_WakeUp_1MSTimer, OSTask_Grab_Invalid, OSTask_Timer_1MS,     OSTaskInitFun1MS,    OSTaskDeInitFun1MS,    OSTaskRunFun1MS},
        { OSTask_WakeUp_1MSTimer, OSTask_Grab_Invalid, OSTask_Timer_10MS,    OSTaskInitFun10MS,   OSTaskDeInitFun10MS,   OSTaskRunFun10MS},
        { OSTask_WakeUp_1MSTimer, OSTask_Grab_Invalid, OSTask_Timer_100MS,   OSTaskInitFun100MS,  OSTaskDeInitFun100MS,  OSTaskRunFun100MS},
    };
    int TableLength = sizeof(stOSTaskInfo)/sizeof(OSTask_Info);

    for(TaskNum = 0; TaskNum < TableLength; TaskNum ++)
        OS_CreatTask(&stOSTaskInfo[TaskNum]);

    for(TaskNum = 0; TaskNum < TableLength; TaskNum ++)
        OS_StartTask(TaskNum);

    OS_Start();

    time_t timestart,timeend;
    timestart = time(NULL);

    while(1)
    {
        if(OS_GetStatus() == false)
            break;

        timeend = time(NULL);
        if(difftime(timeend, timestart) == 1)
        {
            timestart = timeend;
            OS_WakeUp(OSTask_WakeUp_1MSTimer, OSTask_Grab_Invalid);
        }

        struct tm * gmt;
        gmt = localtime(&timeend);
        if(gmt->tm_sec == 0)
        {
            OS_WakeUp(OSTask_WakeUp_Grab, OSTask_Grab_Event1);
        }

        OS_Run();
    }

    OS_Stop();

    for(TaskNum = 0; TaskNum < TableLength; TaskNum ++)
        OS_StopTask(TaskNum);

    for(TaskNum = 0; TaskNum < TableLength; TaskNum ++)
        OS_DestroyTask(TaskNum, &stOSTaskInfo[TaskNum]);

    OS_DeInit();

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

