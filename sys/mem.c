#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define MEM_SIZE (1024 * 1024 * 16)

static volatile uint8_t memory[MEM_SIZE];

typedef struct header {
  size_t size;
  struct header *next;
  uint32_t flags;
  uint32_t pad;
} header_t;

header_t *freep = (header_t*) memory;

#define B2UNITS(x) (((x) + sizeof(header_t) - 1) / sizeof(header_t) + 1)

extern void
kmem_setup(void)
{
  size_t nunits = B2UNITS(MEM_SIZE) - 1;
  freep->next = freep;
  freep->size = nunits;
}

extern void
mem_dump(void)
{
  size_t total_units = 0;
  header_t *p = (header_t*) memory;

  while (total_units != B2UNITS(MEM_SIZE) - 1)
    {
      printf("chunk %p size %d is ", (void*) p, p->size);

      if (p->next == NULL)
        printf("allocated\n");
      else
        printf("free\n");

      total_units += p->size;
      p += p->size;
    }
}

extern void*
kmalloc(size_t nbytes)
{
  size_t nunits = B2UNITS(nbytes);
  header_t *p = freep;

  do
    {
      if (p->size >= nunits)
        {
          if (p->size == nunits)
            freep = p->next == freep ? NULL : p->next;
          else
            {
              p->size -= nunits;
              p += p->size;
              p->size = nunits;
            }

          p->next = NULL;
          return (void*) (p + 1);
        }

      p = p->next;
    }
  while (p != freep);

  return NULL;
}


extern void
kfree(void *ptr)
{
  header_t *bp = (header_t*) ptr - 1;
  header_t *p = freep;

  if (freep == NULL)
    {
      freep = bp;
      freep->next = freep;
      return;
    }

  while (!(bp > p && bp < p->next))
    {
      if (p >= p->next && (bp > p || bp < p->next))
        break;

      p = p->next;
    }

  bp->next = p->next;
  p->next = bp;

  if (bp + bp->size == bp->next)
    {
      bp->size += bp->next->size;
      bp->next = bp->next->next;
    }

  if (p + p->size == p->next)
    {
      p->size += p->next->size;
      p->next = p->next->next;
    }
}
