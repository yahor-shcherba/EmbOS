#include <cpu.h>
#include <traps.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define KERNEL_NULL_SELL  0x00
#define KERNEL_CODE_SELL  0x08
#define KERNEL_DATA_SELL  0x10

#define KERNEL_TYPE_CODE  0x0A
#define KERNEL_TYPE_DATA  0x02

#define GDT_ENTRIES       3
#define IDT_VECTOR_SIZE   256

#define TRAP_GATE         0x8F
#define INTR_GATE         0x8E

typedef void (*trap_handler)(void);

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

struct idt_entry {
  uint16_t handler_low;
  uint16_t sel;
  uint8_t zero;
  uint8_t flags;
  uint16_t handler_high;
} __attribute__ ((packed));

struct pseudo_desc {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

struct trapframe {
  uint32_t fs, ds, es;
  uint32_t ebp, edi, esi, edx, ecx, ebx, eax;
  uint32_t exception_number, error_code;
  uint32_t eip, cs, eflags, esp, ss;
};

struct gdt_entry gdt_table[GDT_ENTRIES];
struct idt_entry idt_table[IDT_VECTOR_SIZE];
struct pseudo_desc gdt_desc;
struct pseudo_desc idt_desc;

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
intr_exception_main(struct trapframe *tp)
{
  printf("%s :: error code %d\n",
    get_exception_message(tp->exception_number), tp->error_code);

  printf("eax : %p ebx : %p ecx : %p edx : %p\n",
    tp->eax, tp->ebx, tp->ecx, tp->edx);

  printf("esi : %p edi : %p ebp : %p esp : %p\n",
    tp->esi, tp->edi, tp->ebp, tp->esp);

  printf("cs  : %p ds  : %p ss  : %p es  : %p\n",
    tp->cs, tp->ds, tp->ss, tp->es);

  printf("fs  : %p gs  : %p eip : %p efl : %p\n",
    tp->fs, tp->ds, tp->eip, tp->eflags);
}

static inline void
load_idt(struct pseudo_desc *desc)
{
  __asm__ volatile
  (
    "lidt (%0)" : : "a" (desc)
  );
}

static inline void
idt_set_entry(int num_entry, trap_handler handler, uint8_t flags)
{
  idt_table[num_entry].handler_low  = (uint32_t) handler & 0xFFFF;
  idt_table[num_entry].handler_high = ((uint32_t) handler >> 16) & 0xFFFF;
  idt_table[num_entry].flags        = flags;
  idt_table[num_entry].sel          = KERNEL_CODE_SELL;
  idt_table[num_entry].zero         = 0;
}

static inline void
init_trap_exceptions(void)
{
  idt_set_entry(TRAP_DIV_ERROR, &trap_exception_0, TRAP_GATE);
  idt_set_entry(TRAP_DBG_EXC, &trap_exception_1, TRAP_GATE);
  idt_set_entry(TRAP_NMI, &trap_exception_2, TRAP_GATE);
  idt_set_entry(TRAP_BREAK_PNT, &trap_exception_3, TRAP_GATE);
  idt_set_entry(TRAP_OVERFLOW, &trap_exception_4, TRAP_GATE);
  idt_set_entry(TRAP_BOUND_RAGE, &trap_exception_5, TRAP_GATE);
  idt_set_entry(TRAP_UD2_EXC, &trap_exception_6, TRAP_GATE);
  idt_set_entry(TRAP_DEV_EXC, &trap_exception_7, TRAP_GATE);
  idt_set_entry(TRAP_DBL_FAULT, &trap_exception_8, TRAP_GATE);
  idt_set_entry(TRAP_COP_SEG, &trap_exception_9, TRAP_GATE);
  idt_set_entry(TRAP_INVLD_TSS, &trap_exception_10, TRAP_GATE);
  idt_set_entry(TRAP_SNP, &trap_exception_11, TRAP_GATE);
  idt_set_entry(TRAP_STACK_FAULT, &trap_exception_12, TRAP_GATE);
  idt_set_entry(TRAP_GNR_PROT, &trap_exception_13, TRAP_GATE);
  idt_set_entry(TRAP_PAGE_FAULT, &trap_exception_14, TRAP_GATE);
  idt_set_entry(TRAP_RESERVE, &trap_exception_15, TRAP_GATE);
  idt_set_entry(TRAP_FPU_ERROR, &trap_exception_16, TRAP_GATE);
  idt_set_entry(TRAP_ALIGMENT_CHK, &trap_exception_17, TRAP_GATE);
  idt_set_entry(TRAP_MACHINE_CHK, &trap_exception_18, TRAP_GATE);
  idt_set_entry(TRAP_SIMD_EXC, &trap_exception_19, TRAP_GATE);
  idt_set_entry(TRAP_VIRT_EXC, &trap_exception_20, TRAP_GATE);
}

static inline void
idt_setup(void)
{
  idt_desc.base = (uint32_t) &idt_table;
  idt_desc.limit = sizeof(struct idt_entry) * IDT_VECTOR_SIZE - 1; 

  for (int i = 0; i < IDT_VECTOR_SIZE; i++)
    memset(&idt_table[i], 0, sizeof(struct idt_entry));

  init_trap_exceptions();

  load_idt(&idt_desc);
}

extern void
cpu_setup(void)
{
  gdt_setup();
  idt_setup();
}
