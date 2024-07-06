#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pwd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <termios.h>
#include <ctype.h>

#include "prompt.h"
#include "warp.h"
#include <dirent.h>
#include "sort.h"
#include "list.h"
#include "peek.h"
#include "pastevents.h"
#include "seek.h"
#include "proclore.h"
#include "signals.h"
#include "iman.h"
#include "fgbg.h"
#include "neonate.h"
#include "activities.h"

#define MAX_DIR_LEN 1000
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

#define MAX_CMD_SIZE 4096
#define MAX_ARGS 10

#define MAX_BACKGROUND_PROCESSES 50

extern char invoked_dir[MAX_DIR_LEN];
extern char old_wd[MAX_DIR_LEN];

extern char prompt_arg[100];

extern pid_t current_foreground_pid;
extern char foreground_name[100] ;

typedef struct {
    pid_t pid;
    char name[100];
    int finished;
} BackgroundProcess;
extern BackgroundProcess background_processes[MAX_BACKGROUND_PROCESSES];
extern int num_background_processes;

int read_word(char* input, char* buffer, int pos);
void exec_cmd(char* input,history h);

#endif
