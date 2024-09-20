#include "/mnt/c/Users/Utente/prova.dir/Ingegneria/progetto/unity/unity.h"
#include "/mnt/c/Users/Utente/prova.dir/Ingegneria/progetto/src/function.h"
#include <stdlib.h>

# define N 3

int **allocateMatrix(int num_nodes) {
    int **matrix = (int **)malloc(num_nodes * sizeof(int *));
    if (matrix == NULL) {
        perror("Failed to allocate memory for matrix rows");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_nodes; i++) {
        matrix[i] = (int *)malloc(num_nodes * sizeof(int));
        if (matrix[i] == NULL) {
            perror("Failed to allocate memory for matrix columns");
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

void freeMatrix(int **matrix, int num_nodes) {
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

void test_calculateCost(void) {
    int num_nodes=N;
    int **graph = allocateMatrix(N);
    int initial_graph[N][N] = {{0, 10, 15}, {10, 0, 35}, {15, 35, 0}};
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            graph[i][j] = initial_graph[i][j];
        }
    }
    int tour[N + 1] = {0, 1, 2, 0};
    int expectedCost = 10 + 35 + 15;
    
    TEST_ASSERT_EQUAL_INT(expectedCost, calculateCost((int **)graph, tour, N));

    freeMatrix(graph, num_nodes);
}

void test_createGraph(void) {
    int num_nodes = N;
    Graph graph = createGraph(num_nodes);

    TEST_ASSERT_EQUAL_INT(num_nodes, graph.num_nodes);
    for (int i = 0; i < num_nodes; i++) {
        TEST_ASSERT_EQUAL_INT(i, graph.nodes[i]);
    }
    freeGraph(&graph);
}

void test_findNearestNeighbor(void) {
    Graph graph = createGraph(N);
    int visited[N] = {1, 0, 0};  // Supponiamo di aver visitato la prima città
    int **distanceMatrix = allocateMatrix(N);
    
    // Inizializzazione della matrice di test
    int initial_distanceMatrix[N][N] = {{0, 10, 15}, {10, 0, 35}, {15, 35, 0}};
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            distanceMatrix[i][j] = initial_distanceMatrix[i][j];
        }
    }

    int nearestCity = findNearestNeighbor(&graph, 0, visited, (int **)distanceMatrix);
    TEST_ASSERT_EQUAL_INT(1, nearestCity);
    freeMatrix(distanceMatrix, N);
}

void test_tspNearestNeighbor(void) {
    Graph graph = createGraph(N);
    int **distanceMatrix = allocateMatrix(N);

    // Inizializzazione della matrice di test
    int initial_distanceMatrix[N][N] = {{0, 10, 15}, {10, 0, 35}, {15, 35, 0}};
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            distanceMatrix[i][j] = initial_distanceMatrix[i][j];
        }
    }
    Solution solution = tspNearestNeighbor(&graph,(int **) distanceMatrix);
    
    // Il costo dovrebbe essere 45 (0 -> 1 -> 2 -> 0)
    TEST_ASSERT_EQUAL_INT(60, solution.cost_solution);
    // Verifica che il percorso sia corretto
    TEST_ASSERT_EQUAL_INT(0, solution.solution[0]);
    TEST_ASSERT_EQUAL_INT(1, solution.solution[1]);
    TEST_ASSERT_EQUAL_INT(2, solution.solution[2]);

    free(solution.solution);
    freeMatrix(distanceMatrix, N);
}
void test_simulatedAnnealing(void) {
    int num_nodes = N;
    int **distanceMatrix = allocateMatrix(num_nodes);

    // Inizializzazione della matrice di distanza
    int initial_distanceMatrix[N][N] = {{0, 10, 15}, {10, 0, 35}, {15, 35, 0}};
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            distanceMatrix[i][j] = initial_distanceMatrix[i][j];
        }
    }

    // Creazione del grafo e soluzione iniziale
    Graph graph = createGraph(num_nodes);
    Solution initialSolution;
    initialSolution.solution = (int *)malloc((num_nodes + 1) * sizeof(int));
    if (initialSolution.solution == NULL) {
        perror("Failed to allocate memory for initial solution");
        exit(EXIT_FAILURE);
    }
    Solution bestSolution;
    bestSolution.solution = (int *)malloc((num_nodes + 1) * sizeof(int));
    if (bestSolution.solution == NULL) {
        perror("Failed to allocate memory for best solution");
        exit(EXIT_FAILURE);
    }
    
    initialSolution = tspNearestNeighbor(&graph, distanceMatrix);
    bestSolution = simulatedAnnealing(&initialSolution, &graph, distanceMatrix);

    // Verifica che la soluzione sia valida e non sia nulla
    TEST_ASSERT_NOT_NULL(bestSolution.solution);
    free(initialSolution.solution);
    free(bestSolution.solution);
    freeMatrix(distanceMatrix, num_nodes);
    freeGraph(&graph);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_calculateCost);
    RUN_TEST(test_createGraph);
    RUN_TEST(test_findNearestNeighbor);
    RUN_TEST(test_tspNearestNeighbor);
    RUN_TEST(test_simulatedAnnealing);
    
    return UNITY_END();
}