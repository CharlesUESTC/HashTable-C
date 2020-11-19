#ifndef _HashSep_H
#include <stdbool.h>

struct ListNode;
typedef struct ListNode *Position;
struct HashTbl;
typedef struct HashTbl *HashTable;

typedef char *KeyType;
typedef int ValueType;
typedef struct Element ElementType;

HashTable init_table(int size);
void make_empty(HashTable h);
void destroy_table(HashTable h);
Position find(HashTable h, KeyType key);
void put(HashTable h, KeyType key, ValueType val);
bool get(HashTable h, KeyType key, ValueType *rval);
void delete(HashTable h, KeyType key);
ElementType retrieve(Position p);

#endif