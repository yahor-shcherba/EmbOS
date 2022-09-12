#ifndef _THREAD_H
#define _THREAD_H

#include <stddef.h>

struct thread;

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
