#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <bits/stdc++.h>
#define INFINITO 10000000

using namespace std;

class Grafo
{
private:
	int V; // número de vértices

	/*ponteiro para um array contendo as listas de adjacências.
	Exemplo: adj[0].push_back(make_pair(1, 2). 
	Onde 1 é o vertice vizinho e 2 é o peso da aresta*/	
	list<pair<int, int> > * adj;

public:
	//Construtor
	Grafo(int V)
	{
		this->V = V; // atribui o número de vértices

		/*
			cria as listas onde cada lista é uma lista de pairs 
			onde cada pair é formado pelo vértice destino e o custo
		*/
		adj = new list<pair<int, int> >[V];
	}



	//Função que adiciona uma aresta ao grafo de origem ao destino, com custo
	void adicionarAresta(int origem, int destino, int custo)
	{
		adj[origem].push_back(make_pair(destino, custo));
	}



	//Função que implementa o algoritmo de Dijkstra
	void dijkstra(int orig)
	{

		cout<< "SAINDO DO VERTICE " << orig << "\n\n";

		/*vetor de distâncias, onde armazena os pesos conforme for 
		percorrendo o caminho da origem ao destino*/
		int distancia[V];

		/*vetor de visitados serve para caso o vértice já tenha sido
		   expandido (visitado), não expandir mais*/
		int visitados[V];

		/*vetor que marca os vertices que formam o caminho*/
		int antecessor[V];

		/*fila de prioridades de pair (distancia, vértice)
		veja, qual dos vertices adjacentes a um vertice, deve visitar por primeiro
		priority_qeue: o primeiro elemento será sempre o maior dos elementos que ele contém*/
		priority_queue < pair<int, int>,
					   vector<pair<int, int> >, greater<pair<int, int> > > pq;

		//inicia o vetor de distâncias e visitados
		for(int i = 0; i < V; i++)
		{
			//Coloca a distancia infinito em todos
			distancia[i] = INFINITO;
			//Coloca false pois ainda nao visitou nenhum
			visitados[i] = false;
		}

		//a distância de origem para origem é 0
		distancia[orig] = 0;

		//insere na fila a distancia da origem
		pq.push(make_pair(distancia[orig], orig));

		antecessor[orig] = orig;

		//loop do algoritmo
		while(!pq.empty())
		{
			pair<int, int> p = pq.top(); // extrai o pair do topo, ou seja, qual vertice adjcente a algu vertice deve visitar
			int u = p.second; //obtém o vértice do pair
			pq.pop(); //remove da fila, pois será visitado

			//verifica se o vértice escolhido não foi expandido
			if(visitados[u] == false)
			{
				//marca como visitado
				visitados[u] = true;

				list<pair<int, int> >::iterator it;

				//percorre os vértices adjacentes de "u"
				for(it = adj[u].begin(); it != adj[u].end(); it++)
				{
					//obtém o vértice adjacente e o custo da aresta
					int v = it->first;
					int custo_aresta = it->second;

					//relaxamento(u, v)
					if(distancia[v] > (distancia[u] + custo_aresta))
					{
						//atualiza a distância de "v" e insere na fila
						distancia[v] = distancia[u] + custo_aresta;
						antecessor[v] = u;
						pq.push(make_pair(distancia[v], v));
					}
				}
			}
		}

	for(int dest=0; dest<V; dest++){
		printf("VERTICE %d\n", dest);
		if (distancia[dest] == INFINITO){
			printf("NAO HA CAMINHO\n");
			printf("\n");
		}
		else{
			int v = dest;
			std::string caminho;
			printf("CAMINHO: ");
			caminho.append(std::to_string(dest));
			while (antecessor[v] != v){
				caminho.append(" ");
				caminho.append(std::to_string(antecessor[v]));
				v = antecessor[v];
			}
		
			reverse(caminho.begin(), caminho.end()); 
			cout << caminho << endl; 

			// retorna a distância mínima até o destino
			printf("DISTANCIA: %d\n", distancia[dest]);
			printf("\n");
		}
	}
}};

int main(int argc, char *argv[])
{
	int tam, vpartida; 

	FILE *arquivo;
	arquivo = fopen("in1.txt", "r+");
	fscanf(arquivo, "%d\n", &tam);
	fscanf(arquivo, "%d\n", &vpartida);
   
    Grafo grafo(tam);

    int origem, destino, peso;
    
    while(!feof(arquivo)){
    	fscanf(arquivo, "%d %d %d\n", &origem, &destino, &peso);
    	grafo.adicionarAresta(origem, destino, peso);
    }

	grafo.dijkstra(vpartida);

	return 0;
}