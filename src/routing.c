#include <stdio.h>
#include <string.h>
#include "routing.h"
#include "utils.h"

static PathResult reconstruct(const Graph *g, int src, int dst, int *prev, double score) {
    PathResult r;
    memset(&r, 0, sizeof(r));
    r.reachable = 0;
    r.score = score;
    if (prev[dst] == -1 && src != dst) return r;
    int rev[MAX_NODES], n = 0;
    for (int at = dst; at != -1; at = prev[at]) rev[n++] = at;
    for (int i = 0; i < n; i++) r.nodes[i] = rev[n - i - 1];
    r.length = n;
    r.reachable = 1;
    for (int i = 0; i + 1 < r.length; i++) r.total_latency += graph_get_latency(g, r.nodes[i], r.nodes[i + 1]);
    return r;
}

PathResult dijkstra_route(const Graph *g, int src, int dst, double priority_factor) {
    double dist[MAX_NODES];
    int prev[MAX_NODES];
    int done[MAX_NODES] = {0};
    MinHeap heap;
    heap_init(&heap);
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[src] = 0;
    heap_push(&heap, src, 0);
    while (!heap_empty(&heap)) {
        HeapItem item = heap_pop(&heap);
        int u = item.node;
        if (u < 0 || done[u]) continue;
        done[u] = 1;
        if (u == dst) break;
        if (!g->nodes[u].active) continue;
        for (Edge *e = g->nodes[u].edges; e; e = e->next) {
            int v = e->to;
            if (!e->active || !g->nodes[v].active) continue;
            double risk_penalty = g->nodes[v].risk_score * priority_factor;
            double nd = dist[u] + e->latency + risk_penalty;
            if (nd < dist[v]) {
                dist[v] = nd;
                prev[v] = u;
                heap_push(&heap, v, nd);
            }
        }
    }
    return reconstruct(g, src, dst, prev, dist[dst]);
}

PathResult bellman_ford_route(const Graph *g, int src, int dst) {
    int dist[MAX_NODES], prev[MAX_NODES];
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[src] = 0;
    for (int pass = 0; pass < g->node_count - 1; pass++) {
        int changed = 0;
        for (int u = 0; u < g->node_count; u++) {
            if (dist[u] == INF || !g->nodes[u].active) continue;
            for (Edge *e = g->nodes[u].edges; e; e = e->next) {
                if (!e->active || !g->nodes[e->to].active) continue;
                if (dist[u] + e->latency < dist[e->to]) {
                    dist[e->to] = dist[u] + e->latency;
                    prev[e->to] = u;
                    changed = 1;
                }
            }
        }
        if (!changed) break;
    }
    return reconstruct(g, src, dst, prev, dist[dst]);
}

void routing_print_path(const Graph *g, PathResult result, const char *label) {
    char text[256];
    if (!result.reachable) {
        printf("\n%s\n", label);
        printf("  Result: no reachable path with the current active links and nodes.\n");
        return;
    }
    path_to_string(g, result.nodes, result.length, text, sizeof(text));
    printf("\n%s\n", label);
    printf("  Path: %s\n", text);
    printf("  Meaning: traffic would cross %d link%s and take about %d ms end to end.\n",
           result.length - 1, result.length == 2 ? "" : "s", result.total_latency);
    printf("  Algorithm score: %.1f", result.score);
    if (strstr(label, "secure")) {
        printf(" (latency plus risk penalty; lower is better)\n");
    } else {
        printf(" (routing cost used by the algorithm; lower is better)\n");
    }
}

void routing_compare(const Graph *g, const char *src_name, const char *dst_name) {
    int src = graph_find_index(g, src_name);
    int dst = graph_find_index(g, dst_name);
    if (src < 0 || dst < 0) {
        printf("Invalid route endpoints.\n");
        return;
    }
    printf("\n=== ROUTING COMPARISON %s -> %s ===\n", src_name, dst_name);
    printf("This compares three ways to choose a path. Dijkstra fastest optimizes latency only,\n");
    printf("Bi-criteria secure also avoids risky nodes, and Bellman-Ford is a baseline shortest-path check.\n");
    routing_print_path(g, dijkstra_route(g, src, dst, 0.0), "Dijkstra fastest");
    routing_print_path(g, dijkstra_route(g, src, dst, 0.8), "Bi-criteria secure");
    routing_print_path(g, bellman_ford_route(g, src, dst), "Bellman-Ford");
}
