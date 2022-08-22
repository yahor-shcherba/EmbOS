#ifndef _STDARG_H
#define _STDARG_H

typedef __builtin_va_list va_list;

#define va_start(ap, last_arg)  __builtin_va_start(ap, last_arg)
#define va-arg(ap, type)        __builtin_va_arg(ap, type)
#define end(ap)                 __builtin_va_end(ap)

#endif /* NOT _STDARG_H */
