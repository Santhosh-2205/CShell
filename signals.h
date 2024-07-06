// JGD

#ifndef __SIGNAL_H
#define __SIGNAL_H

int send_signal(char** args);
void signal_usage_error();
void ctrlc_handler();
void ctrlz_handler();
void kill_childrens();

#endif
