/*
 * main.c for ECE 266 Lab 6, stopwatch
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"
#include "ras.h"
#include <driverlib/adc.c>
#include <driverlib/adc.h>
#include <math.h>

static unsigned seg7Which = 1;

// Stop watch increment function, written in assembly
extern void
stopWatchIncrement(seg7Display_t* seg7Display);

// The running state of the stopwatch system
enum {
    Reset, Run, Pause
}  sysState = Pause;

// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = {
    0, 0, 0, 0, 1
};

void
checkPushButton(uint32_t time)
{
    // Read the pushbutton state; see pbRead() in launchpad.h
    int code = pbRead();
    uint32_t delay = 10;

    switch (code) {
    case 1:
        if (seg7Which == 1) seg7Which = 2;
        else seg7Which = 1;
        delay = 250;
        break;

    case 2:
        break;
    }

    schdCallback(checkPushButton, time + delay);
}

// Callback function for updating clock watch
void
stopWatchUpdate(uint32_t time)                          // The scheduled time
{
    // Update clock and display, only if the stopwatch is running
    if (sysState == Run) {
        stopWatchIncrement(&seg7Display);
    }

    else if (sysState == Reset){
            seg7Display.d1 = 0;
            seg7Display.d2 = 0;
            seg7Display.d3 = 0;
            seg7Display.d4 = 0;
            sysState = Pause;
        }
    seg7DigitUpdate(&seg7Display);

    // Call back after 10 milliseconds
    schdCallback(stopWatchUpdate, time + 100);
}


void rotary(uint32_t time) {
    int delay = 100;        //by milliseconds
    uint32_t count[2];      //

        ADCRead(count);
        double percentage = count[0];
        percentage = (percentage/4096)*100;

        int firstdigit = (int)percentage/10;
        int seconddigit = (int)percentage%10;    //hello

        printf("rot 1 - %d(Data), %lf(percent) %d%d(segdigits)\n\r",count[0] , percentage ,firstdigit , seconddigit);

        switch(seg7Which) {
            case 1:
                seg7Display.d1 = seconddigit;
                seg7Display.d2 = firstdigit;
                break;
            case 2:
                seg7Display.d3 = seconddigit;
                seg7Display.d4 = firstdigit;
                break;
        }
        seg7DigitUpdate(&seg7Display);
        /*//for dual knob
        percentage = count[1];
        percentage = (percentage/4096)*100;

        int thirdigit = (int)percentage/10;
        int fourthdigit = (int)percentage%10;

        seg7Display.d4 = thirdigit;
        seg7Display.d3 = fourthdigit;
        seg7DigitUpdate(&seg7Display);
         *
         */

       schdCallback(rotary, time + delay);

}




int main(void)
{
    lpInit();
    seg7Init();
    ADCInit();
    uint32_t data[2];

    uprintf("%s\n\r", "Lab 6: Knob Control");

    // Update the clock display
    seg7DigitUpdate(&seg7Display);
    ADCRead(data);

    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    schdCallback(stopWatchUpdate, 1000);
    schdCallback(checkPushButton, 1000);
    schdCallback(rotary, 1000);
    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
