#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _SGDB_UTILS_
#define _SGDB_UTILS_

#define BUFFER_SIZE 256

char* read_line(FILE* input_stream);
char* str_dup(char* str_fonte);
int pegar_quantidade_args(char* line);
char* extrair_arg(char* fonte,int n_arg);
char* extrair_colchetes(char* str);
char* extrair_aspas(char* str);
char* mudar_extensao(char* nome_arqv,char* nova_extensao);
#endif