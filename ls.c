#include "global.h"
//colour function to give diff colour to dir,exec files and others
char *col(struct stat details, char **ptr, int i)
{
    int ter = stat(ptr[i], &details);

    if (S_ISDIR(details.st_mode))
        return "\033[0;34m";
    else if (ter == 0 && details.st_mode & S_IXUSR)
        return "\033[0;32m";
    else
        return "\033[0;37m";
}

void func(int a, int l, char **ptr, int c, struct stat details, int flg)
{
    // flg==1 for sub-directory
    if (l)
    {
        int total_ls = 0;
        for (int i = 0; i < c; i++)
        {
            if (flg == 0 && a == 0 && ptr[i][0] == '.')
                continue;
            else if (flg == 1 && a == 0)
            {
                int f2 = 0;
                int ler = strlen(ptr[i]);
                for (int x = 0; x < ler - 1; x++)
                {
                    if (ptr[i][x] == '/' && ptr[i][x + 1] == '.')
                    {
                        f2 = 1;
                        break;
                    }
                }
                if (f2)
                    continue;
            }

            stat(ptr[i], &details);
            total_ls += details.st_blocks;
        }
        if (c!=1)
        printf("total %d\n", total_ls);

        for (int i = 0; i < c; i++)
        {
            if (flg == 0 && a == 0 && ptr[i][0] == '.')
                continue;
            else if (flg == 1 && a == 0)
            {
                int f2 = 0;
                int ler = strlen(ptr[i]);
                for (int x = 0; x < ler - 1; x++)
                {
                    if (ptr[i][x] == '/' && ptr[i][x + 1] == '.')
                    {
                        f2 = 1;
                        break;
                    }
                }
                if (f2)
                    continue;
            }

            stat(ptr[i], &details);
            if (S_ISDIR(details.st_mode))
                printf("%sd", col(details, ptr, i));
            else
                printf("%s-", col(details, ptr, i));
            // for user
            if (details.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");
            if (details.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");
            if (details.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");
            // for group
            if (details.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");
            if (details.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");
            if (details.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");
            // for others
            if (details.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");
            if (details.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");
            if (details.st_mode & S_IXOTH)
                printf("x");
            else
                printf("-");

            // link count
            printf("  ");
            printf("%3d ", details.st_nlink);
            struct passwd *pws;
            pws = getpwuid(details.st_uid);
            printf("%s ", pws->pw_name);
            printf("%s ", getgrgid(details.st_gid)->gr_name);
            printf("%6lld ", details.st_size);
            time_t curr_time = time(NULL);
            struct tm file_time, curr;
            localtime_r(&details.st_mtime, &file_time);
            localtime_r(&curr_time, &curr);
            char time_str[100];

            if (file_time.tm_year == curr.tm_year)
            {
                strftime(time_str, sizeof(time_str), "%b %e %H:%M", &file_time);
                printf("%s  ", time_str);
            }
            else
            {
                strftime(time_str, sizeof(time_str), "%b %e  %Y", &file_time);
                printf("%s  ", time_str);
            }

            if (!flg)
                printf("%s\n%s", ptr[i], std_clr);
            else
            {
                int j = strlen(ptr[i]) - 1;
                while (ptr[i][j] != '/')
                    j--;
                for (int l = j + 1; l < strlen(ptr[i]); l++, j++)
                    printf("%c", ptr[i][l]);
                printf("\n%s", std_clr);
            }
        }
    }
    else
    {
        for (int i = 0; i < c; i++)
        {
            if (flg == 0 && a == 0 && ptr[i][0] == '.')
                continue;
            else if (flg == 1 && a == 0)
            {
                int f2 = 0;
                int ler = strlen(ptr[i]);
                for (int x = 0; x < ler - 1; x++)
                {
                    if (ptr[i][x] == '/' && ptr[i][x + 1] == '.')
                    {
                        f2 = 1;
                        break;
                    }
                }
                if (f2)
                    continue;
            }

            if (flg == 0)
                printf("%s%s\n%s", col(details, ptr, i), ptr[i], std_clr);
            else
            {
                printf("%s", col(details, ptr, i));
                int j = strlen(ptr[i]) - 1;
                while (ptr[i][j] != '/')
                    j--;
                for (int l = j + 1; l < strlen(ptr[i]); l++, j++)
                    printf("%c", ptr[i][l]);
                printf("\n%s", std_clr);
            }
        }
    }
}

void sort_names(char **ptr, int tot)
{
    if (tot == 1)
        return;

    char temp[MAX_FILE_NAME];
    for (int i = 0; i < tot - 1; i++)
    {
        for (int j = 0; j < tot - i - 1; j++)
        {
            if (strcasecmp(ptr[j], ptr[j + 1]) > 0)
            {
                strcpy(temp, ptr[j]);
                strcpy(ptr[j], ptr[j + 1]);
                strcpy(ptr[j + 1], temp);
            }
        }
    }
}

void sort_names2(char **ptr, int tot)
{
    struct stat a, b;
    if (tot == 1)
        return;
    char temp[MAX_FILE_NAME];
    for (int i = 0; i < tot - 1; i++)
    {
        for (int j = 0; j < tot - i - 1; j++)
        {
            stat(ptr[j], &a);
            stat(ptr[j + 1], &b);
            if (S_ISDIR(a.st_mode) == S_ISDIR(b.st_mode))
            {
                if (strcasecmp(ptr[j], ptr[j + 1]) > 0)
                {
                    strcpy(temp, ptr[j]);
                    strcpy(ptr[j], ptr[j + 1]);
                    strcpy(ptr[j + 1], temp);
                }
            }
            else
            {
                if (S_ISDIR(a.st_mode))
                {
                    if (strcasecmp(ptr[j], ptr[j + 1]) > 0)
                    {
                        strcpy(temp, ptr[j]);
                        strcpy(ptr[j], ptr[j + 1]);
                        strcpy(ptr[j + 1], temp);
                    }
                }
            }
        }
    }
}

void func2(int a, int l, char *s, int flg)
{
    struct stat details;
    DIR *d;
    struct dirent *direc;
    d = opendir(s);

    int c = 0;
    if (d)
    {
        while ((direc = readdir(d)) != NULL)
            c++;
        closedir(d);
    }
    else
    {
        stat(s, &details);
        if (S_ISREG(details.st_mode))
            func(a, l, &s, 1, details, 0);
        else
            printf("\033[0;31mError!\n%s", std_clr);
        return;
    }
    char **ptr = (char **)malloc(sizeof(char *) * c);
    d = opendir(s);
    int index = 0;
    if (d)
    {
        while ((direc = readdir(d)) != NULL)
        {
            ptr[index] = (char *)malloc(MAX_FILE_NAME);
            if (!flg)
                strcpy(ptr[index], direc->d_name);
            else
            {
                strcpy(ptr[index], s);
                strcat(ptr[index], "/");
                strcat(ptr[index], direc->d_name);
            }
            index++;
        }
        closedir(d);
    }
    sort_names(ptr, c);

    func(a, l, ptr, c, details, flg);

    // for (int j=0;ptr[j]!=NULL;j++)
    // free(ptr[j]);
    // free(ptr);
}

void print_ls(int a, int l, char *p)
{
    if (p == NULL)
        func2(a, l, ".", 0);
    else if (strcmp(p, ".") == 0)
    {
        // if (l)
        //     printf(".:\n");
        func2(a, l, ".", 0);
    }
    else if (strcmp(p, "..") == 0)
    {
        // if (l)
        //     printf("..:\n");
        char ter[BUFFSIZE];
        int j = 0;
        int b = strlen(base);
        int f = b - 1;
        while (base[f] != '/')
            f--;
        for (int x = 0; x < f; x++)
            ter[x] = base[x];
        func2(a, l, ter, 1);
    }
    else if (strcmp(p, "~") == 0)
        func2(a, l, base, 0);
    else
    {
        if (p[0] == '~')
        {
            char temp[BUFFSIZE];
            strcpy(temp, base);
            int y = strlen(base), x, q = 1;
            for (x = y; x < y + strlen(p) - 1; x++, q++)
                temp[x] = p[q];
            temp[x] = '\0';
            func2(a, l, temp, 1);
        }
        else
            func2(a, l, p, 1);
    }
}

void ls(char *s)
{
//tokenizing for white space characters
    char **toks = malloc(sizeof(char *) * MAX);
    int i = 0;
    while (s != NULL)
    {
        toks[i++] = strdup(s);
        s = strtok(NULL, " \t");
    }
    toks[i] = s;
//for only ls as command
    if (i == 1)
    {
        print_ls(0, 0, NULL);
        return;
    }

    char **ptr = (char **)malloc(sizeof(char *) * i);
    int count = 0, j, is_a = 0, is_l = 0;
//checking for flags -a,-l,-al
    for (j = 1; j < i; j++)
    {
        int l = strlen(toks[j]);

        if (toks[j][0] == '-')
        {
            int fl = 1, for_a = 0, for_l = 0;
            for (int k = 1; k < l; k++)
            {
                if (toks[j][k] != 'l' && toks[j][k] != 'a')
                    fl = 0;
                else if (toks[j][k] == 'a')
                    for_a = 1;
                else
                    for_l = 1;
            }

            if (fl == 1)
            {
                if (for_a)
                    is_a++;
                if (for_l)
                    is_l++;
            }
            else
            {
                printf("\033[0;31mInvalid command!\n%s", std_clr);
                return;
            }
        }
        else
        {
            int len = strlen(toks[j]);
            ptr[count] = (char *)malloc(len);
            strcpy(ptr[count], toks[j]);
            count++;
        }
    }
    ptr[count] = NULL;
    if (is_a)
        is_a = 1;
    if (is_l)
        is_l = 1;

    if (count == 0)
        print_ls(is_a, is_l, NULL);
    else
    {
        //sorting to print file details before directories
        sort_names2(ptr, count);
        for (int j = 0; j < count; j++)
            print_ls(is_a, is_l, ptr[j]);
    }
//freeing allocated memory
    for (int v = 0; ptr[v] != NULL; v++)
    {
        if (ptr[v])
        free(ptr[v]);
    }
}