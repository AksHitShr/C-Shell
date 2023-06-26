#include "global.h"

void sig(char *s)
{
    char **toks1 = malloc(sizeof(char *) * MAX);
    int c = 0;
    while (s != NULL)
    {
        toks1[c++] = strdup(s);
        s = strtok(NULL, " \t");
    }
    toks1[c] = s;

    if (c<3)
    {
        printf("Error!\n");
        return;
    }

    int index=atoi(toks1[1]);
    int signo=atoi(toks1[2]);
    pid_t pid=0;
    for (int i=0;i<100;i++)
    {
        if (arr_comm[i]!=NULL && arr_comm[i]->index==index)
        {
            pid=arr_comm[i]->pid;
            break;
        }
    }

    if (pid==0)
    {
        printf("No such job exists!\n");
        return;
    }

    kill(pid, signo);
}