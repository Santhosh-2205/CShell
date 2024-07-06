#include "headers.h"

int invoke_warp(char* args[10])
{
    int i = 1;
    if(args[i] == NULL)
        warp(invoked_dir);
    while(args[i] != NULL)
    {
        warp(args[i]);
        i++;
    }
    return 0;
}

void warp(char* dest)
{
    char currentDir[1024];
    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("getcwd");
        return;
    }

    if(dest[0] == '-')
    {
        if(chdir(old_wd) == -1){
            perror("chdir");
            return;
        }
    }
    else if(dest[0] == '~')
    {
        char *expandedPath = malloc(strlen(invoked_dir) + strlen(dest) - 1);
        strcpy(expandedPath, invoked_dir);
        strcat(expandedPath, dest + 1);    
        if (chdir(expandedPath) == -1) {
            perror("chdir");
            free(expandedPath);
            return;
        }  
        free(expandedPath);
    }
    else
    {
        if(chdir(dest) == -1){
            perror("chdir");
            return;
        }
    }

    strcpy(old_wd,currentDir);
    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("getcwd");
        return;
    }
}