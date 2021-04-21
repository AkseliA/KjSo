#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#define MAXLEN 1024

void read_stream(FILE *stream, int flag);
void execute_command(char *command[]);
char *search_executable(char *paths[]);

int main(int argc, char *argv[]){
	FILE *input;
	int flag = 0; /*For printing wish> in interactive mode*/
	
	/*Interactive mode - commands from stdin*/
	if(argc == 1){
		flag = 1;
		printf("wish>");
		read_stream(stdin, flag);
	
	}
	
	/*Batch mode - read commands from file*/
	else if(argc == 2){
		input = fopen(argv[1], "r");
		if(input == NULL){
			//TODO ERROR MSG
			exit(1);
		}
		read_stream(input, flag);
		
	}else{
		//TODO error jos enemmän argc
		printf("Liikaa argc\n");
		exit(1);
	}
	
	fclose(input);
	
	return(0);
}

void read_stream(FILE *stream, int flag){
	char *buffer = NULL;
	size_t buffer_size = 0;
	char *command[MAXLEN];
	char *token;
	
	
	while((buffer_size = getline(&buffer, &buffer_size, stream)) > 0){
		//EOF, exit(0)
		if(feof(stream)){
			printf("\n");
			exit(0);
			
		}
		
		if(flag == 1){
			printf("wish>");
		}
		
		//parse input to tokens using strtok()
		token = strtok(buffer, " \n\t");
		 
		if(token == NULL){ /*If input is empty*/
			continue;
		}
		
		int i = 0;
		//Append tokens to command[]
		while(token != NULL){
			command[i] = token;
			i++;
			token = strtok(NULL, " \n");
		}
		command[i] = NULL; //Last value NULL for execv()
		
		
		execute_command(command);
	}
}





void execute_command(char *command[]){
	int argc = 0;
	static char *paths[256] = {"/bin", NULL}; /*Initial shell path */
	char path[256];
	char *file;
	pid_t pid;
	int status;

	
	//Count the number of arguments
	while(command[argc] != NULL){
		argc++;
	}
	
	
	/*Built-in commands:*/
	//user entered exit -> exit(0)
	if(strcmp(command[0], "exit") == 0){
		if(argc != 1){
			//TODO ERROR EXIT
			exit(1);
		}
		printf("\n");
		exit(0);
		
		
	//user entered cd -> moves to specified dir using chdir()
	//if 0 or >1 params on cd-> error
	}else if(strcmp(command[0], "cd") == 0){
		if(argc != 2){
			//TODO liikaa params
			exit(1);
		}
		//Try moving to specified folder
		if(chdir(command[1]) != 0){
			//TODO error
			exit(1);
		}
		
	
	//user entered path command -> overwrites old path(s) using strdup
	//takes 0 or more args
	}else if(strcmp(command[0], "path") == 0){
		int j = 0;
		for(int i = 1; command[i] != NULL; i++){
			paths[j] = strdup(command[i]);
			j++;
		}
		paths[j] = NULL;
	}

	//Non-built-in commands:
	/*Uses fork(), execv(), waitpid() to run the command in child process*/
	/*If execv() returns -> error*/
	else{
		
		file = command[0];
		int found = 0;
		int i = 0;
		
		//Search for the executable file from the paths supplied by user
		//or from the initial path "/bin"
		while(paths[i] != NULL){
			snprintf(path, sizeof(path)+1, "%s/%s", paths[i], file);
			i++;
			if(access(path, X_OK) == 0){
				found = 1;
				break;
			}	
		}
		
		if(found){
			switch(pid = fork()){
				case -1: //Forking error
					//TODO error
					exit(1);
				case 0: //Child process
					if(execv(path, command) == -1){
						//TODO error
						exit(1);
					}
				default: //Parent process
					if(wait(&status) == -1){
						//TODO wait error
						exit(1);
					}
			}
		}
		else{
			//executable not found
			//TODO error?
			printf("error: executable not found.");
			
		
		}
	}

		//If path == NULL -> no commands can be run
		
		//if command uses redirection using > file -> forward output to stdout
		//if more than 1 files after > -> error
		
		//Parallel commands uses & and the commands are ran simultanously
		//wait() or waitpid() to wait for completion before returning to shell
		//or before moving to next line of batch file
		
		
		

}


/*Error message*/
/*char error_message[30] = "An error has occurred\n";*/
/*write(STDERRO_FILENO, error_message, strlen(error_message));*/


