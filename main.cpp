#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int main()
    {
    Stack stk = {};
    StackCtor(&stk);

    // ВЗЛОМ

    StackDtor(&stk);
    }
