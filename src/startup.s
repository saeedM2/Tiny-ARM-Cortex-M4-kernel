/**
  ******************************************************************************
  * @file      startup_stm32.s
  * @author
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m4
  .thumb

.global	g_pfnVectors

/* start address for the initialization values of the .data section.
defined in linker script */
.word	_sidata
/* start address for the .data section. defined in linker script */
.word	_sdata
/* end address for the .data section. defined in linker script */
.word	_edata
/* start address for the .bss section. defined in linker script */
.word	_sbss
/* end address for the .bss section. defined in linker script */
.word	_ebss

.equ  BootRAM,        0xF1E0F85F
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

    .section	.text.Reset_Handler
	.weak	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:

/* Copy the data segment initializers from flash to SRAM */
  movs	r1, #0
  b	LoopCopyDataInit

CopyDataInit:
	ldr	r3, =_sidata
	ldr	r3, [r3, r1]
	str	r3, [r0, r1]
	adds	r1, r1, #4

LoopCopyDataInit:
	ldr	r0, =_sdata
	ldr	r3, =_edata
	adds	r2, r0, r1
	cmp	r2, r3
	bcc	CopyDataInit
	ldr	r2, =_sbss
	b	LoopFillZerobss

/* Zero fill the bss segment. */
FillZerobss:
	movs r3, #0
 	str  r3, [r2]
	adds r2, r2, #4

LoopFillZerobss:
	ldr	r3, = _ebss
	cmp	r2, r3
	bcc	FillZerobss

/* Call the application's entry point.*/
	bl	main

.size	Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/

/******************************************************************************
*
* The minimal vector table for a Cortex-M.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/

 	.section	.isr_vector,"a",%progbits
	.type	g_pfnVectors, %object
	.size	g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
	.long	_estack
  .long	Reset_Handler         /* Reset Handler */
  .long	NMI_Handler           /* NMI Handler */
  .long	HardFault_Handler     /* Hard Fault Handler */
  .long	MemManage_Handler     /* MPU Fault Handler */
  .long	BusFault_Handler      /* Bus Fault Handler */
  .long	UsageFault_Handler    /* Usage Fault Handler */
  .long	0                     /* Reserved */
  .long	0                     /* Reserved */
  .long	0                     /* Reserved */
  .long	0                     /* Reserved */
  .long	SVC_Handler           /* SVCall Handler */
  .long	DebugMon_Handler      /* Debug Monitor Handler */
  .long	0                     /* Reserved */
  .long	PendSV_Handler        /* PendSV Handler */
  .long	SysTick_Handler       /* SysTick Handler */

  /* External interrupts */
  .long	0                     /*  0:  Watchdog Timer            */
  .long	0                     /*  1:  Real Time Clock           */
  .long	0                     /*  2:  Timer0 / Timer1           */
  .long	0                     /*  3:  Timer2 / Timer3           */
  .long	0                     /*  4:  MCIa                      */
  .long	0                     /*  5:  MCIb                      */
  .long	0                     /*  6:  UART0 - DUT FPGA          */
  .long	0                     /*  7:  UART1 - DUT FPGA          */
  .long	0                     /*  8:  UART2 - DUT FPGA          */
  .long	0                     /*  9:  UART4 - not connected     */
  .long	0                     /* 10: AACI / AC97                */
  .long	0                     /* 11: CLCD Combined Interrupt    */
  .long	0                     /* 12: Ethernet                   */
  .long	0                     /* 13: USB Device                 */
  .long	0                     /* 14: USB Host Controller        */
  .long	0                     /* 15: Character LCD              */
  .long	0                     /* 16: Flexray                    */
  .long	0                     /* 17: CAN                        */
  .long	0                     /* 18: LIN                        */
  .long	0                     /* 19: I2C ADC/DAC                */
  .long	0                     /* 20: Reserved                   */
  .long	0                     /* 21: Reserved                   */
  .long	0                     /* 22: Reserved                   */
  .long	0                     /* 23: Reserved                   */
  .long	0                     /* 24: Reserved                   */
  .long	0                     /* 25: Reserved                   */
  .long	0                     /* 26: Reserved                   */
  .long	0                     /* 27: Reserved                   */
  .long	0                     /* 28: Reserved - CPU FPGA CLCD   */
  .long	0                     /* 29: Reserved - CPU FPGA        */
  .long	0                     /* 30: UART3    - CPU FPGA        */
  .long	0                     /* 31: SPI Touchscreen - CPU FPGA */

  .text
	.thumb
	.thumb_func
	.align	2
	.globl	Reset_Handler

  Default_Handler:
      ldr ip, =0xE000ED28                                                       /* contains info about CFSR register and cause of faults */
      b   .
      .size   Default_Handler, . - Default_Handler

  /*    Macro to define default handlers. Default handler
   *    will be weak symbol and just dead loops. They can be
   *    overwritten by other handlers */
      .macro  def_irq_handler handler_name
      .weak   \handler_name
      .set    \handler_name, Default_Handler
      .endm

      def_irq_handler NMI_Handler
      def_irq_handler HardFault_Handler
      def_irq_handler MemManage_Handler
      def_irq_handler BusFault_Handler
      def_irq_handler UsageFault_Handler
      def_irq_handler DebugMon_Handler
      def_irq_handler PendSV_Handler
      def_irq_handler SysTick_Handler
      .end
