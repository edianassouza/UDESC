#include <stdio.h>
#include <stdlib.h>

typedef struct no_thread{
    int x;
    struct no_thread *prox;
} no_thread;

typedef struct lista_thread{
    no_thread *inicio;
} lista_thread;

lista_thread* cria_lista_thread();
void insere_fim_thread(int, lista_thread*);
void insere_inicio_thread(int, lista_thread*);
void insere_pelo_indice_thread(lista_thread*, int, int);
int is_empty_thread(lista_thread*);
void imp_lista_thread(lista_thread*);
int tamanho_thread(lista_thread*);
int excluir_cabeca_thread (lista_thread*);
void excluir_no_pelo_indice_thread (lista_thread*, int);
void lista_contida_thread (lista_thread*, lista_thread*);
int devolve_indice_maior_elemento_thread(lista_thread*);
no_thread* devolve_no_pelo_indice_thread (lista_thread*, int);
int devolve_indice_pelo_no_thread(lista_thread*, no_thread*);
void indice_maiores_elementos_thread(lista_thread*);

void indice_maiores_elementos_thread (lista_thread *lis){
	if (is_empty_thread(lis)){
		printf("-1");
		return;
	}
    no_thread *aux = lis->inicio;
 	int maior=0;
    int i=0;
    
    lista_thread *result = cria_lista_thread();
    
    while(aux != NULL){
    	if (aux->x >= maior){
    		maior = aux->x;
    	}

    	aux = aux->prox;
    }
    
    
    aux = lis->inicio;
    while(aux != NULL){
		i++;
    	if (aux->x == maior){
    		insere_fim_thread(i, result);
    	}

    	aux = aux->prox;
    }
    
    imp_lista_thread(result);   		
}

lista_thread *cria_lista_thread() {
    lista_thread *l = (lista_thread*) malloc(sizeof(lista_thread));
    l->inicio = NULL;

    return l;
}

void insere_fim_thread(int x, lista_thread *l) {
    //verifica se a lista esta vazia
    if (is_empty_thread(l)) {
        insere_inicio_thread(x, l);
        return;
    }

    //criando um novo no para minha lista
    no_thread *n = (no_thread*) malloc(sizeof(no_thread));
    //como estou inserindo no final, o ponteiro proximo sera null
    n->prox = NULL;
    //recebendo o valor
    n->x = x;

    //criando um no auxiliar para percorrer a lista
    no_thread *aux = l->inicio;
    //enquanto o ponteiro que aponta pro proximo for diferente de vazio
    while (aux->prox != NULL) {
        aux = aux->prox;
    }
    //o penultimo vai começar a apontar pro ultimo;
    aux->prox = n;
}

void insere_inicio_thread(int x, lista_thread *l) {
    //criando um novo no para minha lista
    no_thread *n = (no_thread*) malloc(sizeof(no_thread));
    //recebendo o valor
    n->x = x;
    //esse novo no vai apontar pro que era o inicio da lista
    n->prox = l->inicio;
    //o inicio da lista vai apontar pro novo no
    l->inicio = n;
}

void insere_pelo_indice_thread(lista_thread* lis, int x, int indice){
    if (indice==0){
        insere_inicio_thread(x, lis);
    }else {
        no_thread *auxA = devolve_no_pelo_indice_thread(lis, indice);

        if (auxA != NULL){
            no_thread *auxB = devolve_no_pelo_indice_thread(lis, indice-1);
            no_thread *n = (no_thread*) malloc (sizeof(no_thread));
            n->x = x;
            auxB->prox = n;
            n->prox = auxA;    
        }
        
    }
}

//retorna 1 se a lista esta vazia
int is_empty_thread(lista_thread *l) {
    return l->inicio == NULL;
}

void imp_lista_thread(lista_thread *l) {
    no_thread *aux = l->inicio;

    while (aux != NULL) {
        printf("%d ", aux->x);
        aux = aux->prox;
    }
    printf("\n");
}

int tamanho_thread(lista_thread *l) {
    int tam = 0;
    no_thread *aux = l->inicio;

    while (aux != NULL) {
        tam++;
        aux = aux->prox;
    }

    return tam;
}

int excluir_cabeca_thread (lista_thread *lis){
    if (!is_empty_thread(lis)){
        //ponteiro auxiliar aponta pro primeiro no da lista;
        no_thread *aux = lis->inicio;
        //cabeça da lista, que
        int n = aux->x;
        lis->inicio = aux->prox;
        free(aux);
        return n;
    } 
}

void excluir_no_pelo_indice_thread (lista_thread *lis, int indice){
    if (indice==0){
        excluir_cabeca_thread(lis);
    } else {
        no_thread *auxA = devolve_no_pelo_indice_thread(lis, indice);

        if (auxA != NULL){
            no_thread *auxB = devolve_no_pelo_indice_thread(lis, indice - 1);
            auxB->prox = auxA->prox;

            free(auxA);
        }
    }
}

int devolve_indice_maior_elemento_thread(lista_thread * l){
    no_thread *aux = l->inicio;
 	int maior=0;
    int indice_maior=0, i=0;
    while(aux != NULL){
    	i++;
    	if (aux->x > maior){
    		maior = aux->x;
    		indice_maior = i;
    	}

    	aux = aux->prox;
    }
    return indice_maior;
    		
}

no_thread* devolve_no_pelo_indice_thread (lista_thread *lis, int indice){
    if (indice>=0 && indice < (tamanho_thread(lis)-1)){
        no_thread* aux = lis->inicio;

        int i;
        for (i=0; i< indice; i++){
            aux = aux->prox;
        }
        return aux;
    }
    return 0;
}

int devolve_indice_pelo_no_thread (lista_thread *lis, no_thread *x){
    if (x!=NULL){
        no_thread *aux = lis->inicio; 
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

void lista_contida_thread (lista_thread *lis1, lista_thread *lis2){
    int cont=0;
    if (tamanho_thread(lis1) > tamanho_thread(lis2)) {
        printf("nao");
        return;
    } else {
        no_thread *aux1 = lis1->inicio;
        no_thread *aux2 = lis2->inicio;


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

    if (tamanho_thread(lis1) == cont){
        printf("sim");
    } else {
        printf("nao");
    }
}