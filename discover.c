#include "global.h"

char to_find[BUFFSIZE];

void print_all(char* s, int d, int f)
{
    DIR *D;
    struct stat st;
    char t[BUFFSIZE];
    struct dirent *direc;
    D=opendir(s);
    while ((direc = readdir(D)) != NULL)
    {
        strcpy(t, s);
        strcat(t, direc->d_name);
        stat(t, &st);
        if (direc->d_name[0]!='.')
        {
            if ((d==0 && f==0 && strcmp(to_find, "")==0) || (d==1 && f==1 && strcmp(to_find, "")==0) || (d==1 && S_ISDIR(st.st_mode) && strcmp(to_find, direc->d_name)==0))
            printf("%s%s\n", s, direc->d_name);
            else if ((d==1 && S_ISDIR(st.st_mode) && strcmp(to_find, "")==0) || (!S_ISDIR(st.st_mode) && f==1 && strcmp(to_find, "")==0) || (f==1 && !S_ISDIR(st.st_mode) && strcmp(to_find, direc->d_name)==0))
            printf("%s%s\n", s, direc->d_name);
        }
    }
    closedir(D);

    D=opendir(s);
    while ((direc = readdir(D)) != NULL)
    {
        if (direc->d_name[0]=='.')
        continue;

        strcpy(t, s);
        strcat(t, direc->d_name);
        stat(t, &st);

        char temp[BUFFSIZE];

        if (S_ISDIR(st.st_mode))
        {
            strcpy(temp, s);
            strcat(temp, direc->d_name);
            strcat(temp, "/");

            print_all(temp, d, f);
        }
    }
    closedir(D);
}

void func3(char* direc, int d, int f, char* filename)
{
    if (strcmp(filename, "")==0)
    strcpy(to_find, "");
    else
    strcpy(to_find, filename);

    char temp[BUFFSIZE];
    if (strcmp(direc, "..")==0)
        strcpy(temp, "../");
    else if (strcmp(direc, "~")==0)
    {
        strcpy(temp, base);
        strcpy(temp, "/");
    }
    else
    {
        strcpy(temp, direc);
        if (temp[strlen(temp)-1]!='/')
        strcat(temp, "/");
        if (temp[0]!='.' && temp[0]!='/' && temp[1]!='/')
        {
            strcpy(temp, "./");
            strcat(temp, direc);
            strcat(temp, "/");
        }
    }
    print_all(temp, d, f);
}

void discover(char* s)
{
    char **toks = malloc(sizeof(char *) * MAX);
    int count = 0;
    char dir[100];
    char file[100];
    strcpy(file, "");
    while (s != NULL)
    {
        toks[count++] = strdup(s);
        s = strtok(NULL, " \t");
    }
    toks[count] = s;

    int is_d=0,is_f=0;

    if (count==1)
    {
        strcpy(dir, "./");
        func3(dir, 0, 0, file);
        return;
    }

    if (opendir(toks[1])==NULL)
    strcpy(dir, "./");
    else if (opendir(toks[1])!=NULL)
    strcpy(dir, toks[1]);

    if (count>5)
    {
        printf("Error!\n");
        return;
    }

    for (int i=1;i<count;i++)
    {
        if (strcmp(toks[i], "-d")==0)
        is_d=1;
        else if (strcmp(toks[i], "-f")==0)
        is_f=1;
        else if (opendir(toks[i])==NULL)
        strcpy(file, toks[i]);
    }

    if (strcmp(file, "")!=0)
    {
        int t_l=1;
        char n_file[BUFFSIZE];
        int j=0;
        for (int i=1;i<strlen(file)-1;i++,j++)
        n_file[j]=file[i];
        n_file[j]='\0';

        strcpy(file, n_file);
    }
    func3(dir, is_d, is_f, file);
}