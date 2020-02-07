#include <bits/stdc++.h>
#include <unistd.h>
#define INFINITO 10000000
#define RESET "\033[0m"
using namespace std;

struct vertice{
	int index;
	float x;
	float y;
	vector<pair<vertice, float>> adj;
	/*lista de adjacência adj
	O primeiro valor é o vizinho e o segundo valor é o peso da aresta*/
	vertice(){}
};

struct compare  
 {  
   bool operator()(const pair<float, vertice>& x, const pair<float, vertice>& y)  
   {  
       return x.first > y.first;  
   }  
 };

class Grafo{

private:
	int V; // número de vértices
	vertice *vertices;//vetor de vértices que é alocado dinâmicamente


	string vetorCores[15] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m", "\033[37m", "\033[1m\033[30m","\033[1m\033[31m", "\033[1m\033[32m", "\033[1m\033[33m", "\033[1m\033[34m", "\033[1m\033[35m", "\033[1m\033[36m", "\033[1m\033[37m"};

public:
	//Construtor
	Grafo(int V)
	{
		this->V = V; // atribui o número de vértices
		vertices = (vertice*) malloc(sizeof(vertice)*V);
		vertice aux;
		vertices[0].adj.push_back(make_pair(aux, 2));
		
		for(int i=0; i<V; i++){

			vertices[i].index = i+1;
			vertices[i].x = ((float) rand() / (RAND_MAX));
			vertices[i].y = ((float) rand() / (RAND_MAX));
			
			for(int j=0; j<log(vertices[i].index); j++){
				vertices[i].adj.push_back(make_pair(aux, INFINITO));	
			}
		}

		for(int i=0; i<V; i++){
			if(log(vertices[i].index)>0)
				criarArestas(i);
		}
		cout << "\nGrafo GN" << V << " criado\n";
	}

	//Função que imprime os vértices do gráfico
	void imprimeVertices()
	{
		cout << "\033[34m" << "\nVertices\n" << RESET;
		for(int i=0; i<this->V; i++){
			cout << "Vertice " << vertices[i].index << " (" << vertices[i].x << "," << vertices[i].y << ");\n";
		}
	}

	void adicionarAresta(vertice origem, vertice destino, float custo)
	{
		vertices[origem.index-1].adj.push_back(make_pair(destino, custo));		
	}

	void criarArestas(int i){
		float distancia;
		for(int j=0; j<this->V; j++){
			if(i!=j){
				distancia = sqrt(pow(vertices[i].x-vertices[j].x, 2)+pow(vertices[i].y-vertices[j].y, 2));
				if(distancia<vertices[i].adj.back().second){
					vertices[i].adj.pop_back();
					adicionarAresta(vertices[i], vertices[j], distancia);
					sort(vertices[i].adj.begin(), vertices[i].adj.end(), ordem);
				}
			}
		}
	}

	int log(int i){
		int result;
		result = (int) log2(i);
		return result;
	}

	bool static ordem(const pair<vertice, float>& x, const pair<vertice, float>& y) { 
  		return x.second < y.second; 
	}

	void imprimeAdjacentes(){
		cout << "\033[34m" << "\nLista Adjacencia\n" << RESET;
		for(int i=0; i<this->V; i++){
			cout << "Vertice " << i+1 <<": ";
			vector <pair<vertice, float>>::iterator it;
			if(log(i+1)>0){
				for(it=vertices[i].adj.begin(); it!=vertices[i].adj.end(); it++){
					cout << "(" << it->first.index << "," << it->second << "); ";
				}
			}
			cout << "\n";
		}
	}

	void chamadfs(){
		cout << "\033[34m" << "\nDFS\n" << RESET;
		int indice = rand()%((this->V-1)-0 + 1) + 0;
		dfs(vertices[indice]);
	}

	void dfs(vertice raiz){

		stack<vertice> pilha;
		bool visitados[this->V]; // vetor de visitados
	 
		// marca todos como não visitados
		for(int i = 0; i < this->V; i++)
			visitados[i] = false;
	 
		while(true){

			if(!visitados[raiz.index-1])
			{
				sleep(1);
				cout << "Visitando vertice " << raiz.index << "\n";
				visitados[raiz.index-1] = true; // marca como visitado
				pilha.push(raiz); // insere a raiz na pilha
			}

			bool vizinhos = false;
	 		vector<pair<vertice, float>>::iterator it;
			// busca por um vizinho não visitado
			if(log(raiz.index)==0)
				vizinhos=false;
			else{
				for(it = vertices[raiz.index-1].adj.begin(); it != vertices[raiz.index-1].adj.end(); it++)
				{
					if(!visitados[it->first.index-1]) //para ler o valor que esta no indice do iterator, precisa-se do asterisco
					{
						vizinhos = true;
						break;
					}
				}
	 		}

			if(vizinhos)
				raiz = it->first; // atualiza a raiz
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
				sleep(1);
				cout << "Voltando para o vertice " << raiz.index << "\n";
			}
		}
	}

	void chamabfs(){
		cout << "\033[34m" << "\nBFS\n" << RESET;
		int indice = rand()%((this->V-1)-0 + 1) + 0;
		bfs(vertices[indice]);
	}

	void bfs(vertice raiz) {
	 	int nivel = 0;
	    bool visitados[this->V]; 
	    
	    for(int i=0; i<this->V; i++) 
	        visitados[i] = false; 
	   
	    list<pair<vertice, int>> fila; 
	   
	    visitados[raiz.index-1] = true; 
	    fila.push_back(make_pair(raiz, nivel)); 
	  
	 
	    vector<pair<vertice, float>>::iterator it; 
	  
	    while(!fila.empty()) 
	    {  
	    	nivel = fila.front().second;
	        raiz = fila.front().first;
	        if(nivel==15)
	        	nivel=0;
	        sleep(1);
	        cout << vetorCores[nivel] << "Visitando o vertice " << raiz.index << RESET <<"\n"; 
	        fila.pop_front();
	        if(log(raiz.index)==0 && !fila.empty()){
	        	raiz = fila.front().first; 
	        	sleep(1);
	        	cout << vetorCores[nivel] << "Visitando o vertice " << raiz.index << RESET <<"\n"; 
	        	fila.pop_front();
	        }
	  
	        if(!log(raiz.index)==0){
		        for (it = vertices[raiz.index-1].adj.begin(); it != vertices[raiz.index-1].adj.end(); ++it) 
		        { 
		            if (!visitados[it->first.index-1]) 
		            { 
		                visitados[it->first.index-1] = true; 
		                fila.push_back(make_pair(it->first, nivel+1)); 
		            } 
		        }
		    } 
	    } 	
	}

	void chamaDijkstra(){
		cout<< "\033[34m" << "\nDijkstra" << "\n\n" << RESET;
		int indice = rand()%((this->V-1)-0 + 1) + 0;
		dijkstra(vertices[indice]);
	}

	void dijkstra(vertice raiz)
	{
		cout<< "Saindo do vertice " << raiz.index << "\n\n";

		float distancia[this->V];

		int visitados[this->V];

		int antecessor[this->V];

		priority_queue < pair<float, vertice>,
					   vector<pair<float, vertice> >, compare > pq;

		for(int i = 0; i < this->V; i++)
		{
			distancia[i] = INFINITO;
			visitados[i] = false;
		}

		distancia[raiz.index-1] = 0;

		pq.push(make_pair(distancia[raiz.index-1], raiz));

		antecessor[raiz.index-1] = raiz.index;

		while(!pq.empty())
		{
			pair<float, vertice> p = pq.top();
			vertice u = p.second; 
			pq.pop();

			if(visitados[u.index-1] == false)
			{
				visitados[u.index-1] = true;

				vector<pair<vertice, float> >::iterator it;

				if(log(u.index)>0){
					for(it = vertices[u.index-1].adj.begin(); it != vertices[u.index-1].adj.end(); it++)
					{
						vertice v = it->first;
						float custo_aresta = it->second;

						//relaxamento(u, v)
						if(distancia[v.index-1] > (distancia[u.index-1] + custo_aresta))
						{
							distancia[v.index-1] = distancia[u.index-1] + custo_aresta;
							antecessor[v.index-1] = u.index;
							pq.push(make_pair(distancia[v.index-1], v));
						}
					}
				}
			}
		}

	for(int dest=0; dest<this->V; dest++){
		printf("Vertice %d\n", dest+1);
		if (distancia[dest] == INFINITO){
			printf("Nao ha caminho\n");
			printf("\n");
		}
		else{
			int v = dest;
			std::string caminho;
			printf("Caminho: ");
			string numero = std::to_string(dest+1);
			reverse(numero.begin(), numero.end());
			caminho.append(numero);
			while (antecessor[v] != v+1){
				caminho.append(" ");
				numero = std::to_string(antecessor[v]);
				reverse(numero.begin(), numero.end()); 
				caminho.append(numero);
				v = antecessor[v]-1;
			}
		
			reverse(caminho.begin(), caminho.end()); 
			cout << caminho << endl; 

			// retorna a distância mínima até o destino
			printf("Distancia: %f\n", distancia[dest]);
			printf("\n");
		}
	}
	}

	void chamaDijkstraModificado(){
		cout<< "\033[34m" << "\nDijkstra Modificado" << "\n\n"<< RESET;
		int indice = rand()%((this->V-1)-0 + 1) + 0;
		cout<< "Saindo de todos os vertices para o vertice " << vertices[indice].index << "\n";
		for(int i=0; i<this->V; i++){
			dijkstraModificado(vertices[i], vertices[indice]);
			sleep(1);
		}
	}

	void dijkstraModificado(vertice raiz, vertice fixo)
	{
		cout << "\nSaindo do vertice " << raiz.index << "\n";
		float distancia[this->V];

		int visitados[this->V];

		int antecessor[this->V];

		priority_queue < pair<float, vertice>,
					   vector<pair<float, vertice> >, compare > pq;

		for(int i = 0; i < this->V; i++)
		{
			distancia[i] = INFINITO;
			visitados[i] = false;
		}

		distancia[raiz.index-1] = 0;

		pq.push(make_pair(distancia[raiz.index-1], raiz));

		antecessor[raiz.index-1] = raiz.index;

		while(!pq.empty())
		{
			pair<float, vertice> p = pq.top();
			vertice u = p.second; 
			pq.pop();

			if(visitados[u.index-1] == false)
			{
				visitados[u.index-1] = true;

				vector<pair<vertice, float> >::iterator it;

				if(log(u.index)>0){
					for(it = vertices[u.index-1].adj.begin(); it != vertices[u.index-1].adj.end(); it++)
					{
						vertice v = it->first;
						float custo_aresta = it->second;

						//relaxamento(u, v)
						if(distancia[v.index-1] > (distancia[u.index-1] + custo_aresta))
						{
							distancia[v.index-1] = distancia[u.index-1] + custo_aresta;
							antecessor[v.index-1] = u.index;
							pq.push(make_pair(distancia[v.index-1], v));
						}
					}
				}
			}
		}

		if (distancia[fixo.index-1] == INFINITO){
			printf("Nao ha caminho\n");
			printf("\n");
		}
		else{
			int v = fixo.index-1;
			std::string caminho;
			printf("Caminho: ");
			string numero = std::to_string(fixo.index);
			reverse(numero.begin(), numero.end());
			caminho.append(numero);
			while (antecessor[v] != v+1){
				caminho.append(" ");
				numero = std::to_string(antecessor[v]);
				reverse(numero.begin(), numero.end()); 
				caminho.append(numero);
				v = antecessor[v]-1;
			}
		
			reverse(caminho.begin(), caminho.end()); 
			cout << caminho << endl; 

			// retorna a distância mínima até o destino
			printf("Distancia: %f\n", distancia[fixo.index-1]);
			printf("\n");
		}
	
	}

};


int main(int argc, char *argv[])
{
	std::srand(std::time(0));

	Grafo g50(50);

	int op=1;
	while(op!=0){
		cout << "\033[34m" << "\n---------MENU---------\n";
		cout << "\033[34m" << "1 - " << RESET << "Imprime vertices\n";
		cout << "\033[34m" << "2 - " << RESET << "Imprime lista de adjacencia\n";
		cout << "\033[34m" << "3 - " << RESET << "DFS\n";
		cout << "\033[34m" << "4 - " << RESET << "BFS\n";
		cout << "\033[34m" << "5 - " << RESET << "Dijkstra (caminhos)\n";
		cout << "\033[34m" << "6 - " << RESET << "Dijkstra (de todos os vertices para um vertice fixo)\n";
		cout << "\033[34m" << "0 - " << RESET << "Sair\n";
		cout << "Digite a operacao desejada: ";
		cin >> op;

		switch(op){
			case 1:
				g50.imprimeVertices();
				break;
			case 2:
				g50.imprimeAdjacentes();
				break;
			case 3:
				g50.chamadfs();
				break;
			case 4:
				g50.chamabfs();
				break;
			case 5:
				g50.chamaDijkstra();
				break;
			case 6:
				g50.chamaDijkstraModificado();
				break;
		}
	}

	return 0;
}

