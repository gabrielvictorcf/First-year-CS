#include "utils.h"

//Leitura dinamica de arquivo, retorna NULL caso a linha seja vazia
char* read_line(FILE* input_stream){
	char* line = NULL;
	int pos = 0;

	do{
		if(pos % BUFFER_SIZE == 0){
			line = realloc(line, ((pos/BUFFER_SIZE + 1) * BUFFER_SIZE ));
		};
		line[pos] = fgetc(input_stream);
		if (line[pos] == '\r'){
			pos++;
			fseek(input_stream,1,SEEK_CUR);
			break;
		};
	} while(line[pos++] != '\n' && !feof(input_stream));
	if(pos == 1){
		free(line);
		return NULL;
	};

	line[pos - 1] = '\0';
	line = realloc(line, pos);
	return line;
}

//Retorna uma nova string identica a fonte
char* str_dup(char* str_fonte){
	char* nova_str = malloc(strlen(str_fonte) + 1);
	return strcpy(nova_str,str_fonte);
}

//Retorna a quantidade de argumentos/palavras numa string
int pegar_quantidade_args(char* line){
	int pos = 0, args = 0;
	do{
		if(line[pos] == ' ' && line[pos + 1] > 32 ) args++;
	} while (line[++pos] != '\0');
	return args + 1;
}

//Retorna uma string com o argumento desejado
char* extrair_arg(char* fonte,int n_arg){
	char* pos = fonte;
	while (n_arg > 0){
		if(*pos == ' ' && *(pos+1) > 32)n_arg--;
		pos++;
	};
	int n_chars = strcspn(pos," \n\r");
	
	char* arg = malloc(n_chars+1);
	arg = strncpy(arg,pos,n_chars);
	arg[n_chars] = '\0';
	return arg;
}

//Extrai os dados entre [] e os retorna como uma nova string
char* extrair_colchetes(char* str){
	char* str_aux = strrchr(str,'[');
	char* dado = str_dup(str_aux+1);
	int c_pos = strcspn(dado,"]");
	dado[c_pos] = '\0';
	return dado;
}

//Extrai os dados entre aspas e os retorna como uma nova string
char* extrair_aspas(char* str){
	char* dado = strchr(str,'\"');
	dado = str_dup(dado+1);
	int a_pos = strcspn(dado,"\"");
	dado[a_pos] = '\0';
	return dado;
}

//Usada para renomear o arquivo a partir do ".". Deve conter a extensao
char* mudar_extensao(char* nome_arqv,char* nova_extensao){
	nome_arqv = realloc(nome_arqv,strlen(nome_arqv) + strlen(nova_extensao) - 3);
	char* extensao = strrchr(nome_arqv,'.');
	strcpy(extensao,nova_extensao);
	return nome_arqv;
}