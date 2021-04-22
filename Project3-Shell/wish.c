/*Sources:
https://stackoverflow.com/questions/11042218/c-restore-stdout-to-terminal

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#define MAXLEN 1024

void read_stream(FILE *stream, int flag);
void parse_command(char *command[]);
void execute_command(char *command[], int argc);


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
		
		
		parse_command(command);
	}
}



//This function parses the command and calls the function for execution
void parse_command(char *command[]){
	int argc = 0;
	int redirection = -1; //"Flag" for redirection
	int parallel = -1; //"flag" for parallel execution
	FILE *output;
	
	
	int fd;	
	fd = dup(1); //Save original fd for later use
	
	//Count the number of arguments and check for redirection & parallel
	while(command[argc] != NULL){
		
		if(strcmp(command[argc], ">") == 0){
			redirection = argc;
		}
		if(strcmp(command[argc], "&") == 0){
			parallel = 1;
		}
		
		argc++;

	}

	//If there's redirection symbol ">" -> command output will be redirected
	//from stdout to file using fd. More than 1 output file -> error
	if(redirection != -1){
		
		if((argc-redirection-1) != 1){
			//TODO error, too many output files
			printf("VÄÄRÄ MÄÄRÄ");
		}
		//Open output file
		output = fopen(command[redirection+1], "w");
		if(output == NULL){
			//TODO fopen error
			exit(1);
		}
		dup2(fileno(output), fileno(stdout));
		fclose(output);
		command[redirection] = NULL;
		command[redirection+1] = NULL;
		
	}
	
	//If parallel symbol "&" was found -> commands will be ran simultanously
	//-> Separates the commands into *cmd[]
	if(parallel == 1){
		char *cmd[MAXLEN];
		int curr = 0;
		for(int i = 0; i<argc ; i++){
			//When & or the end is reached, executes the command
			if((strcmp(command[i], "&")==0) || i == argc-1){
				
				cmd[curr+1] = NULL; //NULL terminated for execv()
				execute_command(cmd, curr);
				curr = 0;
				
			}else{
				cmd[curr] = command[i];
				curr++;
			}
		}
		
	//Single command execution
	}else{
	
		execute_command(command, argc);
	}
	
	//Restore original stdout redirection with original file descriptor
	dup2(fd, 1);
	close(fd);
		
}


void execute_command(char *command[], int argc){
	static char *paths[256] = {"/bin", NULL}; /*Initial shell path */
	char path[256];
	char *file;
	pid_t pid;
	int status;
	
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
}


/*Error message*/
/*char error_message[30] = "An error has occurred\n";*/
/*write(STDERRO_FILENO, error_message, strlen(error_message));*/


