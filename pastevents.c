//JGD

#include "headers.h"

struct hist
{
    int start;
    int end;
    int size;
    char** cmds;
};

history init_history()
{
    history h = (history)malloc(sizeof(struct hist));
    h->start = 0; // index of first cmd
    h->end = -1;  // index of last cmd
    h->size = 0;
    h->cmds = (char**)malloc(sizeof(char*)*MAX_CMDS);
    for(int i = 0 ; i < MAX_CMDS ; i++)
        h->cmds[i] = (char*)malloc(sizeof(char)*MAX_CMD_SIZE);
    return h;
}

void add_cmd(history h,char* cmd)
{
    if(h->end != -1 && strcmp(h->cmds[h->end],cmd) == 0)
        return;

    if(cmd[0] == '\0' )
        return;

    int flag = 0;
    for(int i = 0 ; !flag && i < strlen(cmd) ; i++)
    {
        if(cmd[i] != '\t' && cmd[i] != '\n' && cmd[i] != ' ')
            flag = 1;
    }
    if(!flag) return;

    if(h->size == MAX_CMDS)
    {
        h->start++;
        h->start %= MAX_CMDS;
        h->size--;
    }

    h->size++;
    h->end++;
    h->end %= MAX_CMDS;
    strcpy(h->cmds[h->end],cmd);
    return;
}

void print_history(history h)
{
    printf("%d %d %d\n",h->start,h->end,h->size);
    if(h->size == 0) return;
    if(h->start <= h->end)
    {
        for(int i = h->start ; i <= h->end ; i++)
            printf("%s",h->cmds[i]);
    }
    else
    {
        for(int i = h->start ; i < MAX_CMDS ; i++)
            printf("%s",h->cmds[i]);
        for(int i = 0 ; i <= h->end ; i++)
            printf("%s",h->cmds[i]);
    }
}

void purge(history h)
{
    h->start = 0;
    h->end = 0;
    h->size = 0;
    for(int i = 0 ; i < MAX_CMDS ; i++)
        h->cmds[i][0] = '\0';
    return;
}

void pastevents_usage()
{
    printf("%s------------------PASTEVENTS USAGE ERROR -----------------%s\n",RED,RESET);
    printf("pastevents           --> prints past commands (max upto 15)\n");
    printf("pastevents purge     --> clears previous commands\n");
    printf("pastevents execute i --> execute ith last command\n");
    printf("%s------------------PASTEVENTS USAGE ERROR -----------------%s\n",RED,RESET);
}

int invoke_pastevents(char* args[10],history h)
{
    if(args[1] == NULL)
    {
        print_history(h);
    }
    else if(strcmp(args[1],"purge") == 0)
    {
        if(args[2] != NULL) pastevents_usage();
        purge(h);
    }
    else if(strcmp(args[1],"execute") == 0)
    {
        int num = atoi(args[2]);
        if(args[3] != NULL) pastevents_usage();
        if(h->size < num)
        {
            if(h->size == 0)
            printf("No commands in history till now\n");
            else
            printf("Number of commands in history is only %d\n",h->size);
            return 0;
        }
        num --;
        num = (h->end+MAX_CMDS-num)%MAX_CMDS;
        //printf("%s\n",h->cmds[num]);
        exec_cmd(h->cmds[num],h);
    }
    else pastevents_usage();
    return 0;
}

void get_history(history h)
{
    int ifile = open("history.txt", O_RDONLY);

    char buffer[MAX_CMD_SIZE];
    ssize_t bytesRead;
    char str[MAX_CMD_SIZE];
    int strIndex = 0;

    while ((bytesRead = read(ifile, buffer, MAX_CMD_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n') {
                str[strIndex] = '\n';
                str[strIndex+1] = '\0';
                add_cmd(h,str);
                strIndex = 0;
            } else {
                str[strIndex++] = buffer[i];
            }
        }
    }
    close(ifile);
}

void save_history(history h)
{
    char buffer[4096] = {};
    sprintf(buffer, "%s/history.txt", invoked_dir);
    FILE* f = fopen(buffer,"w");
    //int f = open(buffer,O_WRONLY);

    if(h->size == 0) return;
    if(h->start <= h->end)
    {
        for(int i = h->start ; i <= h->end ; i++)
        {
            int len = strlen(h->cmds[i]);
            fprintf(f,"%s",h->cmds[i]);
            //write(f,h->cmds[i],strlen(h->cmds[i]));
        }    
    }
    else
    {
        for(int i = h->start ; i < MAX_CMDS ; i++)
        {
            fprintf(f,"%s",h->cmds[i]);
            //write(f,h->cmds[i],strlen(h->cmds[i]));
        }   
        for(int i = 0 ; i <= h->end ; i++)
        {
            fprintf(f,"%s",h->cmds[i]);
           // write(f,h->cmds[i],strlen(h->cmds[i]));
        }
    }
    fclose(f);
    //close(f);

}


