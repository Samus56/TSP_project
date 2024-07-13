#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_NODES 100
#define INT_MAX 999
#define N 15
#define TABU_TENURE 3
#define MAX_TEMPERATURE 1000.0 // Temperatura iniziale
#define MIN_TEMPERATURE 0.1 // Temperatura minima
#define COOLING_RATE 0.995 // Tasso di raffreddamento

typedef struct {
    int num_nodes;
    int nodes[MAX_NODES];
} Graph;

typedef struct {
    int solution[MAX_NODES];
    int cost_solution;
} Solution;

typedef struct {
    int move_a;
    int move_b;
    int tabuTenure;
} Tabu_element;

int calculateCost(int graph[N][N], int tour[N]) {
    int cost = 0;
    for (int i = 0; i < N; i++) {
        cost += graph[tour[i]][tour[i + 1]];
    }
    return cost;
}

Graph createGraph(int num_nodes) {
    Graph graph;
    graph.num_nodes = num_nodes;
    for (int i = 0; i < num_nodes; i++) {
        graph.nodes[i]= i;
    }
    return graph;
}
int findNearestNeighbor(Graph *graph, int currentCity, int visited[N], int distanceMatrix[N][N]) {
    int nearestCity = -1;
    int minDistance = INT_MAX;
    for (int city = 0; city < N; city++) {
        if (!visited[city] && city != currentCity ) 
        {
            if(distanceMatrix[currentCity][city] < minDistance)
            {
                minDistance = distanceMatrix[currentCity][city];
                nearestCity = city;
            }
        }
    }

    return nearestCity;
}

// Funzione per risolvere il TSP con l'algoritmo Nearest Neighbor
Solution tspNearestNeighbor(Graph *graph, int distanceMatrix[N][N]) {
    int visited[N];
    Solution result;
    for (int i = 0; i < graph->num_nodes; i++) {
        visited[i] = 0;  // Inizializza tutti i nodi come non visitati
    }
    // Parti dalla prima città (città 0)
    result.cost_solution = 0;
    int currentCity = 0;
    visited[currentCity] = 1;
    result.solution[0] = currentCity;

    // Costruisci il solution del commesso viaggiatore
    for (int i = 1; i < graph->num_nodes; i++) {
        int nearestCity = findNearestNeighbor(graph,currentCity, visited, distanceMatrix);
        result.solution[i] = nearestCity;
        visited[nearestCity] = 1;
        currentCity = nearestCity;
    }

    // Torna alla prima città per completare il ciclo
    result.solution[graph->num_nodes] = result.solution[0];
    result.cost_solution=calculateCost(distanceMatrix,result.solution);
    return result;
}
// Funzione per scambiare due città in un tour
void swapCities(int tour[N], int i, int j) {
    int temp = tour[i];
    tour[i] = tour[j];
    tour[j] = temp;
}
// Simulated Annealing per risolvere il TSP
Solution simulatedAnnealing(Solution *first_Solution,Graph *graph,int costMatrix[N][N]) {
    Solution bestSolution;
    Solution currentSolution;
    bestSolution=*first_Solution;
    currentSolution=*first_Solution;
    int controll_costraints=0;
    double temperature = MAX_TEMPERATURE;
    while (temperature > MIN_TEMPERATURE) {
        Solution newSolution=currentSolution;
        int index1 = rand() % N;
        int index2 = rand() % N;
        while (index1 == index2) {
            index2 = rand() % N;
        }
        
        do {
            index1= rand() % N;
        } while (index1 == 0); 

        do {
            index2 = rand() % N;
        } while (index2 == 0);
        swapCities(newSolution.solution,index1,index2);
        newSolution.cost_solution=calculateCost(costMatrix,newSolution.solution);
        double delta = newSolution.cost_solution - currentSolution.cost_solution;
        if (delta < 0 || exp(-delta / temperature) > ((double)rand() / RAND_MAX)) {
            currentSolution = newSolution;
            if (currentSolution.cost_solution < bestSolution.cost_solution) {
                bestSolution = currentSolution;
            }
        }
        else{
            swapCities(newSolution.solution,index2,index1);
        }
        temperature *= COOLING_RATE;
    }
    bestSolution.solution[N]=0; //ritorno alla base;
    bestSolution.cost_solution=calculateCost(costMatrix,bestSolution.solution);
    return bestSolution;
}
int main() {
    Graph graph = createGraph(N);
    Solution soluzione;
    Solution soluzione_ottima;
    int costMatrix[N][N]={
        {0,642,900,834,336,380,923,599,359,685,339,607,366,814,183},
        {642,0,315,567,442,382,456,173,426,318,367,516,301,335,462},
        {900,315,0,441,618,555,214,302,595,287,573,534,534,163,735},
        {834,567,441,0,499,465,256,408,477,251,520,239,541,285,735},
        {336,442,618,499,0,64,600,325,24,365,85,279,160,503,503},
        {380,382,555,465,64,0,546,262,44,309,60,263,120,445,272},
        {923,456,214,256,600,546,0,356,576,237,584,423,571,125,782},
        {599,173,302,408,325,262,356,0,304,159,272,344,233,233,439},
        {359,426,595,477,24,44,576,304,0,340,82,261,153,479,272},
        {685,318,287,251,365,309,237,159,340,0,347,250,339,142,549},
        {339,367,573,520,85,60,584,272,82,347,0,323,75,476,215},
        {607,516,534,239,279,263,423,344,261,250,323,0,368,376,532},
        {366,301,534,541,160,120,571,233,153,339,75,368,0,455,211},

        {814,335,163,285,503,445,125,233,479,142,476,376,455,0,666},
        
        {183,462,735,735,257,272,782,439,272,549,215,532,211,666,0},
    };
    soluzione=tspNearestNeighbor(&graph,costMatrix);
   
    printf("Solution Nearest Neighbour con pickup e delivery: \n");
    for(int i=0; i< graph.num_nodes+1; i++)
     printf("%d ",soluzione.solution[i]);
    printf("Il costo della soluzione e' %d \n", soluzione.cost_solution);
    //tspTabuSearch(&graph,costMatrix,&soluzione);
    soluzione_ottima=simulatedAnnealing(&soluzione,&graph,costMatrix);
    printf("Solution Simulated Annealing con pickup e delivery: \n");
    for(int i=0; i< graph.num_nodes+1; i++)
     printf("%d ",soluzione_ottima.solution[i]);
    printf("Il costo della soluzione e' %d \n", soluzione_ottima.cost_solution);

    return 0;
}