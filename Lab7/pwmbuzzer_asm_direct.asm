; pwmled_asm_direct.asm: Assembly PWM-LED functions
; Lab 7, ECE 266, Spring 2020
;
; This version uses direct I/O register access. It is starter code.
;
; Created by Zhao Zhang
;

; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","inc/hw_timer.h","inc/hw_gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmbuzzer.h"

                .text

;*****************************************************************************
; Pointers to GPTM's (General-Purpose Timer Module) I/O registers for Lab 7
;
; The buzzer is connected to J17 of Grove base, which is PC5 and routed to
; WT0CCP1 (wide channel 0, sub-timer B).
;*****************************************************************************

; WTIMER0's B Interval Load (GPTMTBILR) register, offset 0x02C
; For PWM, It stores the PWM period
TIMER_TBILP_ptr    	.field  WTIMER0_BASE + TIMER_O_TBILR

; WTIMER0's B Match (GPTMTBMATCHR) register, offset 0x034
; For PWM, it stores the PWM pulse width for the inverted mode
TIMER_TBMATCHR_ptr 	.field  WTIMER0_BASE + TIMER_O_TBMATCHR

