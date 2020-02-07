#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

typedef struct no {
    unsigned int x;
    char *nome;
    int n_votos;
    struct no *prox;
} no;

typedef struct lista {
    no *inicio;
} lista;

lista* cria_lista();
void insere_fim(unsigned int, char*, lista*);
void insere_inicio(unsigned int, char*, lista*);
int is_empty(lista*);
void imp_lista(lista*);
int tamanho(lista*);
void excluir_cabeca (lista*);
void excluir_no_pelo_indice (lista*, int);
void lista_contida (lista*, lista*);
int devolve_indice_maior_elemento(lista*);
no* devolve_no_pelo_indice (lista*, int);
int devolve_indice_pelo_no(lista*, no*);
void insere_votos(int voto, lista* lis);
int verifica_numero(int numero, lista *lis);


lista *cria_lista() {
    lista *l = (lista*) malloc(sizeof(lista));
    l->inicio = NULL;

    return l;
}

void insere_fim(unsigned int x, char *nome, lista *l) {
    //verifica se a lista esta vazia
    if (is_empty(l)) {
        insere_inicio(x, nome, l);
        return;
    }

    //criando um novo no para minha lista
    no *n = (no*) malloc(sizeof(no));
    //como estou inserindo no final, o ponteiro proximo sera null
    n->prox = NULL;
    //recebendo o valor
    n->x = x;
    n->nome = (char *) malloc(strlen(nome)+1);
    strncpy(n->nome, nome, strlen(nome)+1);
    n->n_votos = 0;

    //criando um no auxiliar para percorrer a lista
    no *aux = l->inicio;
    //enquanto o ponteiro que aponta pro proximo for diferente de vazio
    while (aux->prox != NULL) {
        aux = aux->prox;
    }
    //o penultimo vai começar a apontar pro ultimo;
    aux->prox = n;
}

void insere_inicio(unsigned int x, char* nome, lista *l) {
    //criando um novo no para minha lista
    no *n = (no*) malloc(sizeof(no));
    //recebendo o valor
    n->x = x;
    n->nome = (char *) malloc(strlen(nome)+1);
    strncpy(n->nome, nome, strlen(nome)+1);
    n->n_votos=0;
    //esse novo no vai apontar pro que era o inicio da lista
    n->prox = l->inicio;
    //o inicio da lista vai apontar pro novo no
    l->inicio = n;
}


//retorna 1 se a lista esta vazia
int is_empty(lista *l) {
    return l->inicio == NULL;
}

void imp_lista(lista *l) {
    no *aux = l->inicio;

    while (aux != NULL) {
        printf("\n%d - %s - %d", aux->x, aux->nome, aux->n_votos);
        aux = aux->prox;
    }
    printf("\n");
}

int tamanho(lista *l) {
    int tam = 0;
    no *aux = l->inicio;

    while (aux != NULL) {
        tam++;
        aux = aux->prox;
    }

    return tam;
}

void excluir_cabeca (lista *lis){
    if (!is_empty(lis)){
        //ponteiro auxiliar aponta pro primeiro no da lista;
        no *aux = lis->inicio;
        //cabeça da lista, que
        lis->inicio = aux->prox;
        free(aux);
    } 
}

void excluir_no_pelo_indice (lista *lis, int indice){
    if (indice==0){
        excluir_cabeca(lis);
    } else {
        no *auxA = devolve_no_pelo_indice(lis, indice);

        if (auxA != NULL){
            no *auxB = devolve_no_pelo_indice(lis, indice - 1);
            auxB->prox = auxA->prox;

            free(auxA);
        }
    }
}

int devolve_indice_maior_elemento(lista * l){
    no *aux = l->inicio;
 	int maior=0;
    int indice_maior=0, i=0;
    while(aux != NULL){
    	i++;
    	if (aux->n_votos > maior){
    		maior = aux->n_votos;
    		indice_maior = i;
    	}

    	aux = aux->prox;
    }
    return indice_maior;
    		
}

no* devolve_no_pelo_indice (lista *lis, int indice){
    if (indice>=0 && indice < (tamanho(lis))){
        no* aux = lis->inicio;

        int i;
        for (i=0; i< indice; i++){
            aux = aux->prox;
        }
        return aux;
    }
    return 0;
}

int devolve_indice_pelo_no (lista *lis, no *x){
    if (x!=NULL){
        no *aux = lis->inicio; 
        int index = 0;
        while (aux != x && aux!= NULL){
            aux = aux->prox;
            index++;
        }
        if (aux != NULL){
            return index;    
        }
    }
    return -1;
}

void lista_contida (lista *lis1, lista *lis2){
    int cont=0;
    if (tamanho(lis1) > tamanho(lis2)) {
        printf("nao");
        return;
    } else {
        no *aux1 = lis1->inicio;
        no *aux2 = lis2->inicio;


        while (aux2 != NULL){
            while (aux1 != NULL){
                if (aux1->x == aux2->x){
                    cont++; 
                }
                aux1 = aux1->prox;
            }
            aux1 = lis1->inicio;
            aux2 = aux2->prox;
        }
    }

    if (tamanho(lis1) == cont){
        printf("sim");
    } else {
        printf("nao");
    }
}

void insere_votos(int voto, lista* lis){
     no* aux = lis->inicio;

        int i;
        while (aux != NULL){
            if (aux->x == voto){
                aux->n_votos++;
                return; 
            }
            aux = aux->prox;
        }
}

int verifica_numero(int numero, lista *lis){
    no* aux = lis->inicio;

        int i;
        while (aux != NULL){
            if (aux->x == numero){
                return TRUE;
            }
            aux = aux->prox;
        }
    return FALSE;
}