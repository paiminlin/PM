/*
    From : https://github.com/paiminlin/PM
    From : https://blog.csdn.net/lpaim/article/details/122160744
    Author : PaiMin.lin
    Date : 2022.7.14
*/

#include "Alarm.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef enum 
{
    Alarm_Sill_Mode         = 0,
    Alarm_Resume_Mode       = 1,
    Alarm_Invalid_Mode      = 2,
}Alarm_Mode;

typedef struct Alarm_Info
{
    bool bAlarmInit;
    bool bAlarmStart;
    bool bAlarmStatus[AlarmTask_MAXNUM];
} Alarm_Info;

static int s_AlarmCounter[AlarmTask_MAXNUM] = {0};
static Alarm_Mode s_enLastAlarmMode[AlarmTask_MAXNUM] = {0};

static Alarm_Info s_stAlarmInfo = {0};
static AlarmTask_Info s_stAlarmTaskInfo[AlarmTask_MAXNUM] = {0};

int Alarm_Run(void)
{
    if(s_stAlarmInfo.bAlarmInit == false || s_stAlarmInfo.bAlarmStart == false)
        return 0;

    int TaskNum = 0;

    for(TaskNum = 0; TaskNum <AlarmTask_MAXNUM; TaskNum ++)
    {
        if(s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun != NULL)
        {
            /*
                报警阈值 > 恢复阈值;
            */
            if(s_stAlarmTaskInfo[TaskNum].AlarmSill.Threshold > s_stAlarmTaskInfo[TaskNum].AlarmResume.Threshold)
            {
                if(s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun() >= s_stAlarmTaskInfo[TaskNum].AlarmSill.Threshold)
                {
                    if(s_enLastAlarmMode[TaskNum] != Alarm_Sill_Mode)
                        s_AlarmCounter[TaskNum] = 0;

                    if(s_stAlarmInfo.bAlarmStatus[TaskNum] == false)
                    {
                        s_AlarmCounter[TaskNum] ++;

                        if(s_AlarmCounter[TaskNum] == s_stAlarmTaskInfo[TaskNum].AlarmSill.duration)
                        {
                            s_stAlarmInfo.bAlarmStatus[TaskNum] = true;
                            s_AlarmCounter[TaskNum] = 0;

                            if(s_stAlarmTaskInfo[TaskNum].AlarmSillFun != NULL)
                                s_stAlarmTaskInfo[TaskNum].AlarmSillFun();
                        }
                    }

                    s_enLastAlarmMode[TaskNum] = Alarm_Sill_Mode;
                }
                else if(s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun() <= s_stAlarmTaskInfo[TaskNum].AlarmResume.Threshold)
                {
                    if(s_enLastAlarmMode[TaskNum] != Alarm_Resume_Mode)
                        s_AlarmCounter[TaskNum] = 0;

                    if(s_stAlarmInfo.bAlarmStatus[TaskNum] == true)
                    {
                        s_AlarmCounter[TaskNum] ++;

                        if(s_AlarmCounter[TaskNum] == s_stAlarmTaskInfo[TaskNum].AlarmResume.duration)
                        {
                            s_stAlarmInfo.bAlarmStatus[TaskNum] = false;
                            s_AlarmCounter[TaskNum] = 0;

                            if(s_stAlarmTaskInfo[TaskNum].AlarmResumeFun != NULL)
                                s_stAlarmTaskInfo[TaskNum].AlarmResumeFun();
                        }
                    }

                    s_enLastAlarmMode[TaskNum] = Alarm_Resume_Mode;
                }
            }

            /*
                报警阈值 < 恢复阈值;
            */
            if(s_stAlarmTaskInfo[TaskNum].AlarmSill.Threshold < s_stAlarmTaskInfo[TaskNum].AlarmResume.Threshold)
            {
                if(s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun() <= s_stAlarmTaskInfo[TaskNum].AlarmSill.Threshold)
                {
                    if(s_enLastAlarmMode[TaskNum] != Alarm_Sill_Mode)
                        s_AlarmCounter[TaskNum] = 0;

                    if(s_stAlarmInfo.bAlarmStatus[TaskNum] == false)
                    {
                        s_AlarmCounter[TaskNum] ++;

                        if(s_AlarmCounter[TaskNum] == s_stAlarmTaskInfo[TaskNum].AlarmSill.duration)
                        {
                            s_stAlarmInfo.bAlarmStatus[TaskNum] = true;
                            s_AlarmCounter[TaskNum] = 0;

                            if(s_stAlarmTaskInfo[TaskNum].AlarmSillFun != NULL)
                                s_stAlarmTaskInfo[TaskNum].AlarmSillFun();
                        }
                    }

                    s_enLastAlarmMode[TaskNum] = Alarm_Sill_Mode;
                }
                else if(s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun() >= s_stAlarmTaskInfo[TaskNum].AlarmResume.Threshold)
                {
                    if(s_enLastAlarmMode[TaskNum] != Alarm_Resume_Mode)
                        s_AlarmCounter[TaskNum] = 0;

                    if(s_stAlarmInfo.bAlarmStatus[TaskNum] == true)
                    {
                        s_AlarmCounter[TaskNum] ++;

                        if(s_AlarmCounter[TaskNum] == s_stAlarmTaskInfo[TaskNum].AlarmResume.duration)
                        {
                            s_stAlarmInfo.bAlarmStatus[TaskNum] = false;
                            s_AlarmCounter[TaskNum] = 0;

                            if(s_stAlarmTaskInfo[TaskNum].AlarmResumeFun != NULL)
                                s_stAlarmTaskInfo[TaskNum].AlarmResumeFun();
                        }
                    }

                    s_enLastAlarmMode[TaskNum] = Alarm_Resume_Mode;
                }
            }
        }
    }
    return 0;
}

int Alarm_Start(void)
{
    if(s_stAlarmInfo.bAlarmInit == false)
        return -1;

    s_stAlarmInfo.bAlarmStart = true;

    return 0;
}

int Alarm_Stop(void)
{
    if(s_stAlarmInfo.bAlarmInit == false)
        return -1;

    s_stAlarmInfo.bAlarmStart = false;

    return 0;
}

bool Alarm_GetStatus(int TaskNum)
{
    if(TaskNum < 0 || TaskNum >= AlarmTask_MAXNUM)
        return -1;

    return s_stAlarmInfo.bAlarmStatus[TaskNum];
}

int Alarm_Init(void)
{
    if(s_stAlarmInfo.bAlarmInit == true)
        return 0;

    int TaskNum = 0;

    s_stAlarmInfo.bAlarmInit = true;
    s_stAlarmInfo.bAlarmStart = false;

    for(TaskNum = 0; TaskNum <AlarmTask_MAXNUM; TaskNum ++)
    {
        s_AlarmCounter[TaskNum] = 0;
        s_stAlarmInfo.bAlarmStatus[TaskNum] = false;
        s_enLastAlarmMode[TaskNum] = Alarm_Invalid_Mode;
        s_stAlarmTaskInfo[TaskNum].AlarmSill.Threshold = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmSill.duration = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmResume.Threshold = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmResume.duration = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun = NULL;
        s_stAlarmTaskInfo[TaskNum].AlarmSillFun = NULL;
        s_stAlarmTaskInfo[TaskNum].AlarmResumeFun = NULL;
    }
    return 0;
}

int Alarm_DeInit(void)
{
    if(s_stAlarmInfo.bAlarmStart == true)
        return -1;

    if(s_stAlarmInfo.bAlarmInit == false)
        return 0;

    int TaskNum = 0;

    s_stAlarmInfo.bAlarmInit = false;
    s_stAlarmInfo.bAlarmStart = false;

    for(TaskNum = 0; TaskNum <AlarmTask_MAXNUM; TaskNum ++)
    {
        s_AlarmCounter[TaskNum] = 0;
        s_stAlarmInfo.bAlarmStatus[TaskNum] = false;
        s_enLastAlarmMode[TaskNum] = Alarm_Invalid_Mode;
        s_stAlarmTaskInfo[TaskNum].AlarmSill.Threshold = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmSill.duration = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmResume.Threshold = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmResume.duration = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun = NULL;
        s_stAlarmTaskInfo[TaskNum].AlarmSillFun = NULL;
        s_stAlarmTaskInfo[TaskNum].AlarmResumeFun = NULL;
    }
    return 0;
}

int Alarm_CreatTask(AlarmTask_Info * pstAlarmTaskInfo)
{
    if(s_stAlarmInfo.bAlarmInit == false)
        return -1;

    int TaskNum = 0;

    if(pstAlarmTaskInfo == NULL)
        return -1;

    for(TaskNum = 0; TaskNum <AlarmTask_MAXNUM; TaskNum ++)
    {
        if(s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun == NULL
            && s_stAlarmTaskInfo[TaskNum].AlarmSillFun == NULL
            && s_stAlarmTaskInfo[TaskNum].AlarmResumeFun == NULL)
        {
            s_AlarmCounter[TaskNum] = 0;
            s_stAlarmInfo.bAlarmStatus[TaskNum] = false;
            s_enLastAlarmMode[TaskNum] = Alarm_Invalid_Mode;
            s_stAlarmTaskInfo[TaskNum].AlarmSill.Threshold = pstAlarmTaskInfo->AlarmSill.Threshold;
            s_stAlarmTaskInfo[TaskNum].AlarmSill.duration = pstAlarmTaskInfo->AlarmSill.duration;
            s_stAlarmTaskInfo[TaskNum].AlarmResume.Threshold = pstAlarmTaskInfo->AlarmResume.Threshold;
            s_stAlarmTaskInfo[TaskNum].AlarmResume.duration = pstAlarmTaskInfo->AlarmResume.duration;
            s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun = pstAlarmTaskInfo->AlarmGetDataFun;
            s_stAlarmTaskInfo[TaskNum].AlarmSillFun = pstAlarmTaskInfo->AlarmSillFun;
            s_stAlarmTaskInfo[TaskNum].AlarmResumeFun = pstAlarmTaskInfo->AlarmResumeFun;
            return TaskNum;
        }
    }
    return -1;
}

int Alarm_DestroyTask(int TaskNum, AlarmTask_Info * pstAlarmTaskInfo)
{
    if(s_stAlarmInfo.bAlarmInit == false)
        return -1;

    if( TaskNum < 0 || TaskNum >= AlarmTask_MAXNUM
        || pstAlarmTaskInfo == NULL)
        return -1;

    if(s_stAlarmTaskInfo[TaskNum].AlarmSill.Threshold == pstAlarmTaskInfo->AlarmSill.Threshold
        && s_stAlarmTaskInfo[TaskNum].AlarmSill.duration == pstAlarmTaskInfo->AlarmSill.duration
        && s_stAlarmTaskInfo[TaskNum].AlarmResume.Threshold == pstAlarmTaskInfo->AlarmResume.Threshold
        && s_stAlarmTaskInfo[TaskNum].AlarmResume.duration == pstAlarmTaskInfo->AlarmResume.duration
        && s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun == pstAlarmTaskInfo->AlarmGetDataFun
        && s_stAlarmTaskInfo[TaskNum].AlarmSillFun == pstAlarmTaskInfo->AlarmSillFun
        && s_stAlarmTaskInfo[TaskNum].AlarmResumeFun == pstAlarmTaskInfo->AlarmResumeFun)
    {
        s_AlarmCounter[TaskNum] = 0;
        s_stAlarmInfo.bAlarmStatus[TaskNum] = false;
        s_enLastAlarmMode[TaskNum] = Alarm_Invalid_Mode;
        s_stAlarmTaskInfo[TaskNum].AlarmSill.Threshold = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmSill.duration = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmResume.Threshold = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmResume.duration = 0;
        s_stAlarmTaskInfo[TaskNum].AlarmGetDataFun = NULL;
        s_stAlarmTaskInfo[TaskNum].AlarmSillFun = NULL;
        s_stAlarmTaskInfo[TaskNum].AlarmResumeFun = NULL;
        return 0;
    }
    return -1;
}

//#define ALARM_MAIN_DEBUG
#ifdef ALARM_MAIN_DEBUG

#include <time.h>

int AlarmGetDataFun(void)
{
    time_t seconds = time((time_t *)NULL);
    struct tm * gmt;
    gmt = localtime(&seconds);
    return gmt->tm_sec;
}

int AlarmSillFun(void)
{
    time_t seconds = time((time_t *)NULL);

    struct tm * gmt;
    gmt = localtime(&seconds);
    printf("%s-%d-",__func__, __LINE__);
    printf("%02d:%02d:%02d\n",gmt->tm_hour,gmt->tm_min,gmt->tm_sec);

    return 0;
}

int AlarmResumeFun(void)
{
    time_t seconds = time((time_t *)NULL);

    struct tm * gmt;
    gmt = localtime(&seconds);
    printf("%s-%d-",__func__, __LINE__);
    printf("%02d:%02d:%02d\n",gmt->tm_hour,gmt->tm_min,gmt->tm_sec);

    return 0;
}

int main()
{
    Alarm_Init();

    Alarm_Start();

    /*
        时间秒数小于或等于10时，维持2次(*1s)->报警
        时间秒数大于或等于30时，维持2次(*1s)->回滞
    */
    AlarmTask_Info stAlarmTaskInfo = {0};
    stAlarmTaskInfo.AlarmSill.Threshold = 10;
    stAlarmTaskInfo.AlarmSill.duration = 2;
    stAlarmTaskInfo.AlarmResume.Threshold = 30;
    stAlarmTaskInfo.AlarmResume.duration = 2;
    stAlarmTaskInfo.AlarmGetDataFun = AlarmGetDataFun;
    stAlarmTaskInfo.AlarmSillFun = AlarmSillFun;
    stAlarmTaskInfo.AlarmResumeFun = AlarmResumeFun;
    int TaskNum = Alarm_CreatTask(&stAlarmTaskInfo);

    while(1)
    {
        Alarm_Run();
        if(Alarm_GetStatus(TaskNum) == true)
        {
            printf("Task in Alarm\n");
        }
        sleep(1);
    }

    Alarm_DestroyTask(TaskNum, &stAlarmTaskInfo);

    Alarm_Stop();

    Alarm_DeInit();

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

