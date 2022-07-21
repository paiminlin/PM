/*
    From : https://github.com/paiminlin/PM
    From : 
    Author : PaiMin.lin
    Date : 2022.7.21
*/

#include "Key.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Key_Info
{
    bool bKeyInit;
    bool bKeyTaskCreat[KeyTask_MAXNUM];
    int KeyPressDownTimes[KeyTask_MAXNUM];
} Key_Info;

static Key_Info s_stKeyInfo = {0};
static KeyTask_Info s_stKeyTaskInfo[KeyTask_MAXNUM] = {0};

int Key_Run(void)
{
    if(s_stKeyInfo.bKeyInit == false)
        return 0;

    int TaskNum = 0;
    Key_Status enKeyStatus = Key_Invalid_Status;
    for(TaskNum = 0; TaskNum < KeyTask_MAXNUM; TaskNum ++)
    {
        if(s_stKeyInfo.bKeyTaskCreat[TaskNum] == true)
        {
            if(s_stKeyTaskInfo[TaskNum].KeyGetStatusFun != NULL)
            {
                if(s_stKeyInfo.KeyPressDownTimes[TaskNum] == s_stKeyTaskInfo[TaskNum].stKeyAttribute.ShortPressTimes)
                {
                    if(s_stKeyTaskInfo[TaskNum].KeyHandleFun != NULL)
                    {
                        s_stKeyTaskInfo[TaskNum].KeyHandleFun(Key_Short_PressStatus);
                    }
                }
                else if(s_stKeyInfo.KeyPressDownTimes[TaskNum]%s_stKeyTaskInfo[TaskNum].stKeyAttribute.LongPressTimes == 0)
                {
                    if(s_stKeyTaskInfo[TaskNum].KeyHandleFun != NULL)
                    {
                        s_stKeyTaskInfo[TaskNum].KeyHandleFun(Key_Long_PressStatus);
                    }
                }

                enKeyStatus = s_stKeyTaskInfo[TaskNum].KeyGetStatusFun();
                if(enKeyStatus == Key_PressDown_Status
                    && s_stKeyInfo.KeyPressDownTimes[TaskNum] != KeyPressDown_MAXTIMES)
                {
                    s_stKeyInfo.KeyPressDownTimes[TaskNum] ++;
                }
                else if(enKeyStatus == Key_Release_Status)
                {
                    s_stKeyInfo.KeyPressDownTimes[TaskNum] = 0;
                }
            }
        }
    }
    return 0;
}

int Key_Init(void)
{
    if(s_stKeyInfo.bKeyInit == true)
        return 0;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum < KeyTask_MAXNUM; TaskNum ++)
    {
        s_stKeyInfo.bKeyTaskCreat[TaskNum] = false;
        s_stKeyInfo.KeyPressDownTimes[TaskNum] = 0;

        s_stKeyTaskInfo[TaskNum].stKeyAttribute.ShortPressTimes = 0;
        s_stKeyTaskInfo[TaskNum].stKeyAttribute.LongPressTimes = 0;
        s_stKeyTaskInfo[TaskNum].KeyGetStatusFun = NULL;
        s_stKeyTaskInfo[TaskNum].KeyHandleFun = NULL;
    }
    return 0;
}

int Key_DeInit(void)
{
    if(s_stKeyInfo.bKeyInit == false)
        return 0;

    int TaskNum = 0;
    for(TaskNum = 0; TaskNum < KeyTask_MAXNUM; TaskNum ++)
    {
        s_stKeyInfo.bKeyTaskCreat[TaskNum] = false;
        s_stKeyInfo.KeyPressDownTimes[TaskNum] = 0;

        s_stKeyTaskInfo[TaskNum].stKeyAttribute.ShortPressTimes = 0;
        s_stKeyTaskInfo[TaskNum].stKeyAttribute.LongPressTimes = 0;
        s_stKeyTaskInfo[TaskNum].KeyGetStatusFun = NULL;
        s_stKeyTaskInfo[TaskNum].KeyHandleFun = NULL;
    }
    return 0;
}

int Key_CreatTask(KeyTask_Info * pstKeyTaskInfo)
{
    if(s_stKeyInfo.bKeyInit == false)
        return -1;

    int TaskNum = 0;

    if(pstKeyTaskInfo == NULL)
        return -1;

    for(TaskNum = 0; TaskNum < KeyTask_MAXNUM; TaskNum ++)
    {
        if(s_stKeyTaskInfo[TaskNum].KeyHandleFun == NULL
            && s_stKeyTaskInfo[TaskNum].KeyGetStatusFun == NULL)
        {
            s_stKeyInfo.bKeyTaskCreat[TaskNum] = true;
            s_stKeyInfo.KeyPressDownTimes[TaskNum] = 0;

            s_stKeyTaskInfo[TaskNum].stKeyAttribute.ShortPressTimes = pstKeyTaskInfo->stKeyAttribute.ShortPressTimes;
            s_stKeyTaskInfo[TaskNum].stKeyAttribute.LongPressTimes = pstKeyTaskInfo->stKeyAttribute.LongPressTimes;
            s_stKeyTaskInfo[TaskNum].KeyGetStatusFun = pstKeyTaskInfo->KeyGetStatusFun;
            s_stKeyTaskInfo[TaskNum].KeyHandleFun = pstKeyTaskInfo->KeyHandleFun;
            return TaskNum;
        }
    }
    return -1;
}

int Key_DestroyTask(int TaskNum, KeyTask_Info * pstKeyTaskInfo)
{
    if(s_stKeyInfo.bKeyInit == false)
        return -1;

    if(TaskNum < 0 || TaskNum >= KeyTask_MAXNUM 
        || pstKeyTaskInfo == NULL)
        return -1;

    if(s_stKeyTaskInfo[TaskNum].stKeyAttribute.ShortPressTimes == pstKeyTaskInfo->stKeyAttribute.ShortPressTimes
        && s_stKeyTaskInfo[TaskNum].stKeyAttribute.LongPressTimes == pstKeyTaskInfo->stKeyAttribute.LongPressTimes
        && s_stKeyTaskInfo[TaskNum].KeyGetStatusFun == pstKeyTaskInfo->KeyGetStatusFun
        && s_stKeyTaskInfo[TaskNum].KeyHandleFun == pstKeyTaskInfo->KeyHandleFun)
    {
        s_stKeyInfo.bKeyTaskCreat[TaskNum] = false;
        s_stKeyInfo.KeyPressDownTimes[TaskNum] = 0;

        s_stKeyTaskInfo[TaskNum].stKeyAttribute.ShortPressTimes = 0;
        s_stKeyTaskInfo[TaskNum].stKeyAttribute.LongPressTimes = 0;
        s_stKeyTaskInfo[TaskNum].KeyGetStatusFun = NULL;
        s_stKeyTaskInfo[TaskNum].KeyHandleFun = NULL;
        return 0;
    }
    return -1;
}

//#define Key_MAIN_DEBUG
#ifdef Key_MAIN_DEBUG

int main()
{
    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

