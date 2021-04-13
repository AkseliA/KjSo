/*Source(s):						*/
/*https://www.geeksforgeeks.org/strstr-in-ccpp/	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 1024

void read_stream(char *term, FILE *stream);


int main(int argc, char *argv[]){
	FILE *file;
	/*If there is no parameters*/
	if(argc < 2){
		printf("my-grep: searchterm [file...]\n");
		exit(1);
	}
	
	printf("%d\n", argc);
	if(argc == 2){ /*Input from stdin*/
		read_stream(argv[1], stdin);
		
	}else{/*Input from file(s)*/
		for(int i=2; i < argc; i++){
			file = fopen(argv[i], "r");
			if(file == NULL){
				fprintf(stderr, "my-grep: cannot open file '%s'\n", argv[i]);
				exit(1);
			}
			read_stream(argv[1], file);
			
			fclose(file);
		}
	}
	return 0;
}

void read_stream(char *term, FILE *stream){	
	char buffer[MAXLEN];
	int counter = 0;

	while(fgets(buffer, MAXLEN, stream) != NULL){
		counter++;
		if(strstr(buffer, term) != NULL){
			printf("%d: %s",counter, buffer);
		}
		
	}
}
