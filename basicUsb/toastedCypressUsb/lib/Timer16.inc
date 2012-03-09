;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME: Timer16.inc
;;   Version: 1.1, Updated on 2009/10/15 at 17:11:37
;;  Generated by PSoC Designer 5.0.1127.0
;;
;;  DESCRIPTION: Assembler declarations for the Timer16 user module interface
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress Semiconductor 2009. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

include "m8c.inc"


;--------------------------------------------------
; Constants for Timer16 API's.
;--------------------------------------------------

Timer16_INT_REG:                           equ 0x0e0
Timer16_INT_MASK:                          equ 0x08


;--------------------------------------------------
; @INSTANCE_NAME Default Parameters set in the Device Editor
;--------------------------------------------------

Timer16_PERIOD:                            equ 0x1
Timer16_MODE:                              equ 0x0
Timer16_CLOCK_SELECT:                      equ 0x1


;--------------------------------------------------
; Register Address Constants for Timer16
;--------------------------------------------------

Timer16_CONFIG_REG:                    equ 0xb0   ; Control register
Timer16_DATA1_REG:                     equ 0xb1   ; Function register
Timer16_DATA0_REG:                     equ 0xb2   ; Input register


; end of file Timer16.inc