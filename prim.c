/*
 * Prim's Algorithm - Minimum Spanning Tree (MST)
 * -----------------------------------------------
 * Reads an edge list from a file (format: VERTEX1 VERTEX2 WEIGHT,
 * one edge per line, vertices given as single letters e.g. A B 7).
 * Builds an adjacency matrix, then runs the classic O(V^2) Prim's
 * algorithm to find the Minimum Spanning Tree.
 *
 * Usage:
 *   ./prims input.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_V 26   /* enough for vertices 'A'..'Z' */

int graph[MAX_V][MAX_V];   /* adjacency matrix, INT_MAX = no edge */
int seenVertex[MAX_V];     /* mark which letters actually appeared */
int numVertices = 0;

/* map a letter (A-Z) to a 0-based index and record that it was seen */
int vertexIndex(char c) {
    int idx = c - 'A';
    if (idx < 0 || idx >= MAX_V) {
        fprintf(stderr, "Invalid vertex label: %c\n", c);
        exit(1);
    }
    if (!seenVertex[idx]) {
        seenVertex[idx] = 1;
        numVertices++;
    }
    return idx;
}

char indexToVertex(int idx) {
    return (char)('A' + idx);
}

void initGraph(void) {
    for (int i = 0; i < MAX_V; i++)
        for (int j = 0; j < MAX_V; j++)
            graph[i][j] = INT_MAX;
}

/* read edges from file into the adjacency matrix */
void readGraph(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Could not open input file");
        exit(1);
    }

    char line[256];
    char v1, v2;
    int weight;

    while (fgets(line, sizeof(line), fp)) {
        /* skip blank lines and comment lines starting with '#' */
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '#' || *p == '\n' || *p == '\0') continue;

        if (sscanf(line, " %c %c %d", &v1, &v2, &weight) == 3) {
            int i = vertexIndex(v1);
            int j = vertexIndex(v2);
            graph[i][j] = weight;
            graph[j][i] = weight;   /* undirected graph */
        }
    }

    fclose(fp);
}

/* run Prim's algorithm starting from vertex 0 (the first vertex seen) */
void primMST(void) {
    int inMST[MAX_V] = {0};   /* 1 if vertex is already in the MST */
    int key[MAX_V];           /* smallest known weight connecting to MST */
    int parent[MAX_V];        /* which MST vertex the edge comes from */

    for (int i = 0; i < MAX_V; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
    }

    /* find the first vertex that was actually used, use it as the root */
    int start = -1;
    for (int i = 0; i < MAX_V; i++) {
        if (seenVertex[i]) { start = i; break; }
    }
    if (start == -1) {
        printf("No vertices found in input.\n");
        return;
    }

    key[start] = 0;
    int totalCost = 0;

    for (int count = 0; count < numVertices; count++) {

        /* --- pick the vertex outside the MST with the smallest key --- */
        int u = -1, minKey = INT_MAX;
        for (int v = 0; v < MAX_V; v++) {
            if (seenVertex[v] && !inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1) break;  /* remaining vertices are unreachable */

        inMST[u] = 1;
        totalCost += (key[u] == INT_MAX ? 0 : key[u]);

        if (parent[u] != -1) {
            printf("Edge: %c - %c \t weight = %d\n",
                   indexToVertex(parent[u]), indexToVertex(u), key[u]);
        }

        /* --- update keys of vertices adjacent to u --- */
        for (int v = 0; v < MAX_V; v++) {
            if (seenVertex[v] && !inMST[v] &&
                graph[u][v] != INT_MAX && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }

    /* check every vertex got connected */
    int connected = 1;
    for (int v = 0; v < MAX_V; v++) {
        if (seenVertex[v] && !inMST[v]) connected = 0;
    }

    printf("--------------------------------\n");
    if (connected)
        printf("Total cost of MST = %d\n", totalCost);
    else
        printf("Graph is disconnected: no single MST covers all vertices.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    initGraph();
    readGraph(argv[1]);

    printf("Loaded %d vertices.\n", numVertices);
    printf("Running Prim's Algorithm...\n\n");

    primMST();

    return 0;
}