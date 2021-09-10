#ifndef _HashQuad_H

#include <stdbool.h>

typedef unsigned int Index;
typedef Index Position;

struct Element;
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
ElementType retrieve(HashTable h, Position p);\
HashTable rehash(HashTable h);

#endif  /* _HashQuad_H */
