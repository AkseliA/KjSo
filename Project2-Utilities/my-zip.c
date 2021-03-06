#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_compressed(FILE *stream, int count, char character);

int main(int argc, char *argv[]){
	FILE *file;
	int count = 1;
	char buffer;
	char prevbuffer = -1;
	
	/*If there is no parameters*/
	if(argc == 1){
		printf("my-zip: file1 [file2 ...]\n");
		exit(1);
	}
	
	/*Read file(s)*/
	for(int i=1; i<argc;i++){
		file = fopen(argv[1], "r");
		if(file == NULL){
			printf("my-zip: cannot open file\n");
			exit(1);
		}
		
		/*Read content one character at a time*/
		//https://overiq.com/c-programming-101/fgetc-function-in-c/
		while((buffer = fgetc(file)) != EOF){
			
			/*RLE - comparing current and previous characters*/
			if(buffer == prevbuffer){
				count++;
					
			/*If prev != curr -> char count and char to stdout*/
			}else{
				if(prevbuffer != -1){ 
					write_compressed(stdout, count, prevbuffer);
				}
				
				prevbuffer = buffer;
				count = 1;
			}
		}
		fclose(file);
	}	
	return 0;
}

/*Writes the number of entries and character to stdout using fwrite*/
/*each write operation writes 5 bytes total (number 4 + char 1)	*/
void write_compressed(FILE *stream, int count, char character){
	fwrite(&count, 4, 1, stream);
	fwrite(&character, 1, 1, stream);
	
}
