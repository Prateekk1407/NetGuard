#include <stdio.h>
#include <string.h>
#include "flow.h"
#include "utils.h"

static int bfs_residual(int n, int cap[MAX_NODES][MAX_NODES], int src, int dst, int *parent) {
    int visited[MAX_NODES] = {0};
    Queue q;
    queue_init(&q, MAX_NODES);
    for (int i = 0; i < n; i++) parent[i] = -1;
    visited[src] = 1;
    queue_push(&q, src);
    while (!queue_empty(&q)) {
        int u = queue_pop(&q);
        for (int v = 0; v < n; v++) {
            if (!visited[v] && cap[u][v] > 0) {
                parent[v] = u;
                visited[v] = 1;
                if (v == dst) return 1;
                queue_push(&q, v);
            }
        }
    }
    return 0;
}

FlowResult flow_max_and_cut(const Graph *g, int src, int dst) {
    FlowResult result;
    memset(&result, 0, sizeof(result));
    int cap[MAX_NODES][MAX_NODES] = {{0}};
    int original[MAX_NODES][MAX_NODES] = {{0}};
    for (int u = 0; u < g->node_count; u++) {
        for (Edge *e = g->nodes[u].edges; e; e = e->next) {
            if (e->active && g->nodes[u].active && g->nodes[e->to].active) {
                cap[u][e->to] += e->bandwidth;
                original[u][e->to] += e->bandwidth;
            }
        }
    }
    int parent[MAX_NODES];
    while (bfs_residual(g->node_count, cap, src, dst, parent)) {
        int aug = INF;
        for (int v = dst; v != src; v = parent[v]) {
            int u = parent[v];
            if (cap[u][v] < aug) aug = cap[u][v];
        }
        for (int v = dst; v != src; v = parent[v]) {
            int u = parent[v];
            cap[u][v] -= aug;
            cap[v][u] += aug;
        }
        result.max_flow += aug;
    }
    int reachable[MAX_NODES] = {0};
    Queue q;
    queue_init(&q, MAX_NODES);
    reachable[src] = 1;
    queue_push(&q, src);
    while (!queue_empty(&q)) {
        int u = queue_pop(&q);
        for (int v = 0; v < g->node_count; v++) {
            if (!reachable[v] && cap[u][v] > 0) {
                reachable[v] = 1;
                queue_push(&q, v);
            }
        }
    }
    for (int u = 0; u < g->node_count; u++) {
        for (int v = 0; v < g->node_count; v++) {
            if (reachable[u] && !reachable[v] && original[u][v] > 0) {
                result.cut_from[result.cut_count] = u;
                result.cut_to[result.cut_count] = v;
                result.cut_count++;
            }
        }
    }
    return result;
}

void flow_print_analysis(const Graph *g, const char *src_name, const char *dst_name) {
    int src = graph_find_index(g, src_name);
    int dst = graph_find_index(g, dst_name);
    if (src < 0 || dst < 0) {
        printf("Invalid flow endpoints.\n");
        return;
    }
    FlowResult r = flow_max_and_cut(g, src, dst);
    printf("\n=== MAX FLOW / MIN CUT %s -> %s ===\n", src_name, dst_name);
    printf("Maximum throughput: %d Mbps\n", r.max_flow);
    printf("Meaning: this is the most traffic the active links can carry from %s to %s\n", src_name, dst_name);
    printf("before some link on every possible route becomes saturated.\n");
    printf("\nMinimum cut firewall positions:\n");
    printf("These links form the narrowest boundary between source and destination. Adding\n");
    printf("firewalling, monitoring, or redundancy here gives the biggest control point.\n");
    if (r.cut_count == 0) {
        printf("  No cut links found. The destination may be unreachable or capacity is zero.\n");
    }
    for (int i = 0; i < r.cut_count; i++) {
        printf("  %s -> %s: protect this boundary because all max-flow traffic depends on it.\n",
               g->nodes[r.cut_from[i]].name, g->nodes[r.cut_to[i]].name);
    }
}

void flow_firewall_recommendations(const Graph *g) {
    printf("\n=== FIREWALL PLACEMENT RECOMMENDATIONS ===\n");
    printf("Recommendations prioritize high-risk nodes that do not already have firewall coverage.\n");
    int shown = 0;
    for (int i = 0; i < g->node_count && shown < 5; i++) {
        if (!g->nodes[i].firewall && g->nodes[i].risk_score >= 55) {
            printf("%d. Place firewall at %s\n", shown + 1, g->nodes[i].name);
            printf("   Why: current risk is %.1f/100, so expected protection gain is about %.0f%%.\n",
                   g->nodes[i].risk_score, g->nodes[i].risk_score * 0.55);
            shown++;
        }
    }
    if (!shown) printf("Existing firewall coverage is adequate for current risk threshold.\n");
}
