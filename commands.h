#pragma once

#include <time.h>

int isneighbor(int x, int y)
{
    vector* l = vector_index(&edge_list, x);
    return binary_search_int(l->begin, l->length, y);
}

double edgedensity()
{
    int potentialConnections = (total_nodes * (total_nodes - 1)) / 2;
    return (double)total_edges / potentialConnections;
}

int find_pair(char* str, int len, int* n)
{
    debug("str len: %d\n", len);
    char n1label[15];
    char n2label[15];
    int i;
    for (i=0; i < len && str[i] != ' '; i++)
        n1label[i] = str[i];
    n1label[i] = '\0';
    strcpy(n2label, str+i+1);

    int n1 = hash_find(n1label);
    int n2 = hash_find(n2label);

    if (n1 == -1) {
        printf("Could not find node '%s'\n", n1label);
        return 0;
    }
    if (n2 == -1) {
        printf("Could not find node '%s'\n", n2label);
        return 0;
    }

    n[0] = n1;
    n[1] = n2;
    return 1;
}

int find_single(char* str, int* id)
{
    int n = hash_find(str);
    if (n == -1) {
        fprintf(stderr, "Could not find node '%s'\n", str);
        return 0;
    }
    *id = n;
    return 1;
}


int walk_distance(node_walk* wlk)
{
  int i=-1;
  while (wlk != NULL) {
    i++;
    wlk = wlk->prev;
  }
  return i;
}

// prints the cnt nodes with the highest edges
int top(int* p, int cnt)
{
    int t = 0;
    for (int i=0; i < total_nodes; i++) {

        // get cnt ids in p
        if (t < cnt)
            p[t++] = i;

        // if p is full
        else {
            vector* l = vector_index(&edge_list, i);
            int edges = l->length;
            l = vector_index(&edge_list, p[0]);
            int min = l->length;
            int minIndx = 0;

            // find the min in p
            for (int j=1; j < cnt; j++) {
                l = vector_index(&edge_list, p[j]);
                if (l->length < min) {
                    min = l->length;
                    minIndx = j;
                }
            }

            // if i.length < min, replace 
            l = vector_index(&edge_list, p[minIndx]);
            if (edges > l->length)
                p[minIndx] = i;
        }
    }
    return 1;
}

void isneighbor_print(char* str, int len)
{
    int n[2];
    if(find_pair(str, len, n) && isneighbor(n[0], n[1]))
        printf("yes\n");
    else
        printf("no\n");
}

// stack needs to be pre-allocated
vector walk_all(int start, int finish, node_walk* stack)
{
    // bit flags
    #define NEIGHBOR 2
    #define CHECKED 1
    #define NEIGHBOR_AND_CHECKED 3

    stack[0].id = start;
    stack[0].prev = NULL;
    stack[0].distance = 0;

    int b = 0;
    int t = 1;

    char check[total_nodes];
    memset(check, 0, sizeof check);
    check[start] = CHECKED;

    vector ret; vector_init(&ret, sizeof(vector), 1);

    vector* l = vector_index(&edge_list, finish);
    int* n = l->begin;
    for (int i=0; i < l->length; i++)
        check[n[i]] = NEIGHBOR;

    while (b < t) {
        node_walk* p = &stack[b++];
        vector* l = vector_index(&edge_list, p->id);

        int* n = l->begin;
        for (int i=0; i < l->length; i++) {
            if ((check[n[i]] & CHECKED) == 0 ) {
                check[n[i]] |= CHECKED;
                stack[t].id = n[i];
                stack[t].prev = p;
                stack[t].distance = p->distance + 1;
                ++t;
                if (n[i] == finish) {
                    goto found;
                }
            }
        }
    }

    // return empty vector if finish not found
    return ret;

    // return all paths to finish
    found:
        --t;
        const int min = stack[t].distance;

        vector tmp; vector_init(&tmp, sizeof(int), min+1);
        for (node_walk* wlk = &stack[t]; wlk != NULL; wlk = wlk->prev)
            vector_push(&tmp, &wlk->id);
        vector_push(&ret, &tmp);

        while (b < t) {
            if (check[stack[b].id] == NEIGHBOR_AND_CHECKED && stack[b].distance + 1 == min) {
                vector tmp; vector_init(&tmp, sizeof(int), min+1);
                vector_push(&tmp, &finish);
                for (node_walk* wlk = &stack[b]; wlk != NULL; wlk=wlk->prev)
                    vector_push(&tmp, &wlk->id);
                vector_push(&ret, &tmp);
            }
            ++b;
        }
    return ret;
}

// stack needs to be pre-allocated
node_walk* walk(int start, int finish, node_walk* stack)
{
    stack[0].id = start;
    stack[0].prev = NULL;
    stack[0].distance = 0;

    int b = 0;
    int t = 1;

    char check[total_nodes];
    memset(check, 0, sizeof check);
    check[start] = 1;

    while (b < t) {
        node_walk* p = &stack[b++];
        vector* l = vector_index(&edge_list, p->id);
        int* n = l->begin;
        for (int i=0; i < l->length; i++) {
            if (check[n[i]] == 0) {
                check[n[i]] = 1;
                stack[t].id = n[i];
                stack[t].prev = p;
                stack[t].distance = p->distance + 1;
                if (n[i] == finish) {
                    return &stack[t];
                }
                ++t;
            }
        }
    }
    return NULL;
}

void diameter()
{
    node_walk* stack = malloc(sizeof(node_walk) * (total_nodes+1));
    int max = 0;
    int pivot = 0;

    // select a pivot
    for (int i=1; i < total_nodes; i++) {
        node_walk* wlk = walk(0, i, stack);
        if (wlk && wlk->distance >= max) {
            max = wlk->distance;
            pivot = i;
            printf("%d: ", max);
            print_walk(wlk);
        }
    }

    // max walk to pivot
    for (int i=0; i < total_nodes; i++) {
        node_walk* wlk = walk(i, pivot, stack);
        if (wlk && wlk->distance >= max) {
            max = wlk->distance;
            printf("%d: ", max);
            print_walk(wlk);
        }
    }
    printf("diameter: %d\n", max);

    free(stack);
}

void short_path(char* str, const int len)
{
    int n[2];
    if (find_pair(str,len,n)) {
        node_walk* stack = malloc(sizeof(node_walk) * (total_nodes+1));
        node_walk* wlk = walk(n[0],n[1],stack);
        printf("%d: ", walk_distance(wlk));
        print_walk(wlk);
        free(stack);
    }
}

void short_path_all(char* str, const int len)
{
    int n[2];
    if (find_pair(str,len,n)) {
        node_walk* stack = malloc(sizeof(node_walk) * (total_nodes+1));
        vector rslt = walk_all(n[0], n[1], stack);
        for (int i=0; i < rslt.length; i++) {
            vector* vWlk = vector_index(&rslt, i);
            printf("%d: ", vWlk->length - 1);
            print_walk_vector(vWlk);
        }

        // free vectors
        for (int i=0; i < rslt.length; i++)
            vector_free(vector_index(&rslt, i));
        vector_free(&rslt);
        free(stack);
    }
}

void total(char* str)
{
    if (str[0] == 'n')
        printf("%d node(s)\n", total_nodes);
    else if (str[0] == 'e')
        printf("%d edge(s)\n", total_edges);
    else
        printf("Unknown paramater\n");
    return;
}

void betweeness(char* str)
{
    int id;
    if(!find_single(str, &id))
        return;

    if (short_passes.length == 0) {
        double next = .005;
        printf("calculating all shortest paths...\n");
        node_walk* stack = malloc(sizeof(node_walk) * (total_nodes+1));
        if (stack == NULL) {
            fprintf(stderr, "betweenes malloc bad\n");
            exit(1);
        }
        for(int i=0; i < total_nodes; i++) {
            for (int j=i+1; j < total_nodes; j++) {
                vector shell = walk_all(i, j, stack);
                vector* p = shell.begin;
                for (int k=0; k < shell.length; k++)
                    vector_push(&short_passes, &p[k]);
                vector_free(&shell);
            }
            if ((double)i / total_nodes > next) {
                printf("\r[%d%%] %d of %d", (int)(next*100), i, total_nodes);
                fflush(stdout);
                next += 0.01;
            }
        }
        free(stack);
        printf("\ndone\ntotal shortest paths: %d\n", short_passes.length);
    }

    int passes = 0;
    vector* path = short_passes.begin;
    for(int i=0; i < short_passes.length; i++) {
        int* n = path[i].begin;
        for (int k=1; k < path[i].length-1; k++) {
            if (n[k] == id)
                ++passes;
        }
    }
    printf("'%s' is in %d shortest paths\n", (char*)vector_index(&labels, id), passes);
    printf("betweenness score: %.3lf\n", (double)passes / short_passes.length);
}

void close_cent(char* str)
{
    int id;
    if(!find_single(str, &id))
        return;

    int passCount = 0;
    node_walk* stack = malloc(sizeof(node_walk) * (total_nodes+1));
    for(int i=0; i < total_nodes; i++) {
        node_walk* wlk = walk(id, i, stack);
        passCount += walk_distance(wlk);
    }
    // walk_distance return -1 for itself
    free(stack);
    passCount++;
    printf("total distance to all nodes: %d\n", passCount);
    printf("closeness centrality: %.3lf\n", (double)(total_nodes-1)/passCount);

}

int isin(const int* a, const int len, const int m)
{
    for (int i=0; i < len; i++)
        if (a[i] == m)
            return 1;
    return 0;
}

void twocommunity()
{
    vector A; vector_init(&A, sizeof(int), 1);
    vector B; vector_init(&B, sizeof(int), 1);

    // A and B are initilized with A and B containing half of the nodes
    int i;
    for (i=0; i < total_nodes/2; i++)
        vector_push(&A, &i);
    for (; i < total_nodes; i++)
        vector_push(&B, &i);

    // main running loop
    const int stopA = A.length;
    const int stopB = B.length;

    int j,k;

    for(j=k=0; j < stopA || k < stopB; j++, k++) {

        if (j < stopA) {
            int score = 0;
            int* community = vector_index(&A, j);
            int member = *community;
            vector* neighbors = vector_index(&edge_list, member);
            int* n = neighbors->begin;
            for (int l=0; l < neighbors->length; l++) {
                if (isin(community, A.length-j, n[l]))
                    ++score;
                else
                    --score;
            }
            if (score >= 0)
                vector_push(&A, &member);
            else if (score < 0)
                vector_push(&B, community);
            else {
                vector_push(&A, &member);
                vector_push(&B, &member);
            }
        }

        if (k < stopB) {
            int score = 0;
            int* community = vector_index(&B, k);
            int member = *community;
            vector* neighbors = vector_index(&edge_list, member);
            int* n = neighbors->begin;
            for (int l=0; l < neighbors->length; l++) {
                if (isin(community, B.length-k, n[l]))
                    ++score;
                else
                    --score;
            }
            if (score >= 0)
                vector_push(&B, &member);
            else if (score < 0)
                vector_push(&A, &member);
            else {
                vector_push(&B, &member);
                vector_push(&A, &member);
            }
        }
    }

    printf("Community A: size %d\n", A.length - stopA);
    int* nA = vector_index(&A, stopA);
    for (int i=0; i < A.length - stopA; i++)
        printf("%s\n", (char*)vector_index(&labels, nA[i]));

    printf("Community B: size %d\n", B.length - stopB);
    int* nB = vector_index(&B, stopB);
    for (int i=0; i < B.length - stopB; i++)
        printf("%s\n", (char*)vector_index(&labels, nB[i]));

    vector_free(&A);
    vector_free(&B);
}

void neighbors(char* str)
{
    int id;
    if(find_single(str, &id))
        print_edge(id);
}

void count(char* str)
{
    int id;
    if(find_single(str, &id)) {
        vector* l = vector_index(&edge_list, id);
        printf("%d edges\n", l->length);
    }
}

#define INT_PARAMS 0
#define PARAMS_NEEDED 8
#define MAXCMDLEN 45

void cmd(char* str, int len)
{
    //cs indexes < 2 accept only digits
    static const char cmds[][MAXCMDLEN] = {
        "top=",
        "total=",
        "shortest path=",
        "shortest path all=",
        "neighbors=",
        "count=",
        "is neighbor=",
        "betweenness=",
        "close cent=",
        "diameter",
        "edge density",
        "print all",
        "commands",
        "clear",
        "hash balance",
        "exit",
    };

    static const char cmds_str[][MAXCMDLEN] = {
        "top=(number)",
        "total=('e' or 'n')",
        "shortest path=(label, label)",
        "shortest path all=(label, label)",
        "neighbors=(label)",
        "count=(label)",
        "is neighbor=(label, label)",
        "betweenness=(label)",
        "close cent=(label)",
        "diameter",
        "edge density",
        "print all",
        "commands",
        "clear",
        "hash balance",
        "exit",
    };

    static const int cmdslen = sizeof(cmds) / MAXCMDLEN;

    int cmdslens[cmdslen];
    for (int i=0; i < cmdslen; i++)
      cmdslens[i] = strlen(cmds[i]);

    int match[cmdslen];
    for (int i=0; i < cmdslen; i++)
        match[i] = 1;

    int c = -1;
    int int_param = -1;
    char char_param[TSIZE];
    int last = -1;

    for (int i=0; i < len; i++) {
        for (int k=0; k < cmdslen; k++) {
            if (match[k]) {
                last = k;
                if (i == cmdslens[k]-1) {
                    c = k;
                }
                else if (str[i] != cmds[k][i])
                   match[k] = 0;
            }
        }
    }

    if (c == -1) {
        printf("Unknown command\nDid you mean %s?\n", cmds[last]);
        return;
    }

    if (c <= PARAMS_NEEDED)
      if (len == cmdslens[c]) {
          printf("paramater not defined\n");
          return;
      }

    // cs that need number paramater
    if (c <= INT_PARAMS) {
        char b[10];
        int i, j;
        for (i=0, j=cmdslens[c]; j < len; i++, j++) {
            if (isdigit(str[j]))
                b[i] = str[j];
            else {
                printf("paramater is not a digit\n");
                return;
            }
        }
        b[i] = '\0';
        int_param = atoi(b);
    }
    else
        strcpy(char_param, str+cmdslens[c]);

    int paramlen = len - cmdslens[c];

    clock_t start = clock();
    if (c == 0) print_top(int_param);
    else if (c == 1) total(char_param);
    else if (c == 2) short_path(char_param, paramlen);
    else if (c == 3) short_path_all(char_param, paramlen);
    else if (c == 4) neighbors(char_param);
    else if (c == 5) count(char_param);
    else if (c == 6) isneighbor_print(char_param, paramlen);
    else if (c == 7) betweeness(char_param);
    else if (c == 8) close_cent(char_param);
    else if (c == 9) diameter();
    else if (c == 10) print_edgedensity();
    else if (c == 11) print_all();
    else if (c == 12) for(int i=0; i < cmdslen; i++) printf(" %s\n", cmds_str[i]);
    else if (c == 13) system("clear");
    else if (c == 14) print_hash_balance();
    else if (c == 15) exit(0);
    else if (c == 16) twocommunity(0);
    printf("in %.4lf (sec)\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}
