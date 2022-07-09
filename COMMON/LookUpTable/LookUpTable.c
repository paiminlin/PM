
#include "LookUpTable.h"

#ifdef __cplusplus
extern "C"{
#endif

int LookUpTable_Output(LookUpTable_Mode enLookUpTableMode, float Input, float * pOutput, 
                                const LookUpTable_Info * pstLookUpTableInfo, const int TableLength)
{
    float Output = 0;
    float MixInput = 0;
    float MaxInput = 0;
    float MixOutput = 0;
    float MaxOutput = 0;

    if(pOutput == NULL || pstLookUpTableInfo == NULL)
        return -1;

    if(Input <= pstLookUpTableInfo[0].Input)
    {
        Output = pstLookUpTableInfo[0].Output;
    }
    else if(Input >= pstLookUpTableInfo[TableLength - 1].Input)
    {
        Output = pstLookUpTableInfo[TableLength - 1].Output;
    }
    else
    {
        if(enLookUpTableMode == LookUpTable_PollingMode)
        {
            /*
                顺序查表
            */
            int Num = 0;
            for(Num = 1; Num < TableLength; Num ++)
            {
                if(Input < pstLookUpTableInfo[Num].Input)
                {
                    MixInput = pstLookUpTableInfo[Num - 1].Input;
                    MixOutput = pstLookUpTableInfo[Num - 1].Output;
                    MaxInput = pstLookUpTableInfo[Num].Input;
                    MaxOutput = pstLookUpTableInfo[Num].Output;
                    break;
                }
                else if(Input == pstLookUpTableInfo[Num].Input)
                {
                    * pOutput = pstLookUpTableInfo[Num].Output;
                    return 0;
                }
            }
        }
        else if(enLookUpTableMode == LookUpTable_DichotomyMode)
        {
            /*
                二分法查表
            */
            int Left = 0;
            int Right = TableLength - 1;
            int Mid = Left + (Right - Left) / 2;
            while(Left < Right)
            {
                if(Input < pstLookUpTableInfo[Mid].Input) {
                    Right = Mid;
                    if(Mid == (Left + 1)){
                        break;
                    }
                } else if(Input > pstLookUpTableInfo[Mid].Input) {
                    Left = Mid;
                    if(Mid == (Right - 1)){
                        break;
                    }
                } else if(Input == pstLookUpTableInfo[Mid].Input) {
                    * pOutput = pstLookUpTableInfo[Mid].Output;
                    return 0;
                }
                Mid = Left + (Right - Left) / 2;
            }
            MixInput = pstLookUpTableInfo[Left].Input;
            MixOutput = pstLookUpTableInfo[Left].Output;
            MaxInput = pstLookUpTableInfo[Right].Input;
            MaxOutput = pstLookUpTableInfo[Right].Output;
        }
        else
        {
            return -1;
        }

        /*
            换算公式(线性关系)
            (Output - MixOutput) / (Input - MixInput) = (MaxOutput - MixOutput) / (MaxInput - MixInput);
        */
        Output = (Input - MixInput) * ((MaxOutput - MixOutput) / (MaxInput - MixInput)) + MixOutput;
    }

    * pOutput = Output;
    return 0;
}

//#define LookUpTable_MAIN_DEBUG
#ifdef LookUpTable_MAIN_DEBUG

#include<time.h>

int main()
{
    const LookUpTable_Info stlookUptable[5] = {
        /* Input        Output*/
        { 1,            2.5},
        { 2,            5.0},
        { 3.6,          3.5},
        { 4.5,          7.0},
        { 5,            9.5},
    };

    int Num = 0;
    int TableLength = sizeof(stlookUptable)/sizeof(LookUpTable_Info);
    printf("Input\t\t Output\n");
    for(Num = 0; Num < TableLength; Num ++)
    {
        printf("%f\t %f\n", stlookUptable[Num].Input, stlookUptable[Num].Output);
    }
    printf("\n");

    float CLOCKSPERSEC = CLOCKS_PER_SEC;
    printf("CLOCKS_PER_SEC = %f\n", CLOCKSPERSEC);
    float CLKTCK = CLK_TCK;
    printf("CLK_TCK = %f\n\n", CLKTCK);

    clock_t t_start;
    t_start = clock();

    float Input = 0;
    float Output = 0;

    Input = 2.9;
    LookUpTable_Output(LookUpTable_PollingMode, Input, &Output, stlookUptable, TableLength);
    printf("PollingMode\n");
    printf("%f\t %f\n", Input, Output);
    LookUpTable_Output(LookUpTable_DichotomyMode, Input, &Output, stlookUptable, TableLength);
    printf("DichotomyMode\n");
    printf("%f\t %f\n\n", Input, Output);

    Input = 4.2;
    LookUpTable_Output(LookUpTable_PollingMode, Input, &Output, stlookUptable, TableLength);
    printf("PollingMode\n");
    printf("%f\t %f\n", Input, Output);
    LookUpTable_Output(LookUpTable_DichotomyMode, Input, &Output, stlookUptable, TableLength);
    printf("DichotomyMode\n");
    printf("%f\t %f\n\n", Input, Output);

    clock_t t_end;
    t_end = clock();
    printf("time: %f seconds\n",(float)(t_end-t_start)/CLOCKS_PER_SEC);

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

