#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _BMP256FILE_H_
#define _BMP256FILE_H_

#define QTD_CORES 256
#define	TAMANHO_PIXEL(bits)	(bits/8)

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

struct corBMP{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char reservado;
};

struct paleta{
	struct corBMP* cores;
};

struct conteudoBMP256{
	unsigned char** pixels;
};

typedef struct imagem_bmp256{
	struct cabecalhoarquivo cab_arqv;
	struct cabecalhobmp cab_bmp;
	struct paleta paleta;
	struct conteudoBMP256 conteudo;
}Img_BMP256;

Img_BMP256* gerar_imagem();
Img_BMP256* ler_arquivo_BMP256(const FILE* arqv_BMP256,const char* nome_img);
void free_BMP256(Img_BMP256* img);
void get_cabecalho_arqv(FILE* img_origem,Img_BMP256* img_dest);
void get_cabecalho_bmp(FILE* img_origem,Img_BMP256* img_dest);
void get_paleta(FILE* img_origem,Img_BMP256* img_dest);
void get_conteudo(FILE* img_origem,Img_BMP256* img_dest);
#endif