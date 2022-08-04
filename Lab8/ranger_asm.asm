			   .cdecls "stdint.h", "stdbool.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/timer.h", "ranger.h"
               .text
RANGER_PORT    .field GPIO_PORTB_BASE
TIMER3         .field TIMER3_BASE
			   .global rangerDetect

                   .asmfunc

rangerDetect		PUSH   {lr}


					BL	sendStartPulse

loop				LDR    r0,TIMER3
					MOV	   r1,#false
					BL	   TimerIntStatus
					CMP	   r0,#0
					BEQ	   loop


					LDR   r0,TIMER3
					MOV	  r1,#TIMER_B
					BL	  TimerValueGet
					MOV	  r2,r0

					LDR   r0,TIMER3
					MOV   r1,#TIMER_CAPB_EVENT
					BL     TimerIntClear

loop2				LDR    r0,TIMER3
					MOV	   r1,#false
					BL	   TimerIntStatus
					CMP	   r0,#0
					BEQ	   loop2

					LDR   r0,TIMER3
					MOV	  r1,#TIMER_B
					BL	  TimerValueGet
					MOV	  r3,r0

					LDR   r0,TIMER3
					MOV   r1,#TIMER_CAPB_EVENT
					BL     TimerIntClear

					SUB   r0,r3,r2
					MOV	  r3,#34
					MOV   r2,#10000
					MUL	  r0,r0,r3
					SDIV   r0,r0,r2
					pop	  {pc}
