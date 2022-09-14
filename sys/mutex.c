#include <thread.h>
#include <cpu.h>
#include <stdint.h>
#include <stdlib.h>
#include <panic.h>

extern void
thread_mutex_init(struct mutex *mutex)
{
  mutex->locked = false;
  mutex->waiters = NULL;
}

static void
mutex_set_lock(struct mutex *mutex)
{
  mutex->locked = true;
}

static void
mutex_clear_lock(struct mutex *mutex)
{
  mutex->locked = false;
}

static struct mutex_waiter*
create_waiter(struct thread* thread)
{
  struct mutex_waiter *waiter = (struct mutex_waiter*) malloc(sizeof(*waiter));

  if (waiter == NULL)
    panic("mutex: unable to create mutex waiter\n");

  waiter->thread = thread;
  waiter->next = waiter;
  waiter->prev = waiter;

  return waiter;
}

static void
mutex_add_waiters(struct mutex *mutex, struct thread *thread)
{
  struct mutex_waiter *waiter = create_waiter(thread);

  if (mutex->waiters == NULL)
    {
      mutex->waiters = waiter;
      return;
    }

  struct mutex_waiter *prev = mutex->waiters;
  struct mutex_waiter *next = mutex->waiters->next;

  prev->next = waiter;
  waiter->prev = prev;

  next->prev = waiter;
  waiter->next = next;
}

static struct thread*
mutex_remove_waiters(struct mutex *mutex)
{
  struct thread *thread = NULL;

  if (mutex->waiters == NULL)
    return NULL;

  if (mutex->waiters->prev == mutex->waiters)
    {
      thread = mutex->waiters->thread;
      mutex->waiters = NULL;
    }
  else
    {
      struct mutex_waiter *waiter = mutex->waiters;
      struct mutex_waiter *prev = waiter->prev;
      thread = waiter->thread;

      prev->next = waiter->next;
      waiter->next->prev = prev;
    }

  return thread;
}

extern void
thread_mutex_lock(struct mutex *mutex)
{
  struct thread *thread = thread_self();

  if (mutex->locked)
    {
      mutex_add_waiters(mutex, thread);
      thread_sleep();
    }

  mutex_set_lock(mutex);
}

extern int
thread_mutex_trylock(struct mutex *mutex)
{
  if (mutex->locked)
    {
      mutex_add_waiters(mutex, thread_self());
      return -1;
    }

  mutex_set_lock(mutex);
  return 0;
}

extern void
thread_mutex_unlock(struct mutex *mutex)
{
  uint32_t eflags = 0;
  intr_save(&eflags);
  struct thread *thread = mutex_remove_waiters(mutex);
  mutex_clear_lock(mutex);
  intr_restore(eflags);
  thread_wakeup(thread);
}
