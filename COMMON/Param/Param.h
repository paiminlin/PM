
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef PARAM_H_
#define PARAM_H_

int Param_Run(void);

int Param_Update(void);

int Param_Reset(void);

int Param_Init(void);

int Param_DeInit(void);

int Param_UpdateModuleValue(void);

int Param_SetModuleValue(void);

int Param_GetModuleValue(void);

#endif /* PARAM_H_ */

#ifdef __cplusplus
}
#endif

