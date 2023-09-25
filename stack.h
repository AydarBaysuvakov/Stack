#ifndef __STACK__
#define __STACK__

#define StackCtor_(stk)         StackCtor((stk), #stk,    __LINE__, __FILE__, __FUNCTION__)
#define StackDump_(stk, stk_st) StackDump((stk), (stk_st),  __LINE__, __FILE__, __FUNCTION__)


typedef int       Elem_t;
typedef long long Canary_t;
typedef char      Stack_state;

const Canary_t LEFT_CANARY_VALUE  = 0xDEDDABABA;
const Canary_t RIGHT_CANARY_VALUE = 0xBABADADED;
const size_t   DEFAULT_CAPACITY = 5;
const size_t   DEFAULT_SIZE     = 0;
const size_t   NOT_VALID_VALUE  = 0xBAD;
const Elem_t   POISON = sizeof(Elem_t) * sizeof(Elem_t) * sizeof(Elem_t) * sizeof(Elem_t);//заглушка

struct Stack
    {
    Canary_t  left_canary;

    Elem_t *data;
    size_t  size;
    size_t  capacity;

    const char    *name;
    const char    *file;
    const char    *func;
    unsigned       line;

    Canary_t right_canary;
    };

enum error_t
    {
    OK               = 0,
    ALLOCATION_ERROR = 1
    };

//const int  ERROR_COUNT = 8;
//const char *ERROR_MESSAGE[ERROR_COUNT] = {"Stack is nullptr", "Stack->data is nullptr", "Stack->size < 0", "Stack->capacity < 0", "", "", "", ""};

error_t StackCtor(Stack *stk, const char* name, const unsigned line, const char* file, const char* func);

error_t StackDtor(Stack *stk);

error_t StackFillingWithPoison(Stack *stk);

error_t StackPush(Stack *stk, Elem_t value);

error_t StackPop(Stack *stk, Elem_t *ret_val);

Stack_state StackValid(const Stack *stk);

error_t StackDump(const Stack *stk, Stack_state state, const unsigned line, const char* file, const char* func);

#endif//__STACK__
