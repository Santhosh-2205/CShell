#include "headers.h"


// Given pid return 0 if running, 1 if stopped , -1 if killed/doesn't exist
int process_status(pid_t pid)
{
    char status_path[256];
    snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);

    FILE *status_file = fopen(status_path, "r");
    if (status_file == NULL) {
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), status_file)) {
        if (strncmp(line, "State:", 6) == 0) {
            if (strstr(line, "R (running)"))
                return 0;
            else if (strstr(line, "T (stopped)"))
                return 1;
            else if (strstr(line, "X (dead)"))
                return -1;
        }
    }

    fclose(status_file);
}

// Prints all alive background processes spawned by shell in lexiographic order
void activities()
{
    int n = num_background_processes;
    char* names[n];
    pid_t pids[n];
    int finished[n];
    for(int i = 0 ; i < n ; i++)
    {
        names[i] = background_processes[i].name;
        pids[i] = background_processes[i].pid;
        finished[i] = background_processes[i].finished;
    } 
    bubblesort2(names,pids,finished,n);
    for(int i = 0 ; i < n ; i++)
    {
        int status = process_status(pids[i]);
        if(status == -1) // killed
            continue;
        printf("%d : %s - ",pids[i],names[i]);
        status == 1 ? printf("Stopped\n"): printf("Running\n");
    }
}