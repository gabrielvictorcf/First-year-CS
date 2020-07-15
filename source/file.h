#include <stdio.h>
#include <stdlib.h>

#ifndef _BMP256FILE_H_
#define _BMP256FILE_H_

struct cabecalhoarquivo{
	char assinatura[3];
	int t_arqv;
	int reservado;
	int deslocamento;
};

struct cabecalhobmp{
	int t_cabecalho;
	int largura_img;
	int altura_img;
	int n_planos;
	int bits_per_pxl;
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
	corBMP* cores;
};

struct conteudoBMP256{
	unsigned char** cor;
};

#endif