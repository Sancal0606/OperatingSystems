/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 *
 * Modified by Ilker Demirkol
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define MAX_LINE 80 /* 80 chars per line, per command */

void cd()
{
    char directory[30];
    scanf("%s", &directory);
    if (strcmp(directory, " ") == 0)
    {
        chdir("/home");
    }
    else
    {
        chdir(directory);
    }
    printf("vvvvv\n");
}

void md()
{
    char directory[30];
    scanf("%s", &directory);
    mkdir(directory, 0700);
}

void rd()
{
    char directory[30];
    scanf("%s", &directory);
    rmdir(directory);
}

void builtIn()
{
    char cmd[30];
    scanf("%s", cmd);
    if (strcmp(cmd, "cd") == 0)
    {
        printf("%s is a shell builtin \n", cmd);
    }

    else if (strcmp(cmd, "md") == 0)
    {
        printf("%s is a shell builtin \n", cmd);
    }

    else if (strcmp(cmd, "rd") == 0)
    {
        printf("%s is a shell builtin \n", cmd);
    }

    else
    {
        printf("%s is external \n", cmd);
    }
}

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
    int should_run = 1;
    while (should_run)
    {
        // get current directory
        char directory[30];
        getcwd(directory, sizeof(directory));
        printf(directory);
        printf(">");
        // get command
        char *command[30];
        scanf("%s", &command);
        if (strcmp(command, "cd") == 0)
        {
            cd();
        }

        if (strcmp(command, "md") == 0)
        {
            md();
        }

        if (strcmp(command, "rd") == 0)
        {
            rd();
        }

        if (strcmp(command, "builtin") == 0)
        {
            builtIn();
        }

        if (strcmp(command, "q") == 0)
        {
            return 0;
        }
    }
    return 0;
}