#include "headers.h"

void proclore(char* pid)
{
    char proc_path[256];
    FILE *status_file;
    char line[256];

    snprintf(proc_path, sizeof(proc_path), "/proc/%s/status", pid);

    status_file = fopen(proc_path, "r");
    if (status_file == NULL) {
        printf("No process running with pid %s\n",pid);
        return;
    }

    char process_status;
    char process_group[32];
    char vm_size[32];

    while (fgets(line, sizeof(line), status_file)) {
        if (strncmp(line, "State:", 6) == 0) {
            sscanf(line, "State:\t%c", &process_status);
        } else if (strncmp(line, "PPid:", 5) == 0) {
            sscanf(line, "PPid:\t%s", process_group);
        } else if (strncmp(line, "VmSize:", 7) == 0) {
            sscanf(line, "VmSize:\t%s", vm_size);
        }
    }

    fclose(status_file);

    snprintf(proc_path, sizeof(proc_path), "/proc/%s/exe", pid);
    char executable_path[256];
    ssize_t len = readlink(proc_path, executable_path, sizeof(executable_path) - 1);
    if (len != -1) {
        executable_path[len] = '\0';
    } else {
        strcpy(executable_path, "Unknown");
    }

    pid_t i_pid = atoi(pid);
    pid_t fg_pgid = tcgetpgrp(STDIN_FILENO);


    printf("PID: %s\n", pid);
    printf("Process Status: %c", process_status);
    if (getpgid(i_pid) == fg_pgid)
        printf("+\n");
    else
        printf("\n");

    printf("Process Group: %s\n", process_group);
    printf("Virtual Memory Size: %s\n", vm_size);
    printf("Executable Path: %s\n", executable_path);

}

