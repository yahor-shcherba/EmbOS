.section .text
.code32

.globl thread_load_context
thread_load_context:
  mov 4(%esp), %eax       /* load thread address */
  mov (%eax), %esp        /* setup a thread stack */

  jmp thread_restore_context

.globl thread_switch_context
thread_switch_context:
  mov 4(%esp), %eax       /* prev thread */
  mov 8(%esp), %ecx       /* next thread */

  push %ebp
  push %ebx
  push %edi
  push %esi

  mov %esp, (%eax)        /* save prev stack */
  mov (%ecx), %esp        /* switch to the next stack of the thread */

thread_restore_context:
  pop %esi
  pop %edi
  pop %ebx
  pop %ebp

  ret 
