/*
    From : https://github.com/paiminlin/PM
    From : 
    Author : PaiMin.lin
    Date : 2022.8.14
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef PRINTLOG_H_
#define PRINTLOG_H_

typedef enum 
{
    PrintLog_BebugLevel         = 0,
    PrintLog_WarnLevel          = 1,
    PrintLog_ErrorLevel         = 2,
    PrintLog_InvalidLevel       = 3,
}PrintLog_Level;

void PrintLog_Init(PrintLog_Level enPrintLogLevel);

void PrintLog_DeInit(void);

void PrintLog_Bebug(void);

void PrintLog_Warn(void);

void PrintLog_Error(void);

#endif /* PRINTLOG_H_ */

#ifdef __cplusplus
}
#endif

