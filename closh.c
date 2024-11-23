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
#define MAX_LINE 80 /* 80 chars per line, per command */

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
        char command[30];
        scanf("%s", &command);
        printf("Hello %s", command);
        fflush(stdout);
    }
    return 0;
}