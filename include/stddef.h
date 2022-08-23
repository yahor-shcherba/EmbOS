#ifndef _STDDEF_H
#define _STDDEF_H

typedef unsigned long size_t;

#define NULL (void*) 0

#define offsetof(type, member) __builtin_offsetof(type, member)

#endif /* NOT _STDDEF_H */
