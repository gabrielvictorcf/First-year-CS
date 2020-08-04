#include "bmp256.h"	// Autor: Gabriel Victor C.F.	BCC@ICMC/USP	July2020

//Alloca e retorna uma struct Img_BMP256
Img_BMP256* gerar_imagem(){
	Img_BMP256* nova_img = malloc(1 * sizeof(Img_BMP256));
	nova_img->paleta.cores = malloc(QTD_CORES * sizeof(struct corBMP));
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

//Copia memoria do buffer para o campo. Retorna o deslocamento
//percorrido no buffer, para leitura sequencial
int ler_campo(void* campo,int qntd,const void* buffer_origem){
	memcpy(campo,buffer_origem,qntd);
	return qntd;
}

//Auxiliar: le o cabecalho de um arquivo e armazena na estrutura da Imagem
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

//Auxiliar: le o cabecalho bmp de um arquivo e armazena na estrutura da Imagem
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

//Auxiliar: le a paleta no arquivo e armazena na estrutura da Imagem
void get_paleta(FILE* img_origem,Img_BMP256* img_dest){
	struct paleta* paleta = &img_dest->paleta;

	int shift_lido = 0, t_cor = sizeof(struct corBMP);
	unsigned char* bytes_lidos = malloc(QTD_CORES * t_cor);
	fread(bytes_lidos,1, QTD_CORES*t_cor,img_origem);

	for (int i = 0; i < QTD_CORES; i++){
		shift_lido += ler_campo(&paleta->cores[i],t_cor,bytes_lidos+shift_lido);
	};
	free(bytes_lidos);
}

//Auxiliar: alloca memoria para o conteudo da estrutura Imagem
void gerar_conteudo(Img_BMP256* img,int altura,int largura){
	img->conteudo.pixels = malloc(altura * sizeof(unsigned char*));
	for (int i = 0; i < altura; i++){
		img->conteudo.pixels[i] = malloc(largura);
	};
}

//Auxiliar: le a area de dados do arquivo e armazena na estrutura da Imagem
void get_conteudo(FILE* img_origem,Img_BMP256* img_dest){
	int altura_img = img_dest->cab_bmp.altura_img, largura_img = img_dest->cab_bmp.largura_img;
	BMP_PAD(largura_img);
	gerar_conteudo(img_dest,altura_img,largura_img);
	int tam_pixel = TAMANHO_PIXEL(img_dest->cab_bmp.bits_per_pxl);
	int largura_linha = tam_pixel*largura_img;
	struct conteudoBMP256* conteudo = &img_dest->conteudo;

	int shift_lido = 0;
	unsigned char* bytes_lidos = malloc(img_dest->cab_bmp.t_img);
	fread(bytes_lidos,1,img_dest->cab_bmp.t_img,img_origem);

	for (int i = 0; i < altura_img; i++){
		shift_lido += ler_campo(conteudo->pixels[i],largura_linha,bytes_lidos+shift_lido);
	};
	free(bytes_lidos);
}

//Funcao que le um arquivo e retorna uma estrutura com seus dados.
//Retorna NULL caso arquivo nao seja bitmap
Img_BMP256* ler_arquivo_BMP256(FILE* arqv_BMP256,const char* nome_img){
	Img_BMP256* img_saida = gerar_imagem();
	get_cabecalho_arqv(arqv_BMP256,img_saida);

	char* extensao = strrchr(nome_img,'.');
	char* assinatura = img_saida->cab_arqv.assinatura;
	if(!strcmp(extensao,".bmp") && !strncmp(assinatura,"BM",2)){
		get_cabecalho_bmp(arqv_BMP256,img_saida);
		get_paleta(arqv_BMP256,img_saida);
		get_conteudo(arqv_BMP256,img_saida);
	}else{
		printf("Arquivo nao eh do formato BMP\n");
		free(img_saida->paleta.cores);
		free(img_saida);
		return NULL;
	};
	return img_saida;
}

//Auxiliar: escreve o cabecalho de arquivo da estrutura em um arquivo
void write_cabecalho_arqv(FILE* nova_img,Img_BMP256* img_fonte){
	fwrite(&img_fonte->cab_arqv.assinatura,1,2,nova_img);
	fwrite(&img_fonte->cab_arqv.t_arqv,1,4,nova_img);
	fwrite(&img_fonte->cab_arqv.reservado,1,4,nova_img);
	fwrite(&img_fonte->cab_arqv.deslocamento,1,4,nova_img);
}

//Auxiliar: escreve o cabecalho bmp da estrutura em um arquivo
void write_cabecalho_bmp(FILE* nova_img,Img_BMP256* img_fonte){
	fwrite(&img_fonte->cab_bmp.t_cabecalho,1,4,nova_img);
	fwrite(&img_fonte->cab_bmp.largura_img,1,4,nova_img);
	fwrite(&img_fonte->cab_bmp.altura_img,1,4,nova_img);
	fwrite(&img_fonte->cab_bmp.n_planos,1,2,nova_img);
	fwrite(&img_fonte->cab_bmp.bits_per_pxl,1,2,nova_img);
	fwrite(&img_fonte->cab_bmp.compressao,1,4,nova_img);
	fwrite(&img_fonte->cab_bmp.t_img,1,4,nova_img);
	fwrite(&img_fonte->cab_bmp.pxl_per_metro_hor,1,4,nova_img);
	fwrite(&img_fonte->cab_bmp.pxl_per_metro_ver,1,4,nova_img);
	fwrite(&img_fonte->cab_bmp.n_cores_usadas,1,4,nova_img);
	fwrite(&img_fonte->cab_bmp.n_cores_uteis,1,4,nova_img);
}

//Auxiliar: escreve a paleta da estrutura em um arquivo
void write_paleta(FILE* nova_img,Img_BMP256* img_fonte){
	int t_cor = sizeof(struct corBMP);
	fwrite(img_fonte->paleta.cores,1,QTD_CORES * t_cor,nova_img);
}

//Auxiliar: escreve a area de dados da estrutura em um arquivo
void write_conteudo(FILE* nova_img,Img_BMP256* img_fonte){
	int altura = img_fonte->cab_bmp.altura_img;
	int largura = img_fonte->cab_bmp.largura_img;
	BMP_PAD(largura);
	for (int i = 0; i < altura; i++){
		fwrite(img_fonte->conteudo.pixels[i],1,largura,nova_img);
	};
}

//Funcao que escreve uma estrutura, ja processada, em um arquivo
void escrever_arquivo_BMP256(FILE* arqv_BMP256,Img_BMP256* img_origem){
	write_cabecalho_arqv(arqv_BMP256,img_origem);
	write_cabecalho_bmp(arqv_BMP256,img_origem);
	write_paleta(arqv_BMP256,img_origem);
	write_conteudo(arqv_BMP256,img_origem);
}

//Funcao que imprime campos de ambos cabecalhos
void print_BMP256_cabecalhos(Img_BMP256* imagem){
	struct cabecalhoarquivo cab_arqv = imagem->cab_arqv;
	struct cabecalhobmp cab_bmp = imagem->cab_bmp;
	printf("CABECALHO:\n");
	printf("Iniciais: %c%c\n", cab_arqv.assinatura[0],cab_arqv.assinatura[1]);
	printf("Tamanho do arquivo: %d\n", cab_arqv.t_arqv);
	printf("Reservado: %d\n", cab_arqv.reservado);
	printf("Deslocamento, em bytes, para o inicio da area de dados: %d\n"
		,cab_arqv.deslocamento);

	printf("Tamanho em bytes do segundo cabecalho: %d\n", cab_bmp.t_cabecalho);
	printf("Resolucao: %d x %d\n", cab_bmp.largura_img, cab_bmp.altura_img);
	printf("Numero de planos: %d\n", cab_bmp.n_planos);
	printf("Bits por pixel: %d\n", cab_bmp.bits_per_pxl);
	printf("Compressao usada: %d\n", cab_bmp.compressao);
	printf("Tamanho imagem: %d\n", cab_bmp.t_img);
	printf("Resolucao horizontal: %d pixel por metro\n", cab_bmp.pxl_per_metro_hor);
	printf("Resolucao Vertical: %d pixel por metro\n", cab_bmp.pxl_per_metro_ver);
	printf("Numero de cores usadas: %d\n", cab_bmp.n_cores_usadas);
	printf("Numero de cores importantes: %d\n", cab_bmp.n_cores_uteis);
}

//Funcao que imprime a paleta e suas cores
void print_BMP256_paleta(Img_BMP256* imagem){
	struct corBMP* cores = imagem->paleta.cores;
	for (int i = 0; i < QTD_CORES; i++){
		printf("Paleta[%d]: R:%d G:%d B:%d\n",
		i,
		cores[i].red,
		cores[i].green,
		cores[i].blue);
	};
}

//Funcao que imprime a soma dos valores em cada linha dos dados
void print_BMP256_soma_linhas(Img_BMP256* imagem){
	int altura  = imagem->cab_bmp.altura_img;
	int largura = imagem->cab_bmp.largura_img;
	int largura_padded = largura;
	BMP_PAD(largura_padded);
	int diff = largura_padded - largura;

	long long soma;
	int linha_atual = 0;
	for (int i = altura-1; i >= 0; i--,linha_atual++){
		soma  = 0;
		for (int j = 0; j < largura; j++){
			soma += imagem->conteudo.pixels[i][j];
		};
		soma = soma - diff;
		printf("Soma linha %d: %lld\n", linha_atual, soma);
	};
}