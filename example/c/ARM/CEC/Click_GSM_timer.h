/*

Use mikroE Timer Calculator to generate proper timer configuration
and timer ISR.

https://www.mikroe.com/timer-calculator

*/
#include "Click_GSM_types.h"

#define __GSM_TIMER__

static void gsm_configTimer()
{


    // Configure Timer
}

void Timer_interrupt()
{
    gsm_tick();
    // Reset Flag
}