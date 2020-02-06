// process.h
// Reid Foster, Ethan Guthrie
// 02/04/2020
// Header file for helperFunctions.c

int getProcessCount(void);
struct process makeProcess(int pid);
void printTree(struct process *runningProcesses, int size);
void printTreeHelper(struct process *runningProcesses, struct process current, char *spaces, int size);