#ifndef ROUTING_H
#define ROUTING_H

#include "graph.h"

typedef struct {
    int nodes[MAX_NODES];
    int length;
    int total_latency;
    double score;
    int reachable;
} PathResult;

PathResult dijkstra_route(const Graph *g, int src, int dst, double priority_factor);
PathResult bellman_ford_route(const Graph *g, int src, int dst);
void routing_compare(const Graph *g, const char *src_name, const char *dst_name);
void routing_print_path(const Graph *g, PathResult result, const char *label);

#endif
