PROJNAME=toastedUsbProgrammer1
DEVICE=CY7C64316
BASEDEVICE=CY7C64300
PROJPATH=C:/Users/stone/DOCUME~1/TOASTE~4/TOASTE~1
PSOCDIR=C:/PROGRA~2/Cypress/PSOCDE~1/5.1/Common/CY110F~1
INCLUDE_PATH=C:/PROGRA~2/Cypress/PSOCDE~1/5.1/Common/CY110F~1/tools/include/CY7C64~1
CSRCS= avrisp.c base.c fifobuf.c icsppgmr.c main.c msgdispatch.c offchipboot.c tick.c usb.c usbserial.c
LIBCSRCS=
STDCSRCS=
ASMSRCS= usbctl.asm
LIBASMSRCS= psocconfig.asm psocconfigtbl.asm psocdynamic.asm psocdynamicint.asm spim.asm spimint.asm spis.asm spisint.asm timer16.asm timer16int.asm usbfs.asm usbfs_descr.asm usbfs_drv.asm usbfs_std.asm usbfsint.asm
STDASMSRCS=
OBJECT_SOURCES= avrisp.c base.c fifobuf.c icsppgmr.c main.c msgdispatch.c offchipboot.c tick.c usb.c usbctl.asm usbserial.c
FILLVALUE=0x30
LASTROM=0x7fff
LASTRAM=0x7ff
LAST_DATARAM=0x6ff
CODECOMPRESSOR=
MORE_CFLAGS=-Wf-nomac -Wf-Osize -Wf-LMM8 -D_LMM
RELSTART=0x200
CDEFINES=
LIBS=
UMLIBS=
LIB_PATH=
ENABLE_ALIGN_SHIFT=0
LMM=1
SYS_INC_CONTENTS:=SYSTEM_STACK_PAGE:_equ_7 SYSTEM_STACK_BASE_ADDR:_equ_0h SYSTEM_LARGE_MEMORY_MODEL:_equ_1 SYSTEM_SMALL_MEMORY_MODEL:_equ_0 IMAGECRAFT:_equ_1 HITECH:_equ_2 TOOLCHAIN:_equ_IMAGECRAFT 
SYSTEM_TOOLS=1
CSFLOW=
CONFIG_NAMES=toastedUsbProgrammer1 serialPassThru 
