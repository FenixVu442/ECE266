; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmbuzzer.h"

                .text
                .global buzzerPWMOn

; Field and any other declarations
PERIPHC			.field SYSCTL_PERIPH_GPIOC
PERIPHTIMER		.field SYSCTL_PERIPH_WTIMER0
GPIOCONFIG		.field GPIO_PC5_WT0CCP1
WTIMER0         .field  WTIMER0_BASE
PORTC           .field  GPIO_PORTC_BASE


                .asmfunc
buzzerPWMOn       PUSH    {lr, r0, r1}            ; Push r0, r1 for later function calls

                LDR     r0, WTIMER0					;Loads TIMER0 into r0
                MOV     r1, #TIMER_B				;Assigns r1 with TIMER_B
                LDR		r2, [sp, #4]
                BL      TimerLoadSet

                LDR     r0, WTIMER0					;loads timer0 into r0
                MOV     r1, #TIMER_B
              	LDR		r2, [sp, #0]
                BL      TimerMatchSet

                ADD     sp, #8                     ; releases the stack space for r1, r0
                POP     {pc}
                .endasmfunc
