/*
 * Final Project, Spring 2020
 * Vu Bui
 * Joseff Glass
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <driverlib/adc.c>
#include <driverlib/adc.h>
#include "launchpad.h"
#include "ras.h"
#include "pwmled.h"
#include "pwmbuzzer.h"
#include "seg7.h"
#include "seg7digit.h"

// Stop watch increment function, written in assembly
extern void
stopWatchIncrement(seg7Display_t* seg7Display);

typedef enum { On, Off } OnOff_t;
// System running state
enum {
    Reset, Run, Pause
}  sysState = Run;
volatile static OnOff_t alarmState = Off;
// Buzzer state
volatile static OnOff_t buzzerState = On;

// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = {
    0, 0, 0, 0, 0
};

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

// LED-related constant
#define LED_PWM_PERIOD          5000
#define LED_MAX_PULSE_WIDTH     100

static uint32_t count[2];
static double pitchLevel = 0;

// Buzzer-related constant
uint32_t Max_period = (50000000 / 523.25);  //Sets the Max period for sound C5
uint32_t Min_period = (50000000 / 261.63);     //Sets the Min period c4


/*
 * Task 1: Play a sine pattern on LED
 */

typedef struct
{
    int pwmPeriod;              // PWM period for LED
    int maxPulseWidth;          // maximum pulse width
} LedState_t;

static LedState_t led =
        { LED_PWM_PERIOD /* PWM Period */, LED_MAX_PULSE_WIDTH * 0.4 /* Multiply factor */};

// A sine function that uses degree as input. It converts degree to radian and then
// calls the sin() function of the C library.
static inline double sine(uint16_t degree)
{
    double radian = 2 * M_PI * ((double) (degree % 360) / 360);
    return sin(radian);
}

// update seg7Display
void callbackSeg7Display(uint32_t time) {
        int delay = 100;        //by milliseconds

        ADCRead(count);
        double percentage = count[0];
        percentage = (percentage/4096)*100;

        int firstdigit = (int)percentage/10;
        int seconddigit = (int)percentage%10;

        seg7Display.d1 = seconddigit;
        seg7Display.d2 = firstdigit;
        seg7Display.d3 = 0;
        seg7Display.d4 = 0;
        if (firstdigit < 7) {
            pitchLevel = 40;
        } else if (firstdigit < 9) {
            pitchLevel = 60;
        } else pitchLevel = 99;
        schdCallback(callbackSeg7Display, time + delay);

}

// LED playing callback function
void callbackLedPlay(uint32_t time)
{
    // Rotary read
    ADCRead(count);
    static uint16_t angle = 0;      // the degree of angle, used for calculating sine value
    int delay = 5;                  // the callback delay in ms

    // Calculate PWM parameters for red, blue, and green sub-LEDs using sine function.
    // Use phase shift of 60, 30, and 0 degrees for red, blue, and green
    led.maxPulseWidth = (count[0] * 100) / 4095;
    int pulseWidthRed = sine(angle + 72) * led.maxPulseWidth;
    int pulseWidthBlue = sine(angle + 36) * led.maxPulseWidth;
    int pulseWidthGreen = sine(angle) * led.maxPulseWidth;

    // Set the PWM parameters for the three sub-LEDs
    if (sysState == Pause) ledPwmSet(0, 0, 0, 0);
    else ledPwmSet(led.pwmPeriod, pulseWidthRed, pulseWidthBlue, pulseWidthGreen);

    // Advance the angle by one degree, so a play period is 360 * 5 = 1800 ms
    angle = (angle + 1) % 360;

    // Schedule the next callback
    schdCallback(callbackLedPlay, time + delay);
}

void checkPushButton(uint32_t time)
{
    // Read the pushbutton state; see pbRead() in launchpad.h
    int code = pbRead();
    uint32_t delay = 10;

    switch (code) {
    case 1:
        if (sysState == Run) sysState = Pause;
        else sysState = Run;

    case 2:
        break;
    }

    schdCallback(checkPushButton, time + delay);
}

void buzzing(uint32_t time)
{
    uint32_t delay = 10;
    ADCRead(count);
    double knob1 = count[0];
    double knob2 = pitchLevel;

    knob2 = (knob2/4096); //Serves to control freq

    int pitch = 5000 + (Min_period + knob2*(Min_period - Max_period) * 4095) / 4096;

    int frequency = (0.25*pitch*knob1)/4096;
    // call buzzer function to update volume and pitch
    // buzzerPWMOn(frequency, pitch);
    uprintf("knob1 %lf\n", &knob1);
    uprintf("Knob2 %lf\n", &knob2);
    if (sysState == Pause || seg7Display.d2 < 5) {
        buzzerPWMOn(0, 0);
    }
    else {
        switch (buzzerState) {
        case On:
           buzzerPWMOn(0, 0);
           buzzerState = Off;
           delay = 200;
           break;

       case Off:
           buzzerPWMOn(frequency,pitch); // else
           buzzerState = On;
           delay = 200;
           break;
       }
    }
        schdCallback(buzzing, time + delay);

}


void main(void)
{
    lpInit();
    seg7Init();
    ledPwmInit();
    ADCInit();
    buzzerPWMInit();

    // Update the clock display
    seg7DigitUpdate(&seg7Display);

    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    schdCallback(buzzing, 500);
    schdCallback(stopWatchUpdate, 1000);
    schdCallback(callbackSeg7Display, 1005);
    schdCallback(callbackLedPlay, 1006);
    schdCallback(checkPushButton, 1006);

    // Loop forever
    while (true)
    {
        schdExecute();
    }
}
