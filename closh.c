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
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_LINE		80 /* 80 chars per line, per command */
#define READ_END	0
#define WRITE_END	1

/* Change directory */
void cd(char directory[30],int count){
	if(chdir(directory) != 0){
		perror("Error changing directory");
	}
}

/* Create directory */
void md(char directory[30]){
	mkdir(directory, 0700);
}

/* Delete directory */
void rd(char directory[30]){
	if(rmdir(directory) != 0){
		perror("Error removing directory");
	}
}

/* Print whether a cmd is builtin or not*/
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
	else if(strcmp(cmd,"builtin") == 0)
	{
		printf("%s is a shell builtin \n",cmd);
	}
	else {
		printf("%s is external \n",cmd);
	}	
}

/* Execute a external cmd */
void external_cmd(char entradas[11][50],int count,bool redirect){
	printf("\n");
	/* Create a new process*/
	pid_t pid;
	pid = fork();
	char *args[10 + 2];
	int args_count = 0;
	/* Modify the input char[][] to *char[] */ 
	while(args_count <= 12){
		if(args_count == count){
			break;
		}
		args[args_count] = malloc(101);
		strcpy(args[args_count],entradas[args_count]);
		args_count++;
	}
	if (redirect){
		args_count -= 2;
		args[args_count] = NULL;
	}else{
		args[args_count] = NULL;
	}
	if(pid < 0){
		fprintf(stderr,"Fork failed");
		return 1;
	}
	/* Child executes the cmd*/
	if(pid == 0){
		if(execvp(args[0], args) == -1){
			perror("Error executing");	
		}
	}
	
	/* Parent decides if it waits for the child or not*/
	if(pid > 0){
		bool bg_mode = (entradas[count - 1][strlen(entradas[count - 1])-1]=='&');
		if(bg_mode == false)
		{
			wait(NULL);
		}
	}
	return 0;
}

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    	int should_run = 1;
    	while (should_run)
		{ 
    		// get current directory
    		char directory[30];
    		getcwd(directory, sizeof(directory));
    		printf(">");
    		printf(directory);
        	printf("%c",'%');
        	
        	/* Get user input */
        	char entrada[100];
        	char entradas[11][50];
	    	int count;
	    	FILE *fp;
	    	FILE *original_stdout_fd = dup(STDOUT_FILENO);
	    	FILE *original_stdin_fd = dup(STDIN_FILENO);	
			fgets(entrada, sizeof(entrada), stdin);
			/* Look for the equal sign*/
			int pipe_position = -1;
			bool pipe_flag=false;
			for(int i = 0; i < 100; i++)
			{
				if(entrada[i] == '='){
					pipe_position = i;
				}
			}
			pid_t pid;
			int fd[2];
			pipe(fd);
			char msg[20];
			/* Connect the output of one process to the input of the other */
			if(pipe_position != -1)
			{
				pid = fork();
				/* Parent process*/
				if (pid > 0) 
				{
						/* Read the output */		
					close(fd[WRITE_END]);
					read(fd[READ_END], msg, 20);
					int size_input = strlen(entrada) - 1;
					char ent[100];
					int cont_temp = 0;
					/* Set the entrada to the last part of the original input */
					for(int i = 0; i < size_input - pipe_position - 1;i++)
					{
						ent[i] = entrada[pipe_position + 1 + i];
						cont_temp++;
					}
					dup2(fd[READ_END], STDIN_FILENO);
					for(int i = 0; i < strlen(ent);i++)
					{
						entrada[i] = ent[i];
					}
					entrada[strlen(ent)] = '\0';
				}
				else 
				{
					/* Change the stdout to the pipe */
					entrada[pipe_position] = '\0';
					close(fd[READ_END]);
					dup2(fd[WRITE_END], STDOUT_FILENO);
					pipe_flag = true;
				}
			}
			/* Transform the entrada variable to an array */
			count = sscanf(entrada, "%s %s %s %s %s %s %s %s %s %s %s", &entradas[0], &entradas[1], &entradas[2], &entradas[3], &entradas[4], &entradas[5], &entradas[6], &entradas[7], &entradas[8], &entradas[9], &entradas[10]);    		
			bool flag = false;
			bool redirect = false;
			/* if there is ~ modify the stdout to the file*/
			if(strcmp(entradas[count - 2],"~") == 0)
			{
				redirect = true;
				fp = fopen(entradas[count - 1], "w");
				dup2(fileno(fp), STDOUT_FILENO);
			}
			/* if the first arg is cd calls to cd function */
			if(strcmp(entradas[0],"cd") == 0){
				if(count == 1){
				cd("/home",count);
				}else{
					cd(entradas[1],count);
				}	
				flag = true;
			}
			/* if the first arg is md calls to md function */
			if(strcmp(entradas[0],"md") == 0){	
				md(entradas[1]);
				flag = true;
			}
			/* if the first arg is rd calls to rd function */
			if(strcmp(entradas[0],"rd") == 0){	
				rd(entradas[1]);
				flag = true;
			}
			/* if the first arg is builtin calls to builtin function */
			if(strcmp(entradas[0],"builtin") == 0){	
				builtIn(entradas[1]);
				flag = true;
			}
			/* if the first arg is q finish the program */
			if(strcmp(entradas[0],"q") == 0){	
				return 0;
			}
			/* calls to the external function */
			if(flag == false){
				external_cmd(entradas,count,redirect);
			}
			/* Close all files */
			if(redirect){
				fflush(stdout);
				dup2(original_stdout_fd, STDOUT_FILENO);
				fclose(fp);
				close(original_stdout_fd);
			}
			if(pipe_flag){
				close(fd[WRITE_END]);
				return 0;
			}
			close(fd[READ_END]);
			dup2(original_stdin_fd, STDIN_FILENO);
	}
	return 0;
}
