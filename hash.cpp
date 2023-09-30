#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include "hash.h"

Hash_t HashFunction(const char *array, size_t size)
    {
    assert(array != NULL);

    Hash_t hash = 0;

    for (size_t byte = 0; byte < size; byte++)
        {
        for (size_t bit = 0; bit < CHAR_BIT; bit++)
            {
            hash += array[byte] & 1 << bit;
            }
        }

    return hash * hash / 2;
    }

Hash_t StackHashFunction(const Stack *stk)
    {
    assert(stk != NULL);

    return HashFunction((char*) stk, SIZE_OF_STACK_STRUCT);
    }

Hash_t DataHashFunction(const Stack *stk)
    {
    assert(stk != NULL);

    return HashFunction((char*) stk->data - ONE_CANARY, stk->capacity + TWO_CANARY);
    }

error_t SetHash(Stack *stk)
    {
    assert(stk != NULL);

    stk->data_hash  =  DataHashFunction(stk);

    stk->stack_hash = DEFAULT_HASH_VALUE;
    stk->stack_hash = StackHashFunction(stk);

    return OK;
    }

error_t StackHashCheck(Stack *stk)
    {
    assert(stk != NULL);

    Hash_t curent_stk_hash = stk->stack_hash;
    stk->stack_hash = DEFAULT_HASH_VALUE;
    Hash_t stack_hash = StackHashFunction(stk);
    if (stack_hash != curent_stk_hash)
        {
        return HASH_NOT_COMPARE;
        }
    stk->stack_hash = curent_stk_hash;

    return OK;
    }

error_t DataHashCheck(Stack *stk)
    {
    assert(stk != NULL);

    Hash_t data_hash = DataHashFunction(stk);
    if (data_hash != stk->data_hash)
        {
        return HASH_NOT_COMPARE;
        }

    return OK;
    }


error_t HashCheck(Stack *stk)
    {
    assert(stk != NULL);

    if ( DataHashCheck(stk)) return HASH_NOT_COMPARE;
    if (StackHashCheck(stk)) return HASH_NOT_COMPARE;

    return OK;
    }
