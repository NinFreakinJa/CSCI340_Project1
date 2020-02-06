// process.h
// Reid Foster, Ethan Guthrie
// 02/04/2020
// Includes all functions to assist in the function of the main CSCI340_P1
// function.

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "helperFunctions.h"

// Returns the number of running processes.
int getProcessCount(void){
    // Initializing empty pointer to hold directory entries.
    struct dirent *currDirectory;
    // Getting DIR pointer to proc directory.
    DIR *dir = opendir("/proc");
    // Checking to make sure directory is not empty. If it is empty, -1 is
    // returned.
    if(dir == NULL){
        printf("Unable to read directory: /proc");
        return -1;
    }
    // Initializing variables for parsing directories.
    int counter = 0;
    char dirName[255];
    // Parsing directory to find PIDs (any directory which is a number).
    currDirectory = readdir(dir);
    while(currDirectory != NULL){
        // Writing current directory to parsring variable dirName.
        sprintf(dirName, "%s", currDirectory->d_name);
        // Checking if dirName is a number (uses ASCII codes).
        if(dirName[0] >= 48 && dirName[0] <= 57){
            counter++;
        }
        // Setting currDirectory to the next directory.
        currDirectory = readdir(dir);
    }
    return counter;
}

// Returns a process struct given the PID of the process.
struct process makeProcess(int pid){
    // Initializing an empty process struct and assigning PID.
    struct process newProcess;
    newProcess.pid = pid;
    // Initializing filename variable and assigning correct value based on
    // provided PID.
    char filename[20];
    sprintf(filename, "/proc/%d/stat", pid);
    // Opening stat file for process with provided PID.
    FILE *stat = fopen(filename, "r");
    // Checking to make sure file is not empty. If it is empty, an empty
    // process struct will be returned.
    if(stat == NULL){
        printf("Cannot open file: %s", filename);
        newProcess.ppid = -1;
        return newProcess;
    }
    // Initializing variables for parsing file.
    int counter = 0;
    int spaces = 0;
    char ppid[5];
    char vsize[15];
    bool writeName = false;
    // Parsing file for required attributes (Name (Comm), PID, PPID, and
    // VSize).
    char c = fgetc(stat);
    while(c != EOF){
        // Checking if character is any of certain "important" values.
        if(c == '('){
            writeName = true;
        }else if(c == ')'){
            writeName = false;
        }else if(c == ' ' && !writeName){
            spaces++;
            counter = 0;
        }
        // Getting next character.
        c = fgetc(stat);
        // Writing character to variable if necessary.
        if(writeName && c != ')'){
            newProcess.name[counter] = c;
            counter++;
        }else if(spaces == 3 && c != ' '){
            ppid[counter] = c;
            counter++;
        }else if(spaces == 22 && c != ' '){
            vsize[counter] = c;
            counter++;
        }
    }
    fclose(stat);
    // Converting PPID and VSize strings to integers.
    newProcess.ppid = atoi(ppid);
    newProcess.vsize = (atoi(vsize) / 1000);    // Also converts to kilobytes.
    // Checking for parsing error and returning struct.
    if(newProcess.name == NULL){
        printf("Cannot parse file: %s", filename);
        newProcess.ppid = -1;
        return newProcess;
    }else{
        return newProcess;
    }
}

// Prints the entire tree in a format similar to that of pstree.
void printTree(struct process *runningProcesses, int size){
    for(int i = 0; i < size; i++){
        if(runningProcesses[i].ppid == 0){
            printTreeHelper(runningProcesses, runningProcesses[i], "", size);
        }
    }
    return;
}

// Assists printTree function.
void printTreeHelper(struct process *runningProcesses, struct process current, char *spaces, int size){
    // Prints current process information.
    printf("%s(%d) %s, %d kb\n", spaces, current.pid, current.name, current.vsize);
    // Creates nextSpaces string with two additional spaces for child processes.
    char nextSpaces[50];
    sprintf(nextSpaces, "%s     ", spaces);
    // Searching for child processes.
    for(int i = 0; i < size; i++){
        if(runningProcesses[i].ppid == current.pid){
            printTreeHelper(runningProcesses, runningProcesses[i], nextSpaces, size);
        }
    }
}