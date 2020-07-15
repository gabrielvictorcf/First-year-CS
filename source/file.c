#include "file.h"

Img_BMP256* gerar_imagem(){
	Img_BMP256* nova_img = malloc(1 * sizeof(Img_BMP256));
	nova_img->paleta.cores = malloc(256 * sizeof(struct corBMP));
	return nova_img;
}

void free_BMP256(Img_BMP256* img){
	int altura = img->cab_bmp.altura_img;

	for (int i = 0; i < altura; i++){
		free(img->conteudo.pixels[i]);
	}
	free(img->conteudo.pixels);
	
	free(img->paleta.cores);
	free(img);
}

int ler_campo(void* campo,int qntd,const void* buffer_origem){
	memcpy(campo,buffer_origem,qntd);
	return qntd;
}

void get_cabecalho_arqv(FILE* img_origem,Img_BMP256* img_dest){
	struct cabecalhoarquivo* cabecalho = &img_dest->cab_arqv;
	unsigned char* bytes_lidos = malloc(14);
	int shift_lido = 0;
	fread(bytes_lidos,1,14,img_origem);

	shift_lido += ler_campo(cabecalho->assinatura,2,bytes_lidos);
	shift_lido += ler_campo(&cabecalho->t_arqv,4,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->reservado,4,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->deslocamento,4,bytes_lidos+shift_lido);
	free(bytes_lidos);
}

void get_cabecalho_bmp(FILE* img_origem,Img_BMP256* img_dest){
	struct cabecalhobmp* cabecalho = &img_dest->cab_bmp;
	unsigned char* bytes_lidos = malloc(40);
	int shift_lido = 0;
	fread(bytes_lidos,1,40,img_origem);

	shift_lido += ler_campo(&cabecalho->t_cabecalho,4,bytes_lidos);
	shift_lido += ler_campo(&cabecalho->largura_img,4,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->altura_img,4,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->n_planos,2,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->bits_per_pxl,2,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->compressao,4,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->t_img,4,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->pxl_per_metro_hor,4,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->pxl_per_metro_ver,4,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->n_cores_usadas,4,bytes_lidos+shift_lido);
	shift_lido += ler_campo(&cabecalho->n_cores_uteis,4,bytes_lidos+shift_lido);
	free(bytes_lidos);
}

void get_paleta(FILE* img_origem,Img_BMP256* img_dest){
	struct paleta* paleta = &img_dest->paleta;
	
	int shift_lido = 0, t_cor = sizeof(struct corBMP);
	unsigned char* bytes_lidos = malloc(256 * t_cor);
	fread(bytes_lidos,1, 256*t_cor,img_origem);

	for (int i = 0; i < 256; i++){
		shift_lido += ler_campo(&paleta->cores[i],t_cor,bytes_lidos+shift_lido);
	};
	free(bytes_lidos);
}

void gerar_conteudo(Img_BMP256* img,int altura,int largura){
	img->conteudo.pixels = malloc(altura * sizeof(unsigned char*));
	for (int i = 0; i < altura; i++){
		img->conteudo.pixels[i] = malloc(largura);
	};
}

void get_conteudo(FILE* img_origem,Img_BMP256* img_dest){
	int altura_img = img_dest->cab_bmp.altura_img, largura_img = img_dest->cab_bmp.largura_img;
	gerar_conteudo(img_dest,altura_img,largura_img);
	int qntd_pixels = altura_img * largura_img;
	int tam_pixel = img_dest->cab_bmp.bits_per_pxl / 8;
	int largura_linha = tam_pixel*largura_img;
	struct conteudoBMP256* conteudo = &img_dest->conteudo;

	int shift_lido = 0;
	unsigned char* bytes_lidos = malloc(qntd_pixels);
	fread(bytes_lidos,1,qntd_pixels,img_origem);
	
	for (int i = 0; i < altura_img; i++){
		shift_lido += ler_campo(conteudo->pixels[i],largura_linha,bytes_lidos+shift_lido);
	};
	free(bytes_lidos);
}