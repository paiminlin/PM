
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef _LookUpTable_H_
#define _LookUpTable_H_

typedef enum 
{
    LookUpTable_PollingMode = 0,                /* 顺序查表 */
    LookUpTable_DichotomyMode,                  /* 二分法查表 */
    LookUpTable_InvalidMode,                    /* 无效 */
}LookUpTable_Mode;

typedef struct LookUpTable_Info
{
    float Input;
    float Output;
} LookUpTable_Info;

/*
    pstLookUpTableInfo : 表数据, 顺序表(Input顺序小->大)
    TableLength : 表长度
*/
int LookUpTable_Output(LookUpTable_Mode enLookUpTableMode, float Input, float * pOutput, 
                                const LookUpTable_Info * pstLookUpTableInfo, const int TableLength);

#endif

#ifdef __cplusplus
}
#endif

