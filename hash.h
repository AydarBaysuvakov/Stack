#ifndef __HASH__
#define __HASH__

#include "stack.h"

typedef long long Hash_t;

const size_t BIT_IN_BYTE = 8;
const size_t SIZE_OF_STACK_STRUCT = sizeof(Canary_t) * 2 + sizeof(Elem_t*) + sizeof(size_t) * 2 + sizeof(char*) * 3 + sizeof(unsigned) + sizeof(Hash_t) * 2;
const Hash_t DEFAULT_HASH_VALUE = 0xA1DA2BA15;

Hash_t StackHashFunction(const Stack *stk);

Hash_t  DataHashFunction(const Stack *stk);

error_t SetHash(Stack *stk);

error_t HashCheck(Stack *stk);

error_t DataHashCheck(Stack *stk);

error_t StackHashCheck(Stack *stk);

#endif//__HASH__
