#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "launchpad.h"
#include "ranger.h"

// function initializations for the ranger functions


#define RANGER_PERIPH   SYSCTL_PERIPH_GPIOB
#define RANGER_PORT     GPIO_PORTB_BASE
#define RANGER_PIN      GPIO_PIN_3
#define TIMER3          TIMER3_BASE

void rangerInit() {

//Configure the microcontroller pin as GPIO output pin
SysCtlPeripheralEnable(RANGER_PERIPH);
//Enable the GPIO port peripheral

SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
//Enable the timer peripheral

TimerConfigure(TIMER3, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME_UP));
//Configure the timer and sub-timer for time capture

TimerControlEvent(TIMER3, TIMER_B,TIMER_EVENT_BOTH_EDGES);
// Configure the event types (rising, falling, or both)

TimerEnable(TIMER3, TIMER_B);
//Enable the sub-timer of the timer
}

void sendStartPulse()  {
    //1. Configure the pin type to GPIO output pin
    GPIOPinTypeGPIOOutput(RANGER_PORT, RANGER_PIN);

    //2. Set the pin output to low
    GPIOPinWrite(RANGER_PORT,RANGER_PIN,0);

    //3. Wait for a short time (microseconds)
    waitUs(2); // wait for 2 microseconds

    //4. Set the pin output to high
    GPIOPinWrite(RANGER_PORT,RANGER_PIN,RANGER_PIN);

    //5. Wait for a short time (microseconds)
    waitUs(5); // wait for 2 microseconds

    //6. Set the pin output to low
    GPIOPinWrite(RANGER_PORT,RANGER_PIN,0);

    //7. Configure the pin type to Timer pin
    GPIOPinTypeTimer(RANGER_PORT, RANGER_PIN);// To configure GPIO pin to be compatible with timer input

    //8. Route the pin to timer pin// ths is GPIOPinConfig
    GPIOPinConfigure(GPIO_PB3_T3CCP1);

    TimerIntStatus(TIMER3,false);  //To check the interrupt status of timer

    //9. Clear timer interrupt status
    TimerIntClear(TIMER3,TIMER_CAPB_EVENT); // To clear interrupt status of timer

}
