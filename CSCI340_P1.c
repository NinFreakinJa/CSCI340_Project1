#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

struct process{
    //field # in relation to stat file
    // %d field 1
    int pid;
    // %d field 4
    int ppid;
    // %lu field 23
    int vsize;
    //points to the first child
    struct process *children;
    //points to processes of the same parent
    struct process *siblings;
    //name
    // %s field 2
    char *comm[];
};
//make tree method
//recursive function starting at root (systemd pid=1)
//goes through list PIDs and when PPID is the same it is added and finds its own children until coming back and continuing

struct process makeTree(struct process root,int pidList[],int processCount){
    //base case
    if(processCount<=0){
        return NULL;
    }
    //open stat file for process
    char *filename=strcat("/proc/",itoa(root.pid));
    filename=strcat(filename,"/stat");
    FILE *statF=*fopen(filename,"r");
    char *statR;
    //read stat file
    while(fscanf(statF,"%s",temp)!=EOF){
        statR=strcat(statR,temp);
    }
    char *token=strtok(*statR," \n\t\r\v\f");
    int count=1;
    //split stat file fields and set appropriate variables
    while(token !=NULL && count<24){
        token=strtok(NULL," \n\t\r\v\f");
        count++;
        if(count==2){
            root.comm=token;
        }
        else if(count==4){
            root.ppid=itoa(token);
        }
        else if(count==23){
            root.vsize=itoa(token);
        }
    }
    //new process list with the root removed to avoid redundancy and infinite recursion
    int newpList[processCount-1];
    for(int i=0;i<processCount;i++){
        if(pidList[i]!=root.pid){
            newpList[i]=pidList[i];
        }   
    }
    //will go through and create linked list of child processes
    root.children=&addChildren(root,newpList,processCount-1);
    //close the file
    fclose(statF);
    return root;
}

//recursive process for adding children
struct process addChildren(struct process root, int pidList[], int processCount){
    //base case
    if(processCount<=0){
        return NULL;
    }
    //searches processes for ppid of root
    for(int i=0;i<processCount;i++){
        if(root.pid!=pidList[i]){
            //opens and reads stat file
            char *filename=strcat("/proc/",itoa(pidList[i]));
            filename=strcat(filename,"/stat");
            FILE *statF=*fopen(filename,"r");
            char *statR;
            while(fscanf(statF,"%s",temp)!=EOF){
                statR=strcat(statR,temp);
            }
            char *token=strtok(*statR," \n\t\r\v\f");
            int count=1;
            while(token !=NULL && count<5){
                token=strtok(NULL," \n\t\r\v\f");
                count++;
                //ppid field
                if(count==4){
                    //if ppid is the root
                    if(itoa(token)==root.pid){
                        //creates new root to continue recursion
                        struct process newRoot;
                        newRoot.pid=pidList[i];
                        //removes process from list to avoid redundancy
                        int newpList[processCount-1];
                        for(int i=0;i<processCount;i++){
                             if(pidList[i]!=root.pid){
                                newpList[i]=pidList[i];
                            }   
                        }
                        //creates linked list of siblings
                        newRoot.siblings=&addChildren(root,newpList,processCount-1);
                        //after linked list created the recursion continues for the tree
                        return makeTree(newRoot,pidList,processCount);
                    }
                }
            }
        }
    }
    //another base case where no children are found
    return NULL;
}


//print tree method
//print out using preorder traversal making sure to indent maybe put in increasing int by level to multiply tab by
struct process printTree(struct process root){
    
}

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
    int *pList=malloc(sizeof(int)*300);
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
                pList=realloc(pList,sizeof(int)*(size+50));
                size+=50;
            }
            pList[count-1]=atoi(*directoryName);
        }
    }
    //create tree
    struct process root;
    root.pid=1;
    root=makeTree(root,pList,count);
    //print tree

    closedir(drproc);
}