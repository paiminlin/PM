
#include "Control.h"

#ifdef __cplusplus
extern "C"{
#endif

static ControTask_Info s_stControlTaskInfo[ControlTask_MAXNUM] = {0};
static Control_Mode s_enControlMode[ControlTask_MAXNUM] = {0};
static ControlFlash_Info s_stControlFlashInfo[ControlTask_MAXNUM] = {0};
static bool s_bUpdateControlFlashFlage[ControlTask_MAXNUM] = {0};
static ControlFlash_Info s_stUpdateControlFlashInfo[ControlTask_MAXNUM] = {0};

int Control_Run(void)
{
    int TaskNum = 0;

    for(TaskNum = 0; TaskNum < ControlTask_MAXNUM; TaskNum ++)
    {
        if(s_enControlMode[TaskNum] == Control_StartMode)
        {
            if(s_stControlTaskInfo[TaskNum].ControlStartFun != NULL)
                s_stControlTaskInfo[TaskNum].ControlStartFun();
        }
        else if(s_enControlMode[TaskNum] == Control_StopMode)
        {
            if(s_stControlTaskInfo[TaskNum].ControlStopFun != NULL)
                s_stControlTaskInfo[TaskNum].ControlStopFun();
        }
        else if(s_enControlMode[TaskNum] == Control_LimitedFlashMode)
        {
            if(s_bUpdateControlFlashFlage[TaskNum] == true)
            {
                s_bUpdateControlFlashFlage[TaskNum] = false;
                s_stUpdateControlFlashInfo[TaskNum].RepeatTimes = s_stControlFlashInfo[TaskNum].RepeatTimes;
                s_stUpdateControlFlashInfo[TaskNum].StartTimes = s_stControlFlashInfo[TaskNum].StartTimes;
                s_stUpdateControlFlashInfo[TaskNum].StopTimes = s_stControlFlashInfo[TaskNum].StopTimes;
                s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes = s_stControlFlashInfo[TaskNum].IntermittentTimes;
            }

            if(s_stUpdateControlFlashInfo[TaskNum].RepeatTimes > 0)
            {
                if(s_stUpdateControlFlashInfo[TaskNum].StartTimes > 0)
                {
                    if(s_stControlTaskInfo[TaskNum].ControlStartFun != NULL)
                        s_stControlTaskInfo[TaskNum].ControlStartFun();
                    s_stUpdateControlFlashInfo[TaskNum].StartTimes --;
                }
                else if(s_stUpdateControlFlashInfo[TaskNum].StopTimes > 0)
                {
                    if(s_stControlTaskInfo[TaskNum].ControlStopFun != NULL)
                        s_stControlTaskInfo[TaskNum].ControlStopFun();
                    s_stUpdateControlFlashInfo[TaskNum].StopTimes --;
                }
                else if(s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes > 0)
                {
                    s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes --;
                }

                if(s_stUpdateControlFlashInfo[TaskNum].StartTimes == 0
                    && s_stUpdateControlFlashInfo[TaskNum].StopTimes == 0
                    &&s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes == 0)
                {
                    s_stUpdateControlFlashInfo[TaskNum].RepeatTimes --;
                    s_stUpdateControlFlashInfo[TaskNum].StartTimes = s_stControlFlashInfo[TaskNum].StartTimes;
                    s_stUpdateControlFlashInfo[TaskNum].StopTimes = s_stControlFlashInfo[TaskNum].StopTimes;
                    s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes = s_stControlFlashInfo[TaskNum].IntermittentTimes;
                }
            }
            else
            {
                s_enControlMode[TaskNum] = Control_StopMode;
            }
        }
        else if(s_enControlMode[TaskNum] == Control_UnlimitedFlashMode)
        {
            if(s_bUpdateControlFlashFlage[TaskNum] == true)
            {
                s_bUpdateControlFlashFlage[TaskNum] = false;
                s_stUpdateControlFlashInfo[TaskNum].RepeatTimes = s_stControlFlashInfo[TaskNum].RepeatTimes;
                s_stUpdateControlFlashInfo[TaskNum].StartTimes = s_stControlFlashInfo[TaskNum].StartTimes;
                s_stUpdateControlFlashInfo[TaskNum].StopTimes = s_stControlFlashInfo[TaskNum].StopTimes;
                s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes = s_stControlFlashInfo[TaskNum].IntermittentTimes;
            }

            if(s_stUpdateControlFlashInfo[TaskNum].StartTimes > 0)
            {
                if(s_stControlTaskInfo[TaskNum].ControlStartFun != NULL)
                    s_stControlTaskInfo[TaskNum].ControlStartFun();
                s_stUpdateControlFlashInfo[TaskNum].StartTimes --;
            }
            else if(s_stUpdateControlFlashInfo[TaskNum].StopTimes > 0)
            {
                if(s_stControlTaskInfo[TaskNum].ControlStopFun != NULL)
                    s_stControlTaskInfo[TaskNum].ControlStopFun();
                s_stUpdateControlFlashInfo[TaskNum].StopTimes --;
            }
            else if(s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes > 0)
            {
                s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes --;
            }

            if(s_stUpdateControlFlashInfo[TaskNum].StartTimes == 0
                && s_stUpdateControlFlashInfo[TaskNum].StopTimes == 0
                &&s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes == 0)
            {
                s_stUpdateControlFlashInfo[TaskNum].StartTimes = s_stControlFlashInfo[TaskNum].StartTimes;
                s_stUpdateControlFlashInfo[TaskNum].StopTimes = s_stControlFlashInfo[TaskNum].StopTimes;
                s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes = s_stControlFlashInfo[TaskNum].IntermittentTimes;
            }
        }
    }
    return 0;
}

int Control_SetMode(int TaskNum, Control_Mode enControlMode, 
                                ControlFlash_Info stControlFlashInfo)
{
    if(TaskNum < 0 || TaskNum >= ControlTask_MAXNUM)
        return -1;

    s_enControlMode[TaskNum] = enControlMode;
    s_bUpdateControlFlashFlage[TaskNum] = true;
    s_stControlFlashInfo[TaskNum].RepeatTimes = stControlFlashInfo.RepeatTimes;
    s_stControlFlashInfo[TaskNum].StartTimes = stControlFlashInfo.StartTimes;
    s_stControlFlashInfo[TaskNum].StopTimes = stControlFlashInfo.StopTimes;
    s_stControlFlashInfo[TaskNum].IntermittentTimes = stControlFlashInfo.IntermittentTimes;

    return 0;
}

int Control_GetMode(int TaskNum, Control_Mode *penControlMode, 
                                ControlFlash_Info *pstControlFlashInfo)
{
    if(TaskNum < 0 || TaskNum >= ControlTask_MAXNUM
        || penControlMode == NULL || pstControlFlashInfo == NULL)
        return -1;

    *penControlMode = s_enControlMode[TaskNum];
    if(*penControlMode == Control_LimitedFlashMode
        || *penControlMode == Control_UnlimitedFlashMode)
    {
        pstControlFlashInfo->RepeatTimes = s_stControlFlashInfo[TaskNum].RepeatTimes;
        pstControlFlashInfo->StartTimes = s_stControlFlashInfo[TaskNum].StartTimes;
        pstControlFlashInfo->StopTimes = s_stControlFlashInfo[TaskNum].StopTimes;
        pstControlFlashInfo->IntermittentTimes = s_stControlFlashInfo[TaskNum].IntermittentTimes;
    }
    else
    {
        pstControlFlashInfo->RepeatTimes = 0;
        pstControlFlashInfo->StartTimes = 0;
        pstControlFlashInfo->StopTimes = 0;
        pstControlFlashInfo->IntermittentTimes = 0;
    }
    return 0;
}

int Control_Init(void)
{
    int TaskNum = 0;

    for(TaskNum = 0; TaskNum < ControlTask_MAXNUM; TaskNum ++)
    {
        s_stControlTaskInfo[TaskNum].ControlStartFun = NULL;
        s_stControlTaskInfo[TaskNum].ControlStopFun = NULL;
        s_enControlMode[TaskNum] = Control_InvalidMode;
        s_stControlFlashInfo[TaskNum].RepeatTimes = 0;
        s_stControlFlashInfo[TaskNum].StartTimes = 0;
        s_stControlFlashInfo[TaskNum].StopTimes = 0;
        s_stControlFlashInfo[TaskNum].IntermittentTimes = 0;
        s_bUpdateControlFlashFlage[TaskNum] = false;
        s_stUpdateControlFlashInfo[TaskNum].RepeatTimes = 0;
        s_stUpdateControlFlashInfo[TaskNum].StartTimes = 0;
        s_stUpdateControlFlashInfo[TaskNum].StopTimes = 0;
        s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes = 0;
    }
    return 0;
}

int Control_DeInit(void)
{
    int TaskNum = 0;

    for(TaskNum = 0; TaskNum < ControlTask_MAXNUM; TaskNum ++)
    {
        s_stControlTaskInfo[TaskNum].ControlStartFun = NULL;
        s_stControlTaskInfo[TaskNum].ControlStopFun = NULL;
        s_enControlMode[TaskNum] = Control_InvalidMode;
        s_stControlFlashInfo[TaskNum].RepeatTimes = 0;
        s_stControlFlashInfo[TaskNum].StartTimes = 0;
        s_stControlFlashInfo[TaskNum].StopTimes = 0;
        s_stControlFlashInfo[TaskNum].IntermittentTimes = 0;
        s_bUpdateControlFlashFlage[TaskNum] = false;
        s_stUpdateControlFlashInfo[TaskNum].RepeatTimes = 0;
        s_stUpdateControlFlashInfo[TaskNum].StartTimes = 0;
        s_stUpdateControlFlashInfo[TaskNum].StopTimes = 0;
        s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes = 0;
    }
    return 0;
}

int Control_CreatTaskInfo(ControTask_Info * pstControlTaskInfo)
{
    int TaskNum = 0;

    if(pstControlTaskInfo == NULL)
        return -1;

    for(TaskNum = 0; TaskNum <ControlTask_MAXNUM; TaskNum ++)
    {
        if(s_stControlTaskInfo[TaskNum].ControlStartFun == NULL
            && s_stControlTaskInfo[TaskNum].ControlStopFun == NULL)
        {
            s_stControlTaskInfo[TaskNum].ControlStartFun = pstControlTaskInfo->ControlStartFun;
            s_stControlTaskInfo[TaskNum].ControlStopFun = pstControlTaskInfo->ControlStopFun;
            s_enControlMode[TaskNum] = Control_InvalidMode;
            s_stControlFlashInfo[TaskNum].RepeatTimes = 0;
            s_stControlFlashInfo[TaskNum].StartTimes = 0;
            s_stControlFlashInfo[TaskNum].StopTimes = 0;
            s_stControlFlashInfo[TaskNum].IntermittentTimes = 0;
            s_bUpdateControlFlashFlage[TaskNum] = false;
            s_stUpdateControlFlashInfo[TaskNum].RepeatTimes = 0;
            s_stUpdateControlFlashInfo[TaskNum].StartTimes = 0;
            s_stUpdateControlFlashInfo[TaskNum].StopTimes = 0;
            s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes = 0;
            return TaskNum;
        }
    }
    return -1;
}

int Control_DestroyTaskInfo(int TaskNum, ControTask_Info * pstControlTaskInfo)
{
    if(TaskNum < 0 || TaskNum >= ControlTask_MAXNUM
        || pstControlTaskInfo == NULL)
        return -1;

    if(s_stControlTaskInfo[TaskNum].ControlStartFun == pstControlTaskInfo->ControlStartFun
        && s_stControlTaskInfo[TaskNum].ControlStartFun == pstControlTaskInfo->ControlStartFun)
    {
        s_stControlTaskInfo[TaskNum].ControlStartFun = NULL;
        s_stControlTaskInfo[TaskNum].ControlStopFun = NULL;
        s_enControlMode[TaskNum] = Control_InvalidMode;
        s_stControlFlashInfo[TaskNum].RepeatTimes = 0;
        s_stControlFlashInfo[TaskNum].StartTimes = 0;
        s_stControlFlashInfo[TaskNum].StopTimes = 0;
        s_stControlFlashInfo[TaskNum].IntermittentTimes = 0;
        s_bUpdateControlFlashFlage[TaskNum] = false;
        s_stUpdateControlFlashInfo[TaskNum].RepeatTimes = 0;
        s_stUpdateControlFlashInfo[TaskNum].StartTimes = 0;
        s_stUpdateControlFlashInfo[TaskNum].StopTimes = 0;
        s_stUpdateControlFlashInfo[TaskNum].IntermittentTimes = 0;
        return 0;
    }
    return -1;
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

    ControTask_Info stControlTaskInfo = {0};
    stControlTaskInfo.ControlStartFun = Start;
    stControlTaskInfo.ControlStopFun = Stop;
    int TaskNum = Control_CreatTaskInfo(&stControlTaskInfo);

    ControlFlash_Info stControlFlashInfo = {0};
    stControlFlashInfo.RepeatTimes = 3;
    stControlFlashInfo.StartTimes = 3;
    stControlFlashInfo.StopTimes = 3;
    stControlFlashInfo.IntermittentTimes = 3;
    Control_SetMode(TaskNum, Control_LimitedFlashMode, stControlFlashInfo);

    while(1)
    {
        Control_Mode enControlMode = Control_InvalidMode;
        ControlFlash_Info stControlFlashInfo = {0};
        Control_GetMode(TaskNum, &enControlMode, &stControlFlashInfo);
        if(enControlMode == Control_StopMode)
        {
            break;
        }
        Control_Run();
        sleep(1);
    }

    Control_DestroyTaskInfo(TaskNum, &stControlTaskInfo);

    Control_DeInit();

    return 0;
}

#endif

#ifdef __cplusplus
}
#endif

