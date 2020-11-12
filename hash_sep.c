#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_sep.h"

struct ListNode {
    ElementType element;
    Position next;
};

typedef Position List;

// List *TheList will be an array of lists, allocated later
// The lists use headers (for simplicity),
// though this wastes space
struct HashTbl {
    int tableSize;
    List *table;
};

static int hash(ElementType key, int tableSize);
static void terminate(const char *message);
static int next_prime(int n);

int main(void)
{
    return 0;
}

HashTable init_table(int size)
{
    HashTable h;
    
    h = malloc(sizeof(struct HashTbl));
    if ( h == NULL )
        terminate("Error in initialize: Hash table could not be created.");
    
    h->tableSize = next_prime(size);
    h->table = malloc(sizeof(List) * h->tableSize);
    if ( h->table == NULL ) {
        destroy_table(h);
        terminate("Error in initialize: fail to allocate space for table.");
    }
    
    for ( int i = 0; i < h->tableSize; ++i ) {
        h->table[i] = malloc(sizeof(struct ListNode));
        if ( h->table[i] = NULL ) {
            destroy_table(h);
            terminate("Error in initialize: Out of sapce!");
        }
        else
            h->table[i]->next = NULL;
    }

    return h;
}

Position find(HashTable h, ElementType key)
{
    Position p;
    List l;

    l = h->table[hash(key, h->tableSize)];
    p = l->next;

    while ( p != NULL && strcmp(p->element, key) != 0 )
        p = p->next;
    
    return p;
}

/*
void insert(HashTable h, ElementType key)
{
    Position tmp;
    List l;

    if ( find(h, key) == NULL ) {
        tmp = malloc(sizeof(struct ListNode));
        if ( tmp == NULL ) {
            destroy_table(h);
            terminate("Error in insert: fail to allocate space for new node.");
        }
        tmp->element = key;

        l = h->table[hash(key, h->tableSize)];
        tmp->next = l->next;
        l->next = tmp;
    }
}
*/

void insert(HashTable h, ElementType key)
{
    Position tmp, pos;
    List l;

    l = h->table[hash(key, h->tableSize)];
    pos = l->next;
    while ( pos != NULL && strcmp(pos->element, key) != 0 )
        pos = pos->next;
    
    if ( pos == NULL ) {
        tmp = malloc(sizeof(struct ListNode));
        if ( tmp == NULL ) {
            destroy_table(h);
            terminate("Error in insert: fail to allocate space for new node.");
        }

        tmp->element = key;
        tmp->next = l->next;
        l->next = tmp;
    }
}



static void terminate(const char *message)
{
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}