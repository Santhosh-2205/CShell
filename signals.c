#include "headers.h"

void signal_usage_error()
{
    printf("%s-----------------------PING USAGE ERROR ---------------------%s\n",RED,RESET);
    printf("ping <pid> <signal_number>\n");
    printf("%s-----------------------PING USAGE ERROR ---------------------%s\n",RED,RESET);
}

int send_signal(char** args)
{
    char *endptr;
    pid_t pid = strtol(args[0],&endptr, 10);
    if (*endptr != '\0' && *endptr != '\n') {
        signal_usage_error();
        printf("pid should be integer but provided an non-integer(%s)\n",args[0]);
        return -1;
    }
    if(pid <= 0)
    {
        signal_usage_error();
        printf("pid should be +ve integer but provided an -ve integer(%s)\n",args[0]);
        return -1;
    }

    int signal_number = strtol(args[1],&endptr, 10);
    if (*endptr != '\0' && *endptr != '\n') {
        signal_usage_error();
        printf("signal_number should be integer but provided an non-integer(%s)\n",args[1]);
        return -1;
    }
    if(signal_number < 0)
    {
        signal_usage_error();
        printf("signal_number should be +ve integer but provided an -ve integer(%s)\n",args[1]);
        return -1;
    }
    signal_number %= 32;

    if (kill(pid, signal_number) == 0) {
        printf("Sent signal %d to process with pid %d\n",signal_number,pid);
    } else {
        perror("kill");
    }
    return 1;
}

void ctrlc_handler()
{
    if(current_foreground_pid == -1) // nothing is going on
    {
        printf("\n");
        prompt();
        fflush(stdout);
        return;
    }
    else
    {
        kill(current_foreground_pid,14);
    }
}

void ctrlz_handler()
{
    if (current_foreground_pid != -1) {
        printf("Moving process %d to the background...\n", current_foreground_pid);

        if (tcsetpgrp(STDIN_FILENO, getpgrp()) == -1) {
            perror("tcsetpgrp");
            exit(1);
        }

        background_processes[num_background_processes].pid = current_foreground_pid;
        background_processes[num_background_processes].finished = 0;
        strcpy(background_processes[num_background_processes].name,foreground_name);
        num_background_processes++;
            
        printf("Background process started with PID: %d\n", current_foreground_pid);

        current_foreground_pid = -1;
    }
}

void kill_childrens()
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
    for(int i = 0 ; i < n ; i++)
    {
        int status = process_status(pids[i]);
        if(status == -1) // killed
            continue;
        if (kill(pids[i], SIGTERM) != 0) {
            perror("Failed to send SIGTERM");
        }
    }
    printf("Killed all proceess spawned by shell\n");
}