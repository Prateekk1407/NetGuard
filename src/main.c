#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "routing.h"
#include "security.h"
#include "flow.h"
#include "scheduler.h"
#include "report.h"

static void banner(void) {
    printf("====================================================\n");
    printf("        NetGuard v1.0 - Security Routing Engine     \n");
    printf("====================================================\n");
}

static void print_menu(void) {
    printf("\n=== MENU ===\n");
    printf("1. Show topology\n");
    printf("2. Compare routing algorithms\n");
    printf("3. Show security analysis\n");
    printf("4. Max-flow and min-cut analysis\n");
    printf("5. Run packet scheduler demo\n");
    printf("6. Simulate attack path\n");
    printf("7. Simulate DDoS cascade\n");
    printf("8. Fault injection / firewall update\n");
    printf("9. Export vulnerability report\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

static void recompute(Graph *g, SecurityReport *security) {
    printf("[+] Running security analysis...\n");
    security_analyze(g, security);
}

static void fault_menu(Graph *g, SecurityReport *security) {
    int choice;
    char a[NAME_LEN], b[NAME_LEN];
    printf("\n1. Take down link\n2. Restore link\n3. Take down node\n4. Restore node\n5. Add firewall\nChoice: ");
    if (scanf("%d", &choice) != 1) return;
    if (choice == 1 || choice == 2) {
        printf("Enter two node names: ");
        scanf("%31s %31s", a, b);
        int ok = choice == 1 ? graph_remove_link(g, a, b) : graph_restore_link(g, a, b);
        printf(ok ? "[+] Link state updated.\n" : "[!] Link not found.\n");
    } else if (choice == 3 || choice == 4 || choice == 5) {
        printf("Enter node name: ");
        scanf("%31s", a);
        int ok = 0;
        if (choice == 3) ok = graph_set_node_active(g, a, 0);
        if (choice == 4) ok = graph_set_node_active(g, a, 1);
        if (choice == 5) ok = graph_set_firewall(g, a, 1);
        printf(ok ? "[+] Node state updated.\n" : "[!] Node not found.\n");
    }
    recompute(g, security);
    graph_print_ascii(g);
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

static void auto_demo(Graph *g, SecurityReport *security, RoutingLog *log) {
    int route_src = preferred_node(g, "R1", "router", 0);
    int route_dst = preferred_node(g, "R12", "endpoint", g->node_count - 1);
    int flow_dst = preferred_node(g, "DB1", "database", route_dst);
    int attack_src = preferred_node(g, "WEB1", "server", route_src);
    int ddos_src = preferred_node(g, "R7", "router", route_dst);

    graph_print_summary(g);
    recompute(g, security);
    graph_print_ascii(g);
    security_print(g, security);
    routing_compare(g, g->nodes[route_src].name, g->nodes[route_dst].name);
    flow_print_analysis(g, g->nodes[route_src].name, g->nodes[flow_dst].name);
    scheduler_demo(g, security, log);
    security_attack_paths(g, g->nodes[attack_src].name);
    security_ddos_simulation(g, g->nodes[ddos_src].name);
    report_terminal(g, security, log);
    if (report_export(g, security, log, "reports/vulnerability_report.txt")) {
        printf("\n[+] Full report saved to reports/vulnerability_report.txt\n");
    }
}

int main(int argc, char **argv) {
    const char *topology_path = argc > 1 ? argv[1] : "data/topology.txt";
    Graph graph;
    SecurityReport security;
    RoutingLog log;
    scheduler_log_init(&log);
    graph_init(&graph);
    banner();
    if (!graph_load(&graph, topology_path)) {
        printf("[!] Could not load topology file: %s\n", topology_path);
        return 1;
    }
    if (argc > 2 && strcmp(argv[2], "--demo") == 0) {
        auto_demo(&graph, &security, &log);
        return 0;
    }
    recompute(&graph, &security);
    graph_print_summary(&graph);
    int choice = -1;
    while (choice != 0) {
        print_menu();
        if (scanf("%d", &choice) != 1) break;
        if (choice == 1) graph_print_ascii(&graph);
        else if (choice == 2) {
            char s[NAME_LEN], d[NAME_LEN];
            printf("Source destination: ");
            scanf("%31s %31s", s, d);
            routing_compare(&graph, s, d);
        } else if (choice == 3) {
            security_print(&graph, &security);
            printf("Network Resilience Score: %d/100\n", security_resilience_score(&graph, &security));
        } else if (choice == 4) {
            char s[NAME_LEN], d[NAME_LEN];
            printf("Source destination: ");
            scanf("%31s %31s", s, d);
            flow_print_analysis(&graph, s, d);
        } else if (choice == 5) {
            scheduler_demo(&graph, &security, &log);
        } else if (choice == 6) {
            char s[NAME_LEN];
            printf("Compromised entry node: ");
            scanf("%31s", s);
            security_attack_paths(&graph, s);
        } else if (choice == 7) {
            char s[NAME_LEN];
            printf("DDoS target/start node: ");
            scanf("%31s", s);
            security_ddos_simulation(&graph, s);
        } else if (choice == 8) {
            fault_menu(&graph, &security);
        } else if (choice == 9) {
            if (report_export(&graph, &security, &log, "reports/vulnerability_report.txt")) printf("[+] Report exported to reports/vulnerability_report.txt.\n");
            else printf("[!] Report export failed.\n");
        }
    }
    return 0;
}
