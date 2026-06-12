#include <stdio.h>
#include <time.h>
#include "report.h"
#include "flow.h"
#include "utils.h"

void report_terminal(const Graph *g, const SecurityReport *security, const RoutingLog *log) {
    printf("\n=== NETGUARD VULNERABILITY REPORT ===\n");
    printf("Network Summary: %d nodes, %d links\n", g->node_count, graph_edge_count(g));
    printf("Resilience Score: %d/100 [%s]\n", security_resilience_score(g, security),
           security_resilience_score(g, security) >= 75 ? "STRONG" :
           security_resilience_score(g, security) >= 50 ? "MODERATE RISK" : "CRITICAL RISK");
    security_print(g, security);
    flow_firewall_recommendations(g);
    scheduler_print_log(log);
    printf("\n=== ACTION PLAN ===\n");
    printf("1. Add firewalls to nodes marked HIGH or CRITICAL without existing coverage.\n");
    printf("2. Add redundant links around bridges listed under critical infrastructure.\n");
    printf("3. Route sensitive packets with bi-criteria Dijkstra and audit alert cases.\n");
}

int report_export(const Graph *g, const SecurityReport *security, const RoutingLog *log, const char *path) {
    FILE *fp = fopen(path, "w");
    if (!fp) return 0;
    time_t now = time(NULL);
    fprintf(fp, "NetGuard Vulnerability Report\nGenerated: %s\n", ctime(&now));
    fprintf(fp, "Network Summary: %d nodes, %d links\n", g->node_count, graph_edge_count(g));
    fprintf(fp, "Resilience Score: %d/100\n\n", security_resilience_score(g, security));
    fprintf(fp, "Critical Bridges\n");
    for (int i = 0; i < security->bridge_count; i++) {
        fprintf(fp, "- %s <-> %s criticality=%d/100\n",
                g->nodes[security->bridges[i].from].name,
                g->nodes[security->bridges[i].to].name,
                security->bridges[i].criticality);
    }
    fprintf(fp, "\nArticulation Points\n");
    for (int i = 0; i < g->node_count; i++) {
        if (security->articulation[i]) fprintf(fp, "- %s impact=%d/100\n", g->nodes[i].name, security->articulation_impact[i]);
    }
    fprintf(fp, "\nNode Risk Scores\n");
    for (int i = 0; i < g->node_count; i++) {
        fprintf(fp, "- %-5s %-9s risk=%5.1f centrality=%4.2f firewall=%s\n",
                g->nodes[i].name, g->nodes[i].type, g->nodes[i].risk_score,
                security->centrality[i], g->nodes[i].firewall ? "yes" : "no");
    }
    fprintf(fp, "\nPacket Routing Log\n");
    for (int i = 0; i < log->count; i++) fprintf(fp, "- %s\n", log->lines[i]);
    fprintf(fp, "\nAction Plan\n");
    fprintf(fp, "1. Harden high-risk routers and sensitive servers.\n");
    fprintf(fp, "2. Add redundant links for bridge edges.\n");
    fprintf(fp, "3. Use min-cut recommendations for firewall placement.\n");
    fclose(fp);
    return 1;
}
