#ifndef MAIN_H
#define MAIN_H

#define MAX_NODES 100
#define TABU_TENURE 3
//#define N 3
#define MAX_TEMPERATURE 1000.0  // Temperatura iniziale
#define MIN_TEMPERATURE 0.1     // Temperatura minima
#define COOLING_RATE 0.995      // Tasso di raffreddamento

// Struttura per il grafo
typedef struct {
    int num_nodes;
    int *nodes;  // Usa puntatori anziché array statici
} Graph;

typedef struct {
    int *solution;  // Usa puntatori anziché array statici
    int cost_solution;
} Solution;

int calculateCost(int **graph, int *tour, int num_nodes);
Graph createGraph(int num_nodes);
int findNearestNeighbor(Graph *graph, int currentCity, int *visited, int **distanceMatrix);
Solution tspNearestNeighbor(Graph *graph, int **distanceMatrix);
Solution simulatedAnnealing(Solution *first_Solution, Graph *graph, int **costMatrix);
void freeGraph(Graph *graph);
void freeSolution(Solution *solution);

#endif // MAIN_H