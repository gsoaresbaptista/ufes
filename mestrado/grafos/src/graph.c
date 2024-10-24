#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graph.h"

// Create a new node for the adjacency list
node* graph_create_node(int vertex) {
    node* new_node = (node*)malloc(sizeof(node));
    new_node->vertex = vertex;
    new_node->next = NULL;
    return new_node;
}

// Create a graph with 'vertices' vertices
graph* graph_create(int vertices) {
    graph* g = (graph*)malloc(sizeof(graph));
    g->num_vertices = vertices;
    g->adj_lists = (node**)malloc(vertices * sizeof(node*));
    g->degrees = (int*)malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        g->adj_lists[i] = NULL;
        g->degrees[i] = 0;
    }
    return g;
}

// Add an edge to the graph (undirected)
void graph_add_edge(graph* g, int src, int dest) {
    node* new_node = graph_create_node(dest);
    new_node->next = g->adj_lists[src];
    g->adj_lists[src] = new_node;
    g->degrees[src]++;

    new_node = graph_create_node(src);
    new_node->next = g->adj_lists[dest];
    g->adj_lists[dest] = new_node;
    g->degrees[dest]++;
}

// Remove an edge from the graph
void graph_remove_edge(graph* g, int src, int dest) {
    // Remove dest from src's adjacency list
    node* temp = g->adj_lists[src];
    node* prev = NULL;

    while (temp != NULL && temp->vertex != dest) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return; // Edge not found

    if (prev == NULL) {
        g->adj_lists[src] = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);

    // Remove src from dest's adjacency list (undirected graph)
    temp = g->adj_lists[dest];
    prev = NULL;

    while (temp != NULL && temp->vertex != src) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    if (prev == NULL) {
        g->adj_lists[dest] = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
}

bool tarjan_dfs(graph* g, int u, bool* visited, int* discovery, int* low, int* parent, int src, int dest, int* time_counter) {
    visited[u] = true;
    discovery[u] = low[u] = ++(*time_counter);

    node* adj = g->adj_lists[u];
    while (adj != NULL) {
        int v = adj->vertex; 

        if (!visited[v]) {
            parent[v] = u;
            if (tarjan_dfs(g, v, visited, discovery, low, parent, src, dest, time_counter)) {
                return true;
            }

            low[u] = (low[u] < low[v]) ? low[u] : low[v];

            if (low[v] > discovery[u]) {
                if ((u == src && v == dest) || (u == dest && v == src)) {
                    return true;  // The edge (src, dest) is a bridge
                }
            }
        }
        else if (v != parent[u]) {
            low[u] = (low[u] < discovery[v]) ? low[u] : discovery[v];
        }

        adj = adj->next;
    }

    return false;
}

// Check if the graph has an Eulerian circuit
bool graph_is_eulerian(graph* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        if (g->degrees[i] % 2 != 0) {
            return false;
        }
    }
    return true;
}

// Function to dump the graph into a .dot file
void graph_dump_to_dot(graph* g, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing: %s\n", filename);
        return;
    }

    // Start the graph definition
    fprintf(file, "graph G {\n");
    fprintf(file, "\tlayout = neato;\n");
    fprintf(file, "\tsplines = true;\n");
    fprintf(file, "\tnode [shape=point, width=0.05, color=\"#FF9B9B\"];\n");
    fprintf(file, "\tedge [color=\"#606676\"];\n");
    fprintf(file, "\tbgcolor=\"none\";\n");

    // Loop over all vertices in the graph and create nodes with letters near the points
    for (int i = 0; i < g->num_vertices; i++) {
        char label = 'A' + i;
        fprintf(file, "\t%d [xlabel=\"%c\", fontsize=12, fontname=\"Arial\", fontcolor=\"#FF9B9B\"];\n", i, label);
    }

    // Loop over all vertices and add edges between them
    for (int i = 0; i < g->num_vertices; i++) {
        node* current = g->adj_lists[i];
        while (current != NULL) {
            // Avoid duplicate edges in an undirected graph
            if (i < current->vertex) {
                fprintf(file, "\t%d -- %d;\n", i, current->vertex);  // Undirected edge from i to vertex
            }
            current = current->next;
        }
    }

    fprintf(file, "}\n");  // End of graph
    fclose(file);

    printf("Graph dumped to %s\n", filename);
}

// Free all dynamically allocated memory in the graph
void graph_free(graph* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        node* temp = g->adj_lists[i];
        while (temp) {
            node* to_free = temp;
            temp = temp->next;
            free(to_free);
        }
    }
    free(g->adj_lists);
    free(g->degrees);
    free(g);
}

// Function to check if a vertex has any unvisited edges
static bool graph_has_unvisited_edges(graph* g, int vertex) {
    return g->adj_lists[vertex] != NULL;
}

void graph_hierholzer_algorithm(graph* g, int start_vertex, stack* circuit) {
    stack* current_path = stack_create(10);
    stack_push(current_path, start_vertex);

    while (current_path->top != -1) {
        int current_vertex = current_path->items[current_path->top];

        if (graph_has_unvisited_edges(g, current_vertex)) {
            node* next_node = g->adj_lists[current_vertex];
            int next_vertex = next_node->vertex;

            graph_remove_edge(g, current_vertex, next_vertex);
            stack_push(current_path, next_vertex);
        } else {
            int vertex = stack_pop(current_path);
            stack_push(circuit, vertex);
        }
    }

    stack_free(current_path);
}

graph* create_eulerian_graph(int num_vertices) {
    graph* g = graph_create(num_vertices);

    for (int i = 0; i < num_vertices - 1; i++) {
        add_edge_ensure_eulerian(g, i, i + 1);
    }
    add_edge_ensure_eulerian(g, num_vertices - 1, 0);

    return g;
}

graph** create_multiple_eulerian_graphs(int* sizes, int count) {
    graph** graphs = (graph**)malloc(count * sizeof(graph*));
    for (int i = 0; i < count; i++) {
        graphs[i] = create_eulerian_graph(sizes[i]);
    }
    return graphs;
}

bool is_bridge(graph* g, int src, int dest) {
    int V = g->num_vertices;

    // Arrays for Tarjan's algorithm
    bool* visited = (bool*)malloc(V * sizeof(bool));
    int* discovery = (int*)malloc(V * sizeof(int));
    int* low = (int*)malloc(V * sizeof(int));
    int* parent = (int*)malloc(V * sizeof(int));

    // Initialize arrays
    for (int i = 0; i < V; i++) {
        visited[i] = false;
        discovery[i] = -1;
        low[i] = -1;
        parent[i] = -1;
    }

    int time_counter = 0;

    // Call DFS from the src vertex to determine if (src, dest) is a bridge
    bool is_bridge_edge = tarjan_dfs(g, src, visited, discovery, low, parent, src, dest, &time_counter);

    // Clean up
    free(visited);
    free(discovery);
    free(low);
    free(parent);

    return is_bridge_edge;
}

void fleury_algorithm(graph* g, int start_vertex, stack* circuit) {
    int current_vertex = start_vertex;

    while (g->adj_lists[current_vertex] != NULL) {
        node* temp = g->adj_lists[current_vertex];
        int next_vertex = -1;

        // If there is only one edge, we can skip the bridge check
        if (temp->next == NULL) {
            next_vertex = temp->vertex;
        } else {
            // Traverse the adjacency list and find a non-bridge edge
            while (temp != NULL) {
                next_vertex = temp->vertex;

                // Only check for a bridge if there are multiple edges
                if (!is_bridge(g, current_vertex, next_vertex)) {
                    break; // Found a non-bridge edge
                }

                temp = temp->next;
            }
        }

        // If we reach here, either we've found a non-bridge edge or taken the only edge available
        stack_push(circuit, next_vertex);
        graph_remove_edge(g, current_vertex, next_vertex);  // Remove the edge
        current_vertex = next_vertex;  // Move to the next vertex
    }
}

void dfs(graph* g, int vertex, bool* visited) {
    visited[vertex] = true;

    node* adj_list = g->adj_lists[vertex];
    while (adj_list != NULL) {
        int adjacent_vertex = adj_list->vertex;
        if (!visited[adjacent_vertex]) {
            dfs(g, adjacent_vertex, visited);
        }
        adj_list = adj_list->next;
    }
}

int count_connected_components(graph* g) {
    int num_components = 0;
    bool* visited = (bool*)malloc(g->num_vertices * sizeof(bool));

    // Initialize all vertices as not visited
    for (int i = 0; i < g->num_vertices; i++) {
        visited[i] = false;
    }

    // Perform DFS for each unvisited vertex and count the components
    for (int i = 0; i < g->num_vertices; i++) {
        if (!visited[i]) {
            dfs(g, i, visited);
            num_components++;
        }
    }

    free(visited);
    return num_components;
}

void add_edge_ensure_eulerian(graph* g, int src, int dest) {
    graph_add_edge(g, src, dest);
    g->degrees[src]++;
    g->degrees[dest]++;
}

graph* create_eulerian_graph_by_edges(int num_edges) {
    int num_vertices = num_edges / 2;  // Start with enough vertices for even-degree edges
    graph* g = graph_create(num_vertices);

    // Add edges to ensure an Eulerian circuit
    for (int i = 0; i < num_vertices - 1; i++) {
        add_edge_ensure_eulerian(g, i, i + 1);  // Add a cycle
    }
    add_edge_ensure_eulerian(g, num_vertices - 1, 0);  // Complete the cycle

    // Add additional edges until we reach the required number of edges
    int edges_added = num_vertices;
    while (edges_added < num_edges) {
        int src = rand() % num_vertices;
        int dest = rand() % num_vertices;
        if (src != dest) {  // Avoid self-loops
            add_edge_ensure_eulerian(g, src, dest);
            edges_added++;
        }
    }

    return g;
}
