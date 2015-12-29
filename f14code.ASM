;----------------------------------------------
;F14 Tomcat
;----------------------------------------------
	.DEFINE asm_code $1400
	.DEFINE header   1		; Header Wouter Ras emulator
	.DEFINE filenaam "F14CODE"

.org asm_code-22*header

.IF header
;********************************************************************
; ATM Header for Atom emulator Wouter Ras

name_start:
	.byte filenaam			; Filename
name_end:
	.repeat 16-name_end+name_start	; Fill with 0 till 16 chars
	  .byte $0
	.endrep

	.word asm_code			; 2 bytes startaddress
	.word L1400			; 2 bytes linkaddress
	.word eind_asm-start_asm	; 2 bytes filelength

;********************************************************************
.ENDIF


exec:
start_asm:
	.include "f14code.inc"
	.include "font.inc"
	.include "routines.inc"
	.include "buftab.inc"
	.include "joystick.inc"
	.include "fxmusic.inc"
	.include "sidplay.inc"
eind_asm: