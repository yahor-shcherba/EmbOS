#include <cpu.h>
#include <uart.h>
#include <mem.h>
#include <stdio.h>

void
main(void)
{
  kmem_setup(); /* setup a memory allocater */
  cpu_setup();  /* setup a gdt and idt */
  uart_setup(); /* setup a serial port */

  printf("Welcome to the EmbOS...\n");

  while (1)
    ;
}
