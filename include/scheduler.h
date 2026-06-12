#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "graph.h"
#include "security.h"

#define MAX_PACKETS 64
#define LOG_LEN 160

typedef enum {
    PRIORITY_LOW = 0,
    PRIORITY_HIGH = 1
} PacketPriority;

typedef struct {
    char id[16];
    char type[24];
    PacketPriority priority;
    int source;
    int destination;
    int arrival_order;
} Packet;

typedef struct {
    Packet packets[MAX_PACKETS];
    int size;
    int next_order;
} PacketQueue;

typedef struct {
    char lines[MAX_PACKETS][LOG_LEN];
    int count;
    int dispatched;
    int rerouted;
    int alerts;
    double total_latency;
} RoutingLog;

void scheduler_init(PacketQueue *q);
void scheduler_add(PacketQueue *q, const char *id, const char *type, PacketPriority priority, int source, int destination);
Packet scheduler_pop(PacketQueue *q);
int scheduler_empty(const PacketQueue *q);
void scheduler_demo(Graph *g, const SecurityReport *security, RoutingLog *log);
void scheduler_print_log(const RoutingLog *log);
void scheduler_log_init(RoutingLog *log);

#endif
