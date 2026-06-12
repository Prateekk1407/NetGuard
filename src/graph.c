#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "utils.h"

static int has_matching_reverse_edge(const Graph *g, int from, const Edge *edge);

void graph_init(Graph *g) {
    g->node_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->nodes[i].id = -1;
        g->nodes[i].edges = NULL;
        g->nodes[i].active = 1;
        g->nodes[i].risk_score = 0;
    }
}

int graph_find_index(const Graph *g, const char *name) {
    for (int i = 0; i < g->node_count; i++) {
        if (strcmp(g->nodes[i].name, name) == 0) return i;
    }
    return -1;
}

int graph_add_node(Graph *g, int id, const char *name, const char *type, int sensitivity, int firewall) {
    if (g->node_count >= MAX_NODES || graph_find_index(g, name) >= 0) return 0;
    Node *n = &g->nodes[g->node_count++];
    n->id = id;
    snprintf(n->name, NAME_LEN, "%s", name);
    snprintf(n->type, TYPE_LEN, "%s", type);
    n->sensitivity = sensitivity;
    n->firewall = firewall;
    n->active = 1;
    n->risk_score = 0;
    n->edges = NULL;
    return 1;
}

static void add_edge_oneway(Graph *g, int from, int to, int latency, int bandwidth) {
    Edge *e = malloc(sizeof(Edge));
    if (!e) return;
    e->to = to;
    e->latency = latency;
    e->bandwidth = bandwidth;
    e->active = 1;
    e->next = g->nodes[from].edges;
    g->nodes[from].edges = e;
}

int graph_add_edge(Graph *g, int from, int to, int latency, int bandwidth, int undirected) {
    if (from < 0 || to < 0 || from >= g->node_count || to >= g->node_count) return 0;
    if (latency <= 0 || bandwidth <= 0) return 0;
    add_edge_oneway(g, from, to, latency, bandwidth);
    if (undirected) add_edge_oneway(g, to, from, latency, bandwidth);
    return 1;
}

int graph_load(Graph *g, const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) return 0;
    char line[256], tag[32];
    int line_no = 0;
    while (fgets(line, sizeof(line), fp)) {
        line_no++;
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
        if (sscanf(line, "%31s", tag) != 1) continue;
        if (strcmp(tag, "NODE") == 0) {
            int id, sensitivity, firewall;
            char name[NAME_LEN], type[TYPE_LEN];
            if (sscanf(line, "NODE %d %31s %23s %d %d", &id, name, type, &sensitivity, &firewall) != 5) {
                printf("Invalid NODE at line %d\n", line_no);
                fclose(fp);
                return 0;
            }
            if (!graph_add_node(g, id, name, type, sensitivity, firewall)) {
                printf("Duplicate or invalid node at line %d\n", line_no);
                fclose(fp);
                return 0;
            }
        } else if (strcmp(tag, "EDGE") == 0) {
            char a[NAME_LEN], b[NAME_LEN], mode[8] = "U";
            int latency, bandwidth;
            if (sscanf(line, "EDGE %31s %31s %d %d %7s", a, b, &latency, &bandwidth, mode) < 4) {
                printf("Invalid EDGE at line %d\n", line_no);
                fclose(fp);
                return 0;
            }
            int from = graph_find_index(g, a);
            int to = graph_find_index(g, b);
            if (!graph_add_edge(g, from, to, latency, bandwidth, mode[0] != 'D')) {
                printf("Invalid edge endpoints or weights at line %d\n", line_no);
                fclose(fp);
                return 0;
            }
        }
    }
    fclose(fp);
    return graph_validate(g);
}

int graph_edge_count(const Graph *g) {
    int count = 0;
    for (int i = 0; i < g->node_count; i++) {
        for (Edge *e = g->nodes[i].edges; e; e = e->next) {
            if (i < e->to || !has_matching_reverse_edge(g, i, e)) count++;
        }
    }
    return count;
}

static const char *node_icon(const char *type) {
    if (strcmp(type, "router") == 0) return "R";
    if (strcmp(type, "server") == 0) return "S";
    if (strcmp(type, "database") == 0) return "D";
    if (strcmp(type, "admin") == 0) return "A";
    if (strcmp(type, "endpoint") == 0) return "E";
    if (strcmp(type, "firewall") == 0) return "F";
    return "N";
}

static const char *node_color(const Node *n) {
    if (!n->active) return "#8a8f98";
    if (n->risk_score >= 70.0) return "#d94f45";
    if (n->firewall) return "#2d9d78";
    if (strcmp(n->type, "database") == 0) return "#7860c7";
    if (strcmp(n->type, "server") == 0) return "#2777c6";
    if (strcmp(n->type, "admin") == 0) return "#c48621";
    if (strcmp(n->type, "endpoint") == 0) return "#69788a";
    if (strcmp(n->type, "firewall") == 0) return "#168f69";
    return "#455a72";
}

static void html_escape(FILE *fp, const char *text) {
    for (const char *p = text; *p; p++) {
        if (*p == '&') fputs("&amp;", fp);
        else if (*p == '<') fputs("&lt;", fp);
        else if (*p == '>') fputs("&gt;", fp);
        else if (*p == '"') fputs("&quot;", fp);
        else fputc(*p, fp);
    }
}

static void json_string(FILE *fp, const char *text) {
    fputc('"', fp);
    for (const char *p = text; *p; p++) {
        if (*p == '"' || *p == '\\') {
            fputc('\\', fp);
            fputc(*p, fp);
        } else if (*p == '\n') {
            fputs("\\n", fp);
        } else {
            fputc(*p, fp);
        }
    }
    fputc('"', fp);
}

static int has_matching_reverse_edge(const Graph *g, int from, const Edge *edge) {
    for (Edge *r = g->nodes[edge->to].edges; r; r = r->next) {
        if (r->to == from && r->latency == edge->latency && r->bandwidth == edge->bandwidth) return 1;
    }
    return 0;
}

int graph_export_visual_html(const Graph *g, const char *path) {
    FILE *fp = fopen(path, "w");
    if (!fp) return 0;

    fprintf(fp, "<!doctype html>\n<html lang=\"en\">\n<head>\n<meta charset=\"utf-8\">\n");
    fprintf(fp, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n");
    fprintf(fp, "<title>NetGuard Topology</title>\n<style>\n");
    fputs(":root{color-scheme:light;background:#f5f7fa;color:#17202c;font-family:Inter,Segoe UI,Arial,sans-serif}"
          "body{margin:0}header{padding:16px 24px;background:#17202c;color:white}"
          "h1{margin:0;font-size:22px;font-weight:700;letter-spacing:0}main{display:grid;grid-template-columns:minmax(0,1fr) 340px;min-height:calc(100vh - 58px)}"
          "#canvas{background:#f8fafc;width:100%;height:calc(100vh - 58px)}aside{background:white;border-left:1px solid #d8dee7;padding:16px;overflow:auto}"
          "h2{font-size:14px;margin:18px 0 8px}.stat{display:grid;grid-template-columns:1fr auto;gap:8px;padding:8px 0;border-bottom:1px solid #edf0f4}"
          ".legend{display:grid;grid-template-columns:18px 1fr;gap:8px;align-items:center;margin:7px 0}.swatch{width:14px;height:14px;border-radius:50%}"
          ".hint{font-size:12px;color:#697483;line-height:1.45}.nodeInfo{font-size:13px;line-height:1.5;padding:9px 0;border-bottom:1px solid #edf0f4}"
          ".zone{fill:#eef2f6;stroke:#d9e0ea;stroke-width:1}.zoneTitle{font-size:12px;font-weight:700;fill:#697483;text-anchor:middle}"
          ".link{stroke:#98a4b4;stroke-width:2;fill:none;opacity:.62}.link.wide{stroke-width:4}.link.down{stroke:#c64b42;stroke-dasharray:7 5}.link.dim{opacity:.12}.link.hot{stroke:#17202c;opacity:1;stroke-width:4}"
          ".node rect{rx:7;stroke:#182233;stroke-width:1.1}.node.dim{opacity:.25}.node text{pointer-events:none}.nodeName{font-size:13px;font-weight:800;fill:#17202c}.nodeMeta{font-size:11px;fill:#566273}.badgeText{font-size:11px;font-weight:800;fill:white;text-anchor:middle}"
          ".tooltip{position:fixed;display:none;background:#17202c;color:white;padding:8px 10px;border-radius:6px;font-size:12px;box-shadow:0 8px 24px #11182733;pointer-events:none;max-width:280px}"
          "@media(max-width:850px){main{grid-template-columns:1fr}aside{border-left:0;border-top:1px solid #d8dee7}#canvas{height:72vh}}\n", fp);
    fprintf(fp, "</style>\n</head>\n<body>\n<header><h1>NetGuard Topology Visual</h1></header>\n<main>\n<svg id=\"canvas\" role=\"img\" aria-label=\"Network topology visualization\"></svg>\n<aside>\n");
    fprintf(fp, "<div class=\"stat\"><span>Nodes</span><strong>%d</strong></div>\n", g->node_count);
    fprintf(fp, "<div class=\"stat\"><span>Links</span><strong>%d</strong></div>\n", graph_edge_count(g));
    fprintf(fp, "<h2>Legend</h2>\n");
    fprintf(fp, "<div class=\"legend\"><span class=\"swatch\" style=\"background:#d94f45\"></span><span>High risk</span></div>\n");
    fprintf(fp, "<div class=\"legend\"><span class=\"swatch\" style=\"background:#2d9d78\"></span><span>Firewall protected</span></div>\n");
    fprintf(fp, "<div class=\"legend\"><span class=\"swatch\" style=\"background:#8a8f98\"></span><span>Offline</span></div>\n");
    fprintf(fp, "<p class=\"hint\">Nodes are arranged in automatic distance layers from the most connected node. Hover a node to highlight its direct links; hover a link for latency and bandwidth.</p>\n<h2>Nodes</h2>\n");
    for (int i = 0; i < g->node_count; i++) {
        fprintf(fp, "<div class=\"nodeInfo\"><strong>");
        html_escape(fp, g->nodes[i].name);
        fprintf(fp, "</strong> ");
        html_escape(fp, g->nodes[i].type);
        fprintf(fp, "<br>risk %.1f, sensitivity %d, firewall %s%s</div>\n",
                g->nodes[i].risk_score,
                g->nodes[i].sensitivity,
                g->nodes[i].firewall ? "yes" : "no",
                g->nodes[i].active ? "" : ", offline");
    }
    fprintf(fp, "</aside>\n</main>\n<script>\nconst nodes=[\n");
    for (int i = 0; i < g->node_count; i++) {
        fprintf(fp, "{id:");
        json_string(fp, g->nodes[i].name);
        fprintf(fp, ",type:");
        json_string(fp, g->nodes[i].type);
        fprintf(fp, ",icon:");
        json_string(fp, node_icon(g->nodes[i].type));
        fprintf(fp, ",color:");
        json_string(fp, node_color(&g->nodes[i]));
        fprintf(fp, ",risk:%.1f,sensitivity:%d,firewall:%s,active:%s}%s\n",
                g->nodes[i].risk_score,
                g->nodes[i].sensitivity,
                g->nodes[i].firewall ? "true" : "false",
                g->nodes[i].active ? "true" : "false",
                i + 1 == g->node_count ? "" : ",");
    }
    fprintf(fp, "];\nconst links=[\n");
    int written = 0;
    for (int i = 0; i < g->node_count; i++) {
        for (Edge *e = g->nodes[i].edges; e; e = e->next) {
            if (i > e->to && has_matching_reverse_edge(g, i, e)) continue;
            if (written) fprintf(fp, ",\n");
            fprintf(fp, "{source:");
            json_string(fp, g->nodes[i].name);
            fprintf(fp, ",target:");
            json_string(fp, g->nodes[e->to].name);
            fprintf(fp, ",latency:%d,bandwidth:%d,active:%s}",
                    e->latency, e->bandwidth, e->active ? "true" : "false");
            written = 1;
        }
    }
    fprintf(fp, "\n];\n");
    fputs("const svg=document.getElementById('canvas');const ns='http://www.w3.org/2000/svg';const tooltip=document.createElement('div');tooltip.className='tooltip';document.body.appendChild(tooltip);"
          "const byId=new Map(nodes.map(n=>[n.id,n]));links.forEach(l=>{l.source=byId.get(l.source);l.target=byId.get(l.target)});"
          "const degree=new Map(nodes.map(n=>[n.id,0]));links.forEach(l=>{degree.set(l.source.id,degree.get(l.source.id)+1);degree.set(l.target.id,degree.get(l.target.id)+1)});"
          "const root=nodes.slice().sort((a,b)=>degree.get(b.id)-degree.get(a.id)||b.risk-a.risk||a.id.localeCompare(b.id))[0];"
          "const adj=new Map(nodes.map(n=>[n.id,[]]));links.forEach(l=>{adj.get(l.source.id).push(l.target);adj.get(l.target.id).push(l.source)});"
          "const q=[root];root.level=0;for(let p=0;p<q.length;p++){for(const next of adj.get(q[p].id)){if(next.level===undefined){next.level=q[p].level+1;q.push(next)}}}"
          "nodes.forEach(n=>{if(n.level===undefined)n.level=(root?root.level:0)+1});"
          "const layers=[];nodes.forEach(n=>{(layers[n.level]||(layers[n.level]=[])).push(n)});layers.forEach(a=>a.sort((x,y)=>x.type.localeCompare(y.type)||y.risk-x.risk||x.id.localeCompare(y.id)));"
          "function el(n,c){const x=document.createElementNS(ns,n);if(c)x.setAttribute('class',c);return x}"
          "function clear(){while(svg.firstChild)svg.removeChild(svg.firstChild)}"
          "function setAttrs(e,o){for(const k in o)e.setAttribute(k,o[k]);return e}"
          "function render(){clear();const w=Math.max(svg.clientWidth,720),h=Math.max(svg.clientHeight,520);svg.setAttribute('viewBox',`0 0 ${w} ${h}`);"
          "const marginX=70,cardW=118,cardH=58,gapX=Math.max(170,(w-marginX*2)/Math.max(layers.length-1,1));"
          "layers.forEach((layer,li)=>{const x=marginX+li*gapX;const zone=setAttrs(el('rect','zone'),{x:x-cardW/2-18,y:24,width:cardW+36,height:h-58,rx:8});svg.append(zone);const title=setAttrs(el('text','zoneTitle'),{x:x,y:46});title.textContent=li===0?'Core':`Layer ${li}`;svg.append(title);const usable=h-125;const step=usable/Math.max(layer.length,1);layer.forEach((n,i)=>{n.x=x;n.y=84+step*(i+.5)})});"
          "const linkG=el('g'),nodeG=el('g');svg.append(linkG,nodeG);"
          "const linkEls=links.map(l=>{const line=setAttrs(el('path','link '+(l.bandwidth>=100?'wide ':'')+(l.active?'':'down ')),{d:`M${l.source.x},${l.source.y} C${(l.source.x+l.target.x)/2},${l.source.y} ${(l.source.x+l.target.x)/2},${l.target.y} ${l.target.x},${l.target.y}`});line.addEventListener('mousemove',ev=>showTip(ev,`${l.source.id} to ${l.target.id}<br>${l.latency} ms latency<br>${l.bandwidth} Mbps bandwidth`));line.addEventListener('mouseleave',hideTip);linkG.append(line);return{l,line}});"
          "const nodeEls=nodes.map(n=>{const g=el('g','node');g.dataset.id=n.id;g.setAttribute('transform',`translate(${n.x-cardW/2},${n.y-cardH/2})`);const r=setAttrs(el('rect'),{width:cardW,height:cardH,fill:'#ffffff'});r.setAttribute('stroke',n.color);r.setAttribute('stroke-width',n.risk>=70?3:1.4);const badge=setAttrs(el('rect'),{x:8,y:8,width:24,height:24,rx:5,fill:n.color});const bt=setAttrs(el('text','badgeText'),{x:20,y:25});bt.textContent=n.icon;const name=setAttrs(el('text','nodeName'),{x:40,y:21});name.textContent=n.id;const meta=setAttrs(el('text','nodeMeta'),{x:40,y:39});meta.textContent=`${n.type} | risk ${Math.round(n.risk)}`;const flags=setAttrs(el('text','nodeMeta'),{x:10,y:52});flags.textContent=(n.firewall?'firewall':'no firewall')+(n.active?'':' | offline');g.append(r,badge,bt,name,meta,flags);g.addEventListener('mousemove',ev=>showTip(ev,`${n.id}<br>${n.type}<br>risk ${n.risk.toFixed(1)} / 100<br>sensitivity ${n.sensitivity}<br>${degree.get(n.id)} direct links`));g.addEventListener('mouseenter',()=>focusNode(n));g.addEventListener('mouseleave',()=>{clearFocus();hideTip()});nodeG.append(g);return{n,g}});"
          "function focusNode(n){const near=new Set([n.id]);links.forEach(l=>{if(l.source===n)near.add(l.target.id);if(l.target===n)near.add(l.source.id)});nodeEls.forEach(o=>o.g.classList.toggle('dim',!near.has(o.n.id)));linkEls.forEach(o=>{const hot=o.l.source===n||o.l.target===n;o.line.classList.toggle('hot',hot);o.line.classList.toggle('dim',!hot)})}"
          "function clearFocus(){nodeEls.forEach(o=>o.g.classList.remove('dim'));linkEls.forEach(o=>{o.line.classList.remove('hot');o.line.classList.remove('dim')})}"
          "function showTip(ev,html){tooltip.innerHTML=html;tooltip.style.display='block';tooltip.style.left=(ev.clientX+14)+'px';tooltip.style.top=(ev.clientY+14)+'px'}"
          "function hideTip(){tooltip.style.display='none'}}render();window.addEventListener('resize',render);\n", fp);
    fprintf(fp, "</script>\n</body>\n</html>\n");
    fclose(fp);
    return 1;
}

void graph_print_summary(const Graph *g) {
    printf("[+] Loaded topology: %d nodes, %d links\n", g->node_count, graph_edge_count(g));
}

void graph_print_ascii(const Graph *g) {
    printf("\n=== VISUAL NETWORK TOPOLOGY ===\n");
    if (graph_export_visual_html(g, "reports/topology_visual.html")) {
        printf("[+] Interactive topology visual exported to reports/topology_visual.html\n");
    } else if (graph_export_visual_html(g, "topology_visual.html")) {
        printf("[+] Interactive topology visual exported to topology_visual.html\n");
    } else {
        printf("[!] Could not export topology visual. Showing generated topology data below.\n");
    }
    printf("Legend: R router, S server, D database, A admin, E endpoint, F firewall, N node\n");
    printf("        ! = high risk (>=70)   x = offline   * = firewall enabled\n\n");

    printf("=== NODE STATUS ===\n");
    for (int i = 0; i < g->node_count; i++) {
        char mark = ' ';
        if (!g->nodes[i].active) mark = 'x';
        else if (g->nodes[i].risk_score >= 70.0) mark = '!';
        else if (g->nodes[i].firewall) mark = '*';
        printf("%c %-5s %-9s sensitivity=%d firewall=%-3s risk=%5.1f\n",
               mark, g->nodes[i].name, g->nodes[i].type, g->nodes[i].sensitivity,
               g->nodes[i].firewall ? "yes" : "no", g->nodes[i].risk_score);
    }

    printf("\n=== ACTIVE LINKS ===\n");
    for (int i = 0; i < g->node_count; i++) {
        for (Edge *e = g->nodes[i].edges; e; e = e->next) {
            if (i < e->to || !has_matching_reverse_edge(g, i, e)) {
                int bidirectional = has_matching_reverse_edge(g, i, e);
                const char *connector = e->active ? (bidirectional ? "<-->" : "-->") : "-x-";
                printf("%-5s %s %-5s  latency=%2dms  bandwidth=%3dMbps\n",
                       g->nodes[i].name, connector, g->nodes[e->to].name,
                       e->latency, e->bandwidth);
            }
        }
    }
}

int graph_validate(const Graph *g) {
    if (g->node_count == 0) return 0;
    int visited[MAX_NODES] = {0};
    Queue q;
    queue_init(&q, MAX_NODES);
    visited[0] = 1;
    queue_push(&q, 0);
    while (!queue_empty(&q)) {
        int u = queue_pop(&q);
        for (Edge *e = g->nodes[u].edges; e; e = e->next) {
            if (e->active && !visited[e->to]) {
                visited[e->to] = 1;
                queue_push(&q, e->to);
            }
        }
    }
    for (int i = 0; i < g->node_count; i++) {
        if (!visited[i]) {
            printf("Validation warning: graph is disconnected at %s\n", g->nodes[i].name);
            return 1;
        }
    }
    return 1;
}

static int set_link_state(Graph *g, const char *a, const char *b, int active) {
    int ia = graph_find_index(g, a);
    int ib = graph_find_index(g, b);
    int changed = 0;
    if (ia < 0 || ib < 0) return 0;
    for (Edge *e = g->nodes[ia].edges; e; e = e->next) if (e->to == ib) { e->active = active; changed = 1; }
    for (Edge *e = g->nodes[ib].edges; e; e = e->next) if (e->to == ia) { e->active = active; changed = 1; }
    return changed;
}

int graph_remove_link(Graph *g, const char *a, const char *b) {
    return set_link_state(g, a, b, 0);
}

int graph_restore_link(Graph *g, const char *a, const char *b) {
    return set_link_state(g, a, b, 1);
}

int graph_set_node_active(Graph *g, const char *name, int active) {
    int idx = graph_find_index(g, name);
    if (idx < 0) return 0;
    g->nodes[idx].active = active;
    return 1;
}

int graph_set_firewall(Graph *g, const char *name, int firewall) {
    int idx = graph_find_index(g, name);
    if (idx < 0) return 0;
    g->nodes[idx].firewall = firewall;
    return 1;
}

int graph_get_latency(const Graph *g, int from, int to) {
    for (Edge *e = g->nodes[from].edges; e; e = e->next) {
        if (e->to == to && e->active) return e->latency;
    }
    return INF;
}
