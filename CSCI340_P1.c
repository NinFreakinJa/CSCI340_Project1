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
//make tree method
//recursive function starting at root (systemd pid=1)
//goes through list PIDs and when PPID is the same it is added and finds its own children until coming back and continuing
//print tree method
//print out using preorder traversal making sure to indent maybe put in increasing int by level to multiply tab by

int main(int argv,char *argc[]){
    //proc directory
    DIR *drproc=opendir("/proc");
    struct dirent *entry;
    int count=0;
    if(drproc==NULL){
        printf("Could not open proc directory");
        return 0;
    }
    int size=300;
    //linked list of processes
    //probably need a better type of list for later making the tree
    //replace with make tree
    //change to make a list of PIDs instead
    struct process *pList=malloc(sizeof(struct process)*300);
    while((entry=readdir(drproc))!=NULL){
        char *directoryName=entry->d_name;
        //checks if directory name is a number
        if(*directoryName<'0'||*directoryName>'9'){
            continue;
        }
        else{
            count++;
            //increases list size if it gets too big
            if(count>=size){
                pList=realloc(pList,sizeof(struct process)*(size+50));
                size+=50;
            }
            //just checking if working
            //REMOVE
            printf("%s\n",directoryName);

            //Parse through /proc/<pid>/stat file and get relevent info
            //put info into struct process and into pList
        }
    }
    //create tree

    //REMOVE
    printf("%d",count);
    
    closedir(drproc);
}
