include "m8c.inc"
include "memory.inc"
include "USBFS.inc"
include "USBFS_macros.inc"


;extern UsbCdcLineCoding

    ;;This routine is intended to call a C ISR and return execution to this code
    ; It works by manipulating the stack so that the compiler generated
    ; RETI returns here.  We also supply the value for the flags register
    ; that is restored by the RETI. A RETI expects the stack to contain the
    ; flags value to restore and the program counter location to return.
    
    ;NOTE: This code introduces a stack overhead of four bytes (in addition to the
    ; stack space used by the C ISR)

MACRO TcCallIsrC    
    push A          ;preserve A since we are using it
    call .callstub  ;puts the two byte PC on the stack
    jmp .continue   ;flow returns here after C ISR RETI
    .callstub:
    and F, 0        ;C interrupt handler expects flags to be clear
    mov A, 0   
    push A          ;Push Flags register value that will be popped by generated reti (in this
                    ; case, 0).
                    ; If your asm ISR routine uses a page mode other than 0,
                    ; you would push the appropriate value onto the stack instead of 0.
                    ; Note that this value also has an effect on global interrupt
                    ; enable/disable upon return
    
    ljmp (@0)       ;ljmp to the C ISR. RETI will return
                    ; control to a jmp to the next line
    .continue:
    pop A          ;Restore A
ENDM

AREA InterruptRAM (RAM,REL,CON)

DummyBuffer:
	BLK 8	; dummy buffer for SEND_ENCAPSULATED_COMMAND and GET_ENCAPSULATED_RESPONSE requests


AREA UserModules (ROM, REL)

export _asmTest
export USBFS_DT_h2d_cls_dev_Dispatch
export USBFS_DT_d2h_cls_dev_Dispatch
export USBFS_DT_h2d_cls_ifc_Dispatch
export USBFS_DT_d2h_cls_ifc_Dispatch

_asmTest:
   mov     A, reg[PRT0DR]
   xor     A, (1<<4)
   mov     reg[PRT0DR],A
   ret
   

.LITERAL
;GetSetEncapsulatedCommand:
;	TD_START_TABLE	1
;	TD_ENTRY		USB_DS_RAM, 8, DummyBuffer, NULL_PTR
GetSetEncapsulatedCommand:
	TD_START_TABLE	1
	TD_ENTRY		USB_DS_RAM, 8, DummyBuffer, NULL_PTR

GetSetLineCodingTable:
    TD_START_TABLE  1
    TD_ENTRY        USB_DS_RAM, 7, _UsbCdcLineCoding, NULL_PTR
.ENDLITERAL

; CDC INTERFACE REQUEST: Get_Encapsulated_Response
;-----------------------------------------------------------------------------
; bmRequestType  : (D2H | CLASS | INTERFACE)      		= A1h
; bRequest       : GET_ENCAPSULATED_RESPONSE	     	= 01h
; wValue         : Zero		               				= 0000h
; wIndex         : INTERFACE                      		= --xxh
; wLength        : Amount of data, in bytes associated with recipient
; Data			 : Control protocol-based data (Dummy buffer)
;-----------------------------------------------------------------------------

CB_h2d_cls_ifc_00:
    mov   A, REG[USBFS_EP0DATA+wIndexLo] ; Get the interface number
    cmp   A, 2                           ; Only for CDC interfaces (1 and 2)
    jnc   Not_Supported_Local
    mov   [USBFS_t2], A                  ; Selector
    mov   A,>GetSetEncapsulatedCommand   ; Get the ROM Address MSB
    mov   X,<GetSetEncapsulatedCommand   ; Get the ROM Address LSB
    ljmp   USBFS_GetTableEntry
CB_d2h_cls_ifc_01:
    mov   A, REG[USBFS_EP0DATA+wIndexLo] ; Get the interface number
    cmp   A, 2                           ; Only for CDC interfaces (1 and 2)
    jnc   Not_Supported_Local
    mov   [USBFS_t2], A                  ; Selector
    mov   A,>GetSetEncapsulatedCommand   ; Get the ROM Address MSB
    mov   X,<GetSetEncapsulatedCommand   ; Get the ROM Address LSB
    ljmp   USBFS_GetTableEntry



;-----------------------------------------------------------------------------
; FUNCTION NAME: USBUART_1_CB_h2d_cls_ifc_20
;-----------------------------------------------------------------------------
; CDC INTERFACE REQUEST: Set_Line_Coding
;-----------------------------------------------------------------------------
; bmRequestType  : (H2D | CLASS | INTERFACE)      		= 21h
; bRequest       : SET_LINE_CODING				     	= 20h
; wValue         : Zero		                    		= 0000h
; wIndex         : INTERFACE                      		= --xxh
; wLength        : Report Size                    		= 0007h
; Data			 : Line Coding Structure
;-----------------------------------------------------------------------------
CB_h2d_cls_ifc_20:
    mov   A, REG[USBFS_EP0DATA+wIndexLo] ; Get the interface number
    cmp   A, 2                           ; Only for CDC interfaces (1 and 2)
    jnc   Not_Supported_Local
    mov   [USBFS_t2], A                  ; Selector
    mov   A,>GetSetLineCodingTable       ; Get the ROM Address MSB
    mov   X,<GetSetLineCodingTable       ; Get the ROM Address LSB
    ljmp   USBFS_GetTableEntry


CB_d2h_cls_ifc_21:
;   mov     A, reg[PRT2DR]
;   xor     A, (1<<3)
;   mov     reg[PRT2DR],A

    mov   A, REG[USBFS_EP0DATA+wIndexLo] ; Get the interface number
    cmp   A, 2                           ; Only for CDC interfaces (1 and 2)
    jnc   Not_Supported_Local
    mov   [USBFS_t2], A                  ; Selector
    mov   A,>GetSetLineCodingTable       ; Get the ROM Address MSB
    mov   X,<GetSetLineCodingTable       ; Get the ROM Address LSB
    ljmp  USBFS_GetTableEntry

Not_Supported_Local:
ljmp USBFS_Not_Supported
   
USBFS_DT_h2d_cls_dev_Dispatch:
;TcCallIsrC _usbCtlClassDevice
ljmp USBFS_Not_Supported

USBFS_DT_d2h_cls_dev_Dispatch:   
ljmp USBFS_Not_Supported


USBFS_DT_h2d_cls_ifc_Dispatch:

    mov   A, REG[USBFS_EP0DATA + bRequest]
    and   A, 0x7f
    cmp   A, 0x20                           ; Only for CDC interfaces (1 and 2)
    jz   CB_h2d_cls_ifc_20
    cmp   A, 0
    jz   CB_h2d_cls_ifc_00
    
    TcCallIsrC _usbCtlClassInterface
    cmp A,0
    jz Not_Supported_Local
    ljmp USBFS_InitNoDataStageControlTransfer ; Assume C handled it

USBFS_DT_d2h_cls_ifc_Dispatch:
    mov   A, REG[USBFS_EP0DATA + bRequest]
    and   A, 0x7f
    cmp   A, 0x21                           ; Only for CDC interfaces (1 and 2)
    jz   CB_d2h_cls_ifc_21
    cmp   A, 1
    jz   CB_d2h_cls_ifc_01

    TcCallIsrC _usbCtlClassInterfaceReq
    ljmp USBFS_Not_Supported
    ;ljmp USBFS_InitNoDataStageControlTransfer ; Assume C handled it


;export TcIfcDispatch
; Head here if you get an EP0 Interface request.
;TcIfcDispatch:   
;M8C_SetBank0
;mov     A, reg[PRT0DR]
;xor     A, (1<<4)
;mov     reg[PRT0DR],A
;TcCallIsrC _usbCtlIfc
;jmp USBFS_Not_Supported


export USBFS_DT_h2d_vnd_ifc_Dispatch 
USBFS_DT_h2d_vnd_ifc_Dispatch:

;;This routine is intended to call a C ISR and return execution to this code
; It works by manipulating the stack so that the compiler generated
; RETI returns here.  We also supply the value for the flags register
; that is restored by the RETI. A RETI expects the stack to contain the
; flags value to restore and the program counter location to return.

;NOTE: This code introduces a stack overhead of four bytes (in addition to the
; stack space used by the C ISR)

push A          ;preserve A since we are using it
call .callstub  ;puts the two byte PC on the stack
jmp .continue   ;flow returns here after C ISR RETI
.callstub:
and F, 0        ;C interrupt handler expects flags to be clear
mov A, 0   
push A          ;Push Flags register value that will be popped by generated reti (in this
                ; case, 0).
                ; If your asm ISR routine uses a page mode other than 0,
                ; you would push the appropriate value onto the stack instead of 0.
                ; Note that this value also has an effect on global interrupt
                ; enable/disable upon return

ljmp _usbCtrlIfcHost2Device ;ljmp to the C ISR. RETI will return
                ; control to a jmp to the next line
.continue:
pop A          ;Restore A 
ljmp USBFS_InitNoDataStageControlTransfer

export USBFS_DT_d2h_vnd_ifc_Dispatch 
USBFS_DT_d2h_vnd_ifc_Dispatch:

;;This routine is intended to call a C ISR and return execution to this code
; It works by manipulating the stack so that the compiler generated
; RETI returns here.  We also supply the value for the flags register
; that is restored by the RETI. A RETI expects the stack to contain the
; flags value to restore and the program counter location to return.

;NOTE: This code introduces a stack overhead of four bytes (in addition to the
; stack space used by the C ISR)

push A          ;preserve A since we are using it
call .callstub  ;puts the two byte PC on the stack
jmp .continue   ;flow returns here after C ISR RETI
.callstub:
and F, 0        ;C interrupt handler expects flags to be clear
mov A, 0   
push A          ;Push Flags register value that will be popped by generated reti (in this
                ; case, 0).
                ; If your asm ISR routine uses a page mode other than 0,
                ; you would push the appropriate value onto the stack instead of 0.
                ; Note that this value also has an effect on global interrupt
                ; enable/disable upon return

ljmp _usbCtrlIfcDevice2Host ;ljmp to the C ISR. RETI will return
                ; control to a jmp to the next line
.continue:
pop A          ;Restore A 
ljmp USBFS_InitNoDataStageControlTransfer


export USBFS_DT_h2d_vnd_dev_Dispatch
USBFS_DT_h2d_vnd_dev_Dispatch:
;;This routine is intended to call a C ISR and return execution to this code
; It works by manipulating the stack so that the compiler generated
; RETI returns here.  We also supply the value for the flags register
; that is restored by the RETI. A RETI expects the stack to contain the
; flags value to restore and the program counter location to return.

;NOTE: This code introduces a stack overhead of four bytes (in addition to the
; stack space used by the C ISR)

push A          ;preserve A since we are using it
call .callstub  ;puts the two byte PC on the stack
jmp .continue   ;flow returns here after C ISR RETI
.callstub:
and F, 0        ;C interrupt handler expects flags to be clear
mov A, 0   
push A          ;Push Flags register value that will be popped by generated reti (in this
                ; case, 0).
                ; If your asm ISR routine uses a page mode other than 0,
                ; you would push the appropriate value onto the stack instead of 0.
                ; Note that this value also has an effect on global interrupt
                ; enable/disable upon return

ljmp _usbCtrlHost2Device ;ljmp to the C ISR. RETI will return
                ; control to a jmp to the next line
.continue:
pop A          ;Restore A 
ljmp USBFS_InitNoDataStageControlTransfer
   
   
export USBFS_DT_d2h_vnd_dev_Dispatch
USBFS_DT_d2h_vnd_dev_Dispatch:
;;This routine is intended to call a C ISR and return execution to this code
; It works by manipulating the stack so that the compiler generated
; RETI returns here.  We also supply the value for the flags register
; that is restored by the RETI. A RETI expects the stack to contain the
; flags value to restore and the program counter location to return.

;NOTE: This code introduces a stack overhead of four bytes (in addition to the
; stack space used by the C ISR)

push A          ;preserve A since we are using it
call .callstub  ;puts the two byte PC on the stack
jmp .continue   ;flow returns here after C ISR RETI
.callstub:
and F, 0        ;C interrupt handler expects flags to be clear
mov A, 0   
push A          ;Push Flags register value that will be popped by generated reti (in this
                ; case, 0).
                ; If your asm ISR routine uses a page mode other than 0,
                ; you would push the appropriate value onto the stack instead of 0.
                ; Note that this value also has an effect on global interrupt
                ; enable/disable upon return

ljmp _usbCtrlDevice2Host ;ljmp to the C ISR. RETI will return
                ; control to a jmp to the next line
.continue:
pop A          ;Restore A 
ljmp USBFS_InitNoDataStageControlTransfer
