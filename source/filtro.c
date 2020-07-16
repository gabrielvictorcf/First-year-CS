#include "filtro.h"

void filtro_negativo(Img_BMP256* imagem){
	struct corBMP* corBMP = imagem->paleta.cores;
	for (int i = 0; i < QTD_CORES; i++){
		corBMP[i].blue = FRMLA_NEGATIVO(corBMP[i].blue);
		corBMP[i].green = FRMLA_NEGATIVO(corBMP[i].green);
		corBMP[i].red = FRMLA_NEGATIVO(corBMP[i].red);
	};
}

void filtro_PB(Img_BMP256* imagem){
	struct corBMP* cor = imagem->paleta.cores;
	for (int i = 0; i < QTD_CORES; i++){
		int tom_cinza = FMRLA_PRETOBRANCO(cor[i].blue,cor[i].green,cor[i].red);
		cor[i].blue = tom_cinza;
		cor[i].green = tom_cinza;
		cor[i].red = tom_cinza;
	};
}