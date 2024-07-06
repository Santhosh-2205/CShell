//JGD

#include "headers.h"


int cmp(datatype d1, datatype d2)
{
    int min = strlen(d1);
    if(strlen(d2) < min)
        min = strlen(d2);
    for(int i = 0 ; i < min ; i++)
    {
        if(d1[i] > d2[i])
            return 1;
        if(d1[i] < d2[i])
            return -1;
    }
    if(strlen(d1)==min)
        return -1;
    return 1;
}

void bubblesort(datatype* arr,datatype* col,datatype* des, int len)
{
    for(int i = 0 ; i < len-1 ; i++)
    {
        for(int j = 0 ; j < len-1 ; j++)
        {
            if(cmp(arr[j],arr[j+1]) == 1)
            {
                datatype temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;

                temp = col[j];
                col[j] = col[j+1];
                col[j+1] = temp;

                temp = des[j];
                des[j] = des[j+1];
                des[j+1] = temp;
            }
        }
    }
}

void bubblesort2(datatype* names,pid_t* pids,int* finished, int len)
{
    for(int i = 0 ; i < len-1 ; i++)
    {
        for(int j = 0 ; j < len-1 ; j++)
        {
            if(cmp(names[j],names[j+1]) == 1)
            {
                datatype temp = names[j];
                names[j] = names[j+1];
                names[j+1] = temp;

                pid_t temp1 = pids[j];
                pids[j] = pids[j+1];
                pids[j+1] = temp1;

                int temp2 = finished[j];
                finished[j] = finished[j+1];
                finished[j+1] = temp2;
            }
        }
    }
}