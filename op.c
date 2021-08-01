#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_opcode (const char *src, int size)
{
	unsigned char *usrc = (unsigned char *) src;
	
	int op  = 0;	
	for(int i = size; i > 0; i --) {
		if(*usrc++ == '1')
			switch(i){ 
				case 8:
					op += 128;
				break;
				case 7:
					op += 64;
				break;
				case 6:
					op += 32;
				break;
				case 5:
					op += 16;
				break;
				case 4:
					op += 8;
				break;
				case 3:
					op += 4;
				break;
				case 2:
					op += 2;
				break;
				case 1:
					op += 1;
				break;
				default:
				
				break;
			}
			
	}
	
	return op;
}


int main(int argc, char *argv[]) 
{
	printf("Community Dev Angola\n");
	
	if(argc < 2) {
		printf("missing arguments\n");
		return 0;
	}
	
	FILE *input = fopen(argv[1], "rb");
	if(!input) {
		printf("error: file input not found\n");
		return 0;
	}
	
	fseek(input,0,SEEK_END);
	int size = ftell(input);
	rewind(input);
	
	if(size > 0x10000) {
		printf("error: file input > 64 KiB\n");
		fclose(input);
		return 0;
	}
		
	char *source = (char*) malloc(size + 1);
	unsigned char opcode = 0;
	int bytes = 0;
	int eof = 0;

	memset(source, 0, size + 1);
	if( fread(source, 1, size, input) !=  size ) {
		printf("error: read file input\n");
		fclose(input);
		return 0;
	}
	
	FILE *output = fopen(argv[2], "w+b");
	if(!output) {
		printf("error: create file output\n");
		fclose(input);
		return 0;
	}
	
	source = strtok(source, " \n");
	for ( ; ; ) {
	
		if(!source) {

		 	break;
		}
		
		opcode = get_opcode (source, 8);
		printf("%x ", opcode);
		if( fwrite(&opcode, 1, 1, output) != 1) break;
		bytes ++;
		
		eof += strlen(source);
		if(eof > size ) break;
	
		source = strtok(0, " \n");
	
	}
		
	// fill 512 bytes
	opcode = 0;
	for(int i=bytes; i < 510; i++){
		if( fwrite(&opcode, 1, 1, output) != 1) break;
	}
	
	
	opcode = 0x55;
	fwrite(&opcode, 1, 1, output);
		
	opcode = 0xaa;
	fwrite(&opcode, 1, 1, output);

	printf("\nsucessfull: %d bytes\n", bytes);
	fflush(output);
	fclose(input);
	fclose(output);
	return 0;
}
