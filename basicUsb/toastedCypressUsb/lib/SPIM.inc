;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME: SPIM.inc
;;   Version: 2.5, Updated on 2009/10/20 at 11:19:24
;;  Generated by PSoC Designer 5.0.1127.0
;;
;;  DESCRIPTION:
;;    Assembler include file of SPIM instance of SPIM
;;    user module.
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress Semiconductor 2009. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

; Mask value for global int reg bit for TX instance
SPIM_bINT_MASK:             equ 20h
; SPIM interrupt address
SPIM_INT_REG:               equ 0e0h

; Do not use, this equate will be removed in a future release
bSPIM_INT_MASK:             equ 20h

;--------------------------------------------------
; Register constants and masks
;--------------------------------------------------
SPIM_SPIM_MODE_0:             equ   00h      ;MODE 0 - Leading edge latches data - pos clock
SPIM_SPIM_MODE_1:             equ   02h      ;MODE 1 - Leading edge latches data - neg clock
SPIM_SPIM_MODE_2:             equ   04h      ;MODE 2 - Trailing edge latches data - pos clock
SPIM_SPIM_MODE_3:             equ   06h      ;MODE 3 - Trailing edge latches data - neg clock
SPIM_SPIM_LSB_FIRST:          equ   80h      ;LSB bit transmitted/received first
SPIM_SPIM_MSB_FIRST:          equ   00h      ;MSB bit transmitted/received first

;---------------------------
; SPI Status register masks
;---------------------------
SPIM_SPIM_RX_OVERRUN_ERROR:   equ   40h      ;Overrun error in received data
SPIM_SPIM_TX_BUFFER_EMPTY:    equ   10h      ;TX Buffer register is ready for next data byte
SPIM_SPIM_RX_BUFFER_FULL:     equ   08h      ;RX Buffer register has received current data
SPIM_SPIM_SPI_COMPLETE:       equ   20h      ;SPI Tx/Rx cycle has completed

;--------------------------------------------------
; Registers used by SPIM
;--------------------------------------------------
SPIM_CONFIG_REG:    equ 29h                      ;Control register
SPIM_CONTROL_REG:   equ 2bh                      ;Control register
SPIM_TX_BUFFER_REG: equ 29h                      ;TX Buffer Register
SPIM_RX_BUFFER_REG: equ 2ah                      ;RX Buffer Register

; end of file SPIM.inc