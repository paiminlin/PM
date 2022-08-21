/*
    From : https://github.com/paiminlin/PM
    From : https://blog.csdn.net/lpaim/article/details/114681720
    Author : PaiMin.lin
    Date : 2022.8.21
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LPM
#include "list.h"

#ifdef __cplusplus
extern "C"{
#endif

//#define DemoList_MAIN_DEBUG
#ifdef DemoList_MAIN_DEBUG

#define MAX_BUF         64

struct file_name_info
{
    int index;
    char name[MAX_BUF];
    struct list_head list;
};

int main(int argc, char *argv[]) {

    int i = 0;
    struct file_name_info * pfileinfo = NULL;
    struct file_name_info * ptmpfileinfo = NULL;

    LIST_HEAD(filehead);

    printf("*** insert the file list ***************************\n");
    pfileinfo = NULL;
    for(i = 0; i < 5; i++){
        pfileinfo = (struct file_name_info *)malloc(sizeof(struct file_name_info));
        pfileinfo->index = i + 1;
        snprintf(pfileinfo->name, sizeof(pfileinfo->name), "%d.txt", pfileinfo->index);
        list_add(&pfileinfo->list, &filehead);
    }

    printf("*** printf the file list ***************************\n");
    pfileinfo = NULL;
    list_for_each_entry(pfileinfo, &filehead, list){
        printf("file index : %d name : %s\n", pfileinfo->index, pfileinfo->name);
    }

    printf("*** change the 3 node ******************************\n");
    pfileinfo = NULL;
    list_for_each_entry(pfileinfo, &filehead, list){
        if(pfileinfo->index == 3){
            snprintf(pfileinfo->name, sizeof(pfileinfo->name), "%s.txt", "change");
            break;
        }
    }

    printf("*** printf the file list ***************************\n");
    pfileinfo = NULL;
    list_for_each_entry(pfileinfo, &filehead, list){
        printf("file index : %d name : %s\n", pfileinfo->index, pfileinfo->name);
    }

    printf("*** delete the 3 node ******************************\n");
    pfileinfo = NULL;
    ptmpfileinfo = NULL;
    list_for_each_entry_safe(pfileinfo, ptmpfileinfo, &filehead, list){
        if(pfileinfo->index == 3){
            list_del(&(pfileinfo->list));
            free(pfileinfo);
            break;
        }
    }

    printf("*** printf the file list ***************************\n");
    pfileinfo = NULL;
    list_for_each_entry(pfileinfo, &filehead, list){
        printf("file index : %d name : %s\n", pfileinfo->index, pfileinfo->name);
    } 

    printf("*** delete the file list ***************************\n");
    pfileinfo = NULL;
    ptmpfileinfo = NULL;
    list_for_each_entry_safe(pfileinfo, ptmpfileinfo, &filehead, list){
        list_del(&(pfileinfo->list));
        free(pfileinfo);
    }

    if(list_empty(&filehead))
        printf("the list is empty.\n");
    else
        printf("the list is not empty.\n");

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif
