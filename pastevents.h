#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

#define MAX_CMDS 15

struct hist;
typedef struct hist* history;

history init_history();
void add_cmd(history h,char* cmd);
void print_history(history h);
void purge(history h);
int invoke_pastevents(char* args[10],history h);

void get_history(history h);
void save_history(history h);

#endif

