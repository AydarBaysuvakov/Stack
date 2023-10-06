#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int main()
    {
    Stack stk = {};
    StackCtor(&stk);

    stk.capacity = 10;
    StackPush(&stk, 8);

    StackDtor(&stk);
    }
