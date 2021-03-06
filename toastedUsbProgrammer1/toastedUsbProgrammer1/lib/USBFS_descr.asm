;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME: USBFS_descr.asm
;;   Version: 1.80, Updated on 2011/9/2 at 9:40:44
;;  Generated by PSoC Designer 5.1.2309
;;
;;  DESCRIPTION: USB User Module Descriptors
;;
;;  NOTE: User Module APIs conform to the fastcall convention for marshalling
;;        arguments and observe the associated "Registers are volatile" policy.
;;        This means it is the caller's responsibility to preserve any values
;;        in the X and A registers that are still needed after the API
;;        function returns. Even though these registers may be preserved now,
;;        there is no guarantee they will be preserved in future releases.
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress Semiconductor 2011. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

include "m8c.inc"
include "USBFS.inc"
include "USBFS_macros.inc"

WIZARD: equ 1
WIZARD_DEFINED_REPORT_STORAGE: equ 1
   ;--------------------------------------------------- 
   ;@PSoC_UserCode_BODY_1@ (Do not change this line.)
   ;---------------------------------------------------
   ; Insert your custom code below this banner
   ;---------------------------------------------------
   ; Redefine the WIZARD equate to 0 below by 
   ; uncommenting the WIZARD: equ 0 line 
   ; to allow your custom descriptor to take effect
   ;--------------------------------------------------- 

   WIZARD: equ 0
   MYSTUFF: equ 1
   ; WIZARD_DEFINED_REPORT_STORAGE: equ 0
   
   ;---------------------------------------------------
   ; Insert your custom code above this banner
   ;---------------------------------------------------
   ;PSoC_UserCode_END@ (Do not change this line.)

IF MYSTUFF ;WIZARD
;-----------------------------------------------
; USB Descriptors--Generated by the USB Setup Wizard
;-----------------------------------------------
AREA  UserModules     (ROM,REL,CON)
;----------------------------------------------------------------------
; Descriptor Dispatch Tables
;   The following tables provide the dispatching pointers and
;   descriptor lengths necessary for the request handler to
;   respond to USB requests.
;----------------------------------------------------------------------

;----------------------------------------------------------------------
; Device Dispatch Table
;   for User Module: (USBFS)
;----------------------------------------------------------------------
AREA  func_lit     (ROM,REL,CON)
EXPORT USBFS_DEVICE_LOOKUP
.LITERAL
USBFS_DEVICE_LOOKUP:                             ;
  LT_START    1                                  ; Number of devices defined for USBFS
  LT_ENTRY    USBFS_D0_CONFIG_LOOKUP, USBFS_D0_CONFIG_DESCR_TABLE
.ENDLITERAL

;----------------------------------------------------------------------
; Configuration Dispatch Table
;   for Device: (USBFS_D0)
;----------------------------------------------------------------------
AREA  func_lit     (ROM,REL,CON)
EXPORT USBFS_D0_CONFIG_LOOKUP
.LITERAL
USBFS_D0_CONFIG_LOOKUP:                          ;
  LT_START    1                                  ; Number of configurations
  LT_ENTRY    USBFS_D0_C1_EP_SETUP, NULL_PTR     ; No HID Class Descriptors
.ENDLITERAL

;----------------------------------------------------------------------
; Endpoint Setup Table
; 
; This table provides the data to configure the endpoint mode registers
; for IN/OUT direction.
;----------------------------------------------------------------------
AREA  func_lit     (ROM,REL,CON)
.LITERAL
USBFS_D0_C1_EP_SETUP:                            ;
  DB    USB_DIR_IN                               ; Endpoint EP1(IN)
  DB    USB_DIR_IN                               ; Endpoint EP2(IN)
  DB    USB_DIR_OUT                              ; Endpoint EP3(OUT)
  DB    USB_DIR_UNUSED                           ; Endpoint EP4 not used for this configuration
  DB    USB_DIR_UNUSED                           ; Endpoint EP5 not used for this configuration
  DB    USB_DIR_UNUSED                           ; Endpoint EP6 not used for this configuration
  DB    USB_DIR_UNUSED                           ; Endpoint EP7 not used for this configuration
  DB    USB_DIR_UNUSED                           ; Endpoint EP8 not used for this configuration
.ENDLITERAL

;----------------------------------------------------------------------
; Interface Lookup Table
; 
; This table is indexed by interface number.
;
;----------------------------------------------------------------------
EXPORT USBFS_D0_C1_INTERFACE_RPT_LOOKUP
.LITERAL
USBFS_D0_C1_INTERFACE_RPT_LOOKUP:                ;
  DW    0                                        ; HID Class Reports not defined for this interface
  DW    0                                        ; HID Class Reports not defined for this interface
.ENDLITERAL

;----------------------------------------------------------------------
; HID Class Descriptor transfer descriptor table
;----------------------------------------------------------------------
AREA  func_lit     (ROM,REL,CON)
EXPORT USBFS_D0_C1_HID_CLASS_DESCR_TABLE
.LITERAL
USBFS_D0_C1_HID_CLASS_DESCR_TABLE:               ;
  TD_START_TABLE 2                               ; Number of interfaces/HID Class Descriptors
  TD_ENTRY       USB_DS_ROM, 0, NULL_PTR, NULL_PTR
  TD_ENTRY       USB_DS_ROM, 0, NULL_PTR, NULL_PTR
.ENDLITERAL

;----------------------------------------------------------------------
; Configuration Descriptor Table
;  for (USBFS_D0)
;
;	This table provides transfer descriptors for each USB Configuration
; Descriptor
;----------------------------------------------------------------------
AREA  func_lit     (ROM,REL,CON)
.LITERAL
USBFS_D0_CONFIG_DESCR_TABLE:                     ;
  TD_START_TABLE 1                               ; Number of configurations
  TD_ENTRY       USB_DS_ROM, USBFS_D0_C1_DESCR_SIZE, USBFS_D0_C1_DESCR_START, NULL_PTR
.ENDLITERAL

;----------------------------------------------------------------------
; Device Descriptor Table
;
;	This table provides transfer descriptors for each USB Device Descriptor
;----------------------------------------------------------------------
AREA  func_lit     (ROM,REL,CON)
EXPORT USBFS_DEVICE_DESCR_TABLE
.LITERAL
USBFS_DEVICE_DESCR_TABLE:                        
  TD_START_TABLE 1                               ; Number of devices
  TD_ENTRY       USB_DS_ROM, USBFS_D0_DESCR_SIZE, USBFS_D0_DESCR_START, NULL_PTR
.ENDLITERAL
;----------------------------------------------------------------------
;----------------------------------------------------------------------
; Device Descriptor (USBFS_D0)
; This marks the beginning of the Device Descriptor.  This descriptor
; concatenates all of the descriptors in the following format:
;	Device Descriptor
;		Configuration Descriptor (1)
;	  	Interface 1 Descriptor
;				HID Descriptor 1
;			  	Endpoint Descriptor(s)
;			Interface 2 Descriptor
;				HID Descriptor 2
;					Endpoint Descriptor(s)
;		Configuration Descriptor (2)
;			Interface/HID/Endpoint...
;----------------------------------------------------------------------
AREA  UserModules     (ROM,REL,CON)
.LITERAL
USBFS_D0_DESCR_START:                            ;
  DB    18                                       ; Device Descriptor Length (18)
  DB    1                                        ; bDescriptorType: DEVICE
  DWL   0200H                                    ; bcdUSB (ver 2.0)
  DB    2h ;;2h                                        ; bDeviceClass
  DB    0h // 2h                                        ; bDeviceSubClass 
  DB    0h  //1h                                        ; bDeviceProtocol 
  DB    8                                        ; bMaxPacketSize0
  DWL   F055H                                    ; idVendor
  DWL   2775H                                    ; idProduct
  DWL   0001H                                    ; bcdDevice
  DB    STR_HASH_3                               ; iManufacturer
  DB    STR_HASH_2                               ; iProduct
  DB    STR_HASH_0                               ; iSerialNumber
  DB    1                                        ; bNumConfigurations
USBFS_D0_DESCR_END:                              ;
USBFS_D0_DESCR_SIZE:  EQU    USBFS_D0_DESCR_END - USBFS_D0_DESCR_START

;----------------------------------------------------------------------
; Configuration Descriptor (USBFS_D0_C1)
; This marks the beginning of the Configuration Descriptor.  This descriptor
; concatenates all of the descriptors in the following format:
; Configuration Descriptor
;	Interface 1 Descriptor
;		HID Descriptor 1
;			Endpoint Descriptor(s)
;	Interface 2 Descriptor
;		HID Descriptor 2
;			Endpoint Descriptor(s)
;----------------------------------------------------------------------
USBFS_D0_C1_DESCR_START:                         ;
  DB    9                                        ; Configuration Descriptor Length (9)
  DB    2                                        ; bDescriptorType: CONFIGURATION
  DWL   USBFS_D0_C1_DESCR_SIZE                   ; wTotalLength
  DB    2                                        ; bNumInterfaces
  DB    1                                        ; bConfigurationValue
  DB    STR_HASH_0                               ; iConfiguration
  DB    80H | (1 << 6) | (0 << 5)                ; bmAttributes >
  DB    512/2                                      ; bMaxPower
;----------------------------------------------------------------------
; Interface Descriptor (USBFS_D0_C1_I0)
;----------------------------------------------------------------------
  DB    9                                        ; Interface Descriptor Length (9)
  DB    4                                        ; bDescriptorType: INTERFACE
  DB    0                                        ; bInterfaceNumber (zero based)
  DB    0                                        ; bAlternateSetting
  DB    1                                        ; bNumEndpoints
  DB    2                                        ; bInterfaceClass
  DB    2                                        ; bInterfaceSubClass
  DB    1h ;32h                                      ; bInterfaceProtocol
  DB    STR_HASH_0                               ; iInterface
;----------------------------------------------------------------------
; CDC Class-Specific Descriptors
; Header Functional Descriptor
;----------------------------------------------------------------------
  DB    5                                        ; Descriptors Length (5)
  DB    24H                                      ; bDescriptorType: CS_INTERFACE
  DB	0                                        ; bDescriptorSubType :Header Functional Descriptor
  DWL	0110H                                    ; bcdCDC: CDC Release Number
;----------------------------------------------------------------------
; CDC Class-Specific Descriptors
; Abstract Control Management Functional Descriptor
;----------------------------------------------------------------------
  DB    4                                        ; Descriptors Length (4)
  DB    24H                                      ; bDescriptorType: CS_INTERFACE
  DB	2                                        ; bDescriptorSubType: Abstract Control Model Functional Descriptor
  DB	2h  ; GAS 2                              ; bmCapabilities; 8 = connected notif, 4 = break, 2 = line coding, 1 = comm feature      	 
; CDC Class-Specific Descriptors
; Union Functional Descriptor
;----------------------------------------------------------------------
  DB    5                                        ; Descriptors Length (5)
  DB    24H                                      ; bDescriptorType: CS_INTERFACE
  DB	6                                        ; bDescriptorSubType: Union Functional Descriptor
  DB	0                                    	 ; bMasterInterface
  DB	1                                      	 ; bSlaveInterface
;----------------------------------------------------------------------
; CDC Class-Specific Descriptors
; Call Management Functional Descriptor
;----------------------------------------------------------------------
  DB    5                                        ; Descriptors Length (5)
  DB    24H                                      ; bDescriptorType: CS_INTERFACE
  DB	1                                        ; bDescriptorSubType: Call Management Functional Descriptor
  DB	0  ;2 ;for windoze                                    	 ; bmCapabilities: 0 - no call mgmt (works linux) 2 -Device sends/receives call management information only over the Communication Class Interface.
  DB	1                                    	 ; Interface Number of Data Class interface

;----------------------------------------------------------------------
; Endpoint Descriptor (USBFS_D0_C1_I0_E0)
;----------------------------------------------------------------------
  DB    7                                        ; Endpoint Descriptor Length (7)
  DB    5                                        ; bDescriptorType: ENDPOINT
  DB    (EP1 | USB_DIR_IN)                       ; bEndpointAddress
  DB    3                                        ; bmAttributes
  DWL   16                                        ; wMaxPacketSize
  DB    2                                        ; bInterval
;----------------------------------------------------------------------
; Interface Descriptor (USBFS_D0_C1_I1)
;----------------------------------------------------------------------
  DB    9                                        ; Interface Descriptor Length (9)
  DB    4                                        ; bDescriptorType: INTERFACE
  DB    1                                        ; bInterfaceNumber (zero based)
  DB    0                                        ; bAlternateSetting
  DB    2                                        ; bNumEndpoints
  DB    AH                                       ; bInterfaceClass
  DB    0                                        ; bInterfaceSubClass
  DB    0 ;32H  ; gas was 0h                     ; bInterfaceProtocol
  DB    STR_HASH_0                               ; iInterface
;----------------------------------------------------------------------
; Endpoint Descriptor (USBFS_D0_C1_I1_E0)
;----------------------------------------------------------------------
  DB    7                                        ; Endpoint Descriptor Length (7)
  DB    5                                        ; bDescriptorType: ENDPOINT
  DB    (EP2 | USB_DIR_IN)                       ; bEndpointAddress
  DB    2                                        ; bmAttributes
  DWL   64                                       ; wMaxPacketSize
  DB    0                                        ; bInterval
;----------------------------------------------------------------------
; Endpoint Descriptor (USBFS_D0_C1_I1_E1)
;----------------------------------------------------------------------
  DB    7                                        ; Endpoint Descriptor Length (7)
  DB    5                                        ; bDescriptorType: ENDPOINT
  DB    (EP3 | USB_DIR_OUT)                      ; bEndpointAddress
  DB    2                                        ; bmAttributes
  DWL   64                                       ; wMaxPacketSize
  DB    0xff                                        ; bInterval
;----------------------------------------------------------------------
USBFS_D0_C1_DESCR_END:
USBFS_D0_C1_DESCR_SIZE:	EQU	(USBFS_D0_C1_DESCR_END - USBFS_D0_C1_DESCR_START)
.ENDLITERAL
;----------------------------------------------------------------------
; USB String Descriptors
;
; This section contains the USB String Descriptors generated
; by the USB User Module Descriptor Generator
;
; Note:  The string labels are internally generated by the
; descriptor generator
;
; Descriptors that reference string descriptors, use a hashed
; symbol that is set in an EQU directive with each string
; descriptor.
;----------------------------------------------------------------------
STR_HASH_0:  EQU    0                            ; String Hash for the null string
AREA  UserModules     (ROM,REL,CON)
EXPORT USBFS_StringTable
.LITERAL
USBFS_StringTable:                               
  TD_START_TABLE 3                               ; Number of USB Strings
  TD_ENTRY       USB_DS_ROM, LANGID_DESCR_SIZE, LANGID_DESCR_START, NULL_PTR
  TD_ENTRY       USB_DS_ROM, STRING_2_DESCR_SIZE, STRING_2_DESCR_START, NULL_PTR
  TD_ENTRY       USB_DS_ROM, STRING_3_DESCR_SIZE, STRING_3_DESCR_START, NULL_PTR
;----------------------------------------------------------------------
; LANGID Descriptor
;----------------------------------------------------------------------
LANGID_DESCR_START:                              
  DB    ((1 * 2) + 2)                            ; bLength (N+2)
  DB    3                                        ; bDescriptorType (STRING)
  DWL   1033                                     ;
LANGID_DESCR_END:                                
LANGID_DESCR_SIZE:  EQU    (LANGID_DESCR_END - LANGID_DESCR_START)
;----------------------------------------------------------------------
; String Desciptor
;----------------------------------------------------------------------
STR_HASH_2:  EQU    1                            ; String Hash
STRING_2_DESCR_START:                            
  DB    STRING_2_DESCR_SIZE                      ; bLength
  DB    3                                        ; bDescriptorType (STRING)
  DSU   "UsbThumb AVR Programmer -- stk500v1 protocol" ;
STRING_2_DESCR_END:                              
STRING_2_DESCR_SIZE:  EQU    (STRING_2_DESCR_END - STRING_2_DESCR_START)
;----------------------------------------------------------------------
; String Desciptor
;----------------------------------------------------------------------
STR_HASH_3:  EQU    2                            ; String Hash
STRING_3_DESCR_START:                            
  DB    STRING_3_DESCR_SIZE                      ; bLength
  DB    3                                        ; bDescriptorType (STRING)
  DSU   "Toasted Circuits"                       ;
STRING_3_DESCR_END:                              
STRING_3_DESCR_SIZE:  EQU    (STRING_3_DESCR_END - STRING_3_DESCR_START)
.ENDLITERAL

;-----------------------------------------------
; End of Descriptors
;-----------------------------------------------

ENDIF

IF 0
   ;@PSoC_UserCode_END@ (Do not change this line.)

IF WIZARD
;-----------------------------------------------
; USB Descriptors--Generated by the USB Setup Wizard
;-----------------------------------------------
AREA  lit     (ROM,REL,CON)
;----------------------------------------------------------------------
; Descriptor Dispatch Tables
;   The following tables provide the dispatching pointers and
;   descriptor lengths necessary for the request handler to
;   respond to USB requests.
;----------------------------------------------------------------------

;----------------------------------------------------------------------
; Device Dispatch Table
;   for User Module: (USBFS)
;----------------------------------------------------------------------
AREA  lit     (ROM,REL,CON)
EXPORT USBFS_DEVICE_LOOKUP
.LITERAL
USBFS_DEVICE_LOOKUP:                             ;
  LT_START    1                                  ; Number of devices defined for USBFS
  LT_ENTRY    USBFS_D0_CONFIG_LOOKUP, USBFS_D0_CONFIG_DESCR_TABLE
.ENDLITERAL

;----------------------------------------------------------------------
; Configuration Dispatch Table
;   for Device: (USBFS_D0)
;----------------------------------------------------------------------
AREA  lit     (ROM,REL,CON)
EXPORT USBFS_D0_CONFIG_LOOKUP
.LITERAL
USBFS_D0_CONFIG_LOOKUP:                          ;
  LT_START    1                                  ; Number of configurations
  LT_ENTRY    USBFS_D0_C1_EP_SETUP, NULL_PTR     ; No HID Class Descriptors
.ENDLITERAL

;----------------------------------------------------------------------
; Endpoint Setup Table
; 
; This table provides the data to configure the endpoint mode registers
; for IN/OUT direction.
;----------------------------------------------------------------------
AREA  lit     (ROM,REL,CON)
.LITERAL
USBFS_D0_C1_EP_SETUP:                            ;
  DB    USB_DIR_IN                               ; Endpoint EP1(IN)
  DB    USB_DIR_IN                               ; Endpoint EP2(IN)
  DB    USB_DIR_OUT                              ; Endpoint EP3(OUT)
  DB    USB_DIR_UNUSED                           ; Endpoint EP4 not used for this configuration
  DB    USB_DIR_UNUSED                           ; Endpoint EP5 not used for this configuration
  DB    USB_DIR_UNUSED                           ; Endpoint EP6 not used for this configuration
  DB    USB_DIR_UNUSED                           ; Endpoint EP7 not used for this configuration
  DB    USB_DIR_UNUSED                           ; Endpoint EP8 not used for this configuration
.ENDLITERAL

;----------------------------------------------------------------------
; Interface Lookup Table
; 
; This table is indexed by interface number.
;
;----------------------------------------------------------------------
EXPORT USBFS_D0_C1_INTERFACE_RPT_LOOKUP
.LITERAL
USBFS_D0_C1_INTERFACE_RPT_LOOKUP:                ;
  DW    0                                        ; HID Class Reports not defined for this interface
  DW    0                                        ; HID Class Reports not defined for this interface
.ENDLITERAL

;----------------------------------------------------------------------
; HID Class Descriptor transfer descriptor table
;----------------------------------------------------------------------
AREA  lit     (ROM,REL,CON)
EXPORT USBFS_D0_C1_HID_CLASS_DESCR_TABLE
.LITERAL
USBFS_D0_C1_HID_CLASS_DESCR_TABLE:               ;
  TD_START_TABLE 2                               ; Number of interfaces/HID Class Descriptors
  TD_ENTRY       USB_DS_ROM, 0, NULL_PTR, NULL_PTR
  TD_ENTRY       USB_DS_ROM, 0, NULL_PTR, NULL_PTR
.ENDLITERAL

;----------------------------------------------------------------------
; Configuration Descriptor Table
;  for (USBFS_D0)
;
;	This table provides transfer descriptors for each USB Configuration
; Descriptor
;----------------------------------------------------------------------
AREA  lit     (ROM,REL,CON)
.LITERAL
USBFS_D0_CONFIG_DESCR_TABLE:                     ;
  TD_START_TABLE 1                               ; Number of configurations
  TD_ENTRY       USB_DS_ROM, USBFS_D0_C1_DESCR_SIZE, USBFS_D0_C1_DESCR_START, NULL_PTR
.ENDLITERAL

;----------------------------------------------------------------------
; Device Descriptor Table
;
;	This table provides transfer descriptors for each USB Device Descriptor
;----------------------------------------------------------------------
AREA  lit     (ROM,REL,CON)
EXPORT USBFS_DEVICE_DESCR_TABLE
.LITERAL
USBFS_DEVICE_DESCR_TABLE:                        
  TD_START_TABLE 1                               ; Number of devices
  TD_ENTRY       USB_DS_ROM, USBFS_D0_DESCR_SIZE, USBFS_D0_DESCR_START, NULL_PTR
.ENDLITERAL
;----------------------------------------------------------------------
;----------------------------------------------------------------------
; Device Descriptor (USBFS_D0)
; This marks the beginning of the Device Descriptor.  This descriptor
; concatenates all of the descriptors in the following format:
;	Device Descriptor
;		Configuration Descriptor (1)
;	  	Interface 1 Descriptor
;				HID Descriptor 1
;			  	Endpoint Descriptor(s)
;			Interface 2 Descriptor
;				HID Descriptor 2
;					Endpoint Descriptor(s)
;		Configuration Descriptor (2)
;			Interface/HID/Endpoint...
;----------------------------------------------------------------------
AREA  lit     (ROM,REL,CON)
.LITERAL
USBFS_D0_DESCR_START:                            ;
  DB    18                                       ; Device Descriptor Length (18)
  DB    1                                        ; bDescriptorType: DEVICE
  DWL   0200H                                    ; bcdUSB (ver 2.0)
  DB    0                                        ; bDeviceClass
  DB    0                                        ; bDeviceSubClass 
  DB    0                                        ; bDeviceProtocol 
  DB    8                                        ; bMaxPacketSize0
  DWL   F055H                                    ; idVendor
  DWL   2773H                                    ; idProduct
  DWL   0001H                                    ; bcdDevice
  DB    STR_HASH_3                               ; iManufacturer
  DB    STR_HASH_2                               ; iProduct
  DB    STR_HASH_0                               ; iSerialNumber
  DB    1                                        ; bNumConfigurations
USBFS_D0_DESCR_END:                              ;
USBFS_D0_DESCR_SIZE:  EQU    USBFS_D0_DESCR_END - USBFS_D0_DESCR_START

;----------------------------------------------------------------------
; Configuration Descriptor (USBFS_D0_C1)
; This marks the beginning of the Configuration Descriptor.  This descriptor
; concatenates all of the descriptors in the following format:
; Configuration Descriptor
;	Interface 1 Descriptor
;		HID Descriptor 1
;			Endpoint Descriptor(s)
;	Interface 2 Descriptor
;		HID Descriptor 2
;			Endpoint Descriptor(s)
;----------------------------------------------------------------------
USBFS_D0_C1_DESCR_START:                         ;
  DB    9                                        ; Configuration Descriptor Length (9)
  DB    2                                        ; bDescriptorType: CONFIGURATION
  DWL   USBFS_D0_C1_DESCR_SIZE                   ; wTotalLength
  DB    2                                        ; bNumInterfaces
  DB    1                                        ; bConfigurationValue
  DB    STR_HASH_0                               ; iConfiguration
  DB    80H | (1 << 6) | (0 << 5)                ; bmAttributes >
  DB    250                                      ; bMaxPower
;----------------------------------------------------------------------
; Interface Descriptor (USBFS_D0_C1_I0)
;----------------------------------------------------------------------
  DB    9                                        ; Interface Descriptor Length (9)
  DB    4                                        ; bDescriptorType: INTERFACE
  DB    0                                        ; bInterfaceNumber (zero based)
  DB    0                                        ; bAlternateSetting
  DB    1                                        ; bNumEndpoints
  DB    255                                      ; bInterfaceClass
  DB    0                                        ; bInterfaceSubClass
  DB    0                                        ; bInterfaceProtocol
  DB    STR_HASH_0                               ; iInterface
;----------------------------------------------------------------------
; Endpoint Descriptor (USBFS_D0_C1_I0_E0)
;----------------------------------------------------------------------
  DB    7                                        ; Endpoint Descriptor Length (7)
  DB    5                                        ; bDescriptorType: ENDPOINT
  DB    (EP1 | USB_DIR_IN)                       ; bEndpointAddress
  DB    3                                        ; bmAttributes
  DWL   8                                        ; wMaxPacketSize
  DB    2                                        ; bInterval
;----------------------------------------------------------------------
; Interface Descriptor (USBFS_D0_C1_I1)
;----------------------------------------------------------------------
  DB    9                                        ; Interface Descriptor Length (9)
  DB    4                                        ; bDescriptorType: INTERFACE
  DB    1                                        ; bInterfaceNumber (zero based)
  DB    0                                        ; bAlternateSetting
  DB    2                                        ; bNumEndpoints
  DB    255                                      ; bInterfaceClass
  DB    0                                        ; bInterfaceSubClass
  DB    0                                        ; bInterfaceProtocol
  DB    STR_HASH_0                               ; iInterface
;----------------------------------------------------------------------
; Endpoint Descriptor (USBFS_D0_C1_I1_E0)
;----------------------------------------------------------------------
  DB    7                                        ; Endpoint Descriptor Length (7)
  DB    5                                        ; bDescriptorType: ENDPOINT
  DB    (EP2 | USB_DIR_IN)                       ; bEndpointAddress
  DB    2                                        ; bmAttributes
  DWL   64                                       ; wMaxPacketSize
  DB    0                                        ; bInterval
;----------------------------------------------------------------------
; Endpoint Descriptor (USBFS_D0_C1_I1_E1)
;----------------------------------------------------------------------
  DB    7                                        ; Endpoint Descriptor Length (7)
  DB    5                                        ; bDescriptorType: ENDPOINT
  DB    (EP3 | USB_DIR_OUT)                      ; bEndpointAddress
  DB    2                                        ; bmAttributes
  DWL   64                                       ; wMaxPacketSize
  DB    0                                        ; bInterval
;----------------------------------------------------------------------
USBFS_D0_C1_DESCR_END:
USBFS_D0_C1_DESCR_SIZE:	EQU	(USBFS_D0_C1_DESCR_END - USBFS_D0_C1_DESCR_START)
.ENDLITERAL
;----------------------------------------------------------------------
; USB String Descriptors
;
; This section contains the USB String Descriptors generated
; by the USB User Module Descriptor Generator
;
; Note:  The string labels are internally generated by the
; descriptor generator
;
; Descriptors that reference string descriptors, use a hashed
; symbol that is set in an EQU directive with each string
; descriptor.
;----------------------------------------------------------------------
STR_HASH_0:  EQU    0                            ; String Hash for the null string
AREA  lit     (ROM,REL,CON)
EXPORT USBFS_StringTable
.LITERAL
USBFS_StringTable:                               
  TD_START_TABLE 3                               ; Number of USB Strings
  TD_ENTRY       USB_DS_ROM, LANGID_DESCR_SIZE, LANGID_DESCR_START, NULL_PTR
  TD_ENTRY       USB_DS_ROM, STRING_2_DESCR_SIZE, STRING_2_DESCR_START, NULL_PTR
  TD_ENTRY       USB_DS_ROM, STRING_3_DESCR_SIZE, STRING_3_DESCR_START, NULL_PTR
;----------------------------------------------------------------------
; LANGID Descriptor
;----------------------------------------------------------------------
LANGID_DESCR_START:                              
  DB    ((1 * 2) + 2)                            ; bLength (N+2)
  DB    3                                        ; bDescriptorType (STRING)
  DWL   1033                                     ;
LANGID_DESCR_END:                                
LANGID_DESCR_SIZE:  EQU    (LANGID_DESCR_END - LANGID_DESCR_START)
;----------------------------------------------------------------------
; String Desciptor
;----------------------------------------------------------------------
STR_HASH_2:  EQU    1                            ; String Hash
STRING_2_DESCR_START:                            
  DB    STRING_2_DESCR_SIZE                      ; bLength
  DB    3                                        ; bDescriptorType (STRING)
  DSU   "Lightuino4"                             ;
STRING_2_DESCR_END:                              
STRING_2_DESCR_SIZE:  EQU    (STRING_2_DESCR_END - STRING_2_DESCR_START)
;----------------------------------------------------------------------
; String Desciptor
;----------------------------------------------------------------------
STR_HASH_3:  EQU    2                            ; String Hash
STRING_3_DESCR_START:                            
  DB    STRING_3_DESCR_SIZE                      ; bLength
  DB    3                                        ; bDescriptorType (STRING)
  DSU   "Toasted Circuits"                       ;
STRING_3_DESCR_END:                              
STRING_3_DESCR_SIZE:  EQU    (STRING_3_DESCR_END - STRING_3_DESCR_START)
.ENDLITERAL

;-----------------------------------------------
; End of Descriptors
;-----------------------------------------------

ENDIF
   ;---------------------------------------------------
   ;@PSoC_UserCode_BODY_2@ (Do not change this line.)
   ;---------------------------------------------------
   ; Redefine your descriptor table below. You might
   ; cut and paste code from the WIZARD descriptor
   ; above and then make your changes.
   ;---------------------------------------------------
ENDIF
   
   ;---------------------------------------------------
   ; Insert your custom code above this banner
   ;---------------------------------------------------
   ;@PSoC_UserCode_END@ (Do not change this line.)
; End of File USBFS_descr.asm
