#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	FILE *file;
	int count = 0;
	char character[2];

	
	/*If there are no parameters*/
	if(argc == 1){
		printf("my-unzip: file1 [file2 ...]\n");
		exit(1);
	}

	/*Open compressed file(s)*/
	for(int i=1; i<argc;i++){
		file = fopen(argv[1], "r");
		if(file == NULL){
			printf("my-unzip: cannot open file\n");
			exit(1);
		}

		/*Read file content 5 bytes at a time*/
		while (fread(&count, 4, 1, file)) { /*1. Read the number of occurences*/
			fread(character, 1, 1, file); /*2. Read the character itself*/
			
			/*Print the character according to count*/
			for(int i=0; i<count;i++){
				printf("%s", character);
			}

        	}
		fclose(file);
	}
	
	return 0;
}
