#include "global.h"

void prompt()
{
//getting current absolute path
    getcwd(pathx, BUFFSIZE);
//checking if current path= home dir for code
    if (strcmp(base, pathx) == 0)
    {
        if (fl_fp == 0 || fl_fprocess == 0)
            printf("\r\033[0;31m%s@\033[92m%s\033[96m:~>\033[96m", usernamex, systemnamex);
        else
        {
            printf("\r\033[0;31m%s@\033[92m%s\033[96m:~took %ds>\033[96m", usernamex, systemnamex, fl_fprocess);
            fl_fp=0;
            fl_fprocess=0;
        }
    }
    else
    {
        flag = 1;
        if (strncmp(pathx, base, base_lenx) == 0)
        {
            printf("\r\033[0;31m%s@\033[92m%s\033[96m:~>\033[96m", usernamex, systemnamex);
            for (int i = base_lenx; i < strlen(pathx); i++)
                printf("%c", pathx[i]);
            if (fl_fp != 0 && fl_fprocess)
            {
                printf(": took %ds", fl_fprocess);
                fl_fp=0;
                fl_fprocess=0;
            }
            printf(">");
        }
        else
        {
            if (fl_fp == 0 || fl_fprocess == 0)
                printf("\r\033[0;31m%s@\033[92m%s\033[96m:%s>\033[96m", usernamex, systemnamex, pathx);
            else
            {
                printf("\r\033[0;31m%s@\033[92m%s\033[96m:%s: took %ds>\033[96m", usernamex, systemnamex, pathx, fl_fprocess);
                fl_fp=0;
                fl_fprocess=0;
            }
        }
    }
}
