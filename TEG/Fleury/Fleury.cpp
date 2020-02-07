/*
Alunas: Êrica Peters e Ediana Souza.
Algoritmo de Fleury para identificar um caminho euleriano.
	Primeiro verificamos se o grafo possui um caminho (se é conexo e tem no máximo 2 vértices com grau ímpar);
	Depois percorremos o grafo, verificando as arestas que não são pontes, até voltarmos ao vértice de partida;
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
	void removerAresta(int origem, int destino);
 
	// faz uma DFS a partir de um vértice
	int dfs(int raiz);
	bool eh_conexo(int tam);
	int graus(int vertice);
	void verticePartida(int tam);
	void Fleury(int u);
	bool arestaValida(int u, int v);
	int graus_impares(int tam);
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

void Grafo::removerAresta(int origem, int destino)
{
  // colocando 
   list<int>::iterator iorigem = find(adj[origem].begin(), adj[origem].end(), destino);
  *iorigem = -1;
 
  // Find u in adjacency list of v and replace it with -1
  list<int>::iterator idestino = find(adj[destino].begin(), adj[destino].end(), origem);
  *idestino = -1;
}

int Grafo::graus(int vertice){
	return adj[vertice].size();
}

int Grafo::dfs(int raiz)
{
	int cont=0;
	stack<int> pilha;
	bool visitados[tam]; // vetor de visitados


	// marca todos como não visitados
	for(int i = 0; i < tam; i++)
		visitados[i] = false;
 
	while(true)
	{
		if(!visitados[raiz])
		{
			cont++;
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

		if((vizinhos==true) && (*i!=-1)){
				raiz = *i;
		}
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
			}
	}
	return cont;
}

bool Grafo::eh_conexo(int tam){
	int aux, cont=0;
	for(int i=0; i<tam; i++){
		aux = dfs(i);
		if(aux == tam)
			cont++;
	}
	if(cont == tam)
		return true;
	return false;
}


void Grafo::verticePartida(int tam)
{
	//procura vertice com grau impar
  	int u = 0; 
  	for (int i = 0; i < tam; i++){
      if (adj[i].size() & 1){  
      	u = i;
      	break;
      }
  	}
 
  // Iniciar caminho
  Fleury(u);
  cout << endl;
}

void Grafo::Fleury(int u)
{
  list<int>::iterator i;

for (i = adj[u].begin(); i != adj[u].end(); i++)
  {
       int v = *i;
 
      // Se for -1 ou a aresta nao for valida, nao posso entrar no loop
      if ((v!=-1) && (arestaValida(u, v)))
      {
          cout << u << "-" << v << "  ";
          removerAresta(u, v);
          Fleury(v);
      }
  }
}
 
bool Grafo::arestaValida(int u, int v)
{
  //A aresta eh valida em dois casos:
 
  //Se v eh o unico vertice adjacente de u
  int count = 0;
  list<int>::iterator i;
  for (i = adj[u].begin(); i != adj[u].end(); ++i)
  	if (*i!=-1)
        count++;
  if (count == 1)//v eh o unico adjacente de u
    return true;
 
  //Ou se u-v nao for uma ponte
  //verificar se eh ponte
  int count1 = dfs(u);

  removerAresta(u, v);
  int count2 = dfs(u);
 
  adicionarAresta(u, v);
 
  //se count1>count2 eh uma ponte
  if(count1>count2)
  	return false;
  return true;
}

int Grafo::graus_impares(int tam){
	int cont=0, x;
	for(int i=0; i<tam; i++){
		x = graus(i);
		if(x%2!=0)
			cont++;
	}
	return cont;
}

int main()
{
	int tam, raiz; 

	FILE *arquivo;
	arquivo = fopen("entradaLista2.txt", "r+");
	fscanf(arquivo, "%d\n", &tam);
   
    Grafo grafo(tam);

    int origem, destino;
    
    //lendo arquivo
    while(!feof(arquivo)){
    	fscanf(arquivo, "%d %d\n", &origem, &destino);
    	grafo.adicionarAresta(origem, destino);
    }


    if(grafo.graus_impares(tam)<=2 && grafo.eh_conexo(tam)==true){
    	cout << "Caminho Euleriano: ";
    	grafo.verticePartida(tam);
    }
    else{
    	cout << "Nao possui caminho euleriano\n";
    }

	return 0;
}