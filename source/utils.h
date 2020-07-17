#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef UTILS_H
#define UTILS_H

#define BUFFER_SIZE	40

char* read_line(FILE* input_stream);
char* mudar_extensao(char* nome_arqv,char* nova_extensao);
#endif