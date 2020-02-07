#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int chave;
    struct no *dir;
    struct no *esq;
} no;

typedef struct arvore {
    no *raiz;
} arvore;

arvore* cria_arvore();
no* cria_no(int);
void mostraArvoreEmOrdem (no*);
void mostraArvorePreOrdem (no*);
void mostraArvorePosOrdem (no*);
void insereArvore(arvore*, int);
void buscaArvore(arvore*, int);
int altura (no*);
no* remover_no(no*, int);
int numero_nos(no*);
void free_no(no*);
void caminho(arvore*, int);

int main(){

	arvore *arv = cria_arvore();

	FILE *arquivo;
	arquivo = fopen("entrada3.txt", "r+");
   
    int valor;

	//Variavel que vai ler cada linha do arquivo
	char linha[50];

	//Lendo o arquivo
	while(fgets(linha, sizeof(linha), arquivo) != NULL){

		//%30 foi usado para ler como capacidade máxima, sendo que [^] usado para não para de ler quando chegar no espaço
		sscanf(linha, "%d", &valor);
		insereArvore(arv, valor);
	}

	printf("A RAIZ É %d \n", arv->raiz->chave);
	printf("A ARVORE POSSUI %d nos\n", numero_nos(arv->raiz));
	printf("A ALTURA É %d \n", altura(arv->raiz));

	printf("\n");
	/*Você deve visitar primeiro a sub-árvore esquerda, depois a raiz e por último a sub-árvore direita. */	
	//[E, C, D]
	printf("EM ORDEM:");
	mostraArvoreEmOrdem(arv->raiz);
	printf("\n\n");
	//[C, E, D]
	printf("PRÉ ORDEM:");
	/*Você deve visitar primeiro a raiz, depois a sub-árvore esquerda e por último a sub-árvore direita*/
	mostraArvorePreOrdem(arv->raiz);
	printf("\n\n");
	//[E, D, C]
	printf("PÓS ORDEM:");
	/*Você deve visitar primeiro a sub-árvore esquerda, depois a sub-árvore direita e por último a raiz. */
	mostraArvorePosOrdem(arv->raiz);
}


/*CRIA A ARVORE*/
arvore *cria_arvore() {
    arvore *t = (arvore*) malloc(sizeof(arvore));
    t->raiz = NULL;

    return t;
}

no* cria_no(int elem) {
	no* n = (no*)malloc(sizeof(no));
	n->chave = elem;
	n->dir = NULL;
	n->esq = NULL;
	return n;
}

/*Você deve visitar primeiro a sub-árvore esquerda, depois a raiz e por último a sub-árvore direita. */	
void mostraArvoreEmOrdem (no* raiz){
	if (raiz == NULL){
		return;
	}
	//visita o lado esquerdo
	if (raiz->esq != NULL){
		mostraArvoreEmOrdem (raiz->esq);	
	}
	//printa a raiz
	printf("%d ", raiz->chave);
	if (raiz->dir != NULL){
		mostraArvoreEmOrdem(raiz->dir);
	}
	//vai pro lado direito
}

/*Você deve visitar primeiro a raiz, depois a sub-árvore esquerda e por último a sub-árvore direita*/
void mostraArvorePreOrdem (no *raiz){
	if (raiz == NULL){
		return;
	}
	 //primeiro mostra a raiz
     printf("%d ", raiz->chave);

     //caminha na subárvore da esquerda, até nao ter mais onde ir e vai pro direito
	 mostraArvorePreOrdem(raiz->esq);

	 //caminha na subárvore da direta, até nao ter mais onde ir
	 mostraArvorePreOrdem(raiz->dir);
}

/*Você deve visitar primeiro a sub-árvore esquerda, depois a sub-árvore direita e por último a raiz. */
void mostraArvorePosOrdem (no* raiz){
	if (raiz == NULL){
		return;
	}
    //subfilhos esquerda
    mostraArvorePosOrdem(raiz->esq);
    
    //subfilhos a direita
    mostraArvorePosOrdem(raiz->dir);


    //a raiz vai ser o ultimo a ser printado
    printf("%d ", raiz->chave);
}

void insereArvore(arvore* t, int num){
	if (t->raiz ==  NULL){
		no* n = cria_no(num);
		t->raiz = n;
	}else{
		no* aux = t->raiz;
		
		while (1){
			if (num < aux->chave){
				if (aux->esq == NULL){
					aux->esq = cria_no(num);
					break;
				}
				aux = aux->esq;
			}else{ 
				if (num > aux->chave){
					if (aux->dir == NULL){
						aux->dir = cria_no(num);
						break;
					}
					aux = aux->dir;
				}
			} if (num == aux->chave){
				/*se for igual, nao posso
				inserir de novo*/
				break;
			}
		}
	}
}

void buscaArvore(arvore* t, int num){
	no *aux = t->raiz;
	while (aux!=NULL){
		if (aux->chave == num){
			printf("%d ", aux->chave);
			break;
		}else{
			if (num < aux->chave){
				printf("%d ", aux->chave);
				aux = aux->esq;
			}else{
				printf("%d ", aux->chave);
				aux = aux->dir;
				}
		}
	}
}

int altura (no *raiz){
	if (raiz == NULL){
		return 0;
	}else{
		int h_esq = altura(raiz->esq)+1;
		int h_dir = altura(raiz->dir)+1;
		if (h_esq<h_dir){
			return h_dir;
		}else{
			return h_esq;
		}
	}
}

int numero_nos(no* raiz){
	if(raiz == NULL)
		return 0;
	int altura_esq = numero_nos(raiz->esq);
	int altura_dir = numero_nos(raiz->dir);
	return(altura_esq+altura_dir+1);
}