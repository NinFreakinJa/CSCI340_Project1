#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>


struct process{
    int pid;
    int ppid;
    int vsize;
    struct process *parent;
    char name[];
};

int main(int argv,char *argc[]){
    DIR *directory;
    struct dirent *entry;
    int count;
}