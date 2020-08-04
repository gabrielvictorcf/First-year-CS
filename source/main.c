#include <stdio.h>	// Programa para aplicar filtros à imagens no formato BMP 256 cores - Trabalho 11 Filtro de Foto
#include <stdlib.h> // Autor: Gabriel Victor C.F.   BCC@ICMC/USP	July2020
#include "bmp256.h"	// <---	Contem a estrutura do arquivo e funcoes para leitura, escritura e printing
#include "filtro.h"	// <---	Funcoes que processam uma paleta para aplicar um filtro
#include "utils.h"	// <---	Funcoes auxiliares de strings

int main(){
	char* nome_arqv = read_line(stdin);

	FILE* imagem_entrada = fopen(nome_arqv, "rb");
	if(!imagem_entrada){
		free(nome_arqv);
		printf("Erro no arquivo\n");
		return 0;
	}

	Img_BMP256* imagem_saida = ler_arquivo_BMP256(imagem_entrada,nome_arqv);
	//Com esse if tratamos todos erros restantes
	if(imagem_saida){
		int opcao;
		scanf("%d",&opcao);

		print_BMP256_cabecalhos(imagem_saida);
		printf("PALETA ORIGINAL:\n");
		print_BMP256_paleta(imagem_saida);

		switch (opcao){
		case 1:
			filtro_negativo(imagem_saida);
			nome_arqv = mudar_extensao(nome_arqv,"Negativo.bmp");
			break;
		case 2:
			filtro_PB(imagem_saida);
			nome_arqv = mudar_extensao(nome_arqv,"PretoBranco.bmp");
			break;
		};

		printf("PALETA NOVA:\n");
		print_BMP256_paleta(imagem_saida);
		print_BMP256_soma_linhas(imagem_saida);
		printf("%s\n",nome_arqv);

		FILE* img_resultado = fopen(nome_arqv, "wb");
		escrever_arquivo_BMP256(img_resultado,imagem_saida);
		fclose(img_resultado);
		free_BMP256(imagem_saida);
	}

	fclose(imagem_entrada);
	free(nome_arqv);
	return 0;
}