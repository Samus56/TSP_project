#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "/mnt/c/Users/Utente/prova.dir/Ingegneria/progetto/unity/unity.h"
#include "/mnt/c/Users/Utente/prova.dir/Ingegneria/progetto/src/function.h"

// Funzione di configurazione eseguita prima di ogni test
void setUp(void) {
    srand(time(NULL)); // Inizializza il generatore di numeri casuali
}

// Funzione di pulizia eseguita dopo ogni test
void tearDown(void) {}

// Funzione di supporto per generare una matrice di distanze casuali
int **generateRandomDistanceMatrix(int num_nodes) {
    int **matrix = (int **)malloc(num_nodes * sizeof(int *));
    for (int i = 0; i < num_nodes; i++) {
        matrix[i] = (int *)malloc(num_nodes * sizeof(int));
        for (int j = 0; j < num_nodes; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            } else {
                matrix[i][j] = rand() % 100 + 1;  // Distanze casuali tra 1 e 100
            }
        }
    }
    return matrix;
}

// Funzione di supporto per liberare la matrice
void freeMatrix(int **matrix, int num_nodes) {
    for (int i = 0; i < num_nodes; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Funzione per impostare tutte le distanze a un valore fisso
void setAllDistancesToOne(int **matrix, int num_nodes) {
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            matrix[i][j] = (i == j) ? 0 : 1;  // Distanza 1 tra città diverse
        }
    }
}

// Test per la funzione calculateCost
void test_calculateCost(void) {
    int num_nodes = 4;
    int **graph = generateRandomDistanceMatrix(num_nodes);
    int tour[] = {0, 1, 2, 3, 0};  // Esempio di tour

    int cost = calculateCost(graph, tour, num_nodes);
    TEST_ASSERT_GREATER_OR_EQUAL(0, cost);  // Costo non negativo

    // Test con tutte le distanze uguali a 1
    setAllDistancesToOne(graph, num_nodes);
    cost = calculateCost(graph, tour, num_nodes);
    TEST_ASSERT_EQUAL(num_nodes, cost);  // Costo deve essere uguale al numero di nodi

    freeMatrix(graph, num_nodes);
}

// Test per la funzione tspNearestNeighbor
void test_tspNearestNeighbor(void) {
    int num_nodes = 5;
    Graph graph = createGraph(num_nodes);
    int **distanceMatrix = generateRandomDistanceMatrix(num_nodes);

    Solution solution = tspNearestNeighbor(&graph, distanceMatrix);

    // Verifica che tutte le città siano state visitate esattamente una volta
    int *visited = (int *)calloc(num_nodes, sizeof(int));
    for (int i = 0; i < num_nodes; i++) {
        visited[solution.solution[i]]++;
    }
    for (int i = 0; i < num_nodes; i++) {
        TEST_ASSERT_EQUAL(1, visited[i]);  // Ogni città deve essere visitata una volta
    }

    free(visited);
    freeSolution(&solution);
    freeGraph(&graph);
    freeMatrix(distanceMatrix, num_nodes);
}

// Test per la funzione simulatedAnnealing
void test_simulatedAnnealing(void) {
    int num_nodes = 5;
    Graph graph = createGraph(num_nodes);
    int **costMatrix = generateRandomDistanceMatrix(num_nodes);

    Solution initialSolution = tspNearestNeighbor(&graph, costMatrix);
    Solution finalSolution = simulatedAnnealing(&initialSolution, &graph, costMatrix);

    // Verifica che il costo della soluzione finale non sia superiore a quella iniziale
    TEST_ASSERT_LESS_OR_EQUAL(initialSolution.cost_solution, finalSolution.cost_solution);

    // Verifica che tutte le città siano visitate una volta nel tour finale
    int *visited = (int *)calloc(num_nodes, sizeof(int));
    for (int i = 0; i < num_nodes; i++) {
        visited[finalSolution.solution[i]]++;
    }
    for (int i = 0; i < num_nodes; i++) {
        TEST_ASSERT_EQUAL(1, visited[i]);  // Ogni città deve essere visitata una sola volta
    }

    free(visited);
    freeSolution(&initialSolution);
    freeSolution(&finalSolution);
    freeGraph(&graph);
    freeMatrix(costMatrix, num_nodes);
}

// Test per la funzione swapCities
void test_swapCities(void) {
    int num_nodes = 5;
    int tour[] = {0, 1, 2, 3, 4};

    int *originalTour = (int *)malloc(num_nodes * sizeof(int));
    memcpy(originalTour, tour, num_nodes * sizeof(int));

    // Scambia due città casuali
    swapCities(tour, 1, 3);

    // Verifica che i nodi siano gli stessi, solo in un ordine diverso
    int *visited = (int *)calloc(num_nodes, sizeof(int));
    for (int i = 0; i < num_nodes; i++) {
        visited[originalTour[i]]++;
        visited[tour[i]]--;
    }
    for (int i = 0; i < num_nodes; i++) {
        TEST_ASSERT_EQUAL(0, visited[i]);  // Tutti i nodi devono essere invariati
    }

    free(visited);
    free(originalTour);
}

// Funzione principale che esegue i test
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_calculateCost);
    RUN_TEST(test_tspNearestNeighbor);
    RUN_TEST(test_simulatedAnnealing);
    RUN_TEST(test_swapCities);
    
    return UNITY_END();
}
