#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NODES 100
#define NAME_LEN 32
#define TYPE_LEN 24

typedef struct Edge {
    int to;
    int latency;
    int bandwidth;
    int active;
    struct Edge *next;
} Edge;

typedef struct Node {
    int id;
    char name[NAME_LEN];
    char type[TYPE_LEN];
    int sensitivity;
    int firewall;
    int active;
    double risk_score;
    Edge *edges;
} Node;

typedef struct Graph {
    Node nodes[MAX_NODES];
    int node_count;
} Graph;

void graph_init(Graph *g);
int graph_add_node(Graph *g, int id, const char *name, const char *type, int sensitivity, int firewall);
int graph_add_edge(Graph *g, int from, int to, int latency, int bandwidth, int undirected);
int graph_find_index(const Graph *g, const char *name);
int graph_load(Graph *g, const char *path);
void graph_print_summary(const Graph *g);
void graph_print_ascii(const Graph *g);
int graph_export_visual_html(const Graph *g, const char *path);
int graph_validate(const Graph *g);
int graph_remove_link(Graph *g, const char *a, const char *b);
int graph_restore_link(Graph *g, const char *a, const char *b);
int graph_set_node_active(Graph *g, const char *name, int active);
int graph_set_firewall(Graph *g, const char *name, int firewall);
int graph_edge_count(const Graph *g);
int graph_get_latency(const Graph *g, int from, int to);

#endif
