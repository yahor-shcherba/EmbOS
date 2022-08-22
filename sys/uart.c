#include <uart.h>
#include <io.h>

#define UART_CLOCK  115200
#define BAUD_RATE   57600
#define DIVISOR (UART_CLOCK / BAUD_RATE)

#define COM1_PORT   0x3F8   /* COM1 port */

#define DLAB        0x80
#define REG_LCR     3       /* line control register */
#define REG_LSR     5       /* line status register */

#define BIT_MODE    3       /* character length 8 bits */
#define PARITY_NONE 0       /* no parity bit will be added and non will be expected */
#define STOP_BIT    0       /* send 1 bit after each character data */

#define REG_DIVL    0       /* register low divisor */
#define REG_DIVH    1       /* register high divisor */

#define REG_IER     1       /* interrupt enable register */
#define IER_DATA    1       /* transmitter empty */

#define REG_DATA    0       /* data register */

#define EMPTY       0x20    /* trasmitted empty*/
#define READY       0x01    /* data ready */


extern void
uart_setup(void)
{
  outb(COM1_PORT + REG_LSR,  DLAB);           /* set baud rate dvisior */
  outb(COM1_PORT + REG_DIVL, DIVISOR & 0xF);  /* set divisor low byte */
  outb(COM1_PORT + REG_DIVH, DIVISOR >> 8);   /* set divisor high byte */
  outb(COM1_PORT + REG_LCR, BIT_MODE | PARITY_NONE | STOP_BIT);
  outb(COM1_PORT + REG_IER, IER_DATA);
}

extern void
uart_write(unsigned char byte)
{
  while (1)
    {
       unsigned char data = inb(COM1_PORT + REG_LSR);

        if (data & EMPTY)
          break;
    }

  if (byte == '\n')
    outb(COM1_PORT + REG_DATA, '\r');

  outb(COM1_PORT + REG_DATA, byte);
}
