#include <stdio.h>	// Autor: Gabriel Victor C.F.	BCC@ICMC/USP	July2020
#include <stdlib.h>
#include <string.h>

#ifndef UTILS_H
#define UTILS_H

#define BUFFER_SIZE	40	//Usado pela read_line()

char* read_line(FILE* input_stream);
char* mudar_extensao(char* nome_arqv,char* nova_extensao);
#endif