#ifndef _TRAPS_H
#define _TRAPS_H

#define TRAP_DIV_ERROR    0
#define TRAP_DBG_EXC      1
#define TRAP_NMI          2
#define TRAP_BREAK_PNT    3
#define TRAP_OVERFLOW     4
#define TRAP_BOUND_RAGE   5
#define TRAP_UD2_EXC      6
#define TRAP_DEV_EXC      7
#define TRAP_DBL_FAULT    8
#define TRAP_COP_SEG      9
#define TRAP_INVLD_TSS    10
#define TRAP_SNP          11
#define TRAP_STACK_FAULT  12
#define TRAP_GNR_PROT     13
#define TRAP_PAGE_FAULT   14
#define TRAP_RESERVE      15
#define TRAP_FPU_ERROR    16
#define TRAP_ALIGMENT_CHK 17
#define TRAP_MACHINE_CHK  18
#define TRAP_SIMD_EXC     19
#define TRAP_VIRT_EXC     20


static const char *exception_msg[] = {
  "#DE Divide Error Exception",
  "#DB Debug Exception",
  "NMI Interrupt",
  "#BP Breakpoint Exception",
  "#OF Overflow Exception",
  "#BR BOUNT Range Exceeded Exception",
  "#UD Invalid Opcode Exception",
  "#NM Device Not Available Exception",
  "#DF Double Fault",
  "Coprocessor Segment Overrun",
  "#TS Invalid TSS",
  "#NP Segment Not Present",
  "#SS Stack Segment Fault",
  "#GP General Protection",
  "#PG Page Fault",
  "-",
  "#MF x87 FPU Floating-Point Error",
  "#AC Aligment Check",
  "#MC Machine Check",
  "#XM SIMD Floating-Point Exception",
  "#VE Virtualization Exception"
};

static inline const char*
get_exception_message(int exception_number)
{
  return exception_msg[exception_number];
}

void trap_exception_0(void);
void trap_exception_1(void);
void trap_exception_2(void);
void trap_exception_3(void);
void trap_exception_4(void);
void trap_exception_5(void);
void trap_exception_6(void);
void trap_exception_7(void);
void trap_exception_8(void);
void trap_exception_9(void);
void trap_exception_10(void);
void trap_exception_11(void);
void trap_exception_12(void);
void trap_exception_13(void);
void trap_exception_14(void);
void trap_exception_15(void);
void trap_exception_16(void);
void trap_exception_17(void);
void trap_exception_18(void);
void trap_exception_19(void);
void trap_exception_20(void);

void intr_request_0(void);
void intr_request_1(void);
void intr_request_2(void);
void intr_request_3(void);
void intr_request_4(void);
void intr_request_5(void);
void intr_request_6(void);
void intr_request_7(void);
void intr_request_8(void);
void intr_request_9(void);
void intr_request_10(void);
void intr_request_11(void);
void intr_request_12(void);
void intr_request_13(void);
void intr_request_14(void);
void intr_request_15(void);

#endif /* NOT _TRAPS_H */
