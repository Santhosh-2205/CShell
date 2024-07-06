#ifndef __SORT_H
#define __SORT_H

typedef char* datatype;

int cmp(datatype d1, datatype d2);
void bubblesort(datatype* arr, datatype* col,datatype* des, int len);
void bubblesort2(datatype* names,pid_t* pids,int* finished, int len);

#endif
