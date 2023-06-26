# Assignment 2 (OSN)
## Akshit Sharma
## 2021101029
### LINUX SHELL implementation in C

# Assignment 3 MID SUBMISSION 
> done first 3 specifications
> assumed that the >,< or >> symbols are seperated from names and commands by white space
> did not use execvp for implementing self coded functions

## Run Shell:
```bash
>make
>./a.out
```
## Makefile
    command for compiling all c files.
    type make command to compile all files for shell.

## global.h
    contains all global variable declarations and header files.

## main.c
    sigf():
    signal function to check if any background process has exited and to print its exit status.

    main(): Find username, system name to print in command prompt; Take user input, tokenize it for semi-colons and run loop to execute each
    Also, check if command is only having white space and throwing error for two semi-colons having only white-space in between

## prompt.c
    promt(): Print command prompt on terminal depending on current directory.
    Also displays time taken by a background process after it exits. (if time taken >= 1sec)

## pwd.c
    pwd(): Print Absolute path of current directory using getcwd() function.

## echo.c
    echo():
```bash
    echo <"Strings with tabs and spaces have been handled">
```

## cd.c
    commands handled:
```bash
cd .
cd ..
cd -
cd ~
cd <dir_name>
cd <absolute_path>
cd <dir_path>
```
## ls.c
    list file attributes depending on flags -a and -l.
```bash
    ls 
    ls -al/-la/-l/-a
    ls <flags> <dir_path>
    ls <flags> <file_name>
    ls <flags> <file_path>
    ls <flags> <dir_name>
    ls <flags> <file_name>
```
    col(): function used to give approprite colour to printed data depending on type of file (checked using stat function).
    handled sorted names, files before directories when both given as arguments,etc.

## bpfp.c
    checks if current command is one of :
```bash
cd 
ls
pinfo
echo
pwd
quit
clear
jobs
sig
bg
fg
history
```
    For foreground process, printing time taken by the last process only in prompt.
    Assuming that at most 100 bg processes will be there.

    if it is, then they are executed by calling their respective functions instead of using execvp()

    Otherwise, execute command as foreground or background process by using fork.

    Terminating shell
    quit command exits the shell

    Clearing screen
    clear command clears the terminal screen.

## pinfo.c
    pinfo():
    print pid, process status, virtual memory and executable path of process whose pid is entered or else the current process itself if no argument is given.
    Data is extracted from proc/"pid"/status file.
```bash
>pinfo "pid"
or 
>pinfo
```
## discover.c
    to show directories, files or both with given name
    Otherwise all files or directories
```bash
discover <terget_dir> <type_flags> <"file_name">
discover <target_dir> -d/-f/-d -f
discover -d/-f -d -f <"file_name">
discover <file_name>
discover <target_dir>
``` 
    graph traversal like algorithm used to make recursive function to traverse directories to search and print required files

## history.c
    add_command(): add the command entered by user to the file storing history (.history.txt)

    print_history(): Prints command history when fllowing command is entered.

    fetch(): get the old commands from .history.txt to be used for adding new command or printing history.
```bash
>history
```
    created .history.txt (as hidden file) which prints at most 10 latest commands entered by user.

## jobs.c
     prints a list of all currently running background processes spawned by the shell in alphabetical order of the command name, along with their job number (a sequential number assigned by your shell), process ID, and their state, which can either
    be running or stopped.

    flags -r (for running) or -s (for stopped) or both or none may be there (considering all in last 2 cases)

## sig.c
    Takes the job number (assigned by your shell) of a running job and sends the signal corresponding to the signal number to that process. The shell throws an error if no job with the given number exists.

## fg.c
    Brings the running or stopped background job corresponding to job number to the foreground, and changes its state to running. The shell throws an error if no job with the given job number exists.

## bg.c
     Changes the state of a stopped background job to running (in the background). The shell throws an error if no background job corresponding to the given job number exists, and do nothing if the job is already running in the background.

## SIGNALS
> CTRL+C: It interrupts any currently running foreground job, by sending it the SIGINT signal. This has no effect on the shell if there is no foreground process running.

> CTRL+D: It logs you out of my shell, without having any effect on the actual terminal.

> CTRL+Z:It pushes any currently running foreground job into the background, and change its state from ‘running’ to ‘stopped.’ This has no effect on the shell if there is no foreground process running.

## AUTO-COMPLETE

    done for files and directories within the current directory as well as paths like /home/, etc.
    if only tab is pressed or command+" "+<tab> is pressed, all files whithin this directory are printed.
