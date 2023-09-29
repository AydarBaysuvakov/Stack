#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack.h"
#include "hash.h"


error_t StackCtor(Stack *stk, const char* name, const unsigned line, const char* file, const char* func)
    {
    assert(stk != NULL);

    stk->size     = DEFAULT_SIZE;
    stk->capacity = DEFAULT_CAPACITY;
    stk->data     = (Elem_t*) calloc(DEFAULT_CAPACITY + TWO_CANARY, sizeof(Elem_t));

    if (stk->data == NULL)
        {
        perror("ERROR: cannot allocate memory");
        return ALLOCATION_ERROR;
        }
    StackFillingWithPoison(stk);//and set canary

    stk->name = name;
    stk->line = line;
    stk->file = file;
    stk->func = func;

    stk->left_canary  = LEFT_CANARY_VALUE;
    stk->right_canary = RIGHT_CANARY_VALUE;

    SetHash(stk);

    return OK;
    }

error_t StackDtor(Stack *stk)
    {
    StackState state = StackValid(stk);
    if (state)
        {
        StackDump_(stk, state);
        }

    free(stk->data);

    stk->size     = NOT_VALID_VALUE;
    stk->capacity = NOT_VALID_VALUE;

    stk->left_canary  = NOT_VALID_VALUE;
    stk->right_canary = NOT_VALID_VALUE;

    stk->data_hash  = NOT_VALID_VALUE;
    stk->stack_hash = NOT_VALID_VALUE;

    return OK;
    }

error_t StackFillingWithPoison(Stack *stk)
    {
    assert(stk != NULL);
    assert(stk->data != NULL);
    assert(stk->size <= stk->capacity);

    Elem_t *left_canary  = stk->data;
    Elem_t *data         = stk->data + ONE_CANARY;
    Elem_t *right_canary = stk->data + stk->capacity + ONE_CANARY;

    *left_canary  = DATA_LEFT_CANARY_VALUE;
    *right_canary = DATA_RIGHT_CANARY_VALUE;

    for (size_t iter = stk->size; iter < stk->capacity; iter++)
        {
        data[iter] = POISON;
        }

    return OK;
    }

error_t StackPush(Stack *stk, Elem_t value)
    {
    StackState state = StackValid(stk);
    if (state)
        {
        StackDump_(stk, state);
        }

    if (stk->size == stk->capacity)
        {
        StackReallocUp(stk);
        }

    stk->data[stk->size++ + ONE_CANARY] = value;

    SetHash(stk);

    return OK;
    }

error_t StackPop(Stack *stk, Elem_t *ret_val)
    {
    StackState state = StackValid(stk);
    if (state)
        {
        StackDump_(stk, state);
        }

    *ret_val = stk->data[--stk->size + ONE_CANARY];
    stk->data[stk->size + ONE_CANARY] = POISON;

    if (stk->size <= stk->capacity / REALLOC_DOWN_BORDER && stk->capacity > MIN_REALLOC_DOWN_SIZE)
        {
        StackReallocDown(stk);
        }

    SetHash(stk);

    return OK;
    }

error_t StackReallocUp(Stack *stk)
    {
    StackState state = StackValid(stk);
    if (state)
        {
        StackDump_(stk, state);
        }

    stk->data = (Elem_t*) realloc(stk->data, (stk->capacity * REALLOC_COEFFICIENT + TWO_CANARY) * sizeof(Elem_t));
    if (stk->data == NULL)
        {
        perror("ERROR: cannot allocate memory");
        return ALLOCATION_ERROR;
        }

    stk->capacity *= REALLOC_COEFFICIENT;

    StackFillingWithPoison(stk);//and set canary
    return OK;
    }

error_t StackReallocDown(Stack *stk)
    {
    StackState state = StackValid(stk);
    if (state)
        {
        StackDump_(stk, state);
        }

    stk->data = (Elem_t*) realloc(stk->data, (stk->capacity / REALLOC_COEFFICIENT + TWO_CANARY) * sizeof(Elem_t));
    if (stk->data == NULL)
        {
        perror("ERROR: cannot allocate memory");
        return ALLOCATION_ERROR;
        }

    stk->capacity /= REALLOC_COEFFICIENT;

    StackFillingWithPoison(stk);//and set canary
    return OK;
    }

StackState StackValid(Stack *stk)
    {
    StackState state = 0;
    int bit = 1;

    if (stk == NULL) // 1
        {
        state |= bit;
        return state;
        }
    bit *= 2;
    if (stk->data == NULL) // 2
        {
        state |= bit;
        }
    bit *= 2;
    if (stk->size > stk->capacity) // 3
        {
        state |= bit;
        }
    bit *= 2;
    if (stk->size != 0 && stk->data[stk->size] == POISON) // 4
        {
        state |= bit;
        }
    bit *= 2;
    if (stk->left_canary != LEFT_CANARY_VALUE) // 5
        {
        state |= bit;
        }
    bit *= 2;
    if (stk->right_canary != RIGHT_CANARY_VALUE) // 6
        {
        state |= bit;
        }
    bit *= 2;
    if (stk->data[0] != DATA_LEFT_CANARY_VALUE) // 7
        {
        state |= bit;
        }
    bit *= 2;
    if (stk->data[stk->capacity + ONE_CANARY] != DATA_RIGHT_CANARY_VALUE) // 8
        {
        state |= bit;
        }
    bit *= 2;
    if (DataHashCheck(stk)) // 9
        {
        state |= bit;
        }
    bit *= 2;
    if (StackHashCheck(stk)) // 10
        {
        state |= bit;
        }
    bit *= 2;

    return state;
    }

error_t StackDump(const Stack *stk, StackState state, const unsigned line, const char* file, const char* func)
    {
    printf("Stack[%p] '%s' from %s(%d) %s()\n", stk, stk->name, stk->file, stk->line, stk->func);
    printf(    "\t\tcalled from %s(%d) %s()\n",                      file,      line,      func);
    printf("\t{\n\tsize = %ld\n\tcapacity = %ld\n\tdata[%p]\n\t\t{\n", stk->size, stk->capacity, stk->data);

    for (size_t iter = 0; iter < stk->capacity; iter++)
        {
        printf((stk->data[iter + ONE_CANARY] != POISON) ? "\t\t*" : "\t\t ");
        printf("[%ld] = ", iter);
        printf((stk->data[iter + ONE_CANARY] == POISON) ? "POISON\n" : "%d\n", stk->data[iter + ONE_CANARY]);
        }

    printf("\t\tData left canary  = %X\n", stk->data[0]);
    printf("\t\tData right canary = %X\n", stk->data[stk->capacity + ONE_CANARY]);

    printf("\t\t}\n\t}\n");

    for(char bit = 0; bit < BIT_IN_BYTE * sizeof(state); bit++)
        {
        if (state & (int) pow(2, bit))
            {
            printf("%s\n", ERROR_MESSAGE[bit]);
            }
        }

    return OK;
    }
