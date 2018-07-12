#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define N_BYTES 54
#define TAM_CHAVEP 10
#define LAST_IP "fim indices"
#define TRUE 1
#define FALSE 0

typedef struct IndiceP
{
	char key[9];
	long int byte_offset;
} IndiceP;

typedef struct TabelaIndiceP 
{
	IndiceP *vet_indiceP;
	int tamanho;
} TabelaIndiceP;

typedef struct BTree* BTreeNode;

typedef struct BTree
{
	char **chave;
	BTreeNode *filhos;
	BTreeNode pai;
	int num_chaves;
	int ordem; 
} BTree;

int HeadIndiceP(TabelaIndiceP *ind_P);
int TailIndiceP(TabelaIndiceP *ind_P);

TabelaIndiceP* CriaIndicesPrimarios();
void MostraIndicesPrimarios(TabelaIndiceP *ind_P);
void CriaIndicesPrimariosArq(TabelaIndiceP *ind_P);
char* PegaChave(TabelaIndiceP *ind_P, int index);

void OrdenaIndicesPrimarios(TabelaIndiceP *ind_P, int esquerda, int direita);
char *PegaRegistroPrimario(FILE *fp, long int byte_offset);
int BuscaRegistroPrimario(TabelaIndiceP *ind_P, char *chave_primaria);
int BuscaRegistroPrimarioArq(FILE *fp, TabelaIndiceP *ind_P, char *chave_primaria);
void InclusaoRegistroPrimario(TabelaIndiceP *ind_P, char *registro);
int EhNumero(char* String);

BTreeNode CriaArvoreB(int ordem);
BTreeNode CriaPagina(int ordem);
int LimpaArvoreB(BTreeNode arvB);
BTreeNode CriaFilho(char* chave, int ordem);
BTreeNode CriaArvBIndicePrimario(TabelaIndiceP *ind_P, int ordem);
BTreeNode BuscaArvB(BTreeNode raiz, int *seeks ,char* chave);
char *InsereArvB1(BTreeNode raiz, char* chave);
BTreeNode InsereArvB2(BTreeNode raiz, char *chave);
BTreeNode buscaDir(BTreeNode raiz, char *chave);
void inOrder(BTreeNode raiz);
void inOrder2(BTreeNode raiz);
int InserePag(BTreeNode local, char* chave);

void EscreveArvBArq(FILE *fp, BTreeNode arvB);
BTreeNode LeituraArvBArq(FILE *fp, BTreeNode arvB);
void LimpaPagina(BTreeNode local);

void menu();

int HeadIndiceP(TabelaIndiceP *ind_P) { return (ind_P == NULL) ? -1 : 0; }
//Há ind->tamanho elementos, de 0 até ind->tamanho -1, incluindo LAST_IP que eh o ind->tamanho - 1 o ultimo elemento !
int TailIndiceP(TabelaIndiceP *ind_P) { return (ind_P == NULL) ? -1 : ind_P->tamanho - 1; }

/* Cria um indice a partir de um arquivo do modo especificado:      *
 * MATRIC       NOME                      OP   CURSO  TURMA         *                    
 * 000000000    XXXXXXX 				  00    XX      X           *
 * com uma chave primaria que e a concatenacao dos campos matricula *
 * e nome, e já ordenado.                                           */
TabelaIndiceP *CriaIndicesPrimarios()
{
	FILE *fp;
	fp = fopen("lista.txt", "r+");

	char stringAUX[64];      // String auxiliar para ler as linhas do arquivo
	char chave[10], *aux;          // servira para adotarmos a chave primaria de cada registro
	long int byte_offset = 0;
	int tam_indice = 1;        // Tamanho do indice atual 
	
	TabelaIndiceP *CP = (TabelaIndiceP *) malloc(sizeof(TabelaIndiceP));    // Tabela de indices de Chaves Primarias
	CP->vet_indiceP = (IndiceP *) malloc(sizeof(IndiceP));
	CP->tamanho = 0;
	
	while(fscanf(fp,"%[^\n]\n", stringAUX) > 0){
		// Adquiri e concatena os campos para fazer a chave primaria
		aux = strtok(stringAUX, " ");
		chave[0] = aux[0];
		chave[1] = aux[1] - 32;
		chave[2] = aux[2] - 32;
		chave[3] = '\0';

		do{
			aux = strtok(NULL, " ");
		}while(aux != NULL && EhNumero(aux) == 0);
		strcat(chave, aux);
		chave[9] = '\0';

		// Atribui os valores ao indice
		CP->vet_indiceP[tam_indice-1].byte_offset = byte_offset;
		strcpy(CP->vet_indiceP[tam_indice-1].key, chave);
		CP->tamanho++;
		tam_indice++; 

		// Aloca mais um espaco para o proximo registro no indice 
		CP->vet_indiceP = (IndiceP *) realloc(CP->vet_indiceP, sizeof(IndiceP) * tam_indice);
		// Calcula o byte_offset do prox elemento
		byte_offset = byte_offset + N_BYTES;

	}
	// Um espaco é alocado a mais para indicar o final...
	strcpy(CP->vet_indiceP[tam_indice-1].key, LAST_IP);
	CP->vet_indiceP[tam_indice-1].byte_offset = 0;
	CP->tamanho = tam_indice;

	// Ordena os indices primarios
	OrdenaIndicesPrimarios(CP, HeadIndiceP(CP), TailIndiceP(CP));

	fclose(fp);

	return CP;
}

// Imprime os indices primarios ordenados
void MostraIndicesPrimarios(TabelaIndiceP *ind_P){
	int i=0;

	while(strcmp(ind_P->vet_indiceP[i].key, LAST_IP) != 0){
		if(ind_P->vet_indiceP[i].key[0] == '*'){
			i++;
			continue;
		}
		printf("%s --- %10ld\n", ind_P->vet_indiceP[i].key, ind_P->vet_indiceP[i].byte_offset);
		i++;
	}
	printf("%s\n", ind_P->vet_indiceP[i].key);
}
// Cria um arquivo com todos os indices primarios 
void CriaIndicesPrimariosArq(TabelaIndiceP *ind_P){
	FILE *fp = fopen("lista.txt", "w");

	for(int i = 0 ; i < TailIndiceP(ind_P) ; i++){
		if(ind_P->vet_indiceP[i].key[0] == '*'){
			i++;
			continue;
		}
		fprintf(fp, "%s       %10ld\n", ind_P->vet_indiceP[i].key, ind_P->vet_indiceP[i]. byte_offset);
	}

	fclose(fp);
}

// Ordenacao do indice primario utilizando o mecanismmo de quicksort recursivo.
void OrdenaIndicesPrimarios(TabelaIndiceP *ind_P, int esquerda, int direita)
{
	if(direita >  ind_P->tamanho){
		printf("Tamanho inadequado.\n");
		return;
	}
	int i = esquerda, j = direita;
	char pivo[9];
	strcpy(pivo, ind_P->vet_indiceP[esquerda].key);
	IndiceP temp;
	
	while(j >= i){
		while(strcmp(ind_P->vet_indiceP[i].key, pivo) < 0){
			i++;
		}
		while(strcmp(ind_P->vet_indiceP[j].key, pivo) > 0){
			j--;
		}
		if(j >= i){
			temp = ind_P->vet_indiceP[i];
			ind_P->vet_indiceP[i] = ind_P->vet_indiceP[j];
			ind_P->vet_indiceP[j] = temp;
			i++;
			j--;
		}
	};
	if(j > esquerda){
		OrdenaIndicesPrimarios(ind_P, esquerda, j);
	}
	if(i < direita){
		OrdenaIndicesPrimarios(ind_P, i, direita);
	}
} 

// Retorna o registro com determinado byte_offset
char *PegaRegistroPrimario(FILE *fp, long int byte_offset)
{
	static char saida[64];
	fseek(fp, byte_offset, SEEK_SET);
	if (fscanf(fp,"%[^\n]\n", saida)<0){
		printf("Erro no arquivo utilizado !\n");
	}

	return saida;
}

//	Procura pelo registro com determinada chave, realizando a busca binaria,
//	retornando a posicao em que ocupa no indice primario
int BuscaRegistroPrimario(TabelaIndiceP *ind_P, char *chave_primaria) 
{
	FILE *fp = fopen("lista.txt", "r");
	char string[64];
	int i, j;
	if (fp == NULL) 
	{
		printf("Arquivo nao encontrado !\n");
		return -1;
	}
	
	i = HeadIndiceP(ind_P);
	j = TailIndiceP(ind_P);
	
	while (i < j) {
		int meio = (i + j) / 2;
		
		if (strcmp(ind_P->vet_indiceP[meio].key, chave_primaria) == 0) {
			fseek(fp, ind_P->vet_indiceP[meio].byte_offset, SEEK_SET);
			if (fscanf(fp, "%[^\n]\n", string) < 0){
				printf("Erro no arquivo utilizado !\n");
			}
			printf("%s\n", string);
			fclose(fp);
			return meio;
		} else if (strcmp(ind_P->vet_indiceP[meio].key, chave_primaria) < 0) {
			i = meio + 1;
		} else {
			j = meio - 1;
		}
	}
	
	if(i == j && strcmp(ind_P->vet_indiceP[j].key, chave_primaria) == 0) {
		fseek(fp, ind_P->vet_indiceP[i].byte_offset, SEEK_SET);
		if (fscanf(fp, "%[^\n]\n", string) < 0){
			printf("Erro no arquivo utilizado !\n");
		}
		printf("%s\n", string);
		fclose(fp);
		return i;
	}
	printf("Nao existe essa chave !\n");
	fclose(fp);
	return -1;
}

// Procura pelo registro com determinada chave primaria utilizando busca binaria.
// Retorna o byteoffset em que o registro ocupa no arquivo.
int BuscaRegistroPrimarioArq(FILE *fp, TabelaIndiceP *ind_P, char *chave_primaria){
	int i, j;
	if (fp == NULL) {
		printf("Arquivo inexistente\n");
		return -1;
	}
	
	i = HeadIndiceP(ind_P);
	j = TailIndiceP(ind_P);
	
	while (i < j) {
		int meio = (i + j) / 2;
		
		if (strcmp(ind_P->vet_indiceP[meio].key, chave_primaria) == 0) {
			fseek(fp, ind_P->vet_indiceP[meio].byte_offset, SEEK_SET);
			return ind_P->vet_indiceP[meio].byte_offset;
		} else if (strcmp(ind_P->vet_indiceP[meio].key, chave_primaria) < 0) {
			i = meio + 1;
		} else {
			j = meio - 1;
		}
	}
	
	if(i == j && strcmp(ind_P->vet_indiceP[j].key, chave_primaria) == 0) {
		fseek(fp, ind_P->vet_indiceP[i].byte_offset, SEEK_SET);
		return ind_P->vet_indiceP[i].byte_offset;
	}
	printf("Nao existe essa chave\n");
	return -1;
}

// Faz a inclusao de um registro no arquivo e no indice primario.
void InclusaoRegistroPrimario(TabelaIndiceP *ind_P, char *registro)
{
	FILE *fp = fopen("lista.txt", "r+");
	if(fp == NULL){
		printf("O arquivo não existe\n");
		return;
	}
	int i = 0, j = 0;
	char chave_primaria[10], *aux;

	// Cria a chave primaria do novo registro.
	aux = strtok(registro, " ");
	chave_primaria[0] = aux[0];
	chave_primaria[1] = aux[1] - 32;
	chave_primaria[2] = aux[2] - 32;
	chave_primaria[3] = '\0';

	do{
		aux = strtok(NULL, " ");
		printf("%s\n", aux);
	}while(aux != NULL && EhNumero(aux) == 0);
	strcat(chave_primaria, aux);
	chave_primaria[9] = '\0';

	// Faz a verificacao se o registro já existe
	i = HeadIndiceP(ind_P);
	j = TailIndiceP(ind_P);
	while (i < j) {
		int meio = (i + j) / 2;
		
		if (strcmp(ind_P->vet_indiceP[meio].key, chave_primaria) == 0) {
			printf("Registro já existente\n");
			fclose(fp);
			return;
		} else if (strcmp(ind_P->vet_indiceP[meio].key, chave_primaria) < 0) {
			i = meio + 1;
		} else {
			j = meio - 1;
		}
	}
	
	if(i == j && strcmp(ind_P->vet_indiceP[j].key, chave_primaria) == 0) {
		printf("Registro já existente\n");
		fclose(fp);
		return;
	}
	
	// Busca o devido local no qual se deve adiconar o registro.
	if (i == j) 
	{
		if (strcmp(ind_P->vet_indiceP[i].key, LAST_IP) == 0) 
		{

		} 
		else if (strcmp(ind_P->vet_indiceP[j].key, chave_primaria) < 0) 
		{
			j++;
		} 
		else 
		{

		}
	} 
	else 
	{
		j = i;
	}
	
	if (ind_P->vet_indiceP[0].key[0] == '*') { // Faz a verificacao se possui algo a remover marcado com '*'

		int byte = ind_P->vet_indiceP[0].byte_offset;
		
		// Passa todos abaixo do local a ser inserido para o começo
		for (i = HeadIndiceP(ind_P); i < j - 1; i++){
			ind_P->vet_indiceP[i] = ind_P->vet_indiceP[i + 1];
		}
		
		strcpy(ind_P->vet_indiceP[j - 1].key, chave_primaria);
		ind_P->vet_indiceP[j - 1].byte_offset = byte;
	
		fseek(fp, byte, SEEK_SET);
		fprintf(fp, "%s", registro);
	} else {
		
		ind_P->tamanho++;
		ind_P->vet_indiceP = (IndiceP *) realloc(ind_P->vet_indiceP, sizeof(IndiceP) * ind_P->tamanho);

		// Passa todos do local a ser inserido para a frente
		for (i = TailIndiceP(ind_P); i > j; i--){
			ind_P->vet_indiceP[i] = ind_P->vet_indiceP[i-1];
		}
		
		strcpy(ind_P->vet_indiceP[j].key, chave_primaria);
		// Faz a atualizacao do byte_offset do elemento inserido, como foi inserido ao final do arquivo só é necessário colocar o dele
		// Há ind_P->tamanho registros incluindo o que está sendo inserido e o final,
		// logo há ind_P->tamanho registros antes dele, cada um com tamanho N_BYTES = 54
		ind_P->vet_indiceP[j].byte_offset = (ind_P->tamanho - 2) * N_BYTES;
		
		fseek(fp, 0, SEEK_END);
		fprintf(fp, "%s\r\n", registro);
	}
	fclose(fp);
}

int EhNumero(char* String)
{
	int FirstDigit = 48;
   	int Result = 1;
   	for(int i=0; i<strlen(String); i++){
   	   if((String[i] < FirstDigit) || (String[i] > FirstDigit + 9)) Result = 0;
   	}
   	return Result;
}

char* PegaChave(TabelaIndiceP *ind_P, int index)
{
	return (ind_P->vet_indiceP[index].key);
}

// Encontra o local no qual se deve inserir o elemento atraves da busca binaria
int binary_search(char *key, char **chaves, int ordem) 
{
	int ini = 0, fim = ordem;
	
	while (ini < fim) {
		int meio = (ini + fim) / 2;
		
		if (strcmp(chaves[meio], key) == 0) {
			return meio;
		} else if (strcmp(chaves[meio], key) > 0) {
			fim = meio;
		} else {
			ini = meio + 1;
		}
	}
	
	return ini;
}

// Cria uma arvore B vazia
BTreeNode CriaArvoreB(int ordem){
	int  i;
	BTreeNode arvB = (BTreeNode)malloc(sizeof(BTree));

	arvB->chave = (char**)malloc(sizeof(char*) * ordem-1);
	for (i = 0; i < ordem; ++i){
		arvB->chave[i] = (char*)malloc(sizeof(char) * TAM_CHAVEP);
	}
	arvB->filhos = (BTreeNode*)malloc(sizeof(BTreeNode) * ordem);
	for (i = 0; i < ordem; ++i){
		arvB->filhos[i] = NULL;
	}
	arvB->ordem = ordem;
	arvB->num_chaves = 0;

	arvB->pai = arvB;
	
	return arvB;
}

BTreeNode CriaPagina(int ordem){
	int  i;
	BTreeNode arvB = (BTreeNode)malloc(sizeof(BTree));

	arvB->chave = (char**)malloc(sizeof(char*) * ordem-1);
	for (i = 0; i < ordem; ++i){
		arvB->chave[i] = (char*)malloc(sizeof(char) * TAM_CHAVEP);
	}
	arvB->filhos = (BTreeNode*)malloc(sizeof(BTreeNode) * ordem);
	for (i = 0; i < ordem; ++i){
		arvB->filhos[i] = NULL;
	}
	arvB->ordem = ordem;
	arvB->num_chaves = 0;

	arvB->pai = NULL;
	
	return arvB;
}

int atualizapai(BTreeNode arvB, BTreeNode pai){
	if(arvB == NULL){
		return FALSE;
	}
	int i;

	arvB->pai = pai;

	for (i = 0; i < arvB->ordem ; ++i){
		if(atualizapai(arvB->filhos[i], pai) == FALSE){
			break;
		}
	}

	return TRUE;
}

int LimpaArvoreB(BTreeNode arvB)
{
	static int nivel = 0;
	
	if(arvB == NULL){
		nivel--;
		return FALSE;
	}
	int i;
	
	for (i = 0; i < arvB->ordem -1; ++i){
		free(arvB->chave[i]);
	}
	free(arvB->chave);

	for (i = 0; i <= arvB->num_chaves ; ++i){
		nivel++;
		if(LimpaArvoreB(arvB->filhos[i]) == FALSE){
			break;
		}
	}
	if (arvB->filhos != NULL){
		free(arvB->filhos);
	}

	free(arvB);

	nivel--;
	return TRUE;
}

BTreeNode BuscaArvB(BTreeNode raiz, int *seeks ,char* chave){
	if (raiz == NULL){
		return NULL;
	}
	int i = 0, index = 0;
	while( strcmp(raiz->chave[i], chave) < 0){
		i++;
		index++;
	}

	*seeks = *seeks + 1;
	if(strcmp(raiz->chave[i], chave) == 0){
		return raiz;
	}
	return(BuscaArvB(raiz->filhos[index], seeks, chave));	
}

// Cria a arvoreB baseada nos Indices Primarios gerados.
BTreeNode CriaArvBIndicePrimario(TabelaIndiceP *ind_P, int ordem){
	BTreeNode arvB = CriaArvoreB(ordem);
	int i;
	for (i = 0; i < TailIndiceP(ind_P) ; ++i){
		arvB = InsereArvB2(arvB, PegaChave(ind_P, i));
	}
	return arvB;
}

// Cria novos filhos
BTreeNode create (char **temp, BTreeNode *filhos, int ini, int fim, int ordem) {
	BTreeNode filho = CriaArvoreB(ordem);
	int i, j;
	
	for (i = ini, j = 0; i < fim; i++, j++) {
		strcpy(filho->chave[j], temp[i]);
		filho->filhos[j] = filhos[i];
	}
	filho->filhos[j] = filhos[fim];
	filho->num_chaves = j;
	
	return filho;
}

// Realiza a insercao na arvore B
char *InsereArvB1(BTreeNode raiz, char* chave){
	
	static char **temp;
	static BTreeNode filho2;
	int ind = binary_search(chave, raiz->chave, raiz->num_chaves);
	int i;
	
	printf("%p %d\n", raiz, ind);

	if (ind < raiz->ordem && !strcmp(raiz->chave[ind], chave)) {
		printf("A Chave inserida ja existe na arvore !\n");
		return NULL;
	} else {
		if (raiz->filhos[ind] != NULL) {
			
			char *subiu = (char *) malloc(sizeof(char) * TAM_CHAVEP);
			
			char *p = InsereArvB1(raiz->filhos[ind], chave);
			
			printf("Inseriu %s %d\n", p, ind);
			
			if (p == NULL)
				return NULL;
			
			strcpy(subiu, p);
			
			if (raiz->num_chaves < raiz->ordem - 1) {
				// Somente faz a insercao
				
				int i;
				
				for (i = raiz->num_chaves; i > ind; i--) {
					strcpy(raiz->chave[i], raiz->chave[i - 1]);
					raiz->filhos[i + 1] = raiz->filhos[i];
				}
				strcpy(raiz->chave[ind], subiu);
				raiz->filhos[ind + 1] = filho2;
				
				for (i = 0; i < raiz->ordem; ++i){
					free(temp[i]);
				}
				free(temp);
				filho2 = NULL;
				
				raiz->num_chaves++;
				
				return NULL;
			} else {
				for (i = 0; i < raiz->ordem; ++i){
					free(temp[i]);
				}
				free(temp);
				temp = (char**) malloc(sizeof(char*) * raiz->ordem);
				for (i = 0; i < raiz->ordem; ++i){
					temp[i] = (char*)malloc(sizeof(char) * TAM_CHAVEP);
				}
				for (i = raiz->ordem - 1; i > ind; i--) {
					strcpy(temp[i], raiz->chave[i - 1]);
				}
				strcpy(temp[ind], subiu);
				for (i = ind - 1; i >= 0; i--) {
					strcpy(temp[i], raiz->chave[i]);
				}
				
				BTreeNode *filhos1 = (BTreeNode*)malloc(sizeof(BTreeNode) * raiz->ordem + 1);
				for (i = 0; i < raiz->ordem + 1; ++i){
					filhos1[i] = NULL;
				}
				
				for (i = raiz->ordem; i > ind + 1; i--) {
					filhos1[i] = raiz->filhos[i - 1];
				}
				filhos1[ind + 1] = filho2;
				for (i = ind; i >= 0; i--) {
					filhos1[i] = raiz->filhos[i];
				}
			
				char *sobe = temp[raiz->ordem / 2];
				
				for (i = 0; i < raiz->ordem / 2; i++) {
					strcpy(raiz->chave[i], temp[i]);
					raiz->filhos[i] = filhos1[i];
				}
				raiz->filhos[i] = filhos1[i];
				raiz->num_chaves = raiz->ordem / 2;
				
				BTreeNode new_raiz = CriaPagina(raiz->ordem);
				
				int j;
				for (i = raiz->ordem / 2 + 1, j = 0; i < raiz->ordem; i++, j++) {
					strcpy(new_raiz->chave[j], temp[i]);
					new_raiz->filhos[j] = filhos1[i];
				}
				new_raiz->filhos[j] = filhos1[i];
				new_raiz->num_chaves = j;
				
				
				if (raiz == raiz->pai) {
					// Aumenta o tamanho 
					BTreeNode new_pai = CriaArvoreB(raiz->ordem);
					strcpy(new_pai->chave[0], sobe);
					new_pai->filhos[0] = raiz;
					new_pai->filhos[1] = new_raiz;
					
					atualizapai(new_pai, new_pai);
					
					filho2 = NULL;
					
					new_pai->num_chaves = 1;
					return NULL;
				} else {
					// Cria outra node temporario
					filho2 = new_raiz;
					
					return sobe;
				}
			}
			
		} else if (raiz->num_chaves < raiz->ordem - 1) {
		
			for (i = raiz->num_chaves; ind < i; i--) {
				strcpy(raiz->chave[i], raiz->chave[i - 1]);
			}
			strcpy(raiz->chave[ind], chave);
			raiz->num_chaves++;
			return NULL;
		} else {
			temp = (char**) malloc(sizeof(char*) * raiz->ordem);
			for (i = 0; i < raiz->ordem; ++i){
				temp[i] = (char*)malloc(sizeof(char) * TAM_CHAVEP);
			}
			
			for (i = raiz->ordem - 1; ind < i; i--) {
				strcpy(temp[i], raiz->chave[i - 1]);
			}
			strcpy(temp[ind], chave);
			for (i = ind - 1; i >= 0; i--) {
				strcpy(temp[i], raiz->chave[i]);
			}
			
			BTreeNode *filhos = (BTreeNode*)malloc(sizeof(BTreeNode) * raiz->ordem + 1);
			for (i = 0; i < raiz->ordem + 1; ++i){
				filhos[i] = NULL;
			}
			
			char *sobe = temp[raiz->ordem / 2];
			
			for (i = 0; i < raiz->ordem / 2; i++) 
			{
				strcpy(raiz->chave[i], temp[i]);
			}
			raiz->num_chaves = raiz->ordem / 2;
			
			filho2 = create(temp, filhos, raiz->ordem / 2 + 1, raiz->ordem, raiz->ordem);

			filho2->pai = raiz->pai;

			if (raiz == raiz->pai) 
			{
				
				//aumenta o tamanho
				BTreeNode new_pai = CriaArvoreB(raiz->ordem);
				strcpy(new_pai->chave[0], sobe);
				new_pai->filhos[0] = raiz;
				new_pai->filhos[1] = filho2;
				
				atualizapai(new_pai, new_pai);
				
				for (i = 0; i < raiz->ordem; ++i){
					free(temp[i]);
				}
				free(temp);
				filho2 = NULL;
				
				new_pai->num_chaves = 1;
				
				return NULL;
			} 
			else 
			{
				return sobe;
			}
			return sobe;
		}
	}
}

BTreeNode InsereArvB2(BTreeNode raiz, char *chave) {
	InsereArvB1(raiz, chave);
	
	printf("%p %p\n", raiz, raiz->pai);
	
	return raiz->pai;
}

void LimpaPagina(BTreeNode local){
	if (local == NULL){
		return;
	}
	int i;
	for (i = 0; i < local->num_chaves ; ++i){
		free(local->chave[i]);
	}
	free(local->chave);
	free(local->filhos);
}

// Insere de forma ordenada na pagina dada
int InserePag(BTreeNode local, char* chave){
	int index = 0;
	int i = 0, j;
	// Caso não exista nenhuma chave
	if(local->num_chaves == 0){
		strcpy(local->chave[0], chave);
		local->num_chaves++;
	}
	else{
		// Se existir, procura seu local para inserir ordenado
		while(strcmp(local->chave[i], chave) < 0 && i < local->num_chaves){
			i++;
		}
		// Se o seu local estiver no meio das chaves já colocadas
		if(i < local->num_chaves){
			j = local->num_chaves;
			// Desloca todos as chaves para abrir espaço para a nova chave
			while(j >= i){
				strcpy(local->chave[j+1], local->chave[j]);
				j--;
			}
			strcpy(local->chave[i], chave);
			local->num_chaves++;
		}
		else{
			strcpy(local->chave[local->num_chaves], chave);
			local->num_chaves++;
		}
	}

	return i;
}
// Imprime a arvore em ordem. 
void inOrder(BTreeNode raiz){
	if(raiz == NULL){
		return;
	}

	int i = 0 ;
	BTreeNode aux = raiz;
	while(i < raiz->num_chaves){
		inOrder(raiz->filhos[i]);
		printf("%s\n", raiz->chave[i]);
		i++;
	}
	inOrder(raiz->filhos[i]);
}

// Imprime de outro modo a arvore como se fosse as paginas.
void inOrder2(BTreeNode raiz)
{
	if(raiz == NULL){
		return;
	}

	int i = 0 ;
	BTreeNode aux = raiz;
	
	while (i < raiz->num_chaves) {
		printf("%s\t", raiz->chave[i]);
		i++;
	}
	printf("\n");
	
	i = 0;
	while(i <= raiz->num_chaves){
		inOrder2(raiz->filhos[i]);
		i++;
	}
}

void EscreveArvBArq(FILE *fp, BTreeNode arvB){
	if (fp == NULL || arvB == NULL){
		return;
	}
	int i, j;
	
	char flag = 'r';
	if (arvB->filhos[0] == NULL) {
		flag = 'f';
	}
	
	fwrite(&flag, sizeof(char), 1, fp);
	fwrite(&arvB->ordem, sizeof(int), 1, fp);
	fwrite(&arvB->num_chaves, sizeof(int), 1, fp);

	for (i = 0; i < arvB->num_chaves; i++){
		fwrite(arvB->chave[i], sizeof(char), TAM_CHAVEP, fp);
	}
	fflush(fp);
	if (flag == 'r') {
		for(i = 0 ; i <= arvB->num_chaves; i++){
			EscreveArvBArq(fp, arvB->filhos[i]);
		}
	}

	return;
}

BTreeNode LeituraArvBArq(FILE *fp, BTreeNode arvB)
{
	if (fp == NULL || feof(fp) != 0){
		return NULL;
	}
	int ordem;
	size_t read;
	char flag;
	char chave[TAM_CHAVEP];
	int i, j;

	read = fread(&flag, sizeof(char), 1, fp);
	read = fread(&ordem, sizeof(int), 1, fp);
	
	if (arvB == NULL) {
		arvB = CriaArvoreB(ordem);
	}
	
	read = fread(&arvB->num_chaves, sizeof(int), 1, fp);

	for (i = 0; i < arvB->num_chaves; ++i){
		read = fread(chave, sizeof(char), TAM_CHAVEP, fp);
		strcpy(arvB->chave[i], chave);
	}

	if (flag == 'r') {
		for(i = 0 ; i <= arvB->num_chaves ; i++){
			BTreeNode ret = LeituraArvBArq(fp, NULL);
			arvB->filhos[i] = ret;
		}
	}
	
	return arvB;
}

void menu()
{
	if(system("clear"))
	{
		return;
	}

	printf("\n**************************************************\n");
	printf("Criar Arvore B a partir dos indices primarios.	(1)\n");
	printf("Criar Arvore B vazia.                         	(2)\n");
	printf("Adicionar chave na Arvore B.			(3)\n");
	printf("Realizar busca de chave na Arvore B.          	(4)\n");
	printf("Mostrar Indices Primarios ordenados.		(5)\n");
	printf("Mostrar em Ordem a Arvore B.			(6)\n");
	printf("Mostrar a Arvore B separada com as paginas.	(7)\n");
	printf("Encerrar programa				(8)\n");
	printf("**************************************************\n");

	return;
}

int main(int agrc, char** argv)
{
	TabelaIndiceP* IP = NULL;
	BTreeNode arvoreB = NULL;
	FILE *saida = NULL;
	IP = NULL;
	int op, ordem, seeks;
	char chave[10];

	IP = CriaIndicesPrimarios(); // Criando os Indices primarios !
	while(op != 8)
	{
		menu();
		printf("Escolha a operacao desejada:\n");
		if(scanf("%d", &op) < 0){
			return -1;
		}
		getchar();

		switch (op)
		{
			case 2:
				printf("Digite a ordem da arvore: ");
				if(scanf("%d", &ordem) < 0)
				{
					return -1;
				}
				if (ordem < 2)
				{
					printf("Digite uma ordem maior que 2.\n");
					break;
				}
				arvoreB = CriaArvoreB(ordem);
				saida = fopen("indicelista.bt", "wb");
				break;
			case 3:
				if (arvoreB == NULL)
				{
					printf("Crie uma arvore antes.\n");
					break;
				}
				printf("Digite a chave no formato de 3 Letras e 5 Números (EX: AVC21345)\n");
				if(scanf("%[^\n]", chave)<0)
				{
					return -1;
				}
				arvoreB = InsereArvB2(arvoreB, chave);
				EscreveArvBArq(saida, arvoreB);
				break;
			case 6:
				if(arvoreB == NULL)
				{
					printf("Crie uma arvore antes\n");
					break;
				}
				printf("\n");
				inOrder(arvoreB);
				break;
			case 5:
				if(IP == NULL)
				{
					printf("Arquivo de para gerar indices com falha!\n");
				}
				else
				{
					MostraIndicesPrimarios(IP);
				}
				break;
			case 1:
				if(IP == NULL || arvoreB != NULL)
				{
					printf("Já existe uma arvore B criada !\n");
				}
				else{
					printf("Digite a ordem da arvore: ");
					if(scanf("%d", &ordem) < 0)
					{
						return -1;
					}
					if (ordem < 2)
					{
						printf("Digite uma ordem maior que 2.\n");
						break;
					}
					arvoreB = CriaArvBIndicePrimario(IP, ordem);
					saida = fopen("indicelista.bt", "wb");
					EscreveArvBArq(saida, arvoreB);
				}
				break;
			case 4:
				printf("Digite a chave no formato de 3 Letras e 5 Números (EX: AVC21345)\n");
				if(scanf("%[^\n]", chave) < 0)
				{
					return -1;
				}
				getchar();
				if(BuscaArvB(arvoreB, &seeks, chave) != NULL)
				{
					printf("Numero de seeks = %d\n", seeks);
					seeks = 0;
				}
				else
				{
					printf("Chave nao encontrada !\n");
				}
				break;
			case 7:
				if(arvoreB == NULL)
				{
					printf("Crie uma arvore antes de pedir para mostrar !\n");
					break;
				}
				printf("\n");
				inOrder2(arvoreB);
				break;
			case 8:
				exit(1);
				break;
			default:
				if(system("clear"))
				{
					return -1;
				}
				printf("Nao existe a opcao escolhida!\n");
				break;
		}
		printf("**********PRESSIONE ENTER PARA VOLTAR**********\n");
		getchar();
	}
	// Limpando os ponteiros da arvore B.
	if (arvoreB == NULL)
	{
		LimpaArvoreB(arvoreB);
	}
	// Limpando os ponteiros dos indices primarios.
	if (IP != NULL)
	{
		free(IP);
	}
	// Salvando o arquivo de saida.
	if (saida != NULL)
	{
		fclose(saida);
	}

	return 0;
}