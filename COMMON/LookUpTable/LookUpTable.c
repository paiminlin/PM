
#include "LookUpTable.h"

#ifdef __cplusplus
extern "C"{
#endif

float LookUpTable_Output(float Input, const LookUpTable_Info * pstLookUpTableInfo, const int TableLength)
{
    float Output = 0;
    float MixInput = 0;
    float MaxInput = 0;
    float MixOutput = 0;
    float MaxOutput = 0;

    if(pstLookUpTableInfo == NULL)
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

#ifdef POLLING_LOOKUPTABLE
        /*
            顺序查表
        */
        int Num = 0;
        for(Num = 1; Num < TableLength; Num ++)
        {
            if(Input <= pstLookUpTableInfo[Num].Input)
            {
                MixInput = pstLookUpTableInfo[Num - 1].Input;
                MixOutput = pstLookUpTableInfo[Num - 1].Output;
                MaxInput = pstLookUpTableInfo[Num].Input;
                MaxOutput = pstLookUpTableInfo[Num].Output;
                break;
            }
        }
#elif DICHOTOMY_LOOKUPTABLE
        /*
            二分法查表
        */
        int Left = 0;
        int Right = TableLength - 1;
        int Mid = Left + (Right - Left) / 2;
        while(Left < Right)
        {
            if((Mid == (Left + 1)) || (Mid == (Right - 1))){
                break;
            }else if( Input < pstLookUpTableInfo[Mid].Input) {
                Right = Mid;
            } else if(Input > pstLookUpTableInfo[Mid].Input) {
                Left = Mid;
            }else if(Input = pstLookUpTableInfo[Mid].Input) {
                Right = Mid;
                break;
            }
            Mid = Left + (Right - Left) / 2;
        }
        MixInput = pstLookUpTableInfo[Left].Input;
        MixOutput = pstLookUpTableInfo[Left].Output;
        MaxInput = pstLookUpTableInfo[Right].Input;
        MaxOutput = pstLookUpTableInfo[Right].Output;
#endif

        /*
            换算公式(线性关系)
            (Output - MixOutput) / (Input - MixInput) = (MaxOutput - MixOutput) / (MaxInput - MixInput);
        */
        Output = (Input - MixInput) * ((MaxOutput - MixOutput) / (MaxInput - MixInput)) + MixOutput;
    }

    return Output;
}

//#define LookUpTable_MAIN_DEBUG
#ifdef LookUpTable_MAIN_DEBUG
int main()
{
    LookUpTable_Info stlookUptable[5] = {
        /* Input        Output*/
        { 1,            2.5},
        { 2,            3.5},
        { 3,            4.5},
        { 4,            5.5},
        { 5,            6.5},
    };

    int Num = 0;
    int TableLength = sizeof(stlookUptable)/sizeof(LookUpTable_Info);
    printf("Input\t\t Output\n");
    for(Num = 0; Num < TableLength; Num ++)
    {
        printf("%f\t %f\n", stlookUptable[Num].Input, stlookUptable[Num].Output);
    }
    printf("\n");

    float Input = 1.5;
    float Output = LookUpTable_Output(Input, stlookUptable, TableLength);
    printf("Input\t\t Output\n");
    printf("%f\t %f\n", Input, Output);

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

