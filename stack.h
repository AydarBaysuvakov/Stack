#ifndef STACK_H
#define STACK_H

#include <limits.h>

#define STACK_DEFN_ARGS const char*, const unsigned, const char*, const char*
#define STACK_PASS_ARGS __LINE__, __FILE__, __FUNCTION__
#define StackCtor(stk)         MyStackCtor((stk),           #stk, STACK_PASS_ARGS)
#define StackDump(stk, stk_st) MyStackDump((stk), (stk_st), #stk, STACK_PASS_ARGS)


typedef int       Elem_t;

const size_t   DEFAULT_SIZE     = 0;
const size_t   DEFAULT_CAPACITY = 8;

const long long NOT_VALID_VALUE  = 0xBAD;
const Elem_t    POISON           = (Elem_t) (0xBAD % (((long long) 1) << CHAR_BIT * sizeof(Elem_t)));

const int REALLOC_COEFFICIENT   = 2;
const int MIN_REALLOC_DOWN_SIZE = 16;
const int REALLOC_DOWN_BORDER   = 4;

const size_t LOG_FILE_MAX_NAME_LENGHT     = 60;
const size_t LOG_FILE_NAME_LENGHT = 20;

typedef long long Canary_t;

#ifdef STACK_CANARY_PROT
const Canary_t LEFT_CANARY_VALUE  = 0xDEDDABABA;
const Canary_t RIGHT_CANARY_VALUE = 0xBABADADED;

const Canary_t DATA_LEFT_CANARY_VALUE  = 0xA1DA2;
const Canary_t DATA_RIGHT_CANARY_VALUE = 0xBA15;

const size_t   ONE_CANARY = 1;
const size_t   TWO_CANARY = ONE_CANARY * 2;
#else
const size_t   ONE_CANARY = 0;
const size_t   TWO_CANARY = 0;
#endif

#ifdef STACK_HASH_PROT
#include "hash.h"
const size_t SIZE_OF_STACK_STRUCT = sizeof(Canary_t) * 2 + sizeof(Elem_t*) + sizeof(size_t) * 2 + sizeof(char*) * 3 + sizeof(unsigned) + sizeof(Hash_t) * 2;
#endif

struct Stack
    {
#ifdef STACK_CANARY_PROT
    Canary_t  left_canary;
#endif

    Elem_t *data;
    size_t  size;
    size_t  capacity;

    const char  *name;
    const char  *file;
    const char  *func;
    unsigned     line;

    FILE *logfile;

#ifdef STACK_HASH_PROT
    Hash_t stack_hash;
    Hash_t  data_hash;
#endif

#ifdef STACK_CANARY_PROT
    Canary_t right_canary;
#endif
    };

enum StackErrorBit
{
    STK_NULLPTR               = 1 << 0,
    STK_DATA_NULL             = 1 << 1,
    STK_INVALID_SIZE          = 1 << 2,
    STK_POISON_VAL            = 1 << 3,
#ifdef STACK_CANARY_PROT
    LEFT_CANARY_CILLED        = 1 << 4,
    RIGHT_CANARY_CILLED       = 1 << 5,
    DATA_LEFT_CANARY_CILLED   = 1 << 6,
    DATA_RIGHT_CANARY_CILLED  = 1 << 7,
#endif
#ifdef STACK_HASH_PROT
    DATA_HASH_NOT_COMPARE     = 1 << 8,
    STACK_HASH_NOT_COMPARE    = 1 << 9,
#endif
};

Error_t MyStackCtor(Stack *stk, STACK_DEFN_ARGS);

Error_t StackDtor(Stack *stk);

Error_t FillStack(Stack *stk);

Error_t StackPush(Stack *stk, Elem_t value);

Elem_t  StackPop(Stack *stk);

Elem_t StackTop(const Stack *stk);

Error_t StackRealloc(Stack* stk, size_t new_capacity);

State_t StackValid(Stack *stk);

bool IsStackErrorState(State_t state);

Error_t MyStackDump(const Stack *stk, State_t state, STACK_DEFN_ARGS);

const char* GetStackErrorBitMsg(size_t bit);

#ifdef STACK_CANARY_PROT
Canary_t* GetDataLeftCanary(const Stack *stk);

Canary_t* GetDataRightCanary(const Stack *stk);
#else
Elem_t* GetDataLeftCanary(const Stack *stk);

Elem_t* GetDataRightCanary(const Stack *stk);
#endif

#ifdef STACK_HASH_PROT
Hash_t  StackHashFunction(const Stack *stk);

Hash_t  DataHashFunction(const Stack *stk);

Error_t SetHash(Stack *stk);

Error_t HashCheck(Stack *stk);

Error_t DataHashCheck(Stack *stk);

Error_t StackHashCheck(Stack *stk);
#endif

#endif//STACK_H
