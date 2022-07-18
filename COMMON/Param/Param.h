/*
    From : https://github.com/paiminlin/PM
    From : 
    Author : PaiMin.lin
    Date : 2022.7.18
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef PARAM_H_
#define PARAM_H_

typedef enum 
{
    Param_Deal_Read         = 0,
    Param_Deal_Write        = 1,
    Param_Deal_Invalid      = 2,
}Param_Deal;

typedef enum 
{
    Param_Module_1          = 0,
    Param_Module_2          = 1,
    Param_Module_Invalid    = 2,
}Param_Module;

typedef union ParamModule_Data1
{
   char Buf[1024];
   struct
   {
       int a;
       int b;
   }Data;
} ParamModule_Data1;

typedef union ParamModule_Data2
{
   char Buf[1024];
   struct
   {
       int a;
       int b;
   }Data;
} ParamModule_Data2;

typedef struct ParamModule_Data
{
    ParamModule_Data1 stParamModuleData1;
    ParamModule_Data2 stParamModuleData2;
} ParamModule_Data;

typedef int (*ParamModule1_Fun)(Param_Deal enParamDeal, void * pData, int length);

typedef int (*ParamModule2_Fun)(Param_Deal enParamDeal, void * pData, int length);

typedef struct ParamModule_Info
{
    ParamModule1_Fun ParamModule1Fun;
    ParamModule2_Fun ParamModule2Fun;
} ParamModule_Info;

int Param_Update(Param_Module enParamModule, bool bForceReset);

int Param_Reset(Param_Module enParamModule, bool bForceReset);

int Param_Init(ParamModule_Info * pstParamModuleInfo);

int Param_DeInit(void);

int Param_SetModuleData(Param_Module enParamModule, void * pData, int length);

int Param_GetModuleData(Param_Module enParamModule, void * pData, int length);

#endif /* PARAM_H_ */

#ifdef __cplusplus
}
#endif

