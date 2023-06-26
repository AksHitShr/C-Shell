#include "global.h"

void cd(char* s)
{
    char temp2[BUFFSIZE];
    char temp[BUFFSIZE];
    char **toks = malloc(sizeof(char *) * MAX);
    int i = 0;
    while (s != NULL)
    {
        toks[i++] = strdup(s);
        s = strtok(NULL, " \t");
    }
    toks[i]=s;

    if (i>2)
    {
        printf("Error! Too many arguments to cd.\n");
        return;
    }

    if (i==1)
    {
        getcwd(temp2, BUFFSIZE);
        chdir(base);
        flag=1;
        strcpy(prev_working_dir, temp2);
    }
    else if (strcmp(toks[1], "-")==0)
    {
        if (flag==0)
        {
            printf("bash: cd: OLDPWD not set\n");
            return;
        }
        strcpy(temp2, prev_working_dir);
        strcpy(prev_working_dir, getcwd(temp, BUFFSIZE));
        chdir(temp2);
        printf("%s\n", temp2);
    }
    else if (strcmp(toks[1], "~")==0)
    {
        getcwd(temp2, BUFFSIZE);
        chdir(base);
        flag=1;
        strcpy(prev_working_dir, temp2);
    }
    else if (toks[1][0]=='~' && toks[1][1]=='/')
    {
        char new_p[BUFFSIZE];
        char t[BUFFSIZE];
        strcpy(new_p, base);
        int j=0;
        for (int x=1;x<toks[1][x]!='\0';x++,j++)
        t[j]=toks[1][x];
        t[j]='\0';
        strcat(new_p, t);
        getcwd(temp, BUFFSIZE);
        int ret=chdir(new_p);
        if (ret<0)
        {
            printf("No such file or directory.\n");
            return;
        }
        flag=1;
        strcpy(prev_working_dir, temp);
    }
    else
    {
        getcwd(temp, BUFFSIZE);
        int ret=chdir(toks[1]);
        if (ret<0)
        {
            printf("No such file or directory.\n");
            return;
        }
        flag=1;
        strcpy(prev_working_dir, temp);
    }

    return;
}