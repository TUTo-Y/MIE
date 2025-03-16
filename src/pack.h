#ifndef PACK_H
#define PACK_H

#include <wchar.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
// MSVC 编译器
__declspec(align(16)) typedef struct
#else
// GCC/Clang 编译器
typedef struct __attribute__((aligned(16)))
#endif
{
    wchar_t name[0x10];
    wchar_t age[0x4];
    wchar_t state[0x10];
    wchar_t advice[0x20];
} PACK;

typedef struct
{
    PACK pack;
    
    char alignment[0x10];
}PACKbuf;


void packInit(PACK *pack);

static inline void packSetName(PACK *pack, wchar_t *name)
{
    wcsncpy(pack->name, name, 0x10);
}
static inline void packSetAge(PACK *pack, wchar_t *age)
{
    wcsncpy(pack->age, age, 0x4);
}
static inline void packSetState(PACK *pack, wchar_t *state)
{
    wcsncpy(pack->state, state, 0x10);
}
static inline void packSetAdvice(PACK *pack, wchar_t *advice)
{
    wcsncpy(pack->advice, advice, 0x20);
}

#endif // PACK_H