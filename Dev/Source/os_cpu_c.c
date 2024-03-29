/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                          (c) Copyright 1992-2006, Micrium, Inc., Weston, FL
*                                          All Rights Reserved
*
*                                              MIPS32 4K
*
*                                                MPLAB
*
* File   : os_cpu_c.c
* Version: v1.04
*********************************************************************************************************
*/

#define   OS_CPU_GLOBALS
#include  <ucos_ii.h>

/*
*********************************************************************************************************
*                                          LOCAL VARIABLES
*********************************************************************************************************
*/

#if (OS_VERSION >= 281) && (OS_TMR_EN > 0)
static  INT16U  OSTmrCtr;
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void)
{
#if (OS_VERSION >= 281) && (OS_TMR_EN > 0)
    OSTmrCtr = 0;
#endif
}
#endif

/*
*********************************************************************************************************
*                                        OS INITIALIZATION HOOK
*                                                (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : None
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void)
{
}
#endif

/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called whenever a task is created.
*
* Arguments  : ptcb    is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0
void  OSTaskCreateHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0
    App_TaskCreateHook(ptcb);
#else
    (void)ptcb;
#endif
}
#endif

/*
*********************************************************************************************************
*                                          TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb    is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0
void  OSTaskDelHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0
    App_TaskDelHook(ptcb);
#else
    (void)ptcb;
#endif
}
#endif

/*
*********************************************************************************************************
*                                            IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do
*              such things as STOP the CPU to conserve power.
*
* Arguments  : None
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
void  OSTaskIdleHook (void)
{
#if OS_APP_HOOKS_EN > 0
    App_TaskIdleHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                          STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your
*              application to add functionality to the statistics task.
*
* Arguments  : None
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0
void  OSTaskStatHook (void)
{
#if OS_APP_HOOKS_EN > 0
    App_TaskStatHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is processor-specific.
*
* Arguments  : task     is a pointer to the task code.
*
*              p_arg    is a pointer to a user supplied data area 
*
*              ptos     is a pointer to the top of stack.  OSTaskStkInit() assumes that 'ptos' points to 
*                       a free entry on the stack.  If OS_STK_GROWTH is set to 1 then 'ptos' will contain 
*                       the HIGHEST valid address of the stack.  Similarly, if OS_STK_GROWTH is set to 0, 
*                       'ptos' will contain the lowest valid address of the stack.
*
*              opt      specifies options that can be used to alter the behavior of OSTaskStkInit()
*                       (see ucos_ii.h for OS_TASK_OPT_???).
*
* Returns    : The location corresponding to the top of the stack
*
* Note(s)    : 1) Interrupts are enabled when each task starts executing.
* 
*              2) An initialized stack has the structure shown below.
*
*              OSTCBHighRdy->OSTCBStkPtr + 0x00    Free Entry                    (LOW Memory)
*                                        + 0x04    Status Register
*                                        + 0x08    EPC
*                                        + 0x0C    Special Purpose LO Register
*                                        + 0x10    Special Purpose HI Register
*                                        + 0x14    GPR[1]
*                                        + 0x18    GPR[2]
*                                        + 0x1C    GPR[3]
*                                        + 0x20    GPR[4]
*                                        + 0x24    GPR[5]
*                                        + 0x28    GPR[6]
*                                        + 0x2C    GPR[7]
*                                        + 0x30    GPR[8]
*                                        + 0x34    GPR[9]
*                                        + 0x38    GPR[10]
*                                        + 0x3C    GPR[11]
*                                        + 0x40    GPR[12]
*                                        + 0x44    GPR[13]
*                                        + 0x48    GPR[14]
*                                        + 0x4C    GPR[15]
*                                        + 0x50    GPR[16]
*                                        + 0x54    GPR[17]
*                                        + 0x58    GPR[18]
*                                        + 0x5C    GPR[19]
*                                        + 0x60    GPR[20]
*                                        + 0x64    GPR[21]
*                                        + 0x68    GPR[22]
*                                        + 0x6C    GPR[23]
*                                        + 0x70    GPR[24]
*                                        + 0x74    GPR[25]
*                                        + 0x78    GPR[26]
*                                        + 0x7C    GPR[27]
*                                        + 0x80    GPR[28]
*                                        + 0x84    GPR[30]
*                                        + 0x88    GPR[31]                       (HIGH Memory)
*********************************************************************************************************
*/

OS_STK  *OSTaskStkInit (void (*task)(void *pd), void *p_arg, OS_STK *ptos, INT16U opt)
{
    INT32U  *pstk;
    INT32U  sr_val;
    INT32U  gp_val;


    (void)opt;                                 /* Prevent compiler warning for unused arguments        */              

    asm volatile("mfc0   %0,$12"   : "=r"(sr_val));
    sr_val |= 0x00000001;

    asm volatile("addi   %0,$28,0" : "=r"(gp_val));

    pstk     = (INT32U *)ptos; 

    pstk--;                                    /* Ensure that a free entry is being referenced         */
    *pstk--  = (INT32U)task;                   /* GPR[31] (ra) is used by OSStartHighRdy()             */
    *pstk--  = (INT32U)0x30303030;             /* GPR[30]                                              */
    *pstk--  = gp_val;                         /* GPR[28]                                              */
    *pstk--  = (INT32U)0x27272727;             /* GPR[27]                                              */
    *pstk--  = (INT32U)0x26262626;             /* GPR[26]                                              */
    *pstk--  = (INT32U)0x25252525;             /* GPR[25]                                              */
    *pstk--  = (INT32U)0x24242424;             /* GPR[24]                                              */
    *pstk--  = (INT32U)0x23232323;             /* GPR[23]                                              */
    *pstk--  = (INT32U)0x22222222;             /* GPR[22]                                              */
    *pstk--  = (INT32U)0x21212121;             /* GPR[21]                                              */
    *pstk--  = (INT32U)0x20202020;             /* GPR[20]                                              */
    *pstk--  = (INT32U)0x19191919;             /* GPR[19]                                              */
    *pstk--  = (INT32U)0x18181818;             /* GPR[18]                                              */
    *pstk--  = (INT32U)0x17171717;             /* GPR[17]                                              */
    *pstk--  = (INT32U)0x16161616;             /* GPR[16]                                              */
    *pstk--  = (INT32U)0x15151515;             /* GPR[15]                                              */
    *pstk--  = (INT32U)0x14141414;             /* GPR[14]                                              */
    *pstk--  = (INT32U)0x13131313;             /* GPR[13]                                              */
    *pstk--  = (INT32U)0x12121212;             /* GPR[12]                                              */
    *pstk--  = (INT32U)0x11111111;             /* GPR[11]                                              */
    *pstk--  = (INT32U)0x10101010;             /* GPR[10]                                              */
    *pstk--  = (INT32U)0x09090909;             /* GPR[9]                                               */
    *pstk--  = (INT32U)0x08080808;             /* GPR[8]                                               */
    *pstk--  = (INT32U)0x07070707;             /* GPR[7]                                               */
    *pstk--  = (INT32U)0x06060606;             /* GPR[6]                                               */
    *pstk--  = (INT32U)0x05050505;             /* GPR[5]                                               */
    *pstk--  = (INT32U)p_arg;                  /* GPR[4] (or, equivalently, a0) is used for arguments  */
    *pstk--  = (INT32U)0x03030303;             /* GPR[3]                                               */
    *pstk--  = (INT32U)0x02020202;             /* GPR[2]                                               */
    *pstk--  = (INT32U)0x01010101;             /* GPR[1]                                               */
    *pstk--  = (INT32U)0x00000000;             /* Special Purpose HI Register                          */
    *pstk--  = (INT32U)0x00000000;             /* Special Purpose LO Register                          */
    *pstk--  = (INT32U)task;                   /* EPC                                                  */
    *pstk--  = sr_val;                         /* SR                                                   */

    return ((OS_STK *)pstk);                   /* Return new top of stack                              */
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK    
*
* Description: This function is called when a task switch is performed.  This allows you to perform 
*              other operations during a contex switch.
*
* Arguments  : None
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer OSTCBHighRdy points to the TCB of the task that 
*                 will be switched in (i.e. the highest priority task), and that OSTCBCur points to the
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if (OS_CPU_HOOKS_EN > 0) && (OS_TASK_SW_HOOK_EN > 0)
void  OSTaskSwHook (void)
{
#if OS_APP_HOOKS_EN > 0
    App_TaskSwHook();
#endif
}
#endif       

/*
*********************************************************************************************************
*                                            OS_TCBInit() HOOK 
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0
    App_TCBInitHook(ptcb);
#else
    (void)ptcb;
#endif
}
#endif

/*
*********************************************************************************************************
*                                              TICK HOOK
*
* Description: This function is called after every tick interrupt.
*
* Arguments  : None
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if (OS_CPU_HOOKS_EN > 0) && (OS_TIME_TICK_HOOK_EN > 0)
void  OSTimeTickHook (void)
{
#if OS_APP_HOOKS_EN > 0
    App_TimeTickHook();
#endif

#if OS_TMR_EN > 0 
    OSTmrCtr++;
    if (OSTmrCtr >= (OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC)) {
        OSTmrCtr = 0;
        OSTmrSignal();
    }
#endif
}
#endif
