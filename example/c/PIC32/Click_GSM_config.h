#include "Click_GSM_types.h"

const uint32_t _GSM_TIMER_LIMIT      = 5;	    // 5 ticks
const uint16_t _GSM_BUF_WARNING      = 192;	// 192 bytes activate warning
const uint8_t  _GSM_POLL_ENABLE      = 1; 	// poll enabled
const uint8_t  _GSM_CALLBACK_ENABLE  = 0;	    // calback disabled

const uint32_t _GSM_UART_CFG[ 1 ] = 
{
	9600
};
