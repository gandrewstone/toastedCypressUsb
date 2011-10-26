;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME: SPIS.asm
;;   Version: 2.5, Updated on 2009/10/15 at 17:11:37
;;  Generated by PSoC Designer 5.0.1127.0
;;
;;  DESCRIPTION: SPIS User Module software implementation file.
;;
;;  NOTE: User Module APIs conform to the fastcall16 convention for marshalling
;;        arguments and observe the associated "Registers are volatile" policy.
;;        This means it is the caller's responsibility to preserve any values
;;        in the X and A registers that are still needed after the API functions
;;        returns. For Large Memory Model devices it is also the caller's 
;;        responsibility to preserve any value in the CUR_PP, IDX_PP, MVR_PP and 
;;        MVW_PP registers. Even though some of these registers may not be modified
;;        now, there is no guarantee that will remain the case in future releases.
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress Semiconductor 2009. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

include "m8c.inc"
include "memory.inc"
include "SPIS.inc"

;-----------------------------------------------
;  Global Symbols
;-----------------------------------------------
export   SPIS_EnableInt
export  _SPIS_EnableInt
export   SPIS_DisableInt
export  _SPIS_DisableInt
export   SPIS_Start
export  _SPIS_Start
export   SPIS_Stop
export  _SPIS_Stop
export   SPIS_SetupTxData
export  _SPIS_SetupTxData
export   SPIS_bReadRxData
export  _SPIS_bReadRxData
export   SPIS_bReadStatus
export  _SPIS_bReadStatus

IF (SPIS_SW_SS_Feature)
export   SPIS_DisableSS
export  _SPIS_DisableSS
export   SPIS_EnableSS
export  _SPIS_EnableSS
ENDIF

;-----------------------------------------------
;  Constant Definitions
;-----------------------------------------------
bfCONTROL_REG_START_BIT:   equ   1     ; Control register start bit


area UserModules (ROM, REL)


.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: SPIS_EnableInt
;
;  DESCRIPTION:
;     Enables the SPIS interrupt by setting the interrupt enable mask
;     bit associated with this User Module.
;
;     NOTE:  Remember to enable the global interrupt by calling the
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to preserve their values across calls to fastcall16 
;    functions.
;
 SPIS_EnableInt:
_SPIS_EnableInt:
   RAM_PROLOGUE RAM_USE_CLASS_1
   M8C_EnableIntMask SPIS_INT_REG, SPIS_bINT_MASK
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret

.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: SPIS_DisableInt
;
;  DESCRIPTION:
;     Disables this SPIS's interrupt by clearing the interrupt enable mask bit
;     associated with this User Module.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:  none
;
;  RETURNS: none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to preserve their values across calls to fastcall16 
;    functions.
;
 SPIS_DisableInt:
_SPIS_DisableInt:
   RAM_PROLOGUE RAM_USE_CLASS_1
   M8C_DisableIntMask   SPIS_INT_REG, SPIS_bINT_MASK
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret

.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: SPIS_Start
;
;  DESCRIPTION:
;     Sets the start bit, SPI mode, and LSB/MSB first configuration of the SPIS
;     user module.
;
;     SPIS User Module will be ready to receive data, when an SPI Master initiates
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:
;     BYTE bConfiguration - Consists of SPI Mode and LSB/MSB first bit.
;           Use defined masks - masks can be OR'd together.
;     PASSED in Accumulator.
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to preserve their values across calls to fastcall16 
;    functions.
;
 SPIS_Start:
_SPIS_Start:
   RAM_PROLOGUE RAM_USE_CLASS_1
   ; setup the SPIS configuration setting
   or    A, bfCONTROL_REG_START_BIT
   mov   REG[SPIS_CONTROL_REG], A
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret

.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: SPIS_Stop
;
;  DESCRIPTION:
;     Disables SPIS operation, and de-asserts the slave select signals.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:  none
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to preserve their values across calls to fastcall16 
;    functions.
;
 SPIS_Stop:
_SPIS_Stop:
   RAM_PROLOGUE RAM_USE_CLASS_1
   ; clear the SPIS stop bits
   and   REG[SPIS_CONTROL_REG], ~bfCONTROL_REG_START_BIT
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret

.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: SPIS_SetupTxData
;
;  DESCRIPTION:
;     Loads data into the SPI Tx Buffer in readiness for an SPI Tx/Rx cycle.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:
;     BYTE  bTxData - data to transmit.
;        Passed in A register
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to preserve their values across calls to fastcall16 
;    functions.
;
 SPIS_SetupTxData:
_SPIS_SetupTxData:
   RAM_PROLOGUE RAM_USE_CLASS_1
   mov REG[SPIS_TX_BUFFER_REG], A
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret

.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: SPIS_bReadRxData
;
;  DESCRIPTION:
;     Reads the RX buffer register.  Should check the status register to make
;     sure data is valid.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:  none
;
;  RETURNS:
;     bRxData - returned in A.
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to preserve their values across calls to fastcall16 
;    functions.
;
 SPIS_bReadRxData:
_SPIS_bReadRxData:
   RAM_PROLOGUE RAM_USE_CLASS_1
   mov A, REG[SPIS_RX_BUFFER_REG]
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret

.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: SPIS_bReadStatus
;
;  DESCRIPTION:
;     Reads the SPIS Status bits in the Control/Status register.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:  none
;
;  RETURNS:
;     BYTE  bStatus - transmit status data.  Use the defined bit masks.
;        Returned in Accumulator.
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to preserve their values across calls to fastcall16 
;    functions.
;
 SPIS_bReadStatus:
_SPIS_bReadStatus:
   RAM_PROLOGUE RAM_USE_CLASS_1
   mov A,  REG[SPIS_CONTROL_REG]
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret

.ENDSECTION

IF (SPIS_SW_SS_Feature)

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: SPIS_DisableSS
;
;  DESCRIPTION:
;     Set the active-low "SS" Slave Select signal to the HIGH state
;     via firmware
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:  none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to preserve their values across calls to fastcall16 
;    functions.
;
;    If the software slave select mode is not enabled.  This function
;    may change the hardware SS input signal.
;
 SPIS_DisableSS:
_SPIS_DisableSS:
   RAM_PROLOGUE RAM_USE_CLASS_1
   M8C_SetBank1
   or  reg[SPIS_CONFIG_REG],SPIS_SPI_SLAVE_SELECT
   M8C_SetBank0
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret

.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: SPIS_EnableSS
;
;  DESCRIPTION:
;     Set the active-low "SS" Slave select signal to the LOW state
;     via firmware
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:  none
;
;  RETURNS: none
;
;  SIDE EFFECTS:   
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to preserve their values across calls to fastcall16 
;    functions.
;
;    If the software slave select mode is not enabled.  This function
;    may change the hardware SS input signal.
;
 SPIS_EnableSS:
_SPIS_EnableSS:
   RAM_PROLOGUE RAM_USE_CLASS_1
   M8C_SetBank1
   and  reg[SPIS_CONFIG_REG],~SPIS_SPI_SLAVE_SELECT
   M8C_SetBank0
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret

.ENDSECTION

ENDIF

; End of File SPIS.asm

