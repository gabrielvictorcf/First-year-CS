#include "bmp256.h"

#ifndef _FILTROSBMP_H_
#define _FILTROSBMP_H_

#define FRMLA_NEGATIVO(cor) (INTENSIDADE_MAX - cor)
#define FMRLA_PRETOBRANCO(azul,verde,vermelho)  ( (azul+vermelho+verde) / 3)

void filtro_negativo(Img_BMP256* imagem);
void filtro_PB(Img_BMP256* imagem);
#endif