#include "global.h"
// signal function to check if background processes have exited

void nothing()
{
    printf("\n");
    prompt();
}

void sigf()
{
    int i = 0, p, status;
    // checking if exited, not waiting in parent process
    if ((p = waitpid(-1, &status, WNOHANG)) > 0)
    {
        for (i = 0; i < 100; i++)
        {
            if (arr_comm[i] != NULL && arr_comm[i]->pid == p)
                break;
        }
        if (i > 99)
            return;
        int pid = arr_comm[i]->pid;

        if (pid == fg_hai_noprint)
        {
            fg_hai_noprint = 0;
            return;
        }

        if (WEXITSTATUS(status) == 0)
            printf("\033[0;33m\n%s with pid %d exited normally\n%s", arr_comm[i]->comm_name, p, std_clr);
        else
            printf("\033[0;33m\n%s with pid %d exited abnormally\n%s", arr_comm[i]->comm_name, p, std_clr);
        count_bgp--;
        if (arr_comm[i]->comm_name != NULL)
            free(arr_comm[i]->comm_name);
        if (arr_comm[i] != NULL)
            free(arr_comm[i]);
        arr_comm[i] = NULL;
        sort_fp();
        prompt();
    }
}

int main()
{
    fg_hai_noprint = 0;
    // initialising global variables
    count_bgp = 0;
    fl_fprocess = 0;
    fl_fp = 0;
    flag = 0;
    for (int i = 0; i < 100; i++)
        arr_comm[i] = NULL;

    // clear terminal screen
    printf("\e[1;1H\e[2J");
    // getting details to print in command prompt
    getlogin_r(usernamex, _SC_LOGIN_NAME_MAX);
    gethostname(systemnamex, _SC_HOST_NAME_MAX);

    getcwd(pathx, BUFFSIZE);
    strcpy(base, pathx);
    base_lenx = strlen(base);
    // reading user commands repeatedly
    while (1)
    {
        curr_fg_pid = -1;
        signal(SIGTSTP, sigz_handler);
        signal(SIGINT, nothing);
        signal(SIGCHLD, sigf);
        // reading command and storing it in global var original_command
        // if (fgets(BUFF, BUFFSIZE, stdin)==NULL)
        // {
        //     printf("\n");
        //     exit(0);
        // }
        BUFF = auto_comp();

        strcpy(original_command, BUFF);
        fl_fprocess = 0;
        fl_fp = 1;
        // checking if command is invalid (for only white spaces between colons)
        int semi_count = 0;
        for (int i = 0; i < strlen(BUFF); i++)
        {
            if (BUFF[i] == ';')
                semi_count++;
        }
        // tokenizing for ; seperated commands
        char *tk = strtok(BUFF, "\n;");
        char **toks = malloc(sizeof(char *) * MAX);
        int count = 0;
        while (tk != NULL)
        {
            toks[count++] = strdup(tk);
            tk = strtok(NULL, "\n;");
        }
        toks[count] = tk;

        if (semi_count && count == 0)
        {
            printf("Syntax Error!\n");
            add_command(original_command);
            continue;
        }
        if (semi_count > 1)
        {
            int w, f1 = 1;
            for (int i = 0; i < count; i++)
            {
                w = 1;
                int l = strlen(toks[i]);
                for (int k = 0; k < l; k++)
                {
                    if (toks[i][k] != ' ' && toks[i][k] != '\t' && toks[i][k] != '\n')
                        w = 0;
                }
                if (w == 1)
                    f1 = 0;
            }
            if (f1 == 0 || semi_count + 1 > count)
            {
                printf("Syntax Error!\n");
                add_command(original_command);
                continue;
            }
        }
        // running loop for colon seperated commands
        is_running_fg = 0;
        for (int i = 0; i < count; i++)
        {
            int r_val = 1;
            char temp[100];
            int l = strlen(toks[i]);
            int f = 1;
            for (int k = 0; k < l; k++)
            {
                if (toks[i][k] != ' ' && toks[i][k] != '\t' && toks[i][k] != '\n')
                    f = 0;
            }
            if (f == 1)
                continue;
            // adding command to history
            if (original_command[strlen(original_command) - 1] != '\n')
            {
                original_command[strlen(original_command)] = '\n';
                original_command[strlen(original_command)] = '\0';
            }
            add_command(original_command);

            int j = 0;
            // removing white space from the beginning of command
            while (toks[i][j] == ' ' || toks[i][j] == '\t')
                j++;
            for (int k = 0; j <= l; j++, k++)
                temp[k] = toks[i][j];

            int retval = bpfp(temp);
            // checking if exit was the command
            if (retval == 0)
            {
                // for (int j = 0; j < MAX_NUM_BGP; j++)
                //     free(name_bgp[j]);
                // free(name_bgp);
                return 0;
            }
        }
    }
    // freeing memory allocated for global array of strings
    // for (int j = 0; j < MAX_NUM_BGP; j++)
    //     free(name_bgp[j]);
    // free(name_bgp);
    return 0;
}