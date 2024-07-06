#include "headers.h"

// Given pid makes it fg, if not running makes it's running, wait till it stops and give back control to shell
int fg(int pid) {
    current_foreground_pid = pid;
    if (tcsetpgrp(STDIN_FILENO, getpgid(pid)) == -1) {
        perror("tcsetpgrp");
        return 0;
    }

    if (kill(pid, SIGCONT) == -1) {
        perror("kill");
        return 0;
    }

    int status;
    if (waitpid(pid, &status, WUNTRACED) == -1) {
        perror("waitpid");
        return 0;
    }
    current_foreground_pid = -1;
    if (tcsetpgrp(STDIN_FILENO, getpgrp()) == -1) {
        perror("tcsetpgrp");
        return 0;
    }
}

// Makes process with pid run in background
int bg(int pid)
{
    if(kill(pid, SIGCONT) == -1) {
        perror("kill");
        return 0;
    } 
}

int invoke_fg(char** args)
{
    if(args[1] == NULL)
    {
        fg_usage();
        printf("PLease provide pid\n");
        return 1;
    }
    char *endptr;
    int pid = strtol(args[1],&endptr, 10);
    if (*endptr != '\0' && *endptr != '\n') {
        fg_usage();
        printf("Pid must be integer\n");
        return 1;
    }
    if(pid < 0)
    {
        fg_usage();
        printf("Pid must be +ve integer\n");
        return 1;
    }
    fg(pid);
}

void fg_usage()
{
    printf("%s-------------FG USAGE ERROR -------------%s\n",RED,RESET);
    printf("fg <pid>\n");
    printf("%s-------------FG USAGE ERROR -------------%s\n",RED,RESET);
}

int invoke_bg(char** args)
{
    if(args[1] == NULL)
    {
        bg_usage();
        printf("PLease provide pid\n");
        return 1;
    }
    char *endptr;
    int pid = strtol(args[1],&endptr, 10);
    if (*endptr != '\0' && *endptr != '\n') {
        bg_usage();
        printf("Pid must be integer\n");
        return 1;
    }
    if(pid < 0)
    {
        bg_usage();
        printf("Pid must be +ve integer\n");
        return 1;
    }
    bg(pid);
}

void bg_usage()
{
    printf("%s-------------BG USAGE ERROR -------------%s\n",RED,RESET);
    printf("bg <pid>\n");
    printf("%s-------------BG USAGE ERROR -------------%s\n",RED,RESET);
}

