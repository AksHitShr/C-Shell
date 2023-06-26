#include "global.h"

void echo(char* s)
{
//handling strings with spaces and tabs in between and not accounting for string in ""
    char **toks = malloc(sizeof(char *) * MAX);
    int i = 0;
    while (s != NULL)
    {
        toks[i++] = strdup(s);
        s = strtok(NULL, " \t");
    }
    toks[i] = s;

    if (i==1)
    {
        printf("\n");
        return;
    }
//all white space seperated words will have only one space between them
    for (int i=1;toks[i]!=NULL;i++)
    printf("%s ", toks[i]);
    printf("\n");
}