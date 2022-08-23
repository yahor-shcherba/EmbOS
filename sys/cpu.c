#include <cpu.h>
#include <stdint.h>

#define KERNEL_NULL_SELL  0x00
#define KERNEL_CODE_SELL  0x08
#define KERNEL_DATA_SELL  0x10

#define KERNEL_TYPE_CODE  0x0A
#define KERNEL_TYPE_DATA  0x02

#define GDT_ENTRIES       3

struct gdt_entry {
  uint32_t limit_low : 16;
  uint32_t base_low : 16;
  uint32_t base_middle : 8;
  uint32_t type : 4;
  uint32_t s : 1;
  uint32_t dpl : 2;
  uint32_t p : 1;
  uint32_t limit_high : 4;
  uint32_t avl : 1;
  uint32_t l : 1;
  uint32_t d : 1;
  uint32_t g : 1;
  uint32_t base_high : 8;
} __attribute__((packed));

struct pseudo_desc {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

struct gdt_entry gdt_table[GDT_ENTRIES] = { 0 };
struct pseudo_desc gdt_desc = { 0, 0 };

static inline void
load_gdt(struct pseudo_desc *desc)
{
  __asm__ volatile
  (
    "lgdt (%0);"
    "mov %2, %%eax;"
    "mov %%eax, %%gs;"
    "mov %%eax, %%fs;"
    "mov %%eax, %%es;"
    "mov %%eax, %%ds;"
    "mov %%eax, %%ss;"
    "ljmp %1, $.L0;"
    ".L0:;"
    : : "a" (desc), "i" (KERNEL_CODE_SELL), "i" (KERNEL_DATA_SELL)
  );
}

static inline void
gdt_set_entry(uint32_t num_entry, uint32_t base, uint32_t limit, uint8_t type)
{
  gdt_table[num_entry].limit_low    = limit & 0xFFFF;
  gdt_table[num_entry].base_low     = base & 0xFFFF;
  gdt_table[num_entry].base_middle  = (base >> 16) & 0xFF;
  gdt_table[num_entry].type         = type;
  gdt_table[num_entry].s            = 1;
  gdt_table[num_entry].dpl          = 0;
  gdt_table[num_entry].p            = 1;
  gdt_table[num_entry].limit_high   = (limit >> 16) & 0xF;
  gdt_table[num_entry].avl          = 0;
  gdt_table[num_entry].l            = 0;
  gdt_table[num_entry].d            = 1;
  gdt_table[num_entry].g            = 1;
  gdt_table[num_entry].base_high    = (base >> 24) & 0xF;
}

static inline void
gdt_setup(void)
{
  gdt_set_entry(0, 0, 0, 0);                         /* NULL DESCRIPTOR */
  gdt_set_entry(1, 0, 0xFFFFFF, KERNEL_TYPE_CODE);   /* KERNEL CODE */
  gdt_set_entry(2, 0, 0xFFFFFF, KERNEL_TYPE_DATA);   /* KERNEL DATA */

  gdt_desc.base = (uint32_t) &gdt_table;
  gdt_desc.limit = sizeof(struct gdt_entry) * GDT_ENTRIES - 1;

  load_gdt(&gdt_desc);
}

extern void
cpu_setup(void)
{
  gdt_setup();
}
