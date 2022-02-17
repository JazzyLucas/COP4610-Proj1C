#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* COP4610: Programming Project 1
 * By: Lucas Rendell n01384466
 * Write a program in C which functions as a DOS command interpreter. DOS uses
 * the  commands  cd,  dir,  type,  del,  ren,  and  copy  to  do  the  same  functions  as  the
 * UNIX commands cd, ls, cat, rm, mv, and cp. Your program loops continuously,
 * allowing  the  user  to  type  in  DOS  commands,  which  are  stored  in  the  variables
 * command, arg1 and arg2. The command should be considered by a case
 * statement,  which  executes  an  appropriate  UNIX  command,  depending  on  which
 * DOS command has been given. The program should echo the following
 * instruction to the user: Type Ctrl-C to exit.
 */

int GetCommandNumber(char* command)
{
    if (strcmp(command, "cd") == 0)
    {
        return 0;
    }
    else if (strcmp(command, "dir") == 0)
    {
        return 1;
    }
    else if (strcmp(command, "type") == 0)
    {
        return 2;
    }
    else if (strcmp(command, "del") == 0)
    {
        return 3;
    }
    else if (strcmp(command, "ren") == 0)
    {
        return 4;
    }
    else if (strcmp(command, "copy") == 0)
    {
        return 5;
    }
    return -1;
}

int CommandExecute(int command, char* arg1, char* arg2)
{
    char executionCommand[50] = "";
    // It was required to use a switch command for a variable "command".
    // So I did. :)
    switch (command)
    {
        case 0:
            strcpy(executionCommand, "cd ");
            printf("\nEnter first argument:  ");
            scanf("%s", arg1);
            strcat(executionCommand, arg1);
            system(executionCommand);
            system("pwd");
            return 0;
        case 1:
            strcpy(executionCommand, "ls");
            break;
        case 2:
            strcpy(executionCommand, "cat ");
            printf("\nEnter first argument:  ");
            scanf("%s", arg1);
            strcat(arg1, " ");
            strcat(executionCommand, arg1);
            printf("\nEnter second argument:  ");
            scanf("%s", arg2);
            strcat(executionCommand, arg2);
            break;
        case 3:
            strcpy(executionCommand, "rm ");
            printf("\nEnter first argument:  ");
            scanf("%s", arg1);
            strcat(executionCommand, arg1);
            break;
        case 4:
            strcpy(executionCommand, "mv ");
            printf("\nEnter first argument:  ");
            scanf("%s", arg1);
            strcat(arg1, " ");
            strcat(executionCommand, arg1);
            printf("\nEnter second argument:  ");
            scanf("%s", arg2);
            strcat(executionCommand, arg2);
            break;
        case 5:
            strcpy(executionCommand, "cp ");
            printf("\nEnter first argument:  ");
            scanf("%s", arg1);
            strcat(arg1, " ");
            strcat(executionCommand, arg1);
            printf("\nEnter second argument:  ");
            scanf("%s", arg2);
            strcat(executionCommand, arg2);
            break;
        default:
            return -1;
    }
    system(executionCommand);
    return 0;
}

int main()
{
    char command[50], arg1[50], arg2[50];
    int loop = 1;

    printf("\nn01384466 DOS Interpreter");

    while (loop)
    {
        printf("\n\nType Ctrl-C to exit.");
        printf("\nEnter a command:  ");
        scanf("%s", command);
        int commandNum = GetCommandNumber(command);
        int result = CommandExecute(commandNum, arg1, arg2);
        if (result == -1 || commandNum == -1)
        {
            printf("\nCouldn't execute that command.");
        }
    }
}