#ifndef _PANIC_H
#define _PANIC_H

#include <cpu.h>
#include <stdio.h>

static inline void
panic(const char *s)
{
  printf("%s", s);
  cli();

  for (;;)
    ;
}

#endif /* NOT _PANIC_H */
