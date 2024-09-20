#include "/mnt/c/Users/Utente/prova.dir/Ingegneria/progetto/unity/unity.h"
#include "/mnt/c/Users/Utente/prova.dir/Ingegneria/progetto/src/function.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Funzione di supporto per creare una matrice di distanze di test
int **createTestDistanceMatrix(int num_nodes) {
    int **matrix = (int **)malloc(num_nodes * sizeof(int *));
    for (int i = 0; i < num_nodes; i++) {
        matrix[i] = (int *)malloc(num_nodes * sizeof(int));
        for (int j = 0; j < num_nodes; j++) {
            matrix[i][j] = (i == j) ? 0 : rand() % 100 + 1;
        }
    }
    return matrix;
}

void freeTestDistanceMatrix(int **matrix, int num_nodes) {
    for (int i = 0; i < num_nodes; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
void setUp(void) {
    // Codice opzionale da eseguire prima di ogni test
}

void tearDown(void) {
    // Codice opzionale da eseguire dopo ogni test
}

// Test di integrazione completo
void test_tsp_integration_full(void) {
    int num_nodes = 5;
    int **distanceMatrix = createTestDistanceMatrix(num_nodes);
    Graph graph = createGraph(num_nodes);

    // Verifica che il grafo sia stato creato correttamente
    TEST_ASSERT_NOT_NULL(graph.nodes);
    TEST_ASSERT_EQUAL(num_nodes, graph.num_nodes);

    // Ottieni una soluzione iniziale con Nearest Neighbor
    Solution initialSolution = tspNearestNeighbor(&graph, distanceMatrix);
    // Verifica che la soluzione iniziale sia valida
    TEST_ASSERT_NOT_NULL(initialSolution.solution);
    TEST_ASSERT_EQUAL(initialSolution.solution[0], initialSolution.solution[num_nodes]);  // Ultimo nodo deve tornare al nodo iniziale
    TEST_ASSERT(initialSolution.cost_solution >= 0);

    // Crea una copia della soluzione iniziale per il confronto
    Solution initialSolutionCopy;
    initialSolutionCopy.solution = (int *)malloc((num_nodes + 1) * sizeof(int));
    memcpy(initialSolutionCopy.solution, initialSolution.solution, (num_nodes + 1) * sizeof(int));
    initialSolutionCopy.cost_solution = initialSolution.cost_solution;

    // Ottimizza la soluzione con Simulated Annealing
    Solution bestSolution = simulatedAnnealing(&initialSolution, &graph, distanceMatrix);

    // Verifica che la soluzione ottimizzata sia valida
    TEST_ASSERT_NOT_NULL(bestSolution.solution);
    TEST_ASSERT_EQUAL(0, bestSolution.solution[num_nodes]);  // Ultimo nodo deve tornare al nodo iniziale
    TEST_ASSERT(bestSolution.cost_solution >= 0);

    // Verifica che il costo della soluzione ottimizzata sia minore o uguale a quello della soluzione iniziale
    TEST_ASSERT(bestSolution.cost_solution <= initialSolutionCopy.cost_solution);

    // Verifica l'integrità dei dati durante l'ottimizzazione
    TEST_ASSERT_TRUE(bestSolution.cost_solution >= 0);
    TEST_ASSERT_TRUE(initialSolution.cost_solution >= 0);

    // Libera la memoria
    freeSolution(&initialSolution);
    freeSolution(&bestSolution);
    free(initialSolutionCopy.solution);
    freeTestDistanceMatrix(distanceMatrix, num_nodes);
    freeGraph(&graph);
}

// Main function per eseguire i test
int main(void) {
    UNITY_BEGIN();
    
    srand((unsigned int)time(NULL));  // Inizializza il seed per i numeri casuali
    
    // Esegui il test di integrazione completo
    RUN_TEST(test_tsp_integration_full);
    
    return UNITY_END();
}