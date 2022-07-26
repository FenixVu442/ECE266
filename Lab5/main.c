/*
 * ECE 266, Spring 2020, Lab 5
 *
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"

// ON/Off state type
typedef enum {On, Off} OnOff_t;

// System running state
volatile static OnOff_t sysState = Off;

// Alarm state
volatile static OnOff_t alarmState = Off;

// Buzzer state
volatile static OnOff_t buzzerState = Off;

/*
 * Task 1: Flash LED
 */

// Callback function for playing the buzzer
void callbackBuzzerPlay(uint32_t time)                    // the scheduled time
{
    uint32_t delay = 10;

    if (alarmState == On)
    {
        assert(sysState == On);

        // Turn the buzzer on and off alternatively
        // Adjust the time values to control the sound intensity
        switch (buzzerState)
        {
        case On:
            buzzerOff();
            buzzerState = Off;
            delay = 2988;                           // off for 2988 ms
            break;

        case Off:
            buzzerOn();
            buzzerState = On;
            delay = 12;                            // on for 12 ms
            break;
        }
    }

    // schedule the next callback
    schdCallback(callbackBuzzerPlay, time + delay);
}

/*
 * Task 2: Checking push button, implemented as interrupt handler
 */

// The ISR (interrupt service routine) for checking the pushbuttons
void pbISR()
{
    // Last time pushbutton was pushed
    static uint32_t lastTime = 0;

    // IMPORTANT: Clear interrupt, otherwise the interrupt handler will be executed forever
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    // Read the pushbutton
    int code = pbRead();

    // Get the current system time
    uint32_t time = sysTimeGet();

    // De-bouncing: If a key was pushed within 250 ms, ignore this push
    if (time < lastTime + 250)
        return;

    // Process the pushbutton input
    switch (code)
    {
    case 1:                     // SW1: Turn on the alarming system
        sysState = On;
        alarmState = Off;
        ledTurnOnOff(false /* red */, false /* blue */, true /* green */);
        break;

    case 2:                     // SW2: Turn off the alarming system
        sysState = Off;
        alarmState = Off;
        ledTurnOnOff(false /* red */, false /* blue */, false /* green */);
        buzzerOff();;
        break;
    }

    // record the time to check for de-bouncing next time
    lastTime = time;
}

void motionEnable()  {

    static uint32_t lastTime = 0;

    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);

    uint32_t time = sysTimeGet();

    if (time < lastTime + 250) return;

    if(sysState == On) {
      if(motionDetect()) {
         alarmState = On;
         ledTurnOnOff(true /* red */, false /* blue */, false /* green */);
      }
      else {
         alarmState = Off;
         ledTurnOnOff(false /* red */, false /* blue */, true /* green */);
      }
   }
    lastTime = time;
}


/*
 * Configure the interrupt system
 */
void intrInit()
{
    // Set up ISR for pushbuttons: Port F, pin 0 (SW1) and pin 4 (SW2)
    GPIOIntRegister(GPIO_PORTF_BASE, pbISR); // register the interrupt handler
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, // interrupt on falling edge, note that SW1 and SW2 are active low
                   GPIO_FALLING_EDGE);
    IntPrioritySet(INT_GPIOF, 0); // set interrupt level to 0 (0 is the highest for programmable interrupts)
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // enable interrupts on SW1 and SW2 input
    ///////////////////////
    GPIOIntRegister(GPIO_PORTC_BASE, motionEnable); // register the interrupt handler
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, // interrupt on rising edge, motion detector is active high
                      GPIO_BOTH_EDGES);
    IntPrioritySet(INT_GPIOC, 0); //0 is the highest for programmable interrupts
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);
}

//
static volatile bool sysActivate = false;

/*
 * The main function
 */
int main(void)
{

    lpInit();
    buzzerInit();
    motionInit();
    intrInit();
    //PIRInit();

    // Print out a start message
    uprintf("%s\n\r", "Lab 5 starts");

    // Schedule the first callback events
    schdCallback(callbackBuzzerPlay, 1000);

    // Run the callback scheduler
    while (true)
    {
        schdExecute();

        // Put Tiva C into sleep until the next interrupt happens
        __asm("    wfi");
    }
}
