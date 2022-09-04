#if 0
#include "fsm.h"
#include <ctype.h>

typedef enum {
    IGN_OFF_STATUS      = 0,
    IGN_ACC_STATUS      = 1,
    IGN_ON_STATUS       = 2,
    IGN_INVALID_STATUS  = 3,
} IGN_Status;

typedef enum {
    IGN_OFF_EVENT       = 0,
    IGN_ACC_EVENT       = 1,
    IGN_ON_EVENT        = 2,
    IGN_INVALID_EVENT   = 3,
} IGN_Event;

struct ign_para {
    struct fsm_t *fsm;
    int c;
};

static IGN_Event s_enIGNEvent = IGN_INVALID_EVENT;
static void Set_IGNEvent(IGN_Event enIGNEvent)
{
    s_enIGNEvent = enIGNEvent;
}
static IGN_Event Get_IGNEvent(void *p)
{
    IGN_Event enIGNEvent = s_enIGNEvent;
    s_enIGNEvent = IGN_INVALID_EVENT;
    return enIGNEvent;
}

static void *ToIGN_OFF_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *InIGN_OFF_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    Set_IGNEvent(IGN_ACC_EVENT);
    return NULL;
}
static void *ToIGN_ACC_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *InIGN_ACC_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    Set_IGNEvent(IGN_ON_EVENT);
    return NULL;
}
static void *ToIGN_ON_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *InIGN_ON_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    Set_IGNEvent(IGN_OFF_EVENT);
    return NULL;
}
static void *ToStop(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    fsm_stop(((struct ign_para *)p)->fsm);
    return NULL;
}

int main()
{
    struct ign_para func_ign_para;
    struct fsm_branch ign_branch[IGN_INVALID_STATUS][IGN_INVALID_EVENT] = {
        /* IGN_OFF_STATUS */
        {
            {IGN_ACC_EVENT, IGN_ACC_STATUS, ToIGN_ACC_STATUS},
            {IGN_ON_EVENT, IGN_ON_STATUS, ToIGN_ON_STATUS},
            {IGN_INVALID_EVENT, IGN_OFF_STATUS, InIGN_OFF_STATUS},
        },
        /* IGN_ACC_STATUS */
        {
            {IGN_OFF_EVENT, IGN_OFF_STATUS, ToIGN_OFF_STATUS},
            {IGN_ON_EVENT, IGN_ON_STATUS, ToIGN_ON_STATUS},
            {IGN_INVALID_EVENT, IGN_ACC_STATUS, InIGN_ACC_STATUS},
        },
        /* IGN_ON_STATUS */
        {
            {IGN_OFF_EVENT, IGN_OFF_STATUS, ToIGN_OFF_STATUS},
            {IGN_ACC_EVENT, IGN_ACC_STATUS, ToIGN_ACC_STATUS},
            {IGN_INVALID_EVENT, IGN_ON_STATUS, InIGN_ON_STATUS},
        },
    };
    struct fsm_state fsm_ign_state[IGN_INVALID_STATUS] = {
        {IGN_OFF_STATUS, sizeof(ign_branch[0]) / sizeof(ign_branch[0][0]), ign_branch[0],},
        {IGN_ACC_STATUS, sizeof(ign_branch[1]) / sizeof(ign_branch[1][0]), ign_branch[1],},
        {IGN_ON_STATUS,  sizeof(ign_branch[2]) / sizeof(ign_branch[2][0]), ign_branch[2],},
    };
    func_ign_para.fsm = fsm_create_with_state(fsm_ign_state, 
                sizeof(fsm_ign_state) / sizeof(fsm_ign_state[0]), 
                sizeof(ign_branch[0]) / sizeof(ign_branch[0][0]),
                IGN_OFF_STATUS);
//    while(1){
        fsm_run(func_ign_para.fsm, Get_IGNEvent, &func_ign_para, &func_ign_para, NULL);
//        sleep(1);
//    }

    fsm_release(func_ign_para.fsm);
    return 0;
}
#endif
