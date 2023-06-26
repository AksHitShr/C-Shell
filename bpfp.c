#include "global.h"

void sigc_handler()
{
    if (is_running_fg)
    {
        kill(curr_fg_pid, SIGINT);
        printf("\n");
    }
}

void sigz_handler()
{
    if (is_running_fg && curr_fg_pid>0)
    {
        kill(curr_fg_pid, SIGTSTP);
        int l=0;
        for (;l<100;l++)
        {
            if (arr_comm[l]==NULL)
            break;
        }
        comm_struct* t=(comm_struct*)malloc(sizeof(comm_struct));
        t->index=l+1;
        t->pid=curr_fg_pid;
        t->comm_name=(char*)malloc(strlen(curr_fp_origcomm)+1);
        strcpy(t->comm_name, curr_fp_origcomm);
        arr_comm[l]=t;
        count_bgp++;
        printf("\n[%d] %d\n", t->index, curr_fg_pid);
        sort_fp();
        printf("\n");
        prompt();
        is_running_fg=0;
        curr_fg_pid=-1;
    }
    else
    {
        printf("\n");
        prompt();
    }
}

void sort_fp()
{
    if (count_bgp < 2)
        return;

    comm_struct *temp;

    for (int i = 0; i < 99; i++)
    {
        for (int j = i; j < 99 - i; j++)
        {
            if (arr_comm[j] == NULL && arr_comm[j + 1] == NULL)
                continue;
            else if (arr_comm[j] == NULL)
            {
                arr_comm[j] = arr_comm[j + 1];
                arr_comm[j + 1] = NULL;
            }
            else if (arr_comm[j + 1] == NULL)
                continue;
            else
            {
                temp = arr_comm[j];
                arr_comm[j] = arr_comm[j + 1];
                arr_comm[j + 1] = temp;
            }
        }
    }
}

void bgp(char *comm, char **args, char *orig_comm)
{
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        setpgrp();
        int es = execvp(comm, args);
        if (es < 0)
        {
            printf("\033[0;31m\nInvalid Command!%s\n", std_clr);
            exit(1);
        }
    }
    else
    {
        int l = 0;
        for (; l < 100; l++)
        {
            if (arr_comm[l] == NULL)
                break;
        }
        comm_struct *t = (comm_struct *)malloc(sizeof(comm_struct));
        t->index = l + 1;
        t->pid = pid;
        t->comm_name = (char *)malloc(strlen(orig_comm) + 1);
        strcpy(t->comm_name, orig_comm);
        arr_comm[l] = t;

        count_bgp++;
        printf("[%d] %d\n", t->index, pid);
        sort_fp();
    }
}

int fp(char *comm, char **args, char *orig)
{
    strcpy(curr_fp_origcomm, orig);
    int status;
    pid_t pid = fork();
    time_t begin, end;
    time(&begin);
    if (pid == 0)
    {
        int es = execvp(comm, args);
        if (es < 0)
        {
            time(&end);
            printf("\033[0;31mInvalid Command!%s\n", std_clr);
            exit(1);
        }
    }
    else
    {
        is_running_fg = 1;

        curr_fg_pid = pid;
        signal(SIGTSTP, sigz_handler);
        int rv = waitpid(pid, &status, WCONTINUED | WUNTRACED);

        time(&end);
        int ans = difftime(end, begin);
        if (ans > 0)
            fl_fprocess = ans;


        if (WIFEXITED(status))
        {
            int es = WEXITSTATUS(status);
            if (es != 0)
                return 0;
        }
        is_running_fg = 0;
    }

    return 1;
}
//************

int check_if_my_func(char *m, char *to_pass, int flag)
{
    if (strcmp(m, "cd") == 0)
    {
        if (flag == 0)
            return 1;
        else
            cd(to_pass);
    }
    else if (strcmp(m, "pwd") == 0)
    {
        if (flag == 0)
            return 1;
        else
            pwd(to_pass);
    }
    else if (strcmp(m, "echo") == 0)
    {
        if (flag == 0)
            return 1;
        else
            echo(to_pass);
    }
    else if (strcmp(m, "ls") == 0)
    {
        if (flag == 0)
            return 1;
        else
            ls(to_pass);
    }
    else if (strcmp(m, "discover") == 0)
    {
        if (flag == 0)
            return 1;
        else
            discover(to_pass);
    }
    else if (strcmp(m, "history") == 0)
    {
        if (flag == 0)
            return 1;
        else
            print_history();
    }
    else if (strcmp(m, "jobs") == 0)
    {
        if (flag == 0)
            return 1;
        else
            jobs(to_pass);
    }
    else if (strcmp(m, "sig") == 0)
    {
        if (flag == 0)
            return 1;
        else
            sig(to_pass);
    }
    else if (strcmp(m, "fg") == 0)
    {
        if (flag == 0)
            return 1;
        else
            fg(to_pass);
    }
    else if (strcmp(m, "bg") == 0)
    {
        if (flag == 0)
            return 1;
        else
            bg(to_pass);
    }
    else if (strcmp(m, "pinfo") == 0)
    {
        if (flag == 0)
            return 1;
        else
            pinfo(to_pass);
    }
    else if (strcmp(m, "quit") == 0)
    {
        if (flag == 0)
            return 1;
        else
            exit(0);
    }
    else if (strcmp(m, "clear") == 0)
    {
        if (flag == 0)
            return 1;
        else
            printf("\e[1;1H\e[2J");
    }

    return 0;
}

void run_command(int input, int output, char **args)
{
    pid_t pid = fork();
    int status;

    if (pid == 0)
    {
        if (input != 0)
        {
            dup2(input, 0);
            close(input);
        }

        if (output != 1)
        {
            dup2(output, 1);
            close(output);
        }

        int es = execvp(args[0], args);
        if (es < 0)
        {
            printf("\033[0;31mInvalid Command!%s\n", std_clr);
            exit(1);
        }
    }
    else
        waitpid(pid, &status, WCONTINUED | WUNTRACED);

    return;
}

void comm_handler(char ***commands, int num, char **toks, int new, int app, int ap_fdp, int n_fdp)
{
    int orig = dup(0);

    int i, input = 0, fd[2];

    for (i = 0; i < num - 1; i++)
    {
        pipe(fd);

        int ret = check_if_my_func(commands[i][0], toks[i], 0);
        if (ret)
        {
            char temp[BUFFSIZE];
            strcpy(temp, toks[i]);
            char *pass = strtok(temp, " \t\n");
            int sout;
            if (input != 0)
            {
                sout = dup(0);
                dup2(input, 0);
                close(input);
            }
            int sin;
            if (fd[1] != 1)
            {
                sin = dup(1);
                dup2(fd[1], 1);
                close(fd[1]);
            }
            check_if_my_func(commands[i][0], pass, 1);
            if (fd[1] != 1)
            {
                dup2(sin, 1);
            }
            if (input != 0)
            {
                dup2(sout, 0);
            }
        }
        else
        {
            run_command(input, fd[1], commands[i]);
            close(fd[1]);
        }

        input = fd[0];
    }
    int orig2;

    if (input != 0)
    {
        dup2(input, 0);
        close(input);
    }
    if (new || app)
    {
        orig2 = dup(STDOUT_FILENO);
        if (app)
            dup2(ap_fdp, STDOUT_FILENO);
        else
            dup2(n_fdp, STDOUT_FILENO);
    }

    pid_t pid = fork();

    if (pid == 0)
    {

        int es = execvp(commands[num - 1][0], commands[num - 1]);

        if (es < 0)
        {
            printf("\033[0;31mInvalid Command!%s\n", std_clr);
            exit(1);
        }
    }
    else
    {
        int x;
        waitpid(pid, &x, WCONTINUED | WUNTRACED);
    }
    if (new || app)
    {
        if (new)
            close(n_fdp);
        if (app)
            close(ap_fdp);
        dup2(orig2, STDOUT_FILENO);
    }
    dup2(orig, 0);
}

void pipe_handler(char *s)
{
    int count_pipes;
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == '|')
            count_pipes++;
    }

    char *st = strtok(s, "|");
    char **toks = malloc(sizeof(char *) * MAX);
    int count = 0;
    while (st != NULL)
    {
        toks[count++] = strdup(st);
        st = strtok(NULL, "|");
    }
    toks[count] = st;
    char temp_comm[BUFFSIZE];
    for (int i = 0; i < count; i++)
    {
        strcpy(temp_comm, toks[i]);
        int j = 0;
        while (temp_comm[j] == ' ')
            j++;
        int z = 0;
        for (int k = j; k <= strlen(temp_comm); k++, z++)
            toks[i][z] = temp_comm[k];
    }

    char ***commands_with_args = (char ***)malloc(sizeof(char **) * count);
    int mem_comm[count], last_redir_app = 0, last_new = 0, ap_fdp, n_fdp;
    for (int i = 0; i < count; i++)
    {
        int out_redir = 0, index1, index11, inp_redir = 0, index2, index22, append = 0, index3, index33;
        if (i == count - 1 || i == 0)
        {
            for (int j = 0; j < strlen(toks[i]) - 1; j++)
            {
                if (toks[i][j] == '>' && toks[i][j + 1] == '>')
                {
                    append = 1;
                    index33 = j;
                }
                else if (toks[i][j] == '<')
                {
                    inp_redir = 1;
                    index22 = j;
                }
                else if (toks[i][j] == '>' && toks[i][j + 1] != '>' && toks[i][j - 1] != '>')
                {
                    out_redir = 1;
                    index11 = j;
                }
            }
            if (inp_redir)
            {
                if (inp_redir)
                    toks[i][index22] = ' ';
            }
            if (append)
            {
                last_redir_app = 1;
                char temp[BUFFSIZE];
                int x = 0;
                while (toks[i][x] != '>')
                    x++;
                x += 2;
                while (toks[i][x] == ' ' || toks[i][x] == '\t')
                    x++;
                int y = 0;
                for (int j = x; toks[i][j] != '\0' && toks[i][j] != ' '; j++, y++)
                    temp[y] = toks[i][j];
                temp[y] = '\0';
                ap_fdp = open(temp, O_APPEND | O_CREAT | O_WRONLY, 0644);
                toks[i][index33] = '\0';
            }
            if (out_redir)
            {
                last_new = 1;
                char temp[BUFFSIZE];
                int x = 0;
                while (toks[i][x] != '>')
                    x++;
                x += 2;
                while (toks[i][x] == ' ' || toks[i][x] == '\t')
                    x++;
                int y = 0;
                for (int j = x; toks[i][j] != '\0' && toks[i][j] != ' '; j++, y++)
                    temp[y] = toks[i][j];
                temp[y] = '\0';
                n_fdp = open(temp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                toks[i][index11] = '\0';
            }
        }

        strcpy(temp_comm, toks[i]);
        int len = strlen(toks[i]);
        char *m = strtok(temp_comm, " \t");
        char **temp = (char **)malloc(sizeof(char *) * MAX);
        int c2 = 0;
        while (m != NULL)
        {
            temp[c2++] = strdup(m);
            m = strtok(NULL, " \t");
        }
        temp[c2] = m;

        commands_with_args[i] = (char **)malloc(sizeof(char *) * (c2 + 1));
        for (int j = 0; j < c2; j++)
        {
            commands_with_args[i][j] = (char *)malloc(sizeof(temp[j] + 1));
            strcpy(commands_with_args[i][j], temp[j]);
        }
        commands_with_args[i][c2] = NULL;

        mem_comm[i] = c2 + 1;
        for (int j = 0; j < MAX; j++)
        {
            if (temp[j])
                free(temp[j]);
        }
        if (temp)
            free(temp);
    }

    comm_handler(commands_with_args, count, toks, last_new, last_redir_app, ap_fdp, n_fdp);

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < mem_comm[i]; j++)
        {
            if (commands_with_args[i][j])
                free(commands_with_args[i][j]);
        }
    }
    free(commands_with_args);
}

//*********
int bpfp(char *x)
{
    for (int i = 0; i < strlen(x); i++)
    {
        if (x[i] == '|')
        {
            pipe_handler(x);
            return 1;
        }
    }

    int and_count = 0;
    for (int i = 0; i < strlen(x); i++)
    {
        if (x[i] == '&')
            and_count++;
    }

    char *s = strtok(x, "&");
    char **toks = malloc(sizeof(char *) * MAX);
    int count = 0;
    while (s != NULL)
    {
        toks[count++] = strdup(s);
        s = strtok(NULL, "&");
    }
    toks[count] = s;

    for (int i = 0; i < count; i++)
    {
        int orig;
        orig = dup(STDOUT_FILENO);
        char temp_red[BUFFSIZE];
        strcpy(temp_red, toks[i]);

        int out_redir = 0, index1, index11, inp_redir = 0, index2, index22, append = 0, index3, index33;
        for (int j = 0; j < strlen(toks[i]) - 1; j++)
        {
            if (toks[i][j] == '>' && toks[i][j + 1] == '>')
            {
                append = 1;
                index33 = j;
            }
            else if (toks[i][j] == '<')
            {
                inp_redir = 1;
                index22 = j;
            }
            else if (toks[i][j] == '>' && toks[i][j + 1] != '>' && toks[i][j - 1] != '>')
            {
                out_redir = 1;
                index11 = j;
            }
        }

        char *abc = strtok(temp_red, " \t");
        char **toksx = malloc(sizeof(char *) * MAX);
        int cx = 0, ap_fdp, n_fdp;
        // assuming space present between >> or < or > and file name
        while (abc != NULL)
        {
            toksx[cx] = strdup(abc);
            if (strcmp(toksx[cx], ">>") == 0)
                index3 = cx;
            else if (strcmp(toksx[cx], ">") == 0)
                index1 = cx;
            else if (strcmp(toksx[cx], "<") == 0)
                index2 = cx;

            abc = strtok(NULL, " \t");
            cx++;
        }
        toksx[cx] = abc;
        if (append)
        {
            ap_fdp = open(toksx[index3 + 1], O_APPEND | O_CREAT | O_WRONLY, 0644);
            toks[i][index33] = '\0';
            dup2(ap_fdp, STDOUT_FILENO);
        }
        if (inp_redir)
        {
            toks[i][index22] = ' ';
        }
        if (out_redir)
        {
            n_fdp = open(toksx[index1 + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            toks[i][index11] = '\0';
            dup2(n_fdp, STDOUT_FILENO);
        }

        char arr_co[BUFFSIZE];
        strcpy(arr_co, toks[i]);
        char *m = strtok(toks[i], " \t");
        int retv = check_if_my_func(m, m, 0);
        if (retv)
        {
            check_if_my_func(m, m, 1);
        }
        else
        {
            char **toks3 = malloc(sizeof(char *) * MAX);
            int c2 = 0;
            while (m != NULL)
            {
                toks3[c2++] = strdup(m);
                m = strtok(NULL, " \t");
            }
            toks3[c2] = m;

            char *arr[c2 + 1];
            for (int j = 0; j <= c2; j++)
                arr[j] = toks3[j];

            int r = 1;
            if (and_count == count || (and_count != 0 && and_count < count && i < and_count))
                bgp(toks3[0], arr, arr_co);
            else
                r = fp(toks3[0], arr, arr_co);

            if (r == 0)
            {
                fl_fprocess = 0;
                fl_fp = 0;
                if (count == 1)
                    return 1;
            }
        }
        if (out_redir || append)
        {
            if (out_redir)
                close(n_fdp);
            if (append)
                close(ap_fdp);
            dup2(orig, STDOUT_FILENO);
        }
    }

    return 1;
}