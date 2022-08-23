#ifndef _I8259_H
#define _I8259_H

#define IRQ_LINES       16

#define IRQ_TIMER       0
#define IRQ_KEYBOARD    1
#define IRQ_SLAVE       2
#define IRQ_COM2        3
#define IRQ_COM1        4
#define IRQ_LPT2_PORT   5
#define IRQ_FLOPPY_DEV  6
#define IRQ_LPT1_PORT   7
#define IRQ_CMOS        8
#define IRQ_PNP_1       9
#define IRQ_PNP_2       10
#define IRQ_PNP_3       11
#define IRQ_MOUSE       12
#define IRQ_COPROCESSOR 13
#define IRQ_HARD_1      14
#define IRQ_HARD_2      15

#define MASTER_VECTOR_OFFSET  0x20
#define SLAVE_VECTOR_OFFSET   0x28

typedef void (*irq_handler)(void);

void i8259_setup(void);
void irq_set_handler(int irq_line, void (*handler)(void));

void irq_enable(int irq_line);
void irq_disable(int irq_line);


#endif /* NOT _I8259_H */
