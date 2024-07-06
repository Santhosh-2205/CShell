#ifndef __PEEK_H
#define __PEEK_H

struct peek_flags
{
    int a;
    int l;
};
typedef struct peek_flags* peek_flags;

peek_flags create_flag();
int get_peek_flags(char* args[10],peek_flags f);

char* makeabsolute(char* currentDir);
void copy_list_to_arr(list l,char** arr,int num_files);
int get_files(char* absolute_path,list files,list color);
void get_permisions(int per,char permision[11]);
void get_details(char* absolute_path,list details);

void peek_usage();
int invoke_peek(char* args[10]);
void peek(char* dir_name,peek_flags f);
void print_peek(char** arr,char** col,char** des,peek_flags f,int num_files);

#include <sys/stat.h>
#include <grp.h>
#include <time.h>

#endif
