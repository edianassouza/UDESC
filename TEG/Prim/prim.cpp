 #include<bits/stdc++.h> 
using namespace std; 
# define INFINITO 0x3f3f3f3f 
  
 
typedef pair<int, int> Par; 
  

class Grafo 
{ 
    int V; 
    list< pair<int, int> > *adj; 
  
public: 
    Grafo(int V);

    void adicionarAresta(int u, int v, int w);  
    void prim(); 
}; 
   
Grafo::Grafo(int V) 
{ 
    this->V = V; 
    adj = new list<Par> [V]; 
} 
  
void Grafo::adicionarAresta(int u, int v, int w) 
{ 
    //adiociona u na lista de adjacencia de v e v na lista de adjacencia de u
    adj[u].push_back(make_pair(v, w)); 
    adj[v].push_back(make_pair(u, w)); 
} 
   
void Grafo::prim() 
{ 
    //fila de prioridade min heap
    priority_queue< Par, vector <Par> , greater<Par> > pq; 
  
    int primeiro = 0; 
   
    vector<int> key(V, INFINITO); //todos os vertices iniciados como infinito
    vector<int> pai(V, -1);  
    vector<bool> naArvore(V, false); //todos os vertices iniciados como false 
   
    pq.push(make_pair(0, primeiro)); 
    key[primeiro] = 0; 
  
    while (!pq.empty()) 
    { 

        int u = pq.top().second; 
        pq.pop(); 
  
        naArvore[u] = true;
   
        list< pair<int, int> >::iterator i; 
        for (i = adj[u].begin(); i != adj[u].end(); ++i) 
        { 
            
            int v = (*i).first; 
            int peso = (*i).second; 
  
         
            if (naArvore[v] == false && key[v] > peso) 
            {  
                key[v] = peso; 
                pq.push(make_pair(key[v], v)); 
                pai[v] = u; 
            } 
        } 
    } 

        printf("Arestas da Arvore Geradora Minima\n");
        for (int i = 1; i < V; ++i) 
        printf("%d - %d\n", pai[i], i); 
} 
  
 
int main() 
{
    
    int tam, raiz; 

    FILE *arquivo;
    arquivo = fopen("entrada.txt", "r+");
    fscanf(arquivo, "%d\n", &tam);
   
    Grafo grafo(tam);

    int origem, destino, peso;
    
    while(!feof(arquivo)){
        fscanf(arquivo, "%d %d %d\n", &origem, &destino, &peso);
        grafo.adicionarAresta(origem, destino, peso);
    } 
  
    grafo.prim(); 
  
    return 0; 
} 