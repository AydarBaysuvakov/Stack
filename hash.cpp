#include <math.h>
#include <assert.h>
#include <stdio.h>
#include "hash.h"

Hash_t StackHashFunction(const Stack *stk)
    {
    assert(stk != NULL);

    char *bytes = (char*) stk;
    size_t bytes_count = SIZE_OF_STACK_STRUCT;
    Hash_t stack_hash = 0;

    for (size_t byte = 0; byte < bytes_count; byte++)
        {
        for (size_t bit = 0; bit < BIT_IN_BYTE; bit++)
            {
            stack_hash += bytes[byte] & (int) pow(2, bit);
            }
        }

    return stack_hash * stack_hash / 2;
    }

Hash_t DataHashFunction(const Stack *stk)
    {
    assert(stk != NULL);

    char *bytes = (char*) stk->data;
    size_t bytes_count = (TWO_CANARY + stk->capacity) * sizeof(Elem_t);
    Hash_t data_hash = 0;

    for (size_t byte = 0; byte < bytes_count; byte++)
        {
        for (size_t bit = 0; bit < BIT_IN_BYTE; bit++)
            {
            data_hash += bytes[byte] & (int) pow(2, bit);
            }
        }

    return data_hash * data_hash / 2;
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

    Hash_t data_hash = DataHashFunction(stk);
    if (data_hash != stk->data_hash)
        {
        return HASH_NOT_COMPARE;
        }

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
