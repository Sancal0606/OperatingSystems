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
#include <sys/types.h>
#define MAX_LINE		80 /* 80 chars per line, per command */

void cd(char directory[30],int count){
	if(count==1){
		chdir("/home");
	}else{
		chdir(directory);
	}
}

void md(char directory[30]){
	mkdir(directory, 0700);
}

void rd(char directory[30]){
	rmdir(directory);
}

void builtIn(char cmd[30]){
	if(strcmp(cmd,"cd") == 0)
	{
		printf("%s is a shell builtin \n",cmd);
	}
	else if(strcmp(cmd,"md") == 0)
	{
		printf("%s is a shell builtin \n",cmd);
	}
	else if(strcmp(cmd,"rd") == 0)
	{
		printf("%s is a shell builtin \n",cmd);
	}
	else {
		printf("%s is external \n",cmd);
	}
}

void external_cmd(char entradas[11][50],int count){
	pid_t pid;
	pid = fork();
	if(pid < 0){
		fprintf(stderr,"Fork failed");
		return 1;
	}
	else if(pid == 0){
		char *args[10 + 2];
		int args_count = 0;
		while(args_count <= 12){
			if(args_count == count){
				break;
			}
			args[args_count] = malloc(101);
			strcpy(args[args_count],entradas[args_count]);
			args_count++;
		}
		args[args_count] = NULL;
		execvp(args[0], args);
	}else{
		wait(NULL);
	}
	return 0;
}

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    	int should_run = 1;
		
    	while (should_run){ 
    		// get current directory
    		char directory[30];
    		getcwd(directory, sizeof(directory));
    		printf(directory);
        	printf(">");
        	//get command
        	char entrada[100];
        	char entradas[11][50];
	    	int count;
    		fgets(entrada, sizeof(entrada), stdin);
    		count = sscanf(entrada, "%s %s %s %s %s %s %s %s %s %s %s", &entradas[0], &entradas[1], &entradas[2], &entradas[3], &entradas[4], &entradas[5], &entradas[6], &entradas[7], &entradas[8], &entradas[9], &entradas[10]);
		//process command
        	if(strcmp(entradas[0],"cd") == 0){	
        		cd(entradas[1],count);
        	}
        	else if(strcmp(entradas[0],"md") == 0){	
        		md(entradas[1]);
        	}
        	else if(strcmp(entradas[0],"rd") == 0){	
        		rd(entradas[1]);
        	}
        	else if(strcmp(entradas[0],"builtin") == 0){	
        		builtIn(entradas[1]);
        	}
        	else if(strcmp(entradas[0],"q") == 0){	
        		return 0;
        	}
        	else{
        		external_cmd(entradas,count);
        	}	
    	}
	return 0;
}