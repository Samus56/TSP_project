#include <string.h>
#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define INT_MAX 999
#define TABU_TENURE 3
#define MAX_TEMPERATURE 1000.0 // Temperatura iniziale
#define MIN_TEMPERATURE 0.1 // Temperatura minima
#define COOLING_RATE 0.995 // Tasso di raffreddamento


// Funzione per calcolare il costo di un tour
int calculateCost(int **graph, int *tour, int num_nodes) {
    int cost = 0;
    for (int i = 0; i < num_nodes; i++) {
        cost += graph[tour[i]][tour[i + 1]];
    }
    return cost;
}

// Funzione per creare un grafo
Graph createGraph(int num_nodes) {
    Graph graph;
    graph.num_nodes = num_nodes;
    graph.nodes = (int *)malloc(num_nodes * sizeof(int));
    for (int i = 0; i < num_nodes; i++) {
        graph.nodes[i] = i;
    }
    return graph;
}

// Funzione per trovare il vicino più vicino
int findNearestNeighbor(Graph *graph, int currentCity, int *visited, int **distanceMatrix) {
    int nearestCity = -1;
    int minDistance = INT_MAX;
    for (int city = 0; city < graph->num_nodes; city++) {
        if (!visited[city] && city != currentCity) {
            if (distanceMatrix[currentCity][city] < minDistance) {
                minDistance = distanceMatrix[currentCity][city];
                nearestCity = city;
            }
        }
    }
    return nearestCity;
}

// Funzione per risolvere il TSP con l'algoritmo Nearest Neighbor
Solution tspNearestNeighbor(Graph *graph, int **distanceMatrix) {
    Solution result;
    result.solution = (int *)malloc((graph->num_nodes + 1) * sizeof(int));
    int *visited = (int *)malloc(graph->num_nodes * sizeof(int));
    
    for (int i = 0; i < graph->num_nodes; i++) {
        visited[i] = 0;  // Inizializza tutti i nodi come non visitati
    }
    
    // Parti dalla prima città (città 0)
    result.cost_solution = 0;
    int currentCity = 0;
    visited[currentCity] = 1;
    result.solution[0] = currentCity;

    // Costruisci la soluzione del commesso viaggiatore
    for (int i = 1; i < graph->num_nodes; i++) {
        int nearestCity = findNearestNeighbor(graph, currentCity, visited, distanceMatrix);
        result.solution[i] = nearestCity;
        visited[nearestCity] = 1;
        currentCity = nearestCity;
    }

    // Torna alla prima città per completare il ciclo
    result.solution[graph->num_nodes] = result.solution[0];
    result.cost_solution = calculateCost(distanceMatrix, result.solution, graph->num_nodes);
    
    free(visited);
    return result;
}

// Funzione per scambiare due città in un tour
void swapCities(int *tour, int i, int j) {
    int temp = tour[i];
    tour[i] = tour[j];
    tour[j] = temp;
}

// Simulated Annealing per risolvere il TSP
Solution simulatedAnnealing(Solution *first_Solution, Graph *graph, int **costMatrix) {
    Solution bestSolution;
    Solution currentSolution;
    
    // Inizializzazione
    bestSolution.solution = (int *)malloc((graph->num_nodes + 1) * sizeof(int));
    currentSolution.solution = (int *)malloc((graph->num_nodes + 1) * sizeof(int));
    memcpy(bestSolution.solution, first_Solution->solution, (graph->num_nodes + 1) * sizeof(int));
    memcpy(currentSolution.solution, first_Solution->solution, (graph->num_nodes + 1) * sizeof(int));
    
    bestSolution.cost_solution = first_Solution->cost_solution;
    currentSolution.cost_solution = first_Solution->cost_solution;

    double temperature = MAX_TEMPERATURE;
    
    while (temperature > MIN_TEMPERATURE) {
        Solution newSolution;
        newSolution.solution = (int *)malloc((graph->num_nodes + 1) * sizeof(int));
        memcpy(newSolution.solution, currentSolution.solution, (graph->num_nodes + 1) * sizeof(int));

        int index1 = rand() % graph->num_nodes;
        int index2 = rand() % graph->num_nodes;
        while (index1 == index2) {
             index2 = rand() % graph->num_nodes;
         }
        
        do {
             index1= rand() % graph->num_nodes;
        } while (index1 == 0); 

         do {
             index2 = rand() % graph->num_nodes;
       } while (index2 == 0); 
        
        swapCities(newSolution.solution, index1, index2);
        newSolution.cost_solution = calculateCost(costMatrix, newSolution.solution, graph->num_nodes);

        double delta = newSolution.cost_solution - currentSolution.cost_solution;
        if (delta < 0 || exp(-delta / temperature) > ((double)rand() / RAND_MAX)) {
            free(currentSolution.solution);
            currentSolution = newSolution;
            if (currentSolution.cost_solution < bestSolution.cost_solution) {
                free(bestSolution.solution);
                bestSolution.solution = (int *)malloc((graph->num_nodes + 1) * sizeof(int));
                memcpy(bestSolution.solution, currentSolution.solution, (graph->num_nodes + 1) * sizeof(int));
                bestSolution.cost_solution = currentSolution.cost_solution;
            }
        } else {
            free(newSolution.solution);
        }
        
        temperature *= COOLING_RATE;
    }
    
    // Assicurati che la soluzione migliore ritorni alla città di partenza
    bestSolution.solution[graph->num_nodes] = bestSolution.solution[0];
    bestSolution.cost_solution = calculateCost(costMatrix, bestSolution.solution, graph->num_nodes);

    return bestSolution;
}

// Deallocazione della memoria
void freeGraph(Graph *graph) {
    free(graph->nodes);
}

void freeSolution(Solution *solution) {
    free(solution->solution);
}

