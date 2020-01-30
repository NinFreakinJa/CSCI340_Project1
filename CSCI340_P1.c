#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>


struct process{
    //field # in relation to stat file
    // %d field 1
    int pid;
    // %d field 4
    int ppid;
    // %lu field 23
    int vsize;
    struct process *parent;
    //name
    // %s field 2
    char comm[];
};

int main(int argv,char *argc[]){
    //proc directory
    DIR *drproc=opendir("./proc");
    struct dirent *entry;
    int count=0;
    if(drproc==NULL){
        printf("Could not open proc directory");
        return 0;
    }
    int size=300;
    struct process *pList=malloc(sizeof(struct process)*300);
    while((entry=readdir(drproc))!=NULL){
        char *directoryName=entry->d_name;
        if(*directoryName<'0'||*directoryName>'9'){
            continue;
        }
        else{
            count++;
            if(count>=size){
                pList=realloc(pList,sizeof(struct process)*(size+50));
                size+=50;
            }
            printf("%s",directoryName);
        }
    }
    
    closedir(drproc);
}