#ifndef _HashSep_H

struct ListNode;
typedef struct ListNode *Position;
struct HashTbl;
typedef struct HashTbl *HashTable;

typedef char *ElementType;

HashTable init_table(int size);
void make_empty(HashTable h);
void destroy_table(HashTable h);
Position find(HashTable h, ElementType key);
void insert(HashTable h, ElementType key);
void delete(HashTable h, ElementType key);
ElementType retrieve(Position p);

#endif