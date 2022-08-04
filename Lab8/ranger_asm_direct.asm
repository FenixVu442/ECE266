; 
; Lab 8, ECE 266, Spring 2020
; Starter code for the bonus part
;
; Created by Zhao Zhang
;

        .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","inc/hw_timer.h","driverlib/pin_map.h","ranger.h"

        .text

;*****************************************************************************
; Pointers to timer's I/O registers for Lab 8
;
; The ultrasonic ranger should be connected to Grove jumper J15 (SIG connected
; to pin 40). The GPIO pin is PB3, and the time pin is T3CCP1. The timer is
; TIMER3 and the sub-Time is B.
;*****************************************************************************

; Register 7: GPTM Raw Interrupt Status (GPTMRIS), offset 0x01C
; This register shows the state of the GPTM's internal interrupt signal. These
; bits are set whether or not the interrupt is masked in the GPTMIMR register.
; Each bit can be cleared by writing a 1 to its corresponding bit in GPTMICR.
TIMER_RIS_ptr   .field  TIMER3_BASE + TIMER_O_RIS

; Register 9: GPTM Interrupt Clear (GPTMICR), offset 0x024
; This register is used to clear the status bits in the GPTMRIS and GPTMMIS registers.
; Writing a 1 to a bit clears the corresponding bit in the GPTMRIS and GPTMMIS registers.
TIMER_ICR_ptr   .field  TIMER3_BASE + TIMER_O_ICR

; Register 21: GPTM Timer B Value (GPTMTBV), offset 0x054
; In a 16-bit mode, bits 15:0 contain the value of the counter and bits 23:16
; contain the current, free-running value of the prescaler, which is the upper
; 8 bits of the count in Input Edge Count, Input Edge Time, PWM and one-shot
; or periodic up count modes.
TIMER_TBV_ptr   .field  TIMER3_BASE + TIMER_O_TBV

; Timer B Capture Mode Event Raw Interrupt
; The binary is 0x00000400, with 1 on the bit position of Timer B Capture Mode Event
INT_MASK        .equ    TIMER_RIS_CBERIS

