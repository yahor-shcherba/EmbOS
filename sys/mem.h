#ifndef _MEM_H
#define _MEM_H
#include <stddef.h>

void kmem_setup(void);

void *kmalloc(size_t size);
void *kfree(void *p);

void mem_dump(void);

#endif /* NOT _MEM_H */
