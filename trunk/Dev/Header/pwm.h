/*********************************************************************
 *
 *                  PWM10 Library definitions
 *
 *********************************************************************
 * FileName:        PWM10.h
 * Dependencies:
 * Processor:       PIC32
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology, Inc.
 *
 *
 ********************************************************************/
#ifndef _PWM_H
#define	 _PWM_H 

/*
*********************************************************************************************************
*                                                INCLUDES
*********************************************************************************************************
*/
#include "global.h"
#include "bsp.h"

/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/
//Trame réception: 7 octets: [ID(OxAA)]_[TAILLE_DATA(4)]_[MOTEUR_X(0-100)]_[TAILLE^^OxFF]

#define FREQUENCE_PWM 	(50)
#define TIMER_PWM		BSP_CLK_FREQ /(32*FREQUENCE_PWM)
#define PERIODE_PWM_MS (1000/FREQUENCE_PWM) 

#define DUTY_CYCLE_MOTEUR_ARRET 5

#define VALEUR_PWM_MOTEUR_ARRET (TIMER_PWM/100)*(100-DUTY_CYCLE_MOTEUR_ARRET)

/*
*********************************************************************************************************
*                                                PROTOTYPES
*********************************************************************************************************
*/

void init_pwm(byte channel);
void PWM_Stop();

//Commande des moteurs
void PWM_Set(byte channel, byte speed);

#endif // _PWM_H
