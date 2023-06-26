#include "global.h"

void pwd(char *temp)
{
    if (temp[3]==' ' || temp[3]=='\0')
    {
        char x[BUFFSIZE];
        getcwd(x, BUFFSIZE);
        printf("%s\n", x);
    }
    else
        printf("\033[0;31mError!\n%s", std_clr);
}