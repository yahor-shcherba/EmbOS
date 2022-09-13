#ifndef _THREAD_H
#define _THREAD_H

#include <stddef.h>
#include <stdint.h>

#define THREAD_NAME_LENGTH  30

enum state {
  RUNNING,
  SLEEPING,
  DEAD
};

struct thread {
  void *sp;
  void *stack;
  uint32_t stack_size;
  char name[THREAD_NAME_LENGTH];
  enum state state;
  struct thread *joiner;

  struct thread *next;
  struct thread *prev;
};

int thread_create(struct thread **thread, size_t stack_size, const char *thread_name,
                  void (*start_routine)(void*), void *arg);

void thread_setup(void);

void thread_yield(void);
void thread_sleep(void);
void thread_wakeup(struct thread *thread);

void thread_join(struct thread *thread);
void thread_exit(void);

void sheduler_enable(void) __attribute__((noreturn));
void shedule(void);

const char *thread_name(struct thread* thread);
struct thread *thread_self(void);

#endif /* NOT _THREAD_H */
