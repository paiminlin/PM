
#include "Hall_Motor.h"

#ifdef __cplusplus
extern "C"{
#endif

static bool s_bStudying[Hall_Motor_Invalid_Location] = {0};
static bool s_bUnderStudyByStall[Hall_Motor_Invalid_Location][Hall_Motor_Invalid_Turn] = {0};

static int s_HallMotor_TimeCount[Hall_Motor_Invalid_Location][Hall_Motor_Invalid_Turn] = {0};       /* 霍尔超时计数器 */
static int s_HallMotor_TurnCount[Hall_Motor_Invalid_Location] = {0};                                /* 霍尔脉冲计数器 */

static int s_LastHallMotor_TurnCount[Hall_Motor_Invalid_Location] = {0};

static Hall_Motor_Move s_LastenHallMotorMove[Hall_Motor_Invalid_Location] = {0};

static Hall_Motor_Info s_stHallMotorInfo[Hall_Motor_Invalid_Location] = {0};
static Hall_Motor_TaskInfo s_stHallMotorTaskInfo[Hall_Motor_Invalid_Location] = {0};

static void Motor_Study(Hall_Motor_Location enLocation)
{
    Hall_Motor_MoveInfo stHallMotorMoveInfo = {0};
    stHallMotorMoveInfo.enHallMotorMove = Hall_Motor_Invalid_Move;
    stHallMotorMoveInfo.Customlocation = 0;

    /*
        自学习，正转回到起始点，再反转到堵转点，计算总长度
    */
    if(s_stHallMotorInfo[enLocation].enHallMotorMove != Hall_Motor_ForwardStall_Move
        && s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] == false)
    {
        stHallMotorMoveInfo.enHallMotorMove = Hall_Motor_ForwardStall_Move;
        Hall_Motor_SetMove(enLocation, stHallMotorMoveInfo);
    }
    else if(s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] == true
        && s_stHallMotorInfo[enLocation].enHallMotorMove != Hall_Motor_ReverseStall_Move
        && s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] == false)
    {
        s_HallMotor_TurnCount[enLocation] = 0;
        stHallMotorMoveInfo.enHallMotorMove = Hall_Motor_ReverseStall_Move;
        Hall_Motor_SetMove(enLocation, stHallMotorMoveInfo);
    }
    else if(s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] == true
        && s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] == true)
    {
        /*
            自学习完成
            更新位置，记录长度
        */
        s_stHallMotorInfo[enLocation].bStudy = true;
        s_stHallMotorInfo[enLocation].position = s_HallMotor_TurnCount[enLocation];
        s_stHallMotorInfo[enLocation].Motorlength = s_HallMotor_TurnCount[enLocation];

        Hall_Motor_StopStudy(enLocation);
    }
}

static void Motor_APP(Hall_Motor_Location enLocation)
{
    Hall_Motor_MoveInfo stHallMotorMoveInfo = {0};
    stHallMotorMoveInfo.enHallMotorMove = Hall_Motor_Invalid_Move;
    stHallMotorMoveInfo.Customlocation = 0;

    if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardStall_Move
        || s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseStall_Move)
    {
        if(s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] == true
            || s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] == true)
        {
            Hall_Motor_SetMove(enLocation, stHallMotorMoveInfo);
        }
    }
    else if(s_stHallMotorInfo[enLocation].bStudy == false)
    {
        if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardSoftStop_Move
            || s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseSoftStop_Move
            || s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_Customlocation_Move)
        {
            Hall_Motor_SetMove(enLocation, stHallMotorMoveInfo);
        }
    }
    else if(s_stHallMotorInfo[enLocation].bStudy == true)
    {
        if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardSoftStop_Move
            && s_stHallMotorInfo[enLocation].position <= (MotorSoftStop_MaxCount + MotorStopDeadBand_Count))
        {
            s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Forward_Turn] = true;
            Hall_Motor_SetMove(enLocation, stHallMotorMoveInfo);
        }
        else if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseSoftStop_Move
            && s_stHallMotorInfo[enLocation].position >= (s_stHallMotorInfo[enLocation].Motorlength - MotorSoftStop_MaxCount - MotorStopDeadBand_Count))
        {
            s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Reverse_Turn] = true;
            Hall_Motor_SetMove(enLocation, stHallMotorMoveInfo);
        }
        else if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_Customlocation_Move
            && (abs(s_stHallMotorInfo[enLocation].position - s_stHallMotorInfo[enLocation].Customlocation) <= MotorStopDeadBand_Count))
        {
            Hall_Motor_SetMove(enLocation, stHallMotorMoveInfo);
        }
    }
}

void Hall_Motor_Run()
{
    Hall_Motor_Location enLocation = Hall_Motor_MainDriver_Location;
    for(enLocation = Hall_Motor_MainDriver_Location; enLocation < Hall_Motor_Invalid_Location; enLocation ++)
    {
        if(s_stHallMotorInfo[enLocation].bStudy == false)
        {
            /*
                电机处于运作状态，霍尔脉冲计数器一直没更新，超时计数
            */
            if(s_stHallMotorInfo[enLocation].enHallMotorMove != Hall_Motor_Invalid_Move
                && s_HallMotor_TurnCount[enLocation] == s_LastHallMotor_TurnCount[enLocation])
            {
                if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardStall_Move)
                {
                    if(s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] <= MotorTimeOut_MaxCount)
                        s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] ++;
                    /*
                        超时堵转
                    */
                    if(s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] >= MotorTimeOut_MaxCount)
                    {
                        s_bUnderStudyByStall[enLocation][Hall_Motor_Forward_Turn] = true;
                        s_HallMotor_TurnCount[enLocation] = 0;

                        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = true;
                    }
                }
                else if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseStall_Move)
                {
                    if(s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] <= MotorTimeOut_MaxCount)
                        s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] ++;
                    /*
                        超时堵转
                    */
                    if(s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] >= MotorTimeOut_MaxCount)
                    {
                        if(s_bUnderStudyByStall[enLocation][Hall_Motor_Forward_Turn] == true)
                            s_bUnderStudyByStall[enLocation][Hall_Motor_Reverse_Turn] = true;

                        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = true;
                    }
                }

                if(s_bStudying[enLocation] == false
                    && s_bUnderStudyByStall[enLocation][Hall_Motor_Forward_Turn] == true
                    && s_bUnderStudyByStall[enLocation][Hall_Motor_Reverse_Turn] == true)
                {
                    /*
                        手动学习完成
                        更新位置，记录长度
                    */
                    s_stHallMotorInfo[enLocation].bStudy = true;
                    s_stHallMotorInfo[enLocation].position = s_HallMotor_TurnCount[enLocation];
                    s_stHallMotorInfo[enLocation].Motorlength = s_HallMotor_TurnCount[enLocation];
                }
            }
        }
        else if(s_stHallMotorInfo[enLocation].bStudy == true)
        {
            /*
                电机处于运作状态，霍尔脉冲计数器一直没更新，超时计数
            */
            if(s_stHallMotorInfo[enLocation].enHallMotorMove != Hall_Motor_Invalid_Move
                && s_HallMotor_TurnCount[enLocation] == s_LastHallMotor_TurnCount[enLocation])
            {
                if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardStall_Move
                    || s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardSoftStop_Move
                    || (s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_Customlocation_Move
                        && s_stHallMotorInfo[enLocation].Customlocation <= s_stHallMotorInfo[enLocation].position))
                {
                    if(s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] <= MotorTimeOut_MaxCount)
                        s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] ++;
                    /*
                        超时堵转
                    */
                    if(s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] >= MotorTimeOut_MaxCount)
                    {
                        if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardStall_Move)
                        {
                            s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = true;
                        }
                        else if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardSoftStop_Move
                            && s_stHallMotorInfo[enLocation].position >= (MotorSoftStop_MaxCount + MotorStopDeadBand_Count))
                        {
                            s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = true;
                        }
                        else if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_Customlocation_Move
                            && (abs(s_stHallMotorInfo[enLocation].position - s_stHallMotorInfo[enLocation].Customlocation) >= MotorStopDeadBand_Count))
                        {
                            s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = true;
                        }
                    }
                }
                else if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseStall_Move
                    || s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseSoftStop_Move
                    || (s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_Customlocation_Move
                        && s_stHallMotorInfo[enLocation].Customlocation >= s_stHallMotorInfo[enLocation].position))
                {
                    if(s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] <= MotorTimeOut_MaxCount)
                        s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] ++;
                    /*
                        超时堵转
                    */
                    if(s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] >= MotorTimeOut_MaxCount)
                    {
                        if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseStall_Move)
                        {
                            s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = true;
                        }
                        else if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseSoftStop_Move
                            && s_stHallMotorInfo[enLocation].position <= (s_stHallMotorInfo[enLocation].Motorlength - MotorSoftStop_MaxCount - MotorStopDeadBand_Count))
                        {
                            s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = true;
                        }
                        else if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_Customlocation_Move
                            && (abs(s_stHallMotorInfo[enLocation].Customlocation - s_stHallMotorInfo[enLocation].position) >= MotorStopDeadBand_Count))
                        {
                            s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = true;
                        }
                    }
                }
            }
        }

        if(s_bStudying[enLocation] == true)
        {
            /*
                开启自学习
            */
            Motor_Study(enLocation);
        }
        else
        {
            Motor_APP(enLocation);
        }

        s_LastHallMotor_TurnCount[enLocation] = s_HallMotor_TurnCount[enLocation];
    }
}

/*
    霍尔中断触发
*/
void Hall_Motor_UpdateCount(Hall_Motor_Location enLocation)
{
    if(s_stHallMotorInfo[enLocation].bStudy == false)
    {
        /*
            自学习未完成
            反方向堵转标志位清零(不处于自学习)
            霍尔脉冲计数器递变
        */
        if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardStall_Move)
        {
            if(s_bStudying[enLocation] == false)
                s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = false;

            s_HallMotor_TurnCount[enLocation] --;
        }
        else if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseStall_Move)
        {
            if(s_bStudying[enLocation] == false)
                s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = false;

            s_HallMotor_TurnCount[enLocation] ++;
        }
        /*
            霍尔超时计数器清零
            更新位置
        */
        s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] = 0;
        s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] = 0;
        s_stHallMotorInfo[enLocation].position = s_HallMotor_TurnCount[enLocation];
    }
    else if(s_stHallMotorInfo[enLocation].bStudy == true)
    {
        /*
            自学习完成
            反方向标志位清零
            霍尔脉冲计数器递变
        */
        if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardStall_Move
            || s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ForwardSoftStop_Move
            || (s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_Customlocation_Move
                && s_stHallMotorInfo[enLocation].Customlocation <= s_stHallMotorInfo[enLocation].position))
        {
            s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = false;
            s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Reverse_Turn] = false;
            s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Reverse_Turn] = false;

            if(s_HallMotor_TurnCount[enLocation] <= 0)
                s_HallMotor_TurnCount[enLocation] = 0;
            else
                s_HallMotor_TurnCount[enLocation] --;
        }
        else if(s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseStall_Move
            || s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_ReverseSoftStop_Move
            || (s_stHallMotorInfo[enLocation].enHallMotorMove == Hall_Motor_Customlocation_Move
                && s_stHallMotorInfo[enLocation].Customlocation >= s_stHallMotorInfo[enLocation].position))
        {
            s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = false;
            s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Forward_Turn] = false;
            s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Forward_Turn] = false;

            if(s_HallMotor_TurnCount[enLocation] >= s_stHallMotorInfo[enLocation].Motorlength)
                s_HallMotor_TurnCount[enLocation] = s_stHallMotorInfo[enLocation].Motorlength;
            else
                s_HallMotor_TurnCount[enLocation] ++;

        }
        /*
            霍尔超时计数器清零
            更新位置
        */
        s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] = 0;
        s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] = 0;
        s_stHallMotorInfo[enLocation].position = s_HallMotor_TurnCount[enLocation];
    }
}

void Hall_Motor_DisableStudy(Hall_Motor_Location enLocation)
{
    s_bUnderStudyByStall[enLocation][Hall_Motor_Forward_Turn] = false;
    s_bUnderStudyByStall[enLocation][Hall_Motor_Reverse_Turn] = false;

    s_stHallMotorInfo[enLocation].bStudy = false;
}

void Hall_Motor_StartStudy(Hall_Motor_Location enLocation)
{
    if(s_bStudying[enLocation] == false)
    {
        s_bStudying[enLocation] = true;
        s_stHallMotorInfo[enLocation].bStudy = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].enHallMotorMove = Hall_Motor_Invalid_Move;
        s_stHallMotorInfo[enLocation].Customlocation = 0;

        if(s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun != NULL)
            s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun();
    }
}

void Hall_Motor_StopStudy(Hall_Motor_Location enLocation)
{
    if(s_bStudying[enLocation] == true)
    {
        s_bStudying[enLocation] = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].enHallMotorMove = Hall_Motor_Invalid_Move;
        s_stHallMotorInfo[enLocation].Customlocation = 0;

        if(s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun != NULL)
            s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun();
    }
}

void Hall_Motor_SetMove(Hall_Motor_Location enLocation, Hall_Motor_MoveInfo stHallMotorMoveInfo)
{
//    Hall_Motor_StopStudy(enLocation);

    s_stHallMotorInfo[enLocation].enHallMotorMove = stHallMotorMoveInfo.enHallMotorMove;
    s_stHallMotorInfo[enLocation].Customlocation = stHallMotorMoveInfo.Customlocation;

    if(stHallMotorMoveInfo.enHallMotorMove == Hall_Motor_ForwardStall_Move
        || stHallMotorMoveInfo.enHallMotorMove == Hall_Motor_ForwardSoftStop_Move
        || (stHallMotorMoveInfo.enHallMotorMove == Hall_Motor_Customlocation_Move
            && (s_stHallMotorInfo[enLocation].position - s_stHallMotorInfo[enLocation].Customlocation) >= MotorStartDeadBand_Count))
    {
        /*
            正转
        */
        if(s_stHallMotorTaskInfo[enLocation].HallMotor_ForwardFun != NULL)
            s_stHallMotorTaskInfo[enLocation].HallMotor_ForwardFun();
    }
    else if(stHallMotorMoveInfo.enHallMotorMove == Hall_Motor_ReverseStall_Move
        || stHallMotorMoveInfo.enHallMotorMove == Hall_Motor_ReverseSoftStop_Move
        || (stHallMotorMoveInfo.enHallMotorMove == Hall_Motor_Customlocation_Move
            && (s_stHallMotorInfo[enLocation].Customlocation - s_stHallMotorInfo[enLocation].position) >= MotorStartDeadBand_Count))
    {
        /*
            反转
        */
        if(s_stHallMotorTaskInfo[enLocation].HallMotor_ReverseFun != NULL)
            s_stHallMotorTaskInfo[enLocation].HallMotor_ReverseFun();
    }
    else if(stHallMotorMoveInfo.enHallMotorMove == Hall_Motor_Invalid_Move)
    {
        /*
            停止
        */
        if(s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun != NULL)
            s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun();
    }
}

Hall_Motor_Move Hall_Motor_GetMove(Hall_Motor_Location enLocation)
{
    return s_stHallMotorInfo[enLocation].enHallMotorMove;
}

int Hall_Motor_GetStatus(Hall_Motor_Location enLocation, Hall_Motor_Info *pstHallMotorInfo)
{
    if(pstHallMotorInfo == NULL)
        return -1;

    memcpy(pstHallMotorInfo, &s_stHallMotorInfo[enLocation], sizeof(Hall_Motor_Info));
    return 0;
}

void Hall_Motor_Init()
{
    Hall_Motor_Location enLocation = Hall_Motor_MainDriver_Location;

    for(enLocation = Hall_Motor_MainDriver_Location; enLocation < Hall_Motor_Invalid_Location; enLocation ++)
    {
        s_bStudying[enLocation] = false;
        s_bUnderStudyByStall[enLocation][Hall_Motor_Forward_Turn] = false;
        s_bUnderStudyByStall[enLocation][Hall_Motor_Reverse_Turn] = false;

        s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] = 0;
        s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] = 0;

        s_HallMotor_TurnCount[enLocation] = 0;
        s_LastHallMotor_TurnCount[enLocation] = 0;
        s_LastenHallMotorMove[enLocation] = Hall_Motor_Invalid_Move;

        s_stHallMotorInfo[enLocation].bStudy = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].enHallMotorMove = Hall_Motor_Invalid_Move;
        s_stHallMotorInfo[enLocation].Customlocation = 0;
        s_stHallMotorInfo[enLocation].position = 0;
        s_stHallMotorInfo[enLocation].Motorlength = 0;

        s_stHallMotorTaskInfo[enLocation].HallMotor_ForwardFun = NULL;
        s_stHallMotorTaskInfo[enLocation].HallMotor_ReverseFun = NULL;
        s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun = NULL;
    }
}

void Hall_Motor_DeInit()
{
    Hall_Motor_Location enLocation = Hall_Motor_MainDriver_Location;

    for(enLocation = Hall_Motor_MainDriver_Location; enLocation < Hall_Motor_Invalid_Location; enLocation ++)
    {
        s_bStudying[enLocation] = false;
        s_bUnderStudyByStall[enLocation][Hall_Motor_Forward_Turn] = false;
        s_bUnderStudyByStall[enLocation][Hall_Motor_Reverse_Turn] = false;

        s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] = 0;
        s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] = 0;

        s_HallMotor_TurnCount[enLocation] = 0;
        s_LastHallMotor_TurnCount[enLocation] = 0;
        s_LastenHallMotorMove[enLocation] = Hall_Motor_Invalid_Move;

        s_stHallMotorInfo[enLocation].bStudy = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].enHallMotorMove = Hall_Motor_Invalid_Move;
        s_stHallMotorInfo[enLocation].Customlocation = 0;
        s_stHallMotorInfo[enLocation].position = 0;
        s_stHallMotorInfo[enLocation].Motorlength = 0;

        s_stHallMotorTaskInfo[enLocation].HallMotor_ForwardFun = NULL;
        s_stHallMotorTaskInfo[enLocation].HallMotor_ReverseFun = NULL;
        s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun = NULL;
    }
}

int Hall_Motor_CreatTask(Hall_Motor_Location enLocation, Hall_Motor_TaskInfo * pstHallMotorTaskInfo)
{
    if(pstHallMotorTaskInfo == NULL)
        return -1;

    if(s_stHallMotorTaskInfo[enLocation].HallMotor_ForwardFun == NULL
        && s_stHallMotorTaskInfo[enLocation].HallMotor_ReverseFun == NULL
        && s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun == NULL)
    {
        s_bStudying[enLocation] = false;
        s_bUnderStudyByStall[enLocation][Hall_Motor_Forward_Turn] = false;
        s_bUnderStudyByStall[enLocation][Hall_Motor_Reverse_Turn] = false;

        s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] = 0;
        s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] = 0;

        s_HallMotor_TurnCount[enLocation] = 0;
        s_LastHallMotor_TurnCount[enLocation] = 0;
        s_LastenHallMotorMove[enLocation] = Hall_Motor_Invalid_Move;

        s_stHallMotorInfo[enLocation].bStudy = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].enHallMotorMove = Hall_Motor_Invalid_Move;
        s_stHallMotorInfo[enLocation].Customlocation = 0;
        s_stHallMotorInfo[enLocation].position = 0;
        s_stHallMotorInfo[enLocation].Motorlength = 0;

        s_stHallMotorTaskInfo[enLocation].HallMotor_ForwardFun = pstHallMotorTaskInfo->HallMotor_ForwardFun;
        s_stHallMotorTaskInfo[enLocation].HallMotor_ReverseFun = pstHallMotorTaskInfo->HallMotor_ReverseFun;
        s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun = pstHallMotorTaskInfo->HallMotor_StopFun;
        return 0;
    }
    return -1;
}

int Hall_Motor_DestroyTask(Hall_Motor_Location enLocation, Hall_Motor_TaskInfo * pstHallMotorTaskInfo)
{
    if(pstHallMotorTaskInfo == NULL)
        return -1;

    if(s_stHallMotorTaskInfo[enLocation].HallMotor_ForwardFun == pstHallMotorTaskInfo->HallMotor_ForwardFun
        && s_stHallMotorTaskInfo[enLocation].HallMotor_ReverseFun == pstHallMotorTaskInfo->HallMotor_ReverseFun
        && s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun == pstHallMotorTaskInfo->HallMotor_StopFun)
    {
        s_bStudying[enLocation] = false;
        s_bUnderStudyByStall[enLocation][Hall_Motor_Forward_Turn] = false;
        s_bUnderStudyByStall[enLocation][Hall_Motor_Reverse_Turn] = false;

        s_HallMotor_TimeCount[enLocation][Hall_Motor_Forward_Turn] = 0;
        s_HallMotor_TimeCount[enLocation][Hall_Motor_Reverse_Turn] = 0;

        s_HallMotor_TurnCount[enLocation] = 0;
        s_LastHallMotor_TurnCount[enLocation] = 0;
        s_LastenHallMotorMove[enLocation] = Hall_Motor_Invalid_Move;

        s_stHallMotorInfo[enLocation].bStudy = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bStall[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bSoftStop[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Forward_Turn] = false;
        s_stHallMotorInfo[enLocation].bAntiPpinch[Hall_Motor_Reverse_Turn] = false;
        s_stHallMotorInfo[enLocation].enHallMotorMove = Hall_Motor_Invalid_Move;
        s_stHallMotorInfo[enLocation].Customlocation = 0;
        s_stHallMotorInfo[enLocation].position = 0;
        s_stHallMotorInfo[enLocation].Motorlength = 0;

        s_stHallMotorTaskInfo[enLocation].HallMotor_ForwardFun = NULL;
        s_stHallMotorTaskInfo[enLocation].HallMotor_ReverseFun = NULL;
        s_stHallMotorTaskInfo[enLocation].HallMotor_StopFun = NULL;
        return 0;
    }
    return -1;
}

//#define Hall_Motor_MAIN_DEBUG
#ifdef Hall_Motor_MAIN_DEBUG

#include <time.h>

//#define Hall_Motor_AUTOSTUDY_DEBUG
#define Hall_Motor_MANUALSTUDY_DEBUG

void HallMotor_ForwardFun(void)
{
    time_t seconds;
    seconds = time(NULL);
    struct tm * gmt;
    gmt = localtime(&seconds);
    printf("%s-%d-%dS\n",__func__, __LINE__, gmt->tm_sec);
}

void HallMotor_ReverseFun(void)
{
    time_t seconds;
    seconds = time(NULL);
    struct tm * gmt;
    gmt = localtime(&seconds);
    printf("%s-%d-%dS\n",__func__, __LINE__, gmt->tm_sec);
}

void HallMotor_StopFun(void)
{
    time_t seconds;
    seconds = time(NULL);
    struct tm * gmt;
    gmt = localtime(&seconds);
    printf("%s-%d-%dS\n",__func__, __LINE__, gmt->tm_sec);
}

int main()
{
    Hall_Motor_Init();

    Hall_Motor_TaskInfo stHallMotorTaskInfo = {0};
    stHallMotorTaskInfo.HallMotor_ForwardFun = HallMotor_ForwardFun;
    stHallMotorTaskInfo.HallMotor_ReverseFun = HallMotor_ReverseFun;
    stHallMotorTaskInfo.HallMotor_StopFun = HallMotor_StopFun;
    Hall_Motor_CreatTask(Hall_Motor_MainDriver_Location, &stHallMotorTaskInfo);

#ifdef Hall_Motor_AUTOSTUDY_DEBUG
    Hall_Motor_StartStudy(Hall_Motor_MainDriver_Location);
#endif

    while(1)
    {
        static bool bSet1 = false;
        static bool bSet2 = false;
        static bool bSet3 = false;

        Hall_Motor_Info stHallMotorInfo;
        Hall_Motor_GetStatus(Hall_Motor_MainDriver_Location, &stHallMotorInfo);

#ifdef Hall_Motor_AUTOSTUDY_DEBUG
        if(stHallMotorInfo.bStudy == true
            && bSet1 == false)
        {
            bSet1 = true;
            printf("Study success\n");
            printf("position = %d\n", stHallMotorInfo.position);
            printf("Motorlength = %d\n", stHallMotorInfo.Motorlength);
            Hall_Motor_MoveInfo stHallMotorMoveInfo = {0};
            stHallMotorMoveInfo.enHallMotorMove = Hall_Motor_ForwardSoftStop_Move;
            stHallMotorMoveInfo.Customlocation = 0;
            Hall_Motor_SetMove(Hall_Motor_MainDriver_Location, stHallMotorMoveInfo);
        }

        if(stHallMotorInfo.bSoftStop[Hall_Motor_Forward_Turn] == true
            && bSet2 == false)
        {
            sleep(5);
            bSet2 = true;
            printf("To SoftStop\n");
            printf("position = %d\n", stHallMotorInfo.position);
            printf("Motorlength = %d\n", stHallMotorInfo.Motorlength);
            Hall_Motor_MoveInfo stHallMotorMoveInfo = {0};
            stHallMotorMoveInfo.enHallMotorMove = Hall_Motor_ReverseStall_Move;
            stHallMotorMoveInfo.Customlocation = 0;
            Hall_Motor_SetMove(Hall_Motor_MainDriver_Location, stHallMotorMoveInfo);
        }

        if(stHallMotorInfo.bStall[Hall_Motor_Reverse_Turn] == true
            && bSet3 == false)
        {
            sleep(5);
            bSet3 = true;
            printf("To Stall\n");
            printf("position = %d\n", stHallMotorInfo.position);
            printf("Motorlength = %d\n", stHallMotorInfo.Motorlength);
            Hall_Motor_MoveInfo stHallMotorMoveInfo = {0};
            stHallMotorMoveInfo.enHallMotorMove = Hall_Motor_Customlocation_Move;
            stHallMotorMoveInfo.Customlocation = 20;
            Hall_Motor_SetMove(Hall_Motor_MainDriver_Location, stHallMotorMoveInfo);
            continue;
        }

        if(stHallMotorInfo.enHallMotorMove == Hall_Motor_Invalid_Move
            && bSet1 == true && bSet2 == true && bSet3 == true)
        {
            printf("To specified location\n");
            printf("position = %d\n", stHallMotorInfo.position);
            printf("Motorlength = %d\n", stHallMotorInfo.Motorlength);
            break;
        }
#endif

#ifdef Hall_Motor_MANUALSTUDY_DEBUG
        if(bSet1 == false)
        {
            printf("To Hall_Motor_ReverseStall_Move\n");
            printf("position = %d\n", stHallMotorInfo.position);
            printf("Motorlength = %d\n", stHallMotorInfo.Motorlength);
            bSet1 = true;
            Hall_Motor_MoveInfo stHallMotorMoveInfo = {0};
            stHallMotorMoveInfo.enHallMotorMove = Hall_Motor_ReverseStall_Move;
            stHallMotorMoveInfo.Customlocation = 0;
            Hall_Motor_SetMove(Hall_Motor_MainDriver_Location, stHallMotorMoveInfo);
        }
        else if(bSet1 == true 
            && bSet2 == false 
            && stHallMotorInfo.bStall[Hall_Motor_Reverse_Turn] == true)
        {
            sleep(8);
            printf("To Hall_Motor_ForwardStall_Move\n");
            printf("position = %d\n", stHallMotorInfo.position);
            printf("Motorlength = %d\n", stHallMotorInfo.Motorlength);
            bSet2 = true;
            Hall_Motor_MoveInfo stHallMotorMoveInfo = {0};
            stHallMotorMoveInfo.enHallMotorMove = Hall_Motor_ForwardStall_Move;
            stHallMotorMoveInfo.Customlocation = 0;
            Hall_Motor_SetMove(Hall_Motor_MainDriver_Location, stHallMotorMoveInfo);
        }

        if(bSet1 == true 
            && bSet2 == true 
            && stHallMotorInfo.bStudy == true)
        {
            printf("Study success\n");
            printf("position = %d\n", stHallMotorInfo.position);
            printf("Motorlength = %d\n", stHallMotorInfo.Motorlength);
            break;
        }
#endif

        Hall_Motor_Run();

        time_t seconds;
        seconds = time(NULL);
        struct tm * gmt;
        gmt = localtime(&seconds);
        if((gmt->tm_sec >= 0 && gmt->tm_sec < 25)
            || (gmt->tm_sec >= 30 && gmt->tm_sec < 55))
        {
            Hall_Motor_UpdateCount(Hall_Motor_MainDriver_Location);
        }
        sleep(1);
    }

    Hall_Motor_DestroyTask(Hall_Motor_MainDriver_Location, &stHallMotorTaskInfo);

    Hall_Motor_DeInit();

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

