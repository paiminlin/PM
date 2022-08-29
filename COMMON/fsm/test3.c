#if 0

#include "fsm.h"
#include <time.h>
#include <ctype.h>

enum states {
    FINDFOB_STATUS  = 0,
    WAITFOB_STATUS  = 1,
    NORMAL_STATUS   = 2,
    INVALID_STATUS  = 3,
};

enum event {
    TOFINDFOB_EVENT = 0,
    TOWAITFOB_EVENT = 1,
    TONORMAL_EVENT  = 2,
    INVALID_EVENT   = 3,
};

struct para {
    struct fsm_t *fsm;
    int c;
};

static int s_enEvent = INVALID_EVENT;
static void Set_EventStatus(int event)
{
    s_enEvent = event;
}
static int Get_EventStatus(void *p)
{
    int event = s_enEvent;
    s_enEvent = INVALID_EVENT;
    return event;
}

static void *ToFINDFOB_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    Set_EventStatus(WAITFOB_STATUS);
    return NULL;
}
static void *InFINDFOB_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *ToWAITFOB_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *InWAITFOB_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    time_t seconds = time((time_t *)NULL);
    struct tm * gmt;
    gmt = localtime(&seconds);
    if(gmt->tm_sec%12 == 0){
        Set_EventStatus(TONORMAL_EVENT);
    }
    return NULL;
}
static void *ToNORMAL_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *InNORMAL_STATUS(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}

int main()
{
    struct para func_para;
    struct fsm_branch branch[][4] = {
        /* FINDFOB_STATUS */
        {
            {TOWAITFOB_EVENT, WAITFOB_STATUS, NULL, NULL},
        },
        /* WAITFOB_STATUS */
        {
            {TOFINDFOB_EVENT, WAITFOB_STATUS, InWAITFOB_STATUS, NULL},
            {TOWAITFOB_EVENT, WAITFOB_STATUS, InWAITFOB_STATUS, NULL},
            {TONORMAL_EVENT, NORMAL_STATUS, NULL, NULL},
            {INVALID_EVENT, WAITFOB_STATUS, InWAITFOB_STATUS, NULL},
        },
        /* NORMAL_STATUS */
        {
            {TOFINDFOB_EVENT, FINDFOB_STATUS, ToFINDFOB_STATUS, NULL},
        },
    };
    struct fsm_state fsm_state[] = {
        {FINDFOB_STATUS, sizeof(branch[0]) / sizeof(branch[0][0]), branch[0],},
        {WAITFOB_STATUS, sizeof(branch[1]) / sizeof(branch[1][0]), branch[1],},
        {NORMAL_STATUS,  sizeof(branch[2]) / sizeof(branch[2][0]), branch[2],},
    };
    func_para.fsm = fsm_create_with_state(fsm_state, 
                sizeof(fsm_state) / sizeof(fsm_state[0]), 
                sizeof(branch[0]) / sizeof(branch[0][0]),
                NORMAL_STATUS);
    while(1){
        time_t seconds = time((time_t *)NULL);
        struct tm * gmt;
        gmt = localtime(&seconds);
        if(gmt->tm_sec%15 == 0){
            Set_EventStatus(TOFINDFOB_EVENT);
        }

        fsm_run(func_para.fsm, Get_EventStatus, &func_para, &func_para, NULL);
        sleep(1);
    }
    fsm_release(func_para.fsm);
    return 0;
}
#endif
