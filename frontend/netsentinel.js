/**
 * NetSentinel AI — Groq-powered network security analyst
 * Reads live topology state from app.js and provides context-aware answers.
 */

(function () {
  "use strict";

  /* ── Config ────────────────────────────────────────────────────── */
  const GROQ_ENDPOINT = "https://api.groq.com/openai/v1/chat/completions";
  const GROQ_MODEL    = "llama-3.3-70b-versatile";
  // API key is loaded from frontend/netsentinel.config.js (git-ignored).
  // Copy netsentinel.config.example.js → netsentinel.config.js and fill in your key.
  const GROQ_KEY = window.NETSENTINEL_API_KEY || "";
  const MAX_HISTORY   = 12; // message pairs kept for context

  /* ── DOM refs ───────────────────────────────────────────────────── */
  const fab         = document.getElementById("aiFab");
  const panel       = document.getElementById("aiPanel");
  const closeBtn    = document.getElementById("aiCloseBtn");
  const clearBtn    = document.getElementById("aiClearBtn");
  const messages    = document.getElementById("aiMessages");
  const input       = document.getElementById("aiInput");
  const sendBtn     = document.getElementById("aiSendBtn");
  const statusEl    = document.getElementById("aiStatus");
  const ctxStrip    = document.getElementById("aiContextStrip");
  const suggestions = document.getElementById("aiSuggestions");

  /* ── State ──────────────────────────────────────────────────────── */
  let isOpen      = false;
  let isStreaming = false;
  let history     = [];   // [{role, content}] sent to API

  /* ── Panel toggle ────────────────────────────────────────────────── */
  function openPanel() {
    isOpen = true;
    panel.classList.add("open");
    panel.setAttribute("aria-hidden", "false");
    fab.setAttribute("aria-expanded", "true");
    updateContextStrip();
    input.focus();
    scrollBottom();
  }

  function closePanel() {
    isOpen = false;
    panel.classList.remove("open");
    panel.setAttribute("aria-hidden", "true");
    fab.setAttribute("aria-expanded", "false");
  }

  fab.addEventListener("click", () => isOpen ? closePanel() : openPanel());
  closeBtn.addEventListener("click", closePanel);

  /* Close on Escape */
  document.addEventListener("keydown", e => {
    if (e.key === "Escape" && isOpen) closePanel();
  });

  /* ── Context strip ───────────────────────────────────────────────── */
  function updateContextStrip() {
    const s = window.state;
    if (!s?.graph) {
      ctxStrip.textContent = "No topology loaded — using general network knowledge.";
      return;
    }
    const g   = s.graph;
    const sec = s.security;
    const activeNodes = g.nodes.filter(n => n.active).length;
    const activeLinks = g.edges.filter(e => e.active).length;
    const highRisk    = g.nodes.filter(n => n.risk >= 70).length;
    ctxStrip.textContent =
      `${activeNodes} nodes · ${activeLinks} links · resilience ${sec?.resilience ?? "?"}/100 · ${highRisk} high-risk`;
  }

  /* ── Build system prompt from live state ─────────────────────────── */
  function buildSystemPrompt() {
    const base =
      `You are NetSentinel, an expert AI network security analyst embedded in the NetGuard dashboard. ` +
      `You help students and engineers understand network topology, routing algorithms, security vulnerabilities, ` +
      `and optimisation strategies. Be concise, specific, and reference actual node names and numbers from the topology. ` +
      `Use markdown formatting: **bold** for node names, \`code\` for values, and bullet lists for multiple items. ` +
      `Keep answers focused and under 280 words unless a detailed explanation is explicitly requested.`;

    const s = window.state;
    if (!s?.graph) return base + "\n\nNo topology is currently loaded; answer based on general network security principles.";

    const g   = s.graph;
    const sec = s.security;

    /* Node summary */
    const nodeLines = g.nodes.map(n =>
      `  ${n.name} (${n.type}) sensitivity=${n.sensitivity} firewall=${n.firewall ? "yes" : "no"} ` +
      `risk=${n.risk.toFixed(1)} centrality=${(n.centrality ?? 0).toFixed(2)} active=${n.active ? "yes" : "no"}`
    ).join("\n");

    /* Edge summary (trim to 60 for prompt size) */
    const edgeLines = g.edges.slice(0, 60).map(e =>
      `  ${e.fromName} ${e.directed ? "->" : "<->"} ${e.toName} ` +
      `latency=${e.latency}ms bw=${e.bandwidth}Mbps active=${e.active ? "yes" : "no"}`
    ).join("\n");
    const edgeNote = g.edges.length > 60 ? `  ... (${g.edges.length - 60} more edges)` : "";

    /* Security summary */
    const bridges = sec?.bridges?.map(b =>
      `${g.nodes[b.from].name} <-> ${g.nodes[b.to].name} (criticality ${b.criticality}/100)`
    ).join(", ") || "none";

    const artPoints = g.nodes
      .filter((_, i) => sec?.articulation?.[i])
      .map((n, _i, _arr) => {
        const i = g.nodes.indexOf(n);
        return `${n.name} (impact ${sec.articulationImpact[i]}/100)`;
      }).join(", ") || "none";

    const topRisk = g.nodes.slice()
      .sort((a, b) => b.risk - a.risk)
      .slice(0, 5)
      .map(n => `${n.name}=${n.risk.toFixed(0)}`)
      .join(", ");

    const fwRecs = g.nodes
      .filter(n => !n.firewall && n.risk >= 55)
      .slice(0, 4)
      .map(n => n.name)
      .join(", ") || "none needed";

    return `${base}

## Current Topology (live data)
- **Nodes:** ${g.nodes.length} total, ${g.nodes.filter(n => n.active).length} active
- **Links:** ${g.edges.length} total, ${g.edges.filter(e => e.active).length} active
- **Resilience Score:** ${sec?.resilience ?? "?"}/100
- **Bridge links (SPOFs):** ${bridges}
- **Articulation points:** ${artPoints}
- **Top 5 risk nodes:** ${topRisk}
- **Firewall placement recommendations:** ${fwRecs}

### All Nodes
\`\`\`
${nodeLines}
\`\`\`

### Edges (sample)
\`\`\`
${edgeLines}
${edgeNote}
\`\`\`

Use this live data to answer the user's questions precisely.`;
  }

  /* ── Render helpers ──────────────────────────────────────────────── */
  function scrollBottom() {
    requestAnimationFrame(() => { messages.scrollTop = messages.scrollHeight; });
  }

  function appendMessage(role, htmlContent) {
    const row    = document.createElement("div");
    const avatar = document.createElement("span");
    const bubble = document.createElement("div");

    row.className    = `ai-msg ai-msg--${role === "user" ? "user" : "bot"}`;
    avatar.className = "ai-avatar";
    avatar.textContent = role === "user" ? "U" : "N";
    bubble.className = "ai-bubble";
    bubble.innerHTML = htmlContent;

    row.appendChild(avatar);
    row.appendChild(bubble);
    messages.appendChild(row);
    scrollBottom();
    return bubble;
  }

  function showTyping() {
    const row    = document.createElement("div");
    const avatar = document.createElement("span");
    const bubble = document.createElement("div");

    row.className    = "ai-msg ai-msg--bot ai-typing";
    avatar.className = "ai-avatar";
    avatar.textContent = "N";
    bubble.className = "ai-bubble";
    bubble.innerHTML = '<span class="ai-dot"></span><span class="ai-dot"></span><span class="ai-dot"></span>';

    row.appendChild(avatar);
    row.appendChild(bubble);
    messages.appendChild(row);
    scrollBottom();
    return row;
  }

  /* Convert minimal markdown → HTML for streaming output */
  function md2html(text) {
    return text
      .replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;")
      .replace(/\*\*(.+?)\*\*/g, "<strong>$1</strong>")
      .replace(/`([^`]+)`/g, "<code>$1</code>")
      .replace(/^### (.+)$/gm, "<strong>$1</strong>")
      .replace(/^## (.+)$/gm, "<strong>$1</strong>")
      .replace(/^- (.+)$/gm, "• $1")
      .replace(/\n/g, "<br>");
  }

  /* ── Main send flow ──────────────────────────────────────────────── */
  async function sendMessage(text) {
    text = text.trim();
    if (!text || isStreaming) return;

    /* Disable input */
    isStreaming = true;
    sendBtn.disabled = true;
    input.value = "";
    input.style.height = "42px";
    statusEl.textContent = "Thinking…";

    /* Hide suggestion chips after first real question */
    suggestions.style.display = "none";

    /* Add user message to UI */
    appendMessage("user", md2html(text));

    /* Add to history */
    history.push({ role: "user", content: text });
    if (history.length > MAX_HISTORY * 2) history.splice(0, 2);

    /* Show typing indicator */
    const typingEl = showTyping();

    try {
      const payload = {
        model: GROQ_MODEL,
        messages: [
          { role: "system", content: buildSystemPrompt() },
          ...history.slice(-MAX_HISTORY * 2)
        ],
        temperature: 0.55,
        max_tokens: 600,
        stream: true
      };

      const response = await fetch(GROQ_ENDPOINT, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          "Authorization": `Bearer ${GROQ_KEY}`
        },
        body: JSON.stringify(payload)
      });

      if (!response.ok) {
        const err = await response.json().catch(() => ({}));
        throw new Error(err?.error?.message || `HTTP ${response.status}`);
      }

      /* Remove typing indicator, create real bubble */
      typingEl.remove();
      const bubble = appendMessage("bot", "");
      bubble.classList.add("ai-streaming");

      /* Stream response */
      const reader  = response.body.getReader();
      const decoder = new TextDecoder();
      let   raw     = "";

      while (true) {
        const { done, value } = await reader.read();
        if (done) break;

        const chunk = decoder.decode(value, { stream: true });
        const lines = chunk.split("\n");

        for (const line of lines) {
          if (!line.startsWith("data: ")) continue;
          const data = line.slice(6).trim();
          if (data === "[DONE]") break;
          try {
            const json  = JSON.parse(data);
            const delta = json.choices?.[0]?.delta?.content ?? "";
            if (delta) {
              raw += delta;
              bubble.innerHTML = md2html(raw);
              scrollBottom();
            }
          } catch { /* skip malformed chunk */ }
        }
      }

      /* Done streaming */
      bubble.classList.remove("ai-streaming");
      history.push({ role: "assistant", content: raw });
      statusEl.textContent = "AI Network Analyst · Groq LLaMA 3.3";

    } catch (err) {
      typingEl.remove();
      appendMessage("bot",
        `<strong style="color:var(--danger)">Error</strong><br>${md2html(err.message)}<br>` +
        `<small style="color:var(--muted)">Check your API key or network connection.</small>`
      );
      statusEl.textContent = "Error — retry?";
    }

    isStreaming = false;
    sendBtn.disabled = input.value.trim() === "";
    input.focus();
  }

  /* ── Input handlers ─────────────────────────────────────────────── */
  input.addEventListener("input", () => {
    sendBtn.disabled = input.value.trim() === "" || isStreaming;
    /* Auto-resize */
    input.style.height = "42px";
    input.style.height = Math.min(input.scrollHeight, 120) + "px";
  });

  input.addEventListener("keydown", e => {
    if (e.key === "Enter" && !e.shiftKey) {
      e.preventDefault();
      if (!sendBtn.disabled) sendMessage(input.value);
    }
  });

  sendBtn.addEventListener("click", () => sendMessage(input.value));

  /* ── Suggestion chips ───────────────────────────────────────────── */
  document.querySelectorAll(".ai-chip").forEach(chip => {
    chip.addEventListener("click", () => {
      const q = chip.dataset.q;
      if (q) sendMessage(q);
    });
  });

  /* ── Clear chat ──────────────────────────────────────────────────── */
  clearBtn.addEventListener("click", () => {
    history = [];
    messages.innerHTML = "";
    suggestions.style.display = "flex";
    appendMessage("bot",
      "Chat cleared. I still have the current topology in my context. Ask me anything! 🛡️"
    );
  });

  /* ── Update context strip when topology changes ──────────────────── */
  /* Patch app.js's runFullAnalysis to notify us */
  const _origFull = window.runFullAnalysis;
  if (typeof _origFull === "function") {
    window.runFullAnalysis = function (...args) {
      const result = _origFull.apply(this, args);
      updateContextStrip();
      return result;
    };
  }

  /* Polling fallback — update strip every 3s if panel is open */
  setInterval(() => { if (isOpen) updateContextStrip(); }, 3000);

})();
