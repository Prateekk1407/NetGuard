#ifndef UTILS_H
#define UTILS_H

#include "graph.h"

#define INF 1000000000

typedef struct {
    int node;
    double priority;
} HeapItem;

typedef struct {
    HeapItem data[MAX_NODES * MAX_NODES];
    int size;
} MinHeap;

typedef struct {
    int data[MAX_NODES * MAX_NODES];
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

void heap_init(MinHeap *h);
void heap_push(MinHeap *h, int node, double priority);
HeapItem heap_pop(MinHeap *h);
int heap_empty(const MinHeap *h);

void queue_init(Queue *q, int capacity);
int queue_empty(const Queue *q);
void queue_push(Queue *q, int value);
int queue_pop(Queue *q);

void path_to_string(const Graph *g, const int *path, int len, char *out, int out_size);
void print_color(const char *color, const char *text);
const char *risk_label(double risk);

#endif
