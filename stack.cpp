#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack.h"

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

    return OK;
    }

error_t StackDtor(Stack *stk)
    {
    Stack_state state = StackValid(stk);
    if (state)
        {
        StackDump_(stk, state);
        }

    free(stk->data);

    stk->size     = NOT_VALID_VALUE;
    stk->capacity = NOT_VALID_VALUE;

    return OK;
    }

error_t StackFillingWithPoison(Stack *stk)
    {
    Stack_state state = StackValid(stk);
    if (state)
        {
        StackDump_(stk, state);
        }

    Elem_t *left_canary  = stk->data;
    Elem_t *data         = stk->data + 1;
    Elem_t *right_canary = stk->data + stk->capacity + 1;

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
    Stack_state state = StackValid(stk);
    if (state)
        {
        StackDump_(stk, state);
        }

    if (stk->size == stk->capacity)
        {
        StackReallocUp(stk);
        }

    stk->data[ONE_CANARY + stk->size++] = value;

    return OK;
    }

error_t StackPop(Stack *stk, Elem_t *ret_val)
    {
    Stack_state state = StackValid(stk);
    if (state)
        {
        StackDump_(stk, state);
        }

    *ret_val = stk->data[ONE_CANARY + --stk->size];
    stk->data[ONE_CANARY + stk->size] = POISON;

    if (stk->size <= stk->capacity / REALLOC_DOWN_BORDER && stk->capacity > MIN_REALLOC_DOWN_SIZE)
        {
        StackReallocDown(stk);
        }

    return OK;
    }

error_t StackReallocUp(Stack *stk)
    {
    Stack_state state = StackValid(stk);
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
    Stack_state state = StackValid(stk);
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

Stack_state StackValid(const Stack *stk)
    {
    Stack_state state = 0;

    /* checking */

    return state;
    }

error_t StackDump(const Stack *stk, Stack_state state, const unsigned line, const char* file, const char* func)
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

    /*for(char iter = 0; iter < 8 * sizeof(state); iter++)
        {
        if (state & 1)
            {
            printf("%s", ERROR_MESSAGE[iter]);
            }
        state = state >> 1;
        }*/

    return OK;
    }

