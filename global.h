#ifndef _GLOBAL_H
#define _GLOBAL_H
//header files
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <termios.h>

#define BUFFSIZE 1000
#define MAX 20
#define MAX_NUM_BGP 100

char base[BUFFSIZE];
char prev_working_dir[BUFFSIZE];
int flag;
#define MAX_FILE_NAME 100
#define std_clr "\033[96m"

typedef struct {
    char* comm_name;
    int index;
    int pid;
}comm_struct;

//global variables and arrays
char* BUFF;
char original_command[BUFFSIZE];
char usernamex[_SC_LOGIN_NAME_MAX];
char systemnamex[_SC_HOST_NAME_MAX];
char pathx[BUFFSIZE];
char curr_fp_origcomm[BUFFSIZE];
comm_struct* arr_comm[100];
// char** name_bgp;
int count_bgp;
int count_fg;
int f_sig;
int for_exit_bg;
int fl_fprocess;
int fl_fp;
//int pid_bg[MAX_NUM_BGP];
int pid_fg[MAX_NUM_BGP];
int base_lenx;
int is_running_fg;

int fg_hai_noprint;

//pid for current fg process
pid_t curr_fg_pid;
//function prototypes
void pwd(char* temp);
void ls(char* temp);
void cd(char* s);
void echo(char* s);
void add_command(char* c);
void print_history();
void discover(char* s);
void pinfo(char* s);
void handler();
int bpfp(char* s);
void sigf();
int fp(char* comm, char** args, char* orig);
void run_as_fg(char* temp);
void prompt();
void jobs(char* s);
void sig(char* s);
void fg(char* s);
void bg(char* s);
void sort_fp();
void sigz_handler();
char* auto_comp();

#endif