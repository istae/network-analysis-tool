#pragma once

void print_walk_vector(vector* v)
{
    int* n = vector_index(v, 0);
    int i;
    for(i=0; i < v->length-1; i++)
        printf("%s -> ", (char*)vector_index(&labels, n[i]));
    printf("%s\n", (char*)vector_index(&labels, n[i]));
}

void print_walk(node_walk* wlk)
{
  if (wlk != NULL) {
      char* label;
      while (wlk->prev != NULL) {
          label = vector_index(&labels, wlk->id);
          printf("%s -> ", label);
          wlk = wlk->prev;
      }
      label = vector_index(&labels, wlk->id);
      printf("%s\n", label);
  }
  else
      printf("Nodes aren't connected\n");
}

// descending
int node_sort_cmp(const void* a, const void* b)
{
    const vector* la = vector_index(&edge_list, *(int*)a);
    const vector* lb = vector_index(&edge_list, *(int*)b);
    return (lb->length) - (la->length);
}

void print_top(int cnt)
{
    if (cnt > total_nodes) {
        printf("paramater greater than total number of nodes\n");
        return;
    }
    int top(int*, int);
    int p[cnt];
    top(p, cnt);
    qsort(p, cnt, sizeof(int), node_sort_cmp);
    for (int i=0; i < cnt; i++) {
        vector* l = vector_index(&edge_list, p[i]);
        char* label = vector_index(&labels, p[i]);
        printf("%d: %s %d edges\n", i+1, label, l->length);
    }
}

void print_edge(int id)
{
    vector* l = vector_index(&edge_list, id);
    char* tlabel = vector_index(&labels, id);

    int* n = vector_index(l, 0);
    for (int j=0; j < l->length; j++) {
        char* nlabel = vector_index(&labels, n[j]);
        printf("%s - %s\n", tlabel, nlabel);
    }
}

void print_all()
{
    for (int i=0; i < HASTABLESIZE; i++) {
        for (node* t = table[i]; t != NULL; t = t->next) {
            print_edge(t->id);
            printf("--------------------------------------\n\n");
        }
    }
}

void print_hash_balance()
{
    for (int i=0; i < HASTABLESIZE; i++) {
        if (table[i] == NULL)
            continue;
        for (node* n=table[i]; n != NULL; n = n->next)
            printf("|");
        printf("\n");
    }
}

void print_sentence()
{
    for (int i=0; i < HASTABLESIZE; i++) {
        if (table[i] == NULL)
            continue;
        for (node* n=table[i]; n != NULL; n = n->next) {
            char* label = vector_index(&labels, n->id);
            printf("%s ", label);
        }
        printf("\n");
    }
}

void print_edgedensity()
{
    double edgedensity();
    printf("%.4lf\n", edgedensity());
}
