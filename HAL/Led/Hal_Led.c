/*
    From : https://github.com/paiminlin/PM
    From : https://blog.csdn.net/lpaim/article/details/125649860
    Author : PaiMin.lin
    Date : 2022.8.24
*/

#include "Hal_Led.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Hal_Led_Info
{
    bool bLedInit;
    bool bLedTaskCreat[HAL_LEDTASK_MAXNUM];
    Hal_Led_Mode enLedTaskMode[HAL_LEDTASK_MAXNUM];
    Hal_LedFlash_Info stLedTaskFlashInfo[HAL_LEDTASK_MAXNUM];
} Hal_Led_Info;

static bool s_bUpdateLedTaskFlashFlage[HAL_LEDTASK_MAXNUM] = {0};
static Hal_LedFlash_Info s_stUpdateLedTaskFlashInfo[HAL_LEDTASK_MAXNUM] = {0};

static Hal_Led_Info s_stHalLedInfo = {0};
static Hal_LedTask_Info s_stHalLedTaskInfo[HAL_LEDTASK_MAXNUM] = {0};

int Hal_Led_Run(void)
{
    if(s_stHalLedInfo.bLedInit == false)
        return 0;

    int TaskNum = 0;

    for(TaskNum = 0; TaskNum < HAL_LEDTASK_MAXNUM; TaskNum ++)
    {
        if(s_stHalLedInfo.enLedTaskMode[TaskNum] == HAL_LED_BRIGHT_MODE)
        {
            if(s_stHalLedTaskInfo[TaskNum].HalLedControlFun != NULL)
                s_stHalLedTaskInfo[TaskNum].HalLedControlFun(HAL_LED_BRIGHT_STATUS);
        }
        else if(s_stHalLedInfo.enLedTaskMode[TaskNum] == HAL_LED_DARK_MODE)
        {
            if(s_stHalLedTaskInfo[TaskNum].HalLedControlFun != NULL)
                s_stHalLedTaskInfo[TaskNum].HalLedControlFun(HAL_LED_DARK_STATUS);
        }
        else if(s_stHalLedInfo.enLedTaskMode[TaskNum] == HAL_LED_LIMITEDFLASH_MODE)
        {
            if(s_bUpdateLedTaskFlashFlage[TaskNum] == true)
            {
                s_bUpdateLedTaskFlashFlage[TaskNum] = false;
                s_stUpdateLedTaskFlashInfo[TaskNum].RepeatTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].RepeatTimes;
                s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].BrightTimes;
                s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].DarkTimes;
            }

            if(s_stUpdateLedTaskFlashInfo[TaskNum].RepeatTimes > 0)
            {
                if(s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes > 0)
                {
                    if(s_stHalLedTaskInfo[TaskNum].HalLedControlFun != NULL)
                        s_stHalLedTaskInfo[TaskNum].HalLedControlFun(HAL_LED_BRIGHT_STATUS);
                    s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes --;
                }
                else if(s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes > 0)
                {
                    if(s_stHalLedTaskInfo[TaskNum].HalLedControlFun != NULL)
                        s_stHalLedTaskInfo[TaskNum].HalLedControlFun(HAL_LED_DARK_STATUS);
                    s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes --;
                }

                if(s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes == 0
                    && s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes == 0)
                {
                    s_stUpdateLedTaskFlashInfo[TaskNum].RepeatTimes --;
                    s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].BrightTimes;
                    s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].DarkTimes;
                }
            }
            else
            {
                s_stHalLedInfo.enLedTaskMode[TaskNum] = HAL_LED_DARK_MODE;
            }
        }
        else if(s_stHalLedInfo.enLedTaskMode[TaskNum] == HAL_LED_UNLIMITEDFLASH_MODE)
        {
            if(s_bUpdateLedTaskFlashFlage[TaskNum] == true)
            {
                s_bUpdateLedTaskFlashFlage[TaskNum] = false;
                s_stUpdateLedTaskFlashInfo[TaskNum].RepeatTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].RepeatTimes;
                s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].BrightTimes;
                s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].DarkTimes;
            }

            if(s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes > 0)
            {
                if(s_stHalLedTaskInfo[TaskNum].HalLedControlFun != NULL)
                    s_stHalLedTaskInfo[TaskNum].HalLedControlFun(HAL_LED_BRIGHT_STATUS);
                s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes --;
            }
            else if(s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes > 0)
            {
                if(s_stHalLedTaskInfo[TaskNum].HalLedControlFun != NULL)
                    s_stHalLedTaskInfo[TaskNum].HalLedControlFun(HAL_LED_DARK_STATUS);
                s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes --;
            }

            if(s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes == 0
                && s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes == 0)
            {
                s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].BrightTimes;
                s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].DarkTimes;
            }
        }
    }
    return 0;
}

int Hal_Led_Init(void)
{
    if(s_stHalLedInfo.bLedInit == true)
        return 0;

    s_stHalLedInfo.bLedInit = true;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum < HAL_LEDTASK_MAXNUM; TaskNum ++)
    {
        s_bUpdateLedTaskFlashFlage[TaskNum] = false;
        s_stUpdateLedTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes = 0;
        s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes = 0;
        s_stHalLedInfo.bLedTaskCreat[TaskNum] = false;
        s_stHalLedInfo.enLedTaskMode[TaskNum] = HAL_LED_INVALID_MODE;
        s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].BrightTimes = 0;
        s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].DarkTimes = 0;
        s_stHalLedTaskInfo[TaskNum].enInitHalLedStatus = HAL_LED_INVALID_STATUS;
        s_stHalLedTaskInfo[TaskNum].HalLedControlFun = NULL;
    }
    return 0;
}

int Hal_Led_DeInit(void)
{
    if(s_stHalLedInfo.bLedInit == false)
        return 0;

    s_stHalLedInfo.bLedInit = false;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum < HAL_LEDTASK_MAXNUM; TaskNum ++)
    {
        s_bUpdateLedTaskFlashFlage[TaskNum] = false;
        s_stUpdateLedTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes = 0;
        s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes = 0;
        s_stHalLedInfo.bLedTaskCreat[TaskNum] = false;
        s_stHalLedInfo.enLedTaskMode[TaskNum] = HAL_LED_INVALID_MODE;
        s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].BrightTimes = 0;
        s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].DarkTimes = 0;
        s_stHalLedTaskInfo[TaskNum].enInitHalLedStatus = HAL_LED_INVALID_STATUS;
        s_stHalLedTaskInfo[TaskNum].HalLedControlFun = NULL;
    }
    return 0;
}

int Hal_Led_CreatTask(Hal_LedTask_Info * pstHalLedTaskInfo)
{
    if(s_stHalLedInfo.bLedInit == false)
        return -1;

    if(pstHalLedTaskInfo == NULL)
        return -1;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum <HAL_LEDTASK_MAXNUM; TaskNum ++)
    {
        if(s_stHalLedTaskInfo[TaskNum].enInitHalLedStatus == HAL_LED_INVALID_STATUS
            && s_stHalLedTaskInfo[TaskNum].HalLedControlFun == NULL)
        {
            s_bUpdateLedTaskFlashFlage[TaskNum] = false;
            s_stUpdateLedTaskFlashInfo[TaskNum].RepeatTimes = 0;
            s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes = 0;
            s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes = 0;
            s_stHalLedInfo.bLedTaskCreat[TaskNum] = true;
            s_stHalLedInfo.enLedTaskMode[TaskNum] = HAL_LED_INVALID_MODE;
            s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].RepeatTimes = 0;
            s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].BrightTimes = 0;
            s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].DarkTimes = 0;
            s_stHalLedTaskInfo[TaskNum].enInitHalLedStatus = pstHalLedTaskInfo->enInitHalLedStatus;
            s_stHalLedTaskInfo[TaskNum].HalLedControlFun = pstHalLedTaskInfo->HalLedControlFun;

            if(pstHalLedTaskInfo->HalLedControlFun != NULL)
                pstHalLedTaskInfo->HalLedControlFun(pstHalLedTaskInfo->enInitHalLedStatus);

            return TaskNum;
        }
    }
    return -1;
}

int Hal_Led_DestroyTask(int TaskNum, Hal_LedTask_Info * pstHalLedTaskInfo)
{
    if(s_stHalLedInfo.bLedInit == false)
        return -1;

    if(TaskNum < 0 || TaskNum >= HAL_LEDTASK_MAXNUM
        || pstHalLedTaskInfo == NULL)
        return -1;

    if(s_stHalLedTaskInfo[TaskNum].enInitHalLedStatus == pstHalLedTaskInfo->enInitHalLedStatus
        && s_stHalLedTaskInfo[TaskNum].HalLedControlFun == pstHalLedTaskInfo->HalLedControlFun)
    {
        s_bUpdateLedTaskFlashFlage[TaskNum] = false;
        s_stUpdateLedTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stUpdateLedTaskFlashInfo[TaskNum].BrightTimes = 0;
        s_stUpdateLedTaskFlashInfo[TaskNum].DarkTimes = 0;
        s_stHalLedInfo.bLedTaskCreat[TaskNum] = false;
        s_stHalLedInfo.enLedTaskMode[TaskNum] = HAL_LED_INVALID_MODE;
        s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].RepeatTimes = 0;
        s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].BrightTimes = 0;
        s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].DarkTimes = 0;
        s_stHalLedTaskInfo[TaskNum].enInitHalLedStatus = HAL_LED_INVALID_STATUS;
        s_stHalLedTaskInfo[TaskNum].HalLedControlFun = NULL;
        return 0;
    }
    return -1;
}

int Hal_Led_SetMode(int TaskNum, Hal_Led_Mode enLedTaskMode, 
                                Hal_LedFlash_Info stLedTaskFlashInfo)
{
    if(TaskNum < 0 || TaskNum >= HAL_LEDTASK_MAXNUM)
        return -1;

    if(s_stHalLedInfo.bLedTaskCreat[TaskNum] == false)
        return -1;

    s_bUpdateLedTaskFlashFlage[TaskNum] = true;
    s_stHalLedInfo.enLedTaskMode[TaskNum] = enLedTaskMode;
    s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].RepeatTimes = stLedTaskFlashInfo.RepeatTimes;
    s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].BrightTimes = stLedTaskFlashInfo.BrightTimes;
    s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].DarkTimes = stLedTaskFlashInfo.DarkTimes;

    return 0;
}

int Hal_Led_GetMode(int TaskNum, Hal_Led_Mode *penLedTaskMode, 
                                Hal_LedFlash_Info *pstLedTaskFlashInfo)
{
    if(TaskNum < 0 || TaskNum >= HAL_LEDTASK_MAXNUM
        || penLedTaskMode == NULL || pstLedTaskFlashInfo == NULL)
        return -1;

    *penLedTaskMode = s_stHalLedInfo.enLedTaskMode[TaskNum];
    if(*penLedTaskMode == HAL_LED_LIMITEDFLASH_MODE
        || *penLedTaskMode == HAL_LED_UNLIMITEDFLASH_MODE)
    {
        pstLedTaskFlashInfo->RepeatTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].RepeatTimes;
        pstLedTaskFlashInfo->BrightTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].BrightTimes;
        pstLedTaskFlashInfo->DarkTimes = s_stHalLedInfo.stLedTaskFlashInfo[TaskNum].DarkTimes;
    }
    else
    {
        pstLedTaskFlashInfo->RepeatTimes = 0;
        pstLedTaskFlashInfo->BrightTimes = 0;
        pstLedTaskFlashInfo->DarkTimes = 0;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif

