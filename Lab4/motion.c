#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "motion.h"

#define MOTION_PERIPH   SYSCTL_PERIPH_GPIOC
#define MOTION_PORT     GPIO_PORTC_BASE
#define MOTION_PIN      GPIO_PIN_4


//void motionInit()                                    //in asm
//{
//    // Enable the port peripheral used by the buzzer
//    SysCtlPeripheralEnable(MOTION_PERIPH);
//
//    // Configure the pin as output
//    GPIOPinTypeGPIOInput(MOTION_PORT, MOTION_PIN);
//}


bool motionDetect()                             // in asm
{
   GPIOPinRead(MOTION_PORT, MOTION_PIN);
}
