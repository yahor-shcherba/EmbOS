#include <i8254.h>
#include <i8259.h>
#include <io.h>
#include <stdio.h>
#include <stdint.h>
#include <thread.h>

#define DIV_CEIL(n, d) (((n) + (d) - 1) / (d))

#define I8254_FREQ        1193182
#define TIMER_FREQ        100

#define PORT_CHANNEL_0    0x40
#define PORT_MODE         0x43

#define MODE_RATE_GEN     0x04
#define MODE_BINARY       0x00
#define MODE_RW_LSB       0x10
#define MODE_RW_MSB       0x20
#define MODE_COUNTER_0    0x00

#define INIT_COMMAND \
  (MODE_BINARY | MODE_RATE_GEN | MODE_RW_LSB | MODE_RW_MSB | MODE_COUNTER_0)

static void
tick(void)
{
  outb(0x20, 0x20);
  thread_yield();
}

extern void
i8254_setup(void)
{
  uint16_t word = (I8254_FREQ + TIMER_FREQ - 1) / TIMER_FREQ;
  outb(PORT_MODE, INIT_COMMAND);

  outb(PORT_CHANNEL_0, word & 0xFF);
  outb(PORT_CHANNEL_0, word >> 8);

  irq_set_handler(IRQ_TIMER, tick);
}
