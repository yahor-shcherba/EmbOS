#ifndef _ASSERT_H
#define _ASSERT_H

#include <cpu.h>

#ifdef NDEBUG

#define assert(expr) ((void) (expr))

#else

#define assert(expr)                                \
{                                                   \
  if (__builtin_expect(!!(!(expr)), 0))             \
    {                                               \
      cli();                                        \
      printf("%s:%d: %s: Assertion '%s' failed.",   \
        __FILE__, __LINE__, __func__, #expr );      \
      for (;;)                                      \
        ;                                           \
    }\
}


#endif /* NDEBUG */
#endif /* NOT _ASSERT_H */
