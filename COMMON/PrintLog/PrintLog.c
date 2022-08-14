/*
    From : https://github.com/paiminlin/PM
    From : 
    Author : PaiMin.lin
    Date : 2022.8.14
*/

#include "PrintLog.h"

#ifdef __cplusplus
extern "C"{
#endif

#define NONE            "\e[0m"
#define BLACK           "\e[0;30m"
#define L_BLACK         "\e[1;30m"
#define RED             "\e[0;31m"
#define L_RED           "\e[1;31m"
#define GREEN           "\e[0;32m"
#define L_GREEN         "\e[1;32m"
#define BROWN           "\e[0;33m"
#define YELLOW          "\e[1;33m"
#define BLUE            "\e[0;34m"
#define L_BLUE          "\e[1;34m"
#define PURPLE          "\e[0;35m"
#define L_PURPLE        "\e[1;35m"
#define CYAN            "\e[0;36m"
#define L_CYAN          "\e[1;36m"
#define GRAY            "\e[0;37m"
#define WHITE           "\e[1;37m" 
#define BOLD            "\e[1m"
#define UNDERLINE       "\e[4m"
#define BLINK           "\e[5m"
#define REVERSE         "\e[7m"
#define HIDE            "\e[8m"
#define CLEAR           "\e[2J"
#define CLRLINE         "\r\e[K" //or "\e[1K\r"

static PrintLog_Level s_enPrintLogLevel = PrintLog_InvalidLevel;

void PrintLog_Init(PrintLog_Level enPrintLogLevel)
{
    s_enPrintLogLevel = enPrintLogLevel;
    return ;
}

void PrintLog_DeInit(void)
{
    s_enPrintLogLevel = PrintLog_InvalidLevel;
    return ;
}

void PrintLog_Bebug(void)
{
    if(s_enPrintLogLevel <= PrintLog_BebugLevel)
    {
        printf("[%s %s] %s: %s: %d :\n", __DATE__, __TIME__, __FILE__, __func__, __LINE__);
    }
    return ;
}

void PrintLog_Warn(void)
{
    if(s_enPrintLogLevel <= PrintLog_WarnLevel)
    {
        printf("[%s %s] %s: %s: %d :\n", __DATE__, __TIME__, __FILE__, __func__, __LINE__);
    }
    return ;
}

void PrintLog_Error(void)
{
    if(s_enPrintLogLevel <= PrintLog_ErrorLevel)
    {
        printf("[%s %s] %s: %s: %d :\n", __DATE__, __TIME__, __FILE__, __func__, __LINE__);
    }
    return ;
}

//#define PrintLog_MAIN_DEBUG
#ifdef PrintLog_MAIN_DEBUG

int main()
{
    PrintLog_Init(PrintLog_BebugLevel);
    PrintLog_Bebug();
    PrintLog_Warn();
    PrintLog_Error();
    PrintLog_DeInit();
    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

