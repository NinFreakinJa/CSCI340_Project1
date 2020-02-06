// CSCI340_P1.c
// Reid Foster, Ethan Guthrie
// 02/04/2020
// Provides the function of the program "pstree" on nix systems.

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helperFunctions.h"
#include "process.h"

int main(int argc, char const *argv[]){
    // Getting number of running processes.
    int processCount = getProcessCount();
    if(processCount == -1){
        return -1;
    }
    // Initializing array of process structures of size processCount.
    struct process runningProcesses[processCount];
    
    // Filling array with processes. This code block will be very similar
    // to that from getProcessCount()
    /*--------------------------------------------------------------------*/
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
    char dirName[100];
    // Parsing directory to find PIDs (any directory which is a number).
    currDirectory = readdir(dir);
    while(currDirectory != NULL){
        // Writing current directory to parsring variable dirName.
        sprintf(dirName, "%s", currDirectory->d_name);
        // Checking if dirName is a number (uses ASCII codes).
        if(dirName[0] >= 48 && dirName[0] <= 57){
            runningProcesses[counter] = makeProcess(atoi(dirName));
            counter++;
        }
        // Setting currDirectory to the next directory.
        currDirectory = readdir(dir);
    }

    // Printing tree.
    printTree(runningProcesses, processCount);

    return 0;
}
