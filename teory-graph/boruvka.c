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

int find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

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

void run_boruvka(int skip_node) {
    for (int i = 0; i < N; i++) { parent[i] = i; rnk[i] = 0; }

    int num_components = (skip_node == -1) ? N : N - 1;
    int total_cost = 0;
    int round = 0;

    int cheapest[MAX_N]; 

    while (num_components > 1) {
        round++;
        for (int i = 0; i < N; i++) cheapest[i] = -1;

        
        for (int i = 0; i < M; i++) {
            if (edges[i].w == -1) continue;
            if (edges[i].u == skip_node || edges[i].v == skip_node) continue;

            int set_u = find(edges[i].u);
            int set_v = find(edges[i].v);
            if (set_u == set_v) continue; 

            if (cheapest[set_u] == -1 || edges[i].w < edges[cheapest[set_u]].w)
                cheapest[set_u] = i;
            if (cheapest[set_v] == -1 || edges[i].w < edges[cheapest[set_v]].w)
                cheapest[set_v] = i;
        }

        int merged_this_round = 0;
        printf("-- Round %d --\n", round);

        for (int c = 0; c < N; c++) {
            if (c == skip_node) continue;
            if (cheapest[c] == -1) continue;

            Edge e = edges[cheapest[c]];
            int set_u = find(e.u);
            int set_v = find(e.v);
            if (set_u == set_v) continue; /* already merged earlier this round */

            printf("  Component of %-3s merges via edge %s-%s (weight %d)\n",
                   labels[c], labels[e.u], labels[e.v], e.w);

            parent[set_u] = set_v;
            total_cost += e.w;
            num_components--;
            merged_this_round++;
        }

        if (merged_this_round == 0) {
            printf("  No further merges possible - graph is disconnected.\n");
            break;
        }
    }

    printf("\nTotal MST cost: %d\n", total_cost);
    if (num_components > 1) {
        printf("WARNING: graph could not be fully connected - %d separate "
               "components remain.\n", num_components);
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

    printf("Boruvka's Algorithm\n\n");
    run_boruvka(skip_node);

    return 0;
}
