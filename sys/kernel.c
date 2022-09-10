#include <cpu.h>
#include <i8259.h>
#include <i8254.h>
#include <uart.h>
#include <mem.h>
#include <thread.h>
#include <stdio.h>

void
test1(void *arg)
{
  (void) arg;

  for (;;)
    {
      printf("thread '%s' running \n", thread_name(thread_self()));
      thread_yield();
    }
}

void
main(void)
{
  kmem_setup();       /* setup a memory allocater */
  thread_setup();     /* setup a thread module */
  i8259_setup();      /* setup a pic (i8259A) controller */
  i8254_setup();      /* setup a pit (i8254) controller */
  cpu_setup();        /* setup a gdt and idt */
  uart_setup();       /* setup a serial port */

  printf("Welcome to the EmbOS...\n");

  struct thread *t1, *t2, *t3, *t4;
  thread_create(&t1, 1024, "test1", test1, NULL);
  thread_create(&t2, 1024, "test2", test1, NULL);
  thread_create(&t3, 1024, "test3", test1, NULL);
  thread_create(&t4, 1024, "test4", test1, NULL);

  sheduler_enable();
}
