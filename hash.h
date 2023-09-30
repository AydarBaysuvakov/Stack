#ifndef HASH_H
#define HASH_H

#include "stack.h"

typedef long long Hash_t;

const size_t SIZE_OF_STACK_STRUCT = sizeof(Canary_t) * 2 + sizeof(Elem_t*) + sizeof(size_t) * 2 + sizeof(char*) * 3 + sizeof(unsigned) + sizeof(Hash_t) * 2;
const Hash_t DEFAULT_HASH_VALUE = 0xA1DA2BA15;

Hash_t  HashFunction(const char *array, size_t size);

Hash_t  StackHashFunction(const Stack *stk);

Hash_t  DataHashFunction(const Stack *stk);

error_t SetHash(Stack *stk);

error_t HashCheck(Stack *stk);

error_t DataHashCheck(Stack *stk);

error_t StackHashCheck(Stack *stk);

#endif//HASH_H_
