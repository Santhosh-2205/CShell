#include "headers.h"

struct seek_flags
{
    int d;
    int f;
    int e;
};
typedef struct seek_flags* seek_flags;

seek_flags create_seek_seek_flags()
{
    seek_flags f = (seek_flags)malloc(sizeof(struct seek_flags));
    f->d = 0;
    f->e = 0;
    f->f = 0;
    return f;
}

int get_seek_seek_flags(char* args[10],seek_flags f)
{
    int i = 1;
    while(args[i] != NULL && args[i][0] == '-')
    {
        for(int j = 1 ; j < strlen(args[i]) ; j++)
        {
            if(args[i][j] == 'd') f->d = 1;
            else if(args[i][j] == 'e') f->e = 1;
            else if(args[i][j] == 'f') f->f = 1;
            else 
            {
                printf("no such flag\n");
                return -1;
            }
        }
        i++;
    }
    return i;
}

int seek(char* file_to_search,char* target_dir,seek_flags f,char fullpath[1024]);
int after_seek(int num_files,char* fullpath,seek_flags f);

void seek_usage()
{
    printf("%s-----------------------SEEK USAGE ERROR ---------------------%s\n",RED,RESET);
    printf("seek <flags> <search> <target_directory>\n");
    printf("if <target_directory> not provided it is set to invoked dir\n");
    printf("Flags :\n");
    printf("-d : Only look for directories (ignore files even if name matches)\n");
    printf("-f : Only look for files (ignore directories even if name matches)\n");
    printf("-e : If only 1 match found, if it's dir changes to it, if file prints it's contents\n");
    printf("%s-----------------------SEEK USAGE ERROR ---------------------%s\n",RED,RESET);
}

int invoke_seek(char* args[10])
{
    seek_flags f = create_seek_seek_flags();
    int i = get_seek_seek_flags(args,f);

    if(i == -1)
    {
        seek_usage();
        return -1;
    }

    if(f->d == 1 && f->f == 1)
    { 
        seek_usage();
        printf("Flags f,d can't be used togather\n");
        return 0;
    }
    
    char* file_to_search = args[i]; // handle error
    if(!file_to_search)
    { 
        seek_usage();
        printf("Provide a file name to search\n");
        return 0;
    }

    if(args[i+1] == NULL) args[i+1] = ".";
    else if(args[i+2] != NULL)
    {
        seek_usage();
        printf("Too many arguments\n");
        return 0;
    }
    char* target_dir = makeabsolute(args[i+1]);

    char called_dir[MAX_DIR_LEN];
    getcwd(called_dir, sizeof(called_dir));
    if( chdir(target_dir) == -1)
    {
        printf("Invalid directory \"%s\"\n",args[i+1]);
        return 0;
    }

    char fullpath[1024];
    int num_files = seek(file_to_search,".",f,fullpath);
    if(after_seek(num_files,fullpath,f) != 1)
    {
        if(chdir(called_dir) == -1)
        perror("chdir");
    }
    return 0;
}

int file_cmp(char* f1, char* f2)
{
    // f1-> no extension
    int l1 = strlen(f1);
    int l2 = strlen(f2);
    if(l1 > l2) return -1;

    char temp[l2+1];
    strcpy(temp,f2);
    if(l2 > l1 && temp[l1] == '.') temp[l1] = '\0';
    return strcmp(f1,temp);
}

int seek(char* file_to_search,char* target_dir,seek_flags f,char nfullpath[1024])
{
    int num_files = 0;

    DIR *dir;
    dir = opendir(target_dir);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        char fullpath[1024];
        snprintf(fullpath, 1024, "%s/%s", target_dir, entry->d_name);

        struct stat fileStat;
        if (lstat(fullpath, &fileStat) == -1) {
            printf("error is %s\n",fullpath);
            perror("lstat");
            continue;
        }

        if(S_ISDIR(fileStat.st_mode))
        {
            if(entry->d_name[0] == '.') 
                continue;
            num_files += seek(file_to_search,fullpath,f,nfullpath);
            if(file_cmp(file_to_search,entry->d_name) == 0 && f->f == 0)
            {
                printf("%s%s/%s%s\n",BLUE,target_dir,entry->d_name,RESET);
                num_files++;
                strcpy(nfullpath,fullpath);
                continue;
            }
            else
                continue;
        }
        else
        {
            if(file_cmp(file_to_search,entry->d_name) == 0 && S_ISREG(fileStat.st_mode) && f->d == 0)     
            {
                printf("%s%s/%s%s\n",GREEN,target_dir,entry->d_name,RESET);
                num_files++;
                strcpy(nfullpath,fullpath);
            } 
            else
                continue;
        }
    }
    return num_files;
}

int after_seek(int num_files,char* fullpath,seek_flags f)
{
    if(num_files == 0)
    {
        printf("No matches found!!\n");
    }

    if(num_files == 1 && f->e == 1)
    {
        struct stat fileStat;
        if (lstat(fullpath, &fileStat) == -1) {
            perror("lstat i got it");
        }  
        if(S_ISDIR(fileStat.st_mode))
        {
            getcwd(old_wd, sizeof(old_wd));
            chdir(fullpath);
            return 1;
        }

        if(S_ISREG(fileStat.st_mode))     
        {
            int file = open(fullpath, O_RDONLY);
            if (file == -1) {
                printf("File not found: %s\n", fullpath);
                return -1;
            }
            char buffer[1000];
            ssize_t num_read;
            while ((num_read = read(file, buffer, sizeof(buffer))) > 0) {
                write(STDOUT_FILENO, buffer, num_read);
            }
            close(file);
        } 
    }
}