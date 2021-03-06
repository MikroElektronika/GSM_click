/*
Example for GSM Click

    Date          : Jan 2018.
    Author        : MikroE Team

Test configuration STM32 :
    
    MCU              : STM32F107VCT6
    Dev. Board       : EasyMx PRO v7 for STM32
    ARM Compiler ver : v6.0.0.0

---

Description :

The application is composed of three sections :

- System Initialization - Initializes all necessary GPIO pins, UART used for
the communcation with GSM module and UART used for infromation logging
- Application Initialization - Initializes driver, power on module and sends few
command for the default module configuration
- Application Task - running in parallel core state machine and checks for call flag. 
If call detected parser will hang up call.

Additional Functions :

All additional functions such as timer initialization and default handler. 

Notes :

- Echo must be turned off for succefull parsing. ( ATE0 )
- UART polling works much better with HFC enabled.
- In case of usage of other MCUs Timer initialization must be adjusted according to your MCU.

*/

#define __GSM__

#include "Click_GSM_types.h"
#include "Click_GSM_config.h"
#include "Click_GSM_timer.h"

static uint8_t callFlag;

void gsm_default_handler( uint8_t *rsp, uint8_t *evArgs )
{
    mikrobus_logWrite( rsp, _LOG_TEXT );

//  SKIP <CR> and <LF>
    if (0 == strncmp("RING", rsp + 2, 4))
    {
        callFlag = 1;
    }
}

void systemInit()
{
    callFlag = 0;
    
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_AN_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_PWM_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_INT_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );
    mikrobus_uartInit( _MIKROBUS1, &_GSM_UART_CFG[0] );
    mikrobus_logInit( _LOG_USBUART_A, 9600 );
}

void applicationInit()
{
// TIMER INIT
    gsm_configTimer();

// DRIVER INIT
    gsm_uartDriverInit((T_GSM_P)&_MIKROBUS1_GPIO, (T_GSM_P)&_MIKROBUS1_UART);
    gsm_coreInit( gsm_default_handler, 1500 );

// MODULE POWER ON
    gsm_hfcEnable( true );
    gsm_modulePower( true );

// MODULE INIT
    gsm_cmdSingle( "AT" );
    gsm_cmdSingle( "AT" );
    gsm_cmdSingle( "AT" );
    gsm_cmdSingle( "ATE0" );
    gsm_cmdSingle( "AT+IFC=2,2" );
    gsm_cmdSingle( "AT+CMGF=1" );
}

void applicationTask()
{
// CORE STATE MACHINE
    gsm_process();

    if (0 != callFlag)
    {
        gsm_cmdSingle( "ATH" );
        Delay_ms( 3000 );

        callFlag = 0;
    }
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
        applicationTask();
    }
}

/* -------------------------------------------------------------------------- */