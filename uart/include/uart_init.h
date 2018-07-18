/*
 * uart_init.h
 *
 */

#include <stdbool.h>
#include <stdio.h>

#include "rcc_clock_reg.h"
#include "gpio_reg.h"
#include "uart_reg.h"
#include "common.h"
#include "interrupt_reg.h"

void InitUART(void);
