/*

Alunas: Êrica Peters do Carmo e Ediana da Silva de Souza
Algoritmo para determinar se um grafo (direcionado ou nao direcionado) eh conexo utilizando DFS.

*/
 
#include <iostream>
#include <list>
#include <algorithm>
#include <stack>
 
using namespace std;
 
class Grafo
{
	int V; // número de vértices
	list<int> *adj; // ponteiro para um vetor que contem as listas de nos adjacentes
 
public:
	Grafo(int V); // construtor
	void insere_aresta(int v1, int v2);
 	void le_grafo_gnd(char* c);
 	void le_grafo_gd(char* c);
	int dfs(int v);
	void conexo();
};
 
Grafo::Grafo(int V)
{
	this->V = V; // numero de vertices
	adj = new list<int>[V]; // cria as listas de nos adjacentes
}
 
void Grafo::insere_aresta(int v1, int v2)
{
	adj[v1].push_back(v2);//adiciona v2 na lista de nos adjacentes de v1
}
 
int Grafo::dfs(int v)
{
	stack<int> pilha;
	bool visitados[V];
	int cont=0;
 
	// todos os vertices sao marcados como nao visitados
	for(int i = 0; i < V; i++)
		visitados[i] = false;
 
	while(true)
	{
		if(!visitados[v])
		{
			cont++;
			visitados[v] = true;
			pilha.push(v); // insere o vertice na pilha
		}
 
		bool tem_vizinho = false;
		list<int>::iterator it;
 
		// busca por um no adjacente que ainda nao foi visitado
		for(it = adj[v].begin(); it != adj[v].end(); it++)
		{
			if(!visitados[*it])
			{
				tem_vizinho = true;
				break;
			}
		}
 
		if(tem_vizinho)
			v = *it; // atualiza o "v"
		else
		{
			pilha.pop();//quando todos os nos adjacentes já foram visitados ou se não tem nos adjacentes

			if(pilha.empty())
				break;// terminou a busca

			v = pilha.top();//pega o topo da pilha
		}
	}
	return cont;//retorna o numero de vertices adjacentes que foi possivel visitar
}

void Grafo::le_grafo_gnd(char* c){//le o arquivo e adiciona os nos
	FILE* arquivo;
	int n_nos, vertice1, vertice2;
	arquivo = fopen(c,"r+");
	fscanf(arquivo,"%d", &n_nos);
	while(!feof(arquivo)){
		fscanf(arquivo,"%d %d", &vertice1, &vertice2);
		this->insere_aresta(vertice1, vertice2);
		this->insere_aresta(vertice2, vertice1);
	}
}

void Grafo::le_grafo_gd(char* c){//le o arquivo e adiciona os nos
	FILE* arquivo;
	int n_nos, vertice1, vertice2;
	arquivo = fopen(c,"r+");
	fscanf(arquivo,"%d", &n_nos);
	while(!feof(arquivo)){
		fscanf(arquivo,"%d %d", &vertice1, &vertice2);
		this->insere_aresta(vertice1, vertice2);
	}
}

void Grafo::conexo(){//verifica a conexao
	int cont=0;
	for(int i=0; i<this->V; i++){
		if(this->dfs(i)==this->V)
			cont++;
	}
	if(cont==this->V)//se cont=numero de nos, todos os nos chegam em todos os nos
		cout<<"\n\nO grafo eh conexo\n";
	else if(cont==0)
		cout<<"\n\nO grafo nao eh conexo e nao tem nenhum componente fortemente conexo";
	else
		cout<<"\n\nO grafo nao eh conexo, mas tem "<<cont<<" componente(s) fortemente conexo\n";
}
 
int main()
{
	
	while(true){

		cout<<"\n\n-----------VERIFICADOR GRAFOS CONEXOS-----------\n";
		cout<<"\nDigite o exemplo que desa abrir:\n";
		cout<<"[0] GND 1\t[1] GND2\t[2] GD1\t[3] GND2\t[4] Sair\n";

		char *c;
		int x;
		bool aux = true;

		cin>>x;

		if(x==0)
			c = (char*)"in_gnd1.txt";
		else if(x==1)
			c = (char*)"in_gnd2.txt";
		else if(x==2)
			c = (char*)"in_gd1.txt";
		else if(x==3)
			c = (char*)"in_gnd2.txt";
		else if(x==4)
			exit(0);
		else{
			cout<<"\nValor invalido. Tente novamente";
			aux = false;
		}
		
		if(aux){
			FILE* arquivo;
			int n_nos;
			arquivo = fopen(c,"r+");
			fscanf(arquivo,"%d", &n_nos);
	 
			Grafo grafo(n_nos);
	 	
	 		if(x==0 || x==1)
				grafo.le_grafo_gnd(c);
			else
				grafo.le_grafo_gd(c);
		
			grafo.conexo();
		}

	}

	return 0;
}