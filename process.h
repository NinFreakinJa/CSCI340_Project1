// process.h
// Reid Foster, Ethan Guthrie
// 02/04/2020
// Contains the process struct. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct process{
    // /proc/[PID]/stat attributes:
    char name[100];     // Attribute 2: %s
    int pid;            // Attribute 1: %d
    int ppid;           // Attribute 4: %d
    int vsize;          // Attribute 23:%lu
                        // vsize will be provided in kilobytes.
};