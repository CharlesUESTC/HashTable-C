#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_sep.h"

#define XXH_INLINE_ALL
#include "xxhash.h"

#define HASH_SEED 31

struct Element {
    KeyType key;
    ValueType value;
};

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
    int count;
    List *table;
};

static unsigned int hash(KeyType key, int tableSize);
static void terminate(const char *message);
static int next_prime(int n);

int main(void)
{
    HashTable ht = init_table(100);
    ValueType v1, v2, v3;
    
    put(ht, "Charles_98_", 22);
    put(ht, "linda-1018", 25);
    put(ht, "xyz_LOVEthink", 23);

    if ( get(ht, "linda-1018", &v1) )
        printf("%d\n", v1);
    if ( get(ht, "Charles_98_", &v2) )
        printf("%d\n", v2);
    if ( get(ht, "xyz_lovethink", &v3))
        printf("%d\n", v3);
    
    delete(ht, "xyz_LOVEthink");
    if ( get(ht, "xyz_LOVEthink", &v3) )
        printf("%d\n", v3);
    
    put(ht, "linda-1018", 18);
    if ( get(ht, "linda-1018", &v2) )
        printf("%d\n", v2);
    
    make_empty(ht);
    if ( get(ht, "Charles_98", &v1) )
        printf("%d\n", v1);
    
    destroy_table(ht);

    return 0;
}

HashTable init_table(int size)
{
    HashTable h;
    
    h = malloc(sizeof(struct HashTbl));
    if ( h == NULL )
        terminate("Error in initialize: Hash table could not be created.");
    
    h->tableSize = next_prime(size);
    h->count = 0;
    h->table = malloc(sizeof(List) * h->tableSize);
    if ( h->table == NULL ) {
        free(h);
        terminate("Error in initialize: fail to allocate space for table.");
    }
    
    for ( int i = 0; i < h->tableSize; ++i ) {
        h->table[i] = malloc(sizeof(struct ListNode));
        if ( h->table[i] == NULL ) {
            for ( int j = 0; j < i; ++j )
                free(h->table[j]);
            free(h->table);
            free(h);
            terminate("Error in initialize: Out of sapce!");
        }
        else
            h->table[i]->next = NULL;
    }

    return h;
}

void make_empty(HashTable h)
{
    Position tmp;
    List l;

    for ( int i = 0; i < h->tableSize; ++i ) {
        l = h->table[i];
        while ( l->next != NULL ) {
            tmp = l->next;
            l->next = tmp->next;
            free(tmp);
        }
    }

    h->count = 0;
}

void destroy_table(HashTable h)
{
    make_empty(h);
    for ( int i = 0; i < h->tableSize; ++i )
        free(h->table[i]);
    free(h->table);
    free(h);
}

Position find(HashTable h, KeyType key)
{
    Position p;
    List l;

    l = h->table[hash(key, h->tableSize)];
    p = l->next;

    while ( p != NULL && strcmp(p->element.key, key) != 0 )
        p = p->next;
    
    return p;
}

// not good implementation, because using hash twice
/*
void put(HashTable h, KeyType key)
{
    Position tmp;
    List l;

    if ( find(h, key) == NULL ) {
        tmp = malloc(sizeof(struct ListNode));
        if ( tmp == NULL ) {
            destroy_table(h);
            terminate("Error in put: fail to allocate space for new node.");
        }
        tmp->element = key;

        l = h->table[hash(key, h->tableSize)];
        tmp->next = l->next;
        l->next = tmp;
    }
}
*/

void put(HashTable h, KeyType key, ValueType val)
{
    Position tmp, pos;
    List l;

    l = h->table[hash(key, h->tableSize)];
    pos = l->next;
    while ( pos != NULL && strcmp(pos->element.key, key) != 0 )
        pos = pos->next;
    
    if ( pos == NULL ) {
        tmp = malloc(sizeof(struct ListNode));
        if ( tmp == NULL ) {
            destroy_table(h);
            terminate("Error in put: fail to allocate space for new node.");
        }

        tmp->element.key = key;
        tmp->element.value = val;
        tmp->next = l->next;
        l->next = tmp;

        h->count++;
    }
    else
        pos->element.value = val;
}

bool get(HashTable h, KeyType key, ValueType *rval)
{
    Position pos;
    bool flag = false;

    pos = find(h, key);
    if ( pos != NULL ) {
        flag = true;
        *rval = pos->element.value;
    }

    return flag;
}

void delete(HashTable h, KeyType key)
{
    Position pos, tmp;

    pos = h->table[hash(key, h->tableSize)];
    while ( pos->next != NULL && strcmp(pos->next->element.key, key) != 0 )
        pos = pos->next;
    
    if ( pos->next != NULL ) {
        tmp = pos->next;
        pos->next = tmp->next;
        free(tmp);
        h->count--;
    }
}

ElementType retrieve(Position p)
{
    return p->element;
}

static int next_prime(int n)
{
    int *a, *p;
    int i, j, k = 0;
    a = malloc(sizeof(int) * (2 * n - 2));
    p = malloc(sizeof(int) * n);
    memset(a, 0, sizeof(int) * (2 * n - 2));

    for ( i = 2; i < 2 * n - 2; ++i ) {
        if ( a[i] == 0 ) {
            if ( i >= n )
                break;
            p[++k] = i;
        }
        for ( j = 1; j <= k && i * p[j] < 2 * n - 2; ++j ) {
            a[i * p[j]] = 1;
            if ( i % p[j] == 0 )
                break;
        }
    }

    free(a);
    free(p);

    return i;
}

static unsigned int hash(KeyType key, int tableSize)
{
    int len = strlen(key);

    return XXH32(key, len, HASH_SEED) % tableSize;
}

static void terminate(const char *message)
{
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}