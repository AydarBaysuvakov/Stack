#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "stack.h"

int main()
    {
    Stack stk = {};
    StackCtor(&stk);


    StackDtor(&stk);
    }
