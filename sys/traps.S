.section .text
.code32

#define TRAP_EXCEPTION(number)        \
.globl trap_exception_##number;       \
trap_exception_##number:              \
  push $0;                            \
  push $number;                       \
  jmp trap_exception_common;

#define TRAP_EXCEPTION_ERROR(number)  \
.globl trap_exception_##number;       \
trap_exception_##number:              \
  push $number;                       \
  jmp trap_exception_common

#define IRQ_ROUTINE(number)           \
.globl intr_request_##number;         \
intr_request_##number:                \
  push $number;                       \
  call irq_main;                      \
  add $0x4, %esp;                     \
  iret

trap_exception_common:
  push %eax
  push %ebx
  push %ecx
  push %edx
  push %esi
  push %edi
  push %ebp

  push %es
  push %ds
  push %fs

  push %esp
  cli
  call intr_exception_main
  hlt
  iret

TRAP_EXCEPTION(0)           /* Divide Error */
TRAP_EXCEPTION(1)           /* Debug Exception*/
TRAP_EXCEPTION(2)           /* NMI Interrupt */
TRAP_EXCEPTION(3)           /* Breakpoint */
TRAP_EXCEPTION(4)           /* Overflow */
TRAP_EXCEPTION(5)           /* BOUND Range Exceeded */
TRAP_EXCEPTION(6)           /* Invalid opcode */
TRAP_EXCEPTION(7)           /* Device not avaliable */
TRAP_EXCEPTION_ERROR(8)     /* Dobule fault */
TRAP_EXCEPTION(9)           /* Coprocessor Segment Ovverun */
TRAP_EXCEPTION_ERROR(10)    /* Invalid TSS */
TRAP_EXCEPTION_ERROR(11)    /* Segment Not Present */
TRAP_EXCEPTION_ERROR(12)    /* Stack Segment Fault */
TRAP_EXCEPTION_ERROR(13)    /* General Protection */
TRAP_EXCEPTION_ERROR(14)    /* Page Fault */
TRAP_EXCEPTION(15)          /* reserved */
TRAP_EXCEPTION(16)          /* x87 FPU floating-point error */
TRAP_EXCEPTION_ERROR(17)    /* Aligned check */
TRAP_EXCEPTION(18)          /* Machine Check */
TRAP_EXCEPTION(19)          /* SIMD Floationg-point Exception */
TRAP_EXCEPTION(20)          /* Virtualization Exception */

IRQ_ROUTINE(0)              /* timer */
IRQ_ROUTINE(1)              /* keyboard */
IRQ_ROUTINE(2)              /* slave pic */
IRQ_ROUTINE(3)              /* com2 port */
IRQ_ROUTINE(4)              /* com1 port */
IRQ_ROUTINE(5)              /* lpt2 port */
IRQ_ROUTINE(6)              /* floppy device */
IRQ_ROUTINE(7)              /* lpt1 port */
IRQ_ROUTINE(8)              /* cmos */
IRQ_ROUTINE(9)              /* PNP1 port */
IRQ_ROUTINE(10)             /* PNP2 port */
IRQ_ROUTINE(11)             /* PNP3 port */
IRQ_ROUTINE(12)             /* mouse */
IRQ_ROUTINE(13)             /* coprocessor*/
IRQ_ROUTINE(14)             /* hard1 */
IRQ_ROUTINE(15)             /* hard2 */
