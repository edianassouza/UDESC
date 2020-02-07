/*
Alunas: Êrica Peters e Ediana Souza.
Algoritmo de DFS para busca em profundidade em grafos.
	Dada uma raiz, percorremos o grafo montando sua árvore de busca em largura.
	Ao final, dado um vértice, mostramos qual é seu pai, seus descendentes e seus ancestrais.
*/

#include <iostream>
#include <list>
#include <algorithm> // função find
#include <stack> // pilha para usar na DFS
 
using namespace std;
 
class Grafo
{
	int tam; // número de vértices
	list<int> *adj; //ponteiro para lista de nós adjacentes
 
public:
	Grafo(int tam);
	void adicionarAresta(int origem, int destino); // adiciona uma aresta no grafo
 
	// faz uma DFS a partir de um vértice
	void dfs(int raiz);
	void familia(int raiz, int vertice);
};
 
Grafo::Grafo(int tam)
{
	this->tam = tam; // atribui o número de vértices
	adj = new list<int>[tam]; // cria as listas
}
 
void Grafo::adicionarAresta(int origem, int destino)
{
	// adiciona vértice v2 à lista de vértices adjacentes de v1
	adj[origem].push_back(destino);
	adj[destino].push_back(origem);
}
 
void Grafo::dfs(int raiz)
{
	stack<int> pilha;
	bool visitados[tam]; // vetor de visitados
 
	// marca todos como não visitados
	for(int i = 0; i < tam; i++)
		visitados[i] = false;
 
	while(true)
	{
		if(!visitados[raiz])
		{
			cout << "Visitando vertice " << raiz << "\n";
			visitados[raiz] = true; // marca como visitado
			pilha.push(raiz); // insere a raiz na pilha
		}
 
		bool vizinhos = false;
		list<int>::iterator i;
 
		// busca por um vizinho não visitado
		for(i = adj[raiz].begin(); i != adj[raiz].end(); i++)
		{
			if(!visitados[*i]) //para ler o valor que esta no indice do iterator, precisa-se do asterisco
			{
				vizinhos = true;
				break;
			}
		}
 
		if(vizinhos)
			raiz = *i; // atualiza a raiz
		else
		{
			// se todos os vizinhos estão visitados ou não existem vizinhos
			// remove da pilha
			pilha.pop();
			// se a pilha ficar vazia, então terminou a busca
			if(pilha.empty())
				break;
			// se chegou aqui, é porque pode pegar elemento do topo
			raiz = pilha.top();
			cout << "Voltando para o vertice " << raiz << "\n";
		}
	}
}

void Grafo::familia(int raiz, int vertice)
{
	list<int> descendentes;
	list<int> ancestrais;
	int paizao; 
	int ancestral;
	bool ja_chegou = false; //flag descendentes
	bool fez_pop = false; //flag do pai
	bool eh_ancestral = false; // flag ancestrais
	bool rv = false; // flag que verifica se raiz é igual ao vertice
	stack<int> pilha;
	bool visitados[tam]; // vetor de visitados

	if(raiz == vertice)
		rv = true;

	// marca todos como não visitados
	for(int i = 0; i < tam; i++)
		visitados[i] = false;
 
	while(true)
	{
		if(!visitados[raiz])
		{
			cout << "Visitando vertice " << raiz << "\n";
			visitados[raiz] = true; // marca como visitado
			pilha.push(raiz); // insere a raiz na pilha
		}
 
		bool vizinhos = false;
		list<int>::iterator i;
 
		// busca por um vizinho não visitado
		for(i = adj[raiz].begin(); i != adj[raiz].end(); i++)
		{
			if(!visitados[*i]) //para ler o valor que esta no indice do iterator, precisa-se do asterisco
			{
				vizinhos = true;
				break;
			}
		}
 
		if(vizinhos){
			raiz = *i; // atualiza a raiz
			if(ja_chegou == true && rv==false)
				descendentes.push_back(raiz);
			if(raiz == vertice && rv==false)
				ja_chegou = true;
			if(rv == true)
				descendentes.push_back(raiz);
		}
		else
		{
			// se todos os vizinhos estão visitados ou não existem vizinhos
			// remove da pilha
			if(raiz == vertice && rv==false){
				ja_chegou=false;
				fez_pop = true;
			}
			if(raiz == ancestral && rv==false)
				eh_ancestral = true;
			pilha.pop();
			// se a pilha ficar vazia, então terminou a busca
			if(pilha.empty())
				break;
			// se chegou aqui, é porque pode pegar elemento do topo
			raiz = pilha.top();
			if(fez_pop == true && rv==false){
				paizao = raiz;
				fez_pop = false;
				ancestral = paizao;
				ancestrais.push_back(ancestral);
			}
			if(eh_ancestral == true && rv == false){
				ancestral = raiz;
				ancestrais.push_front(raiz);
				eh_ancestral = false;
			}
			cout << "Voltando para o vertice " << raiz << "\n";
		}
	}


	cout << "\n";
	if(rv == true)
		cout << "Ñao ha pai nem ancestrais pois " << vertice << " eh raiz\n";
	else{
		cout << "Pai: " << paizao << "\n";
		cout << "Ancestrais: " << "";
		for(list<int>::iterator i = ancestrais.begin(); i != ancestrais.end(); i++)
			cout << "" << *i << ", ";
		cout << "\n";
	}
 	cout << "Descendentes: " << "";
	for(list<int>::iterator i = descendentes.begin(); i != descendentes.end(); i++)
		cout << "" << *i << ", ";
	cout << "\n";

}
 
int main()
{
	int tam, raiz, vertice; 

	FILE *arquivo;
	arquivo = fopen("in1.txt", "r+");
	fscanf(arquivo, "%d\n", &tam);
   
    Grafo grafo(tam);

    int origem, destino;
    
    while(!feof(arquivo)){
    	fscanf(arquivo, "%d %d\n", &origem, &destino);
    	grafo.adicionarAresta(origem, destino);
    }
	
	//grafo.dfs(raiz);

    printf("Digite a raiz da arvore: ");
    scanf("%d", &raiz);
    printf("\n");

    grafo.dfs(raiz);

    printf("Digite o vertice para analizar: ");
    scanf("%d", &vertice);
    printf("\n");

 	grafo.familia(raiz, vertice);

	return 0;
}