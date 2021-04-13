#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 1024

struct Lines{
	char* string;
	struct Lines* next;
};
typedef struct Lines lines;

lines *read_data(FILE* stream, lines *head);
lines *reverse_ll(lines *head);
void print_lines(lines *head);
void write_file(lines *head, char *filename);
void free_memory(lines *head);


int main(int argc, char *argv[]){
	lines *head = NULL;
	
	if(argc == 1){
		fprintf(stdout, "Reading lines from stdio(ctrl+D to finish)\n");
		head = read_data(stdin, head);
		head = reverse_ll(head);
		print_lines(head);

	}else if(argc == 2){
		FILE* file;
		file = fopen(argv[1], "r");
		if(file == NULL){
			fprintf(stderr, "cannot open file '%s'\n", argv[1]);
			exit(1);
		}
		fprintf(stdout, "Reading files from '%s'\n", argv[1]);
		head = read_data(file, head);
		head = reverse_ll(head);
		print_lines(head);

	
	}else if(argc == 3){
		if(strcmp(argv[1], argv[2]) == 0){
			fprintf(stdout, "Input and output file must differ.\n");
			exit(1);
		}
		FILE* file;
		file = fopen(argv[1], "r");
		if(file == NULL){
			fprintf(stderr, "error: cannot open file '%s'.\n", argv[1]);
			exit(1);
		}
		
		fprintf(stdout, "Reading files from '%s'\n", argv[1]);
		head = read_data(file, head);
		head = reverse_ll(head);
		write_file(head, argv[2]);

	}else{
		fprintf(stdout, "usege: reverse <input> <output>\n");
		exit(1);
	}
	
	free_memory(head);

	return 0;
}

lines *read_data(FILE* stream, lines *head){
	char buffer[MAXLEN];
	lines *ptr, *ptrNew;
	while(fgets(buffer, MAXLEN, stream) != NULL){
		
		
		ptrNew = (lines*)malloc(sizeof(lines));
		if(ptrNew == NULL){
			fprintf(stderr, "error: malloc failed.\n");
			exit(1);
		}
		ptrNew->string = malloc(strlen(buffer) + 1);
		if(ptrNew->string == NULL){
			fprintf(stderr, "error: malloc failed.\n");
			exit(1);
		}
		strcpy(ptrNew->string, buffer);
		ptrNew->next = NULL;
		
		/*Sijoitetaan oikealle paikalle*/
		if(head == NULL){
			ptr = head = ptrNew;
		}else{
			ptr = ptr->next = ptrNew;
		}
	}
	fclose(stream);
	return head;
}
//https://www.geeksforgeeks.org/reverse-a-linked-list/
lines *reverse_ll(lines *head){
	lines *prev = NULL; 
	lines *next = NULL;
	lines *ptr = head;
	fprintf(stdout, "Reversing...\n");
	while(ptr != NULL){
		next = ptr->next;
		ptr->next = prev;
		
		prev = ptr;
		ptr = next;
	}
	head = prev;
	
	return head;
}

void print_lines(lines *head){
	lines *ptr = head;
	fprintf(stdout, "\nPrinting lines in reverse order:\n");
	while(ptr != NULL){
		fprintf(stdout, "%s", ptr->string);
		ptr = ptr->next;
	}
}

void write_file(lines *head, char *filename){
	lines *ptr = head;
	FILE *fout = fopen(filename, "w");
	if(fout == NULL){
		fprintf(stderr, "error: cannot open file '%s'.", filename);
		exit(1);
	}
	fprintf(stdout, "\nWriting lines to '%s' in reverse order.\n", filename);
	while(ptr != NULL){
		fprintf(fout, "%s", ptr->string);	
		ptr = ptr->next;	
	}
	fclose(fout);
	
}

void free_memory(lines *head){
	lines *ptr;
	ptr = head;
	while(ptr != NULL){
		head = ptr->next;
		free(ptr->string);
		free(ptr);
		ptr = head;
	}
}
