/*********************************************************************
 *
 *                  OCMP Simple PWM Application
 *
 *********************************************************************
 * FileName:        ocmp_simple_pwm.c
 * Dependencies:
 * Processor:       PIC32
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 *********************************************************************
 *
 * This Program Demonstrates continuous Pulse generation on
 * Output compare module1 of the PIC32 controller.
 *
 * The H/W setup contains PIC32 placed on an Explorer16 board.
 * Observe the output pulse on OC1 pin using an Oscilloscope.
 *
 **********************************************************************/
#include <plib.h>

#include "global.h"
#include "pwm.h"
#include "motor.h"

/**
 * @brief: init pwm
 * @param: byte channel Select the PWM output
 * @return: void
 */
void init_pwm(byte channel)
{

	
	switch(channel)
	{
		case MOTOR1:
		/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value*/
			OpenOC1( OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , TIMER_PWM, VALEUR_PWM_MOTEUR_ARRET);
		break;
		
		case MOTOR2:
			OpenOC2( OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , TIMER_PWM, VALEUR_PWM_MOTEUR_ARRET);
		break;
		case MOTOR3:
			OpenOC3( OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , TIMER_PWM, VALEUR_PWM_MOTEUR_ARRET);
		break;
		case MOTOR4:
			OpenOC4( OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , TIMER_PWM, VALEUR_PWM_MOTEUR_ARRET);
		break;
		
		default:
		
		break;
	}	
	DBPRINTF("timer2 =%d, pwm = %d\n", TIMER_PWM, VALEUR_PWM_MOTEUR_ARRET);

}
/**
 * @brief: Stop a pwm
 * @param: void
  * @return: void
 */
void PWM_Stop(void)
{
	/* Close Output Compare */
	CloseOC1();
}

/**
 * @brief: uptdate a pwm duty cycle
 * @param: byte speed	Speed of the motor in %
 * @param: byte channel Select the PWM output
 * @return: void
 */
void PWM_Set(byte channel,byte speed)
{
	float dutyCycle = (float)speed / PERIODE_PWM_MS;
	float pwm_Float = (TIMER_PWM/100)*(100-(dutyCycle+DUTY_CYCLE_MOTEUR_ARRET));
	int valeur_pwm = (int)(pwm_Float);


	DBPRINTF("Assignation au moteur %d d'une vitesse de %d \%, soit %d ticks\n", channel, speed, valeur_pwm);


	switch(channel)
	{
		case MOTOR1:
			CloseOC1(); 
			OpenOC1( OC_ON | OC_IDLE_CON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , TIMER_PWM, valeur_pwm);
			break;
		case MOTOR2: 
			CloseOC2();
			OpenOC2( OC_ON | OC_IDLE_CON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , TIMER_PWM, valeur_pwm);
			break;
		case MOTOR3: 
			CloseOC3();
			OpenOC3( OC_ON | OC_IDLE_CON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , TIMER_PWM, valeur_pwm);
			break;
		case MOTOR4:
			CloseOC4(); 
			OpenOC4( OC_ON | OC_IDLE_CON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , TIMER_PWM, valeur_pwm);
			break;
		default:
			break;
	}
}

