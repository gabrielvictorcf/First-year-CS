#include "file.h"

Img_BMP256* gerar_imagem(){
	Img_BMP256* nova_img = malloc(1 * sizeof(Img_BMP256));
	nova_img->paleta.cores = malloc(256 * sizeof(struct corBMP));
	nova_img->nome = malloc(40);
	return nova_img;
}

void free_BMP256(Img_BMP256* img){
	//lembrar de dar free no conteudo!
	
	free(img->nome);
	free(img->paleta.cores);
	free(img);
}

void get_cabecalho_arqv(FILE* img_origem,Img_BMP256* img_dest){
	struct cabecalhoarquivo* cabecalho = &img_dest->cab_arqv;
	unsigned char* bytes_lidos;
	int shift_lido = 0;
	fread(bytes_lidos,1,54,img_origem);

	memcpy(cabecalho->assinatura,bytes_lidos,2);
	shift_lido += 2;
	memcpy(cabecalho->assinatura,bytes_lidos+shift_lido,4);
	shift_lido += 4;
	memcpy(cabecalho->assinatura,bytes_lidos+shift_lido,4);
}

void get_cabecalho_bmp(FILE* img_origem,Img_BMP256* img_dest){
	struct cabecalhobmp* cabecalho = &img_dest->cab_bmp;
	unsigned char* bytes_lidos;
	int shift_lido = 0;
	fread(bytes_lidos,1,54,img_origem);

	memcpy(cabecalho->t_cabecalho,bytes_lidos,4);
	shift_lido += 4;
	memcpy(cabecalho->largura_img,bytes_lidos,4);
	shift_lido += 4;
	memcpy(cabecalho->altura_img,bytes_lidos,4);
	shift_lido += 4;
	memcpy(cabecalho->n_planos,bytes_lidos,2);
	shift_lido += 2;
	memcpy(cabecalho->bits_per_pxl,bytes_lidos,2);
	shift_lido += 2;
	memcpy(cabecalho->compressao,bytes_lidos,4);
	shift_lido += 4;
	memcpy(cabecalho->t_img,bytes_lidos,4);
	shift_lido += 4;
	memcpy(cabecalho->pxl_per_metro_hor,bytes_lidos,4);
	shift_lido += 4;
	memcpy(cabecalho->pxl_per_metro_ver,bytes_lidos,4);
	shift_lido += 4;
	memcpy(cabecalho->n_cores_usadas,bytes_lidos,4);
	shift_lido += 4;
	memcpy(cabecalho->n_cores_uteis,bytes_lidos,4);
}