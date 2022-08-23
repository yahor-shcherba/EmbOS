#include <uart.h>
#include <stdio.h>

void
main(void)
{
  uart_setup();

  printf("hello, world\n");

  while (1)
    ;
}
