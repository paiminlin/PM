
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef _LookUpTable_H_
#define _LookUpTable_H_

//#define POLLING_LOOKUPTABLE         1           //顺序查表
#define DICHOTOMY_LOOKUPTABLE       1           //二分法查表

typedef struct LookUpTable_Info
{
    float Input;
    float Output;
} LookUpTable_Info;

/*
    pstLookUpTableInfo : 表数据, 顺序表(Input顺序小->大)
    TableLength : 表长度
*/
float LookUpTable_Output(float Input, const LookUpTable_Info * pstLookUpTableInfo, const int TableLength);

#endif

#ifdef __cplusplus
}
#endif

