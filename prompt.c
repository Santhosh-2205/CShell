#include "headers.h"

char* makerelative(char* currentDir)
{
    // Input  : Absolute path of file/dir
    // Output : If inside ~(invoke dir) makes it relative

    int curr_dir_len = strlen(currentDir);
    int home_dir_len = strlen(invoked_dir);

    if(curr_dir_len < home_dir_len)
    return currentDir;

    for(int i = 0 ; i < home_dir_len ; i++)
    {
        if(invoked_dir[i] != currentDir[i])
        return currentDir;
    }

    int size = curr_dir_len - home_dir_len + 1;
    char* relative = (char*)malloc(sizeof(size + 1));
    if (!relative) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    relative[0] = '~';
    for(int i = 0 ; i < size ; i++)
    relative[i+1] = currentDir[home_dir_len+i];
    relative[size] = '\0';

    return relative;
}

void prompt() {

    char systemname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX]; 
    char currentDir[1024];

    if (gethostname(systemname, HOST_NAME_MAX) == -1) {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    if (getlogin_r(username, LOGIN_NAME_MAX) != 0) {
        perror("getlogin_r");
        exit(EXIT_FAILURE);
    }

    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    char* relative_currentdir =  makerelative(currentDir);

    printf("%s<%s@%s%s:", GREEN,username,systemname,RESET);
    printf("%s%s%s", BLUE,relative_currentdir,RESET);

    if(prompt_arg)
    {
        printf("%s>",prompt_arg);
    }
    else
    {
        printf(">");
    }
}
