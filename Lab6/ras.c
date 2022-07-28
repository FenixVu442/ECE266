#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include "launchpad.h"
#include "ras.h"
#include <driverlib/adc.h>


void ADCInit(){
//
// Enable the ADC0 module.
//
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
//
// Wait for the ADC0 module to be ready.
//
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
{
}
//
// Enable the first sample sequencer to capture the value of channel 0 when
// the processor trigger occurs.
//
ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH7);
ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH6);

ADCSequenceEnable(ADC0_BASE, 0);
//
// Trigger the sample sequence.
//
}

/*void ADCRead(uint32_t data[2]){



    ADCIntClear(ADC0_BASE, 0);
    ADCProcessorTrigger(ADC0_BASE, 0);
    while (!ADCIntStatus(ADC0_BASE, 0, false)){

    }


ADCSequenceDataGet(ADC0_BASE, 0, data);
ADCIntClear(ADC0_BASE,0);
//return (uint32_t)data;
}
*/
