#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "pwmled.h"
#include "pwmbuzzer.h"
#include "ras.h"

#define WTIMER0         WTIMER0_BASE
#define PORTC           GPIO_PORTC_BASE


void buzzerPWMInit()
{
    // Enable Timer 0 and Timer 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);

    // Connect pins to those timers
    GPIOPinTypeTimer(PORTC, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);



    // Select PWM for Timer 0 sub-Timer B, and Timer 1 sub-Timers A & B
    TimerConfigure(WTIMER0, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    // Invert the PWM waveform, so that the Match register value is the pulse width.
    // Otherwise, the pulse width will be (Load value) - (Match value).
    // The inversion is done by enabling output inversion on the PWM pins.
    TimerControlLevel(WTIMER0, TIMER_B, true /* output inversion */);


    // Turn off the LEDs initially
    TimerLoadSet(WTIMER0, TIMER_B, 200);
    TimerMatchSet(WTIMER0, TIMER_B, 0);


    // Enable the Timer 0's TimerA
    TimerEnable(WTIMER0, TIMER_B);

}
