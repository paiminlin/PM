#if 0
#include "fsm.h"
#include <ctype.h>

typedef enum {
    FOB_FIND_STATUS     = 0,
    FOB_WAIT_STATUS     = 1,
    FOB_NORMAL_STATUS   = 2,
    FOB_INVALID_STATUS  = 3,
} Fob_Status;

typedef enum {
    FOB_FIND_EVENT      = 0,
    FOB_WAIT_EVENT      = 1,
    FOB_NORMAL_EVENT    = 2,
    FOB_INVALID_EVENT   = 3,
} Fob_Event;

struct fob_para {
    struct fsm_t *fsm;
    int c;
};

static Fob_Event s_enFobEvent = FOB_INVALID_EVENT;
static void Set_FobEvent(Fob_Event enFobevent)
{
    s_enFobEvent = enFobevent;
}
static Fob_Event Get_FobEvent(void *p)
{
    Fob_Event enFobevent = s_enFobEvent;
    s_enFobEvent = FOB_INVALID_EVENT;
    return enFobevent;
}

static void *ToFOB_FIND_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *InFOB_FIND_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *ToFOB_WAIT_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *InFOB_WAIT_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *ToFOB_NORMAL_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *InFOB_NORMAL_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}

int main()
{
    struct fob_para func_fob_para;
    struct fsm_branch fob_branch[][4] = {
        /* FOB_FIND_STATUS */
        {
            {FOB_WAIT_EVENT, FOB_WAIT_STATUS, ToFOB_WAIT_STATUS},
        },
        /* FOB_WAIT_STATUS */
        {
            {FOB_FIND_EVENT, FOB_WAIT_STATUS, InFOB_WAIT_STATUS},
            {FOB_WAIT_EVENT, FOB_WAIT_STATUS, InFOB_WAIT_STATUS},
            {FOB_NORMAL_EVENT, FOB_NORMAL_STATUS, ToFOB_NORMAL_STATUS},
            {FOB_INVALID_EVENT, FOB_WAIT_STATUS, InFOB_WAIT_STATUS},
        },
        /* FOB_NORMAL_STATUS */
        {
            {FOB_FIND_EVENT, FOB_FIND_STATUS, ToFOB_FIND_STATUS},
        },
    };
    struct fsm_state fsm_fob_state[] = {
        {FOB_FIND_STATUS, sizeof(fob_branch[0]) / sizeof(fob_branch[0][0]), fob_branch[0],},
        {FOB_WAIT_STATUS, sizeof(fob_branch[1]) / sizeof(fob_branch[1][0]), fob_branch[1],},
        {FOB_NORMAL_STATUS,  sizeof(fob_branch[2]) / sizeof(fob_branch[2][0]), fob_branch[2],},
    };
    func_fob_para.fsm = fsm_create_with_state(fsm_fob_state, 
                sizeof(fsm_fob_state) / sizeof(fsm_fob_state[0]), 
                sizeof(fob_branch[0]) / sizeof(fob_branch[0][0]),
                FOB_NORMAL_STATUS);
    while(1){
        fsm_run(func_fob_para.fsm, Get_FobEvent, &func_fob_para, &func_fob_para, NULL);
        sleep(1);
    }

    fsm_release(func_fob_para.fsm);
    return 0;
}
#endif
