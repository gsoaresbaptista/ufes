#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stdio.h>
#include "stack.h"

typedef struct node {
    int vertex;
    struct node* next;
} node;

typedef struct graph {
    int num_vertices;
    node** adj_lists;
    int* degrees;
} graph;

/**
 * @brief Creates a graph with a specified number of vertices.
 * 
 * @param vertices Number of vertices in the graph.
 * @return Pointer to the newly created graph.
 * 
 * @complexity O(V), where V is the number of vertices.
 */
graph* graph_create(int vertices);

/**
 * @brief Adds an edge between two vertices in the graph (undirected).
 * 
 * @param g Pointer to the graph.
 * @param src Source vertex.
 * @param dest Destination vertex.
 * 
 * @complexity O(1) for each addition.
 */
void graph_add_edge(graph* g, int src, int dest);

/**
 * @brief Removes an edge between two vertices in the graph.
 * 
 * @param g Pointer to the graph.
 * @param src Source vertex.
 * @param dest Destination vertex.
 * 
 * @complexity O(E), where E is the number of edges adjacent to the vertices.
 */
void graph_remove_edge(graph* g, int src, int dest);

/**
 * @brief Checks if the graph has an Eulerian circuit (i.e., all vertices have even degrees).
 * 
 * @param g Pointer to the graph.
 * @return True if the graph has an Eulerian circuit, false otherwise.
 * 
 * @complexity O(V), where V is the number of vertices.
 */
bool graph_is_eulerian(graph* g);

/**
 * @brief Dumps the graph into a .dot file to be used with Graphviz.
 * 
 * @param g Pointer to the graph.
 * @param filename Name of the output file.
 * 
 * @complexity O(V + E), where V is the number of vertices and E is the number of edges.
 */
void graph_dump_to_dot(graph* g, const char* filename);

/**
 * @brief Frees all dynamically allocated memory in the graph.
 * 
 * @param g Pointer to the graph.
 * 
 * @complexity O(V + E), where V is the number of vertices and E is the number of edges.
 */
void graph_free(graph* g);

/**
 * @brief Finds the Eulerian circuit using Hierholzer's Algorithm.
 * 
 * @param g Pointer to the graph.
 * @param start_vertex Starting vertex for the algorithm.
 * @param circuit Stack to store the Eulerian circuit.
 * 
 * @complexity O(E), where E is the number of edges.
 */
void graph_hierholzer_algorithm(graph* g, int start_vertex, stack* circuit);

/**
 * @brief Adds an edge between two vertices and ensures both vertices have even degrees.
 * 
 * @param g Pointer to the graph.
 * @param src Source vertex.
 * @param dest Destination vertex.
 * 
 * @complexity O(1) for each addition.
 */
void add_edge_ensure_eulerian(graph* g, int src, int dest);

/**
 * @brief Creates an Eulerian graph with a specified number of vertices.
 * 
 * @param num_vertices Number of vertices in the graph.
 * @return Pointer to the newly created Eulerian graph.
 * 
 * @complexity O(V), where V is the number of vertices.
 */
graph* create_eulerian_graph(int num_vertices);

/**
 * @brief Creates multiple Eulerian graphs with varying sizes.
 * 
 * @param sizes Array of sizes for the Eulerian graphs.
 * @param count Number of graphs to create.
 * @return Array of pointers to the created Eulerian graphs.
 * 
 * @complexity O(V * C), where V is the number of vertices in each graph and C is the count.
 */
graph** create_multiple_eulerian_graphs(int* sizes, int count);

/**
 * @brief Checks if an edge between two vertices is a bridge.
 * 
 * @param g Pointer to the graph.
 * @param src Source vertex.
 * @param dest Destination vertex.
 * @return true if the edge is a bridge, false otherwise.
 */
bool is_bridge(graph* g, int src, int dest);

/**
 * @brief Implements Fleury's algorithm to find an Eulerian circuit.
 * 
 * @param g Pointer to the graph.
 * @param start_vertex Starting vertex for the algorithm.
 * @param circuit Stack to store the Eulerian circuit.
 */
void fleury_algorithm(graph* g, int start_vertex, stack* circuit);

/**
 * @brief Counts the number of connected components in the graph using DFS.
 * 
 * @param g Pointer to the graph.
 * @return The number of connected components in the graph.
 */
int count_connected_components(graph* g);

/**
 * @brief Perform DFS and mark visited vertices.
 * 
 * @param g Pointer to the graph.
 * @param vertex Current vertex to explore.
 * @param visited Array to mark visited vertices.
 */
void dfs(graph* g, int vertex, bool* visited);

/**
 * @brief Creates an Eulerian graph with a specified number of edges.
 * 
 * @param num_edges Number of edges in the graph.
 * @return Pointer to the newly created Eulerian graph.
 */
graph* create_eulerian_graph_by_edges(int num_edges);

#endif
