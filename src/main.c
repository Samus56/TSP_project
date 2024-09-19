#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#define N 15
int main() {
    // Inizializzazione del grafo e della matrice dei costi
    int num_nodes = N;
    int **costMatrix = (int **)malloc(num_nodes * sizeof(int *));
    for (int i = 0; i < num_nodes; i++) {
        costMatrix[i] = (int *)malloc(num_nodes * sizeof(int));
    }
    
    // Inizializzazione della matrice dei costi con valori dati
    int static_costMatrix[N][N] = {
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
        {183,462,735,735,257,272,782,439,272,549,215,532,211,666,0}
    };

    // Copia i dati dalla matrice statica a costMatrix
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            costMatrix[i][j] = static_costMatrix[i][j];
        }
    }
    
    // Creazione del grafo
    Graph graph = createGraph(num_nodes);
    
    // Trova la soluzione con Nearest Neighbor
    Solution soluzione = tspNearestNeighbor(&graph, costMatrix);
    printf("Solution Nearest Neighbor: \n");
    for (int i = 0; i < graph.num_nodes + 1; i++) {
        printf("%d ", soluzione.solution[i]);
    }
    printf("\nIl costo della soluzione e' %d \n", soluzione.cost_solution);
    
    // Trova la soluzione ottimale con Simulated Annealing
    Solution soluzione_ottima = simulatedAnnealing(&soluzione, &graph, costMatrix);
    printf("Solution Simulated Annealing: \n");
    for (int i = 0; i < graph.num_nodes + 1; i++) {
        printf("%d ", soluzione_ottima.solution[i]);
    }
    printf("\nIl costo della soluzione e' %d \n", soluzione_ottima.cost_solution);
    
    // Deallocazione della memoria
    freeSolution(&soluzione);
    freeSolution(&soluzione_ottima);
    freeGraph(&graph);
    for (int i = 0; i < num_nodes; i++) {
        free(costMatrix[i]);
    }
    free(costMatrix);
    
    return 0;
}