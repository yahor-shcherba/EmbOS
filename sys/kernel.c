#include <cpu.h>
#include <i8259.h>
#include <i8254.h>
#include <uart.h>
#include <mem.h>
#include <thread.h>
#include <stdio.h>

struct thread *t1, *t2;

void
test1(void *arg)
{
  (void) arg;

  thread_yield();
  printf("thread joiner\n");
  thread_exit();
}

void
main_thread(void *arg)
{
  (void) arg;

  struct thread *t1;
  thread_create(&t1, 1024, "joiner thread", test1, NULL);
  thread_join(t1);

  printf("thread '%s' running \n", thread_name(thread_self()));
  thread_exit();
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

  thread_create(&t2, 1024, "main thread", main_thread, NULL);

  sheduler_enable();
}
