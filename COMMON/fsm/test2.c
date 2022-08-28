
#include "fsm.h"
#include <ctype.h>

enum states {
    OFF = 0,
    ACC = 1,
    ON  = 2,
};

enum event {
    SSB     = 0,
    STOP    = 1,
    INVALID = 2,
};

struct para {
    struct fsm_t *fsm;
    int c;
};

static int Get_SSBStatus(void *p)
{
    ((struct para *)p)->c = getchar();
    if (((struct para *)p)->c == 'S')
        return SSB;
    else if (((struct para *)p)->c == 'Q')
        return STOP;
    else
        return INVALID;
}

static void *ToOFF(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *ToACC(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *ToON(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    return NULL;
}
static void *ToStop(void *p)
{
    printf("%s-%d\n",__func__, __LINE__);
    fsm_stop(((struct para *)p)->fsm);
    return NULL;
}
#if 0
int main()
{
    struct para func_para;
    struct fsm_branch branch[][2] = {
        /* OFF */
        {
            /* SSB */
            {SSB, ACC, ToACC,},
            {STOP, OFF, ToStop,},
        },
        /* ACC */
        {
            /* SSB */
            {SSB, ON, ToON,},
            {STOP, ACC, NULL,},
        },
        /* ON */
        {
            /* SSB */
            {SSB, OFF, ToOFF,},
            {STOP, ON, NULL,},
        },
    };
    struct fsm_state fsm_state[] = {
        {OFF, sizeof(branch[0]) / sizeof(branch[0][0]), branch[0],},
        {ACC, sizeof(branch[1]) / sizeof(branch[1][0]), branch[1],},
        {ON,  sizeof(branch[2]) / sizeof(branch[2][0]), branch[2],},
    };
    func_para.fsm = fsm_create_with_state(fsm_state, 
                sizeof(fsm_state) / sizeof(fsm_state[0]), 
                sizeof(branch[0]) / sizeof(branch[0][0]),
                OFF);
    fsm_run(func_para.fsm, Get_SSBStatus, &func_para, &func_para, NULL);
    fsm_release(func_para.fsm);
    return 0;
}
#endif
