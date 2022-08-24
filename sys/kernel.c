#include <cpu.h>
#include <i8259.h>
#include <i8254.h>
#include <uart.h>
#include <mem.h>
#include <stdio.h>

void
main(void)
{
  kmem_setup();   /* setup a memory allocater */
  i8259_setup();  /* setup a pic (i8259A) controller */
  i8254_setup();  /* setup a pit (i8254) controller */
  cpu_setup();    /* setup a gdt and idt */
  uart_setup();   /* setup a serial port */
 
  printf("Welcome to the EmbOS...\n");
  sti();

  while (1)
    ;
}
