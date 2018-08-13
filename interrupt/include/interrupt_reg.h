/*
 * interrupt_reg.h
 *
 * Note: the page numbers are referencing STM32F411 datasheet
 */

/* interrupt macro defines for register init and configs */

#define NVIC_BASE_ADDRESS           0xE000E100                                  /* see pg 218 of PM2014 (pg207 of PM0214 Programming manual) */
#define NVIC_ISER0                  NVIC_BASE_ADDRESS + 0x000                   /* "interrupt set-enable registers" See pg 209 of PM2014 */
#define NVIC_ISER1                  NVIC_BASE_ADDRESS + 0x004                   /* "interrupt set-enable registers" See pg 209 of PM2014 (pg 218 of PM0214 Programming manual) */
#define NVIC_ISER2                  NVIC_BASE_ADDRESS + 0x008

/* interrupt and system handler priority macro defines */

#define SVC_SH_PRI0_MASK               0x00111111                                  /* set svc call handler priority to 0x0 (highest possible) */

/* system handler priority register */
#define SHPR1                       0xE000ED18                                  /* table 4.4.8: System handler priority registers (SHPRx) on page 232 */
#define SHPR2                       0xE000ED1C                                  /* table 4.4.8: System handler priority registers (SHPRx) on page 232 */
#define SHPR3                       0xE000ED20                                  /* table 4.4.8: System handler priority registers (SHPRx) on page 232 */

/* interrrupt priority register */
#define NVIC_BASE_IPR0              0xE000E400
#define IPR0                        NVIC_BASE_IPR0 + 0x000
#define IPR1                        NVIC_BASE_IPR0 + 0x004
#define IPR2                        NVIC_BASE_IPR0 + 0x008                      /* IRQ 8 to 11 Priority Register */
#define IPR3                        NVIC_BASE_IPR0 + 0x00c
#define IPR4                        NVIC_BASE_IPR0 + 0x010
#define IPR5                        NVIC_BASE_IPR0 + 0x014
#define IPR6                        NVIC_BASE_IPR0 + 0x018
#define IPR7                        NVIC_BASE_IPR0 + 0x01c
#define IPR8                        NVIC_BASE_IPR0 + 0x020
#define IPR9                        NVIC_BASE_IPR0 + 0x024
#define IPR10                       NVIC_BASE_IPR0 + 0x028
#define IPR11                       NVIC_BASE_IPR0 + 0x02c

#define SVC_NVIC_PRI0_MASK          0x00111111
