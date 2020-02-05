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
    char *comm;
};

struct process makeTree(struct process root,int pidList[],int processCount);
struct process addChildren(struct process croot, int cpidList[], int cprocessCount);

//recursive process for adding children
struct process addChildren(struct process croot, int cpidList[], int cprocessCount){
        //base case
        if(cprocessCount<=0){
            struct process endR;
            endR.pid=-1;
            return endR;
        }
        //searches processes for ppid of croot
        
        for(int i=0;i<cprocessCount;i++){
            if(croot.pid!=cpidList[i]){
                //opens and reads stat file
                int pidLength=0;
                int ltemp=cpidList[i];
                while(ltemp!=0){
                    ltemp=ltemp/10;
                    pidLength++;
                }
                char *cfilename=malloc(sizeof(char)*(strlen("/proc//stat")+pidLength));
                sprintf(cfilename,"/proc/%d/stat",cpidList[i]);
                //char *cfilename=strcat("/proc/",itoa(cpidList[i]));
                //cfilename=strcat(cfilename,"/stat");
                
                FILE *cstatF=fopen(cfilename,"r");
                
                /*char cstatR[5200];
                int cfileCount=0;
                while(feof(cstatF)){
                    cstatR[cfileCount++]=fgetc(cstatF);
                }*/
                //char *ctemp=malloc(sizeof(char)*5200);
                //while(fscanf(cstatF,"%s",ctemp)!=EOF){
                  //  printf("3\n");
                    //cstatR=strcat(cstatR," ");
                    //cstatR=strcat(cstatR,ctemp);
                    //printf("%s",cstatR);
                //}
                /*long clength;
                char *cbuffer=0;
                if(cstatF){
                    fseek(cstatF,0L,SEEK_END);
                    clength=ftell(cstatF);
                    fseek(cstatF,0L,SEEK_SET);
                    cbuffer=(char*)calloc(clength,sizeof(char));
                    if(cbuffer){
                        fread(cbuffer,sizeof(char),clength,cstatF);
                    }
                    fclose(cstatF);
                }*/

                char line[300];
                fgets(line,300,cstatF);
                fclose(cstatF);
                char *ctoken=strtok(line," \n\t\r\v\f");
                    //char *ctoken=strtok(cstatR," \n\t\r\v\f");
                    int ccount=1;
                    while(ctoken !=NULL && ccount<5){
                        ctoken=strtok(NULL," \n\t\r\v\f");
                        ccount++;
                        //ppid field
                        if(ccount==4){
                            //if ppid is the croot
                            if(atoi(ctoken)==croot.pid){
                                 //creates new croot to continue recursion
                                struct process newRoot;
                                newRoot.pid=cpidList[i];
                                //removes process from list to avoid redundancy
                                int cnewpList[cprocessCount-1];
                                for(int i=0;i<cprocessCount;i++){
                                    if(cpidList[i]!=croot.pid){
                                    cnewpList[i]=cpidList[i];
                                }   
                            }
                            //creates linked list of siblings
                            struct process nRootChild=addChildren(croot,cnewpList,cprocessCount-1);
                            newRoot.siblings=&nRootChild;
                            //after linked list created the recursion continues for the tree
                            return makeTree(newRoot,cpidList,cprocessCount);
                        }
                    }
                }
            }
        }
        //another base case where no children are found
        struct process endR;
        endR.pid=-1;
        return endR;
    }


//make tree method
//recursive function starting at root (systemd pid=1)
//goes through list PIDs and when PPID is the same it is added and finds its own children until coming back and continuing


struct process makeTree(struct process root,int pidList[],int processCount){
    //base case
    if(processCount<=0){
        struct process endR;
        endR.pid=-1;
        return endR;
    }
    //open stat file for process
    int pidLength=0;
    int ltemp=root.pid;
    while(ltemp!=0){
        ltemp=ltemp/10;
        pidLength++;
    }
    char *filename=malloc(sizeof(char)*(strlen("/proc//stat")+pidLength));
    sprintf(filename,"/proc/%d/stat",root.pid);
    //char *filename=strcat("/proc/",itoa(root.pid));
    //filename=strcat(filename,"/stat");
    
    FILE *statF=fopen(filename,"r");
    
    //char *statR;
    //read stat file
    //char statR[5200];
    //while(fscanf(statF,"%s",temp)!=EOF){
     //   printf("1\n");
     //   statR=strcat(statR," ");
     //   statR=strcat(statR,temp);
     //   printf("%s",statR);
   //}
    /*int fileCount=0;
    while(feof(statF)){
        statR[fileCount++]=fgetc(statF);
    }*/
    /*long length;
    char *buffer=0;
    if(statF){
        fseek(statF,0L,SEEK_END);
        length=ftell(statF);
        fseek(statF,0L,SEEK_SET);
        buffer=(char*)calloc(length,sizeof(char));
        if(buffer){
            fread(buffer,sizeof(char),length,statF);
        }
        fclose(statF);
    }*/

    char line[300];
    fgets(line,300,statF);
    fclose(statF);
    char *token=strtok(line," \n\t\r\v\f");
        int count=1;
        //split stat file fields and set appropriate variables
        while(token !=NULL && count<24){
            token=strtok(NULL," \n\t\r\v\f");
            count++;
            if(count==2){
                root.comm=token;
            }
            else if(count==4){
                root.ppid=atoi(token);
            }
            else if(count==23){
                root.vsize=atoi(token);
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
    struct process rootChild;
    rootChild=addChildren(root,newpList,processCount-1);
    root.children=&rootChild;
    //close the file
    return root;
}




//print tree method
//print out using preorder traversal making sure to indent maybe put in increasing int by level to multiply tab by
void printTree(struct process root,int level){
    if(root.pid==-1){
        return;
    }
    for(int i=0;i<level;i++){
        printf("    ");
    }
    printf("(%d) %s, %d kb\n",root.pid,root.comm,root.vsize);
    printTree(*root.children,level+1);
    if(root.pid!=1){
        printTree(*root.siblings,level);
    }   
}

int main(int argv,char *argc[]){
    //proc directory
    DIR *drproc=opendir("/proc");
    struct dirent *entry;
    int count=0;
    if(drproc==NULL){
        printf("Could not open proc directory\n");
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
                size+=50;
                pList=realloc(pList,sizeof(int)*(size));
            }
            pList[count-1]=atoi(directoryName);
        }
    }
    //create tree
    struct process root;
    root.pid=1;
    struct process endR;
    endR.pid=-1;
    root.siblings=&endR;
    root=makeTree(root,pList,count);
    //print tree
    printTree(root,0);

    closedir(drproc);
}