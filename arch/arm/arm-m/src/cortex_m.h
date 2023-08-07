#ifndef DEV_CORTEX_M_H_INCLUDED
#define DEV_CORTEX_M_H_INCLUDED

/* System Control Map */
#define SCS_BASE                        (uint32_t) (0xE000E000)                                 /* System Control Space Base Address */
#define SCB_BASE                        (SCS_BASE + 0x0D00)                                     /* System Control Block Base Address */
#define DCB_BASE                        (SCS_BASE + 0x0DF0)                                     /* Debug Control Block Base Address */

/* System Control Block (SCB) */
#define SCB_ICSR                        (volatile uint32_t *) (SCB_BASE + 0x004)                /* Interrupt Control and State Register */
#define SCB_VTOR                        (volatile uint32_t *) (SCB_BASE + 0x008)                /* Vector Table Offset Register */
#define SCB_AIRCR                       (volatile uint32_t *) (SCB_BASE + 0x00c)                /* Application Interrupt/Reset Control Register */
#define SCB_SCR                         (volatile uint32_t *) (SCB_BASE + 0x010)                /* System Control Register */
#define SCB_SHPR                        (volatile uint8_t *)  (SCB_BASE + 0x018)                /* System Handler Priority Register */
#define SCB_SHCSR                       (volatile uint32_t *) (SCB_BASE + 0x024)                /* System Handler Control and State Register */
#define SCB_CFSR                        (volatile uint32_t *) (SCB_BASE + 0x028)                /* Configurable fault status register - Describes Usage, Bus, and Memory faults */
#define SCB_HFSR                        (volatile uint32_t *) (SCB_BASE + 0x02C)                /* Hard fault status register - Describes hard fault */
#define SCB_DFSR                        (volatile uint32_t *) (SCB_BASE + 0x030)                /* Debug fault status register -- Describes debug event */
#define SCB_MMFAR                       (volatile uint32_t *) (SCB_BASE + 0x034)                /* Memory management fault address register - Address that caused fault */
#define SCB_BFAR                        (volatile uint32_t *) (SCB_BASE + 0x038)                /* Bus fault address register - Address that caused fault */
#define SCB_CPACR                       (volatile uint32_t *) (SCB_BASE + 0x088)                /* Coprocessor (FPU) Access Control Register */

/* Debug Control Block (DCB) */
#define DCB_DHCSR                       (volatile uint32_t *) (DCB_BASE + 0x000)                /* Debug Halting Control and Status Register*/
#define DCB_DCRSR                       (volatile uint32_t *) (DCB_BASE + 0x004)                /* Debug Core Register Selector Register */
#define DCB_DCRDR                       (volatile uint32_t *) (DCB_BASE + 0x008)                /* Debug Core Register Data Register */
#define DCB_DEMCR                       (volatile uint32_t *) (DCB_BASE + 0x00C)                /* Debug Exception and Monitor Control Register */


/**********************************************************************************************************************************************/

/* System Control Block */
#define SCB_ICSR_PENDSVCLR              (uint32_t) (1 << 27)                                    /* Clear PendSV interrupt */
#define SCB_ICSR_PENDSVSET              (uint32_t) (1 << 28)                                    /* Set PendSV interrupt */
#define SCB_ICSR_RETTOBASE				(uint32_t) (1 << 11)									/* Whether there are preempted active exceptions */

#define SCB_SCR_SLEEPONEXIT             (uint32_t) (1 << 1)                                     /* Sleep on return from interrupt routine */
#define SCB_SCR_SLEEPDEEP               (uint32_t) (1 << 2)                                     /* Use deep sleep as low power mode */
#define SCB_SCR_SEVONPEND               (uint32_t) (1 << 4)                                     /* Send event on pending exception */

#define SCB_SHCSR_MEMFAULTENA           (uint32_t) (1 << 16)                                    /* Enables Memory Management Fault */
#define SCB_SHCSR_BUSFAULTENA           (uint32_t) (1 << 17)                                    /* Enables Bus Fault */
#define SCB_SHCSR_USEFAULTENA           (uint32_t) (1 << 18)                                    /* Enables Usage Fault */

/* Hard Fault Status Register */
#define SCB_HFSR_VECTTBL                (uint32_t) (1 << 1)                                     /* Vector table hard fault.  Bus fault on vector table read during exception handling. */
#define SCB_HFSR_FORCED                 (uint32_t) (1 << 30)                                    /* Forced hard fault.  Escalation of another fault. */
#define SCB_HFSR_DEBUGEVT               (uint32_t) (1 << 31)                                    /* Indicates when a debug event has occured. */

/* Debug Fault Status Register */
#define SCB_DFSR_HALTED                 (uint32_t) (1 << 0)                                     /* Indicates a 'single-step' debug event was generated. */
#define SCB_DFSR_BKPT                   (uint32_t) (1 << 1)                                     /* Indicates a breakpoint event was generated. */
#define SCB_DFSR_DWTTRAP                (uint32_t) (1 << 2)                                     /* Indicates a debug event was generated by DWT. */

/* Memory Management Fault Status Register */
#define SCB_MMFSR_IACCVIOL              (uint8_t)  (1 << 0)                                     /* Instruction access violation.  No address in MMFAR */
#define SCB_MMFSR_DACCVIOL              (uint8_t)  (1 << 1)                                     /* Data access violation.  Address in MMFAR */
#define SCB_MMFSR_MUNSTKERR             (uint8_t)  (1 << 3)                                     /* Fault on unstacking from exception.  No address in MMAR */
#define SCB_MMFSR_MSTKERR               (uint8_t)  (1 << 4)                                     /* Fault on stacking for exception.  No address in MMFAR */
#define SCB_MMFSR_MLSPERR               (uint8_t)  (1 << 5)                                     /* Fault during FP lazy state preservation. */
#define SCB_MMFSR_MMARVALID             (uint8_t)  (1 << 7)                                     /* MMFAR holds valid address */

/* Bus Fault Status Register */
#define SCB_BFSR_IBUSERR                (uint8_t)  (1 << 0)                                     /* Instruction bus error.  No address in BFAR */
#define SCB_BFSR_PRECISERR              (uint8_t)  (1 << 1)                                     /* Precise data bus error.  Address in BFAR */
#define SCB_BFSR_IMPRECISERR            (uint8_t)  (1 << 2)                                     /* Imprecise data bus error.  No address in BFAR */
#define SCB_BFSR_UNSTKERR               (uint8_t)  (1 << 3)                                     /* Fault on unstacking from exception.  No address in BFAR */
#define SCB_BFSR_STKERR                 (uint8_t)  (1 << 4)                                     /* Fault on stacking for exception.  No address in BFAR */
#define SCB_BFSR_LSPERR                 (uint8_t)  (1 << 5)                                     /* Fault on FP lazy state preservation. */
#define SCB_BFSR_BFARVALID              (uint8_t)  (1 << 7)                                     /* BFAR holds valid address */

/* Usage Fault Status Register */
#define SCB_UFSR_UNDEFINSTR             (uint16_t) (1 << 0)                                     /* Undefined instruction */
#define SCB_UFSR_INVSTATE               (uint16_t) (1 << 1)                                     /* Invalid state - PC stacked for exception return attempts illegal use of epsr */
#define SCB_UFSR_INVPC                  (uint16_t) (1 << 2)                                     /* Invalid PC load */
#define SCB_UFSR_NOCP                   (uint16_t) (1 << 3)                                     /* No coprocessor */
#define SCB_UFSR_UNALIGNED              (uint16_t) (1 << 8)                                     /* Unaligned access */
#define SCB_UFSR_DIVBYZERO              (uint16_t) (1 << 9)                                     /* Divide by zero */

#define SCB_CPACR_CP10_FULL             (uint32_t) (0x3 << 20)                                  /* Access privileges for coprocessor 10 (FPU) */
#define SCB_CPACR_CP11_FULL             (uint32_t) (0x3 << 22)                                  /* Access privileges for coprocessor 11 (FPU) */

/* Debug Exception and Monitor Control Register */
#define DCB_DEMCR_MON_EN                (uint32_t) (1 << 16)                                    /* Enable the DebugMonitor Exception */
#define DCB_DEMCR_MON_STEP              (uint32_t) (1 << 18)                                    /* Make step request pending */
#define DCB_DEMCR_TRCENA                (uint32_t) (1 << 24)                                    /* Global enable for all DWT and ITM features */

#endif
