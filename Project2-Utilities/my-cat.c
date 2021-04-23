#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 1024

void read_file(char *fname);

int main(int argc, char *argv[]){

	//open and read file(s) passed as agruments
	for(int i=1; i < argc; i++){
		read_file(argv[i]);
	}
	
	return 0;
}

void read_file(char *fname){
	FILE *file;
	char buffer[MAXLEN];
	
	file = fopen(fname, "r");
	if(file == NULL){
		printf("my-cat: cannot open file\n");
		exit(1);
	}
	
	while(fgets(buffer, MAXLEN, file) != NULL){
		printf("%s", buffer);
	}
	
	fclose(file);
}
