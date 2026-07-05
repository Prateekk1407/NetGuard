# NetGuard 🛡️

**Intelligent Network Routing & Security Analysis Engine**

NetGuard is a real-time network routing and security analysis engine built entirely in C, with a companion interactive browser dashboard. Unlike traditional tools that isolate packet routing from vulnerability scanning, NetGuard operates as a unified pipeline where every low-level network algorithm directly impacts packet paths and security metrics.

The engine introduces **Security-Aware Routing** — a bi-criteria optimisation that dynamically routes high-priority packets (e.g., financial transactions, database queries) through paths that minimise both latency and structural security risk simultaneously.

---

## 🚀 Key Features

* **Security-Aware Routing:** Custom bi-criteria Dijkstra algorithm penalises high-risk nodes, forcing sensitive data to take safer alternate routes.

* **Critical Infrastructure Detection:** Real-time auditing using Tarjan's Algorithm to identify articulation points and bridges that act as Single Points of Failure (SPoFs).

* **Traffic Heatmapping:** Computes Brandes' Betweenness Centrality to map global traffic flows and flag heavily utilised nodes as High-Value Targets for attackers.

* **Firewall Placement Optimiser:** Utilises Edmonds-Karp Max-Flow and Min-Cut analysis to pinpoint the exact network bottlenecks where firewalls should be placed to block maximum lateral attack paths.

* **Interactive Simulation:** Live fault injection allowing you to simulate DDoS cascades (via BFS propagation), sever links, or take down routers to watch the engine recalculate routing and risk scores in real-time.

* **Color-Coded Terminal UI:** Rich ASCII terminal reporting with dynamic packet scheduler logs and a unified Network Resilience Score.

* **🤖 NetSentinel AI Assistant:** A floating AI chatbot powered by Groq's LLaMA 3.3-70B model, embedded directly in the dashboard. NetSentinel reads the live topology state and answers natural-language questions about routing decisions, risk scores, firewall strategy, and attack surfaces in real-time.

---

## 🧠 Algorithmic Core

NetGuard is built from scratch without external C++ STL libraries. The system relies on a custom Adjacency List (`AdjList`) and Min-Heap priority queues.

| Module | Algorithm | Time Complexity | Purpose |
| --- | --- | --- | --- |
| **Routing** | Standard Dijkstra | $O((V+E)\log V)$ | Fastest path routing for low-priority traffic. |
| **Routing** | Bi-Criteria Dijkstra | $O((V+E)\log V)$ | Evaluates edges using $Weight = Latency + (Risk \times 0.8)$. |
| **Routing** | Bellman-Ford | $O(VE)$ | Secondary routing for congested networks and negative cycle detection. |
| **Security** | Tarjan's DFS | $O(V+E)$ | Single-pass discovery of bridges and articulation points. |
| **Security** | Brandes' Centrality | $O(V(V+E)\log V)$ | Computes node traffic intermediation frequency. |
| **Flow** | Edmonds-Karp (BFS) | $O(VE^2)$ | Throughput capacity and Min-Cut firewall placement. |

---

## 🛠️ Installation & Build Instructions

NetGuard requires a standard Unix/Linux environment with GCC, or a Windows environment with MinGW/GCC.

1. **Clone the repository:**
```bash
git clone https://github.com/Prateekk1407/NetGuard.git
cd NetGuard
```

2. **Compile the engine:**
```bash
make
```

3. **Run the engine:**
```bash
./netguard data/topology.txt
```

4. **Run the auto-demo mode:**
```bash
./netguard data/topology.txt --demo
```

---

## 🌐 Interactive Frontend Dashboard

NetGuard ships with a full browser-based dashboard that reimplements all C-module algorithms in JavaScript for interactive exploration.

### Running the dashboard

```bash
# From the project root
cd frontend
python -m http.server 8080
```

Then open **http://localhost:8080** in your browser.

### Dashboard sections

| Section | Description |
| --- | --- |
| **Topology** | Upload a `.txt` file or use the built-in presets. Renders an interactive SVG network graph with hover-to-highlight. |
| **Routing** | Compare Dijkstra fastest, Bi-criteria secure, and Bellman-Ford paths between any two nodes. |
| **Security** | Lists all bridge links and articulation points with criticality scores. Shows a live risk heatmap. |
| **Flow** | Computes max-flow and min-cut between any source/sink pair with firewall placement recommendations. |
| **Simulation** | Runs the packet scheduler demo and traces a DDoS cascade from any starting node. |
| **Faults** | Take nodes and links online/offline and add firewalls interactively — all analysis updates live. |
| **Report** | Generates and downloads a full vulnerability report as a `.txt` file. |

---

## 🤖 NetSentinel AI Assistant

NetSentinel is an AI-powered network security analyst embedded in the dashboard. It is powered by **Groq's LLaMA 3.3-70B** model via the Groq API.

### How it works

1. Click the **glowing circular button** in the bottom-right corner of the dashboard.
2. The chat panel opens with a live **context strip** showing the current topology summary (nodes, links, resilience score, high-risk count).
3. Ask any question in natural language — NetSentinel has full knowledge of the loaded topology including all node names, risk scores, bridge links, articulation points, centrality values, and firewall coverage.
4. Responses are **streamed** token-by-token for a live, interactive feel.

### Example questions

```
Which nodes have the highest risk scores and why?
Where are the single points of failure in this network?
Which path should a bank transaction take from R1 to DB1?
Where should I add firewalls for the best protection?
Explain what the resilience score means.
What would happen to the network if R7 went offline?
```

### Quick suggestion chips

The panel includes one-click chips for the most common questions: **Top risks**, **Critical links**, **Firewall plan**, and **Resilience**.

---

## 📂 File Structure

```text
NetGuard/
├── src/
│   ├── main.c          # Entry point, main menu, module orchestration
│   ├── graph.c/h       # Graph builder, Adjacency List, file loader, HTML exporter
│   ├── routing.c/h     # Dijkstra (Standard + Bi-criteria) & Bellman-Ford
│   ├── security.c/h    # Tarjan's, Risk Scorer, Centrality, DDoS cascade
│   ├── flow.c/h        # Edmonds-Karp max flow, min-cut, firewall optimiser
│   ├── scheduler.c/h   # Priority queue, security gate, packet dispatch
│   ├── report.c/h      # Terminal report, ANSI colors, file export
│   └── utils.c/h       # Custom MinHeap, Queue, Stack implementations
├── include/            # Header files
├── frontend/
│   ├── index.html      # SPA dashboard shell
│   ├── styles.css      # Dark-mode design system + NetSentinel chat styles
│   ├── app.js          # JS reimplementation of all C algorithms
│   └── netsentinel.js  # NetSentinel AI chat engine (Groq API + streaming)
├── data/
│   ├── topology.txt           # 20-node sample network topology
│   └── enterprise_topology.txt # 46-node enterprise preset
├── reports/
│   └── topology_visual.html   # Auto-generated interactive SVG topology map
├── build/              # Compiled object files
├── Makefile            # GCC compilation rules (Windows + Unix)
├── netguard.exe        # Pre-built Windows binary
└── README.md
```

*(Lines of code: ~2,500–3,000 C + ~1,200 JS)*

---

## 💻 Sample Terminal Output

Upon loading `topology.txt`, NetGuard prints an ASCII map and immediately runs a structural audit:

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

Full vulnerability reports are automatically saved to `reports/vulnerability_report.txt`.

---

## 🛡️ Advanced Capabilities

### 1. Interactive Fault Injection
NetGuard allows administrators to interactively simulate network outages. You can sever links or completely take down nodes in real-time. The engine immediately recalculates the routing paths and adjusts the network resilience score to reflect the new topology state.

### 2. DDoS Cascade & Attack Path Simulation
Beyond standard routing, the engine can trace potential attack paths from a compromised entry node. Using BFS, it simulates how a DDoS attack might cascade through the network, identifying which routers and endpoints would be overwhelmed first.

### 3. Max-Flow & Min-Cut Analysis
Using the Edmonds-Karp algorithm, NetGuard calculates the maximum throughput between any source and destination. It also identifies the "Min-Cut" — the exact edges that, if severed, would disconnect the network — which is critical for optimal firewall placement.

### 4. NetSentinel AI (Groq LLaMA 3.3-70B)
The embedded AI assistant translates every algorithm result into human-readable insights. It maintains conversation history, streams responses in real-time, and always has access to the current topology state so its answers reflect the live analysis.

---

## 📈 Roadmap & Future Enhancements

- [x] **Interactive HTML Topology Visualiser** — auto-generated from the C engine.
- [x] **Browser Dashboard** — full JS reimplementation of all algorithms.
- [x] **NetSentinel AI Assistant** — Groq-powered contextual network analyst.
- [ ] **IPv6 Support:** Upgrade the underlying structures to handle extended IPv6 address spaces.
- [ ] **Distributed Consensus:** Implement raft-like consensus for decentralised route agreement.
- [ ] **BGP Simulation:** Add BGP route propagation mechanics for inter-domain routing analysis.

---

## 🏗️ Architecture & Data Structures

NetGuard leverages lightweight, highly optimised data structures:
* **Dynamic Adjacency List:** Nodes and edges track real-time network states including `latency`, `bandwidth`, `firewall` status, and computed `risk_score`.
* **Custom MinHeap:** Implements the priority queue backing Dijkstra's algorithm without any external libraries.
* **Circular Queue:** Used in BFS-based algorithms (DDoS cascade, graph validation, Edmonds-Karp).
* **Topology Visualiser:** Generates an interactive HTML representation of the network graph (`reports/topology_visual.html`) alongside the terminal UI.

## 📊 Security Metrics Explained

The engine evaluates network vulnerability through several lenses:
* **Articulation Impact:** Uses Tarjan's algorithm to calculate not just if a node is a SPoF, but measures its "impact score" (how much of the network gets partitioned if it goes down).
* **Bridge Criticality:** Evaluates the severity of a single severed link based on node sensitivity scores.
* **Betweenness Centrality:** Determines traffic bottlenecks. Nodes with high intermediation frequency become prime targets for attackers.
* **Resilience Score:** A composite metric (0–100) combining average risk, bridge count, and firewall coverage.

---

## ✍️ Author

**Prateek Khemka**
- GitHub: [@Prateekk1407](https://github.com/Prateekk1407)
