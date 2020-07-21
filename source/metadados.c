#include "metadados.h"

//As funcoes de print serao designadas
//ao ponteiro de funcao print de cada campo.
void print_string(char* nome,void* str){
	printf("%s: %s\n",nome,((char*)str));
};

void print_float(char* nome,void* fl){
	printf("%s: %.2f\n",nome,*(float*)fl);
};

void print_inteiro(char* nome,void* num){
	printf("%s: %d\n",nome,(*(int*)num));
};

void print_double(char* nome,void* dbl){
	printf("%s: %.2lf\n",nome,(*(double*)dbl));
};

void inserir_string(char* campo,FILE* arqv_reg,int t_campo){
	char* str = calloc(t_campo,1);
	char* str_aux = extrair_aspas(campo);
	str = strcpy(str,str_aux);
	fwrite(str,t_campo,1,arqv_reg);
	free(str);
	free(str_aux);
};

void inserir_int(char* campo,FILE* arqv_reg,int t_campo){
	int i_num = atoi(campo);
	fwrite(&i_num,sizeof(int),1,arqv_reg);
};

void inserir_float(char* campo,FILE* arqv_reg,int t_campo){
	float f_num = atof(campo);
	fwrite(&f_num,sizeof(float),1,arqv_reg);
};

void inserir_double(char* campo,FILE* arqv_reg,int t_campo){
	double d_num = atof(campo);
	fwrite(&d_num,sizeof(double),1,arqv_reg);
};

//Funcao aux que gera e classifica um campo lendo 2 linhas dum .dat
struct campo* gerar_campo(FILE* arqv_dat){
	char* linha = read_line(arqv_dat);
	if(!linha) return NULL;
	struct campo* novo_campo = malloc(1*sizeof(struct campo));

	char* str_aux = extrair_arg(linha,1);
	strncpy(novo_campo->nome,str_aux,T_NOME_CAMPO);
	free(linha);
	free(str_aux);


	linha = read_line(arqv_dat);
	str_aux = strchr(linha,' ');
	switch (str_aux[1]){
	case 'c':
		str_aux = extrair_colchetes(str_aux);
		novo_campo->len = atoi(str_aux);
		free(str_aux);
		novo_campo->print = print_string;
		novo_campo->inserir = inserir_string;
		break;
	case 'i':
		novo_campo->len = sizeof(int);
		novo_campo->print = print_inteiro;
		novo_campo->inserir = inserir_int;
		break;
	case 'f':
		novo_campo->len = sizeof(float);
		novo_campo->print = print_float;
		novo_campo->inserir = inserir_float;
		break;
	case 'd':
		novo_campo->len = sizeof(double);
		novo_campo->print = print_double;
		novo_campo->inserir = inserir_double;
		break;
	};
	free(linha);
	return novo_campo;	
}

//Essa funcao processa um .dat inteiramente e gera um banco
banco_dados* processar_metadados(FILE* arqv_dat){
	banco_dados* novo_banco = malloc(1*sizeof(banco_dados));
	novo_banco->registros = NULL;
	novo_banco->qtd_registros = 0;

	char* linha = read_line(arqv_dat);
	novo_banco->nome_arqv = extrair_arg(linha,1);
	free(linha);
	
	novo_banco->chave = gerar_campo(arqv_dat);
	novo_banco->tam_registro = novo_banco->chave->len;
	novo_banco->campos = NULL;
	int qtd = 0;
	struct campo* campo;
	while ((campo = gerar_campo(arqv_dat))){
		novo_banco->campos = realloc(novo_banco->campos, (qtd+1) * sizeof(struct campo*));
		novo_banco->campos[qtd] = malloc(sizeof(struct campo));
		memcpy(novo_banco->campos[qtd],campo,sizeof(struct campo));
		free(campo);
		novo_banco->tam_registro += novo_banco->campos[qtd]->len;
		qtd++;
	};
	free(campo);
	
	novo_banco->qtd_campos = qtd;
	return novo_banco;
}

void free_banco(banco_dados* banco){
	for (short i = 0; i < banco->qtd_campos; i++){
		free(banco->campos[i]);
	};
	free(banco->campos);
	free(banco->chave);
	free(banco->nome_arqv);
	free(banco->registros);
	free(banco);
}

void processar_insert(char* dados,banco_dados* banco,FILE* arqv_reg){
	fseek(arqv_reg,0,SEEK_END);
	char* campo = strtok(dados,",");
	int chave = atoi(campo);
	fwrite(&chave,sizeof(int),1,arqv_reg);

	for (short i = 0; i < banco->qtd_campos; i++){
		campo = strtok(NULL,",");
		int t_campo = banco->campos[i]->len;
		banco->campos[i]->inserir(campo,arqv_reg,t_campo);
	};
	banco->qtd_registros++;
}

int comp_indices(const void* n1,const void* n2){
	return (*(struct indice*)n1).chave - (*(struct indice*)n2).chave;
}

void carregar_registros(banco_dados* banco,FILE* reg){
	if(!reg) return;

	unsigned char* registros = malloc(banco->qtd_registros * banco->tam_registro);
	fseek(reg,0,SEEK_SET);
	fread(registros,banco->tam_registro,banco->qtd_registros,reg);

	banco->registros = registros;
}

void indexar_banco(banco_dados* banco,FILE* reg){
	if(!banco->registros) carregar_registros(banco,reg);
	
	struct indice* RRN = malloc(banco->qtd_registros * sizeof(struct indice));
	for (unsigned i = 0; i < banco->qtd_registros; i++){
		int shift = i * banco->tam_registro;
		memcpy(&RRN[i].chave,banco->registros+shift,sizeof(int));
		RRN[i].offset = shift;
	};
	
	qsort(RRN,banco->qtd_registros,sizeof(struct indice),comp_indices);

	char* idx_name = str_dup(banco->nome_arqv);
	idx_name = mudar_extensao(idx_name,".idx");
	FILE* idx = fopen(idx_name,"wb");
	free(idx_name);

	fwrite(RRN,sizeof(struct indice),banco->qtd_registros,idx);
	free(RRN);
	fclose(idx);
}

void print_registro(unsigned char* registro,banco_dados* banco){
	banco->chave->print(banco->chave->nome,registro);

	unsigned shift = banco->chave->len;
	for (short i = 0; i < banco->qtd_campos; i++){
		banco->campos[i]->print(banco->campos[i]->nome,registro+shift);
		shift += banco->campos[i]->len;
	};
	printf("\n");
}

void search_banco(banco_dados* banco,int chave,FILE* reg){
	char* nome_idx = str_dup(banco->nome_arqv);
	nome_idx = mudar_extensao(nome_idx,".idx");

	FILE* idx = fopen(nome_idx,"r");
	if(!idx){
		indexar_banco(banco,reg);
		idx = fopen(nome_idx,"r");
	};
	free(nome_idx);

	struct indice* RRN = malloc(banco->qtd_registros * sizeof(struct indice));
	fread(RRN,sizeof(struct indice),banco->qtd_registros,idx);
	fclose(idx);

	struct indice* ind_procurado = bsearch(&chave,RRN,banco->qtd_registros,sizeof(struct indice),comp_indices);
	if(!ind_procurado){
		free(RRN);
	 	return;
	};

	fseek(reg,ind_procurado->offset,SEEK_SET);
	unsigned char* registro = calloc(1,banco->tam_registro);
	fread(registro,banco->tam_registro,1,reg);
	print_registro(registro,banco);
	free(registro);
	free(RRN);
}