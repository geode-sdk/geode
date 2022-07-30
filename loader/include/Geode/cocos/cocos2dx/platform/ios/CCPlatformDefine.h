#ifndef __CCPLATFORMDEFINE_H__
#define __CCPLATFORMDEFINE_H__

#include <assert.h>

#ifdef GEODE_EXPORTING
    #define CC_DLL __attribute__((visibility("default")))
#else
    #define CC_DLL 
#endif

#define CC_ASSERT(cond) assert(cond)


#define CC_UNUSED_PARAM(unusedparam) (void)unusedparam

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif



#endif /* __CCPLATFORMDEFINE_H__*/
