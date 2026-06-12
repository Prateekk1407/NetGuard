#ifndef SECURITY_H
#define SECURITY_H

#include "graph.h"
#include "routing.h"

typedef struct {
    int from;
    int to;
    int criticality;
} BridgeInfo;

typedef struct {
    BridgeInfo bridges[MAX_NODES * 2];
    int bridge_count;
    int articulation[MAX_NODES];
    int articulation_impact[MAX_NODES];
    double centrality[MAX_NODES];
    int ddos_order[MAX_NODES];
    int ddos_count;
} SecurityReport;

void security_analyze(Graph *g, SecurityReport *report);
void security_print(const Graph *g, const SecurityReport *report);
double security_path_risk(const Graph *g, const PathResult *path);
void security_attack_paths(const Graph *g, const char *entry_name);
void security_ddos_simulation(const Graph *g, const char *start_name);
int security_resilience_score(const Graph *g, const SecurityReport *report);

#endif
