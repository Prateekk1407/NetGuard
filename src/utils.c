#include <stdio.h>
#include <string.h>
#include "utils.h"

static void heap_swap(HeapItem *a, HeapItem *b) {
    HeapItem t = *a;
    *a = *b;
    *b = t;
}

void heap_init(MinHeap *h) {
    h->size = 0;
}

void heap_push(MinHeap *h, int node, double priority) {
    if (h->size >= MAX_NODES * MAX_NODES) return;
    int i = h->size++;
    h->data[i].node = node;
    h->data[i].priority = priority;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[p].priority <= h->data[i].priority) break;
        heap_swap(&h->data[p], &h->data[i]);
        i = p;
    }
}

HeapItem heap_pop(MinHeap *h) {
    HeapItem out = {-1, 0};
    if (h->size == 0) return out;
    out = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (1) {
        int l = i * 2 + 1;
        int r = l + 1;
        int s = i;
        if (l < h->size && h->data[l].priority < h->data[s].priority) s = l;
        if (r < h->size && h->data[r].priority < h->data[s].priority) s = r;
        if (s == i) break;
        heap_swap(&h->data[i], &h->data[s]);
        i = s;
    }
    return out;
}

int heap_empty(const MinHeap *h) {
    return h->size == 0;
}

void queue_init(Queue *q, int capacity) {
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    q->capacity = capacity;
}

int queue_empty(const Queue *q) {
    return q->size == 0;
}

void queue_push(Queue *q, int value) {
    if (q->size >= q->capacity) return;
    q->data[q->rear] = value;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
}

int queue_pop(Queue *q) {
    if (queue_empty(q)) return -1;
    int value = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return value;
}

void path_to_string(const Graph *g, const int *path, int len, char *out, int out_size) {
    out[0] = '\0';
    for (int i = 0; i < len; i++) {
        strncat(out, g->nodes[path[i]].name, out_size - (int)strlen(out) - 1);
        if (i + 1 < len) strncat(out, "->", out_size - (int)strlen(out) - 1);
    }
}

void print_color(const char *color, const char *text) {
    printf("%s%s\033[0m", color, text);
}

const char *risk_label(double risk) {
    if (risk >= 80) return "CRITICAL";
    if (risk >= 60) return "HIGH";
    if (risk >= 35) return "MEDIUM";
    return "LOW";
}
