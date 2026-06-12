#include <stdio.h>
#include <string.h>
#include "security.h"
#include "utils.h"

static void dfs_tarjan(const Graph *g, int u, int parent, int *time, int *disc, int *low, int *child_count, SecurityReport *r) {
    disc[u] = low[u] = ++(*time);
    for (Edge *e = g->nodes[u].edges; e; e = e->next) {
        int v = e->to;
        if (!e->active || !g->nodes[v].active) continue;
        if (v == parent) continue;
        if (!disc[v]) {
            child_count[u]++;
            dfs_tarjan(g, v, u, time, disc, low, child_count, r);
            if (low[v] < low[u]) low[u] = low[v];
            if (parent != -1 && low[v] >= disc[u]) r->articulation[u] = 1;
            if (low[v] > disc[u] && r->bridge_count < MAX_NODES * 2) {
                r->bridges[r->bridge_count].from = u;
                r->bridges[r->bridge_count].to = v;
                r->bridges[r->bridge_count].criticality = 50 + g->nodes[u].sensitivity * 6 + g->nodes[v].sensitivity * 6;
                if (r->bridges[r->bridge_count].criticality > 100) r->bridges[r->bridge_count].criticality = 100;
                r->bridge_count++;
            }
        } else if (disc[v] < low[u]) {
            low[u] = disc[v];
        }
    }
    if (parent == -1 && child_count[u] > 1) r->articulation[u] = 1;
}

static void compute_centrality(const Graph *g, SecurityReport *r) {
    for (int s = 0; s < g->node_count; s++) {
        for (int t = 0; t < g->node_count; t++) {
            if (s == t) continue;
            PathResult p = dijkstra_route(g, s, t, 0.0);
            if (!p.reachable) continue;
            for (int i = 1; i + 1 < p.length; i++) r->centrality[p.nodes[i]] += 1.0;
        }
    }
    double max = 0;
    for (int i = 0; i < g->node_count; i++) if (r->centrality[i] > max) max = r->centrality[i];
    if (max > 0) {
        for (int i = 0; i < g->node_count; i++) r->centrality[i] /= max;
    }
}

void security_analyze(Graph *g, SecurityReport *r) {
    memset(r, 0, sizeof(*r));
    int disc[MAX_NODES] = {0}, low[MAX_NODES] = {0}, child_count[MAX_NODES] = {0}, time = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].active && !disc[i]) dfs_tarjan(g, i, -1, &time, disc, low, child_count, r);
    }
    compute_centrality(g, r);
    for (int i = 0; i < g->node_count; i++) {
        double risk = g->nodes[i].sensitivity * 12.0 + r->centrality[i] * 45.0;
        if (r->articulation[i]) {
            risk += 22.0;
            r->articulation_impact[i] = 25 + child_count[i] * 10;
            if (r->articulation_impact[i] > 95) r->articulation_impact[i] = 95;
        }
        if (!g->nodes[i].firewall) risk += 10.0;
        if (strcmp(g->nodes[i].type, "database") == 0 || strcmp(g->nodes[i].type, "admin") == 0) risk += 12.0;
        if (!g->nodes[i].active) risk = 100.0;
        if (risk > 100.0) risk = 100.0;
        g->nodes[i].risk_score = risk;
    }
}

double security_path_risk(const Graph *g, const PathResult *path) {
    if (!path->reachable || path->length == 0) return 100.0;
    double risk = 0;
    for (int i = 0; i < path->length; i++) if (g->nodes[path->nodes[i]].risk_score > risk) risk = g->nodes[path->nodes[i]].risk_score;
    return risk;
}

void security_print(const Graph *g, const SecurityReport *r) {
    printf("\n=== CRITICAL INFRASTRUCTURE ===\n");
    printf("A bridge is a link whose failure can split the active network. An articulation\n");
    printf("point is a node whose failure can isolate part of the topology.\n\n");
    if (r->bridge_count == 0) printf("[OK] No bridge links found in the active topology.\n");
    for (int i = 0; i < r->bridge_count; i++) {
        printf("[!] Bridge link: %s <-> %s\n",
               g->nodes[r->bridges[i].from].name, g->nodes[r->bridges[i].to].name);
        printf("    Meaning: if this link goes down, some traffic may lose its only route.\n");
        printf("    Criticality: %d/100 (%s priority to protect or add redundancy)\n",
               r->bridges[i].criticality, r->bridges[i].criticality >= 75 ? "high" : "medium");
    }
    for (int i = 0; i < g->node_count; i++) {
        if (r->articulation[i]) {
            printf("[!] Articulation node: %s\n", g->nodes[i].name);
            printf("    Meaning: this node is a single point of failure for part of the network.\n");
            printf("    Estimated outage impact: %d/100\n", r->articulation_impact[i]);
        }
    }
    printf("\n=== TRAFFIC HEATMAP / RISK SCORES ===\n");
    printf("Centrality shows how often a node appears on shortest paths. Risk combines\n");
    printf("centrality, sensitivity, firewall coverage, role, and offline status.\n\n");
    int shown = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].risk_score >= 60 || r->centrality[i] > 0.35) {
            printf("%s (%s)\n", g->nodes[i].name, g->nodes[i].type);
            printf("  Centrality: %.2f/1.00 - %s traffic dependency\n",
                   r->centrality[i], r->centrality[i] >= 0.70 ? "very high" : r->centrality[i] >= 0.35 ? "noticeable" : "low");
            printf("  Risk score: %.1f/100 - %s\n",
                   g->nodes[i].risk_score, risk_label(g->nodes[i].risk_score));
            printf("  Why it matters: %s%s%s\n",
                   g->nodes[i].firewall ? "firewall protected; " : "no firewall; ",
                   g->nodes[i].sensitivity >= 4 ? "sensitive asset; " : "normal sensitivity; ",
                   r->articulation[i] ? "single point of failure" : "not a single point of failure");
            shown++;
        }
    }
    if (!shown) printf("[OK] No nodes crossed the risk or traffic-dependency display threshold.\n");
}

void security_attack_paths(const Graph *g, const char *entry_name) {
    int src = graph_find_index(g, entry_name);
    if (src < 0) {
        printf("Invalid attacker entry node.\n");
        return;
    }
    printf("\n=== ATTACK PATH SIMULATION FROM %s ===\n", entry_name);
    printf("Each target below is a sensitive asset. Path exposure is the highest node risk\n");
    printf("encountered along that route, so higher values mean an attacker has a more dangerous path.\n");
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].sensitivity >= 4 || strcmp(g->nodes[i].type, "database") == 0 || strcmp(g->nodes[i].type, "admin") == 0) {
            PathResult p = dijkstra_route(g, src, i, 0.25);
            routing_print_path(g, p, g->nodes[i].name);
            if (p.reachable) {
                double exposure = security_path_risk(g, &p);
                printf("  Path exposure: %.1f/100 - %s attacker route\n",
                       exposure, exposure >= 70.0 ? "high-risk" : exposure >= 45.0 ? "moderate-risk" : "lower-risk");
            }
        }
    }
}

void security_ddos_simulation(const Graph *g, const char *start_name) {
    int start = graph_find_index(g, start_name);
    if (start < 0) {
        printf("Invalid DDoS node.\n");
        return;
    }
    int visited[MAX_NODES] = {0}, depth[MAX_NODES] = {0};
    Queue q;
    queue_init(&q, MAX_NODES);
    visited[start] = 1;
    queue_push(&q, start);
    printf("\n=== DDOS CASCADE FROM %s ===\n", start_name);
    printf("Hop 0 is the starting node. Each hop is one link farther away. Overflow load is\n");
    printf("an estimated pressure level after the attack spreads through neighboring links.\n\n");
    while (!queue_empty(&q)) {
        int u = queue_pop(&q);
        int load = 100 - depth[u] * 18;
        if (load < 15) load = 15;
        printf("Hop %d: %s\n", depth[u], g->nodes[u].name);
        printf("  Estimated overflow load: %d%%\n", load);
        printf("  Status: %s - %s\n",
               load > 55 ? "DEGRADED" : "STRESSED",
               load > 55 ? "service quality is likely to drop" : "node is under pressure but may keep serving traffic");
        for (Edge *e = g->nodes[u].edges; e; e = e->next) {
            if (e->active && !visited[e->to] && g->nodes[e->to].active) {
                visited[e->to] = 1;
                depth[e->to] = depth[u] + 1;
                queue_push(&q, e->to);
            }
        }
    }
}

int security_resilience_score(const Graph *g, const SecurityReport *r) {
    int firewall_count = 0;
    double avg_risk = 0;
    for (int i = 0; i < g->node_count; i++) {
        firewall_count += g->nodes[i].firewall;
        avg_risk += g->nodes[i].risk_score;
    }
    avg_risk /= g->node_count;
    int score = 100 - (int)(avg_risk * 0.55) - r->bridge_count * 4;
    score += firewall_count * 2;
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    return score;
}
