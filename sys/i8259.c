#include <i8259.h>
#include <io.h>
#include <stdint.h>

#define MASTER_PORT           0x20
#define SLAVE_PORT            0xA0

#define MASTER_DATA           0x21
#define SLAVE_DATA            0x22

#define ICW1_ICW4             0x01  /* ICW4 Disabled */
#define ICW1_INIT             0x10  /* initialization command */

#define COMMAND_EOI           0x20

#define ICW4_8086             0x01  /* 8086/88 (MCS 80/85) MODE */

static irq_handler irq_handlers[IRQ_LINES];

extern void
irq_main(int irq_line)
{
  irq_handlers[irq_line]();

  if (irq_line > 7)
    outb(SLAVE_PORT, COMMAND_EOI);

  outb(MASTER_PORT, COMMAND_EOI);
}

static void
default_handler_irq(void)
{
  __asm__ volatile ("nop");
}

extern void
i8259_setup(void)
{
  outb(MASTER_PORT, ICW1_INIT | ICW1_ICW4);
  outb(SLAVE_PORT, ICW1_INIT | ICW1_ICW4);

  outb(MASTER_DATA, MASTER_VECTOR_OFFSET);
  outb(SLAVE_DATA, SLAVE_VECTOR_OFFSET);

  outb(MASTER_DATA, 4);
  outb(SLAVE_DATA, 2);

  outb(MASTER_DATA, ICW4_8086);
  outb(SLAVE_DATA, ICW4_8086);

  outb(MASTER_DATA, 0x0);
  outb(SLAVE_DATA, 0x0);

  for (int i = 0; i < IRQ_LINES; i++)
    irq_handlers[i] = &default_handler_irq;
}

extern void
irq_set_handler(int irq_line, irq_handler handler)
{
  irq_handlers[irq_line] = handler;
}

extern void
irq_enable(int irq_line)
{
  uint8_t port = MASTER_PORT;

  if (irq_line >= 8)
    {
      port = SLAVE_PORT;
      irq_line -= 8;
    }

  outb(port, inb(port) | (1 << irq_line));
}

extern void
irq_disable(int irq_line)
{
  uint8_t port = MASTER_PORT;

  if (irq_line >= 8)
    {
      port = SLAVE_PORT;
      irq_line -= 8;
    }

  outb(port, inb(port) & ~(1 << irq_line));
}
