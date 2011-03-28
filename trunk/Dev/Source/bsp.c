/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                              (c) Copyright 2006, Micrium, Inc., Weston, FL
*                                           All Rights Reserved
*
*                                          Board Support Package
*                                               Microchip
*                                              Explorer 16
*********************************************************************************************************
*/

#include "bsp.h"
#include "osc.h"
#include "ports.h"
#include "CoreTimer.h"
#include "system.h"
#include "timer.h"
#include "uart.h"
#include "adc10.h"
#include "pwm.h"
#include "drone.h"
#include "uart_header.h"

/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/

#define  BSP_TMR_RELOAD          (BSP_CLK_FREQ / (2 * OS_TICKS_PER_SEC))
#define  BSP_DLY_CONST            BSP_CLK_FREQ / 1000000 

#define  PB0_MASK                 _PORTD_RD13_MASK
#define  PB0_TRIS                 TRISDbits.TRISD13
#define  PB0_IO                   LATDbits.LATD13
#define  PB0                      PORTDbits.RD13

#define  PB1_MASK                 _PORTD_RD7_MASK

/*
*********************************************************************************************************
*                                               EXTERNES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*/

static  void  LED_Init      (void);

static  void  Tmr_Init      (void);

static  void  PB_IntInit    (void);
static  void  PB_Config     (void);
static  void  PB_Init       (void);



static  void  ADC_Init      (void);
static  void  ADC_TmrInit   (void);
static  void  ADC_IntInit   (void);
static  void  ADC_Config    (void) ;

/*
*********************************************************************************************************
*                                                 LED ON
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to be ON
*                      1    turns ON LED D0 on the board
*                      .
*                      .
*                      8    turns on LED D9 on the board
*********************************************************************************************************
*/

void  LED_On (CPU_INT08U led)
{
    switch (led) {
        case ALL_LED:
             mPORTDSetBits(PORT_RED_LED | PORT_GREEN_LED | PORT_ORANGE_LED);
             break;
        case GREEN_LED:
             mPORTDSetBits(PORT_GREEN_LED);
             break;
        case ORANGE_LED:
             mPORTDSetBits(PORT_ORANGE_LED);
             break;
        case RED_LED:
             mPORTDSetBits(PORT_RED_LED);
             break;
        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                                LED OFF
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to turn OFF
*                      0    indicates that you want ALL the LEDs to be OFF
*                      1    turns OFF LED D0 on the board
*                      .
*                      .
*                      8    turns OFF LED D8 on the board
*********************************************************************************************************
*/

void  LED_Off (CPU_INT08U led)
{
    switch (led) {
        case ALL_LED:
             mPORTDClearBits(PORT_RED_LED | PORT_ORANGE_LED | PORT_GREEN_LED);
             break;
        case GREEN_LED:
             mPORTDClearBits(PORT_GREEN_LED);
             break;
        case ORANGE_LED:
             mPORTDClearBits(PORT_ORANGE_LED);
             break;
        case RED_LED:
             mPORTDClearBits(PORT_RED_LED);
             break;
        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                              LED TOGGLE
*
* Description : This function is used to alternate the state of an LED
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to toggle
*                      1    toggle LED D0 on the board
*                      .
*                      .
*                      8    toggle LED D8 on the board
*********************************************************************************************************
*/

void  LED_Toggle (CPU_INT08U led)
{
    switch (led) {
        case ALL_LED:
             mPORTDToggleBits(PORT_GREEN_LED | PORT_ORANGE_LED | PORT_RED_LED);
             break;
        case GREEN_LED:
             mPORTDToggleBits(PORT_GREEN_LED);
             break;
        case ORANGE_LED:
             mPORTDToggleBits(PORT_ORANGE_LED);
             break;
        case RED_LED:
             mPORTDToggleBits(PORT_RED_LED);
             break;
        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                       LED_Init()
*
* Description: This function initializes all of the board's leds
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  LED_Init (void) 
{
    LED_Off(ALL_LED);                                                               /* Turn off all of the LEDs                   */
}

/*
*********************************************************************************************************
*                                          INITIALIZE I/Os
*
* Description : This function initializes the GPIO pins.  All the I/O pins are initialized in this function
*               so you don't have to look at multiple places for I/O initialization.
*
* Arguments   : None
*
*********************************************************************************************************
*/

static  void  BSP_IO_Init (void)
{                                                                             /* Each LED pin is set up as an output        */
    mPORTDOutputConfig(PORT_GREEN_LED | PORT_ORANGE_LED | PORT_RED_LED );
}

/*
*********************************************************************************************************
*                                       Tmr_Init()
*
* Description: This function should intialize the timers used by your application
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  Tmr_Init (void)
{
    OpenCoreTimer(BSP_TMR_RELOAD);                                            /* Initialize the Count and Compare registers */                                               
    mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_3));                        /* Enable the core timer's interrupt          */	    
}

/*
*********************************************************************************************************
*                                  BSP_TickISR_Handler()
*
* Description : 
*
* Arguments   : None
*********************************************************************************************************
*/

void  BSP_TickISR_Handler (void)
{
    mCTClearIntFlag();                                                        /* Clear the core timer interrupt             */
    UpdateCoreTimer(BSP_TMR_RELOAD);                                          /* Prepare for the next core timer interrupt  */
}

/*
*********************************************************************************************************
*                                    BSP_Except_Handler()
*
* Description: This function offers a means of servicing exceptions that are not triggered by interrupts
*              or the syscall instruction.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_Except_Handler (void)
{
}

/*
*********************************************************************************************************
*                                    OSProbe_TmrInit()
*
* Description: Initialize the timer used by uC/Probe.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  OSProbe_TmrInit (void)
{
    OpenTimer1(T1_ON | T1_PS_1_1 | T1_SOURCE_INT, 0);
}

/*
*********************************************************************************************************
*                                    OSProbe_TmrRd()
*
* Description: Read the value of the 32-bit timer used by uC/Probe
*
* Arguments  : None
*
* Returns    : The current 32-bit value of the timer
*********************************************************************************************************
*/

INT32U  OSProbe_TmrRd (void)
{
    return  (ReadTimer1());
}

/*
*********************************************************************************************************
*                                    BSP_TIMER1Handler()
*
* Description: This function handles interrupts from the UART.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_TIMER1Handler (void)
{
	INT8U err;

	OSFlagPost(drone.acq.start_acq, (1 << 10), OS_FLAG_SET, &err);
	//LED_Toggle(ORANGE_LED);
	mT1ClearIntFlag();	
}



/*
*********************************************************************************************************
*                                       TIMER1_Init()
*
* Description: This function performs the initialization for the TIMER1.
*
* Arguments  : frequence
*
* Returns    : None
*********************************************************************************************************
*/

void  TIMER1_Init (INT16U frequence)
{
	unsigned int valeurTimer = BSP_CLK_FREQ/(256*frequence);
	mPORTDSetPinsDigitalOut(IOPORT_BIT_3);
	mPORTDClearBits(IOPORT_BIT_3);
	// STEP 2. configure Timer 1 using internal clock, 1:256 prescale
   OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, valeurTimer );

    // set up the timer interrupt with a priority of 2
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);                                
}

/*
*********************************************************************************************************
*                                       TIMER2_Init()
*
* Description: This function performs the initialization for the TIMER2.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  TIMER2_Init (void)
{
	/* Open Timer2 with Period register value */
	OpenTimer2(T2_ON | T2_PS_1_32, TIMER_PWM);
	ConfigIntTimer2(T2_INT_OFF | T2_INT_PRIOR_2);
}

/*
*********************************************************************************************************
*                                       ADC_Init()
*
* Description: This function performs the initialization for the ADC.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  ADC_Init (void)
{
    ADC_Config();                                                       /* Configure ADC settings                           */
    //ADC_IntInit();                                                      /* Configure the interrupt settings                 */
    //ADC_TmrInit();                                                      /* Initialize the timer used for the ADC            */
    EnableADC10();                                                      /* Enable the ADC                                   */
}

/*
*********************************************************************************************************
*                                     ADC_Config()
*
* Description: This function performs the configuration of the ADC.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  ADC_Config (void) 
{
    CPU_INT32U  config;
    CPU_INT32U  config1;
    CPU_INT32U  config2;
    CPU_INT32U  config3;
    CPU_INT32U  config4;
    CPU_INT32U  config5;
    
    
    config  = ADC_CH0_NEG_SAMPLEA_NVREF
            | ADC_CH0_POS_SAMPLEA_AN5;
    
    config1 = ADC_MODULE_ON
            | ADC_IDLE_STOP
            | ADC_FORMAT_INTG
            | ADC_CLK_AUTO
            | ADC_AUTO_SAMPLING_ON
            | ADC_SAMP_ON;	//ADC_CLK_TMR
            
    config2 = ADC_VREF_AVDD_AVSS
            | ADC_OFFSET_CAL_DISABLE
            | ADC_SCAN_OFF
            | ADC_SAMPLES_PER_INT_16
            | ADC_ALT_BUF_ON
            | ADC_ALT_INPUT_OFF;//ADC_ALT_INPUT_ON
    
    config3 = ADC_SAMPLE_TIME_15
            | ADC_CONV_CLK_INTERNAL_RC
            | ADC_CONV_CLK_Tcy2;
           
    config4 = SKIP_SCAN_ALL;
     
    config5 = ENABLE_AN5_ANA;
    
    SetChanADC10(config);        
    OpenADC10(config1, config2, config3, config4, config5);
}

/*
*********************************************************************************************************
*                                     ADC_IntInit()
*
* Description: This function intializes the ADC interrupt.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  ADC_IntInit (void)
{
    mAD1SetIntPriority(INT_PRIORITY_LEVEL_3);                           /* Set interrupt priority level to 3                        */
    mAD1ClearIntFlag();                                                 /* Clear interrupt flag, just in case                       */
    mAD1IntEnable(0);                                                   /* Disable interrupts                                        */
}
        
/*
*********************************************************************************************************
*                                     BSP_ADCHandler()
*
* Description: This function handles ADC interrupts.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_ADCHandler (void)
{
                                                                        /* Insert application code here                             */
    mAD1ClearIntFlag();                                                 /* Clear the interrupt before exiting ISR                   */
}

/*
*********************************************************************************************************
*                                     ADC_GetVal()
*
* Description: This function returns the ADC value.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

short  ADC_GetVal (void)
{
    CPU_INT08U  buffer;
    short value;
    
    buffer = 8 * (~ReadActiveBufferADC10() & 0x1);                      /* Select non active buffer                                 */
    
    value = (short)(ReadADC10(buffer));                                         /* Return ADC reading                                       */
	
	return value;
}


/*
*********************************************************************************************************
*                                     ADC_GetVal()
*
* Description: This function returns the battery voltage (ADC on 8 bits)
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/
short Battery_getVoltage (void)
{
    return ADC_GetVal() << 2;
}



/*
*********************************************************************************************************
*                                    ADC_TmrInit()
*
* Description: Initialize the timer used by the ADC.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  ADC_TmrInit (void)
{
    OpenTimer3(T3_ON | T3_PS_1_8 | T3_SOURCE_INT, 0);                  /* Timer 3 enabled with 1:8 prescaler                        */
}    

/* 
*********************************************************************************************************
*                                        PB_Init()
*
* Description: This function performs the initialization for the push buttons. On the Explorer 16, the push
*              buttons are all multiplexed except for push button 0. Therefore push button 0 is the only
*              one that is configured to be interrupt driven using the change notice interrupt.
*     
*                                   +----+      +----+      +----+      +----+         
*                                   | [] |      | [] |      | [] |      | [] |      
*              Push Button # :    3 +----+    2 +----+    1 +----+    0 +----+      
*                                       
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  PB_Init (void)
{
    PB_Config();                                                        /* Configure the port pins                                  */
    PB_IntInit();                                                       /* Configure interrupt settings                             */
}    

/*
*********************************************************************************************************
*                                        PB_Config()
*
* Description: This function configures the port pins for the push button.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  PB_Config (void)
{
    PB0_TRIS = 1;                                                       /* Set the pin corresponding to our push button as input    */
}    
    
/*
*********************************************************************************************************
*                                        PB_IntInit()
*
* Description: This function performs the interrupt initialization for the push button.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  PB_IntInit (void)
{
    CPU_INT32U  dummy_read;
    CPU_INT16U  config;
    
    
    config = CHANGE_INT_ON
           | CHANGE_INT_PRI_3; 
    
    CNCON = 0x8000;                                                     /* Enable the change notice module                          */
        
    EnableCN19();                                                       /* Enable change notice pin 19, tied to our push button     */
    EnableCN16();                                                       /* Enable change notice pin 13, tied to our push button     */
    
    ConfigCNPullups(CN19_PULLUP_ENABLE);                                /* Enable a weak pull-up corresponding to the CN pin        */
    ConfigCNPullups(CN16_PULLUP_ENABLE);                                /* Enable a weak pull-up corresponding to the CN pin        */
    
    dummy_read = PORTD;                                                 /* Perform a dummy read to clear any mismatch conditions    */
            
    mCNClearIntFlag();                                                  /* Clear the int flag just in case it was triggered         */
    
    ConfigIntCN(config);                                                /* Enable CN interrupts at priority level 3                 */
}    

/*
*********************************************************************************************************
*                                     BSP_CNHandler()
*
* Description: This function handles change notice interrupts.
*
* Arguments  : None
*
* Returns    : None
* 
* Notes      : Each push of the user push button will actually generate 2 interrupts, as there will actually be
*              two changes of state with the IO pin. However, since the second generation occurs as the pin returns
*              to its original state there is no simple way to reliably detect which pin generated this interrupt.
*              In this implementation only the initial change notice interrupt is serviced.
*********************************************************************************************************
*/

void  BSP_CNHandler (void)
{    
    CPU_INT32U  reg_val;
   

    reg_val = PORTD;                                                    /* Read register to clear change notice mismatch condition  */ 
    
    if ((reg_val & PB0_MASK) == 0)
	{          
                                                             /* Insert your application code here                        */                                                                  /* Insert your application code here                        */
    } 
	else if((reg_val & PB1_MASK) == 0)
	{
		
	}
    
    mCNClearIntFlag();
}

/*
*********************************************************************************************************
*                                                BSP_Dly()
* 
* Description: Perform a short delay (~1uS) by performing multiple NOP.
*
* Arguments  : us           The number of uS to delay (approximate).
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_Dly (CPU_INT32U  us)
{
    CPU_INT32U  dly;
    
    
    dly = us * BSP_DLY_CONST;
    
    while (dly--) {
        asm("NOP");
    }    
}    
    
/*
*********************************************************************************************************
*                                             BSP_IntDisAll()
* 
* Description: Disable all interrupts at the interrupt controller.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    INTDisableInterrupts();
}

/*
*********************************************************************************************************
*                                      BSP_InitIntCtrl()
*
* Description: This function intializes the external interrupt controller.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  BSP_InitIntCtrl (void) 
{
    INTCONSET = 0x1000;	
    INTEnableSystemMultiVectoredInt();
}

/*
*********************************************************************************************************
*                                             BSP_InitIO()
* 
* Description: Initialize all the I/O devices.
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_InitIO (void)    
{
                                                                        /* Enable optimal performance                       */
    SYSTEMConfigPerformance(BSP_CLK_FREQ);
    mOSCSetPBDIV(0);                                                    /* Use 1:1 CPU Core:Peripheral clocks               */

#if JTAG_ENABLE	
	DDPCONbits.JTAGEN = 1;                                              /* Maintain the port pins for JTAG use              */
#else
    DDPCONbits.JTAGEN = 0;                                              /* Free the JTAG port pins for use as GPIO          */
#endif

    BSP_InitIntCtrl();                                                  /* Initialize the interrupt controller              */
    BSP_IO_Init();                                                      /* Initialize the board's I/Os                      */
    Tmr_Init();                                                         /* Initialize the timers                            */
    LED_Init();                                                         /* Initialize LEDs                                    */
    PB_Init();                                                          /* Initialize the push buttons                      */
    ADC_Init();
}
