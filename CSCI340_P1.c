#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>


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
    int count;
    if(drproc==NULL){

    }
}