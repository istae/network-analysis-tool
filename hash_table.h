#pragma once

#include <vector.h>
#include "node.h"

#define HASTABLESIZE 997
#define MULTILPIER 97

// globals
node* table[HASTABLESIZE];
vector edge_list;
vector labels;
vector short_passes;
node* last = NULL;

// hash function return index on ^table
node* newnode(char* str)
{
    static int id = 0;
    node* n = malloc(sizeof(node));
    n->id = id++;
    n->next = NULL;
    vector list; vector_init(&list, sizeof(int), 1);
    vector_push(&edge_list, &list);
    vector_push(&labels, str);
    return n;
}

// given a node list (in or out), insert n in to the list on the condition that n is new in the list
void node_list_insert(int edge_index, int n_id)
{
    vector* l = vector_index(&edge_list, edge_index);
    int* n = l->begin;

    debug("%d: ", edge_index);
    for (int i=0; i < l->length; i++)
        debug("%d ", n[i]);
    debug("| %d ", n_id);

    // int pos = lower_bound(n, l->length, &n_id, sizeof(int));
    int pos = lower_bound_int(n, l->length, n_id);
    debug("pos(%d) --- ", pos);

    if (pos >= l->length) {
        vector_push(l, &n_id);
        ++total_edges;
    }
    else if (n[pos] != n_id) {
        vector_insert_int(l, n_id, pos);
        ++total_edges;
    }

    n = l->begin;
    for (int i=0; i < l->length; i++)
        debug("%d ", n[i]);
    debug("\n");
}

void remove_last()
{
    last = NULL;
}

int hash(char* str)
{
    unsigned int h = 0;
    unsigned char* p;
    for (p = (unsigned char*)str; *p != '\0'; p++)
        h = h * MULTILPIER + *p;
    return h % HASTABLESIZE;
}

int hash_find(char* nlabel)
{
    int i = hash(nlabel);
    for (node* n=table[i]; n != NULL; n = n->next) {
        char* label = vector_index(&labels, n->id);
        if (strcmp(label, nlabel) == 0)
            return n->id;
        }
    return -1;
}

// insert into the table
void hash_insert(char* str)
{
    const int i = hash(str);
    node* slot = table[i];

    // empty line
    if (slot == NULL) {
        ++total_nodes;
        node* n = newnode(str);
        if (last != NULL) {
            node_list_insert(last->id, n->id);
            node_list_insert(n->id, last->id);
        }
        last = n;
        table[i] = n;
        return;
    }

    // find in line our key
    node* prev = slot;
    while (slot != NULL) {
        char* label = vector_index(&labels, slot->id);
        if (strcmp(label, str) == 0) {
            if (last != NULL) {
                node_list_insert(last->id, slot->id);
                node_list_insert(slot->id, last->id);
            }
            last = slot;
            return;
        }
        prev = slot;
        slot = slot->next;
    }

    // end of line, not found
    ++total_nodes;
    node* n = newnode(str);
    if (last != NULL) {
        node_list_insert(last->id, n->id);
        node_list_insert(n->id, last->id);
    }
    prev->next = n;
    last = n;
}
