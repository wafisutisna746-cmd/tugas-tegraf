#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 100
#define MAX_M 200

int N, M;
char labels[MAX_N][32];

typedef struct {
    int u, v, w;
} Edge;

Edge edges[MAX_M];
int parent[MAX_N], rnk[MAX_N];

void read_graph(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", path);
        exit(1);
    }

    fscanf(f, "%d", &N);
    for (int i = 0; i < N; i++) fscanf(f, "%s", labels[i]);
    fscanf(f, "%d", &M);

    for (int i = 0; i < M; i++)
        fscanf(f, "%d %d %d", &edges[i].u, &edges[i].v, &edges[i].w);

    fclose(f);
}

int cmp_edge(const void *a, const void *b) {
    return ((Edge *)a)->w - ((Edge *)b)->w;
}

int find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]]; /* path compression */
        x = parent[x];
    }
    return x;
}

int union_sets(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return 0; /* already connected -> would create a cycle */
    if (rnk[a] < rnk[b]) { int t = a; a = b; b = t; }
    parent[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
    return 1;
}

/* Remove edges touching a failed node by marking weight = -1 (sentinel) */
void fail_node(int node) {
    for (int i = 0; i < M; i++)
        if (edges[i].u == node || edges[i].v == node)
            edges[i].w = -1;
    printf("[simulation] Node %s marked unreachable (its edges disabled)\n\n", labels[node]);
}

void fail_edge(int u, int v) {
    for (int i = 0; i < M; i++) {
        if ((edges[i].u == u && edges[i].v == v) ||
            (edges[i].u == v && edges[i].v == u)) {
            edges[i].w = -1;
        }
    }
    printf("[simulation] Edge %s-%s marked unusable\n\n", labels[u], labels[v]);
}

void run_kruskal(int skip_node) {
    for (int i = 0; i < N; i++) { parent[i] = i; rnk[i] = 0; }

    qsort(edges, M, sizeof(Edge), cmp_edge);

    int total_cost = 0, edges_used = 0;

    printf("Step | Edge considered | Weight | Result\n");
    printf("------------------------------------------\n");

    for (int i = 0; i < M; i++) {
        if (edges[i].w == -1) continue;             /* disabled edge */
        if (edges[i].u == skip_node || edges[i].v == skip_node) continue;

        int accepted = union_sets(edges[i].u, edges[i].v);
        printf("%4d | %s-%-10s | %6d | %s\n",
               i + 1,
               labels[edges[i].u], labels[edges[i].v],
               edges[i].w,
               accepted ? "added" : "skipped (cycle)");

        if (accepted) {
            total_cost += edges[i].w;
            edges_used++;
        }
    }

    printf("------------------------------------------\n");
    printf("Total MST cost: %d\n", total_cost);

    int expected_edges = (skip_node == -1) ? N - 1 : N - 2;
    if (edges_used < expected_edges) {
        printf("WARNING: graph became disconnected — only %d edges could be added "
               "(needed %d).\n", edges_used, expected_edges);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input.txt> [--fail-edge u v | --fail-node n]\n", argv[0]);
        return 1;
    }

    read_graph(argv[1]);

    int skip_node = -1;
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--fail-edge") == 0 && i + 2 < argc) {
            fail_edge(atoi(argv[i + 1]), atoi(argv[i + 2]));
            i += 2;
        } else if (strcmp(argv[i], "--fail-node") == 0 && i + 1 < argc) {
            skip_node = atoi(argv[i + 1]);
            fail_node(skip_node);
            i += 1;
        }
    }

    printf("Kruskal's Algorithm\n\n");
    run_kruskal(skip_node);

    return 0;
}
