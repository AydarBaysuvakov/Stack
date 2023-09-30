#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include "stack.h"
#include "hash.h"

// html logfile.html <pre/>
// Makefile + FIX ALL flags warnings!!!!!

error_t MyStackCtor(Stack *stk, const char* name, const unsigned line, const char* file, const char* func)
    {
    assert(stk != NULL);

    stk->size     = DEFAULT_SIZE;
    stk->capacity = DEFAULT_CAPACITY;
    stk->data     = (Elem_t*) calloc(DEFAULT_CAPACITY + TWO_CANARY, sizeof(Elem_t)) + ONE_CANARY;

    if (stk->data == NULL)
        {
        perror("ERROR: cannot allocate memory");
        return ALLOCATION_ERROR;
        }
    FillStack(stk);

    stk->name = name;
    stk->line = line;
    stk->file = file;
    stk->func = func;

#ifndef STACK_CANARY_PROT
    stk->left_canary  = LEFT_CANARY_VALUE;
    stk->right_canary = RIGHT_CANARY_VALUE;
#endif

//?
    SetHash(stk);

    return OK;
    }

error_t StackDtor(Stack *stk)
    {
    StackState state = StackValid(stk);
    if (state)
        {
        StackDump(stk, state);
        }

    free(stk->data - ONE_CANARY);

    stk->size     = NOT_VALID_VALUE;
    stk->capacity = NOT_VALID_VALUE;

    stk->left_canary  = NOT_VALID_VALUE;
    stk->right_canary = NOT_VALID_VALUE;

    stk->data_hash  = NOT_VALID_VALUE;
    stk->stack_hash = NOT_VALID_VALUE;

    return OK;
    }

error_t FillStack(Stack *stk)
    {
    assert(stk != NULL);
    assert(stk->data != NULL);
    assert(stk->size <= stk->capacity);

    Elem_t *left_canary  = stk->data - ONE_CANARY;
    Elem_t *right_canary = stk->data + stk->capacity;

    *left_canary  = DATA_LEFT_CANARY_VALUE;
    *right_canary = DATA_RIGHT_CANARY_VALUE;

    for (size_t iter = stk->size; iter < stk->capacity; iter++)
        {
        stk->data[iter] = POISON;
        }

    return OK;
    }

error_t StackPush(Stack *stk, Elem_t value)
    {
    StackState state = StackValid(stk);
    if (state)
        {
        StackDump(stk, state);
        }

    if (stk->size == stk->capacity)
        {
        StackRealloc(stk, stk->capacity * REALLOC_COEFFICIENT);
        }

    stk->data[stk->size++] = value;

    SetHash(stk);

    return OK;
    }

error_t StackPop(Stack *stk, Elem_t *ret_val)
    {
    StackState state = StackValid(stk);
    if (state)
        {
        StackDump(stk, state);
        }

    *ret_val = stk->data[--stk->size];
    stk->data[stk->size] = POISON;

    if (stk->size <= stk->capacity / REALLOC_DOWN_BORDER && stk->capacity > MIN_REALLOC_DOWN_SIZE)
        {
        StackRealloc(stk, stk->capacity / REALLOC_COEFFICIENT);
        }

    SetHash(stk);

    return OK;
    }

error_t StackRealloc(Stack* stk, size_t new_capacity)
    {
    StackState state = StackValid(stk);
    if (state)
        {
        StackDump(stk, state);
        }

    stk->data = (Elem_t*) realloc(stk->data - ONE_CANARY, (new_capacity + TWO_CANARY) * sizeof(Elem_t)) + ONE_CANARY;
    if (stk->data == NULL)
        {
        perror("ERROR: cannot allocate memory");
        return ALLOCATION_ERROR;
        }

    stk->capacity = new_capacity;

    FillStack(stk);
    return OK;
    }

// / or  % 4 4 4 4 8 to align right canary

StackState StackValid(Stack *stk)
    {
    StackState state = 0;

    if (stk == NULL) // 1
        {
        state |= STK_NULLPTR;
        return state;
        }
    if (stk->data == NULL) // 2
        {
        state |= STK_DATA_NULL;
        }
    if (stk->size > stk->capacity) // 3
        {
        state |= STK_INVALID_SIZE;
        }
    if (stk->size != 0 && stk->data[stk->size - 1] == POISON) // 4
        {
        state |= STK_POISON_VAL;
        }
    if (stk->left_canary != LEFT_CANARY_VALUE) // 5
        {
        state |= LEFT_CANARY_CILLED;
        }
    if (stk->right_canary != RIGHT_CANARY_VALUE) // 6
        {
        state |= RIGHT_CANARY_CILLED;
        }
    if (stk->data[-ONE_CANARY] != DATA_LEFT_CANARY_VALUE) // 7
        {
        state |= DATA_LEFT_CANARY_CILLED;
        }
    if (stk->data[stk->capacity] != DATA_RIGHT_CANARY_VALUE) // 8
        {
        state |= DATA_RIGHT_CANARY_CILLED;
        }
    if (DataHashCheck(stk)) // 9
        {
        state |= DATA_HASH_NOT_COMPARE;
        }
    if (StackHashCheck(stk)) // 10
        {
        state |= STACK_HASH_NOT_COMPARE;
        }

    return state;
    }

error_t MyStackDump(const Stack *stk, StackState state, const char* name, const unsigned line, const char* file, const char* func)
    {
    printf("Stack[%p] '%s' from %s(%d) %s()\n", stk, stk->name, stk->file, stk->line, stk->func);
    printf(    "\t\tcalled from %s(%d) %s()\n",                      file,      line,      func);
    printf("\t{\n\tsize = %ld\n\tcapacity = %ld\n\tdata[%p]\n\t\t{\n", stk->size, stk->capacity, stk->data);

    for (size_t iter = 0; iter < stk->capacity; iter++)
        {
        printf((stk->data[iter] != POISON) ? "\t\t*" : "\t\t ");
        printf("[%ld] = ", iter);
        printf((stk->data[iter] == POISON) ? "POISON\n" : "%d\n", stk->data[iter]);
        }

    printf("\t\tData left canary  = %X\n", stk->data[-ONE_CANARY]);
    printf("\t\tData right canary = %X\n", stk->data[stk->capacity]);

    printf("\t\t}\n");

    printf("\tLeft canary  = %llX\n", stk->left_canary);
    printf("\tRight canary = %llX\n", stk->right_canary);

    printf("\t}\n");

    for(char bit = 0; bit < CHAR_BIT * sizeof(state); bit++)
        {
        if (state & 1 << bit)
            {
            printf("%s\n", GetStackErrorBitMsg(bit));
            }
        }

    return OK;
    }

const char* GetStackErrorBitMsg(size_t bit)
{
    const int  ERROR_COUNT = sizeof(StackState) * CHAR_BIT;
    const char * const ERROR_MESSAGE[ERROR_COUNT] = {
        "Stack is nullptr",
        "Stack->data is nullptr",
        "Stack->size > Stack->capacity",
        "Curent value is poison",
        "Left canary killed",
        "Right canary killed",
        "Data left canary killed",
        "Data right canary killed",
        "Data hash not compare",
        "Stack hash not compare"
    };

    return ERROR_MESSAGE[bit];
}
