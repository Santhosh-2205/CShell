#include "headers.h"

char *months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

char* makeabsolute(char* currentDir)
{
    if(currentDir[0] != '~')
        return currentDir;

    int size = strlen(currentDir) + strlen(invoked_dir) + 1;
    char* absolute = (char*)malloc(sizeof(char)*size);

    strcpy(absolute,invoked_dir);
    strcat(absolute,currentDir + 1);
    return absolute;
}

peek_flags create_flag()
{
    peek_flags f1 = malloc(sizeof(struct peek_flags));
    f1->a = 0;
    f1->l = 0;
    return f1;
}

int get_peek_flags(char* args[10],peek_flags f)
{
    int i = 1;
    while(args[i] != NULL && args[i][0] == '-')
    {
        for(int j = 1 ; j < strlen(args[i]) ; j++)
        {
            if(args[i][j] == 'a') f->a = 1;
            else if(args[i][j] == 'l') f->l = 1;
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

void copy_list_to_arr(list l,char** arr,int num_files)
{
    for(int i = 0 ; i < num_files ; i++)
    {
        l = l->next;
        arr[i] = l->data;
    }
}

int get_files(char* absolute_path,list files,list color)
{
    int num_files = 0;

    DIR *dir;
    dir = opendir(absolute_path);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        add(files,entry->d_name);

        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", absolute_path, entry->d_name);

        struct stat fileStat;
        if (lstat(fullpath, &fileStat) == -1) {
            perror("lstat");
            continue;
        }        

        if (S_ISDIR(fileStat.st_mode)) {
            add(color,"b");
        } else if (fileStat.st_mode & S_IXUSR) {
            add(color,"g");
        } else if (S_ISREG(fileStat.st_mode)) {
            add(color,"w");
        }
        num_files++;
    }

    return num_files;
}

void get_permisions(int per,char permision[11])
{
    permision[0] = '-';
    permision[1] = ((per/64) & 4) == 4 ? 'r' : '-';
    permision[2] = ((per/64) & 2) == 2 ? 'w' : '-';
    permision[3] = ((per/64) & 1) == 1 ? 'x' : '-';
    per %= 64;
    permision[4] = ((per/8) & 4) == 4 ? 'r' : '-';
    permision[5] = ((per/8) & 2) == 2 ? 'w' : '-';
    permision[6] = ((per/8) & 1) == 1 ? 'x' : '-';
    per %= 8;
    permision[7] = (per & 4) == 4 ? 'r' : '-';
    permision[8] = (per & 2) == 2 ? 'w' : '-';
    permision[9] = (per & 1) == 1 ? 'x' : '-';
    permision[10] = '\0';
}

void get_details(char* absolute_path,list details)
{
    DIR *dir;
    dir = opendir(absolute_path);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", absolute_path, entry->d_name);

        struct stat fileStat;
        if (lstat(fullpath, &fileStat) == -1) {
            perror("lstat");
            continue;
        }

        int per = fileStat.st_mode % (8*8*8);
        char permision[11];
        get_permisions(per,permision);
        if (S_ISDIR(fileStat.st_mode)) 
            permision[0] = 'd';
        else if(S_ISLNK(fileStat.st_mode))
            permision[0] = 'l';

        long int hardlinks = fileStat.st_nlink;

        char owner[100];
        strcpy(owner,getpwuid(fileStat.st_uid)->pw_name);

        char group[100];
        strcpy(group,getgrgid(fileStat.st_gid)->gr_name);

        long int size = fileStat.st_size;

        time_t mod_time = fileStat.st_mtime;
        struct tm *time_info = localtime(&mod_time);

        char* month = months[time_info->tm_mon];

        char detail[1024];
        snprintf(detail,sizeof(detail),"%s %ld %s %s %ld %s %02d %02d:%02d",permision,hardlinks,owner,group,size,month,time_info->tm_mday,time_info->tm_hour, time_info->tm_min);
        add(details,detail);
    }
}

void peek_usage()
{
    printf("%s-------------PEEK USAGE ERROR -------------%s\n",RED,RESET);
    printf("peek dir --> changes cwd to dir\n");
    printf("dir can be absolute path or relative to cwd\n");
    printf("%s-------------PEEK USAGE ERROR -------------%s\n",RED,RESET);
}

int invoke_peek(char* args[10])
{
    peek_flags f = create_flag();
    int i = get_peek_flags(args,f);
    if(i == -1)
    {
        peek_usage();
        return -1;
    }
    if(args[i] == NULL) 
    peek(".",f);
    else if(args[i+1] != NULL) peek_usage();
    else
    peek(args[i],f);
}

void peek(char* dir_name,peek_flags f)
{
    char* absolute_path =  makeabsolute(dir_name);

    list files = create_list();
    list color = create_list();
    list description = create_list();

    int num_files = get_files(absolute_path,files,color);

    get_details(absolute_path,description);

    char* arr[num_files];
    copy_list_to_arr(files,arr,num_files);

    char* col[num_files];
    copy_list_to_arr(color,col,num_files);

    char* des[num_files];
    copy_list_to_arr(description,des,num_files);

    bubblesort(arr,col,des,num_files);

    print_peek(arr,col,des,f,num_files);
}

void print_peek(char** arr,char** col,char** des,peek_flags f,int num_files)
{
    int start = 0;

    if(f->a == 0)
    {
        while(arr[start][0] == '.')
        start++;
    }

    if(f->l == 1)
    {
        for(int i = start ; i < num_files ; i++)
        {
            printf("%s ",des[i]);
            if(col[i][0] == 'b')
            printf("%s%s%s\n",BLUE,arr[i],RESET);
            else if(col[i][0] == 'g')
            printf("%s%s%s\n",GREEN,arr[i],RESET);
            else if(col[i][0] == 'w')
            printf("%s\n",arr[i]);
        } 
        return;
    }
    
    for(int i = start ; i < num_files ; i++)
    {
        if(col[i][0] == 'b')
        printf("%s%s%s  ",BLUE,arr[i],RESET);
        else if(col[i][0] == 'g')
        printf("%s%s%s  ",GREEN,arr[i],RESET);
        else if(col[i][0] == 'w')
        printf("%s  ",arr[i]);
    }  
    printf("\n") ;
}

