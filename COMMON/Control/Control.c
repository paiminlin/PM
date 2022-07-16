/*
    From : https://github.com/paiminlin/PM
    From : https://blog.csdn.net/lpaim/article/details/125649860
    Author : PaiMin.lin
    Date : 2022.7.16
*/

#include "Control.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Control_Info
{
    bool bControlInit;
    bool bControlStart;
    bool bControlTaskCreat[ControlTask_MAXNUM];
    Control_Mode enControlTaskMode[ControlTask_MAXNUM];
    ControlFlash_Info stControlTaskFlashInfo[ControlTask_MAXNUM];
} Control_Info;

static bool s_bUpdateControlTaskFlashFlage[ControlTask_MAXNUM] = {0};
static ControlFlash_Info s_stUpdateControlTaskFlashInfo[ControlTask_MAXNUM] = {0};

static Control_Info s_stControlInfo = {0};
static ControTask_Info s_stControlTaskInfo[ControlTask_MAXNUM] = {0};

int Control_Run(void)
{
    if(s_stControlInfo.bControlInit == false || s_stControlInfo.bControlStart == false)
        return 0;

    int TaskNum = 0;

    for(TaskNum = 0; TaskNum < ControlTask_MAXNUM; TaskNum ++)
    {
        if(s_stControlInfo.enControlTaskMode[TaskNum] == Control_StartMode)
        {
            if(s_stControlTaskInfo[TaskNum].ControlStartFun != NULL)
                s_stControlTaskInfo[TaskNum].ControlStartFun();
        }
        else if(s_stControlInfo.enControlTaskMode[TaskNum] == Control_StopMode)
        {
            if(s_stControlTaskInfo[TaskNum].ControlStopFun != NULL)
                s_stControlTaskInfo[TaskNum].ControlStopFun();
        }
        else if(s_stControlInfo.enControlTaskMode[TaskNum] == Control_LimitedFlashMode)
        {
            if(s_bUpdateControlTaskFlashFlage[TaskNum] == true)
            {
                s_bUpdateControlTaskFlashFlage[TaskNum] = false;
                s_stUpdateControlTaskFlashInfo[TaskNum].RepeatTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].RepeatTimes;
                s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].StartTimes;
                s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].StopTimes;
                s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].IntermittentTimes;
            }

            if(s_stUpdateControlTaskFlashInfo[TaskNum].RepeatTimes > 0)
            {
                if(s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes > 0)
                {
                    if(s_stControlTaskInfo[TaskNum].ControlStartFun != NULL)
                        s_stControlTaskInfo[TaskNum].ControlStartFun();
                    s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes --;
                }
                else if(s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes > 0)
                {
                    if(s_stControlTaskInfo[TaskNum].ControlStopFun != NULL)
                        s_stControlTaskInfo[TaskNum].ControlStopFun();
                    s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes --;
                }
                else if(s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes > 0)
                {
                    s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes --;
                }

                if(s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes == 0
                    && s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes == 0
                    &&s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes == 0)
                {
                    s_stUpdateControlTaskFlashInfo[TaskNum].RepeatTimes --;
                    s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].StartTimes;
                    s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].StopTimes;
                    s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].IntermittentTimes;
                }
            }
            else
            {
                s_stControlInfo.enControlTaskMode[TaskNum] = Control_StopMode;
            }
        }
        else if(s_stControlInfo.enControlTaskMode[TaskNum] == Control_UnlimitedFlashMode)
        {
            if(s_bUpdateControlTaskFlashFlage[TaskNum] == true)
            {
                s_bUpdateControlTaskFlashFlage[TaskNum] = false;
                s_stUpdateControlTaskFlashInfo[TaskNum].RepeatTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].RepeatTimes;
                s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].StartTimes;
                s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].StopTimes;
                s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].IntermittentTimes;
            }

            if(s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes > 0)
            {
                if(s_stControlTaskInfo[TaskNum].ControlStartFun != NULL)
                    s_stControlTaskInfo[TaskNum].ControlStartFun();
                s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes --;
            }
            else if(s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes > 0)
            {
                if(s_stControlTaskInfo[TaskNum].ControlStopFun != NULL)
                    s_stControlTaskInfo[TaskNum].ControlStopFun();
                s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes --;
            }
            else if(s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes > 0)
            {
                s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes --;
            }

            if(s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes == 0
                && s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes == 0
                &&s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes == 0)
            {
                s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].StartTimes;
                s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].StopTimes;
                s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].IntermittentTimes;
            }
        }
    }
    return 0;
}

int Control_Start(void)
{
    if(s_stControlInfo.bControlInit == false)
        return -1;

    s_stControlInfo.bControlStart = true;

    return 0;
}

int Control_Stop(void)
{
    if(s_stControlInfo.bControlInit == false)
        return -1;

    s_stControlInfo.bControlStart = false;

    return 0;
}

int Control_Init(void)
{
    if(s_stControlInfo.bControlInit == true)
        return 0;

    int TaskNum = 0;

    s_stControlInfo.bControlInit = true;
    s_stControlInfo.bControlStart = false;

    for(TaskNum = 0; TaskNum < ControlTask_MAXNUM; TaskNum ++)
    {
        s_bUpdateControlTaskFlashFlage[TaskNum] = false;
        s_stUpdateControlTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes = 0;
        s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes = 0;
        s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes = 0;
        s_stControlInfo.bControlTaskCreat[TaskNum] = false;
        s_stControlInfo.enControlTaskMode[TaskNum] = Control_InvalidMode;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].StartTimes = 0;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].StopTimes = 0;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].IntermittentTimes = 0;
        s_stControlTaskInfo[TaskNum].ControlStartFun = NULL;
        s_stControlTaskInfo[TaskNum].ControlStopFun = NULL;
    }
    return 0;
}

int Control_DeInit(void)
{
    if(s_stControlInfo.bControlStart == true)
        return -1;

    if(s_stControlInfo.bControlInit == false)
        return 0;

    int TaskNum = 0;

    s_stControlInfo.bControlInit = false;
    s_stControlInfo.bControlStart = false;

    for(TaskNum = 0; TaskNum < ControlTask_MAXNUM; TaskNum ++)
    {
        s_bUpdateControlTaskFlashFlage[TaskNum] = false;
        s_stUpdateControlTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes = 0;
        s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes = 0;
        s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes = 0;
        s_stControlInfo.bControlTaskCreat[TaskNum] = false;
        s_stControlInfo.enControlTaskMode[TaskNum] = Control_InvalidMode;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].StartTimes = 0;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].StopTimes = 0;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].IntermittentTimes = 0;
        s_stControlTaskInfo[TaskNum].ControlStartFun = NULL;
        s_stControlTaskInfo[TaskNum].ControlStopFun = NULL;
    }
    return 0;
}

int Control_CreatTask(ControTask_Info * pstControlTaskInfo)
{
    if(s_stControlInfo.bControlInit == false)
        return -1;

    int TaskNum = 0;

    if(pstControlTaskInfo == NULL)
        return -1;

    for(TaskNum = 0; TaskNum <ControlTask_MAXNUM; TaskNum ++)
    {
        if(s_stControlTaskInfo[TaskNum].ControlStartFun == NULL
            && s_stControlTaskInfo[TaskNum].ControlStopFun == NULL)
        {
            s_bUpdateControlTaskFlashFlage[TaskNum] = false;
            s_stUpdateControlTaskFlashInfo[TaskNum].RepeatTimes = 0;
            s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes = 0;
            s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes = 0;
            s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes = 0;
            s_stControlInfo.bControlTaskCreat[TaskNum] = true;
            s_stControlInfo.enControlTaskMode[TaskNum] = Control_InvalidMode;
            s_stControlInfo.stControlTaskFlashInfo[TaskNum].RepeatTimes = 0;
            s_stControlInfo.stControlTaskFlashInfo[TaskNum].StartTimes = 0;
            s_stControlInfo.stControlTaskFlashInfo[TaskNum].StopTimes = 0;
            s_stControlInfo.stControlTaskFlashInfo[TaskNum].IntermittentTimes = 0;
            s_stControlTaskInfo[TaskNum].ControlStartFun = pstControlTaskInfo->ControlStartFun;
            s_stControlTaskInfo[TaskNum].ControlStopFun = pstControlTaskInfo->ControlStopFun;
            return TaskNum;
        }
    }
    return -1;
}

int Control_DestroyTask(int TaskNum, ControTask_Info * pstControlTaskInfo)
{
    if(s_stControlInfo.bControlInit == false)
        return -1;

    if(TaskNum < 0 || TaskNum >= ControlTask_MAXNUM
        || pstControlTaskInfo == NULL)
        return -1;

    if(s_stControlTaskInfo[TaskNum].ControlStartFun == pstControlTaskInfo->ControlStartFun
        && s_stControlTaskInfo[TaskNum].ControlStartFun == pstControlTaskInfo->ControlStartFun)
    {
        s_bUpdateControlTaskFlashFlage[TaskNum] = false;
        s_stUpdateControlTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stUpdateControlTaskFlashInfo[TaskNum].StartTimes = 0;
        s_stUpdateControlTaskFlashInfo[TaskNum].StopTimes = 0;
        s_stUpdateControlTaskFlashInfo[TaskNum].IntermittentTimes = 0;
        s_stControlInfo.bControlTaskCreat[TaskNum] = false;
        s_stControlInfo.enControlTaskMode[TaskNum] = Control_InvalidMode;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].StartTimes = 0;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].StopTimes = 0;
        s_stControlInfo.stControlTaskFlashInfo[TaskNum].IntermittentTimes = 0;
        s_stControlTaskInfo[TaskNum].ControlStartFun = NULL;
        s_stControlTaskInfo[TaskNum].ControlStopFun = NULL;
        return 0;
    }
    return -1;
}

int Control_SetTaskMode(int TaskNum, Control_Mode enControlTaskMode, 
                                ControlFlash_Info stControlTaskFlashInfo)
{
    if(TaskNum < 0 || TaskNum >= ControlTask_MAXNUM)
        return -1;

    if(s_stControlInfo.bControlStart == false || s_stControlInfo.bControlTaskCreat[TaskNum] == false)
        return -1;

    s_bUpdateControlTaskFlashFlage[TaskNum] = true;
    s_stControlInfo.enControlTaskMode[TaskNum] = enControlTaskMode;
    s_stControlInfo.stControlTaskFlashInfo[TaskNum].RepeatTimes = stControlTaskFlashInfo.RepeatTimes;
    s_stControlInfo.stControlTaskFlashInfo[TaskNum].StartTimes = stControlTaskFlashInfo.StartTimes;
    s_stControlInfo.stControlTaskFlashInfo[TaskNum].StopTimes = stControlTaskFlashInfo.StopTimes;
    s_stControlInfo.stControlTaskFlashInfo[TaskNum].IntermittentTimes = stControlTaskFlashInfo.IntermittentTimes;

    return 0;
}

int Control_GetTaskMode(int TaskNum, Control_Mode *penControlTaskMode, 
                                ControlFlash_Info *pstControlTaskFlashInfo)
{
    if(TaskNum < 0 || TaskNum >= ControlTask_MAXNUM
        || penControlTaskMode == NULL || pstControlTaskFlashInfo == NULL)
        return -1;

    *penControlTaskMode = s_stControlInfo.enControlTaskMode[TaskNum];
    if(*penControlTaskMode == Control_LimitedFlashMode
        || *penControlTaskMode == Control_UnlimitedFlashMode)
    {
        pstControlTaskFlashInfo->RepeatTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].RepeatTimes;
        pstControlTaskFlashInfo->StartTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].StartTimes;
        pstControlTaskFlashInfo->StopTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].StopTimes;
        pstControlTaskFlashInfo->IntermittentTimes = s_stControlInfo.stControlTaskFlashInfo[TaskNum].IntermittentTimes;
    }
    else
    {
        pstControlTaskFlashInfo->RepeatTimes = 0;
        pstControlTaskFlashInfo->StartTimes = 0;
        pstControlTaskFlashInfo->StopTimes = 0;
        pstControlTaskFlashInfo->IntermittentTimes = 0;
    }
    return 0;
}

//#define Control_MAIN_DEBUG
#ifdef Control_MAIN_DEBUG

int Start(void)
{
    printf("%s-%d\n",__func__, __LINE__);

    return 0;
}

int Stop(void)
{
    printf("%s-%d\n",__func__, __LINE__);

    return 0;
}

int main()
{
    Control_Init();

    Control_Start();

    ControTask_Info stControlTaskInfo = {0};
    stControlTaskInfo.ControlStartFun = Start;
    stControlTaskInfo.ControlStopFun = Stop;
    int TaskNum = Control_CreatTask(&stControlTaskInfo);

    ControlFlash_Info stControlTaskFlashInfo = {0};
    stControlTaskFlashInfo.RepeatTimes = 3;
    stControlTaskFlashInfo.StartTimes = 3;
    stControlTaskFlashInfo.StopTimes = 3;
    stControlTaskFlashInfo.IntermittentTimes = 3;
    Control_SetTaskMode(TaskNum, Control_LimitedFlashMode, stControlTaskFlashInfo);

    while(1)
    {
        Control_Mode enControlTaskMode = Control_InvalidMode;
        ControlFlash_Info stControlTaskFlashInfo = {0};
        Control_GetTaskMode(TaskNum, &enControlTaskMode, &stControlTaskFlashInfo);
        if(enControlTaskMode == Control_StopMode)
        {
            break;
        }
        Control_Run();
        sleep(1);
    }

    Control_DestroyTask(TaskNum, &stControlTaskInfo);

    Control_Stop();

    Control_DeInit();

    return 0;
}

#endif

#ifdef __cplusplus
}
#endif

