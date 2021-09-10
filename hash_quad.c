#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_quad.h"

#define XXH_INLINE_ALL
#include "xxhash.h"

#define HASH_SEED 31

enum KindOfEntry {Legitimate, Empty, Deleted};

struct Element {
    KeyType key;
    ValueType value;
};

struct HashEntry {
    ElementType element;
    enum KindOfEntry info;
};

typedef struct HashEntry Cell;

/* Cell *table will be an array of */
/* HashEntry cells, allocated later */
struct HashTbl {
    int tableSize;
    int count; /* Cell not empty, deleted included */
    Cell *table;
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

    if (get(ht, "linda-1018", &v1))
        printf("%d\n", v1);
    if (get(ht, "Charles_98_", &v2))
        printf("%d\n", v2);
    if (get(ht, "xyz_lovethink", &v3))
        printf("%d\n", v3);
    
    delete(ht, "xyz_LOVEthink");
    if (get(ht, "xyz_LOVEthink", &v3))
        printf("%d\n", v3);
    
    put(ht, "linda-1018", 18);
    if (get(ht, "linda-1018", &v2))
        printf("%d\n", v2);
    
    make_empty(ht);
    if (get(ht, "Charles_98", &v1))
        printf("%d\n", v1);
    
    destroy_table(ht);

    return 0;
}

HashTable init_table(int size)
{
    HashTable h;

    h = malloc(sizeof(struct HashTbl));
    if (h == NULL)
        terminate("Error in initialize: Hash table could not be created.");
    
    h->tableSize = next_prime(size);
    h->count = 0;
    h->table = malloc(sizeof(Cell) * h->tableSize);
    if (h->table == NULL) {
        free(h);
        terminate("Error in initialize: fail to allocate space for table.");
    }
    
    for (int i = 0; i < h->tableSize; ++i) 
        h->table[i].info = Empty;

    return h;
}

void make_empty(HashTable h)
{
    for (int i = 0; i < h->tableSize; ++i)
        h->table[i].info = Empty;

    h->count = 0;
}

void destroy_table(HashTable h)
{
    free(h->table);
    free(h);
}

Position find(HashTable h, KeyType key)
{
    Position currentPos;
    int collisionNum = 0;

    currentPos = hash(key, h->tableSize);
    while (h->table[currentPos].info != Empty &&
           strcmp(h->table[currentPos].element.key, key) != 0) {
        currentPos += 2 * ++collisionNum - 1;
        if (currentPos >= h->tableSize)
            currentPos -= h->tableSize;
    }

    return currentPos;
}

void put(HashTable h, KeyType key, ValueType val)
{
    Position pos;

    pos = find(h, key);
    if (h->table[pos].info == Empty) {
        h->table[pos].info = Legitimate;
        h->table[pos].element.key = key;
        h->table[pos].element.value = val;
        ++h->count;
    }
    else
        h->table[pos].element.value = val;
}

bool get(HashTable h, KeyType key, ValueType *rval)
{
    Position pos;
    bool flag = false;

    pos = find(h, key);
    if (h->table[pos].info == Legitimate) {
        flag = true;
        *rval = h->table[pos].element.value;
    }

    return flag;
}

void delete(HashTable h, KeyType key)
{
    Position pos;

    pos = find(h, key);
    
    if (h->table[pos].info == Legitimate)
        h->table[pos].info = Deleted;
}

ElementType retrieve(HashTable h, Position p)
{
    return h->table[p].element;
}

HashTable rehash(HashTable h)
{
    HashTable newTable;

    /* Get a new, empty table */
    newTable = init_table(2 * h->tableSize);

    /* Scan through old table, reinserting into new */
    for (int i = 0; i < h->tableSize; ++i)
        if (h->table[i].info == Legitimate)
            put(newTable, h->table[i].element.key, h->table[i].element.value);
    
    free(h->table);
    free(h);

    return newTable;
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
