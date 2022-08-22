#define MULTIBOOT_MAGIC 0x1BADB002
#define MULTIBOOT_CHECK 0x2BADB002
#define MULTIBOOT_FLAGS 0x0

#define KERNEL_STACK_SIZE 4096

.code32
.section .hdr, "a"
  .long MULTIBOOT_MAGIC
  .long MULTIBOOT_FLAGS
  .long -(MULTIBOOT_FLAGS + MULTIBOOT_MAGIC)

.section .data
.align 4
  stack: .fill KERNEL_STACK_SIZE, 1, 0

.section .text, "ax"

.globl _start
_start:
  /* setup a stack pointer */
  mov $stack + KERNEL_STACK_SIZE, %esp
  jmp main

halt:
  cli
  hlt
