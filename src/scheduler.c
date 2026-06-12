#include <stdio.h>
#include <string.h>
#include "scheduler.h"
#include "routing.h"
#include "utils.h"

void scheduler_init(PacketQueue *q) {
    q->size = 0;
    q->next_order = 0;
}

void scheduler_log_init(RoutingLog *log) {
    memset(log, 0, sizeof(*log));
}

void scheduler_add(PacketQueue *q, const char *id, const char *type, PacketPriority priority, int source, int destination) {
    if (q->size >= MAX_PACKETS) return;
    Packet *p = &q->packets[q->size++];
    snprintf(p->id, sizeof(p->id), "%s", id);
    snprintf(p->type, sizeof(p->type), "%s", type);
    p->priority = priority;
    p->source = source;
    p->destination = destination;
    p->arrival_order = q->next_order++;
}

static int better_packet(const Packet *a, const Packet *b) {
    if (a->priority != b->priority) return a->priority > b->priority;
    return a->arrival_order < b->arrival_order;
}

Packet scheduler_pop(PacketQueue *q) {
    int best = 0;
    for (int i = 1; i < q->size; i++) if (better_packet(&q->packets[i], &q->packets[best])) best = i;
    Packet out = q->packets[best];
    q->packets[best] = q->packets[--q->size];
    return out;
}

int scheduler_empty(const PacketQueue *q) {
    return q->size == 0;
}

static void append_log(RoutingLog *log, const char *line) {
    if (log->count < MAX_PACKETS) snprintf(log->lines[log->count++], LOG_LEN, "%s", line);
}

static int first_node_of_type(const Graph *g, const char *type) {
    for (int i = 0; i < g->node_count; i++) {
        if (strcmp(g->nodes[i].type, type) == 0) return i;
    }
    return -1;
}

static int preferred_node(const Graph *g, const char *name, const char *type, int fallback) {
    int idx = graph_find_index(g, name);
    if (idx >= 0) return idx;
    if (type) {
        idx = first_node_of_type(g, type);
        if (idx >= 0) return idx;
    }
    return fallback >= 0 && fallback < g->node_count ? fallback : 0;
}

void scheduler_demo(Graph *g, const SecurityReport *security, RoutingLog *log) {
    (void)security;
    PacketQueue q;
    scheduler_init(&q);
    int r1 = preferred_node(g, "R1", "router", 0);
    int r12 = preferred_node(g, "R12", "endpoint", g->node_count - 1);
    int db = preferred_node(g, "DB1", "database", r12);
    int admin = preferred_node(g, "ADM1", "admin", r1);
    scheduler_add(&q, "PKT_01", "EMAIL", PRIORITY_LOW, r1, r12);
    scheduler_add(&q, "PKT_02", "BANK_TXN", PRIORITY_HIGH, r1, r12);
    scheduler_add(&q, "PKT_03", "DB_QUERY", PRIORITY_HIGH, r1, db);
    scheduler_add(&q, "PKT_04", "LOG_SYNC", PRIORITY_LOW, admin, db);
    printf("\n=== INCOMING PACKETS ===\n");
    printf("The scheduler sends HIGH priority packets first. Packets with the same priority\n");
    printf("keep their arrival order.\n\n");
    for (int i = 0; i < q.size; i++) {
        printf("%s\n", q.packets[i].id);
        printf("  Type: %s\n", q.packets[i].type);
        printf("  Priority: %s\n", q.packets[i].priority == PRIORITY_HIGH ? "HIGH - latency and safety matter most" : "LOW - normal handling");
        printf("  Route request: %s -> %s\n",
               g->nodes[q.packets[i].source].name, g->nodes[q.packets[i].destination].name);
    }
    printf("\n=== ROUTING DECISIONS ===\n");
    printf("For HIGH priority packets, NetGuard checks the fastest path. If that path is\n");
    printf("too risky, it tries a security-aware reroute.\n");
    while (!scheduler_empty(&q)) {
        Packet p = scheduler_pop(&q);
        PathResult fastest = dijkstra_route(g, p.source, p.destination, 0.0);
        PathResult chosen = fastest;
        double risk = security_path_risk(g, &fastest);
        char path_text[256] = "";
        char line[LOG_LEN];
        if (p.priority == PRIORITY_HIGH && risk > 70.0) {
            PathResult secure = dijkstra_route(g, p.source, p.destination, 0.8);
            double secure_risk = security_path_risk(g, &secure);
            if (secure.reachable && secure_risk < risk) {
                chosen = secure;
                log->rerouted++;
                printf("\n%s\n", p.id);
                printf("  Decision: fastest path was flagged at %.1f/100 risk, so a safer route was selected.\n", risk);
            } else {
                log->alerts++;
                printf("\n%s\n", p.id);
                printf("  Decision: ALERT - fastest path risk is %.1f/100 and no safer alternate path was found.\n", risk);
            }
        } else {
            printf("\n%s\n", p.id);
            printf("  Decision: fastest available path is acceptable for this packet.\n");
        }
        path_to_string(g, chosen.nodes, chosen.length, path_text, sizeof(path_text));
        printf("  Selected path: %s\n", chosen.reachable ? path_text : "none");
        printf("  Latency: %d ms\n", chosen.total_latency);
        printf("  Route exposure: %.1f/100\n", security_path_risk(g, &chosen));
        printf("  Status: %s\n", chosen.reachable ? "OK - packet can be dispatched" : "BLOCKED - no reachable route");
        snprintf(line, sizeof(line), "%s %-10s %-4s %s %dms risk=%.1f",
                 p.id, p.type, p.priority == PRIORITY_HIGH ? "HIGH" : "LOW",
                 path_text, chosen.total_latency, security_path_risk(g, &chosen));
        append_log(log, line);
        log->dispatched++;
        log->total_latency += chosen.total_latency;
    }
}

void scheduler_print_log(const RoutingLog *log) {
    printf("\n=== PACKET ROUTING LOG ===\n");
    for (int i = 0; i < log->count; i++) printf("%s\n", log->lines[i]);
    if (log->dispatched) {
        printf("Throughput metrics: dispatched=%d avg_latency=%.1fms rerouted=%d alerts=%d\n",
               log->dispatched, log->total_latency / log->dispatched, log->rerouted, log->alerts);
    }
}
