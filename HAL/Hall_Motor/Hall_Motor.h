
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#define MotorTimeOut_MaxCount                   100 /* 霍尔超时计数 100*10Ms (电机堵转) */

#define MotorSoftStop_MaxCount                  10  /* 软停点 霍尔步数 */

#define MotorStartDeadBand_Count                10  /* 启动死区 霍尔步数 */

#define MotorStopDeadBand_Count                 10  /* 停止死区 霍尔步数 */

typedef enum 
{
    Hall_Motor_Forward_Turn                  = 0,    /* 正转 靠近起始点 */
    Hall_Motor_Reverse_Turn                  = 1,    /* 反转 */
    Hall_Motor_Invalid_Turn                  = 2,
}Hall_Motor_Turn;

typedef enum 
{
    Hall_Motor_ForwardStall_Move             = 0,    /* 正转直到堵转 用于自学习和自动校准 */
    Hall_Motor_ReverseStall_Move             = 1,    /* 反转直到堵转 用于自学习和自动校准 */
    Hall_Motor_ForwardSoftStop_Move          = 2,    /* 正转直到软停点 常规场景 */
    Hall_Motor_ReverseSoftStop_Move          = 3,    /* 反转直到软停点 常规场景 */
    Hall_Motor_CUSTOMLOCATION_Move           = 4,    /* 自定义位置 用于迎宾和礼让 */
    Hall_Motor_Invalid_Move                  = 5,
}Hall_Motor_Move;

typedef enum 
{
    Hall_Motor_MainDriver_Location            = 0,    /* 主驾驶座椅 */
    Hall_Motor_SecondDriver_Location          = 1,    /* 副驾驶座椅 */
    Hall_Motor_Invalid_Location               = 2,
}Hall_Motor_Location;

typedef struct Hall_Motor_MoveInfo
{
    Hall_Motor_Move enHallMotorMove;                /* 移动方式 */
    int Customlocation;                             /*  */
} Hall_Motor_MoveInfo;

typedef void (*Hall_Motor_ForwardFun)(void);

typedef void (*Hall_Motor_ReverseFun)(void);

typedef void (*Hall_Motor_StopFun)(void);

/*
    正转 靠近起始点
    自学习，正转回到起始点，再反转到堵转点，计算总长度
*/
typedef struct Hall_Motor_Info
{
    bool bStudy;                                    /* 自学习标志位 */
    bool bStall[Hall_Motor_Invalid_Turn];           /* 堵转标志位 */
    bool bSoftStop[Hall_Motor_Invalid_Turn];        /* 软停点标志位 预留 */
    bool bAntiPpinch[Hall_Motor_Invalid_Turn];      /* 防夹标志位 预留 */
    Hall_Motor_Move enHallMotorMove;                /* 移动方式 */
    int Customlocation;                             /*  */
    int position;                                   /* 当前位置 */
    int Motorlength;                                /* 总长度 等同于单程霍尔计数总数 */
} Hall_Motor_Info;

typedef struct Hall_Motor_TaskInfo
{
    Hall_Motor_ForwardFun HallMotor_ForwardFun;     /* 正转回调 */
    Hall_Motor_ReverseFun HallMotor_ReverseFun;     /* 反转回调 */
    Hall_Motor_StopFun HallMotor_StopFun;           /* 停止回调 */
} Hall_Motor_TaskInfo;

void Hall_Motor_10MsRun();

/*
    霍尔中断触发
*/
void Hall_Motor_UpdateCount(Hall_Motor_Location enLocation);

void Hall_Motor_DisableStudy(Hall_Motor_Location enLocation);

/*
    自学习，正转回到起始点，再反转到堵转点，计算总长度
*/
void Hall_Motor_StartStudy(Hall_Motor_Location enLocation);

void Hall_Motor_StopStudy(Hall_Motor_Location enLocation);

void Hall_Motor_SetMove(Hall_Motor_Location enLocation, Hall_Motor_MoveInfo stHallMotorMoveInfo);

Hall_Motor_Move Hall_Motor_GetMove(Hall_Motor_Location enLocation);

void Hall_Motor_GetStatus(Hall_Motor_Location enLocation, Hall_Motor_Info *pstHallMotorInfo);

void Hall_Motor_Init();

void Hall_Motor_DeInit();

void Hall_Motor_CreatTask(Hall_Motor_Location enLocation, Hall_Motor_TaskInfo * pstHallMotorTaskInfo);

void Hall_Motor_DestroyTask(Hall_Motor_Location enLocation, Hall_Motor_TaskInfo * pstHallMotorTaskInfo);

#ifdef __cplusplus
}
#endif

