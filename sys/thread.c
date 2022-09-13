#include <thread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cpu.h>
#include <i8259.h>
#include <stddef.h>
#include <stdint.h>
#include <panic.h>
#include <assert.h>

#define IDLE_STACK_SIZE     512

struct shed_runq {
  struct thread *current;
  struct thread *idle;
};

struct shed_runq *runq;

extern void thread_load_context(struct thread *thread) __attribute__((noreturn));
extern void thread_switch_context(struct thread *prev, struct thread *next);

static void
thread_idle(void *arg)
{
  (void) arg;

  for (;;)
    ;
}

extern void
thread_main(void (*start_routine)(void*), void *arg)
{
  assert(start_routine);

  sti();
  start_routine(arg);
  thread_exit();
}

static bool
shed_runq_queue_empty(void)
{
  if (runq->current == NULL || runq->current == runq->idle)
    return true;

  return false;
}

static struct thread*
shed_runq_get_current(void)
{
  return runq->current;
}

static struct thread*
shed_runq_get_next(void)
{
  struct thread *thread = NULL;

  if (shed_runq_queue_empty())
    thread = runq->idle;
  else
    thread = runq->current->next;

  runq->current = thread;

  return runq->current;
}

static void
shed_runq_add(struct thread *thread)
{
  if (shed_runq_queue_empty())
    {
      runq->current = thread;
      return;
    }

  struct thread *prev = runq->current->prev;
  struct thread *next = runq->current;

  prev->next = thread;
  thread->prev = prev;

  next->prev = thread;
  thread->next = next;
}

static void
shed_runq_remove(struct thread *thread)
{
  if (thread == thread->prev)
    {
      runq->current = NULL;
      return;
    }

  struct thread *prev = thread->prev;
  struct thread *next = thread->next;

  prev->next = next;
  next->prev = prev;
}

static void
thread_set_state(struct thread *thread, enum state state)
{
  thread->state = state;
}

static bool
thread_is_running(struct thread *thread)
{
  return thread->state == RUNNING;
}

static bool
thread_is_dead(struct thread *thread)
{
  return thread->state == DEAD;
}

static void
thread_set_name(struct thread *thread, const char *name)
{
  strncpy(thread->name, name, THREAD_NAME_LENGTH);
}

const char*
thread_name(struct thread *thread)
{
  return thread->name;
}

extern struct thread*
thread_self(void)
{
  return runq->current;
}

extern void
__attribute__((noreturn))
sheduler_enable(void)
{
  struct thread *thread = shed_runq_get_next();
  thread_load_context(thread);
}

extern void
shedule(void)
{
  struct thread *prev = shed_runq_get_current();
  uint32_t eflags = 0;

  if (prev != NULL && !thread_is_running(prev))
    shed_runq_remove(prev);

  struct thread *next = shed_runq_get_next();

  assert(next);

  if (prev != next)
    thread_switch_context(prev, next);
}

static void
stack_init(struct thread *thread, size_t stack_size,
           void (*start_routine)(void*), void *arg)
{
  uint32_t *sp = (uint32_t*)((char*)thread->stack + stack_size);

  *sp-- = (uint32_t) arg;           /* 2st argument */
  *sp-- = (uint32_t) start_routine; /* 1st argument */
  *sp-- = 0;                        /* return address */
  *sp-- = (uint32_t) thread_main;   /* thread main */
  *sp-- = 0;                        /* ebp */
  *sp-- = 0;                        /* ebx */
  *sp-- = 0;                        /* edi */
  *sp = 0;                          /* esi */

  thread->sp = sp;
}

static void
thread_init(struct thread *thread, void *stack, size_t stack_size,
            const char *thread_name, void (*start_routine)(void*), void *arg)
{
  thread->stack = stack;
  stack_init(thread, stack_size, start_routine, arg);
  thread_set_name(thread, thread_name);
  thread_set_state(thread, RUNNING);


  thread->prev = thread;
  thread->next = thread;
}

extern int
thread_create(struct thread **thread, size_t stack_size, const char *thread_name,
              void (*start_routine)(void*), void *arg)
{
  *thread = (struct thread*) malloc(sizeof(*thread));

  if (*thread == NULL)
    return -1;

  void *stack = malloc(stack_size);

  if (stack == NULL)
    {
      free(thread);
      return -1;
    }

  thread_init(*thread, stack, stack_size, thread_name, start_routine, arg);
  shed_runq_add(*thread);

  return 0;
}

static struct thread*
create_idle()
{
  struct thread *thread = (struct thread*) malloc(sizeof(*thread));

  if (thread == NULL)
    panic("idle thread: unable to create thread");

  void *stack = malloc(IDLE_STACK_SIZE);

  if (stack == NULL)
    panic("idle thread: unuble to create stack");

  thread_init(thread, stack, IDLE_STACK_SIZE, "idle", thread_idle, NULL);

  return thread;
}

extern void
thread_setup(void)
{
  runq = (struct shed_runq*) malloc(sizeof(*runq));
  runq->idle = create_idle();
  runq->current = NULL;
}

extern void
thread_yield(void)
{
  shedule();
}

extern void
thread_sleep(void)
{
  struct thread *thread = thread_self();
  uint32_t eflags = 0;

  intr_save(&eflags);
  assert(thread_is_running(thread));
  thread_set_state(thread, SLEEPING);
  shedule();
  assert(thread_is_running(thread));
  intr_restore(eflags);
}

extern void
thread_wakeup(struct thread *thread)
{
  uint32_t eflags = 0;

  if (!thread || (thread == thread_self()))
    return;

  intr_save(&eflags);

  if (!thread_is_running(thread))
    {
      assert(!thread_is_dead(thread));
      thread_set_state(thread, RUNNING);
      shed_runq_add(thread);
    }

  intr_restore(eflags);
}

extern void
thread_destroy(struct thread *thread)
{
  free(thread->stack);
  free(thread);
}

extern void
thread_exit(void)
{
  struct thread *thread = thread_self();
  uint32_t eflags;
  intr_save(&eflags);
  assert(thread_is_running(thread));
  thread_set_state(thread, DEAD);
  thread_wakeup(thread->joiner);
  thread_destroy(thread);
  shedule();

  panic("thread: dead thread is running!\n");
}

extern void
thread_join(struct thread *thread)
{
  uint32_t eflags = 0;
  intr_save(&eflags);

  thread->joiner = thread_self();

  while (!thread_is_dead(thread))
    thread_sleep();

  intr_restore(eflags);
}
