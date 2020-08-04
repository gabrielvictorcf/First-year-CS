#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#ifndef	_SGDB_H_
#define	_SGDB_H_

#define T_NOME_CAMPO 20

struct campo{
	char nome[T_NOME_CAMPO];
	short len;
	void (*print)(char*,void*);
	void (*inserir)(char*,FILE*,int);
};

struct indice{
	int chave;
	unsigned offset;
};

typedef struct BANCO_DADOS{
	char* nome_arqv;
	struct campo* chave;
	struct campo** campos;
	short qtd_campos;
	unsigned char* registros;
	short tam_registro;
	unsigned qtd_registros;
}banco_dados;

banco_dados* processar_metadados(FILE* arqv_dat);
void processar_insert(char* dados,banco_dados* banco,FILE* arqv_reg);
void indexar_banco(banco_dados* banco,FILE* reg);
void search_banco(banco_dados* banco,int chave,FILE* reg);
void free_banco(banco_dados* banco);
#endif