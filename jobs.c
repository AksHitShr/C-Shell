#include "global.h"

void do_jobs(int is_r, int is_s)
{
    sort_fp();
    char temp[BUFFSIZE];
    for (int g = 0; g < count_bgp; g++)
    {
        int pid=arr_comm[g]->pid;
        int print_index=arr_comm[g]->index;
        sprintf(temp, "/proc/%d/status", pid);

        FILE *fp = fopen(temp, "r");

        if (!fp)
        {
            printf("Error open file proc/%d/status\n", pid);
            return;
        }

        char **data;
        data = (char **)malloc(sizeof(char *) * 100);
        for (int i = 0; i < 100; i++)
            data[i] = (char *)malloc(200);
        int co = 0;
        unsigned long len = 200;

        for (int i = 0; i < 200; i++)
        {
            int y = getline(&data[co], &len, fp);
            if (y > 0)
                co++;
            else
                break;
        }

        char *t1 = strtok(data[2], " \t");
        char **toks1 = malloc(sizeof(char *) * MAX);
        int c = 0;
        while (t1 != NULL)
        {
            toks1[c++] = strdup(t1);
            t1 = strtok(NULL, " \t");
        }
        toks1[c] = t1;

        if ((is_r==1 && is_s==1) || (is_r==0 && is_s==0) || is_s==1)
        {
            if (strcmp(toks1[1], "T")==0)
            printf("[%d] Stopped %s [%d]\n", print_index, arr_comm[g]->comm_name, pid);
        }
        if ((is_r==1 && is_s==1) || (is_r==0 && is_s==0) || is_r==1)
        {
            if (strcmp(toks1[1], "T"))
            printf("[%d] Running %s [%d]\n", print_index, arr_comm[g]->comm_name, pid);
        }

        fclose(fp);
    }
}

void jobs(char *s)
{
    char **toks = malloc(sizeof(char *) * MAX);
    int count = 0;
    while (s != NULL)
    {
        toks[count++] = strdup(s);
        s = strtok(NULL, " \t");
    }
    toks[count] = s;

    int is_r = 0, is_s = 0;

    if (count > 1)
    {
        if (strcmp(toks[1], "-r") == 0)
            is_r = 1;
        if (strcmp(toks[1], "-s") == 0)
            is_s = 1;
    }

    if (count > 2)
    {
        if (strcmp(toks[2], "-r") == 0)
            is_r = 1;
        if (strcmp(toks[2], "-s") == 0)
            is_s = 1;
    }

    do_jobs(is_r, is_s);
}