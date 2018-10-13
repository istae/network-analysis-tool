#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <utilities.h>
#include "hash_table.h"
#include "print.h"
#include "commands.h"

#define MAXCHARS 32

void program_exit()
{
    vector* le = edge_list.begin;
    for (int i=0; i < edge_list.length; i++)
        vector_dest(le++);

    vector* ls = short_passes.begin;
    for (int i=0; i < short_passes.length; i++)
        vector_dest(ls++);

    vector_dest(&edge_list);
    vector_dest(&labels);
    vector_dest(&short_passes);

    printf("\nbye\n");
}


int main(int argc, char** argv)
{
    char* pname = argv[0];
    char* fname = NULL;
    FILE* f;
    int PRINT = 0;
    int BALANCE = 0;
    int SENTENCE = 0;

    // process argv for options
    while (--argc > 0) {
        if ((*++argv)[0] == '-')
            switch (*++argv[0]) {
            case 'f':
                fname = *++argv;
                --argc;
                break;
            case 'p':
                PRINT = 1;
                break;
            case 'b':
                BALANCE = 1;
                break;
            case 'l':
                SENTENCE = 1;
                break;
            default:
                argc = 0;
                break;
            }
        else
            argc = 0;
    }


    if (argc != 0) {
        fprintf(stderr, "%s: wrong arguments\n", pname);
        return 1;
    }
    if (fname != NULL) {
        f = fopen(fname, "r");
        if (f == NULL) {
            fprintf(stderr, "%s cant access %s\n", pname, fname);
            return 1;
        }
    }
    else
        f = stdin;

    // init hash table
    for (int i=0; i < HASTABLESIZE; i++)
        table[i] = NULL;

    // init edge list and label vectors
    vector_init(&edge_list, sizeof(vector), 1);
    vector_init(&labels, sizeof(char)*TSIZE, 1);
    vector_init(&short_passes, sizeof(vector), 1);

    // exit
    atexit(program_exit);

    // build the network from input
    char line[MAXCHARS+1];
    int len;
    while((len = _getline(f, line, MAXCHARS))) {
        if (len == MAXCHARS) {
            printf("WARNING: input node label '%s' exceeded %d characters\nContinue? y or n: ", line, MAXCHARS);
            int d = getchar();
            if (d != 'y')
                exit(1);
        }
        if (len == 1 && line[0] == '.')
            remove_last();
        else if (line[0] != '\n')
            hash_insert(line);
    }
    total_edges /= 2;

    // if -p is specified, print network connections
    if (PRINT)
        print_all();

    // if -b is specifiedm print hash table balance
    if (BALANCE)
        print_hash_balance();


    // if -l print words in each line in each hash table index
    if (SENTENCE)
        print_sentence();


    // get commands
    printf("\n[Commands] type 'commands' for the list\n");

    char c[] = "commands";
    cmd("commands", sizeof c);
    printf("\n");

    while((len = _getline(stdin, line, 50))) {
        if (line[0] != '\n')
            cmd(line, len);
        printf("\n");
    }

    return 0;
}
