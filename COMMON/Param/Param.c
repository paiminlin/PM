/*
    From : https://github.com/paiminlin/PM
    From : 
    Author : PaiMin.lin
    Date : 2022.7.19
*/

#include "Param.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Param_Info
{
    bool bParamInit;
    bool bParamUpdate[Param_Module_Invalid];
} Param_Info;

static Param_Info s_stParamInfo = {0};
static ParamModule_Info s_stParamModuleInfo = {0};
static ParamModule_Data s_stParamModuleData = {0};

int Param_Update(bool bForceUpdate)
{
    if(s_stParamInfo.bParamInit == false)
        return 0;

    if(s_stParamInfo.bParamUpdate[Param_Module_1] == true || bForceUpdate == true)
    {
        s_stParamInfo.bParamUpdate[Param_Module_1] = false;
        if(s_stParamModuleInfo.ParamModule1Fun != NULL)
        {
            s_stParamModuleInfo.ParamModule1Fun(Param_Deal_Write, s_stParamModuleData.stParamModuleData1.Buf, 
                                                sizeof(s_stParamModuleData.stParamModuleData1.Buf));
        }
    }

    if(s_stParamInfo.bParamUpdate[Param_Module_2] == true || bForceUpdate == true)
    {
        s_stParamInfo.bParamUpdate[Param_Module_2] = false;
        if(s_stParamModuleInfo.ParamModule2Fun != NULL)
        {
            s_stParamModuleInfo.ParamModule2Fun(Param_Deal_Write, s_stParamModuleData.stParamModuleData2.Buf, 
                                                sizeof(s_stParamModuleData.stParamModuleData2.Buf));
        }
    }
    return 0;

}

int Param_Reset(bool bForceReset)
{
    if(s_stParamInfo.bParamInit == false)
        return -1;

    if(s_stParamModuleInfo.ParamModule1Fun != NULL)
    {
        ParamModule_Data1 s_stParamModuleData1 = {0};
        if((s_stParamModuleInfo.ParamModule1Fun(Param_Deal_Read, s_stParamModuleData1.Buf, 
                                            sizeof(s_stParamModuleData1.Buf)) == 0) || bForceReset == true)
        {
            memcpy(s_stParamModuleData.stParamModuleData1.Buf, s_stParamModuleData1.Buf, sizeof(s_stParamModuleData1.Buf));
        }
    }

    if(s_stParamModuleInfo.ParamModule2Fun != NULL)
    {
        ParamModule_Data2 s_stParamModuleData2 = {0};
        if((s_stParamModuleInfo.ParamModule2Fun(Param_Deal_Read, s_stParamModuleData2.Buf, 
                                            sizeof(s_stParamModuleData2.Buf)) == 0) || bForceReset == true)
        {
            memcpy(s_stParamModuleData.stParamModuleData2.Buf, s_stParamModuleData2.Buf, sizeof(s_stParamModuleData2.Buf));
        }
    }
    return 0;
}

int Param_Init(ParamModule_Info * pstParamModuleInfo)
{
    if(s_stParamInfo.bParamInit == true)
        return 0;

    if(pstParamModuleInfo == NULL)
        return -1;

    Param_Module enParamModule = Param_Module_1;

    s_stParamInfo.bParamInit = true;
    for(enParamModule = Param_Module_1; enParamModule < Param_Module_Invalid; enParamModule ++)
    {
        s_stParamInfo.bParamUpdate[enParamModule] = false;
    }

    s_stParamModuleInfo.ParamModule1Fun = pstParamModuleInfo->ParamModule1Fun;
    s_stParamModuleInfo.ParamModule2Fun = pstParamModuleInfo->ParamModule2Fun;

    memset(&s_stParamModuleData, 0x00, sizeof(s_stParamModuleData));
    Param_Reset(true);

    return 0;
}

int Param_DeInit(void)
{
    if(s_stParamInfo.bParamInit == false)
        return 0;

    Param_Module enParamModule = Param_Module_1;

    s_stParamInfo.bParamInit = false;
    for(enParamModule = Param_Module_1; enParamModule < Param_Module_Invalid; enParamModule ++)
    {
        s_stParamInfo.bParamUpdate[enParamModule] = false;
    }
    s_stParamModuleInfo.ParamModule1Fun = NULL;
    s_stParamModuleInfo.ParamModule2Fun = NULL;
    memset(&s_stParamModuleData, 0x00, sizeof(s_stParamModuleData));

    return 0;
}

int Param_SetModuleData(Param_Module enParamModule, void * pData, int length)
{
    if(s_stParamInfo.bParamInit == false)
        return -1;

    if(pData == NULL)
        return -1;

    switch(enParamModule)
    {
        case Param_Module_1:
            if(memcmp(s_stParamModuleData.stParamModuleData1.Buf, (char *)pData, length) != 0)
            {
                s_stParamInfo.bParamUpdate[Param_Module_1] = true;
                memcpy(s_stParamModuleData.stParamModuleData1.Buf, (char *)pData, length);
            }
            break;

        case Param_Module_2:
            if(memcmp(s_stParamModuleData.stParamModuleData2.Buf, (char *)pData, length) != 0)
            {
                s_stParamInfo.bParamUpdate[Param_Module_2] = true;
                memcpy(s_stParamModuleData.stParamModuleData2.Buf, (char *)pData, length);
            }
            break;

        default :
            break;
    }

    return 0;
}

int Param_GetModuleData(Param_Module enParamModule, void * pData, int length)
{
    if(s_stParamInfo.bParamInit == false)
        return -1;

    if(pData == NULL)
        return -1;

    switch(enParamModule)
    {
        case Param_Module_1:
            memcpy((char *)pData, s_stParamModuleData.stParamModuleData1.Buf, length);
            break;

        case Param_Module_2:
            memcpy((char *)pData, s_stParamModuleData.stParamModuleData2.Buf, length);
            break;

        default :
            break;
    }

    return 0;
}

//#define Param_MAIN_DEBUG
#ifdef Param_MAIN_DEBUG

#include <time.h>
#include <unistd.h>

#define FILENAME        "./Paramtest.txt"

int ParamModule1Fun(Param_Deal enParamDeal, void * pData, int length)
{
    if(pData == NULL)
        return -1;

    int ret = -1;
    FILE * fp = NULL;

    if(enParamDeal == Param_Deal_Read)
    {
        char buf[64] = "Defult";

        fp = fopen(FILENAME, "r+");
        if(fp == NULL){
            printf("%s-%d\n",__func__, __LINE__);
            memcpy(pData, buf, sizeof(buf));
            return -1;
        }

        ret = fread(pData, length, 1, fp);
        if(ret < 0){
            printf("%s-%d\n",__func__, __LINE__);
            memcpy(pData, buf, sizeof(buf));
            fclose(fp);
            return -1;
        }

        printf("%s-%d\n",__func__, __LINE__);
        fclose(fp);
    }
    else if(enParamDeal == Param_Deal_Write)
    {
        fp = fopen(FILENAME, "w+");
        if(fp == NULL){
            printf("%s-%d\n",__func__, __LINE__);
            return -1;
        }

        ret = fwrite(pData, length, 1, fp);
        if(ret < 0){
            printf("%s-%d\n",__func__, __LINE__);
            fclose(fp);
            return -1;
        }

        printf("%s-%d\n",__func__, __LINE__);
        fclose(fp);
    }

    return 0;
}

int main()
{
    ParamModule_Info stParamModuleInfo = {0};
    stParamModuleInfo.ParamModule1Fun = ParamModule1Fun;
    stParamModuleInfo.ParamModule2Fun = NULL;

    Param_Init(&stParamModuleInfo);

    ParamModule_Data1 stParamModuleData1 = {0};

    while(1)
    {
        time_t seconds;
        seconds = time(NULL);
        struct tm * gmt;
        gmt = localtime(&seconds);
        if((gmt->tm_sec)%5 == 0)
        {
            char buf[64] = {0};
            memset(buf, 0x0, sizeof(buf));
            snprintf(buf, sizeof(buf), "%02d:%02d:%02d", gmt->tm_hour,gmt->tm_min,gmt->tm_sec);
            memcpy(stParamModuleData1.Buf, buf, sizeof(buf));
            Param_SetModuleData(Param_Module_1, stParamModuleData1.Buf, sizeof(stParamModuleData1.Buf));
            break;
        }
    }

    Param_GetModuleData(Param_Module_1, stParamModuleData1.Buf, sizeof(stParamModuleData1.Buf));
    printf("%s\n", stParamModuleData1.Buf);

    Param_Update(true);

    Param_DeInit();

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

