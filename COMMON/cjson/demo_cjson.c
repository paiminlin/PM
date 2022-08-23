/*
    From : https://github.com/paiminlin/PM
    From : https://blog.csdn.net/lpaim/article/details/115499334
    Author : PaiMin.lin
    Date : 2022.8.23
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cJSON.h"

#ifdef __cplusplus
extern "C"{
#endif

//#define Democjson_MAIN_DEBUG
#ifdef Democjson_MAIN_DEBUG

void doit(char * text)
{
    int i = 0, j = 0;
    char * out = NULL;
    cJSON * json = NULL;

    json = cJSON_Parse(text);
    if (!json) {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else{
        out = cJSON_Print(json);
        printf("%s\n\n", out);
        free(out);
        out = NULL;

        cJSON * json_province = cJSON_GetArrayItem(json, 0);
        out = cJSON_Print(json_province);
        printf("%s\n", out);
        free(out);
        printf("name = %s\n\n", cJSON_GetObjectItem(json, "name")->valuestring);

        cJSON * json_area = cJSON_GetArrayItem(json, 1);
        out = cJSON_Print(json_area);
        printf("%s\n", out);
        free(out);

        for(i = 0; i < cJSON_GetArraySize(json_area); i ++){

            cJSON * json_city = cJSON_GetArrayItem(json_area, i);
            if(NULL == json_city){
                continue;
            }
            out = cJSON_Print(json_city);
            printf("%s\n", out);
            free(out);

            for(j = 0; j < cJSON_GetArraySize(json_city); j ++ ){
                cJSON * Citylist = cJSON_GetArrayItem(json_city, j);
                if(NULL == Citylist){
                    continue;
                }
                printf("Citylist[%d] : %s\n", j, Citylist->valuestring);
            }
        }

        cJSON_Delete(json);
    }
}

void dofile(char * filename)
{
    FILE * file = NULL;
    long len = 0;
    char * data = NULL;

    file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);

    len = ftell(file);

    fseek(file, 0, SEEK_SET);

    data=(char*)malloc(len + 1);

    fread(data, 1, len, file);

    fclose(file);

    doit(data);

    free(data);

    return ;
}

int main(){

    dofile("./json");

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif
