#include "global.h"

void pinfo(char *s)
{
    int pid;
//tokenizing for white spaces to get command and arguments
    char **toks = (char **)malloc(sizeof(char *) * MAX);
    int count = 0;
    while (s != NULL)
    {
        toks[count++] = strdup(s);
        s = strtok(NULL, " \t");
    }
    toks[count] = s;

    if (count > 2)
    {
        printf("Too many arguments for pinfor.\n");
        return;
    }

    if (count == 1)
        pid = getpid();
    else
        pid = atoi(toks[1]);
//making path for system file from which process info is to be extracted
    char temp[BUFFSIZE];
    sprintf(temp, "/proc/%d/status", pid);
//opening proc/pid/status file
    FILE *fp = fopen(temp, "r");

    if (!fp)
    {
        printf("Error open file proc/%d/status\n", pid);
        return;
    }
    printf("pid : %d\n", pid);
    char** data;
    data=(char**)malloc(sizeof(char*)*100);
    for (int i=0;i<100;i++)
    data[i]=(char*)malloc(200);
    int co = 0;
    unsigned long len = 200;

    for (int i = 0; i<200; i++)
    {
        int y = getline(&data[co], &len, fp);
        if (y > 0)
            co++;
        else
            break;
    }

    char *t1 = strtok(data[3], " \t");
    char **toks1 = malloc(sizeof(char *) * MAX);
    int c = 0;
    while (t1 != NULL)
    {
        toks1[c++] = strdup(t1);
        t1= strtok(NULL, " \t");
    }
    toks1[c] = t1;

    char *t2 = strtok(data[2], " \t");
    char **toks2 = malloc(sizeof(char *) * MAX);
    int c2 = 0;
    while (t2 != NULL)
    {
        toks2[c2++] = strdup(t2);
        t2= strtok(NULL, " \t");
    }
    toks2[c2] = t2;

    char *t3 = strtok(data[6], " \t");
    char **toks3 = malloc(sizeof(char *) * MAX);
    int c3 = 0;
    while (t3 != NULL)
    {
        toks3[c3++] = strdup(t3);
        t3= strtok(NULL, " \t");
    }
    toks3[c3] = t3;

    printf("process Status : %s", toks2[1]);
    if (strcmp(toks1[1], toks3[1])==0)
    printf("+\n");
    else
    printf("\n");

    char *t4 = strtok(data[17], " \t");
    char **toks4 = malloc(sizeof(char *) * MAX);
    int c4 = 0;
    while (t4 != NULL)
    {
        toks4[c4++] = strdup(t4);
        t4= strtok(NULL, " \t");
    }
    toks4[c4] = t4;

    printf("memory : %s\n", toks4[1]);

    fclose(fp);

    for (int i=0;i<c;i++)
    free(data[i]);

    char ex_path[BUFFSIZE];

    sprintf(temp, "/proc/%d/exe", pid);
    fp=fopen(temp, "r");

    int ret=0;

    ret=readlink(temp, ex_path, BUFFSIZE);

    if (ret==0)
    printf("Error while opening prog/%d/exe\n", pid);
    else
    {
        ex_path[ret]='\0';
        printf("executable Path : %s\n", ex_path);
    }
}
