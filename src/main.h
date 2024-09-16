#ifndef MAIN_H
#define MAIN_H

#define MAX_NODES 100
#define N 15
#define TABU_TENURE 3
#define MAX_TEMPERATURE 1000.0  // Temperatura iniziale
#define MIN_TEMPERATURE 0.1     // Temperatura minima
#define COOLING_RATE 0.995      // Tasso di raffreddamento

// Struttura per il grafo
typedef struct {
    int num_nodes;
    int nodes[MAX_NODES];
} Graph;

// Struttura per la soluzione
typedef struct {
    int solution[MAX_NODES];
    int cost_solution;
} Solution;

// Struttura per l'elemento della tabu list
typedef struct {
    int move_a;
    int move_b;
    int tabuTenure;
} Tabu_element;

// Funzioni
int calculateCost(int graph[N][N], int tour[N]);
Graph createGraph(int num_nodes);
int findNearestNeighbor(Graph *graph, int currentCity, int visited[N], int distanceMatrix[N][N]);
Solution tspNearestNeighbor(Graph *graph, int distanceMatrix[N][N]);
Solution simulatedAnnealing(Solution *initialSolution, Graph *graph, int costMatrix[N][N]);

#endif // MAIN_H