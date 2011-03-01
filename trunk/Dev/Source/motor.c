/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2006, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            MIPS Sample code
*
* File : motor.c
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include <includes.h>
#include "global.h"
#include "motor.h"
#include "pwm.h"

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            FUNCTION
*********************************************************************************************************
*/

/**
 * @brief: init a motor
 * @param: TMotor* This  Pointer to a motor struct
 * @param: byte speed	Speed of the motor in %
 * @param: byte channel Select the PWM output
 * @return: void
 */
void init_motor(TMotor* This, byte speed, byte channel)
{
	This->speed = speed;
	This->set_speed = &PWM_Set;
	This->channel = channel;
	init_pwm(This->channel);
}