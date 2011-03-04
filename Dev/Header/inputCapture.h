#ifndef _INPUT_CAPTURE1_H
#define _INPUT_CAPTURE1_H


/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/

#include "bsp_peripherals.h"
#include "bsp.h"



/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/

#define PRESCALER_TIMER_3 256


#define SECOND_PER_TIMER3_TICK 		(((1.0/BSP_CLK_FREQ) * PRESCALER_TIMER_3))
#define MICROSECOND_PER_TIMER3_TICK (1000000*SECOND_PER_TIMER3_TICK)



/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void INPUT_CAPTURE_Init(void);
void INPUT_CAPTURE_Stop();

short int getAltitudeAsCentiMeter();
short int getAltitudeAsInch();
short int getAltitudeAsMilliMeter();
short int getHighStateTime();

void        BSP_INPUT_CAPTURE1Handler     (void);

#endif /*_INPUT_CAPTURE1_H */
