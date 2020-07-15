#include "utils.h"
#define BUFFER_SIZE	40

char* read_line(FILE* input_stream){
	char* line = NULL;
	int position = 0;

	do{
		if(position % BUFFER_SIZE == 0){
			line = realloc(line, ((position/BUFFER_SIZE + 1) * BUFFER_SIZE ));
		};
		line[position] = fgetc(input_stream);
		if (line[position] == '\r'){
			position++;
			fseek(input_stream,1,SEEK_CUR);
			break;
		};
	} while(line[position++] != '\n' && !feof(input_stream));
	line[position - 1] = '\0';
	line = realloc(line, position);
	return line;
}