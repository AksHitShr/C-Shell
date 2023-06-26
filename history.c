#include "global.h"
#define size 1000
//function to get previously stored data from .history.txt
int fetch(char* comm[], char* path)
{
    int command_count=0;
    unsigned long len=1000;
//opening file
    FILE* fp=fopen(path, "r");

    if (fp==0)
    return 0;
//reading line by line for user entered commands
    for (int i=0;i<20;i++)
    {
        int x=getline(&comm[i], &len, fp);
        if (x>0)
        command_count++;
        else
        break;
    }

    fclose(fp);
    return command_count;
}
//function to add commands to history
void add_command(char* command)
{
    char* comm[21];
    for (int i=0;i<21;i++)
    comm[i]=(char*)malloc(size);

    char temp[BUFFSIZE];
    strcpy(temp, base);
//creating path for .history.txt file 
    if (base[strlen(base)-1]=='/')
    strcat(temp, "history.txt");
    else
    strcat(temp, "/history.txt");

    int command_count=fetch(comm, temp);
    FILE* fp=fopen(temp, "w");

    if (fp==0)
    return;
//printing all commands if <20 else the latest 20 commands
    if (command_count==0 || (command_count<20 && strcmp(command, comm[command_count-1])!=0))
    {
        strcpy(comm[command_count++], command);
        for (int i=0;i<command_count;i++)
        fprintf(fp, "%s", comm[i]);
    }
    else if (command_count==20 && strcmp(command, comm[19])!=0)
    {
        strcpy(comm[20], command);
        command_count++;
        for (int i=1;i<21;i++)
        fprintf(fp, "%s", comm[i]);
    }
    else
    {
        for (int i=0;i<command_count;i++)
        fprintf(fp, "%s", comm[i]);
    }

    fclose(fp);
    for (int i=0;i<21;i++)
    free(comm[i]);
}

void print_history()
{
    char* comm[21];
    for (int i=0;i<21;i++)
    comm[i]=(char*)malloc(size);

    char temp[BUFFSIZE];
    strcpy(temp, base);
//creating path for .history.txt file 
    if (base[strlen(base)-1]=='/')
    strcat(temp, "history.txt");
    else
    strcat(temp, "/history.txt");
    int command_count=fetch(comm, temp);

    int tot=10,i,j=0;
//checking if num(commands)<10
    if (command_count<10)
    {
        tot=command_count;
        i=0;
    }
    else
    i=command_count-10;
//printing history commands
    for (;j<tot;i++,j++)
    printf("%s", comm[i]);

    for (int i=0;i<21;i++)
    free(comm[i]);
}