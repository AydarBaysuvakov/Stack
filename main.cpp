#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int main()
    {
    Stack stk = {};
    StackCtor_(&stk);
    StackPush(&stk, 5);
    StackPush(&stk, 100);
    StackPush(&stk, 64 * 64);
    StackPush(&stk, 0);
    StackPush(&stk, 5);
    StackPush(&stk, 100);
    StackPush(&stk, 16 * 16);
    StackPush(&stk, 0);
    StackPush(&stk, 5);
    Elem_t ret = 0;
    StackPop(&stk, &ret);
    printf("%d\n", ret);
    StackDump_(&stk, 0);
    StackDtor(&stk);
    }
