#ifndef FLOW_H
#define FLOW_H

#include "graph.h"

typedef struct {
    int max_flow;
    int cut_from[MAX_NODES * MAX_NODES];
    int cut_to[MAX_NODES * MAX_NODES];
    int cut_count;
} FlowResult;

FlowResult flow_max_and_cut(const Graph *g, int src, int dst);
void flow_print_analysis(const Graph *g, const char *src_name, const char *dst_name);
void flow_firewall_recommendations(const Graph *g);

#endif
