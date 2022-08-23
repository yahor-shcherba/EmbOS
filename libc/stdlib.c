#include <stdio.h>
#include <string.h>
#include <mem.h>

extern int
atoi(const char *s)
{
  static char digit[] = "0123456789";
  int n = 0, sign = 1;

  while (*s == ' ')
    s++;

  if (*s == '-')
    {
      sign = -1;
      s++;
    }

  if (*s == '+')
    s++;

  if (strchr(digit, *s) != NULL)
    {
      n = *s++ - '0';

      while (strchr(digit, *s) != NULL)
        {
          n *= 10;
          n += *s - '0';
          s++;
        }
    }

  return n * sign;
}

extern void*
malloc(size_t size)
{
  return kmalloc(size);
}

extern void
free(void *p)
{
  kfree(p);
}
