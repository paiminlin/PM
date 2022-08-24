/*
    From : https://github.com/paiminlin/PM
    From : https://blog.csdn.net/lpaim/article/details/126498161
    Author : PaiMin.lin
    Date : 2022.8.23
*/

#include "Hal_Key.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Hal_Key_Info
{
    bool bKeyInit;
    bool bKeyTaskCreat[HAL_KEYTASK_MAXNUM];
    int KeyPressDownTimes[HAL_KEYTASK_MAXNUM];
} Hal_Key_Info;

static Hal_Key_Info s_stHalKeyInfo = {0};
static Hal_KeyTask_Info s_stHalKeyTaskInfo[HAL_KEYTASK_MAXNUM] = {0};

int Hal_Key_Run(void)
{
    if(s_stHalKeyInfo.bKeyInit == false)
        return 0;

    int TaskNum = 0;
    Hal_Key_Status enKeyStatus = HAL_KEY_INVALID_STATUS;
    for(TaskNum = 0; TaskNum < HAL_KEYTASK_MAXNUM; TaskNum ++)
    {
        if(s_stHalKeyInfo.bKeyTaskCreat[TaskNum] == true)
        {
            if(s_stHalKeyTaskInfo[TaskNum].HalKeyGetStatusFun != NULL)
            {
                enKeyStatus = s_stHalKeyTaskInfo[TaskNum].HalKeyGetStatusFun();
                if(enKeyStatus == HAL_KEY_PRESS_STATUS)
                {
                    s_stHalKeyInfo.KeyPressDownTimes[TaskNum] ++;
                }

                if(s_stHalKeyInfo.KeyPressDownTimes[TaskNum] == s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.ShortPressTimes)
                {
                    if(s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun != NULL)
                    {
                        s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun(HAL_KEY_SHORTPRESS_STATUS);
                    }
                }
                else if(s_stHalKeyInfo.KeyPressDownTimes[TaskNum] >= s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.LongPressTimes
                    && s_stHalKeyInfo.KeyPressDownTimes[TaskNum]%s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.LongPressTimes == 0)
                {
                    if(s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun != NULL)
                    {
                        s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun(HAL_KEY_LONGPRESS_STATUS);
                    }
                }
 
                if(enKeyStatus == HAL_KEY_RELEASE_STATUS)
                {
                    if(s_stHalKeyInfo.KeyPressDownTimes[TaskNum] >= s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.ShortPressTimes)
                    {
                        if(s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun != NULL)
                        {
                            s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun(HAL_KEY_RELEASEPRESS_STATUS);
                        }
                    }
                    s_stHalKeyInfo.KeyPressDownTimes[TaskNum] = 0;
                }
            }
        }
    }
    return 0;
}

int Hal_Key_Init(void)
{
    if(s_stHalKeyInfo.bKeyInit == true)
        return 0;

    s_stHalKeyInfo.bKeyInit = true;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum < HAL_KEYTASK_MAXNUM; TaskNum ++)
    {
        s_stHalKeyInfo.bKeyTaskCreat[TaskNum] = false;
        s_stHalKeyInfo.KeyPressDownTimes[TaskNum] = 0;

        s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.ShortPressTimes = 0;
        s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.LongPressTimes = 0;
        s_stHalKeyTaskInfo[TaskNum].HalKeyGetStatusFun = NULL;
        s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun = NULL;
    }
    return 0;
}

int Hal_Key_DeInit(void)
{
    if(s_stHalKeyInfo.bKeyInit == false)
        return 0;

    s_stHalKeyInfo.bKeyInit = false;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum < HAL_KEYTASK_MAXNUM; TaskNum ++)
    {
        s_stHalKeyInfo.bKeyTaskCreat[TaskNum] = false;
        s_stHalKeyInfo.KeyPressDownTimes[TaskNum] = 0;

        s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.ShortPressTimes = 0;
        s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.LongPressTimes = 0;
        s_stHalKeyTaskInfo[TaskNum].HalKeyGetStatusFun = NULL;
        s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun = NULL;
    }
    return 0;
}

int Hal_Key_CreatTask(Hal_KeyTask_Info * pstHalKeyTaskInfo)
{
    if(s_stHalKeyInfo.bKeyInit == false)
        return -1;

    if(pstHalKeyTaskInfo == NULL)
        return -1;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum < HAL_KEYTASK_MAXNUM; TaskNum ++)
    {
        if(s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun == NULL
            && s_stHalKeyTaskInfo[TaskNum].HalKeyGetStatusFun == NULL)
        {
            s_stHalKeyInfo.bKeyTaskCreat[TaskNum] = true;
            s_stHalKeyInfo.KeyPressDownTimes[TaskNum] = 0;

            s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.ShortPressTimes = pstHalKeyTaskInfo->stHalKeyAttribute.ShortPressTimes;
            s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.LongPressTimes = pstHalKeyTaskInfo->stHalKeyAttribute.LongPressTimes;
            s_stHalKeyTaskInfo[TaskNum].HalKeyGetStatusFun = pstHalKeyTaskInfo->HalKeyGetStatusFun;
            s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun = pstHalKeyTaskInfo->HalKeyHandleFun;
            return TaskNum;
        }
    }
    return -1;
}

int Hal_Key_DestroyTask(int TaskNum, Hal_KeyTask_Info * pstHalKeyTaskInfo)
{
    if(s_stHalKeyInfo.bKeyInit == false)
        return -1;

    if(TaskNum < 0 || TaskNum >= HAL_KEYTASK_MAXNUM 
        || pstHalKeyTaskInfo == NULL)
        return -1;

    if(s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.ShortPressTimes == pstHalKeyTaskInfo->stHalKeyAttribute.ShortPressTimes
        && s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.LongPressTimes == pstHalKeyTaskInfo->stHalKeyAttribute.LongPressTimes
        && s_stHalKeyTaskInfo[TaskNum].HalKeyGetStatusFun == pstHalKeyTaskInfo->HalKeyGetStatusFun
        && s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun == pstHalKeyTaskInfo->HalKeyHandleFun)
    {
        s_stHalKeyInfo.bKeyTaskCreat[TaskNum] = false;
        s_stHalKeyInfo.KeyPressDownTimes[TaskNum] = 0;

        s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.ShortPressTimes = 0;
        s_stHalKeyTaskInfo[TaskNum].stHalKeyAttribute.LongPressTimes = 0;
        s_stHalKeyTaskInfo[TaskNum].HalKeyGetStatusFun = NULL;
        s_stHalKeyTaskInfo[TaskNum].HalKeyHandleFun = NULL;
        return 0;
    }
    return -1;
}

#ifdef __cplusplus
}
#endif

