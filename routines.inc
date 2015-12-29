;----------------------------------------------------------------
; vdu16, set clear 4
;----------------------------------------------------------------

vdu16:
	lda #$f0
	sta $b000
	rts

;----------------------------------------------------------------
; vdu12, clear screen
;----------------------------------------------------------------

vdu12:
	lda #22
	sta YPOS
nextrow:
	lda #1
	sta XPOS
nextcolumn:
	lda #$20
	jsr PRINTCHAR
	lda XPOS
	cmp #21
	bne nextcolumn

	dec YPOS
	bne nextrow

	lda #1
	sta XPOS	
	sta YPOS
	inc YPOS
	rts

;----------------------------------------------------------------
; NEWLINE
;
; XPOS=0
; YPOS=YPOS+1
;----------------------------------------------------------------

NEWLINE:
	lda #1
	sta XPOS
	inc YPOS
	rts

;----------------------------------------------------------------
; OSCLI, loadfile
;
; X=lb data
; Y=hb data
;
; data: $8000,"filename"
;----------------------------------------------------------------

OSCLI:

	stx $80
	sty $81

; Set load-/exec address

	ldy #0
	lda ($80),y
	sta $82		; LB Load
	sta $84		; LB Exec
	iny
	lda ($80),y
	sta $83		; HB Load
	sta $85		; HB Exec

; Set filename pointer

	lda $80
	clc
	adc #2
	sta $80
	lda $81
	adc #0
	sta $82

; ------ BUGBUGBUG -------
;
; Copy filename to $140
;

	ldy #0
nameloop:
	lda ($80),y
	sta $140,y
	iny
	cmp #$0d
	bne nameloop

; ------ BUGBUGBUG -------

; Load file

	ldx #$80
	jmp $ffe0

;----------------------------------------------------------------
; Input device handling
;
; 0 = Keyboard
; 1 = JoyMMC
; 2 = JoyKey
;----------------------------------------------------------------

scan_z:
	bit L0381	; Scan Z-key
	bvs joy_z
	bpl key_z
	jsr joyget
	eor #$ff
	and #$2
	rts
key_z:
	lda #$f1
	sta $b000
	lda $b001
;	eor #$ff
	and #$20
	rts
joy_z:
	lda #$f0
	sta $b000
	lda $b001
;	eor #$ff
	and #$2
	rts

scan_x:
	bit L0381	; Scan X-key
	bvs joy_x
	bpl key_x
	jsr joyget
	eor #$ff
	and #$1
	rts
key_x:
	lda #$f3
	sta $b000
	lda $b001
;	eor #$ff
	and #$20
	rts
joy_x:
	lda #$f0
	sta $b000
	lda $b001
;	eor #$ff
	and #$8
	rts

scan_d:
	bit L0381	; Scan .-key
	bvs joy_d
	bpl key_d
	jsr joyget
	eor #$ff
	and #$4
	rts
key_d:
	lda #$f9
	sta $b000
	lda $b001
;	eor #$ff
	and #$8
	rts
joy_d:
	lda #$f0
	sta $b000
	lda $b001
;	eor #$ff
	and #$4
	rts

scan_u:
	bit L0381	; Scan ;-key
	bvs joy_u
	bpl key_u
	jsr joyget
	eor #$ff
	and #$8
	rts
key_u:
	lda #$f2
	sta $b000
	lda $b001
;	eor #$ff
	and #$4
	rts
joy_u:
	lda #$f0
	sta $b000
	lda $b001
;	eor #$ff
	and #$10
	rts

scan_a:
	bit L0381	; Scan RETURN-key
	bvs joy_a
	bpl key_a
	jsr joyget
	eor #$ff
	and #$10
	rts
key_a:
	lda #$f6
	sta $b000
	lda $b001
;	eor #$ff
	and #$2
	rts
joy_a:
	lda #$f0
	sta $b000
	lda $b001
;	eor #$ff
	and #$1
	rts

wait_key:
	bit L0381	; Scan any-key
	bvs wait
	bpl wait
wait_loop:
	jsr joyget
	beq wait_loop
	rts
wait:
	jmp $fe94	

;----------------------------------------------------------------
; Joystick on PORTB AtoMMC interface
;
; AtoMMC  Joystick
; -----------------
;   PB0  -  Right
;   PB1  -  Left
;   PB2  -  Down
;   PB3  -  Up
;   PB4  -  Jump
;   PB5  -  nc
;   PB6  -  nc
;   PB7  -  nc
;  
;   GND  -  GND
;----------------------------------------------------------------


;----------------------------------------------------------------
; Joystick on keyboard Atom User Group standard
;
;   27  -  Right
;   13  -  Left
;   1d  -  Down
;   31  -  Up
;   09  -  Jump

;----------------------------------------------------------------

