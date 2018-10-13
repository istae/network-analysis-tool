#pragma once

#define TSIZE 30

typedef struct node {
    int id;
    struct node* next;
} node;

typedef struct node_walk {
    unsigned int distance;
    unsigned int id;
    struct node_walk* prev;
} node_walk;

static int total_nodes = 0;
static int total_edges = 0;
