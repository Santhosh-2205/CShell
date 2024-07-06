#ifndef __FGBG_H
#define __FGBG_H

int fg(int pid);
int bg(int pid);

int invoke_fg(char** args);
void fg_usage();

int invoke_bg(char** args);
void bg_usage();

#endif