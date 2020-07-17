#include "utils.h"	// Autor: Gabriel Victor C.F.	BCC@ICMC/USP	July2020

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

//Usada para renomear o arquivo a partir do ".". Deve conter a extensao
char* mudar_extensao(char* nome_arqv,char* nova_extensao){
	int t_extensao = strlen(nova_extensao) - 4;	//nao precisamos allocar o .xxx
	nome_arqv = realloc(nome_arqv,strlen(nome_arqv) + t_extensao + 1);
	char* extensao = strrchr(nome_arqv,'.');
	strcpy(extensao,nova_extensao);
	return nome_arqv;
}