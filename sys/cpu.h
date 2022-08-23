#ifndef _CPU_H
#define _CPU_H

#define CPU_EFLAG_IF  0x0200

void cpu_setup(void);

static inline void
cli(void)
{
  __asm__ volatile ("cli");
}

static inline void
sti(void)
{
  __asm__ volatile ("sti");
}

static inline void
hlt(void)
{
  __asm__ volatile ("hlt");
}

static inline unsigned int
get_eflags(void)
{
  unsigned int eflags;

  __asm__ volatile ("pushf; pop %0" : "=r" (eflags));

  return eflags;
}

static inline void
set_eflags(int eflags)
{
  __asm__ volatile("push %0; popf;" : : "a" (eflags));
}

static inline void
intr_save(unsigned int *eflags)
{
  __asm__ volatile
  (
    "cli; pushf; pop (%0)" : : "a" (eflags)
  );
}

static inline void
intr_restore(unsigned int eflags)
{
  __asm__ volatile
  (
    "push %0; popf; sti;" : : "a" (eflags)
  );
}

static inline int
intr_enabled(void)
{
  return get_eflags() & CPU_EFLAG_IF;
}

#endif /* NOT _CPU_H */
