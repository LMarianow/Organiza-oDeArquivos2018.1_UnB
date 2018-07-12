#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/****************************ESTRUTURAS UTILIZADAS********************************/
typedef struct Lista
{
  char chave_primaria[31];
  char curso[3];
  int op;
  int matric;
  char nome[43];
  char turma;
  int num_arq;  // NUMERO DO ARQUIVO 1 OU 2
  int posicao; // BYTEOFFSET
  struct Lista *prox;
  struct Lista *ant;
} Lista;

typedef struct Aponta
{
  Lista *inicio;
  Lista *fim;
  int tamanho;
} Aponta;

typedef struct ListaInvertida
{
  int id;
  char chave_primaria[31];
  char curso[3];
  struct ListaInvertida* prox;
}ListaInvertida;

typedef struct Pilha
{
  int qtd;
  int arq;
  int nrr[20];
}Pilha;
/**********************************************************************************/

/******************************DECLARACOES DE FUNCOES******************************/
/* FUNCOES PARA CRIACAO E MANIPULACAO DA PED (PILHA DE ESPACOS DISPONIVEIS) */
Pilha * CriaPilha();
void InserePilha(Pilha *pi, int p, int arq);
int RemovePilha(Pilha *pi);
/****************************************************************************/
/* FUNCÕES PARA INTERAÇÃO COM USUÁRIO */
int menu();
void escolha(int opcao, Aponta *lista1, Aponta *lista2, Aponta *lista3, ListaInvertida **listainvertida1, ListaInvertida **listainvertida2, Pilha *PED);
/**************************************/
/* FUNCOES PARA CRIACAO E MANIPULACAO DA LISTA DUPLAMENTE ENCADEADA DOS REGISTROS */
void InsereOrdenado(Aponta *lista, int matricula, char *nome, char *curso, char turma, int op, int num_arq, int posicao, char *chave_primaria);
void MostraLista(Aponta *lista,int arquivo);
Aponta *CriaLista();
Lista *AlocaLista(int matricula, char *nome, char* curso, char turma, int op, int num_arq, int posicao, char *chave_primaria);
void RetiraFim(Aponta *lista);
void RetiraInicio(Aponta *lista);
void free_node(Lista **n);
int vazia (Aponta *lista);
void libera(Aponta *lista);
Lista *RetornaNo(Aponta *lista,int indice);
/**********************************************************************************/
/* FUNCOES PARA CRIACAO E MANIPULACAO DOS INDICES PRIMARIOS E SECUNDARIOS */
void CriaIndices(Aponta *lista,int opcao);
void ManipulaString(char *string, int desloca, int num_arq, Aponta *lista);
void IncluiRegistroLista(Aponta *lista,int arquivo,Pilha *PED);
void AtualizaIndices(Aponta *lista, int num_arq);
void Atualizacao(Aponta *lista, int indice, int num_arq);
void EscreveIndicesPrimarios(Aponta *lista, int num_arq);
void EscreveIndicesSecundarios(Aponta *lista, int num_arq);
void MostrarChavesPrimarias(Aponta *lista,int arquivo);
void MostrarChavesSecundarias(Aponta *lista,int arquivo);
void Exclusao(Aponta* lista, int arquivo, int indice, Pilha *PED);
void RemoveIndice(Aponta *lista, int indice);
/**************************************************************************/
/* FUNCOES PARA JUNCAO(MERGE) DOS INDICES DOS REGISTROS DA LISTA 1 E 2 ORDENADOS */
void CriaArquivoMerge(Aponta *lista);
Lista *CombinaListas(Lista *esquerda, Lista *direita);
void RemoveIguais(Aponta *lista);
/*********************************************************************************/
/*FUNCOES UTILIZADAS PARA CRIACAO E MANIPULACAO DA LISTA INVERTIDA*/
ListaInvertida* CriaInvertida();
ListaInvertida* InicializaInvertida(int id);
void InsereInvertida(ListaInvertida* pai, char *curso , char *chave_primaria);
int VerificaCurso(char *curso);
void MostraListaInvertida(Aponta *Lista, ListaInvertida **ListaInvertida,int arquivo);
void BuscaCurso(Aponta *lista, ListaInvertida** pai, int arquivo);
/******************************************************************/
/**********************************************************************************/

/******************************IMPLEMENTACAO DAS FUNCOES******************************/
/* Funcao para criar a PED */
Pilha * CriaPilha()
{
  Pilha *pi;
  pi = (Pilha*) malloc(sizeof(Pilha));
  pi->qtd = 0;
  return pi;
}
/* Funcao para inserir elemento na PED */
void InserePilha(Pilha *pi, int p, int arq)
{
  /*  Pode estar cheia ou pi == null */
  pi->nrr[pi->qtd] = p;
  pi->arq = arq;
  pi->qtd++;
}
/* Funcao para remover elemento da PED */
int RemovePilha(Pilha *pi)
{
  int topo = pi->nrr[pi->qtd-1];

  pi->qtd--;

  return topo;
}
/* Funcao para mostrar o menu ao usuario */
int menu()
{
  int opcao;
  printf("\n*************************************************\n");
  printf("Incluir Registro                              (1)\n");
  printf("Excluir Registro                              (2)\n");
  printf("Atualizar Registro                            (3)\n");
  printf("Mostrar Todos os Registros                    (4)\n");
  printf("Mostrar Indices Primarios                     (5)\n");
  printf("Mostrar Indices Secundarios                   (6)\n");
  printf("Mostrar Lista Invertida                       (7)\n");
  printf("Encerrar programa e Gerar Merging das Listas  (0)\n");
  printf("*************************************************\n");
  printf("Escolha a operacao desejada:\n");
  scanf("%d",&opcao);
  printf("\n");
  return opcao;
}
/* Funcao que recebe a opcao escolhida pelo usuario no menu e realiza as operacoes necessarias */
void escolha( int opcao, Aponta *lista1, Aponta *lista2, Aponta *lista3, ListaInvertida **listainvertida1, ListaInvertida **listainvertida2, Pilha *PED)
{
  int num_arq;

  switch(opcao)
  {
    case 1:
      printf("Escolha o arquivo:\n");
      printf("Lista1 (1)\n");
      printf("Lista2 (2)\n");
      scanf("%d",&num_arq);
      getchar();
      if (num_arq == 1)
      {
        IncluiRegistroLista(lista1,1,PED);
      }
      else
      {
        IncluiRegistroLista(lista2,2,PED);
      } 
      break;
    case 2:
      printf("Escolha o arquivo:\n");
      printf("Lista1 (1)\n");
      printf("Lista2 (2)\n");
      scanf("%d",&num_arq);
      if (num_arq == 1)
      {
          MostraLista(lista1,1);
          printf("\nEscolha o ID do registro a ser removido:\n");
          int num;
          scanf("%d", &num);
          Exclusao(lista1, 1, num, PED);
      }
      else
      {
          MostraLista(lista2,2);
          printf("\nEscolha o ID do registro a ser removido:\n");
          int num;
          scanf("%d", &num);
          Exclusao(lista2, 2, num, PED);
      }
      break;
    case 3:
      printf("Escolha o arquivo:\n");
      printf("Lista1 (1)\n");
      printf("Lista2 (2)\n");
      scanf("%d",&num_arq);
      if (num_arq == 1)
      {
          MostraLista(lista1,1);
          printf("\nEscolha o ID do registro a ser atualizado:\n");
          int num;
          scanf("%d", &num);
          Atualizacao(lista1, num, 1);
      }
      else
      {
          MostraLista(lista2,2);
          printf("\nEscolha o ID do registro a ser atualizado:\n");
          int num;
          scanf("%d", &num);

          Atualizacao(lista2, num, 2);
      }
      break;
    case 4:
      MostraLista(lista1,1);
      MostraLista(lista2,2);
      break;
    case 5:
      printf("Escolha o arquivo:\n");
      printf("Lista1 (1)\n");
      printf("Lista2 (2)\n");
      scanf("%d",&num_arq);
      if (num_arq == 1)
        MostrarChavesPrimarias(lista1,1);
      else
        MostrarChavesPrimarias(lista2,2);
      break;
    case 6:
      printf("Escolha o arquivo:\n");
      printf("Lista1 (1)\n");
      printf("Lista2 (2)\n");
      scanf("%d",&num_arq);
      if (num_arq == 1)
        MostrarChavesSecundarias(lista1,1);
      else
        MostrarChavesSecundarias(lista2,2);
      break;
    case 7:
      printf("Escolha o arquivo:\n");
      printf("Lista1 (1)\n");
      printf("Lista2 (2)\n");
      scanf("%d",&num_arq);
      if (num_arq == 1)
        MostraListaInvertida(lista1,listainvertida1,1);
      else
        MostraListaInvertida(lista2,listainvertida2,2);
      break;
    case 0:
      lista3->inicio = CombinaListas(lista1->inicio,lista2->inicio);
      RemoveIguais(lista3);
      CriaArquivoMerge(lista3);
      break;
    default:
      printf("Opcao Invalida!!\n");
  }
}
/* Funcao para criar uma lista */
Aponta *CriaLista()
{
  Aponta* ptr = (Aponta*)malloc(sizeof(Aponta));
  ptr->inicio = NULL;
  ptr->fim = NULL;
  ptr->tamanho = 0;
  return ptr;
}
/* Funcao para alocar os elementos da lista */
Lista *AlocaLista(int matricula, char *nome, char* curso, char turma, int op, int num_arq, int posicao, char *chave_primaria)
{
  Lista* novo = (Lista*)malloc(sizeof(Lista));
  novo->matric = matricula;
  strcpy(novo->nome, nome);
  strcpy(novo->curso,curso);
  novo->turma=turma;
  strcpy(novo->chave_primaria, chave_primaria);
  novo->op = op;
  novo->posicao = posicao;
  novo->num_arq = num_arq;
  novo->ant = NULL;
  novo->prox = NULL;
  return novo;
}
/* Funcao para inserir ordenadamente na lista */
void InsereOrdenado(Aponta *lista, int matricula, char *nome, char *curso, char turma, int op, int num_arq, int posicao, char *chave_primaria) 
{
  Lista *atual = lista->inicio;
  Lista *novo = AlocaLista(matricula, nome, curso, turma, op, num_arq, posicao, chave_primaria);

  /*
  * loop para descobrir o maior elemento
  */
  while(atual != NULL && (strcmp(atual->chave_primaria, chave_primaria) < 0))
  {
    atual = atual->prox;
  }

  /*
   * parou o loop, entao a chave encontrada precisa ser armazenada antes do node atual
   */
  if(lista->inicio == NULL && lista->fim == NULL)
  {
    lista->inicio = lista->fim = novo;
  }
  else if(atual == NULL)
  {
    // significa que deve ser adicionada ao fim da lista
    novo->ant = lista->fim;
    novo->prox = NULL;
    lista->fim->prox = novo;
    lista->fim = novo;
  }
  else if(atual->ant == NULL)
  {
    // inserir no comeco da lista
    novo->ant = NULL;
    novo->prox = atual;
    lista->inicio->ant = novo;
    lista->inicio = novo;
  }
  else
  {
    // inserir em algum lugar no meio da lista
    novo->ant = atual->ant;
    novo->prox = atual;
    novo->ant->prox = novo->prox->ant = novo;
  }
  lista->tamanho++;
}

/* FUNCAO QUE RETORNO O NO DA LISTA A PARTIR DO INDICE */
Lista *RetornaNo(Aponta *lista,int indice)
{
  if(indice >= 0 && indice < lista->tamanho)
  {
    Lista *aux = lista->inicio;
    int i;
    for(i=0;i< indice; i++)
    {
      aux = aux->prox;
    }

    return aux;
  }
  else
  {
    printf("Indice invalido!\n");
    return NULL;
  }
}
/* Funcao que exibe na tela todos os elementos da Lista */
void MostraLista(Aponta *lista,int arquivo)
{
  Lista *aux = lista->inicio;
  int i=0;
  if (arquivo == 1)
    printf("\t\t\t REGISTROS ARQUIVO LISTA 1\n");
  else
    printf("\t\t\t REGISTROS ARQUIVO LISTA 2\n");
  printf("MATRIC NOME                                             OP CURSO    TURMA|ID\n");
  for(;aux!=NULL;aux=aux->prox)
  {
    ++i;
    printf("0%d %s\t%d  %s\t\t%c|%d\n",aux->matric, aux->nome, aux->op, aux->curso, aux->turma, i);
  }
}

/* Funcao para liberar a memoria alocada por um no da lista */
void free_node(Lista **n) 
{
  if (n != NULL && *n != NULL) 
  {
    free(*n);
    *n = NULL;
  }
}
/* Funcao para liberar a memoria alocada por um no da lista */
void RemoveIguais(Aponta *lista) 
{
  Lista *atual = lista->inicio;
  Lista *subsequente = lista->inicio->prox;

  if (atual != NULL) {
      while (atual->prox != NULL) {
          if (strcmp(atual->chave_primaria, atual->prox->chave_primaria) == 0) { 
              subsequente = atual->prox->prox;
              Lista *remove_node = atual->prox;
              atual->prox = subsequente;
              free_node(&remove_node);
          } else {
              atual = atual->prox;
          }
      }
  }
}

/* Função que verifica se a Lista esta vazia */
int vazia (Aponta *lista)
{
  if(lista == NULL)
  {/*Lista eh valida? */
    return 0;
  }
  else
  {
    return 1; /*Lista nao esta vazia */
  }
}

void Atualizacao(Aponta *lista, int indice, int num_arq)
{
  int opcao,posicao,n,vezes;
  char dado[50], nova_chave_primaria[31],string[100], nome[43], turma[3], matricula[7], curso[2], op[6];
  Lista* aux = RetornaNo(lista,indice-1);
  FILE *fp;
  sprintf(matricula, "%06d", aux->matric);
  sprintf(matricula, "%s", matricula);
  sprintf(nova_chave_primaria, "%s", matricula);
/*  Menu escolha do campo */
  printf("\nO que deseja alterar?\n1 - MATRICULA\n2 - NOME\n3 - OP\n4 - CURSO\n5 - TURMA\n");
  printf("\nQual o campo sera alterado? Escolha o campo de acordo com seu numero correspondente:\n");
  scanf("%d", &opcao);
  getchar();
  printf("\nEscreva o valor do novo dado:\n");
  scanf("%[^\n]%*c", dado);
  
  sprintf(op, "%d", aux->op);
  sprintf(turma, "%c", aux->turma);
  strcpy(nome,aux->nome);
  strcpy(curso,aux->curso);
  posicao = aux->posicao;
  switch(opcao)
  {
      case 1:
      /*  Mudar o valor do registro na lista */
        aux->matric = atoi(dado);
        strcpy(nova_chave_primaria, dado);
      /*  Mudando indices primarios e secundarios --> geracao de nova chave primaria*/
        strncat(nova_chave_primaria, aux->nome, 24);
        RemoveIndice(lista, indice-1);
        InsereOrdenado(lista, atoi(dado), nome, curso, turma[0], atoi(op), num_arq, posicao, nova_chave_primaria);
        strcpy(string,dado);
        strcat(string, " ");
        strcat(string, nome);
        n = strlen(string);
        vezes = 48 - n;
        for(int i = 0; i < vezes; i++)
            strcat(string, " ");
        strcat(string, op);
        strcat(string, "  ");
        strcat(string, curso);
        strcat(string,"       ");
        strcat(string, turma);
        AtualizaIndices(lista,num_arq);
      /*  Mudanca no arquivo de registros*/
        if(num_arq == 1)
        {
          fp = fopen("lista1.txt", "r+");
          fseek(fp, aux->posicao, SEEK_SET); /*A partir do começo, 1 registro possui 64 bytes*/
          fprintf(fp,"%s",string);
          fclose(fp);
        }
        else
        {
          fp = fopen("lista2.txt", "r+");
          fseek(fp, aux->posicao, SEEK_SET); /*A partir do começo, 1 registro possui 64 bytes*/
          fprintf(fp,"%s",string);
          fclose(fp);
        }
        break;
      case 2:
      /*  Mudar o valor do registro na lista */
        strcpy(aux->nome, dado);
        sprintf(matricula, "%s", nova_chave_primaria);
      /*  Mudando indices primarios e secundarios --> geracao de nova chave primaria */
        strncat(nova_chave_primaria, aux->nome, 24);
        InsereOrdenado(lista, aux->matric, aux->nome, aux->curso, aux->turma, aux->op, aux->num_arq, aux->posicao, nova_chave_primaria);
        RemoveIndice(lista, indice-1);
        AtualizaIndices(lista,num_arq);
      /*  Mudanca no arquivo de registros*/
        if(num_arq == 1)
        {
          fp = fopen("lista1.txt", "r+");
          fseek(fp, aux->posicao+7, SEEK_SET); /*A partir do começo, 1 registro possui 64 bytes*/
          fprintf(fp, "%-40.40s", dado);
          fclose(fp);
        }
        else
        {
          fp = fopen("lista2.txt", "r+");
          fseek(fp, aux->posicao+7, SEEK_SET); /*A partir do começo, 1 registro possui 64 bytes*/
          fprintf(fp, "%-40.40s", dado);
          fclose(fp);
        }
        break;
      case 3:
      /*  Mudar o valor do registro na lista */
        aux->op = atoi(dado);
        strcpy(string,nova_chave_primaria);
        strcat(string, " ");
        strcat(string, nome);
        n = strlen(string);
        vezes = 48 - n;
        for(int i = 0; i < vezes; i++)
            strcat(string, " ");
        strcat(string, dado);
        strcat(string, "  ");
        strcat(string, curso);
        strcat(string,"       ");
        strcat(string, turma);
      /*  Mudanca no arquivo de registros*/
        if(num_arq == 1)
        {
          fp = fopen("lista1.txt", "r+");
          fseek(fp, aux->posicao, SEEK_SET); /*A partir do começo, 1 registro possui 64 bytes*/
          fprintf(fp,"%s",string);
          fclose(fp);
        }
        else
        {
          fp = fopen("lista2.txt", "r+");
          fseek(fp, aux->posicao, SEEK_SET); /*A partir do começo, 1 registro possui 64 bytes*/
          fprintf(fp,"%s",string);
          fclose(fp);
        }
        break;
      case 4:
      /*  Mudar o valor do registro na lista */
        strcpy(aux->curso, dado);
        strcpy(string,nova_chave_primaria);
        strcat(string, " ");
        strcat(string, nome);
        n = strlen(string);
        vezes = 48 - n;
        for(int i = 0; i < vezes; i++)
            strcat(string, " ");
        strcat(string, op);
        strcat(string, "  ");
        strcat(string, dado);
        strcat(string,"       ");
        strcat(string, turma);
      /*  Mudanca no arquivo de registros*/
        if(num_arq == 1)
        {
          fp = fopen("lista1.txt", "r+");
          fseek(fp, aux->posicao, SEEK_SET); /*A partir do começo, 1 registro possui 64 bytes*/
          fprintf(fp,"%s",string);
          fclose(fp);
        }
        else
        {
          fp = fopen("lista2.txt", "r+");
          fseek(fp, aux->posicao, SEEK_SET); /*A partir do começo, 1 registro possui 64 bytes*/
          fprintf(fp,"%s",string);
          fclose(fp);
        }
        break;
      case 5:
      /*  Mudar o valor do registro na lista */
        aux->turma=dado[0];
        strcpy(string,nova_chave_primaria);
        strcat(string, " ");
        strcat(string, nome);
        n = strlen(string);
        vezes = 48 - n;
        for(int i = 0; i < vezes; i++)
            strcat(string, " ");
        strcat(string, op);
        strcat(string, "  ");
        strcat(string, curso);
        strcat(string,"       ");
        strcat(string, dado);
        /*  Mudanca no arquivo de registros*/
        if(num_arq == 1)
        {
          fp = fopen("lista1.txt", "r+");
          fseek(fp, aux->posicao, SEEK_SET); /*A partir do começo, 1 registro possui 64 bytes*/
          fprintf(fp,"%s",string);
          fclose(fp);
        }
        else
        {
          fp = fopen("lista2.txt", "r+");
          fseek(fp, aux->posicao, SEEK_SET); /*A partir do começo, 1 registro possui 64 bytes*/
          fprintf(fp,"%s",string);
          fclose(fp);
        }
        break;
      default:
        printf("Nao existe esse campo!\n");
        break;
    }
    AtualizaIndices(lista,num_arq);
} 
/* FUNCAO PARA EXCLUSAO DE UM REGISTRO FAZENDO O DEVIDO ARMAZENAMENTO NA PED PARA O REUSO DE ESPACOS */
void Exclusao(Aponta* lista, int arquivo, int indice, Pilha *PED)
{
  Lista* aux = RetornaNo(lista,indice-1);

  int nrr = aux->posicao; /* Posicao relativa do resgistro. */

  RemoveIndice(lista, indice-1);

  FILE *fp;
  if(arquivo == 1)
    fp = fopen("lista1.txt", "r+");
  else
    fp = fopen("lista2.txt", "r+");
  fseek(fp, nrr, SEEK_SET);  /*A partir do começo, 1 registro possui 64 bytes*/
  fprintf(fp, "*"); /* Adicionando marcador ao 1 byte do registro para reuso */
  fclose(fp);

  InserePilha(PED, nrr, arquivo); /* Inserindo espaco na PED */
  AtualizaIndices(lista,arquivo); /* Atualizando indices */
}
/* Funcao para liberar todos elementos da lista */
void libera(Aponta* lista)
{
  Lista* q = lista->inicio;
  while (q!=NULL)
  {
      Lista* t = q->prox;
      free(q);
      q = t;
  }
  free(lista);
}
/* Funcao para remover um elemento da lista dado seu devido indice */
void RemoveIndice(Aponta *lista, int indice) 
{
  indice++;
  if(indice == 1)
  {
    RetiraInicio(lista);
    return;
  }

  if(indice == lista->tamanho)
  {
    RetiraFim(lista);
    return;
  } 
  indice--;
  Lista *atual = RetornaNo(lista,indice);
  Lista * seguinte = RetornaNo(lista,indice+1);

  if(atual != NULL)
  {
    Lista  *anterior = RetornaNo(lista,indice-1); 
    if(anterior == NULL)
    {
      seguinte->ant = NULL;
      return; 
    }
    anterior->prox = seguinte;
    seguinte->ant = anterior;
    lista->tamanho--;
    free(atual);
  }
}
/* Funcao para remover um elemento do fim da lista */
void RetiraFim(Aponta *lista)
{
  if (vazia(lista) == 1)
  {
    Lista *aux = lista->fim; 
    lista->fim = lista->fim->ant;
    lista->fim->prox = NULL;
    free(aux);
    lista->tamanho--;   
  }
  else 
  {
    printf("Lista esta vazia!\n");
    return ;
  }
}
/* Funcao para remover um elemento do inicio da lista */
void RetiraInicio(Aponta *lista)
{
  if( (vazia(lista) == 1))
  {
    Lista * aux = lista->inicio->prox;
    aux->ant = NULL;   
    free(lista->inicio);
    lista->inicio = aux;
    lista->tamanho--;
  }
  else 
  {
    printf("Lista esta vazia!\n");
    return ;
  }
}
/* Funcao para criar o arquivo com os indices ordenados da lista 1 e 2 */
void CriaArquivoMerge(Aponta *lista)
{
  FILE *arquivo;
  Lista *aux = lista->inicio;

  arquivo = fopen("MergeListas.ind", "w");
  fprintf(arquivo, "CHAVE-PRIMARIA\t\t\t\t\tCURSO\n");

  if(arquivo == NULL)
  {
    printf("Erro ao tentar abrir o arquivo MergeListas.ind!");
    exit(1);
  }
  for(;aux!=NULL;aux=aux->prox)
  {
    fprintf(arquivo, "%s\t", aux->chave_primaria);
    fprintf(arquivo, " %s\n", aux->curso);
  }

  fclose(arquivo);
}
/* Funcao para manipulacao de strings das listas separando cada campo */
void ManipulaString(char *linha, int desloca, int opcao, Aponta *lista)
{
  int i, k=0, eh_op=0, eh_nome=0, eh_curso=0, eh_matricula=0,eh_turma=0, tamanho = strlen(linha);
  char aux[5], chave_primaria[31], MATRIC[7], nome[43], OP[3], Curso[3], Turma;

  for(i=0;i<tamanho;i++)
  {
    if(eh_matricula == 0) /* Verifica se eh numero e armazena matric como string */
    {
      while(linha[i] != ' ' && eh_matricula == 0)
      {
        MATRIC[i] = linha[i];
        i++;
      }
      MATRIC[i] = '\0';
      eh_matricula = 1;
    }

    if( linha[i] == ' '  && eh_nome ==0) /* Armazena Nome */
    {
      i++;
      while(atoi(aux) == 0 )
      {
        nome[k] = linha[i];

        k++;
        i++;
        aux[0] = linha[i]; /* Para no ultimo caracter, antes de um numero */
      }
      nome[k] = '\0';
      k=0;
      eh_nome = 1;
    }

    if(atoi(aux) != 0 && eh_op==0 && eh_nome==1) /* Armazena OP como string */
    {
      aux[0] = linha[i];

      while(atoi(aux) != 0 )
      {
        OP[k] = linha[i];
        k++; i++;
        aux[0] = linha[i];
      }
      OP[k] = '\0';
      eh_op =1;
      i++;
      k=0;
    }
    aux[0] = linha[i];
    if( linha[i] != ' ' && eh_curso == 0 && eh_op == 1)  /* Sigla do Curso */
    {
      Curso[0] = linha[i];
      Curso[1] = linha[++i];
      Curso[2] = '\0';
      eh_curso = 1;
      i++;
    }

    if( linha[i] != ' ' && eh_turma == 0 && eh_curso == 1) /* Letra da Turma */
    {
      Turma=linha[i];
      eh_turma = 1;
    }
  }
  strcpy(chave_primaria,MATRIC);
  strncat(chave_primaria ,nome ,24); /* Cria a chave primaria a partir da concatenacao da matricula com nome */
  InsereOrdenado(lista, atoi(MATRIC), nome, Curso, Turma, atoi(OP), opcao, desloca, chave_primaria);
}
/* Funcao para criacao dos indices primarios e secundarios */
void CriaIndices(Aponta* lista,int opcao)
{
    FILE *arquivo;
    int byteoff = 0; /* Variavel que vai ser usada para controlar o byte de cada linha */
    int fim_arquivo = 0;
    char linha[100];
    
    if(opcao == 1)
    {
        arquivo = fopen("lista1.txt","rb");
    }
    else if(opcao == 2)
    {
        arquivo = fopen("lista2.txt","rb");
    }

    /* Descobrindo o tamanho do arquivo */
    fseek(arquivo, 0L, SEEK_END); /* Aponto para o fim do arquivo */
    fim_arquivo = ftell(arquivo); /* Retorna o valor do fim do arquivo */
    fseek(arquivo, 0, SEEK_SET); /* Aponta de volta para o inicio do arquivo para fazer a leitura */
    /* -------------------------------- */
    printf("\nTamanho do Arquivo Lista%d.txt: %d\n", opcao, fim_arquivo);
    if(arquivo == NULL) /* Verifica se abriu o arquivo corretamente */
    {
      printf("\nFalha ao tentar abrir o arquivo lista%d.txt\n",opcao);
    }
    else
    {
      while(!feof(arquivo)) /* Enquanto nao encontrar o fim do arquivo */
      {
        fgets(linha, sizeof(linha), arquivo);
        if (byteoff >= fim_arquivo)
          break;    
        ManipulaString(linha, byteoff, opcao, lista); /* Separa os campos da linha em strings */
        byteoff += 64; /* Cada registro possui 64 bytes */
      }
      EscreveIndicesPrimarios(lista, opcao);  /* Escreve as informaçoes necessarias no indice primario */
      EscreveIndicesSecundarios(lista, opcao);  /* Escreve as informaçoes necessarias no indice secundario */
    }
    fclose(arquivo);
}
/* Função que escreve as informações no indiceprimario1.ind ou no indiceprimario2.ind */
void EscreveIndicesPrimarios(Aponta *lista, int num_arq)
{
  FILE *arquivo;
  Lista *aux = lista->inicio;

  if(num_arq == 1)
  {
    arquivo = fopen("indiceprimario1.ind", "w");
    fprintf(arquivo, "CHAVE-PRIMARIA\t\t\tBYTE-OFFSET\n");
  }
  else
  {
    arquivo = fopen("indiceprimario2.ind", "w");
    fprintf(arquivo, "CHAVE-PRIMARIA\t\t\tBYTE-OFFSET\n");
  }

  if(arquivo == NULL)
  {
    printf("Erro ao tentar abrir o arquivo indicelista%d.ind!", num_arq);
    exit(2);
  }
  for(;aux!=NULL;aux=aux->prox)
  {
    fprintf(arquivo, "%s\t", aux->chave_primaria);
    fprintf(arquivo, "%d\n", aux->posicao);
  }

  fclose(arquivo);
}
/* Funcao que inclui as informacoes de um novo registro na Lista */
void IncluiRegistroLista(Aponta *lista, int arquivo, Pilha *PED)
{
  FILE *fp;
  int  num_arq, posicao=0,n,vezes,mat;
  char string[100],nome[43], turma[3], matricula[7], curso[2], op[6], chave_primaria[31], turmac;

  printf("Qual é a matrícula do novo aluno?\n");
  scanf("%[^\n]%*c", matricula);
  strcpy(string, matricula);
  mat = atoi(string);
  sprintf(chave_primaria, "%s", string);

  printf("Qual é o nome do novo aluno?\n");
  scanf("%[^\n]%*c", nome);
  strncat(chave_primaria, nome, 24);

  printf("Qual é o OP do novo aluno?\n");
  scanf("%[^\n]%*c", op);

  printf("Qual é o curso do novo aluno?\n");
  scanf("%[^\n]%*c", curso);

  printf("Qual é a turma do novo aluno?\n");
  scanf("%[^\n]%*c", turma);

  strcat(string, " ");
  strcat(string, nome);
  n = strlen(string);
  vezes = 48 - n;
  for(int i = 0; i < vezes; i++)
      strcat(string, " ");
  strcat(string, op);
  strcat(string, "  ");
  strcat(string, curso);
  strcat(string,"       ");
  strcat(string, turma);

  if ( PED->qtd != 0 && PED->arq == arquivo)
  {
    /*Caso tenha ocorrido remocoes 
    * Adiciona no lugar onde tem espaço? -> PED */
    int nrr = RemovePilha(PED);
    posicao = nrr;
    if (arquivo == 1)
      fp = fopen("lista1.txt", "r+");
    else
      fp = fopen("lista2.txt", "r+");
    fseek(fp, nrr, SEEK_SET);
    fprintf(fp,"%s",string);
    fclose(fp);
  }
  else
  {
    if(arquivo == 1)
    {
      fp = fopen("lista1.txt","a");
      fprintf(fp,"%s\n",string);
      fclose(fp);
      posicao = lista->fim->posicao + 64;
    }
    else
    { 
      fp = fopen("lista2.txt","a");
      fprintf(fp,"%s\n",string);
      fclose(fp);
      posicao = lista->fim->posicao + 64;
    }
  }
  turmac=turma[0];
  InsereOrdenado(lista, mat, nome, curso, turmac, atoi(op), arquivo, posicao, chave_primaria);
  AtualizaIndices(lista,arquivo);
}
/*Funcao para fazer o merging das duas listas */
Lista *CombinaListas(Lista *esquerda, Lista *direita)
{
  if (esquerda==NULL)
  {
    return direita;
  } 
  else if (direita==NULL)
  {
    return esquerda;
  } 
  else if (strcmp(esquerda->chave_primaria, direita->chave_primaria) < 0)
  {
    esquerda->prox = CombinaListas(esquerda->prox,direita);
    (esquerda->prox)->ant = esquerda;
    esquerda->ant = NULL;
    return esquerda;
  } 
  else 
  {
    direita->prox = CombinaListas(esquerda,direita->prox);
    (direita->prox)->ant = direita;
    direita->ant = NULL;
    return direita;
  }
}
/* Função que escreve as informações no indicesecundario1.txt ou no indicesecundario2.txt */
void EscreveIndicesSecundarios(Aponta *lista, int num_arq)
{
  FILE *arquivo;
  Lista *aux = lista->inicio;

  if(num_arq == 1)
  {
    arquivo = fopen("indicesecundario1.ind", "w");
    fprintf(arquivo, "CURSO\t\tCHAVE PRIMARIA\n");
  }
  else
  {
    arquivo = fopen("indicesecundario2.ind", "w");
    fprintf(arquivo, "CURSO\t\tCHAVE PRIMARIA\n");
  }

  if(arquivo == NULL)
  {
    printf("Erro ao tentar abrir o arquivo indicesecundario%d.ind!", num_arq);
    exit(2);
  }
  for(;aux!=NULL;aux=aux->prox)
  {
    fprintf(arquivo, "%s\t", aux->curso);
    fprintf(arquivo, "%s\n", aux->chave_primaria);
  }

  fclose(arquivo);
}
/* Função que regrava os arquivos indices caso haja mudanças */
void AtualizaIndices(Aponta *lista, int num_arq)
{
  EscreveIndicesPrimarios(lista, num_arq);
  EscreveIndicesSecundarios(lista, num_arq);
}
/* Mostra indices primarios com seus respectivos PRRs */
void MostrarChavesPrimarias(Aponta *lista,int arquivo) 
{
  int i=0;
  Lista *aux = lista->inicio;
  printf("Indice primario do arquivo %d:\n\n",arquivo);
  printf("Chave Primaria                |PRR|ID\n");
  for(;aux!=NULL;aux=aux->prox)
  {
    ++i;
    printf("%s|%d|%d\n", aux->chave_primaria, aux->posicao, i);
  }
}
/* Mostra indice secundarios com seus respectivos ID */
void MostrarChavesSecundarias(Aponta *lista,int arquivo)
{
  int i=0;
  Lista *aux = lista->inicio;
  printf("Indice secundario do arquivo %d:\n\n",arquivo);
  printf("Curso | Chave Primaria           |ID\n");
  for(;aux!=NULL;aux=aux->prox)
  {
    ++i;
    printf("%s|%s|%d\n", aux->curso, aux->chave_primaria, i);
  }
}
/* Função que verifica o registro a partir do curso */
int verifica_curso(char *curso)
{
  if(strcmp(curso , "CC") == 0)
    return 1;
  else if(strcmp(curso , "EC") == 0)
    return 2;
  else if (strcmp(curso , "EM") == 0)
    return 3;
return 0;
}
/*Funcao que cria a lista invertida*/
ListaInvertida* CriaInvertida()
{
  return NULL;
}
/*Funcao que inicializa a lista invertida*/
ListaInvertida* InicializaInvertida(int id)
 {
  ListaInvertida* novo = (ListaInvertida*) malloc(sizeof(ListaInvertida));
  novo->id = id;
  novo->prox = NULL;
  return novo;
}
/*Funcao que insere elementos na lista invertida*/
void InsereInvertida(ListaInvertida* pai, char *curso , char *chave_primaria)
{
  ListaInvertida* filho = (ListaInvertida*) malloc(sizeof(ListaInvertida));
  strcpy(filho->chave_primaria , chave_primaria);
  strcpy(filho->curso , curso);
  filho->prox = NULL;
  if(pai->prox == NULL)
  {
    pai->prox = filho;
  }
  else
  {
    ListaInvertida* vertice = pai;
    ListaInvertida* temp;
    while(vertice != NULL)
    {
      temp = vertice;
      vertice = vertice->prox;
    }
    temp->prox = filho;
  }
}
/* Funcao que mostra elementos filhos na lista invertida */
void printa_filho(ListaInvertida* n)
{

  if(n == NULL)
    return;

  printf("\n%s  %s", n->chave_primaria,n->curso);
  printa_filho(n->prox);
}
/* Funcao que mostra elementos pais na lista invertida */
void printa_pai(ListaInvertida* n)
{
  if(n == NULL)
    return;

  printf("Chave Primaria: \t\tCurso:\t");
  printa_filho(n->prox);
  printf("\n");
}
/* Funcao que mostra a lista invertida de determinado arquivo */
void MostraListaInvertida(Aponta *Lista, ListaInvertida **ListaInvertida,int arquivo)
{
  int i;
  BuscaCurso(Lista, ListaInvertida, arquivo);
  for (i = 0; i < 3; i++ )
  {
    printf("\n");
    printa_pai(ListaInvertida[i]);
  }

}
/* Funcao que pesquisa o curso a partir da lista invertida */
void BuscaCurso(Aponta *lista, ListaInvertida** pai, int arquivo)
{
  int recebe;
  Lista *aux = lista->inicio;
  for(; aux != NULL; aux = aux->prox ) /* Imprime todas as informações até chegar ao fim da Lista */
  {
    recebe = verifica_curso(aux->curso);

   if(recebe == 1 && aux->num_arq == arquivo) /* CC*/
      InsereInvertida(pai[0] , aux->curso , aux->chave_primaria);
    if(recebe == 2 && aux->num_arq == arquivo) /* EC*/
      InsereInvertida(pai[1] , aux->curso , aux->chave_primaria);
    if(recebe == 3 && aux->num_arq == arquivo) /* EM*/
      InsereInvertida(pai[2] , aux->curso , aux->chave_primaria);
  }
}
/* Main do programa */
int main()
{
  int i,opcao;
  Pilha *PED = CriaPilha();
  Aponta *Lista1 = CriaLista();
  Aponta *Lista2 = CriaLista();
  Aponta *Lista3 = CriaLista();

  ListaInvertida** ListaInvertida1 = (ListaInvertida**) malloc(sizeof(ListaInvertida*));
  ListaInvertida** ListaInvertida2 = (ListaInvertida**) malloc(sizeof(ListaInvertida*));
  /* For para alocar a lista invertida com respeito a adjacencia com os cursos */
  for(i=0 ; i < 9; i++)
  {
   ListaInvertida1[i] = InicializaInvertida(i+1);
   ListaInvertida2[i] = InicializaInvertida(i+1);
  }

  CriaIndices(Lista1,1);
  CriaIndices(Lista2,2);

  do
  {
    opcao = menu();
    escolha(opcao , Lista1, Lista2, Lista3, ListaInvertida1, ListaInvertida2, PED);
  }while( opcao != 0);

  libera(Lista1);
  libera(Lista2);
  Lista1 = NULL;
  Lista2 = NULL;
  return 0;
}