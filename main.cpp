#include <stdlib.h>
#include "stack.h"

int main()
    {
    Stack stk = {};
    StackCtor_(&stk);
    stk.data[0] = 1;
    StackDump_(&stk, 0);
    StackDtor(&stk);
    }
