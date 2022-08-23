#include <stdio.h>
#include <uart.h>
#include <stdarg.h>

extern int
putchar(int c)
{
  uart_write((unsigned char) c);
  return 1;
}

extern int
puts(const char *s)
{
  char *d = (char*) s;

  while (*s)
    uart_write((unsigned char) *s++);

  return (int) (s - d) + 1;
}

static int
printint(unsigned int value, int base, int sign, int field_width)
{
  static char *table = "0123456789ABCDEF";
  char buffer[64] = { 0x0 };
  int n = 0, count = 0;

  if (sign)
    value = ~value + 1;

  do
    buffer[n++] = *(value % base + table);
  while (value /= base);

  if (sign && base == 10)
    putchar('-');

  count = n;

  field_width -= n;

  while (field_width-- > 0)
    uart_write('0');

  while (n >= 0)
    uart_write(buffer[n--]);

  return count;
}

extern int
printf(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  int i_var;
  int count = 0;

  while (*fmt != '\0')
    {
      if (*fmt != '%')
        {
          uart_write(*fmt++);
          count++;
          continue;
        }

      fmt++;
      switch(*fmt++)
        {
          case 'd':
            i_var = va_arg(ap, int);
            count += printint(i_var, 10, i_var < 0, 0);
            break;

          case 'x':
            count += printint(va_arg(ap, unsigned int), 16, 0, 0);
            break;
            
          case 'p':
            puts("0x");
            count += printint(va_arg(ap, unsigned int), 16, 0, 8);
            break;

          case 's':
            count += puts(va_arg(ap, const char*));
            break;
          
          case 'c':
            count += putchar(va_arg(ap, unsigned int));
            break;

          default:
            count++;
            putchar('%');
        }
    }

  va_end(ap);

  return count;
}
