/*
 * interrupt_reg.h
 *
 */

/* interrupt macro defines for register init and configs */

#define NVIC_BASE_ADDRESS           0xE000E100  // See pg 218 of PM2014
#define NVIC_ISER0                  NVIC_BASE_ADDRESS + 0x00  // "Interrupt set-enable registers" See pg 209 of PM2014
#define NVIC_ISER1                  NVIC_BASE_ADDRESS + 0x04  // "Interrupt set-enable registers" See pg 209 of PM2014
