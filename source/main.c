#include <stdio.h>
#include <stdlib.h>
#include "metadados.h"

int main(int argc, char const *argv[]){
	char* nome_dat = read_line(stdin);
	FILE* dat = fopen(nome_dat,"r");
	free(nome_dat);
	if(!dat) return 0;

	banco_dados* meu_banco = processar_metadados(dat);
	fclose(dat);

	FILE* reg = fopen(meu_banco->nome_arqv,"wb+");
	char* ln_atual = read_line(stdin);
	char* comando = extrair_arg(ln_atual,0);
	while (strcmp(comando,"exit")){
		char* dados = strchr(ln_atual,' ');
		if(!strcmp(comando,"insert")){
			processar_insert(dados,meu_banco,reg);
		}
		else if(!strcmp(comando,"index")){
			indexar_banco(meu_banco,reg);
		}
		else if(!strcmp(comando,"search")){
			int chave = atoi(dados);
			search_banco(meu_banco,chave,reg);
		};
		free(ln_atual);
		free(comando);
		ln_atual = read_line(stdin);
		comando = extrair_arg(ln_atual,0);
	}

	free(ln_atual);
	free(comando);
	free_banco(meu_banco);
	fclose(reg);
	return 0;
}