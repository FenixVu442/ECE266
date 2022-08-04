#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"
#include "ranger.h"


// Stop watch increment function, written in assembly
extern void
stopWatchIncrement(seg7Display_t* seg7Display);

// The running state of the stopwatch system
enum {
    Run, Pause
}  sysState = Pause;
enum {
    ModeOne, ModeTwo
}  ModeType = ModeTwo;

// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = {
    0, 0, 0, 0, 1
};

// Callback function for updating clock watch
void
stopWatchUpdate(uint32_t time)                          // The scheduled time
{
    uint32_t xyz;
    xyz=rangerDetect();
    uprintf("%d\n\r  xyz", xyz);

    if(sysState==Run){
        if(ModeType==ModeOne){
            if(xyz>9999){
                seg7Display.d1=0;
                seg7Display.d2=0;
                seg7Display.d3=0;
                seg7Display.d4=0;
                seg7Display.colon=0;
                }else{
                  seg7Display.d1=xyz%10;
                  seg7Display.d2=(xyz/10)%10;
                  seg7Display.d3=(xyz%1000)/100;
                  seg7Display.d4=xyz/1000;
                  seg7Display.colon=0;}

                }else{
                    int inch = xyz/25.4;
                    seg7Display.d3=(inch/12)%100;
                    seg7Display.d4=(inch/12)/10;
                    int reminder = inch%12;
                    seg7Display.d2= (reminder/10);
                    seg7Display.d1= (reminder%10);
                    seg7Display.colon=1;
               }
    }
    seg7DigitUpdate(&seg7Display);

    // Call back after 10 milliseconds
    schdCallback(stopWatchUpdate, time + 100);
}

// Callback function for checking push button. SW1 is the RESET button, and
// SW2 is the START/PAUSE/RESUME button.
void
checkPushButton(uint32_t time)
{
    int code = pbRead();
    uint32_t delay;

    switch (code) {
    case 1:   // SW1 is the Reset button, only when the stopwatch is paused
    if(sysState==Pause){
        sysState=Run;
    }else{
        sysState=Pause;
    }
        delay = 250;                // software debouncing
        break;

    case 2:                         // SW2 is the Start/Pause/Resume button

        if(ModeType==ModeOne){
            ModeType=ModeTwo;
        }else{
            ModeType=ModeOne;
        }

        delay = 250;                // software debouncing
        break;

    default:
        delay = 10;
    }

    schdCallback(checkPushButton, time + delay);
}
void
rangeUp(uint32_t time)
{

    schdCallback(rangeUp, time+100);
}

int main(void)
{
    lpInit();
    seg7Init();
    rangerInit();

    uprintf("%s\n\r", "Lab 3: Stopwatch");

    // Update the clock display
    seg7DigitUpdate(&seg7Display);

    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    schdCallback(stopWatchUpdate, 1000);
    schdCallback(checkPushButton, 1005);
    schdCallback(rangeUp, 100);

    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
