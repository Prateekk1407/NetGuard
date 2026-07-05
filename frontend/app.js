const SAMPLE_TOPOLOGY = `# NetGuard sample topology
# Format:
# NODE <id> <name> <type> <sensitivity 1-5> <firewall 0/1>
# EDGE <from> <to> <latency_ms> <bandwidth_mbps> <U/D>
NODE 1 R1 router 2 0
NODE 2 R2 router 2 0
NODE 3 R3 router 3 0
NODE 4 R4 router 3 0
NODE 5 R5 router 2 1
NODE 6 R6 router 2 0
NODE 7 R7 router 4 0
NODE 8 R8 router 2 1
NODE 9 R9 router 3 0
NODE 10 R10 router 2 1
NODE 11 R11 router 2 0
NODE 12 R12 endpoint 3 0
NODE 13 WEB1 server 3 1
NODE 14 WEB2 server 2 0
NODE 15 DB1 database 5 1
NODE 16 DB2 database 5 0
NODE 17 ADM1 admin 5 1
NODE 18 EMP1 endpoint 1 0
NODE 19 EMP2 endpoint 1 0
NODE 20 FW1 firewall 4 1
EDGE R1 R2 2 80 U
EDGE R1 R3 3 90 U
EDGE R2 R4 4 70 U
EDGE R3 R4 2 50 U
EDGE R3 R7 3 60 U
EDGE R3 R8 4 80 U
EDGE R4 R5 2 70 U
EDGE R4 R9 5 40 U
EDGE R5 R6 2 80 U
EDGE R6 R10 4 60 U
EDGE R7 R12 4 55 U
EDGE R8 R10 3 90 U
EDGE R9 R11 3 50 U
EDGE R10 R12 4 90 U
EDGE R11 R12 3 70 U
EDGE R1 WEB1 2 100 U
EDGE R2 WEB2 3 100 U
EDGE R10 DB1 2 120 U
EDGE R11 DB2 4 80 U
EDGE R5 ADM1 3 70 U
EDGE R8 FW1 1 150 U
EDGE FW1 DB1 2 150 U
EDGE FW1 ADM1 2 120 U
EDGE R12 EMP1 2 60 U
EDGE R12 EMP2 2 60 U
EDGE WEB1 R7 6 45 U
EDGE WEB2 R9 4 50 U
EDGE R6 R7 5 50 U
EDGE R8 R9 6 45 U
EDGE R10 R11 3 70 U
EDGE R5 R8 4 60 U
EDGE R2 R6 6 55 U
EDGE R7 DB2 7 35 U
EDGE R9 DB2 2 75 U`;

const ENTERPRISE_TOPOLOGY = `# NetGuard extensive enterprise topology
NODE 1 CORE1 router 4 1
NODE 2 CORE2 router 4 1
NODE 3 EDGE1 router 3 1
NODE 4 EDGE2 router 3 1
NODE 5 DIST1 router 3 0
NODE 6 DIST2 router 3 0
NODE 7 DIST3 router 3 0
NODE 8 DIST4 router 3 0
NODE 9 FW_EDGE firewall 5 1
NODE 10 FW_DB firewall 5 1
NODE 11 FW_CLOUD firewall 4 1
NODE 12 IDS1 firewall 4 1
NODE 13 WAF1 firewall 4 1
NODE 14 WEB1 server 3 1
NODE 15 WEB2 server 3 1
NODE 16 API1 server 4 1
NODE 17 API2 server 4 1
NODE 18 AUTH1 server 5 1
NODE 19 CACHE1 server 2 0
NODE 20 CACHE2 server 2 0
NODE 21 MAIL1 server 4 1
NODE 22 DNS1 server 4 1
NODE 23 DB_MAIN database 5 1
NODE 24 DB_REPL database 5 1
NODE 25 DB_AUDIT database 5 1
NODE 26 FILE1 server 4 1
NODE 27 BACKUP1 server 5 1
NODE 28 VAULT1 database 5 1
NODE 29 CLOUD_GW router 4 1
NODE 30 SAAS1 server 3 0
NODE 31 CLOUD_DB database 5 1
NODE 32 VPN_HUB router 4 1
NODE 33 BRANCH1 router 2 0
NODE 34 BRANCH2 router 2 0
NODE 35 BRANCH3 router 2 0
NODE 36 ADMIN1 admin 5 1
NODE 37 ADMIN2 admin 5 1
NODE 38 SOC1 admin 5 1
NODE 39 HR1 endpoint 2 0
NODE 40 FIN1 endpoint 4 0
NODE 41 DEV1 endpoint 3 0
NODE 42 DEV2 endpoint 3 0
NODE 43 SALES1 endpoint 1 0
NODE 44 SALES2 endpoint 1 0
NODE 45 IOT1 endpoint 2 0
NODE 46 IOT2 endpoint 2 0
EDGE CORE1 CORE2 1 400 U
EDGE CORE1 EDGE1 2 300 U
EDGE CORE2 EDGE2 2 300 U
EDGE EDGE1 EDGE2 3 180 U
EDGE CORE1 DIST1 2 220 U
EDGE CORE1 DIST2 3 220 U
EDGE CORE2 DIST3 2 220 U
EDGE CORE2 DIST4 3 220 U
EDGE DIST1 DIST2 4 120 U
EDGE DIST3 DIST4 4 120 U
EDGE DIST2 DIST3 5 100 U
EDGE EDGE1 FW_EDGE 1 250 U
EDGE EDGE2 FW_EDGE 1 250 U
EDGE FW_EDGE WAF1 1 220 U
EDGE WAF1 WEB1 2 180 U
EDGE WAF1 WEB2 2 180 U
EDGE WEB1 API1 3 140 U
EDGE WEB2 API2 3 140 U
EDGE API1 API2 2 100 U
EDGE API1 AUTH1 2 120 U
EDGE API2 AUTH1 2 120 U
EDGE API1 CACHE1 1 160 U
EDGE API2 CACHE2 1 160 U
EDGE FW_EDGE DNS1 1 80 U
EDGE FW_EDGE MAIL1 2 90 U
EDGE IDS1 FW_EDGE 1 100 U
EDGE DIST2 FW_DB 2 200 U
EDGE DIST3 FW_DB 2 200 U
EDGE FW_DB DB_MAIN 2 180 U
EDGE FW_DB DB_REPL 3 150 U
EDGE FW_DB DB_AUDIT 4 120 U
EDGE DB_MAIN DB_REPL 2 160 U
EDGE DB_MAIN DB_AUDIT 5 90 U
EDGE DB_AUDIT VAULT1 4 70 U
EDGE DIST4 FILE1 3 110 U
EDGE FILE1 BACKUP1 8 80 U
EDGE BACKUP1 VAULT1 6 70 U
EDGE AUTH1 VAULT1 4 80 D
EDGE CORE2 FW_CLOUD 2 180 U
EDGE FW_CLOUD CLOUD_GW 2 160 U
EDGE CLOUD_GW SAAS1 18 80 U
EDGE CLOUD_GW CLOUD_DB 24 70 U
EDGE CLOUD_DB DB_REPL 28 60 D
EDGE CORE1 VPN_HUB 2 160 U
EDGE VPN_HUB BRANCH1 18 70 U
EDGE VPN_HUB BRANCH2 24 60 U
EDGE VPN_HUB BRANCH3 30 50 U
EDGE BRANCH1 BRANCH2 35 30 U
EDGE DIST1 ADMIN1 2 100 U
EDGE DIST1 SOC1 2 100 U
EDGE DIST2 ADMIN2 3 100 U
EDGE ADMIN1 VAULT1 5 60 D
EDGE ADMIN2 DB_MAIN 4 70 D
EDGE SOC1 IDS1 2 90 U
EDGE DIST1 HR1 2 90 U
EDGE DIST2 FIN1 2 90 U
EDGE DIST3 DEV1 2 90 U
EDGE DIST3 DEV2 2 90 U
EDGE DIST4 SALES1 2 80 U
EDGE DIST4 SALES2 2 80 U
EDGE DIST4 IOT1 3 30 U
EDGE DIST4 IOT2 3 30 U
EDGE BRANCH1 WEB1 22 45 U
EDGE BRANCH1 HR1 20 35 U
EDGE BRANCH2 FIN1 26 35 U
EDGE BRANCH2 SAAS1 32 25 U
EDGE BRANCH3 SALES1 34 25 U
EDGE BRANCH3 SALES2 34 25 U
EDGE DEV1 API1 6 60 U
EDGE DEV2 API2 6 60 U
EDGE FIN1 DB_AUDIT 9 40 D
EDGE HR1 MAIL1 5 50 U
EDGE IOT1 IDS1 8 25 U
EDGE IOT2 IDS1 8 25 U`;

const els = {
  topologyInput: document.querySelector("#topologyInput"),
  topologyFile: document.querySelector("#topologyFile"),
  fileStatus: document.querySelector("#fileStatus"),
  analyzeBtn: document.querySelector("#analyzeBtn"),
  networkSvg: document.querySelector("#networkSvg"),
  tooltip: document.querySelector("#tooltip"),
  routeSource: document.querySelector("#routeSource"),
  routeDestination: document.querySelector("#routeDestination"),
  flowSource: document.querySelector("#flowSource"),
  flowDestination: document.querySelector("#flowDestination"),
  attackSource: document.querySelector("#attackSource"),
  ddosSource: document.querySelector("#ddosSource"),
  faultNode: document.querySelector("#faultNode"),
  faultLink: document.querySelector("#faultLink"),
  routingResults: document.querySelector("#routingResults"),
  flowResults: document.querySelector("#flowResults"),
  securityResults: document.querySelector("#securityResults"),
  riskList: document.querySelector("#riskList"),
  attackResults: document.querySelector("#attackResults"),
  simulationResults: document.querySelector("#simulationResults"),
  faultResults: document.querySelector("#faultResults"),
  reportOutput: document.querySelector("#reportOutput"),
  visualTitle: document.querySelector("#visualTitle"),
  menuToggle: document.querySelector("#menuToggle"),
  navLinks: document.querySelector("#navLinks")
};

let state = { graph: null, security: null, report: "" };
window.state = state;   // exposed for NetSentinel AI
const INF = 1e9;

function nodeIcon(type) {
  return { router: "R", server: "S", database: "D", admin: "A", endpoint: "E", firewall: "F" }[type] || "N";
}

function nodeColor(node) {
  if (!node.active) return "#8A8F98";
  if (node.risk >= 70) return "#D94F45";
  if (node.firewall) return "#38B2AC";
  return { database: "#7860C7", server: "#2777C6", admin: "#C48621", endpoint: "#69788A", firewall: "#168F69" }[node.type] || "#455A72";
}

function parseTopology(text) {
  const nodes = [];
  const byName = new Map();
  const edges = [];
  const lines = text.split(/\r?\n/);
  lines.forEach((raw, lineNumber) => {
    const line = raw.trim();
    if (!line || line.startsWith("#")) return;
    const parts = line.split(/\s+/);
    if (parts[0] === "NODE") {
      if (parts.length < 6) throw new Error(`Invalid NODE at line ${lineNumber + 1}`);
      const node = {
        id: Number(parts[1]),
        name: parts[2],
        type: parts[3],
        sensitivity: Number(parts[4]),
        firewall: parts[5] === "1",
        active: true,
        risk: 0,
        centrality: 0
      };
      if (byName.has(node.name)) throw new Error(`Duplicate node ${node.name}`);
      byName.set(node.name, nodes.length);
      nodes.push(node);
    } else if (parts[0] === "EDGE") {
      if (parts.length < 5) throw new Error(`Invalid EDGE at line ${lineNumber + 1}`);
      edges.push({
        fromName: parts[1],
        toName: parts[2],
        latency: Number(parts[3]),
        bandwidth: Number(parts[4]),
        directed: (parts[5] || "U").toUpperCase().startsWith("D"),
        active: true
      });
    }
  });

  edges.forEach(edge => {
    edge.from = byName.get(edge.fromName);
    edge.to = byName.get(edge.toName);
    if (edge.from === undefined || edge.to === undefined) throw new Error(`Invalid edge ${edge.fromName} -> ${edge.toName}`);
    if (edge.latency <= 0 || edge.bandwidth <= 0) throw new Error(`Invalid edge weights ${edge.fromName} -> ${edge.toName}`);
  });

  return { nodes, edges, byName, adj: buildAdj(nodes, edges, true), directedAdj: buildAdj(nodes, edges, false) };
}

function buildAdj(nodes, edges, undirectedView) {
  const adj = Array.from({ length: nodes.length }, () => []);
  edges.forEach(edge => {
    if (!edge.active || !nodes[edge.from].active || !nodes[edge.to].active) return;
    adj[edge.from].push({ to: edge.to, latency: edge.latency, bandwidth: edge.bandwidth, edge });
    if (undirectedView || !edge.directed) {
      adj[edge.to].push({ to: edge.from, latency: edge.latency, bandwidth: edge.bandwidth, edge });
    }
  });
  return adj;
}

function dijkstra(graph, src, dst, riskFactor = 0) {
  if (!graph.nodes[src]?.active || !graph.nodes[dst]?.active) return { reachable: false, nodes: [], latency: 0, score: INF };
  const n = graph.nodes.length;
  const dist = Array(n).fill(INF);
  const prev = Array(n).fill(-1);
  const used = Array(n).fill(false);
  dist[src] = 0;
  for (let pass = 0; pass < n; pass++) {
    let u = -1;
    for (let i = 0; i < n; i++) if (!used[i] && (u < 0 || dist[i] < dist[u])) u = i;
    if (u < 0 || dist[u] >= INF) break;
    used[u] = true;
    if (u === dst) break;
    for (const edge of graph.directedAdj[u]) {
      const v = edge.to;
      const nd = dist[u] + edge.latency + graph.nodes[v].risk * riskFactor;
      if (nd < dist[v]) {
        dist[v] = nd;
        prev[v] = u;
      }
    }
  }
  return reconstructPath(graph, src, dst, prev, dist[dst]);
}

function bellmanFord(graph, src, dst) {
  if (!graph.nodes[src]?.active || !graph.nodes[dst]?.active) return { reachable: false, nodes: [], latency: 0, score: INF };
  const n = graph.nodes.length;
  const dist = Array(n).fill(INF);
  const prev = Array(n).fill(-1);
  dist[src] = 0;
  for (let pass = 0; pass < n - 1; pass++) {
    let changed = false;
    for (let u = 0; u < n; u++) {
      if (dist[u] >= INF) continue;
      for (const edge of graph.directedAdj[u]) {
        if (dist[u] + edge.latency < dist[edge.to]) {
          dist[edge.to] = dist[u] + edge.latency;
          prev[edge.to] = u;
          changed = true;
        }
      }
    }
    if (!changed) break;
  }
  return reconstructPath(graph, src, dst, prev, dist[dst]);
}

function reconstructPath(graph, src, dst, prev, score) {
  if (score >= INF) return { reachable: false, nodes: [], latency: 0, score };
  const path = [];
  for (let at = dst; at !== -1; at = prev[at]) path.push(at);
  path.reverse();
  if (path[0] !== src) return { reachable: false, nodes: [], latency: 0, score };
  let latency = 0;
  for (let i = 0; i + 1 < path.length; i++) latency += latencyBetween(graph, path[i], path[i + 1]);
  return { reachable: true, nodes: path, latency, score };
}

function latencyBetween(graph, from, to) {
  const edge = graph.directedAdj[from].find(item => item.to === to);
  return edge ? edge.latency : 0;
}

function pathText(graph, path) {
  return path.reachable ? path.nodes.map(i => graph.nodes[i].name).join(" -> ") : "No reachable path";
}

function analyzeSecurity(graph) {
  const n = graph.nodes.length;
  const disc = Array(n).fill(0);
  const low = Array(n).fill(0);
  const childCount = Array(n).fill(0);
  const articulation = Array(n).fill(false);
  const articulationImpact = Array(n).fill(0);
  const bridges = [];
  let timer = 0;

  function dfs(u, parent) {
    disc[u] = low[u] = ++timer;
    for (const edge of graph.adj[u]) {
      const v = edge.to;
      if (v === parent) continue;
      if (!disc[v]) {
        childCount[u]++;
        dfs(v, u);
        low[u] = Math.min(low[u], low[v]);
        if (parent !== -1 && low[v] >= disc[u]) articulation[u] = true;
        if (low[v] > disc[u]) {
          bridges.push({
            from: u,
            to: v,
            criticality: Math.min(100, 50 + graph.nodes[u].sensitivity * 6 + graph.nodes[v].sensitivity * 6)
          });
        }
      } else {
        low[u] = Math.min(low[u], disc[v]);
      }
    }
    if (parent === -1 && childCount[u] > 1) articulation[u] = true;
  }

  for (let i = 0; i < n; i++) if (!disc[i]) dfs(i, -1);

  const centrality = Array(n).fill(0);
  for (let s = 0; s < n; s++) {
    for (let t = 0; t < n; t++) {
      if (s === t) continue;
      const path = dijkstra(graph, s, t, 0);
      if (!path.reachable) continue;
      for (let i = 1; i + 1 < path.nodes.length; i++) centrality[path.nodes[i]] += 1;
    }
  }
  const maxCentrality = Math.max(1, ...centrality);
  centrality.forEach((value, index) => {
    graph.nodes[index].centrality = value / maxCentrality;
  });

  graph.nodes.forEach((node, i) => {
    let risk = node.sensitivity * 12 + graph.nodes[i].centrality * 45;
    if (articulation[i]) {
      risk += 22;
      articulationImpact[i] = Math.min(95, 25 + childCount[i] * 10);
    }
    if (!node.firewall) risk += 10;
    if (node.type === "database" || node.type === "admin") risk += 12;
    if (!node.active) risk = 100;
    node.risk = Math.min(100, risk);
  });

  const avgRisk = graph.nodes.reduce((sum, node) => sum + node.risk, 0) / Math.max(1, n);
  const firewallCount = graph.nodes.filter(node => node.firewall).length;
  const resilience = Math.max(0, Math.min(100, Math.round(100 - avgRisk * 0.55 - bridges.length * 4 + firewallCount * 2)));
  return { bridges, articulation, articulationImpact, resilience };
}

function maxFlowMinCut(graph, src, dst) {
  if (!graph.nodes[src]?.active || !graph.nodes[dst]?.active) return { flow: 0, cut: [] };
  const n = graph.nodes.length;
  const cap = Array.from({ length: n }, () => Array(n).fill(0));
  const original = Array.from({ length: n }, () => Array(n).fill(0));
  graph.edges.forEach(edge => {
    if (!edge.active || !graph.nodes[edge.from].active || !graph.nodes[edge.to].active) return;
    cap[edge.from][edge.to] += edge.bandwidth;
    original[edge.from][edge.to] += edge.bandwidth;
    if (!edge.directed) {
      cap[edge.to][edge.from] += edge.bandwidth;
      original[edge.to][edge.from] += edge.bandwidth;
    }
  });

  let flow = 0;
  const parent = Array(n).fill(-1);
  while (bfsResidual(cap, src, dst, parent)) {
    let aug = INF;
    for (let v = dst; v !== src; v = parent[v]) aug = Math.min(aug, cap[parent[v]][v]);
    for (let v = dst; v !== src; v = parent[v]) {
      cap[parent[v]][v] -= aug;
      cap[v][parent[v]] += aug;
    }
    flow += aug;
  }

  const reachable = Array(n).fill(false);
  const q = [src];
  reachable[src] = true;
  for (let head = 0; head < q.length; head++) {
    const u = q[head];
    for (let v = 0; v < n; v++) {
      if (!reachable[v] && cap[u][v] > 0) {
        reachable[v] = true;
        q.push(v);
      }
    }
  }
  const cut = [];
  for (let u = 0; u < n; u++) {
    for (let v = 0; v < n; v++) {
      if (reachable[u] && !reachable[v] && original[u][v] > 0) cut.push({ from: u, to: v });
    }
  }
  return { flow, cut };
}

function bfsResidual(cap, src, dst, parent) {
  parent.fill(-1);
  const seen = Array(cap.length).fill(false);
  const q = [src];
  seen[src] = true;
  for (let head = 0; head < q.length; head++) {
    const u = q[head];
    for (let v = 0; v < cap.length; v++) {
      if (!seen[v] && cap[u][v] > 0) {
        parent[v] = u;
        seen[v] = true;
        if (v === dst) return true;
        q.push(v);
      }
    }
  }
  return false;
}

function pathRisk(graph, path) {
  if (!path.reachable) return 100;
  return Math.max(...path.nodes.map(i => graph.nodes[i].risk));
}

function preferredNode(graph, name, type, fallback = 0) {
  if (graph.byName.has(name)) return graph.byName.get(name);
  const typed = graph.nodes.findIndex(node => node.type === type);
  return typed >= 0 ? typed : Math.min(fallback, graph.nodes.length - 1);
}

function renderSelects(graph) {
  const options = graph.nodes.map((node, i) => `<option value="${i}">${node.name} (${node.type})</option>`).join("");
  [els.routeSource, els.routeDestination, els.flowSource, els.flowDestination, els.attackSource, els.ddosSource, els.faultNode].forEach(select => { select.innerHTML = options; });
  renderLinkSelect(graph);
  els.routeSource.value = preferredNode(graph, "R1", "router");
  els.routeDestination.value = preferredNode(graph, "R12", "endpoint", graph.nodes.length - 1);
  els.flowSource.value = els.routeSource.value;
  els.flowDestination.value = preferredNode(graph, "DB1", "database", Number(els.routeDestination.value));
  els.attackSource.value = preferredNode(graph, "WEB1", "server", Number(els.routeSource.value));
  els.ddosSource.value = preferredNode(graph, "R7", "router", Number(els.routeSource.value));
}

function renderLinkSelect(graph) {
  els.faultLink.innerHTML = graph.edges.map((edge, i) => {
    const connector = edge.directed ? "->" : "<->";
    const status = edge.active ? "" : " [down]";
    return `<option value="${i}">${edge.fromName} ${connector} ${edge.toName}${status}</option>`;
  }).join("");
}

function renderGraph(graph) {
  const svg = els.networkSvg;
  svg.replaceChildren();
  const width = Math.max(svg.clientWidth || 900, 760);
  const height = Math.max(svg.clientHeight || 620, 540);
  svg.setAttribute("viewBox", `0 0 ${width} ${height}`);
  const ns = "http://www.w3.org/2000/svg";
  const make = (tag, cls) => {
    const node = document.createElementNS(ns, tag);
    if (cls) node.setAttribute("class", cls);
    return node;
  };
  const set = (node, attrs) => {
    Object.entries(attrs).forEach(([key, value]) => node.setAttribute(key, value));
    return node;
  };

  const degree = graph.nodes.map((_, i) => graph.adj[i].length);
  const root = degree.indexOf(Math.max(...degree));
  graph.nodes.forEach(node => { delete node.level; });
  graph.nodes[root].level = 0;
  const queue = [root];
  for (let head = 0; head < queue.length; head++) {
    const u = queue[head];
    graph.adj[u].forEach(edge => {
      if (graph.nodes[edge.to].level === undefined) {
        graph.nodes[edge.to].level = graph.nodes[u].level + 1;
        queue.push(edge.to);
      }
    });
  }
  graph.nodes.forEach(node => { if (node.level === undefined) node.level = 1; });
  const layers = [];
  graph.nodes.forEach((node, i) => {
    node.index = i;
    (layers[node.level] ||= []).push(node);
  });
  layers.forEach(layer => layer.sort((a, b) => a.type.localeCompare(b.type) || b.risk - a.risk || a.name.localeCompare(b.name)));

  const cardW = graph.nodes.length > 35 ? 108 : 124;
  const cardH = graph.nodes.length > 35 ? 52 : 58;
  const marginX = 70;
  const gapX = Math.max(cardW + 52, (width - marginX * 2) / Math.max(1, layers.length - 1));
  layers.forEach((layer, layerIndex) => {
    const x = marginX + gapX * layerIndex;
    svg.append(set(make("rect", "zone"), { x: x - cardW / 2 - 16, y: 28, width: cardW + 32, height: height - 58, rx: 18 }));
    const title = set(make("text", "zone-title"), { x: x - cardW / 2, y: 52 });
    title.textContent = layerIndex === 0 ? "Core" : `Layer ${layerIndex}`;
    svg.append(title);
    const usable = height - 130;
    const step = usable / Math.max(1, layer.length);
    layer.forEach((node, i) => {
      node.x = x;
      node.y = 88 + step * (i + 0.5);
    });
  });

  const edgeLayer = make("g");
  const nodeLayer = make("g");
  svg.append(edgeLayer, nodeLayer);
  const edgeEls = graph.edges.map(edge => {
    const from = graph.nodes[edge.from];
    const to = graph.nodes[edge.to];
    const mid = (from.x + to.x) / 2;
    const path = set(make("path", `edge ${edge.bandwidth >= 100 ? "wide" : ""} ${edge.directed ? "directed" : ""} ${edge.active ? "" : "down"}`), {
      d: `M${from.x},${from.y} C${mid},${from.y} ${mid},${to.y} ${to.x},${to.y}`
    });
    path.addEventListener("mousemove", event => showTip(event, `<strong>${from.name} to ${to.name}</strong><br>${edge.latency} ms latency<br>${edge.bandwidth} Mbps bandwidth<br>${edge.directed ? "Directed" : "Undirected"} link`));
    path.addEventListener("mouseleave", hideTip);
    edgeLayer.append(path);
    return { edge, path };
  });

  const nodeEls = graph.nodes.map(node => {
    const group = make("g", "node-card");
    group.setAttribute("transform", `translate(${node.x - cardW / 2}, ${node.y - cardH / 2})`);
    group.dataset.index = node.index;
    const shell = set(make("rect", "shell"), { width: cardW, height: cardH, rx: 16 });
    shell.setAttribute("stroke", nodeColor(node));
    shell.setAttribute("stroke-width", node.risk >= 70 ? 3 : 1.3);
    const badge = set(make("rect"), { x: 8, y: 8, width: 24, height: 24, rx: 8, fill: nodeColor(node) });
    const badgeText = set(make("text", "node-badge"), { x: 20, y: 25 });
    badgeText.textContent = nodeIcon(node.type);
    const name = set(make("text", "node-name"), { x: 39, y: 21 });
    name.textContent = node.name.length > 10 ? `${node.name.slice(0, 9)}...` : node.name;
    const meta = set(make("text", "node-meta"), { x: 39, y: 39 });
    meta.textContent = `${node.type} | ${Math.round(node.risk)}`;
    const flags = set(make("text", "node-meta"), { x: 10, y: cardH - 7 });
    flags.textContent = node.firewall ? "firewall" : "open";
    group.append(shell, badge, badgeText, name, meta, flags);
    group.addEventListener("mouseenter", () => focusNode(node.index, nodeEls, edgeEls));
    group.addEventListener("mousemove", event => showTip(event, `<strong>${node.name}</strong><br>${node.type}<br>risk ${node.risk.toFixed(1)} / 100<br>sensitivity ${node.sensitivity}<br>${degree[node.index]} direct links`));
    group.addEventListener("mouseleave", () => { clearFocus(nodeEls, edgeEls); hideTip(); });
    nodeLayer.append(group);
    return { node, group };
  });
}

function focusNode(index, nodeEls, edgeEls) {
  const nearby = new Set([index]);
  edgeEls.forEach(({ edge }) => {
    if (edge.from === index) nearby.add(edge.to);
    if (edge.to === index) nearby.add(edge.from);
  });
  nodeEls.forEach(({ node, group }) => group.classList.toggle("dim", !nearby.has(node.index)));
  edgeEls.forEach(({ edge, path }) => {
    const hot = edge.from === index || edge.to === index;
    path.classList.toggle("hot", hot);
    path.classList.toggle("dim", !hot);
  });
}

function clearFocus(nodeEls, edgeEls) {
  nodeEls.forEach(({ group }) => group.classList.remove("dim"));
  edgeEls.forEach(({ path }) => path.classList.remove("hot", "dim"));
}

function showTip(event, html) {
  els.tooltip.innerHTML = html;
  els.tooltip.style.display = "block";
  els.tooltip.style.left = `${event.clientX + 14}px`;
  els.tooltip.style.top = `${event.clientY + 14}px`;
}

function hideTip() {
  els.tooltip.style.display = "none";
}

function renderSecurity(graph, security) {
  document.querySelector("#metricNodes").textContent = graph.nodes.length;
  document.querySelector("#metricLinks").textContent = graph.edges.filter(edge => edge.active).length;
  document.querySelector("#metricResilience").textContent = security.resilience;
  document.querySelector("#metricCritical").textContent = graph.nodes.filter(node => node.risk >= 70).length;
  document.querySelector("#heroRisk").textContent = `${Math.max(...graph.nodes.map(node => node.risk)).toFixed(0)}/100`;

  els.securityResults.innerHTML = "";
  if (!security.bridges.length) {
    addResult(els.securityResults, "No bridge links found", "No single link currently splits the active network.");
  } else {
    security.bridges.slice(0, 8).forEach(bridge => {
      addResult(
        els.securityResults,
        "Bridge link",
        `${graph.nodes[bridge.from].name} <-> ${graph.nodes[bridge.to].name}`,
        `Failure can isolate traffic. Criticality ${bridge.criticality}/100.`
      );
    });
  }
  graph.nodes.forEach((node, i) => {
    if (security.articulation[i]) {
      addResult(
        els.securityResults,
        "Articulation node",
        node.name,
        `Single point of failure. Outage impact ${security.articulationImpact[i]}/100.`
      );
    }
  });

  els.riskList.innerHTML = "";
  graph.nodes.slice().sort((a, b) => b.risk - a.risk).slice(0, 12).forEach(node => {
    const row = document.createElement("div");
    const cls = riskClass(node.risk);
    const hue = riskHue(node.risk);
    row.className = `risk-row ${cls}`;
    row.style.setProperty("--risk-hue", hue);
    row.innerHTML = `<strong>${node.name}</strong><span class="risk-track"><span class="risk-fill" style="width:${node.risk}%"></span></span><span class="risk-score">${node.risk.toFixed(0)}</span><small>${riskReason(node, security)}</small>`;
    els.riskList.append(row);
  });
}

function compareRoutes() {
  const graph = state.graph;
  const src = Number(els.routeSource.value);
  const dst = Number(els.routeDestination.value);
  const fastest = dijkstra(graph, src, dst, 0);
  const secure = dijkstra(graph, src, dst, 0.8);
  const bellman = bellmanFord(graph, src, dst);
  els.routingResults.innerHTML = "";
  addPathResult(els.routingResults, "Dijkstra fastest", graph, fastest, `Lowest latency. Exposure ${pathRisk(graph, fastest).toFixed(1)}/100.`);
  addPathResult(els.routingResults, "Bi-criteria secure", graph, secure, `Trades speed for lower risk. Score ${secure.score.toFixed(1)}.`);
  addPathResult(els.routingResults, "Bellman-Ford", graph, bellman, "Baseline shortest-path check.");
  document.querySelector("#heroRoute").textContent = fastest.reachable ? `${fastest.latency}ms` : "--";
}

function computeFlow() {
  const graph = state.graph;
  const src = Number(els.flowSource.value);
  const dst = Number(els.flowDestination.value);
  const result = maxFlowMinCut(graph, src, dst);
  els.flowResults.innerHTML = "";
  addResult(
    els.flowResults,
    "Maximum throughput",
    `${result.flow} Mbps from ${graph.nodes[src].name} to ${graph.nodes[dst].name}`,
    "Total traffic the active paths can carry before the route set saturates."
  );
  if (result.cut.length) {
    result.cut.slice(0, 8).forEach(edge => {
      addResult(
        els.flowResults,
        "Min-cut control point",
        `${graph.nodes[edge.from].name} -> ${graph.nodes[edge.to].name}`,
        "Good place for firewalling, monitoring, or redundancy."
      );
    });
  } else {
    addResult(els.flowResults, "Min-cut control point", "None found", "Destination may already be unreachable or have zero capacity.");
  }
  document.querySelector("#heroFlow").textContent = `${result.flow} Mbps`;
}

function runAttackPaths() {
  const graph = state.graph;
  const src = Number(els.attackSource.value);
  els.attackResults.innerHTML = "";
  graph.nodes.forEach((node, i) => {
    const sensitive = node.sensitivity >= 4 || node.type === "database" || node.type === "admin" || node.type === "firewall";
    if (!sensitive || i === src) return;
    const path = dijkstra(graph, src, i, 0.25);
    addPathResult(els.attackResults, node.name, graph, path, `Attacker exposure ${pathRisk(graph, path).toFixed(1)}/100.`);
  });
}

function runSimulations() {
  const graph = state.graph;
  els.simulationResults.innerHTML = "";

  addResult(
    els.simulationResults,
    "Packet gate",
    "Priority queue processed demo packets.",
    "HIGH packets are checked for safer reroutes when the fastest path is risky."
  );
  schedulerDemo(graph).forEach(item => addResult(els.simulationResults, item.title, item.body, item.meaning));

  const ddosStart = Number(els.ddosSource.value);
  const cascade = ddosCascade(graph, ddosStart);
  addResult(
    els.simulationResults,
    "DDoS cascade",
    `Start node: ${graph.nodes[ddosStart]?.name || "unknown"}`,
    "Hop count means link distance from the starting node. Load drops as the wave spreads."
  );
  if (!cascade.length) {
    addResult(els.simulationResults, "DDoS not run", "The selected start node is offline or unavailable.", "Choose an active node and run simulations again.");
    return;
  }
  cascade.slice(0, 28).forEach(item => {
    addResult(
      els.simulationResults,
      `DDoS hop ${item.depth}`,
      `${graph.nodes[item.node].name} | overflow ${item.load}%`,
      item.load > 55 ? "Degraded: service quality likely drops." : "Stressed: pressure exists but service may continue."
    );
  });
}

function refreshAnalysis(message) {
  const graph = state.graph;
  graph.adj = buildAdj(graph.nodes, graph.edges, true);
  graph.directedAdj = buildAdj(graph.nodes, graph.edges, false);
  state.security = analyzeSecurity(graph);
  renderLinkSelect(graph);
  renderGraph(graph);
  renderSecurity(graph, state.security);
  compareRoutes();
  computeFlow();
  runAttackPaths();
  runSimulations();
  buildReport();
  els.visualTitle.textContent = `${graph.nodes.length} nodes / ${graph.edges.filter(edge => edge.active).length} active links`;
  if (message) addResult(els.faultResults, "Updated", message);
}

function setNodeState(active) {
  const graph = state.graph;
  const node = graph.nodes[Number(els.faultNode.value)];
  node.active = active;
  refreshAnalysis(`${node.name} is now ${active ? "online" : "offline"}.`);
}

function setLinkState(active) {
  const graph = state.graph;
  const edge = graph.edges[Number(els.faultLink.value)];
  edge.active = active;
  refreshAnalysis(`${edge.fromName} ${edge.directed ? "->" : "<->"} ${edge.toName} is now ${active ? "online" : "offline"}.`);
}

function addFirewall() {
  const graph = state.graph;
  const node = graph.nodes[Number(els.faultNode.value)];
  node.firewall = true;
  refreshAnalysis(`Firewall enabled at ${node.name}.`);
}

function schedulerDemo(graph) {
  const r1 = preferredNode(graph, "R1", "router");
  const r12 = preferredNode(graph, "R12", "endpoint", graph.nodes.length - 1);
  const db = preferredNode(graph, "DB1", "database", r12);
  const admin = preferredNode(graph, "ADM1", "admin", r1);
  const packets = [
    { id: "PKT_01", type: "EMAIL", priority: 1, source: r1, destination: r12 },
    { id: "PKT_02", type: "BANK_TXN", priority: 2, source: r1, destination: r12 },
    { id: "PKT_03", type: "DB_QUERY", priority: 2, source: r1, destination: db },
    { id: "PKT_04", type: "LOG_SYNC", priority: 1, source: admin, destination: db }
  ].sort((a, b) => b.priority - a.priority);
  return packets.map(packet => {
    let chosen = dijkstra(graph, packet.source, packet.destination, 0);
    const risk = pathRisk(graph, chosen);
    let decision = packet.priority === 2 ? "High priority" : "Low priority";
    let meaning = "Fastest path is acceptable for this packet.";
    if (packet.priority === 2 && risk > 70) {
      const secure = dijkstra(graph, packet.source, packet.destination, 0.8);
      if (secure.reachable && pathRisk(graph, secure) < risk) {
        chosen = secure;
        decision = "High priority rerouted";
        meaning = "Fastest path was risky, so the safer route won.";
      } else {
        decision = "High priority alert";
        meaning = "No safer alternate path was found.";
      }
    }
    return {
      title: `${packet.id} ${packet.type}`,
      body: `${decision}: ${pathText(graph, chosen)} | ${chosen.latency}ms | exposure ${pathRisk(graph, chosen).toFixed(1)}/100`,
      meaning
    };
  });
}

function ddosCascade(graph, start) {
  if (!graph.nodes[start]?.active) return [];
  const seen = Array(graph.nodes.length).fill(false);
  const depth = Array(graph.nodes.length).fill(0);
  const q = [start];
  const output = [];
  seen[start] = true;
  for (let head = 0; head < q.length; head++) {
    const u = q[head];
    const load = Math.max(15, 100 - depth[u] * 18);
    output.push({ node: u, depth: depth[u], load });
    for (const edge of graph.adj[u]) {
      if (!seen[edge.to]) {
        seen[edge.to] = true;
        depth[edge.to] = depth[u] + 1;
        q.push(edge.to);
      }
    }
  }
  return output;
}

function addResult(container, title, body, note = "") {
  const item = document.createElement("div");
  item.className = "result-item";
  item.innerHTML = `<strong>${title}</strong><span>${body}</span>${note ? `<small>${note}</small>` : ""}`;
  container.append(item);
}

function addPathResult(container, title, graph, path, meta) {
  addResult(
    container,
    title,
    `<span class="path">${pathText(graph, path)}</span>`,
    path.reachable ? `${path.latency}ms, ${Math.max(0, path.nodes.length - 1)} hops. ${meta}` : meta
  );
}

function riskReason(node, security) {
  const reasons = [];
  if (!node.firewall) reasons.push("open");
  if (node.sensitivity >= 4) reasons.push("sensitive");
  if (node.centrality >= 0.35) reasons.push("busy");
  const index = state.graph.nodes.indexOf(node);
  if (security.articulation[index]) reasons.push("cut node");
  return reasons.slice(0, 3).join(" + ") || "normal";
}

function riskClass(risk) {
  if (risk >= 80) return "critical";
  if (risk >= 60) return "high";
  if (risk >= 35) return "medium";
  return "low";
}

function riskHue(risk) {
  return Math.max(0, Math.round(150 - Math.min(100, Math.max(0, risk)) * 1.5));
}

function buildReport() {
  const graph = state.graph;
  const security = state.security;
  const critical = graph.nodes.filter(node => node.risk >= 70).sort((a, b) => b.risk - a.risk);
  const lines = [];
  lines.push("NetGuard Vulnerability Report");
  lines.push(`Generated: ${new Date().toLocaleString()}`);
  lines.push(`Network Summary: ${graph.nodes.length} nodes, ${graph.edges.filter(edge => edge.active).length} active links`);
  lines.push(`Resilience Score: ${security.resilience}/100`);
  lines.push("");
  lines.push("Critical Bridges");
  if (!security.bridges.length) lines.push("- None found");
  security.bridges.forEach(bridge => lines.push(`- ${graph.nodes[bridge.from].name} <-> ${graph.nodes[bridge.to].name} criticality=${bridge.criticality}/100`));
  lines.push("");
  lines.push("Articulation Points");
  graph.nodes.forEach((node, i) => {
    if (security.articulation[i]) lines.push(`- ${node.name} impact=${security.articulationImpact[i]}/100`);
  });
  lines.push("");
  lines.push("Highest Risk Nodes");
  critical.slice(0, 12).forEach(node => lines.push(`- ${node.name} ${node.type} risk=${node.risk.toFixed(1)} centrality=${node.centrality.toFixed(2)} firewall=${node.firewall ? "yes" : "no"}`));
  lines.push("");
  lines.push("Firewall Recommendations");
  const recs = graph.nodes.filter(node => !node.firewall && node.risk >= 55).slice(0, 5);
  if (!recs.length) lines.push("- Existing firewall coverage is adequate for current threshold.");
  recs.forEach((node, i) => lines.push(`${i + 1}. Place firewall at ${node.name} | estimated protection gain ${(node.risk * 0.55).toFixed(0)}%`));
  lines.push("");
  lines.push("Action Plan");
  lines.push("1. Harden high-risk routers, admin nodes, and databases.");
  lines.push("2. Add redundant links around bridges and articulation points.");
  lines.push("3. Use security-aware routing for high-priority packets.");
  lines.push("4. Review min-cut edges for firewall placement and segmentation.");
  state.report = lines.join("\n");
  els.reportOutput.textContent = state.report;
}

function runFullAnalysis() {
  try {
    const graph = parseTopology(els.topologyInput.value);
    const security = analyzeSecurity(graph);
    state = { graph, security, report: "" };
    window.state = state;   // keep NetSentinel reference in sync
    els.faultResults.innerHTML = "";
    renderSelects(graph);
    renderGraph(graph);
    renderSecurity(graph, security);
    compareRoutes();
    computeFlow();
    runAttackPaths();
    runSimulations();
    buildReport();
    els.visualTitle.textContent = `${graph.nodes.length} nodes / ${graph.edges.filter(edge => edge.active).length} active links`;
  } catch (error) {
    alert(error.message);
  }
}

function downloadReport() {
  const blob = new Blob([state.report], { type: "text/plain" });
  const url = URL.createObjectURL(blob);
  const a = document.createElement("a");
  a.href = url;
  a.download = "netguard_frontend_report.txt";
  a.click();
  URL.revokeObjectURL(url);
}

document.querySelectorAll("[data-preset]").forEach(button => {
  button.addEventListener("click", () => {
    document.querySelectorAll("[data-preset]").forEach(item => item.classList.remove("active"));
    button.classList.add("active");
    els.topologyInput.value = button.dataset.preset === "enterprise" ? ENTERPRISE_TOPOLOGY : SAMPLE_TOPOLOGY;
    els.fileStatus.textContent = "Using built-in preset";
    runFullAnalysis();
  });
});

els.topologyFile.addEventListener("change", async event => {
  const file = event.target.files[0];
  if (!file) return;
  els.topologyInput.value = await file.text();
  els.fileStatus.textContent = file.name;
  runFullAnalysis();
});
els.analyzeBtn.addEventListener("click", runFullAnalysis);
document.querySelector("#routeBtn").addEventListener("click", compareRoutes);
document.querySelector("#flowBtn").addEventListener("click", computeFlow);
document.querySelector("#attackBtn").addEventListener("click", runAttackPaths);
document.querySelector("#simulateBtn").addEventListener("click", runSimulations);
document.querySelector("#takeNodeBtn").addEventListener("click", () => setNodeState(false));
document.querySelector("#restoreNodeBtn").addEventListener("click", () => setNodeState(true));
document.querySelector("#takeLinkBtn").addEventListener("click", () => setLinkState(false));
document.querySelector("#restoreLinkBtn").addEventListener("click", () => setLinkState(true));
document.querySelector("#addFirewallBtn").addEventListener("click", addFirewall);
document.querySelector("#downloadReportBtn").addEventListener("click", downloadReport);
document.querySelector("#copyReportBtn").addEventListener("click", () => navigator.clipboard.writeText(state.report));
window.addEventListener("resize", () => state.graph && renderGraph(state.graph));
els.menuToggle.addEventListener("click", () => {
  const open = els.navLinks.classList.toggle("open");
  els.menuToggle.setAttribute("aria-expanded", String(open));
});
els.navLinks.addEventListener("click", () => els.navLinks.classList.remove("open"));

window.runFullAnalysis = runFullAnalysis;  // exposed for NetSentinel AI
els.topologyInput.value = SAMPLE_TOPOLOGY;
runFullAnalysis();
