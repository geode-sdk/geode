#pragma once

// _ThrowInfo and all of those other structs are hardcoded into MSVC,
// but don't exist in other compilers like Clang, causing <ehdata.h> to not compile.
//
// We check if they don't exist and define them manually.
// source: https://www.geoffchappell.com/studies/msvc/language/predefined/index.htm

#ifndef _ThrowInfo

typedef struct _PMD
{
    int mdisp;
    int pdisp;
    int vdisp;
} _PMD;

typedef void (*_PMFN) (void);

#pragma warning (disable:4200)
#pragma pack (push, _TypeDescriptor, 8)
typedef struct _TypeDescriptor
{
    const void *pVFTable;
    void *spare;
    char name [];
} _TypeDescriptor;
#pragma pack (pop, _TypeDescriptor)
#pragma warning (default:4200)

typedef const struct _s__CatchableType {
    unsigned int properties;
    _TypeDescriptor *pType;
    _PMD thisDisplacement;
    int sizeOrOffset;
    _PMFN copyFunction;
} _CatchableType;

#pragma warning (disable:4200)
typedef const struct _s__CatchableTypeArray {
    int nCatchableTypes;
    _CatchableType *arrayOfCatchableTypes [];
} _CatchableTypeArray;
#pragma warning (default:4200)

typedef const struct _s__ThrowInfo {
    unsigned int attributes;
    _PMFN pmfnUnwind;
    int (__cdecl *pForwardCompat) (...);
    _CatchableTypeArray *pCatchableTypeArray;
} _ThrowInfo;

#endif // _ThrowInfo

#include <ehdata.h>