#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "stack.h"

void compare_algorithms(int num_edges, FILE* csv_file) {
    stack* hierholzer_circuit = stack_create(10);
    stack* fleury_circuit = stack_create(10);

    graph* g_hierholzer = create_eulerian_graph_by_edges(num_edges);
    clock_t start_hierholzer = clock();
    graph_hierholzer_algorithm(g_hierholzer, 0, hierholzer_circuit);
    clock_t end_hierholzer = clock();
    double time_hierholzer = ((double)(end_hierholzer - start_hierholzer)) / CLOCKS_PER_SEC;

    graph* g_fleury = create_eulerian_graph_by_edges(num_edges);
    clock_t start_fleury = clock();
    fleury_algorithm(g_fleury, 0, fleury_circuit);
    clock_t end_fleury = clock();
    double time_fleury = ((double)(end_fleury - start_fleury)) / CLOCKS_PER_SEC;

    fprintf(csv_file, "%d, %f, %f\n", num_edges, time_hierholzer, time_fleury);

    graph_free(g_hierholzer);
    graph_free(g_fleury);
    stack_free(hierholzer_circuit);
    stack_free(fleury_circuit);
}

int main() {
    FILE* csv_file = fopen("algorithm_comparison.csv", "w");
    fprintf(csv_file, "Edge Size, Hierholzer Time (s), Fleury Time (s)\n");

    int sizes[] = {
        10, 15, 20, 25, 30, 35, 40, 45, 50,
        55, 60, 65, 70, 75, 80, 85, 90, 95,
        100, 105, 110, 115, 120, 125, 130,
        135, 140, 145, 150
    };
    int count = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < count; i++) {
        compare_algorithms(sizes[i], csv_file);
    }
    fclose(csv_file);

    return 0;
}
