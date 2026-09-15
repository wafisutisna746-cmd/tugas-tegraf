#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EDGES 100
#define MAX_VERTICES 50

// Structure to represent an edge
typedef struct {
    char u;
    char v;
    int weight;
} Edge;

// Disjoint Set Union (DSU) structure
typedef struct {
    int parent[MAX_VERTICES];
    int rank[MAX_VERTICES];
} DSU;

// Initialize DSU
void initDSU(DSU *dsu, int n) {
    for (int i = 0; i < n; i++) {
        dsu->parent[i] = i;
        dsu->rank[i] = 0;
    }
}

// Find operation with path compression
int findParent(DSU *dsu, int node) {
    if (dsu->parent[node] != node) {
        dsu->parent[node] = findParent(dsu, dsu->parent[node]);
    }
    return dsu->parent[node];
}

// Union operation by rank
int unionSets(DSU *dsu, int u, int v) {
    int rootU = findParent(dsu, u);
    int rootV = findParent(dsu, v);

    if (rootU == rootV) {
        return 0; // Cycle detected
    }

    if (dsu->rank[rootU] < dsu->rank[rootV]) {
        dsu->parent[rootU] = rootV;
    } else if (dsu->rank[rootU] > dsu->rank[rootV]) {
        dsu->parent[rootV] = rootU;
    } else {
        dsu->parent[rootV] = rootU;
        dsu->rank[rootU]++;
    }
    return 1; // Successfully joined
}

// Comparator function to sort edges in ascending order of weight
int compareEdges(const void *a, const void *b) {
    Edge *e1 = (Edge *)a;
    Edge *e2 = (Edge *)b;
    return e1->weight - e2->weight;
}

// Helper function to get or assign an integer ID for a vertex label
int getVertexIndex(char name, char vertices[], int *vertexCount) {
    for (int i = 0; i < *vertexCount; i++) {
        if (vertices[i] == name) {
            return i;
        }
    }
    vertices[*vertexCount] = name;
    (*vertexCount)++;
    return *vertexCount - 1;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open input.txt\n");
        printf("Make sure input.txt is located in the same directory.\n");
        return 1;
    }

    Edge edges[MAX_EDGES];
    int edgeCount = 0;

    char vertices[MAX_VERTICES];
    int vertexCount = 0;

    // Read edges from file: <node1> <node2> <weight>
    while (fscanf(file, " %c %c %d", &edges[edgeCount].u, &edges[edgeCount].v, &edges[edgeCount].weight) == 3) {
        getVertexIndex(edges[edgeCount].u, vertices, &vertexCount);
        getVertexIndex(edges[edgeCount].v, vertices, &vertexCount);
        edgeCount++;
        if (edgeCount >= MAX_EDGES) break;
    }
    fclose(file);

    printf("=========================================\n");
    printf("           KRUSKAL'S ALGORITHM           \n");
    printf("=========================================\n");
    printf("Total Vertices: %d\n", vertexCount);
    printf("Total Edges   : %d\n\n", edgeCount);

    // Step 1: Sort all edges in non-decreasing order of weight
    qsort(edges, edgeCount, sizeof(Edge), compareEdges);

    printf("--- Sorted Edges by Weight ---\n");
    for (int i = 0; i < edgeCount; i++) {
        printf("%d. %c - %c : %d\n", i + 1, edges[i].u, edges[i].v, edges[i].weight);
    }
    printf("\n");

    // Step 2: Initialize DSU
    DSU dsu;
    initDSU(&dsu, vertexCount);

    Edge mst[MAX_VERTICES];
    int mstEdgeCount = 0;
    int totalWeight = 0;

    // Step 3: Iterate through sorted edges and apply Kruskal's algorithm
    printf("--- Kruskal's Step-by-Step Selection ---\n");
    for (int i = 0; i < edgeCount; i++) {
        int uIdx = getVertexIndex(edges[i].u, vertices, &vertexCount);
        int vIdx = getVertexIndex(edges[i].v, vertices, &vertexCount);

        if (unionSets(&dsu, uIdx, vIdx)) {
            mst[mstEdgeCount++] = edges[i];
            totalWeight += edges[i].weight;
            printf("Edge %c - %c (Weight: %d) -> ADDED to MST\n", edges[i].u, edges[i].v, edges[i].weight);
        } else {
            printf("Edge %c - %c (Weight: %d) -> REJECTED (Forms cycle)\n", edges[i].u, edges[i].v, edges[i].weight);
        }

        // An MST always has exactly (V - 1) edges
        if (mstEdgeCount == vertexCount - 1) {
            break;
        }
    }

    // Step 4: Display MST Results
    printf("\n=========================================\n");
    printf("       MINIMUM SPANNING TREE (MST)       \n");
    printf("=========================================\n");
    for (int i = 0; i < mstEdgeCount; i++) {
        printf("  %c - %c  |  Weight: %d\n", mst[i].u, mst[i].v, mst[i].weight);
    }
    printf("-----------------------------------------\n");
    printf("Total Minimum Weight: %d\n", totalWeight);
    printf("=========================================\n");

    return 0;
}
