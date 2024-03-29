#include <p32xxxx.h>


    ##################################################################
    # Entry point of the entire application
    ##################################################################
    .section .reset,"ax",@progbits
    .set noreorder
    .ent _reset
_reset:
    ##################################################################
    # If entered because of an NMI, jump to the NMI handler.
    ##################################################################
    mfc0    t0,_CP0_STATUS
    ext t0,t0,19,1      # Extract NMI bit
    beqz    t0,_no_nmi
    nop
    la  t0,_nmi_handler
    jr  t0
    nop
_no_nmi:

    ##################################################################
    # Initialize Stack Pointer
    #   _stack is initialized by the linker script to point to the
    #    starting location of the stack in DRM
    ##################################################################
    la  sp,_stack

    ##################################################################
    # Initialize Global Pointer
    #   _gp is initialized by the linker script to point to "middle"
    #   of the small variables region
    ##################################################################
    la  gp,_gp
    
    ##################################################################
    # Initialize Global Pointer in Shadow Set
    #   The SRSCtl's PSS field must be set to the shadow set in which
    #   to initialize the global pointer.  Since we only have a
    #   single shadow set (besides the normal), we will initialize
    #   SRSCtl<PSS> to SRSCtl<HSS>.  We then write the global pointer
    #   to the previous shadow set to ensure that on interrupt, the
    #   global pointer has been initialized.
    ##################################################################
    mfc0    t1,_CP0_SRSCTL      # Read SRSCtl register
    ext t2,t1,26,4      # to obtain HSS field
    ins t1,t2,6,4       # Put HSS field
    mtc0    t1,_CP0_SRSCTL      # into SRSCtl<PSS>
    wrpgpr  gp,gp           # Set global pointer in PSS

    ##################################################################
    # Call the "on reset" procedure
    ##################################################################
    la  t0,_on_reset
    jalr    t0
    nop

    ##################################################################
    # Clear uninitialized data sections
    ##################################################################
    la  t0,_bss_begin
    la  t1,_bss_end
    b   _bss_check
    nop

_bss_init:  
    sw  zero,0x0(t0)
    sw  zero,0x4(t0)
    sw  zero,0x8(t0)
    sw  zero,0xc(t0)
    addu    t0,16
_bss_check:
    bltu    t0,t1,_bss_init
    nop
    
    ##################################################################
    # Copy initialized data from program flash to data memory
    #   src=_data_image_begin dst=_data_begin stop=_data_end
    ##################################################################
    la  t0,_data_image_begin
    la  t1,_data_begin
    la  t2,_data_end
    b   _init_check
    nop

_init_data: 
    lw  t3,(t0)
    sw  t3,(t1)
    addu    t0,4
    addu    t1,4
_init_check:
    bltu    t1,t2,_init_data
    nop

    ##################################################################
    # If there are no RAM functions, skip the next two sections --
    # copying RAM functions from program flash to data memory and
    # initializing bus matrix registers.
    ##################################################################
        la      t1,_ramfunc_length
    beqz    t1,_ramfunc_done
    nop

    ##################################################################
    # Copy RAM functions from program flash to data memory
    #   src=_ramfunc_image_begin dst=_ramfunc_begin stop=_ramfunc_end
    ##################################################################
    la  t0,_ramfunc_image_begin
    la  t1,_ramfunc_begin
    la  t2,_ramfunc_end

_init_ramfunc:  
    lw  t3,(t0)
    sw  t3,(t1)
    addu    t0,4
    addu    t1,4
_ramfunc_check:
    bltu    t1,t2,_init_ramfunc
    nop

    ##################################################################
    # Initialize bus matrix registers if RAM functions exist in the
    # application
    ##################################################################
    la  t1,_bmxdkpba_address
    la  t2,BMXDKPBA
    sw  t1,0(t2)
    la  t1,_bmxdudba_address
    la  t2,BMXDUDBA
    sw  t1,0(t2)
    la  t1,_bmxdupba_address
    la  t2,BMXDUPBA
    sw  t1,0(t2)
_ramfunc_done:

    ##################################################################
    # Initialize CP0 registers
    ##################################################################
    # Initialize Count register
    ##################################################################
    mtc0    zero,_CP0_COUNT

    ##################################################################
    # Initialize Compare register
    ##################################################################
    li  t2,-1
    mtc0    t2,_CP0_COMPARE

    ##################################################################
    # Initialize EBase register
    ##################################################################
    la  t1,_ebase_address
    mtc0    t1,_CP0_EBASE

    ##################################################################
    # Initialize IntCtl register
    ##################################################################
    la  t1,_vector_spacing
    li  t2,0                    # Clear t2 and
    ins t2,t1,5,5       # shift value to VS field
    mtc0    t2,_CP0_INTCTL

    ##################################################################
    # Initialize CAUSE registers
    # - Enable counting of Count register <DC = 0>
    # - Use special exception vector <IV = 1>
    # - Clear pending software interrupts <IP1:IP0 = 0>
    ##################################################################
    li  t1,0x00800000
    mtc0    t1,_CP0_CAUSE

    ##################################################################
    # Initialize STATUS register
    # - Access to Coprocessor 0 not allowed in user mode <CU0 = 0>
    # - User mode uses configured endianness <RE = 0>
    # - Preserve Bootstrap Exception vectors <BEV>
    # - Preserve soft reset <SR> and non-maskable interrupt <NMI>
    # - CorExtend enabled based on whether CorExtend User Defined
    #   Instructions have been implemented <CEE = Config<UDI>>
    # - Disable any pending interrups <IM7..IM2 = 0, IM1..IM0 = 0>
    # - Disable hardware interrupts <IPL7:IPL2 = 0>
    # - Base mode is Kernel mode <UM = 0>
    # - Error level is normal <ERL = 0>
    # - Exception level is normal <EXL = 0>
    # - Interrupts are disabled <IE = 0>
    ##################################################################
    mfc0    t0,_CP0_CONFIG
    ext t1,t0,22,1      # Extract UDI from Config register
    sll t1,t1,17        # Move UDI to Status.CEE location
    mfc0    t0,_CP0_STATUS
    and t0,t0,0x00580000    # Preserve SR, NMI, and BEV
    or  t0,t1,t0        # Include Status.CEE (from UDI)
    mtc0    t0,_CP0_STATUS

    ##################################################################
    # Call the "on bootstrap" procedure
    ##################################################################
    la  t0,_on_bootstrap
    jalr    t0
    nop

    ##################################################################
    # Initialize Status<BEV> for normal exception vectors
    ##################################################################
    mfc0    t0,_CP0_STATUS
    and t0,t0,0xffbfffff    # Clear BEV
    mtc0    t0,_CP0_STATUS

    ##################################################################
    # Call main
    ##################################################################
    la  t0,main
    jalr    t0
    nop     

    ##################################################################
    # Just in case, go into infinite loop
    ##################################################################
1:
    b   1b
    nop
    
    .end _reset
    .globl _reset

    ##################################################################
    # Boot Exception Vector Handler
    # Jumps to _bootstrap_exception_handler
    ##################################################################
    .section .bev_handler,"ax",@progbits
    .set noreorder
    .ent _bev_exception
_bev_exception:
    la  t0,_bootstrap_exception_handler
    jr  t0
    nop

    .end _bev_exception
            
    ##################################################################
    # General Exception Vector Handler
    # Jumps to _general_exception_handler
    ##################################################################
    .section .gen_handler,"ax",@progbits
    .set noreorder
    .ent _gen_exception
_gen_exception:
    la  k0,OSCtxSw
    jr  k0
    nop

    .end _gen_exception
            
    ##################################################################
    # Core Timer Interrupt Handler
    # Jumps to CoreTimerIntHandler
    ##################################################################
    .section .vector_0,"ax",@progbits
    .set noreorder
    .ent _tmr_int
_tmr_int:
    la  k0,CoreTimerIntHandler
    jr  k0
    nop

    .end _tmr_int
