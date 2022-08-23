/*
    From : https://github.com/paiminlin/PM
    From : https://blog.csdn.net/lpaim/article/details/110148447
    Author : PaiMin.lin
    Date : 2022.8.23
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dictionary.h"
#include "iniparser.h"

#ifdef __cplusplus
extern "C"{
#endif

//#define Demoini_MAIN_DEBUG
#ifdef Demoini_MAIN_DEBUG

#define FILENAME        "./test.ini"

void create_example_ini_file()
{
    FILE * ini;

    if ((ini=fopen(FILENAME, "w"))==NULL) {
        fprintf(stderr, "iniparser: cannot create %s\n", FILENAME);
        return ;
    }

    fprintf(ini,
    "#\n"
    "# This is an example of ini file\n"
    "#\n"
    "\n"
    "[Pizza]\n"
    "\n"
    "Ham       = yes ;\n"
    "Mushrooms = TRUE ;\n"
    "Capres    = 0 ;\n"
    "Cheese    = Non ;\n"
    "\n"
    "\n"
    "[Wine]\n"
    "\n"
    "Grape     = Cabernet Sauvignon ;\n"
    "Year      = 1989 ;\n"
    "Country   = Spain ;\n"
    "Alcohol   = 12.5  ;\n"
    "\n");
    fclose(ini);
}

int main(){

    if(access(FILENAME, F_OK | W_OK | R_OK) < 0)
    {
        // create a new ini file
        create_example_ini_file();
    }

    dictionary * ini;
    int b ;
    int i ;
    double d ;
    const char * s;

    ini = iniparser_load(FILENAME);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", FILENAME);
        return -1 ;
    }

    printf("****************************\n");

    /* Get pizza attributes */
    printf("Pizza:\n");

    b = iniparser_getboolean(ini, "pizza:ham", -1);
    printf("Ham:       [%d]\n", b);

    b = iniparser_getboolean(ini, "pizza:mushrooms", -1);
    printf("Mushrooms: [%d]\n", b);

    b = iniparser_getboolean(ini, "pizza:capres", -1);
    printf("Capres:    [%d]\n", b);

    b = iniparser_getboolean(ini, "pizza:cheese", -1);
    printf("Cheese:    [%d]\n", b);

    /* Get wine attributes */
    printf("Wine:\n");

    s = iniparser_getstring(ini, "wine:grape", NULL);
    printf("Grape:     [%s]\n", s ? s : "UNDEF");

    i = iniparser_getint(ini, "wine:year", -1);
    printf("Year:      [%d]\n", i);

    s = iniparser_getstring(ini, "wine:country", NULL);
    printf("Country:   [%s]\n", s ? s : "UNDEF");

    d = iniparser_getdouble(ini, "wine:alcohol", -1.0);
    printf("Alcohol:   [%g]\n", d);

    printf("****************************\n");

    char buf[8];
    memset(buf, 0x0, sizeof(buf));
    snprintf(buf, 8, "%d", 2020);
    iniparser_set(ini, "wine:year", buf);

    iniparser_set(ini, "wine:country", "LuFeng");

    FILE *fp = fopen(FILENAME, "w");
    if (NULL != fp)
    {
        iniparser_dump_ini(ini, fp);
        fclose(fp);
    }

    iniparser_dump(ini, stderr);

    printf("****************************\n");

    iniparser_freedict(ini);

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

