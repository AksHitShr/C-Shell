#include "global.h"

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */

void handle_tab(char *str, int *t)
{
    struct stat x;
    DIR *d;
    struct dirent *direc;
    int p = 0, flag = 0, count;
    int len = strlen(str);

    if (len)
    {
        for (; p < len; p++)
        {
            if (str[p] == ' ')
                count++;
        }
        if (count == len)
            flag = 1;
    }

    if (*t == 0)
        flag = 1;

    if (flag == 1)
    {
        printf("\n");
        d = opendir(".");
        if (d)
        {
            while ((direc = readdir(d)) != NULL)
            {
                if (direc->d_name[0] == '.')
                    continue;
                stat(direc->d_name, &x);
                if (S_ISDIR(x.st_mode))
                    printf("%s/\n", direc->d_name);
                else
                    printf("%s \n", direc->d_name);
            }
            closedir(d);
        }
        prompt();
        printf("%s", str);
        return;
    }

    char addition[150];
    int i;

    for (p = len - 1; p > 0; p--)
    {
        if (str[p] == ' ')
            break;
    }
    p++;
    for (i = 0; p < len; i++, p++)
        addition[i] = str[p];
    addition[i] = '\0';

    int total = 0;
    d = opendir(".");
    if (d)
    {
        while ((direc = readdir(d)) != NULL)
        {
            if (direc->d_name[0] == '.')
                continue;
            stat(direc->d_name, &x);
            if (strncmp(direc->d_name, addition, i) == 0)
                total++;
        }
        closedir(d);
    }
    if (total == 0)
        return;
    char arr_names[total][100];
    int max = 1000;
    int co = 0;
    d = opendir(".");
    if (d)
    {
        while ((direc = readdir(d)) != NULL)
        {
            if (direc->d_name[0] == '.')
                continue;
            stat(direc->d_name, &x);
            if (strncmp(direc->d_name, addition, i) == 0)
            {
                strcpy(arr_names[co], direc->d_name);
                if (S_ISDIR(x.st_mode))
                strcat(arr_names[co], "/");
                int l = strlen(arr_names[co++]);
                if (l < max)
                    max = l;
            }
        }
        closedir(d);
    }

    for (int j = i; j < max; j++)
    {
        int nc = 1;
        char ch = arr_names[0][j];
        for (int z = 1; z < total; z++)
        {
            if (arr_names[z][j] == ch)
                nc++;
        }
        if (nc == total)
            str[(*t)++] = ch;
        else
            break;
    }

    printf("\n");
    d = opendir(".");
    if (total > 1)
    {
        if (d)
        {
            while ((direc = readdir(d)) != NULL)
            {
                if (direc->d_name[0] == '.')
                    continue;
                stat(direc->d_name, &x);
                if (strncmp(direc->d_name, addition, i) == 0)
                {
                    if (S_ISDIR(x.st_mode))
                        printf("%s/\n", direc->d_name);
                    else
                        printf("%s\n", direc->d_name);
                }
            }
            closedir(d);
        }
    }
    prompt();
    printf("%s", str);
}

char *auto_comp()
{
    char *inp = malloc(sizeof(char) * 1000);
    char c;
    int pt = 0;
    // while (1) {
    setbuf(stdout, NULL);
    enableRawMode();
    prompt();
    memset(inp, '\0', 100);
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
                break;
            else if (c == 27)
            {
                char buf[3];
                buf[2] = 0;
                if (read(STDIN_FILENO, buf, 2) == 2)
                { // length of escape code
                    printf("\rarrow key: %s", buf);
                }
            }
            else if (c == 127)
            { // backspace
                if (pt > 0)
                {
                    if (inp[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                        {
                            printf("\b");
                        }
                    }
                    inp[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9)
            { // TAB character
                // inp[pt++] = c;
                // for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                //     printf(" ");
                // }
                // printf("%s\n", inp);
                handle_tab(inp, &pt);
                // if (pt==0)
                pt = strlen(inp);
            }
            else if (c==26)
            return NULL;
            else if (c == 4)
            {
                printf("\n");
                exit(0);
            }
            else
            {
                printf("%d\n", c);
            }
        }
        else
        {
            inp[pt++] = c;
            printf("%c", c);
        }
    }
    printf("\n");
    disableRawMode();

    // printf("\nInput Read: [%s]\n", inp);

    return inp;
}