# NetGuard 🛡️

**Intelligent Network Routing & Security Analysis Engine**

NetGuard is a real-time, command-line network routing and security analysis engine built entirely in C. Unlike traditional tools that isolate packet routing from vulnerability scanning, NetGuard operates as a unified pipeline where every low-level network algorithm directly impacts packet paths and security metrics.

The engine introduces **Security-Aware Routing**, a bi-criteria optimization that dynamically routes high-priority packets (e.g., financial transactions, database queries) through paths that minimize both latency and structural security risk simultaneously.

---

## 🚀 Key Features

* **Security-Aware Routing:** Custom bi-criteria Dijkstra algorithm penalizes high-risk nodes, forcing sensitive data to take safer alternate routes.

* **Critical Infrastructure Detection:** Real-time auditing using Tarjan's Algorithm to identify articulation points and bridges that act as Single Points of Failure (SPoFs).

* **Traffic Heatmapping:** Computes Brandes' Betweenness Centrality to map global traffic flows and flag heavily utilized nodes as High-Value Targets for attackers.

* **Firewall Placement Optimizer:** Utilizes Edmonds-Karp Max-Flow and Min-Cut analysis to pinpoint the exact network bottlenecks where firewalls should be placed to block maximum lateral attack paths.

* **Interactive Simulation:** Live fault injection allowing you to simulate DDoS cascades (via BFS propagation), sever links, or take down routers to watch the engine recalculate routing and risk scores in real-time.

* **Color-Coded Terminal UI:** Rich ASCII terminal reporting with dynamic packet scheduler logs and a unified Network Resilience Score.

---

## 🧠 Algorithmic Core

NetGuard is built from scratch without external C++ STL libraries. The system relies on a custom Adjacency List (`AdjList`) and Min-Heap priority queues.

| Module | Algorithm | Time Complexity | Purpose |
| --- | --- | --- | --- |
| **Routing** | Standard Dijkstra | $O((V+E)\log V)$ | Fastest path routing for low-priority traffic. |
| **Routing** | Bi-Criteria Dijkstra | $O((V+E)\log V)$ | Evaluates edges using $Weight = Latency + (Risk \times 0.8)$. |
| **Routing** | Bellman-Ford | $O(VE)$ | Secondary routing for congested networks and negative cycle (loop) detection. |
| **Security** | Tarjan's DFS | $O(V+E)$ | Single-pass discovery of bridges and articulation points. |
| **Security** | Brandes' Centrality | $O(V(V+E)\log V)$ | Computes node traffic intermediation frequency. |
| **Flow** | Edmonds-Karp (BFS) | $O(VE^2)$ | Throughput capacity and Min-Cut firewall placement. |

---

## 🛠️ Installation & Build Instructions

NetGuard requires a standard Unix/Linux environment with GCC.

1. **Clone the repository:**
```bash
git clone https://github.com/Prateekk1407/NetGuard.git
cd NetGuard
```

2. **Compile the engine:**
NetGuard comes with a configured `Makefile`.

```bash
make
```

3. **Run the engine:**
Provide a topology configuration text file as an argument.

```bash
./netguard topology.txt
```

---

## 📂 File Structure

```text
NetGuard/
├── main.c           # Entry point, main menu, module orchestration
├── graph.c/h        # Graph builder, Adjacency List, file loader
├── routing.c/h      # Dijkstra (Standard + Bi-criteria) & Bellman-Ford
├── security.c/h     # Tarjan's, Risk Scorer, Centrality, DDoS cascade
├── flow.c/h         # Edmonds-Karp max flow, min-cut, firewall optimizer
├── scheduler.c/h    # Priority queue, security gate, packet dispatch
├── report.c/h       # Terminal report, ANSI colors, file export
├── utils.c/h        # Custom MinHeap, Queue, Stack implementations
├── topology.txt     # Sample network topology input file
├── Makefile         # GCC compilation rules
└── README.md        # Project documentation
```

*(Lines of code: ~2,500-3,000)* 

---

## 💻 Sample Usage

Upon loading `topology.txt`, NetGuard will print an ASCII map and immediately run a structural audit. When packets are dispatched, the terminal logs the routing decisions:

```text
=== INCOMING PACKETS ===
PKT_01 | EMAIL       | LOW
PKT_02 | BANK_TXN    | HIGH

=== ROUTING DECISIONS ===
PKT_01 [LOW]
 -> R1->R3->R7->R12 | 10ms [OK]

PKT_02 [HIGH]
 -> R1->R3->R7->R12 | Checking...
 [!] R7 risk score: 89/100 -> PATH FLAGGED
 [>] Secure path: R1->R3->R8->R10->R12 | 14ms
 [i] +4ms latency cost for security

Network Resilience Score: 61/100 [MODERATE RISK]
```

Full vulnerability reports are automatically saved to `vulnerability_report.txt` in the root directory.
