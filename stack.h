#ifndef __STACK__
#define __STACK__

#define StackCtor_(stk)         StackCtor((stk), #stk,    __LINE__, __FILE__, __FUNCTION__)
#define StackDump_(stk, stk_st) StackDump((stk), (stk_st),  __LINE__, __FILE__, __FUNCTION__)

#include <math.h>

typedef long long Hash_t;
typedef int       Elem_t;
typedef long long Canary_t;
typedef int       StackState;

const Canary_t LEFT_CANARY_VALUE  = 0xDEDDABABA;
const Canary_t RIGHT_CANARY_VALUE = 0xBABADADED;

const Elem_t   DATA_LEFT_CANARY_VALUE  = (Elem_t) (0xA1DA2 % (int) pow(2, 8 * sizeof(Elem_t)));
const Elem_t   DATA_RIGHT_CANARY_VALUE = (Elem_t) (0xBA15  % (int) pow(2, 8 * sizeof(Elem_t)));

const size_t   DEFAULT_SIZE     = 0;
const size_t   DEFAULT_CAPACITY = 8;

const size_t   ONE_CANARY       = 1;
const size_t   TWO_CANARY       = 2;

const long long NOT_VALID_VALUE  = 0xBAD;
const Elem_t    POISON           = (Elem_t) (0xBAD % (int) pow(2, 8 * sizeof(Elem_t)));

const int REALLOC_COEFFICIENT   = 2;
const int MIN_REALLOC_DOWN_SIZE = 16;
const int REALLOC_DOWN_BORDER   = 4;

const size_t BIT_IN_BYTE_ = 8;

enum error_t
    {
    OK               = 0,
    ALLOCATION_ERROR = 1,
    HASH_NOT_COMPARE = 2
    };

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

    Hash_t stack_hash;
    Hash_t  data_hash;

    Canary_t right_canary;
    };

const int  ERROR_COUNT = sizeof(StackState) * BIT_IN_BYTE_;
const char * const ERROR_MESSAGE[ERROR_COUNT] = {"1.Stack is nullptr", "2.Stack->data is nullptr", "3.Stack->size > Stack->capacity", "4.Curent value is poison", "5.Left canary killed", "6.Right canary killed", "7.Data left canary killed", "8.Data right canary killed", "9.Data hash not compare", "10.Stack hash not compare"};

error_t StackCtor(Stack *stk, const char* name, const unsigned line, const char* file, const char* func);

error_t StackDtor(Stack *stk);

error_t StackFillingWithPoison(Stack *stk);

error_t StackPush(Stack *stk, Elem_t value);

error_t StackPop(Stack *stk, Elem_t *ret_val);

error_t StackReallocUp(Stack *stk);

error_t StackReallocDown(Stack *stk);

StackState StackValid(Stack *stk);

error_t StackDump(const Stack *stk, StackState state, const unsigned line, const char* file, const char* func);

#endif//__STACK__
