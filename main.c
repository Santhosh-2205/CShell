#include "headers.h"

pid_t current_foreground_pid = -1;
char foreground_name[100] ;

BackgroundProcess background_processes[MAX_BACKGROUND_PROCESSES];
int num_background_processes = 0;


// Iterates through all the background processes spawned by shell
// If any of the process is found to be finished with execution prints it's pid
void check_background_processes() {
    int i;
    for (i = 0; i < num_background_processes; i++) {
        if (!background_processes[i].finished) {
            pid_t pid = waitpid(background_processes[i].pid, NULL, WNOHANG);
            if (pid > 0) {
                printf("Background process with PID %d has finished.\n", pid);
                background_processes[i].finished = 1;
            }
        }
    }
}

char invoked_dir[MAX_DIR_LEN];
char old_wd[MAX_DIR_LEN];
char prompt_arg[100];
 
// Tokenize input string by tab and spaces and stores in args
int tokenize(char* input,char* args[MAX_ARGS])
{
    input[strlen(input)-1] = '\0';
    int num_args = 0;
    char* token = strtok(input,"\t ");
    while(token != NULL)
    {
        args[num_args++] = token;
        token = strtok(NULL,"\t ");
    }
    args[num_args] = NULL;
    return num_args;
}


// Given a command runs it in background via exevp
void run_back(char** args,history h)
{
    pid_t pid = fork();
    if(pid == 0)
    {
        execvp(args[0], args);
        perror("Exec error");
    }
    else
    {
        if (num_background_processes < MAX_BACKGROUND_PROCESSES) 
        {
            background_processes[num_background_processes].pid = pid;
            background_processes[num_background_processes].finished = 0;
            strcpy(background_processes[num_background_processes].name,args[0]);
            num_background_processes++;
                
            printf("Background process started with PID: %d\n", pid);
        }
        else
        {
            for(int i = 0 ; i < num_background_processes ; i++)
            {
                if(background_processes[i].finished == 1)
                {
                    background_processes[i].pid = pid;
                    background_processes[i].finished = 0;
                }
                else if(i == num_background_processes -1)
                {
                    printf("Too many background processes are running\n");
                    printf("Please try after some time\n");
                }
            }
        }
    }
}


// Given a command runs it in foreground
int execute(char** args,history h)
{
    int pe = 0;
    if(strcmp(args[0],"warp") == 0)
    {
        int x = invoke_warp(args);
    }
    else if(strcmp(args[0],"activities") == 0)
    {
        activities();
    }
    else if(strcmp(args[0],"peek") == 0)
    {
        int x = invoke_peek(args);
    }
    else if(strcmp(args[0],"pastevents") == 0)
    {
        int x = invoke_pastevents(args,h);
        pe = 1;
    }
    else if(strcmp(args[0],"seek") == 0)
    {
        int x = invoke_seek(args);
    }
    else if(strcmp(args[0],"ping") == 0)
    {
        int x = send_signal(args+1);
    }
    else if(strcmp(args[0],"fg") == 0)
    {
        invoke_fg(args);
    }
    else if(strcmp(args[0],"bg") == 0)
    {
        invoke_bg(args);
    }
    else if(strcmp(args[0],"iMan") == 0)
    {
        invoke_iman(args);
    }
    else if(strcmp(args[0],"neonate") == 0)
    {
        invoke_neonate(args);
    }
    else if(strcmp(args[0],"proclore") == 0)
    {
        if(args[1] == NULL)
        {
            pid_t pid = getpid();
            char id[20];
            snprintf(id, sizeof(args[1]), "%d", pid);
            proclore(id);
        }
        else
        proclore(args[1]);
    }
    else
    {
        pid_t p = fork();
        if(p == 0)
        {
            execvp(args[0], args);
            perror("Exec error");
        }
        else
        {
            current_foreground_pid = p;
            strcpy(foreground_name,args[0]);
            waitpid(current_foreground_pid, NULL, WUNTRACED);
        }
    }
    return pe;
}

// If redirection is present does it and then sends it to foreground/background for execution.
int redirection_handling(char** args,int fg,history h)
{
    int i = 0;
    while(args[i] != NULL) 
    {
        if(strcmp(args[i], "<") == 0)
        {
            args[i] = NULL;
            int fd2 = open(args[i+1],O_RDONLY);
            dup2(fd2,0);
        }
        else if(strcmp(args[i],">") == 0)
        {
            if(args[i+1] != NULL && strcmp(args[i+1],">") == 0)
            {
                args[i] = NULL;
                i++;
                int fd1 = open(args[i+1],O_WRONLY|O_APPEND|O_CREAT);
                dup2(fd1,1);
            }
            else
            {
                args[i] = NULL;
                int fd1 = open(args[i+1],O_CREAT|O_WRONLY|O_TRUNC, 0644);
                dup2(fd1,1);
            }
        }
        i++;
    }
    int pe = 0;
    if(fg)
    {
        pe = execute(args,h);
        current_foreground_pid = -1;
    }
    else
    {
        run_back(args,h);
    }
    return pe;
}


// Breaks it by pipes ,execute(pass to redirection_handling) each command separately by rules of piping 
int pipe_seperation(char** args,int fg,history h)
{
    int saved_stdout;
    saved_stdout = dup(1);

    int saved_stdin;
    saved_stdin = dup(0);
 
    int i = 0;
    int no_pipes = 0;
    while(args[i] != NULL)
    {
        if(strcmp(args[i],"|") == 0)
            no_pipes++;
        i++;
    }

    int pe = 0;
    if(no_pipes == 0)
    {
        pe = redirection_handling(args,fg,h);
        return pe;
    }

    int fd[no_pipes][2];
    for(int i = 0 ; i < no_pipes ; i++)
        pipe(fd[i]);

    int start = 0;
    int pipes_encountered = 0;
    i = 0;
    while(args[i] != NULL)
    {
        if(strcmp(args[i],"|") == 0 || args[i+1] == NULL)
        {
            int saved_stdout;
            saved_stdout = dup(1);

            int saved_stdin;
            saved_stdin = dup(0);

            if(pipes_encountered == 0)
            {
                dup2(fd[pipes_encountered][1],STDOUT_FILENO);
                close(fd[pipes_encountered][1]);
            }
            else if(pipes_encountered == no_pipes )
            {
                dup2(fd[pipes_encountered-1][0],STDIN_FILENO);
                close(fd[pipes_encountered-1][0]);
            }
            else
            {
                dup2(fd[pipes_encountered-1][0],STDIN_FILENO);
                dup2(fd[pipes_encountered][1],STDOUT_FILENO);
                close(fd[pipes_encountered-1][0]);
                close(fd[pipes_encountered][1]);
            }
            if(strcmp(args[i],"|") == 0)
                args[i] = NULL;
            pe = redirection_handling(args+start,fg,h);
            start = i+1;
            pipes_encountered++;

            dup2(saved_stdin,STDIN_FILENO);
            dup2(saved_stdout,STDOUT_FILENO);
        }
        i++;
    }
    dup2(saved_stdin,STDIN_FILENO);
    dup2(saved_stdout,STDOUT_FILENO);
    return pe;
}

// Given input tokenizes it, breaks it by & and ;, sends them for pipe_separationfor further execution
void exec_cmd(char* input,history h)
{
    int saved_stdout;
    saved_stdout = dup(1);

    int saved_stdin;
    saved_stdin = dup(0);

    char* args[MAX_ARGS];
    
    int l = strlen(input);
    char inp_copy[2*l]; // adds space before and after symbols - &,;,|
 
    int cur = 0;
    inp_copy[cur++] = input[0];
    for(int i = 1 ; i < l ; i++)
    {
        inp_copy[cur++] = input[i];
        if(input[i] == '&' || input[i] == ';' || input[i+1] == '&' || input[i+1] == ';' || input[i] == '>' || input[i+1] == '>' || input[i] == '<' || input[i+1] == '<')
            inp_copy[cur++] = ' ';
    }

    inp_copy[cur++] = '\0';
 
    int num_args = tokenize(inp_copy,args);
 
    cur = 0;

    int pe = 0;

    for(int i = 0 ; i < num_args ; i++)
    {
        if(strcmp(args[i],"&") == 0)
        {
            args[i] = NULL;
            pe = pipe_seperation(args+cur,0,h);
            dup2(saved_stdin,0);
            dup2(saved_stdout,1);
            cur = i+1;
        }
        else if(strcmp(args[i],";") == 0)
        {
            args[i] = NULL;
            char* prom;
            pe = pipe_seperation(args+cur,1,h);
            dup2(saved_stdin,0);
            dup2(saved_stdout,1);
            cur = i+1;
        }       
    }

    if(cur < num_args)
    pe = pipe_seperation(args+cur,1,h);
    dup2(saved_stdin,0);
    dup2(saved_stdout,1);

    if(!pe)
    add_cmd(h,input);

    check_background_processes();
    return;
}

int main()
{
    getcwd(invoked_dir, sizeof(invoked_dir));
    history h = init_history();
    get_history(h);

    if (signal(SIGINT, ctrlc_handler) == SIG_ERR) {
        perror("signal");
        return 1;
    }

    signal(SIGTSTP, ctrlz_handler);
 
    while (1)
    {
        prompt();
 
        char input[MAX_CMD_SIZE];
        if(fgets(input, MAX_CMD_SIZE, stdin) == NULL)
        {
            kill_childrens();
            printf("Logging out of shell\n");
            exit(EXIT_SUCCESS);
        }
 
        exec_cmd(input,h);
 
        save_history(h);
    }
}
