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

void PrintLog_Init(void);

void PrintLog_DeInit(void);

void PrintLog(void);

#endif /* PRINTLOG_H_ */

#ifdef __cplusplus
}
#endif

