
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
static ParamModule_Info stParamModuleInfo = {0};
static ParamModule_Data stParamModuleData = {0};

int Param_Update(Param_Module enParamModule)
{
    if(s_stParamInfo.bParamInit == false)
        return 0;

    if(s_stParamInfo.bParamUpdate[Param_Module_1] == true
        && (enParamModule == Param_Module_1 || enParamModule == Param_Module_Invalid))
    {
        s_stParamInfo.bParamUpdate[Param_Module_1] = false;
        if(stParamModuleInfo.ParamModule1Fun != NULL)
        {
            stParamModuleInfo.ParamModule1Fun(Param_Deal_Write, stParamModuleData.stParamModuleData1.Buf, 
                                                sizeof(stParamModuleData.stParamModuleData1.Buf));
        }
    }

    if(s_stParamInfo.bParamUpdate[Param_Module_2] == true
        && (enParamModule == Param_Module_2 || enParamModule == Param_Module_Invalid))
    {
        s_stParamInfo.bParamUpdate[Param_Module_2] = false;
        if(stParamModuleInfo.ParamModule2Fun != NULL)
        {
            stParamModuleInfo.ParamModule2Fun(Param_Deal_Write, stParamModuleData.stParamModuleData2.Buf, 
                                                sizeof(stParamModuleData.stParamModuleData2.Buf));
        }
    }
    return 0;

}

int Param_Reset(Param_Module enParamModule, bool bReset)
{
    if(s_stParamInfo.bParamInit == false)
        return -1;

    if(enParamModule == Param_Module_1 && stParamModuleInfo.ParamModule1Fun != NULL)
    {
        ParamModule_Data1 stParamModuleData1 = {0};
        if((stParamModuleInfo.ParamModule1Fun(Param_Deal_Read, stParamModuleData1.Buf, 
                                            sizeof(stParamModuleData1.Buf)) == 0) || bReset == true)
        {
            memcpy(stParamModuleData.stParamModuleData1.Buf, stParamModuleData1.Buf, sizeof(stParamModuleData1.Buf));
        }
    }

    if(enParamModule == Param_Module_2 && stParamModuleInfo.ParamModule2Fun != NULL)
    {
        ParamModule_Data2 stParamModuleData2 = {0};
        if((stParamModuleInfo.ParamModule2Fun(Param_Deal_Read, stParamModuleData2.Buf, 
                                            sizeof(stParamModuleData2.Buf)) == 0) || bReset == true)
        {
            memcpy(stParamModuleData.stParamModuleData2.Buf, stParamModuleData2.Buf, sizeof(stParamModuleData2.Buf));
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

    stParamModuleInfo.ParamModule1Fun = pstParamModuleInfo->ParamModule1Fun;
    stParamModuleInfo.ParamModule2Fun = pstParamModuleInfo->ParamModule2Fun;

    memset(&stParamModuleData, 0x00, sizeof(ParamModule_Data));

    for(enParamModule = Param_Module_1; enParamModule < Param_Module_Invalid; enParamModule ++)
    {
        Param_Reset(enParamModule, true);
    }
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
    stParamModuleInfo.ParamModule1Fun = NULL;
    stParamModuleInfo.ParamModule2Fun = NULL;
    memset(&stParamModuleData, 0x00, sizeof(ParamModule_Data));

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
            if(memcmp(stParamModuleData.stParamModuleData1.Buf, (char *)pData, length) != 0)
            {
                s_stParamInfo.bParamUpdate[Param_Module_1] = true;
                memcpy(stParamModuleData.stParamModuleData1.Buf, (char *)pData, length);
            }
            break;

        case Param_Module_2:
            if(memcmp(stParamModuleData.stParamModuleData2.Buf, (char *)pData, length) != 0)
            {
                s_stParamInfo.bParamUpdate[Param_Module_2] = true;
                memcpy(stParamModuleData.stParamModuleData2.Buf, (char *)pData, length);
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
            memcpy((char *)pData, stParamModuleData.stParamModuleData1.Buf, length);
            break;

        case Param_Module_2:
            memcpy((char *)pData, stParamModuleData.stParamModuleData2.Buf, length);
            break;

        default :
            break;
    }

    return 0;
}

//#define Param_MAIN_DEBUG
#ifdef Param_MAIN_DEBUG

int main()
{
    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

