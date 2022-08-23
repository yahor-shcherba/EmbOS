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
