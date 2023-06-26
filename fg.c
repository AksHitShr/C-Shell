#include "global.h"

void fg(char* s)
{
    time_t now,after;
    now=time(NULL);
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
    //printf("%d\n", pid);
    int st;
    fg_hai_noprint=pid;

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    //tcsetpgrp(STDIN_FILENO, pid);
    tcsetpgrp(STDIN_FILENO, getpgid(pid));
    kill(pid, SIGCONT);
    waitpid(pid, &st, WUNTRACED);
    after=time(NULL);
    //printf("%ld\n", after-now);
    if (after-now>=1)
    {
        fl_fprocess=after-now;
        fl_fp=1;
    }

    if (arr_comm[i]->comm_name!=NULL)
    free(arr_comm[i]->comm_name);
    if (arr_comm[i]!=NULL)
    free(arr_comm[i]);
    arr_comm[i]=NULL;
    count_bgp--;
    tcsetpgrp(STDIN_FILENO, getpid());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    curr_fg_pid=pid;
    is_running_fg=1;
}