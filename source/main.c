#include <stdio.h>
#include <stdlib.h>
#include "bmp256.h"
#include "filtro.h"
#include "utils.h"

int main(){
	char* nome_arqv = read_line(stdin);
	
	FILE* imagem_entrada = fopen(nome_arqv, "rb");
	if(!imagem_entrada){
		free(nome_arqv);
		printf("Erro no arquivo\n");
		return 0;
	}

	Img_BMP256* imagem_saida = ler_arquivo_BMP256(imagem_entrada,nome_arqv);
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