#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca_lista.h"
#include "biblioteca_lista_thread.h"
#include "utils.h"
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>

#define FALSE 0
#define TRUE 1

lista* inicializa_lista_candidatos(char* nome_arquivo);
void *contagem_votos(void *i);
void vericar_voto(char* nome_arquivo, lista *lis);
void *consome_voto();
void mostrando_resultados(lista *lis);

pthread_barrier_t barreira; 
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;

unsigned int terminaram = 0;
char* nome_arquivo;
lista *lista_candidatos;
lista_thread *lista_contagem; 

long votos_totais, votos_invalidos;
int n_threads;
int tamanho_max = 0;

int main(int argc, char *argv[]){

	pthread_cond_init(&condc, NULL);
	pthread_cond_init(&condp, NULL);

	//Preenchendo os valores das variáveis globais
	n_threads = atoi(argv[1]);
	nome_arquivo = argv[2];

	if (argc != 3){
		printf("\nerro nos parametros do main\n");
		exit(1);
	}

	FILE *arquivo_teste=NULL;
	arquivo_teste = fopen(argv[2],"r");
    if(arquivo_teste == NULL){
    	printf("Não foi possível ler o arquivo na main\n");
    	exit(1);
	}


	//A lista recebe os dados dos candidatos, ou seja, leitura do arquivo. A crianção da lista ocorre na pŕopria função
	lista_candidatos = inicializa_lista_candidatos(nome_arquivo);

	//Criando a lista de contagem para os votos
	lista_contagem = cria_lista_thread();
	
	//Referente à barreira das threads
	pthread_t threads[n_threads+1];
	int rc; 
	rc = pthread_barrier_init(&barreira, NULL, n_threads);
	if (rc!=0){
		perror("\nerro em pthread_barrier_init()");
		exit(1);
	}

	//Criando mutex
	pthread_mutex_init(&the_mutex, NULL);

	//Criando as threads
	long i;
	for (i=1; i<=n_threads; i++){
		//printf("criando thread %ld\n", i);
		rc = pthread_create(&threads[i], NULL, contagem_votos, (void *)i);
		 if (rc != 0) {
	       perror("erro em pthread_create()");
	       exit(1);
	  }
	}

	//Criando thread consumidora
	pthread_t consumidor;
	rc = pthread_create(&consumidor, NULL, consome_voto, NULL);
	if (rc != 0) {
	       perror("erro em pthread_create()");
	       exit(1);
	}

	rc = pthread_join(consumidor, NULL);
	if (rc != 0) {
	       perror("erro em pthread_join()");
	       exit(1);
	}

	imp_lista_thread(lista_contagem);

	void *ret;
	//Encerrando as threads
	for (i=1; i<=n_threads; i++){
		rc = pthread_join(threads[i], &ret);
		if (rc != 0) {
	       perror("erro em pthread_join()");
	       exit(1);
	  }
	  votos_totais+=(long) ret;
	}

	mostrando_resultados(lista_candidatos);
	pthread_exit(NULL);
}

void mostrando_resultados(lista *lis){

	//criando arquivo de saida
	char *saida = malloc(sizeof(char)*20); 
	sprintf(saida, "saida-%d-%s", n_threads, nome_arquivo);

	//Abrindo o arquivo de saida
	FILE* arquivo;
	arquivo = fopen(saida, "w");

	printf("Total de votos processados: %ld\n", votos_totais);
	fprintf(arquivo, "Total de votos processados: %ld\n", votos_totais);
	long votos_validos = votos_totais - votos_invalidos;
	double porcentagem = (100/(double)votos_totais) * (double)(votos_validos); 
	printf("Votos validos: %ld (%.2f) \n", votos_validos, porcentagem);
	fprintf(arquivo, "Votos validos: %ld (%.2f) \n", votos_validos, porcentagem);
	printf("Tamanho maximo da fila: %d nos\n\n", tamanho_max);
	fprintf(arquivo, "Tamanho maximo da fila: %d nos\n\n", tamanho_max);
	printf("Resultado final:\n");
	fprintf(arquivo, "Resultado final:\n");
	printf(fmtcabvoto);
	fprintf(arquivo, fmtcabvoto);

	int indice_maior;
	no *aux = lis->inicio;
    while (tamanho(lis)!=1) {
    	indice_maior = devolve_indice_maior_elemento(lis);
    	for (int i=0; i<indice_maior-1; i++){
    		aux = aux->prox;

    	}
    	porcentagem = (100/(double)votos_validos)*((double)aux->n_votos);
       	printf("%8u  %-30s  %13u  %6.2f\n", aux->x, aux->nome, aux->n_votos, porcentagem);
       	fprintf(arquivo, "%8u  %-30s  %13u  %6.2f\n", aux->x, aux->nome, aux->n_votos, porcentagem);
        excluir_no_pelo_indice(lis, indice_maior-1);
        aux = devolve_no_pelo_indice(lis, 0);
    }
    porcentagem = (100/(double)votos_validos)*((double)aux->n_votos);
    printf("%8u  %-30s  %13u  %6.2f\n", aux->x, aux->nome, aux->n_votos, porcentagem);
    fprintf(arquivo, "%8u  %-30s  %13u  %6.2f\n", aux->x, aux->nome, aux->n_votos, porcentagem);
    excluir_no_pelo_indice(lis, 0);

}

//Função da thread consumidora
void *consome_voto(){

	int n;

	pthread_mutex_lock(&the_mutex);
	while((is_empty_thread(lista_contagem)!=1) || (terminaram < n_threads)) {
		while((is_empty_thread(lista_contagem)==1) && (terminaram < n_threads)) {
			pthread_cond_wait(&condc, &the_mutex);
		}
		if (is_empty_thread(lista_contagem) != 1) {
			n = excluir_cabeca_thread (lista_contagem);
			pthread_mutex_unlock(&the_mutex);
			insere_votos(n, lista_candidatos);
			pthread_mutex_lock(&the_mutex);
        } 
		//pthread_mutex_lock(&the_mutex);
		//printf("...contando voto %d\n", n);
	}
	pthread_mutex_unlock(&the_mutex);
	pthread_exit(NULL);
}

//Função que cria as threads, barreira, le arquivo e insere os votos
void *contagem_votos(void *i){
	long total = 0; 
	long invalidos = 0; 
	long t = (long) i;
	//printf("\nChegando na barreira: %ld \n", t);

	//Esperando as duas threads ficarem prontas 
	pthread_barrier_wait(&barreira);
	//printf("\nSaindo da barreira: %ld\n", t);

	//Num que lê o voto no arquivo
	char *num = malloc(sizeof(char)*10);
	FILE *arquivo; 

	//casting de long para int. Fez-se isso para conseguir usar a função sprintf
	int aux = (int) t;
	char numero[20];
	sprintf(numero, "%d", aux);

	//Concatenando
	char *risco = "-";
	char *n_arquivo = malloc(sizeof(char)*10);
	strcpy(n_arquivo, nome_arquivo);
	strcat(n_arquivo, risco);
	strcat(n_arquivo, numero);

	//Abrindo o arquivo
	arquivo = fopen(n_arquivo, "r+");

	if(arquivo == NULL){
    	printf("Não foi possível ler o arquivo na main\n");
	}

	//Criação da variavel para ler a linha do arquivo
	char linha[50];

	//Criação do auxx para usar na função strtol
	char **auxx; 

	//Criou-se o aux0 para poder comparar na função strcmp
	char *aux0 = malloc(sizeof(char)*1);
	aux0 = "0";


	//Inicia a leitura do arquivo
	while(fgets(linha, sizeof(linha), arquivo) != NULL){
		sscanf(linha, "%s\n", num);

		//Garantindo o acesso a uma só thread
		

		pthread_mutex_lock(&the_mutex);

		//while(is_empty_thread(lista_contagem)!=1){
		//	pthread_cond_wait(&condp, &the_mutex);
		//}

		//Verifica se o valor é 0, pois a função strcmp retorna zero se igual
		if ((strcmp(num,aux0) == 0)){
			//Verifica se o valor 0 pertence realmente a um candidato
			if(verifica_numero(0, lista_candidatos)){
				insere_fim_thread(0, lista_contagem);
				//printf("Thread %d inserindo %d\n", aux, 0);
				total++;
				if(tamanho_thread(lista_contagem)>tamanho_max)
					tamanho_max = tamanho_thread(lista_contagem);
			}
			//Se não pertencer a nenhum candidato, é um voto inválido
			else
				invalidos++;
		} else {
			
			//Verifica se é numérico, se não for, coloca zero.
			unsigned int voto = (unsigned int) strtol(num, auxx, 10);

			//Se for diferente de zero, é número
			if (voto != 0){
				//Verifica se o valor pertence a algum candidato
				if(verifica_numero(voto, lista_candidatos)==TRUE){
					insere_fim_thread(voto, lista_contagem);
					//printf("Thread %d inserindo %d\n", aux, voto);
					total++;
					if(tamanho_thread(lista_contagem)>tamanho_max)
						tamanho_max = tamanho_thread(lista_contagem);
				}
			//Se não pertencer a nenhum candidato, é um voto inválido
				else
					invalidos++;
			}
			//Se for igual a zero, significa que a função strol retornou zero, indicando que não é valor númerico. Portanto, voto inválido
			else
				invalidos++;
		}
		pthread_cond_signal(&condc);

		pthread_mutex_unlock(&the_mutex);
	}

	//printf("\nThread %d tem %d totais e %d invalido\n", aux, total, invalidos);

	//Recebe vazio para limpar a concatenação
	n_arquivo = "";

	votos_invalidos += invalidos;
	pthread_mutex_lock(&the_mutex);
	terminaram++;
	pthread_cond_signal(&condc);	
	pthread_mutex_unlock(&the_mutex);

	//Termina as threads
	pthread_exit((void *)total+invalidos);
	
}

//Função que cria a lista de candidatos, lê arquivo e insere os dados na lista
lista* inicializa_lista_candidatos(char* nome_arquivo){

	//Criando a lista
	lista *lista_candidatos = cria_lista();

	//Variaveis necessárias
	unsigned int numero;
	char *nome = malloc(sizeof(char)*30);
	FILE *arquivo;
	arquivo = fopen(nome_arquivo, "r+");

	//Variavel que vai ler cada linha do arquivo
	char linha[50];

	//Lendo o arquivo
	while(fgets(linha, sizeof(linha), arquivo) != NULL){

		//%30 foi usado para ler como capacidade máxima, sendo que [^] usado para não para de ler quando chegar no espaço
		sscanf(linha, "%d %30[^\n]s", &numero, nome);

		//Verificando se o anterior é nulo, por casa do memset. Se nulo, não irá inserir na lista
		if (nome[0]!= ' '){
			insere_fim(numero, nome, lista_candidatos);	
		}

		//limpando a variavel do sobrenome
		memset(nome, ' ', 30); 
	}

	return lista_candidatos;
}