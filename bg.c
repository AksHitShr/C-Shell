#include "global.h"

void bg(char* s)
{
    char **toks1 = malloc(sizeof(char *) * MAX);
    int c = 0;
    while (s != NULL)
    {
        toks1[c++] = strdup(s);
        s = strtok(NULL, " \t");
    }
    toks1[c] = s;

    if (c>2)
    {
        printf("Error!\n");
        return;
    }

    int pid=0,i=0;
    int index=atoi(toks1[1]);

    for (;i<100;i++)
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

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    kill(pid, SIGCONT);
    //arr_comm[i]=NULL;

    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}