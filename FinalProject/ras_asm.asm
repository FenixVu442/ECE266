
				.cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "driverlib/adc.h", "launchpad.h", "ras.h"
                .text
                .global ADCRead
ADCRead  .asmfunc

ADC_PORT	.field    ADC0_BASE


rasRead		PUSH {r0,LR}
			LDR 	r0, ADC_PORT
			MOV		r1, #0
			BL		ADCProcessorTrigger

repeat
			LDR 	r0, ADC_PORT
			MOV		r1, #0
			MOV		r2, #0

			BL 		ADCIntStatus
			CMP 	r0, #0

			BEQ 	repeat

			LDR 	r0, ADC_PORT
			MOV 	r1, #0
			POP  	{r2}

			BL 		ADCSequenceDataGet

			LDR		r0, ADC_PORT
			MOV 	r1, #0

			BL		ADCIntClear

			POP 	{PC}

			.endasmfunc

            .end
