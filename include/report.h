#ifndef REPORT_H
#define REPORT_H

#include "graph.h"
#include "security.h"
#include "scheduler.h"

void report_terminal(const Graph *g, const SecurityReport *security, const RoutingLog *log);
int report_export(const Graph *g, const SecurityReport *security, const RoutingLog *log, const char *path);

#endif
