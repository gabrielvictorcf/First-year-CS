#include <stdio.h>	// Autor: Gabriel Victor C.F.	BCC@ICMC/USP	July2020
#include <stdlib.h>
#include <string.h>

#ifndef _BMP256FILE_H_
#define _BMP256FILE_H_

#define QTD_CORES 256	//Quantidade máxima do formato 256 cores
#define INTENSIDADE_MAX 255	//Intensidade maxima de cada RGB na paleta
#define	TAMANHO_PIXEL(bits)	(bits/8)
//Arredonda num, para cima, ao multiplo de 4 mais próximo
#define BMP_PAD(num) ( num = (num % 4)? num + (4 - num % 4): num)

struct cabecalhoarquivo{
	char assinatura[2];
	int t_arqv;
	int reservado;
	int deslocamento;
};

struct cabecalhobmp{
	int t_cabecalho;
	int largura_img;
	int altura_img;
	short n_planos;
	short bits_per_pxl;
	int compressao;
	int t_img;
	int pxl_per_metro_hor;
	int pxl_per_metro_ver;
	int n_cores_usadas;
	int n_cores_uteis;
};

//Armazena cor na mesma ordem achada no BMP
struct corBMP{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reservado;
};

struct paleta{
	struct corBMP* cores;
};

struct conteudoBMP256{
	unsigned char** pixels;
};

//Com todas estruturas anteriores temos o modelo da Imagem Bitmap 256-cores
typedef struct imagem_bmp256{
	struct cabecalhoarquivo cab_arqv;
	struct cabecalhobmp cab_bmp;
	struct paleta paleta;
	struct conteudoBMP256 conteudo;
}Img_BMP256;

Img_BMP256* gerar_imagem();
Img_BMP256* ler_arquivo_BMP256(FILE* arqv_BMP256,const char* nome_img);
void escrever_arquivo_BMP256(FILE* arqv_BMP256,Img_BMP256* img_origem);
void free_BMP256(Img_BMP256* img);
void print_BMP256_cabecalhos(Img_BMP256* imagem);
void print_BMP256_paleta(Img_BMP256* imagem);
void print_BMP256_soma_linhas(Img_BMP256* imagem);
#endif