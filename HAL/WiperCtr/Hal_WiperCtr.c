/*
    From : https://github.com/paiminlin/PM
    From : 
    Author : PaiMin.lin
    Date : 2023.3.12
*/

#include "Hal_WiperCtr.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Hal_WiperCtr_Info
{
    bool bWiperCtrInit;
    bool bWiperCtrTaskCreat[HAL_WIPERCTRTASK_MAXNUM];
    Hal_WiperCtr_Mode enWiperCtrTaskMode[HAL_WIPERCTRTASK_MAXNUM];
} Hal_WiperCtr_Info;

static Hal_WiperCtr_Info s_stHalWiperCtrInfo = {0};
static Hal_WiperCtrTask_Info s_stHalWiperCtrTaskInfo[HAL_WIPERCTRTASK_MAXNUM] = {0};

static Hal_WiperCtrTask_Info s_stUpdateHalWiperCtrTaskInfo[HAL_WIPERCTRTASK_MAXNUM] = {0};

static void WiperCtr_ClearUpdateInfo(int TaskNum, Hal_WiperCtr_Mode enHalWiperCtrMode)
{
    if(enHalWiperCtrMode != HAL_WIPERCTR_OFF_MODE)
    {
        s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes = 0;
    }
    if(enHalWiperCtrMode != HAL_WIPERCTR_LOW_MODE)
    {
        ;
    }
    if(enHalWiperCtrMode != HAL_WIPERCTR_HIGH_MODE)
    {
        ;
    }
    if(enHalWiperCtrMode != HAL_WIPERCTR_AUTO_MODE)
    {
//        s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrAutoInfo = ;
    }
    if(enHalWiperCtrMode != HAL_WIPERCTR_WASH_MODE)
    {
        s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes = 0;
        s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes = 0;
        s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW2RepeatTimes = 0;
    }
    if(enHalWiperCtrMode != HAL_WIPERCTR_REPAIR_MODE)
    {
        s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrRepairInfo.LOWKeepTimes = 0;
    }

    if(enHalWiperCtrMode == HAL_WIPERCTR_INVALID_MODE)
    {
        s_stUpdateHalWiperCtrTaskInfo[TaskNum].HalWiperCtrResetFun = NULL;
        s_stUpdateHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun = NULL;
    }

    return;
}

int Hal_WiperCtr_Run(void)
{
    if(s_stHalWiperCtrInfo.bWiperCtrInit == false)
        return 0;

    int TaskNum = 0;

    for(TaskNum = 0; TaskNum < HAL_WIPERCTRTASK_MAXNUM; TaskNum ++)
    {
        if(s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] == HAL_WIPERCTR_OFF_MODE)
        {
            WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_OFF_MODE);

            if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrResetFun != NULL)
            {
                if(s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes < s_stHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes)
                {
                    if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrResetFun(false) == true)
                    {
                        s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes ++;
                    }
                    else
                    {
                        s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes = 0;

                        if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun != NULL)
                            s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun(HAL_WIPERCTR_LOW_MODE);
                    }
                }
                else if(s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes == s_stHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes)
                {
                    if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun != NULL)
                        s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun(HAL_WIPERCTR_OFF_MODE);

                    Hal_WiperCtr_SetMode(TaskNum, HAL_WIPERCTR_INVALID_MODE);
                }
            }
        }
        else if(s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] == HAL_WIPERCTR_LOW_MODE)
        {
            WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_LOW_MODE);

            if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun != NULL)
                s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun(HAL_WIPERCTR_LOW_MODE);
        }
        else if(s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] == HAL_WIPERCTR_HIGH_MODE)
        {
            WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_LOW_MODE);

            if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun != NULL)
                s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun(HAL_WIPERCTR_HIGH_MODE);
        }
        else if(s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] == HAL_WIPERCTR_AUTO_MODE)
        {
            WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_AUTO_MODE);
        }
        else if(s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] == HAL_WIPERCTR_WASH_MODE)
        {
            WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_WASH_MODE);

            if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrResetFun != NULL)
            {
                if(s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes < s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes)
                {
                    if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrResetFun(true) == true)
                    {
                        s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes ++;
                    }
                    else
                    {
                        if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun != NULL)
                            s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun(HAL_WIPERCTR_LOW_MODE);
                    }

                    if(s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes == s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes)
                    {
                        if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun != NULL)
                            s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun(HAL_WIPERCTR_OFF_MODE);
                    }
                }
                else if(s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes == s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes
                    && s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes < s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes)
                {
                    s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes ++;

                    if(s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes == s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes)
                    {
                        if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun != NULL)
                            s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun(HAL_WIPERCTR_LOW_MODE);
                    }
                }
                else if(s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes == s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes
                    && s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW2RepeatTimes < s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW2RepeatTimes)
                {
                    if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrResetFun(true) == true)
                    {
                        s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW2RepeatTimes ++;
                    }

                    if(s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW2RepeatTimes == s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW2RepeatTimes)
                    {
                        Hal_WiperCtr_SetMode(TaskNum, HAL_WIPERCTR_OFF_MODE);
                    }
                }
            }
        }
        else if(s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] == HAL_WIPERCTR_REPAIR_MODE)
        {
            WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_REPAIR_MODE);

            if(s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrRepairInfo.LOWKeepTimes < s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrRepairInfo.LOWKeepTimes)
            {
                s_stUpdateHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrRepairInfo.LOWKeepTimes ++;

                if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun != NULL)
                    s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun(HAL_WIPERCTR_LOW_MODE);
            }
            else
            {
                if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun != NULL)
                    s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun(HAL_WIPERCTR_OFF_MODE);

                Hal_WiperCtr_SetMode(TaskNum, HAL_WIPERCTR_INVALID_MODE);
            }
        }
        else if(s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] == HAL_WIPERCTR_INVALID_MODE)
        {
            WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_INVALID_MODE);
        }
    }
    return 0;
}

int Hal_WiperCtr_Init(void)
{
    if(s_stHalWiperCtrInfo.bWiperCtrInit == true)
        return 0;

    s_stHalWiperCtrInfo.bWiperCtrInit = true;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum < HAL_WIPERCTRTASK_MAXNUM; TaskNum ++)
    {
        s_stHalWiperCtrInfo.bWiperCtrTaskCreat[TaskNum] = false;
        s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] = HAL_WIPERCTR_INVALID_MODE;
        s_stHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes = 0;
//        s_stHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrAutoInfo = ;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW2RepeatTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrRepairInfo.LOWKeepTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrResetFun = NULL;
        s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun = NULL;

        WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_INVALID_MODE);
    }
    return 0;
}

int Hal_WiperCtr_DeInit(void)
{
    if(s_stHalWiperCtrInfo.bWiperCtrInit == false)
        return 0;

    s_stHalWiperCtrInfo.bWiperCtrInit = false;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum < HAL_WIPERCTRTASK_MAXNUM; TaskNum ++)
    {
        s_stHalWiperCtrInfo.bWiperCtrTaskCreat[TaskNum] = false;
        s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] = HAL_WIPERCTR_INVALID_MODE;
        s_stHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes = 0;
//        s_stHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrAutoInfo = ;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW2RepeatTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrRepairInfo.LOWKeepTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrResetFun = NULL;
        s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun = NULL;

        WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_INVALID_MODE);
    }
    return 0;
}

int Hal_WiperCtr_CreatTask(Hal_WiperCtrTask_Info * pstHalWiperCtrTaskInfo)
{
    if(s_stHalWiperCtrInfo.bWiperCtrInit == false)
        return -1;

    if(pstHalWiperCtrTaskInfo == NULL)
        return -1;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum <HAL_WIPERCTRTASK_MAXNUM; TaskNum ++)
    {
        if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun == NULL)
        {
            s_stHalWiperCtrInfo.bWiperCtrTaskCreat[TaskNum] = true;
            s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] = HAL_WIPERCTR_INVALID_MODE;
            s_stHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes = pstHalWiperCtrTaskInfo->stHalWiperCtrOffInfo.KeepResetTimes;
//            s_stHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrAutoInfo = ;
            s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes = pstHalWiperCtrTaskInfo->stHal_WiperCtrWashInfo.LOW1RepeatTimes;
            s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes = pstHalWiperCtrTaskInfo->stHal_WiperCtrWashInfo.OFFTimes;
            s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW2RepeatTimes = pstHalWiperCtrTaskInfo->stHal_WiperCtrWashInfo.LOW2RepeatTimes;
            s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrRepairInfo.LOWKeepTimes = pstHalWiperCtrTaskInfo->stHal_WiperCtrRepairInfo.LOWKeepTimes;
            s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrResetFun = pstHalWiperCtrTaskInfo->HalWiperCtrResetFun;
            s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun = pstHalWiperCtrTaskInfo->HalWiperCtrControlFun;

            WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_INVALID_MODE);

            if(pstHalWiperCtrTaskInfo->HalWiperCtrControlFun != NULL)
                pstHalWiperCtrTaskInfo->HalWiperCtrControlFun(HAL_WIPERCTR_INVALID_MODE);

            return TaskNum;
        }
    }
    return -1;
}

int Hal_WiperCtr_DestroyTask(int TaskNum, Hal_WiperCtrTask_Info * pstHalWiperCtrTaskInfo)
{
    if(s_stHalWiperCtrInfo.bWiperCtrInit == false)
        return -1;

    if(TaskNum < 0 || TaskNum >= HAL_WIPERCTRTASK_MAXNUM
        || pstHalWiperCtrTaskInfo == NULL)
        return -1;

    if(s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun == pstHalWiperCtrTaskInfo->HalWiperCtrControlFun)
    {
        s_stHalWiperCtrInfo.bWiperCtrTaskCreat[TaskNum] = false;
        s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] = HAL_WIPERCTR_INVALID_MODE;
        s_stHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrOffInfo.KeepResetTimes = 0;
//        s_stHalWiperCtrTaskInfo[TaskNum].stHalWiperCtrAutoInfo = ;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW1RepeatTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.OFFTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrWashInfo.LOW2RepeatTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].stHal_WiperCtrRepairInfo.LOWKeepTimes = 0;
        s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrResetFun = NULL;
        s_stHalWiperCtrTaskInfo[TaskNum].HalWiperCtrControlFun = NULL;

        WiperCtr_ClearUpdateInfo(TaskNum, HAL_WIPERCTR_INVALID_MODE);

        return 0;
    }
    return -1;
}

int Hal_WiperCtr_SetMode(int TaskNum, Hal_WiperCtr_Mode enWiperCtrTaskMode)
{
    if(TaskNum < 0 || TaskNum >= HAL_WIPERCTRTASK_MAXNUM)
        return -1;

    if(s_stHalWiperCtrInfo.bWiperCtrTaskCreat[TaskNum] == false)
        return -1;

    s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum] = enWiperCtrTaskMode;

    return 0;
}

int Hal_WiperCtr_GetMode(int TaskNum, Hal_WiperCtr_Mode *penWiperCtrTaskMode)
{
    if(TaskNum < 0 || TaskNum >= HAL_WIPERCTRTASK_MAXNUM
        || penWiperCtrTaskMode == NULL)
        return -1;

    *penWiperCtrTaskMode = s_stHalWiperCtrInfo.enWiperCtrTaskMode[TaskNum];
    if(*penWiperCtrTaskMode == HAL_WIPERCTR_AUTO_MODE)
    {
    }
    else
    {
    }
    return 0;
}

//#define WIPERCTR_MAIN_DEBUG
#ifdef WIPERCTR_MAIN_DEBUG

#include <time.h>

int HalWiperCtrResetFun(bool bResetChange)
{
    static bool bLastReset = false;
    bool bReset = false;

    time_t seconds;
    seconds = time(NULL);
    struct tm * gmt;
    gmt = localtime(&seconds);

    Hal_WiperCtr_Mode enWiperCtrTaskMode;
    Hal_WiperCtr_GetMode(0, &enWiperCtrTaskMode);
    if(1)//if(enWiperCtrTaskMode == HAL_WIPERCTR_OFF_MODE)
    {
        if(gmt->tm_sec % 1 == 0)
        {
            bReset = true;
        }
        else
        {
            bReset = false;
        }

    }
    else
    {
        if(gmt->tm_sec % 2 == 0)
        {
            bReset = true;
        }
        else
        {
            bReset = false;
        }
    }

    if(bResetChange == false)
    {
//        return bReset;
    }
    else if(bResetChange == true)
    {
        if(bLastReset != bReset && bReset == true)
        {
//            return true;
            bReset = true;

        }
        else
        {
//            return false;
            bReset = false;
        }
    }

    bLastReset = bReset;

//    printf("%s-%d-%d\n",__func__, __LINE__, bReset);

    return bReset;
}

int HalWiperCtrControlFun(Hal_WiperCtr_Mode enHalWiperCtrMode)
{
    if(enHalWiperCtrMode == HAL_WIPERCTR_OFF_MODE)
        printf("%s-%d-HAL_WIPERCTR_OFF_MODE\n",__func__, __LINE__);
    else if(enHalWiperCtrMode == HAL_WIPERCTR_LOW_MODE)
        printf("%s-%d-HAL_WIPERCTR_LOW_MODE\n",__func__, __LINE__);
    else if(enHalWiperCtrMode == HAL_WIPERCTR_HIGH_MODE)
        printf("%s-%d-HAL_WIPERCTR_HIGH_MODE\n",__func__, __LINE__);
    else if(enHalWiperCtrMode == HAL_WIPERCTR_AUTO_MODE)
        printf("%s-%d-HAL_WIPERCTR_AUTO_MODE\n",__func__, __LINE__);
    else if(enHalWiperCtrMode == HAL_WIPERCTR_WASH_MODE)
        printf("%s-%d-HAL_WIPERCTR_WASH_MODE\n",__func__, __LINE__);
    else if(enHalWiperCtrMode == HAL_WIPERCTR_REPAIR_MODE)
        printf("%s-%d-HAL_WIPERCTR_REPAIR_MODE\n",__func__, __LINE__);
}

int main()
{
    Hal_WiperCtr_Init();

    Hal_WiperCtrTask_Info stHalWiperCtrTaskInfo = {0};
    stHalWiperCtrTaskInfo.stHalWiperCtrOffInfo.KeepResetTimes = 5;
//    stHalWiperCtrTaskInfo.stHalWiperCtrAutoInfo = ;
    stHalWiperCtrTaskInfo.stHal_WiperCtrWashInfo.LOW1RepeatTimes = 5;
    stHalWiperCtrTaskInfo.stHal_WiperCtrWashInfo.OFFTimes = 5;
    stHalWiperCtrTaskInfo.stHal_WiperCtrWashInfo.LOW2RepeatTimes = 5;
    stHalWiperCtrTaskInfo.stHal_WiperCtrRepairInfo.LOWKeepTimes = 10;
    stHalWiperCtrTaskInfo.HalWiperCtrResetFun = HalWiperCtrResetFun;
    stHalWiperCtrTaskInfo.HalWiperCtrControlFun = HalWiperCtrControlFun;

    int TaskNum = Hal_WiperCtr_CreatTask(&stHalWiperCtrTaskInfo);

    while(1)
    {
        Hal_WiperCtr_Mode enWiperCtrTaskMode;
        Hal_WiperCtr_GetMode(TaskNum, &enWiperCtrTaskMode);
        if(enWiperCtrTaskMode == HAL_WIPERCTR_OFF_MODE)
            printf("%s-%d-HAL_WIPERCTR_OFF_MODE\n",__func__, __LINE__);
        else if(enWiperCtrTaskMode == HAL_WIPERCTR_LOW_MODE)
            printf("%s-%d-HAL_WIPERCTR_LOW_MODE\n",__func__, __LINE__);
        else if(enWiperCtrTaskMode == HAL_WIPERCTR_HIGH_MODE)
            printf("%s-%d-HAL_WIPERCTR_HIGH_MODE\n",__func__, __LINE__);
        else if(enWiperCtrTaskMode == HAL_WIPERCTR_AUTO_MODE)
            printf("%s-%d-HAL_WIPERCTR_AUTO_MODE\n",__func__, __LINE__);
        else if(enWiperCtrTaskMode == HAL_WIPERCTR_WASH_MODE)
            printf("%s-%d-HAL_WIPERCTR_WASH_MODE\n",__func__, __LINE__);
        else if(enWiperCtrTaskMode == HAL_WIPERCTR_REPAIR_MODE)
            printf("%s-%d-HAL_WIPERCTR_REPAIR_MODE\n",__func__, __LINE__);
        else if(enWiperCtrTaskMode == HAL_WIPERCTR_INVALID_MODE)
            printf("%s-%d-HAL_WIPERCTR_INVALID_MODE\n",__func__, __LINE__);

        static bool bSet1 = false;
        static bool bSet2 = false;
        static bool bSet3 = false;

        if(bSet1 == false)
        {
            bSet1 = true;
            Hal_WiperCtr_SetMode(TaskNum, HAL_WIPERCTR_WASH_MODE);
//            Hal_WiperCtr_SetMode(TaskNum, HAL_WIPERCTR_REPAIR_MODE);
        }

        Hal_WiperCtr_Run();

        sleep(1);

        Hal_WiperCtr_GetMode(TaskNum, &enWiperCtrTaskMode);
        if(enWiperCtrTaskMode == HAL_WIPERCTR_INVALID_MODE)
        {
            break;
        }
    }

    Hal_WiperCtr_DestroyTask(TaskNum, &stHalWiperCtrTaskInfo);

    Hal_WiperCtr_DeInit();

    return 0;
}

#endif

#ifdef __cplusplus
}
#endif

