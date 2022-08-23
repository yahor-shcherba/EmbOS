#include <uart.h>
#include <mem.h>
#include <stdio.h>

void
main(void)
{
  kmem_setup(); /* setup a memory allocater */
  uart_setup(); /* setup a serial port */


  printf("Welcome to the EmbOS...\n");

  while (1)
    ;
}
