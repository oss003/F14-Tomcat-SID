;
; Code equates
;

; Screen vars

L5E78       = $8101
L5FB8       = $8201
L60F8       = $8301
L6238       = $8401
L6378       = $8501
L64B8       = $8601
L65F8       = $8701
L6738       = $8801
L6878       = $8901
L69B8       = $8a01
L6AF8       = $8b01
L6C38       = $8c01
L6D78       = $8d01
L6EB8       = $8e01
L6FF8       = $8f01
L7138       = $9001
L7278       = $9101
L73B8       = $9201
L74F8       = $9301
L7638       = $9401
L7778       = $9501
L78B8       = $9601

; OS calls

;LFFB6       = $FFB6	; Length lookup table
;LFFB7       = $FFB7	; LB vector table
;LFFB8       = $FFB8	; HB vector table

LFE71	= $FE71
LFFE0       = $FE94	; OSRDCH
LFFE7       = NEWLINE	; Newline
LFFEE       = PRINTCHAR	; OSWRCH
LFFF4       = $FFF4	; OSBYTE
LFFF7       = OSCLI	; OSCLI

LFFFF       = $FFFF

;
; Start of code
;
;            *= $0000
;
L0000      = $00		; Horizontal bytepointer ($10-$04)

L0001      = $01		; Horizontal direction, 0=nothing, 1=right, 2=left
L0002      = $02		; Vertical direction, 0=nothing, 1=up, 2=down
L0003      = $03		; Death flag

L0004      = $04		; X-pos plane ($42-$4E)
L0005      = $05		; Y-pos plane ($2C-$98)

L0006      = $06		; Shooting??

L0007      = $07		; Sprite address
L0008      = $08

L0009      = $09		; Mask address
L000A      = $0a

L000B      = $0b		; Screen buffer address
L000C      = $0c

L000D      = $0d
L000E      = $0e
L000F      = $0f
L0010      = $10
L0011      = $11
L0012      = $12
L0013      = $13
L0014      = $14
L0015      = $15
L0016      = $16
L0017      = $17
L0018      = $18
L0019      = $19
L001A      = $1a

L001B      = $1b		; Screen address
L001C      = $1c

L001D      = $1d
L001E      = $1e

L0021      = $21
L0022      = $22
L0023      = $23
L0024      = $24
L0025      = $25
L0026      = $26
L0027      = $27
L0028      = $28
L0029      = $29
L002A      = $2a
L002B      = $2b
L002C      = $2c
L002D      = $2d
L002E      = $2e
L002F      = $2f
L0030      = $30
L0031      = $31
L0032      = $32
L0033      = $33
L0034      = $34

L0035      = $35		; Score hb
L0036      = $36		; Score mb
L0037      = $37		; Score lb

L0038      = $38		; Tmp score hb
L0039      = $39		; Tmp score mb
L003A      = $3a		; Tmp score lb

L003B      = $3B		; Lives
L003C      = $3c
L003D      = $3D		; Levelnr

L003E      = $3e

L003F      = $3f		; HiScore hb
L0040      = $40		; HiScore mb
L0041      = $41		; HiScore lb

L0070      = $70
L0071      = $71
L0072      = $72
L0073      = $73

L0200      = $200

L0380	= $380			; Start level
L0381	= $381			; Input device

L0400	= $400
L0540	= $540

L4D00	= $4d00
L5600	= $5600
L5602	= $5602
L5604	= $5604
L5606	= $5606
L5608	= $5608
L5628	= $5628
L5658	= $5658
L5688	= $5688
L5700	= $5700


;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; Start of program
; Download and install code, to be erased
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

; Relocate and decode program

L1400:

; VDU codes
; - $88,$85??
; - MODE 4

	jsr vdu16

; - Set CURSOR
; - Colour 1
; - Text window 7,26,26,5
; - CLS

	jsr vdu12
	
; *LO.BACKSCR 8000-9800

            ldx #<L0C8A               ; *LOAD BackScr 8000
            ldy #>L0C8A               ; A0 0C
            jsr LFFF7               ; 20 F7 FF

; *LO.LEVEL1 4D00-5500

	lda L0380		; Load level to start
	clc
	adc #$30
	sta L0CA7

            ldx #<L0CA0               ; *LOAD LEVEL1 4D00
            ldy #>L0CA0               ; A0 0C
            jsr LFFF7              ; 20 F7 FF

            jmp L1440              ; 4C 40 14

L0C8A:      .byte <$8000,>$8000,"F14SCR",$0d
;L0C93:      .byte <L303E,>L303E,"GRAFIX",$0d
;L0C9B:      .byte "TAPE",$0d
L0CA0:      .byte <L4D00,>L4D00,"LEVEL"
L0CA7:      .byte "1",$0D

;------------------------------------------------
; Align code to start at $1400
;------------------------------------------------

	.repeat $1440-*
	  .byte 0
	.endrep

L1440:
	lda #0
	sta L003F              ; HiScore hb
	sta L0040              ; HiScore mb
	sta L0041              ; HiScore lb

	bit L0381		; Check if joystick choosen (bit7=1)
	bpl L1441
	jsr joyinit

;--------------------------------------------
; Program entry
;--------------------------------------------

L1441:
            jsr L14FC              ; Load levelfile 1 + Print blank intro screen

L1443:      jsr L15DC              ; Game loop

            lda L003B              ; A5 3B
            bmi L144E              ; 30 04

            lda L0003              ; Death flag
            bne L1443              ; D0 F5
L144E:      jsr L16BA              ; Game over

;            cli                    ; Flush keyboard buffer
;            lda #$15               ; A9 15
;            ldx #$00               ; A2 00
;            ldy #$00               ; A0 00
;            jsr LFFF4              ; 20 F4 FF
;            sei                    ; 78

; Check if Score>= Hiscore

	lda L0035
	cmp L003F
	bcc L1450
	beq chk_m
	jmp set_score
chk_m:
	lda L0036
	cmp L0040
	bcc L1450
	beq chk_h
	jmp set_score
chk_h:
	lda L0037
	cmp L0041
	bcc L1450
set_score:
	lda L0035	; Set new Hiscore
	sta L003F
	lda L0036
	sta L0040
	lda L0037
	sta L0041

	jsr L1761h	; Print Hiscore on screen
L1450:
            jmp L1441              ; 4C 40 14

;--------------------------------------------

L145F:      .byte "F14 Tomcat"			; y=$00
            .byte "(C) D'N'A 1989"		; y=$0a
            .byte "Players Software"
            .byte "Press a key"			; y=$28
            .byte "Searching for LEVEL"		; y=$33
            .byte "Press play"			; y=$46
            .byte "stop on tape"		; y=$50
            .byte "Well Done!"			; y=$5c
            .byte "You have destroyed"		; y=$66
            .byte "ARTROCK 6"			; y=$78
            .byte "Game Over"			; y=$81
;            .byte "Please rewind tape."		; y=$8a

;--------------------------------------------
; Init
; - Load levelfile 1
; - Print blank intro screen
; - Set vars
;--------------------------------------------

L14FC:
;            sei
            cld                    ; D8

            lda L172F              ; Levelnr
            cmp #$31               ; Check if '1'
            beq L1537              ; If so jump

;            lda #$0C               ; CLS
;            jsr LFFEE              ; Write character
	jsr vdu12

            jsr LFFE7              ; Newline

;            ldy #$8A               ; Print Please rewind tape
;            ldx #$13               ; A2 13
;            jsr L1744              ; 20 44 17
            jsr LFFE7              ; Newline

            lda #$31               ; Set '1'
            sta L172F              ; Set Levelnr file
            lda #$01               ; A9 01
            sta L003D              ; Set Levelnr

;            lda #$15               ; Flush keyboard buffer
;            ldx #$00               ; A2 00
;            ldy #$00               ; A0 00
;            jsr LFFF4              ; 20 F4 FF

            cli                    ; 58

            jsr L16DF              ; Load levelfile

;            lda #$15               ; Flush keyboard buffer
;            ldx #$00               ; A2 00
;            ldy #$00               ; A0 00
;            jsr LFFF4              ; 20 F4 FF

;            sei                    ; 78

L1537:      lda #$00               ; A9 00
            sta L003B              ; Set lives
            jsr L1761              ; Print lives on screen

;            lda #$0C               ; CLS
;            jsr LFFEE              ; Write character
	jsr vdu12

            ldx #$05+1               ; A2 05
            ldy #$02+1               ; A0 02
            jsr L1736              ; Move cursor to X,Y
            ldx #$0A               ; Print F14 Tomcat
            ldy #$00               ; A0 00
            jsr L1744              ; 20 44 17

            ldx #$03+1               ; A2 03
            ldy #$12+1               ; A0 12
            jsr L1736              ; Move cursor to X,Y
            ldx #$0E               ; Print (C) D'N'A 1989
            ldy #$0A               ; A0 0A
            jsr L1744              ; 20 44 17

            ldx #$05+1               ; A2 05
            ldy #$14+1               ; A0 14
            jsr L1736              ; Move cursor to X,Y
            ldx #$0B               ; Print Press a key
            ldy #$28               ; A0 28
            jsr L1744              ; 20 44 17

;            lda #$15               ; Flush keyboar buffer
;            ldx #$00               ; A2 00
;            ldy #$00               ; A0 00
;            jsr LFFF4              ; 20 F4 FF

            cli                    ; 58
;            jsr LFFE0              ; Read keypress
	jsr wait_key

;            sei                    ; 78

	jsr init		; Start SID music

            lda #$00               ; Clear buffer????
            tay                    ; A8
L157E:      sta L5700,Y            ; 99 00 57
            sta L5628,Y            ; 99 28 56
            sta L5688,Y            ; 99 88 56
            iny                    ; C8
            bne L157E              ; D0 F4

; Set vars

            lda #$FF               ; A9 FF
            sta L0003              ; Death
            sta L0006              ; 85 06

            lda #$00               ; A9 00
            sta L0001              ; Horizontal direction
            sta L0028              ; 85 28
            sta L0029              ; 85 29
            sta L002B              ; 85 2B
            sta L002F              ; 85 2F
            sta L0026              ; 85 26
            sta L0025              ; 85 25
            sta L0024              ; 85 24
            sta L002A              ; 85 2A

            sta L0035              ; Score hb
            sta L0036              ; Score mb
            sta L0037              ; Score lb

            sta L003C              ; 85 3C

            lda L0380               ; A9 01
            sta L003D              ; Levelnr

            lda #$06               ; A9 06
            sta L003B              ; Lives

            lda #$0F               ; A9 0F
            sta L0027              ; 85 27

            lda #$0A               ; A9 0A
            sta L0000                ; 85 00

            lda #$40               ; A9 40
            sta L0011              ; 85 11

            lda #$30               ; A9 30
            sta L0012              ; 85 12

            lda #$54               ; A9 54
            sta L0013              ; 85 13

            lda #$E0               ; A9 E0
            sta L0014              ; 85 14

            lda #$48               ; A9 48
            sta L0004              ; X-pos plane

            lda #$50               ; A9 50
            sta L0005              ; Y-pos plane

            jsr L1DC5              ; 20 C5 1D

            lda #$30               ; A9 30
            sta L0034              ; 85 34

            rts                    ; 60

;--------------------------------------------
; Game loop
;--------------------------------------------

L15DC:      jsr L17EF              ; Scan keyboard
            jsr L180C              ; 20 0C 18
            jsr L1805              ; 20 05 18
            jsr L19AB              ; Adjust horizontal position plane
            jsr L1816              ; 20 16 18

            lda L0034              ; A5 34
            bne L1665              ; D0 76

            lda #$00               ; A9 00
            sta L002A              ; 85 2A

            ldx #$32               ; A2 32
L15F5:      ldy #$32               ; A0 32

L15F7:      lda #$02               ; 2 Points
            jsr L174F              ; add to score

            stx L001D              ; 86 1D
            sty L001E              ; 84 1E
            jsr L1771              ; Print score on screen

            ldy L001E              ; A4 1E
            ldx L001D              ; A6 1D
            dey                    ; 88
            bne L15F7              ; D0 ED
            dex                    ; CA
            bne L15F5              ; D0 E8

            lda L003D              ; Check if a levels played
            cmp #$04               ; C9 04
            beq L1676              ; If so finish

            inc L003D              ; Increment Levelnr

;            lda #$15               ; Flush keyboard buffer
;            ldx #$00               ; A2 00
;            ldy #$00               ; A0 00
;            jsr LFFF4              ; 20 F4 FF

            cli                    ; 58

;            lda #$0C               ; CLS
;            jsr LFFEE              ; Write character
	jsr vdu12

            jsr L16DF              ; Load levelfile

;            lda #$15               ; Flush keyboard buffer
;            ldx #$00               ; A2 00
;            ldy #$00               ; A0 00
;            jsr LFFF4              ; 20 F4 FF

;            sei                    ; 78

; Reset vars

            lda #$FF               ; A9 FF
            sta L0006              ; 85 06

            lda #$00               ; A9 00
            sta L0001              ; Horizontal direction

            sta L0028              ; 85 28
            sta L0029              ; 85 29
            sta L002B              ; 85 2B
            sta L002F              ; 85 2F
            sta L0026              ; 85 26
            sta L0025              ; 85 25
            sta L0024              ; 85 24
            sta L002A              ; 85 2A

            lda #$0F               ; A9 0F
            sta L0027              ; 85 27

            lda #$0A               ; A9 0A
            sta $00                ; 85 00

            lda #$48               ; A9 48
            sta L0004              ; X-pos plane

            lda #$50               ; A9 50
            sta L0005              ; Y-pos plane

            jsr L1DC5              ; 20 C5 1D

            lda #$30               ; A9 30
            sta L0034              ; 85 34

            inc L003B              ; Lives
            jmp L15DC              ; 4C DC 15

;--------------------------------------------

L1665:      jsr L1841              ; 20 41 18
            jsr L21A5              ; 20 A5 21
            jsr L1771              ; Print score on screen
            jsr L1761              ; Print lives on screen
	jsr L1761h	; Print Hiscore on screen
	jsr L1762	; Print Level

            lda #$00               ; A9 00
            sta L0001              ; Horizontal direction

            rts                    ; 60

;--------------------------------------------
; Level finish message
;--------------------------------------------

L1676:      ldx #$05+1               ; A2 05
            ldy #$02+1               ; A0 02
            jsr L1736              ; Move cursor to X,Y

            ldy #$5C               ; Print Well done!
            ldx #$0A               ; A2 0A
            jsr L1744              ; 20 44 17

            jsr L16CC              ; Waitloop
            ldx #$01+1               ; A2 01
            ldy #$04+1               ; A0 04
            jsr L1736              ; Move cursor to X,Y

            ldy #$66               ; Print You have destroyed
            ldx #$12               ; A2 12
            jsr L1744              ; 20 44 17

            jsr L16CC              ; Waitloop

            ldx #$05+1               ; A2 05
            ldy #$06+1               ; A0 06
            jsr L1736              ; Move cursor to X,Y

            ldy #$78               ; Print ARTROCK 6
            ldx #$09               ; A2 09
            jsr L1744              ; 20 44 17

            jsr L16CC              ; Waitloop
            jsr L16CC              ; Waitloop
            jsr L16CC              ; Waitloop
            jsr L16CC              ; Waitloop
            jsr L16CC              ; Waitloop

            lda #$FF               ; A9 FF
            sta L003B              ; Lives
            rts                    ; 60

;--------------------------------------------
; Game over message
;--------------------------------------------

L16BA:
	lda #$40
	sta $b80e
	jsr reset

            ldx #$06+1               ; A2 06
            ldy #$0A+1               ; A0 0A
            jsr L1736              ; Move cursor to X,Y

            ldy #$81               ; Print Game over
            ldx #$09               ; A2 09
            jsr L1744              ; 20 44 17

            jsr L16CC              ; Waitloop
            rts                    ; 60

;--------------------------------------------
; Waitloop
;--------------------------------------------

L16CC:      lda #$0F               ; A9 0F
            sta L002D              ; 85 2D
            lda #$FF               ; A9 FF
            tax                    ; AA
            tay                    ; A8
L16D4:      dex                    ; CA
            bne L16D4              ; D0 FD
            dey                    ; 88
            bne L16D4              ; D0 FA
            dec L002D              ; C6 2D
            bne L16D4              ; D0 F6
            rts                    ; 60

;--------------------------------------------
; Load level
;--------------------------------------------

L16DF:
            ldy #$33               ; Print Searching for Level
            ldx #$13               ; A2 13
            jsr L1744              ; 20 44 17

            lda L003D              ; Get levelnr
            clc                    ; 18
            adc #$30               ; 69 30
            sta L172F              ; Set in loadfile
            jsr LFFEE              ; Print levelnr

            jsr LFFE7              ; Newline

;            ldy #$46               ; Print Press play
;            ldx #$0A               ; A2 0A
;            jsr L1744              ; 20 44 17
;
;            iny                    ; C8
;            iny                    ; C8
;            iny                    ; C8
;            iny                    ; C8
;            ldx #$08               ; A2 08
;            jsr L1744              ; Print on tape
;
;            jsr LFFE7              ; Newline

            ldx #<L1725            ; OSCLI loadfile
            ldy #>L1725            ; A0 17
            jsr LFFF7              ; 20 F7 FF

            jsr LFFE7              ; Newline
;
;            ldy #$46               ; Print Press
;            ldx #$06               ; A2 06
;            jsr L1744              ; 20 44 17
;
;            iny                    ; C8
;            iny                    ; C8
;            iny                    ; C8
;            iny                    ; C8
;            ldx #$0C               ; A2 0C
;            jsr L1744              ; Print stop on tape

            ldy #$61               ; Print Done
            ldx #$04               ; A2 06
            jsr L1744              ; 20 44 17
            jsr LFFE7              ; Newline

            jsr L16CC              ; Waitloop
            rts                    ; 60

;--------------------------------------------
; Load level data
;--------------------------------------------

L1725:
            .byte <L4D00,>L4D00,"LEVEL"
L172F:      .byte "1"
            .byte $0D

;--------------------------------------------
; Move cursor to X,Y
;--------------------------------------------

L1736:
	stx XPOS
	sty YPOS

;            lda #$1F               ; Move cursor
;            jsr LFFEE              ; Write character
;            txa                    ; 8A
;            jsr LFFEE              ; 20 EE FF
;            tya                    ; 98
;            jsr LFFEE              ; 20 EE FF
            rts                    ; 60

;--------------------------------------------
; Print text
;
; Y=pointer
; X=aantal
;--------------------------------------------

L1744:      lda L145F,Y            ; B9 5F 14
            jsr LFFEE              ; Write character
            iny                    ; C8
            dex                    ; CA
            bne L1744              ; D0 F6
            rts                    ; 60

;--------------------------------------------
; Add A to score
;--------------------------------------------

L174F:      clc                    ; 18
            adc L0037              ; Score lb
            sta L0037              ; Score lb
            lda L0036              ; Score mb
            adc #$00               ; 69 00
            sta L0036              ; Score mb
            lda L0035              ; Score hb
            adc #$00               ; 69 00
            sta L0035              ; Score hb
            rts                    ; 60

;--------------------------------------------
; Print Lives on screen
;--------------------------------------------

lives_addr	= $9119	; 25,17

L1761:      lda #<lives_addr       ; Set screenaddress
            sta L001B              ; 85 1B
            lda #>lives_addr       ; A9 5A
            sta L001C              ; 85 1C

            lda L003B              ; Print lives
            and #$0F               ; 29 0F
            jsr L17C8              ; 20 C8 17

            rts                    ; 60

;--------------------------------------------
; Print Level on screen
;--------------------------------------------

level_addr	= $911c	; 25,17

L1762:      lda #<level_addr       ; Set screenaddress
            sta L001B              ; 85 1B
            lda #>level_addr       ; A9 5A
            sta L001C              ; 85 1C

            lda L003D              ; Print lives
            and #$0F               ; 29 0F
            jsr L17C8              ; 20 C8 17

            rts                    ; 60

;--------------------------------------------
; Print score on screen
;--------------------------------------------

score_addr	= $8e18	; 20,18

L1771:      lda #<score_addr       ; A9 40
            sta L001B              ; 85 1B
            lda #>score_addr       ; A9 5A
            sta L001C              ; 85 1C

            lda L0035              ; Score hb
            sta L003A              ; 85 3A

            lda L0036              ; Score mb
            sta L0039              ; 85 39

            lda L0037              ; Score lb
            sta L0038              ; 85 38

            ldy #$00               ; A0 00
L1787:      ldx #$18               ; A2 18
            lda #$00               ; A9 00
L178B:      asl L0038              ; 06 38
            rol L0039              ; 26 39
            rol L003A              ; 26 3A
            rol A                  ; 2A
            cmp #$0A               ; C9 0A
            bcc L179A              ; 90 04
            sbc #$0A               ; E9 0A
            inc L0038              ; E6 38
L179A:      dex                    ; CA
            bne L178B              ; D0 EE

            pha                    ; 48
            iny                    ; C8
            lda L003A              ; A5 3A
            ora L0039              ; 05 39
            ora L0038              ; 05 38
            bne L1787              ; D0 E0

L17A7:      cpy #$06               ; C0 06
            beq L17B2              ; F0 07
            iny                    ; C8
            lda #$00               ; A9 00
            pha                    ; 48
            jmp L17A7              ; 4C A7 17

L17B2:      pla                    ; 68
            jsr L17C8              ; 20 C8 17
            dey                    ; 88
            bne L17B2              ; D0 F9
            rts                    ; 60

            pha                    ; 48
            lsr A                  ; 4A
            lsr A                  ; 4A
            lsr A                  ; 4A
            jsr L17C8              ; 20 C8 17
            pla                    ; 68
            and #$0F               ; 29 0F
            jsr L17C8              ; 20 C8 17
            rts                    ; 60

; Print character A

L17C8:      stx L002D              ; Save X
            sty L002E              ; Save Y

            asl A                  ; Set numdata addres
            asl A                  ; 0A
            asl A                  ; 0A
            tax                    ; AA

            ldy #$00               ; A0 00
L17D2:      lda L27F0,X            ; BD F0 27
            sta (L001B),Y          ; 91 1B
	clc
	lda L001B
	adc #$1f
	sta L001B
	lda L001C
	adc #0
	sta L001C

            inx                    ; E8
            iny                    ; C8
            cpy #$08               ; C0 08
            bne L17D2              ; D0 F5

            lda L001B              ; A5 1B
            sec                    ; 18
            sbc #$f7               ; 69 08
            sta L001B              ; 85 1B
            lda L001C              ; A5 1C
            sbc #$00               ; 69 00
            sta L001C              ; 85 1C

            ldy L002E              ; A4 2E
            ldx L002D              ; A6 2D
            rts                    ; 60

;--------------------------------------------
; Print Hiscore on screen
;--------------------------------------------

hiscore_addr	= $9518	; 20,21

L1761h:      lda #<hiscore_addr       ; A9 40
            sta L001B              ; 85 1B
            lda #>hiscore_addr       ; A9 5A
            sta L001C              ; 85 1C

            lda L003F              ; Score hb
            sta L003A              ; 85 3A

            lda L0040              ; Score mb
            sta L0039              ; 85 39

            lda L0041              ; Score lb
            sta L0038              ; 85 38

            ldy #$00               ; A0 00
L1787h:      ldx #$18               ; A2 18
            lda #$00               ; A9 00
L178Bh:      asl L0038              ; 06 38
            rol L0039              ; 26 39
            rol L003A              ; 26 3A
            rol A                  ; 2A
            cmp #$0A               ; C9 0A
            bcc L179Ah              ; 90 04
            sbc #$0A               ; E9 0A
            inc L0038              ; E6 38
L179Ah:      dex                    ; CA
            bne L178Bh              ; D0 EE

            pha                    ; 48
            iny                    ; C8
            lda L003A              ; A5 3A
            ora L0039              ; 05 39
            ora L0038              ; 05 38
            bne L1787h              ; D0 E0

L17A7h:      cpy #$06               ; C0 06
            beq L17B2h              ; F0 07
            iny                    ; C8
            lda #$00               ; A9 00
            pha                    ; 48
            jmp L17A7h              ; 4C A7 17

L17B2h:      pla                    ; 68
            jsr L17C8              ; 20 C8 17
            dey                    ; 88
            bne L17B2h              ; D0 F9
            rts                    ; 60

            pha                    ; 48
            lsr A                  ; 4A
            lsr A                  ; 4A
            lsr A                  ; 4A
            jsr L17C8              ; 20 C8 17
            pla                    ; 68
            and #$0F               ; 29 0F
            jsr L17C8              ; 20 C8 17
            rts                    ; 60


;--------------------------------------------
; Scan keyboard
;--------------------------------------------

L17EF:
            jsr L19C5              ; Keyboard scan left/right, Z/X
            jsr L19EE              ; Keyboard scan right, X
            jsr L1A0B              ; Keyboard scan up, :
            jsr L1A26              ; Keyboard scan left, Z
            jsr L1A41              ; Keyboardscan fire, RETURN
            jsr L1A54              ; Keyboardscan freeze/unfreeze, DELETE/COPY
            jsr L1A6D              ; Keyboardscan fire, ????
            rts                    ; 60

;--------------------------------------------

L1805:      jsr L189F              ; 20 9F 18
            jsr L1852              ; 20 52 18
            rts                    ; 60

;--------------------------------------------

L180C:      jsr L1984              ; Adjust vertical position plane
            jsr L1C3D              ; 20 3D 1C
            jsr L1D8E              ; 20 8E 1D
            rts                    ; 60

;--------------------------------------------

L1816:      lda L0029              ; A5 29
            bne L1827              ; D0 0D
            inc L0027              ; E6 27

            lda L0027              ; A5 27
            and #$0F               ; 29 0F
            sta L0027              ; 85 27
            bne L1827              ; D0 03

            jsr L1DFA              ; 20 FA 1D

L1827:      lda L0029              ; A5 29
            beq L182D              ; F0 02
            dec L0034              ; C6 34

L182D:      lda L0028              ; A5 28
            cmp #$75               ; C9 75
            bne L1837              ; D0 04

            lda #$FF               ; A9 FF
            sta L0029              ; 85 29

L1837:      jsr L2050              ; 20 50 20
            jsr L1E44              ; 20 44 1E
            jsr L1E8C              ; 20 8C 1E

            rts                    ; 60

;--------------------------------------------

L1841:      jsr L1DBE              ; 20 BE 1D
            jsr L1D95              ; 20 95 1D
            jsr L1A7E              ; 20 7E 1A
            rts                    ; 60

            jsr L1AAB              ; 20 AB 1A
            jsr L1AD0              ; 20 D0 1A
            rts                    ; 60

;--------------------------------------------

L1852:      ldy L003C              ; A4 3C
            lda L1D5F,Y            ; B9 5F 1D
            sta L003C              ; 85 3C
            lda L0025              ; A5 25
            beq L189E              ; F0 41
            ldx #$00               ; A2 00
            stx L002D              ; 86 2D

;--------------------------------------------

L1861:      lda L0004              ; X-pos plane
            clc                    ; 18
            adc #$03               ; 69 03
            sta L0030              ; 85 30

            lda L0005              ; Y-pos plane
            clc                    ; 18
            adc #$1C               ; 69 1C
            sta L0031              ; 85 31

            ldx L002D              ; A6 2D
            lda L5628,X            ; BD 28 56
            cmp L0004              ; X-pos plane
            bcc L1896              ; 90 1E

            cmp L0030              ; C5 30
            bcs L1896              ; B0 1A

            lda L5658,X            ; BD 58 56
            cmp L0005              ; Y-pos plane
            bcc L1896              ; 90 13

            cmp L0031              ; C5 31
            bcs L1896              ; B0 0F

            lda L003C              ; A5 3C
            bne L1891              ; D0 06

            lda #$04               ; A9 04
            sta L003C              ; 85 3C
            dec L003B              ; Lives

L1891:      lda #$00               ; A9 00
            sta L5688,X            ; 9D 88 56

L1896:      inc L002D              ; E6 2D
            lda L002D              ; A5 2D
            cmp L0025              ; C5 25
            bne L1861              ; D0 C3

L189E:      rts                    ; 60

;--------------------------------------------

L189F:      lda L0024              ; A5 24
            beq L1902              ; F0 5F
            clc                    ; 18
            asl A                  ; 0A
            tax                    ; AA
L18A6:      dex                    ; CA
            lda L5608,X            ; BD 08 56
            sta L0031              ; 85 31
            dex                    ; CA
            lda L5608,X            ; BD 08 56
            sta L0030              ; 85 30
            stx L0032              ; 86 32
            ldx L0030              ; A6 30
            ldy L0031              ; A4 31
            jsr L196E              ; 20 6E 19
            stx L0030              ; 86 30
            sty L0031              ; 84 31
            jsr L1EBA              ; 20 BA 1E
            cmp #$AF               ; C9 AF
            beq L18E1              ; F0 1B
            cmp #$BB               ; C9 BB
            bcc L18FE              ; 90 34
            cmp #$CC               ; C9 CC
            bcc L18E1              ; 90 13
            cmp #$D4               ; C9 D4
            bcc L18FE              ; 90 2C
            cmp #$DC               ; C9 DC
            bcc L18E1              ; 90 0B
            cmp #$E6               ; C9 E6
            bcc L18FE              ; 90 24
            cmp #$EA               ; C9 EA
            bcc L18EF              ; 90 11
            jmp L18FE              ; 4C FE 18

;--------------------------------------------

L18E1:      lda #$32               ; 50 Points
            jsr L174F              ; add to score

            lda #$DC               ; A9 DC
            ldx L0030              ; A6 30
            ldy L0031              ; A4 31
            jsr L1EC9              ; 20 C9 1E

L18EF:      lda #$0A               ; 10 Points
            jsr L174F              ; add to score

            ldx L0032              ; A6 32
            lda #$00               ; A9 00
            sta L5608,X            ; 9D 08 56
            jmp L18FE              ; 4C FE 18
L18FE:      ldx L0032              ; A6 32
            bne L18A6              ; D0 A4
L1902:      lda L0024              ; A5 24
            beq L196D              ; F0 67
            clc                    ; 18
            asl A                  ; 0A
            tax                    ; AA
L1909:      dex                    ; CA
            lda L5608,X            ; BD 08 56
            sta L0031              ; 85 31
            dex                    ; CA
            lda L5608,X            ; BD 08 56
            sta L0030              ; 85 30
            inc L0030              ; E6 30
            inc L0030              ; E6 30
            inc L0031              ; E6 31
            inc L0031              ; E6 31
            stx L0032              ; 86 32
            lda L0026              ; A5 26
            beq L1969              ; F0 46

            ldx #$00               ; A2 00
L1925:      lda L5600,X            ; BD 00 56
            clc                    ; 18
            adc #$03               ; 69 03
            cmp L0030              ; C5 30
            bcc L1964              ; 90 35

            lda L5600,X            ; BD 00 56
            cmp L0030              ; C5 30
            bcs L1964              ; B0 2E

            lda L5602,X            ; BD 02 56
            cmp L0031              ; C5 31
            bcs L1964              ; B0 27

            lda L5602,X            ; BD 02 56
            clc                    ; 18
            adc #$18               ; 69 18
            cmp L0031              ; C5 31
            bcc L1964              ; 90 1D

            lda L5606,X            ; BD 06 56
            cmp #$04               ; C9 04
            beq L1964              ; F0 16

            cmp #$05               ; C9 05
            beq L1964              ; F0 12

            cmp #$06               ; C9 06
            beq L1964              ; F0 0E

            cmp #$07               ; C9 07
            beq L1964              ; F0 0A

            lda #$04               ; A9 04
            sta L5606,X            ; 9D 06 56

            lda #$64               ; 100 points
            jsr L174F              ; add to score

L1964:      inx                    ; E8
            cpx L0026              ; E4 26
            bne L1925              ; D0 BC
L1969:      ldx L0032              ; A6 32
            bne L1909              ; D0 9C
L196D:      rts                    ; 60

;--------------------------------------------

L196E:      tya                    ; 98
            clc                    ; 18
            adc L0027              ; 65 27
            adc #$14               ; 69 14
            eor #$FF               ; 49 FF
            lsr A                  ; 4A
            lsr A                  ; 4A
            lsr A                  ; 4A
            lsr A                  ; 4A
            tay                    ; A8
            txa                    ; 8A
            sec                    ; 38
            sbc $00                ; E5 00
            sbc #$30               ; E9 30
            lsr A                  ; 4A
            tax                    ; AA
            rts                    ; 60

;--------------------------------------------
; Adjust vertical position plane
;--------------------------------------------

L1984:      lda L0002              ; Vertical direction
            beq L19A6              ; F0 1E

            cmp #$01               ; C9 01
            beq L1993              ; F0 07

            cmp #$02               ; C9 02
            beq L199E              ; F0 0E
            jmp L19A6              ; 4C A6 19

L1993:      inc L0005              ; Y-pos plane
            inc L0005              ; Y-pos plane
            inc L0005              ; Y-pos plane
            inc L0005              ; Y-pos plane
            jmp L19A6              ; 4C A6 19

L199E:      dec L0005              ; Y-pos plane
            dec L0005              ; Y-pos plane
            dec L0005              ; Y-pos plane
            dec L0005              ; Y-pos plane

L19A6:      lda #$00               ; A9 00
            sta L0002              ; Vertical direction
            rts                    ; 60

;--------------------------------------------
; Adjust horizontal position plane
;--------------------------------------------

L19AB:      lda L0001              ; Horizontal direction
            beq L19B7              ; F0 08

            cmp #$02               ; C9 02
            beq L19BA              ; F0 07

            cmp #$01               ; C9 01
            beq L19BF              ; F0 08

L19B7:      jmp L19C4              ; 4C C4 19

L19BA:      dec L0000                ; C6 00
            jmp L19C4              ; 4C C4 19

L19BF:      inc L0000                ; E6 00
            jmp L19C4              ; 4C C4 19

L19C4:      rts                    ; 60

;--------------------------------------------
; Keyboard scan left/right, Z/X
;--------------------------------------------

L19C5:      lda L003C              ; A5 3C
            bne L19ED              ; D0 24

;            lda #$81               ; Keyboardscan Z
;            ldy #$FF               ; A0 FF
;            ldx #$9E               ; A2 9E
;            jsr LFFF4              ; 20 F4 FF
;            tya                    ; 98
;            bpl L19ED              ; 10 18

	jsr scan_z
	bne L19ED

;            lda #$81               ; Keyboardscan X
;            ldy #$FF               ; A0 FF
;            ldx #$BD               ; A2 BD
;            jsr LFFF4              ; 20 F4 FF
;            tya                    ; 98
;            bmi L19ED              ; 30 0C

	jsr scan_x
	beq L19ED

            lda L0000                ; A5 00
            cmp #$10               ; C9 10
            beq L19ED              ; F0 06
 
            lda #$01               ; A9 01
            sta L0001              ; Horizontal direction
            dec L0004              ; X-pos plane

L19ED:      rts                    ; 60

;--------------------------------------------
; Keyboard scan right, X
;--------------------------------------------

L19EE:      lda L003C              ; A5 3C
            bne L1A0A              ; D0 18

;            lda #$81               ; Keyboardscan X
;            ldy #$FF               ; A0 FF
;            ldx #$BD               ; A2 BD
;            jsr LFFF4              ; 20 F4 FF
;            tya                    ; 98
;            bpl L1A0A              ; 10 0C

	jsr scan_x
	bne L1A0A

            lda L0000                ; A5 00
            cmp #$04               ; C9 04
            beq L1A0A              ; F0 06

            lda #$02               ; A9 02
            sta L0001              ; Horizontal direction
            inc L0004              ; X-pos plane

L1A0A:      rts                    ; 60

;--------------------------------------------
; Keyboard scan up, ;
;--------------------------------------------

L1A0B:      lda L003C              ; A5 3C
            bne L1A25              ; D0 16

;            lda #$81               ; Keyboardscan :
;            ldy #$FF               ; A0 FF
;            ldx #$B7               ; A2 B7
;            jsr LFFF4              ; 20 F4 FF
;            tya                    ; 98
;            bpl L1A25              ; 10 0A

	jsr scan_u
	bne L1A25              

            lda L0005              ; Y-pos plane
            cmp #$96               ; C9 96
            bcs L1A25              ; B0 04
            lda #$01               ; A9 01
            sta L0002              ; Vertical direction

L1A25:      rts                    ; 60

;--------------------------------------------
; Keyboard scan down, .
;--------------------------------------------

L1A26:      lda L003C              ; A5 3C
            bne L1A40              ; D0 16

;            lda #$81               ; Keyboardscan .
;            ldy #$FF               ; A0 FF
;            ldx #$97               ; A2 97
;            jsr LFFF4              ; 20 F4 FF
;            tya                    ; 98
;            bpl L1A40              ; 10 0A

	jsr scan_d
	bne L1A40              

            lda L0005              ; Y-pos plane
            cmp #$30               ; C9 30
            bcc L1A40              ; 90 04

            lda #$02               ; A9 02
            sta L0002              ; Vertical direction

L1A40:      rts                    ; 60

;--------------------------------------------
; Keyboardscan fire, RETURN
;--------------------------------------------

L1A41:      lda L003C              ; A5 3C
            bne L1A53              ; D0 0E

;            lda #$81               ; Keyboardscan RETURN
;            ldy #$FF               ; A0 FF
;            ldx #$B6               ; A2 B6
;            jsr LFFF4              ; 20 F4 FF
;            tya                    ; 98
;            bpl L1A53              ; 10 02

	jsr scan_a
	bne L1A53              

            inc L0006              ; E6 06

	lda L0006
	bne L1A53
	lda #<K1D3E		; Fire sound
	ldy #>K1D3E
	jsr K1006


L1A53:      rts                    ; 60

;--------------------------------------------
; Keyboardscan freeze/unfreeze, DELETE/COPY
;--------------------------------------------

L1A54:
;            lda #$81               ; Keyboardscan DELETE
;            ldy #$FF               ; A0 FF
;            ldx #$A6               ; A2 A6
;            jsr LFFF4              ; 20 F4 FF
;            tya                    ; 98
;            bpl L1A6C              ; 10 0C

	lda #$f4	; Scan DELETE-key
	sta $b000
	lda $b001
;	eor #$ff
	and #$2
	bne L1A6C              

L1A60:
;            lda #$81               ; Keyboardscan COPY
;            ldy #$FF               ; A0 FF
;            ldx #$96               ; A2 96
;            jsr LFFF4              ; 20 F4 FF
;            tya                    ; 98
;            bpl L1A60              ; 10 F4

	lda #$f5	; Scan COPY-key
	sta $b000
	lda $b001
;	eor #$ff
	and #$2
	bne L1A60              

L1A6C:      rts                    ; 60

;--------------------------------------------
; Keyboardscan end, E
;--------------------------------------------

L1A6D:
;            lda #$81               ; Keyboardscan Q
;            ldy #$FF               ; A0 FF
;            ldx #$EF               ; A2 EF
;            jsr LFFF4              ; 20 F4 FF
;            tya                    ; 98
;            bpl L1A7D              ; 10 04

	lda #$f2	; Scan E-key
	sta $b000
	lda $b001
;	eor #$ff
	and #$8
	bne L1A7D              

            lda #$00               ; A9 00
            sta L0003              ; Death

L1A7D:      rts                    ; 60

;--------------------------------------------

L1A7E:      lda L003C              ; A5 3C
            bne L1A94              ; D0 12
            lda #$04               ; A9 04
            sta L000F              ; 85 0F
            lda #$28               ; A9 28
            sta L0010              ; 85 10
            ldx L0004              ; X-pos plane
            lda #$00               ; A9 00
            ldy L0005              ; Y-pos plane
            jsr L233F              ; 20 3F 23
            rts                    ; 60

;--------------------------------------------

L1A94:      lda #$03               ; A9 03
            sta L000F              ; 85 0F
            lda #$18               ; A9 18
            sta L0010              ; 85 10
            ldx L0004              ; X-pos plane
            inx                    ; E8
            lda L0005              ; Y-pos plane
            clc                    ; 18
            adc #$08               ; 69 08
            tay                    ; A8
            lda L003C              ; A5 3C
            jsr L233F              ; 20 3F 23
            rts                    ; 60

;--------------------------------------------

L1AAB:      lda #$02               ; A9 02
            sta L000F              ; 85 0F
            lda #$08               ; A9 08
            sta L0010              ; 85 10
            lda L0024              ; A5 24
            beq L1ACF              ; F0 18
            clc                    ; 18
            asl A                  ; 0A
            tax                    ; AA
L1ABA:      dex                    ; CA
            lda L5608,X            ; BD 08 56
            tay                    ; A8
            dex                    ; CA
            txa                    ; 8A
            pha                    ; 48
            lda L5608,X            ; BD 08 56
            tax                    ; AA
            lda #$08               ; A9 08
            jsr L233F              ; 20 3F 23
            pla                    ; 68
            tax                    ; AA
            bne L1ABA              ; D0 EB
L1ACF:      rts                    ; 60

;--------------------------------------------

L1AD0:      lda #$02               ; A9 02
            sta L000F              ; 85 0F
            lda #$08               ; A9 08
            sta L0010              ; 85 10
            lda L0025              ; A5 25
            beq L1AF1              ; F0 15
            tax                    ; AA
L1ADD:      dex                    ; CA
            lda L5658,X            ; BD 58 56
            tay                    ; A8
            txa                    ; 8A
            pha                    ; 48
            lda L5628,X            ; BD 28 56
            tax                    ; AA
            lda #$08               ; A9 08
            jsr L233F              ; 20 3F 23
            pla                    ; 68
            tax                    ; AA
            bne L1ADD              ; D0 EC
L1AF1:      rts                    ; 60

;--------------------------------------------

L1AF2:      lda L0006              ; A5 06
            cmp #$00               ; C9 00
            bne L1B18              ; D0 20

            lda #$FE               ; A9 FE
            sta L0006              ; 85 06

            lda L0024              ; A5 24
            cmp #$40               ; C9 40
            beq L1B18              ; F0 16
            inc L0024              ; E6 24

            clc                    ; 18
            asl A                  ; 0A
            tax                    ; AA
            lda L0004              ; X-pos plane
            clc                    ; 18
            adc #$01               ; 69 01
            sta L5608,X            ; 9D 08 56
            inx                    ; E8

            lda L0005              ; Y-pos plane
            clc                    ; 18
            adc #$20               ; 69 20
            sta L5608,X            ; 9D 08 56

L1B18:      ldx #$00               ; A2 00
            ldy #$00               ; A0 00
            lda L0024              ; A5 24
            beq L1B4D              ; F0 2D
            asl A                  ; 0A
            sta L0022              ; 85 22
L1B23:      lda L5608,X            ; BD 08 56
            beq L1B45              ; F0 1D
            sta L5608,Y            ; 99 08 56
            inx                    ; E8
            iny                    ; C8
            lda L5608,X            ; BD 08 56
            cmp #$DC               ; C9 DC
            bcs L1B49              ; B0 15
            sta L5608,Y            ; 99 08 56
L1B37:      inx                    ; E8
            iny                    ; C8
            cpx L0022              ; E4 22
            bne L1B23              ; D0 E6
            tya                    ; 98
            clc                    ; 18
            lsr A                  ; 4A
            sta L0024              ; 85 24
            jmp L1B4D              ; 4C 4D 1B

;--------------------------------------------

L1B45:      inx                    ; E8
            dey                    ; 88
            bne L1B37              ; D0 EE
L1B49:      dey                    ; 88
            dey                    ; 88
            bne L1B37              ; D0 EA
L1B4D:      lda L0024              ; A5 24
            beq L1B72              ; F0 21
            clc                    ; 18
            asl A                  ; 0A
            tax                    ; AA
L1B54:      dex                    ; CA
            lda L5608,X            ; BD 08 56
            adc #$08               ; 69 08
            sta L5608,X            ; 9D 08 56
            dex                    ; CA
            lda L0001              ; Horizontal direction
            beq L1B6E              ; F0 0C
            cmp #$01               ; C9 01
            beq L1B6B              ; F0 05
            dec L5608,X            ; DE 08 56
            bne L1B6E              ; D0 03
L1B6B:      inc L5608,X            ; FE 08 56
L1B6E:      cpx #$00               ; E0 00
            bne L1B54              ; D0 E2
L1B72:      rts                    ; 60

;--------------------------------------------

L1B73:      lda L0025              ; A5 25
            bne L1B7A              ; D0 03
            jmp L1C3C              ; 4C 3C 1C

;--------------------------------------------

L1B7A:      ldx #$00               ; A2 00
            stx L002D              ; 86 2D
L1B7E:      ldx L002D              ; A6 2D
            lda L5658,X            ; BD 58 56
            cmp #$DC               ; C9 DC
            bcc L1B8C              ; 90 05
            lda #$00               ; A9 00
            sta L5688,X            ; 9D 88 56
L1B8C:      cmp #$32               ; C9 32
            bcs L1B95              ; B0 05
            lda #$00               ; A9 00
            sta L5688,X            ; 9D 88 56
L1B95:      lda L5628,X            ; BD 28 56
            cmp #$5F               ; C9 5F
            bcc L1BA1              ; 90 05
            lda #$00               ; A9 00
            sta L5688,X            ; 9D 88 56
L1BA1:      cmp #$30               ; C9 30
            bcs L1BAA              ; B0 05
            lda #$00               ; A9 00
            sta L5688,X            ; 9D 88 56
L1BAA:      lda L0001              ; Horizontal direction
            beq L1BBA              ; F0 0C
            cmp #$01               ; C9 01
            beq L1BB7              ; F0 05
            dec L5628,X            ; DE 28 56
            bne L1BBA              ; D0 03
L1BB7:      inc L5628,X            ; FE 28 56
L1BBA:      inc L002D              ; E6 2D
            lda L002D              ; A5 2D
            cmp L0025              ; C5 25
            bne L1B7E              ; D0 BC
            ldx #$00               ; A2 00
            stx L002E              ; 86 2E
            stx L002D              ; 86 2D
L1BC8:      ldy L002E              ; A4 2E
            ldx L002D              ; A6 2D
            lda L5688,Y            ; B9 88 56
            bne L1BD6              ; D0 05
            inc L002E              ; E6 2E
            jmp L1C32              ; 4C 32 1C
L1BD6:      lda L5688,Y            ; B9 88 56
            sta L5688,X            ; 9D 88 56
            lda L5628,Y            ; B9 28 56
            sta L5628,X            ; 9D 28 56
            lda L5658,Y            ; B9 58 56
            sta L5658,X            ; 9D 58 56
            lda L5688,X            ; BD 88 56
            and #$01               ; 29 01
            beq L1C01              ; F0 12
            dec L5658,X            ; DE 58 56
            dec L5658,X            ; DE 58 56
            dec L5658,X            ; DE 58 56
            dec L5658,X            ; DE 58 56
            dec L5658,X            ; DE 58 56
            dec L5658,X            ; DE 58 56
L1C01:      lda L5688,X            ; BD 88 56
            and #$08               ; 29 08
            beq L1C1A              ; F0 12
            inc L5658,X            ; FE 58 56
            inc L5658,X            ; FE 58 56
            inc L5658,X            ; FE 58 56
            inc L5658,X            ; FE 58 56
            inc L5658,X            ; FE 58 56
            inc L5658,X            ; FE 58 56
L1C1A:      lda L5688,X            ; BD 88 56
            and #$04               ; 29 04
            beq L1C24              ; F0 03
            dec L5628,X            ; DE 28 56
L1C24:      lda L5688,X            ; BD 88 56
            and #$02               ; 29 02
            beq L1C2E              ; F0 03
            inc L5628,X            ; FE 28 56
L1C2E:      inc L002D              ; E6 2D
            inc L002E              ; E6 2E
L1C32:      lda L002E              ; A5 2E
            cmp L0025              ; C5 25
            bne L1BC8              ; D0 90
            lda L002D              ; A5 2D
            sta L0025              ; 85 25
L1C3C:      rts                    ; 60

;--------------------------------------------

L1C3D:      lda L0026              ; A5 26
            cmp #$02               ; C9 02
            beq L1C6A              ; F0 27

            jsr L22E7              ; 20 E7 22
            and #$80               ; 29 80
            beq L1C6A              ; F0 20

            jsr L22E7              ; 20 E7 22
            and #$0F               ; 29 0F
            asl A                  ; 0A
            adc #$30               ; 69 30
            adc $00                ; 65 00

            ldx L0026              ; A6 26
            sta L5600,X            ; 9D 00 56

            lda #$FA               ; A9 FA
            sta L5602,X            ; 9D 02 56

            lda #$0D               ; A9 0D
            sta L5604,X            ; 9D 04 56

            lda #$01               ; A9 01
            sta L5606,X            ; 9D 06 56

            inc L0026              ; E6 26
L1C6A:      lda L0026              ; A5 26
            bne L1C71              ; D0 03
            jmp L1D5E              ; 4C 5E 1D

;--------------------------------------------

L1C71:      ldx #$00               ; A2 00
L1C73:      lda L5606,X            ; BD 06 56
            tay                    ; A8

            lda L1D5F,Y            ; B9 5F 1D
            sta L5606,X            ; 9D 06 56
            cmp #$04               ; C9 04
            bcs L1CF2              ; B0 71

            lda L5604,X            ; BD 04 56
            tay                    ; A8
            inc L5604,X            ; FE 04 56

            lda L1D67,Y            ; B9 67 1D
            bpl L1C99              ; 10 0C
            and #$7F               ; 29 7F
            sta L5604,X            ; 9D 04 56
            inc L5604,X            ; FE 04 56

            tay                    ; A8
            lda L1D67,Y            ; B9 67 1D

L1C99:      and #$40               ; 29 40
            beq L1CAD              ; F0 10
            lda L1D67,Y            ; B9 67 1D
            and #$1F               ; 29 1F
            sta L002D              ; 85 2D
            lda L5600,X            ; BD 00 56
            clc                    ; 18
            adc L002D              ; 65 2D
            sta L5600,X            ; 9D 00 56
L1CAD:      lda L1D67,Y            ; B9 67 1D
            and #$20               ; 29 20
            beq L1CC4              ; F0 10
            lda L1D67,Y            ; B9 67 1D
            and #$1F               ; 29 1F
            sta L002D              ; 85 2D
            lda L5600,X            ; BD 00 56
            sec                    ; 38
            sbc L002D              ; E5 2D
            sta L5600,X            ; 9D 00 56
L1CC4:      lda L1D7A,Y            ; B9 7A 1D
            and #$40               ; 29 40
            beq L1CDB              ; F0 10
            lda L1D7A,Y            ; B9 7A 1D
            and #$1F               ; 29 1F
            sta L002D              ; 85 2D
            lda L5602,X            ; BD 02 56
            clc                    ; 18
            adc L002D              ; 65 2D
            sta L5602,X            ; 9D 02 56
L1CDB:      lda L1D7A,Y            ; B9 7A 1D
            and #$20               ; 29 20
            beq L1CF2              ; F0 10
            lda L1D7A,Y            ; B9 7A 1D
            and #$1F               ; 29 1F
            sta L002D              ; 85 2D
            lda L5602,X            ; BD 02 56
            sec                    ; 38
            sbc L002D              ; E5 2D
            sta L5602,X            ; 9D 02 56
L1CF2:      lda L0001              ; Horizontal direction
            cmp #$00               ; C9 00
            beq L1D05              ; F0 0D
            cmp #$01               ; C9 01
            beq L1D02              ; F0 06
            dec L5600,X            ; DE 00 56
            dec L5600,X            ; DE 00 56
L1D02:      inc L5600,X            ; FE 00 56
L1D05:      lda L5600,X            ; BD 00 56
            cmp L0004              ; X-pos plane
            bne L1D1D              ; D0 11
            stx L002D              ; 86 2D
            lda L5602,X            ; BD 02 56
            tay                    ; A8
            lda L5600,X            ; BD 00 56
            tax                    ; AA
            lda #$01               ; A9 01
            jsr L217D              ; 20 7D 21
            ldx L002D              ; A6 2D
L1D1D:      lda L5602,X            ; BD 02 56
            cmp #$14               ; C9 14
            bcs L1D29              ; B0 05
            lda #$00               ; A9 00
            sta L5606,X            ; 9D 06 56
L1D29:      inx                    ; E8
            cpx L0026              ; E4 26
            beq L1D31              ; F0 03
            jmp L1C73              ; 4C 73 1C
L1D31:      ldx #$00               ; A2 00
            ldy #$00               ; A0 00
L1D35:      lda L5606,X            ; BD 06 56
            bne L1D3E              ; D0 04
            inx                    ; E8
            jmp L1D58              ; 4C 58 1D

;--------------------------------------------

L1D3E:      lda L5606,X            ; BD 06 56
            sta L5606,Y            ; 99 06 56
            lda L5600,X            ; BD 00 56
            sta L5600,Y            ; 99 00 56
            lda L5602,X            ; BD 02 56
            sta L5602,Y            ; 99 02 56
            lda L5604,X            ; BD 04 56
            sta L5604,Y            ; 99 04 56
            inx                    ; E8
            iny                    ; C8
L1D58:      cpx L0026              ; E4 26
            bne L1D35              ; D0 D9
            sty L0026              ; 84 26
L1D5E:      rts                    ; 60

;--------------------------------------------

L1D5F:      .byte $00,$03,$01,$02,$05,$06,$07,$00

L1D67:      .byte $41,$41,$41,$41,$41,$41,$41,$41
            .byte $41,$21,$21,$21,$21,$21,$21,$21
            .byte $21,$21,$80

L1D7A:      .byte $2C,$28,$23,$21,$00,$41,$43,$44
            .byte $48,$48,$44,$43,$41,$00,$21,$23
            .byte $24,$2C,$00,$60

;--------------------------------------------

L1D8E:      jsr L1AF2              ; 20 F2 1A
            jsr L1B73              ; 20 73 1B
            rts                    ; 60

;--------------------------------------------

L1D95:      lda L0026              ; A5 26
            beq L1DBD              ; F0 24
            lda #$03               ; A9 03
            sta L000F              ; 85 0F
            lda #$18               ; A9 18
            sta L0010              ; 85 10
            ldx #$00               ; A2 00
L1DA3:      lda L5602,X            ; BD 02 56
            tay                    ; A8
            lda L5600,X            ; BD 00 56
            sta L002D              ; 85 2D
            txa                    ; 8A
            pha                    ; 48
            lda L5606,X            ; BD 06 56
            ldx L002D              ; A6 2D
            jsr L233F              ; 20 3F 23
            pla                    ; 68
            tax                    ; AA
            inx                    ; E8
            cpx L0026              ; E4 26
            bne L1DA3              ; D0 E6
L1DBD:      rts                    ; 60

L1DBE:      jsr L1AAB              ; 20 AB 1A
            jsr L1AD0              ; 20 D0 1A
            rts                    ; 60

;--------------------------------------------
;--------------------------------------------

L1DC5:      lda #$00               ; A9 00
            sta L001B              ; 85 1B
            lda #$00               ; A9 00
            sta L001D              ; 85 1D

L1DCD:      lda L0028              ; A5 28
            clc                    ; 18
            adc L001D              ; 65 1D
            tay                    ; A8
            ldx L001B              ; A6 1B

            jsr L1E70              ; 20 70 1E

            pha                    ; 48
            ldx L001B              ; A6 1B
            lda #$0B               ; A9 0B
            sec                    ; 38
            sbc L001D              ; E5 1D
            tay                    ; A8
            pla                    ; 68

            jsr L1EC9              ; 20 C9 1E

            inc L001B              ; E6 1B
            lda L001B              ; A5 1B
            cmp #$10               ; C9 10
            bne L1DCD              ; D0 E0
            lda #$00               ; A9 00
            sta L001B              ; 85 1B
            inc L001D              ; E6 1D
            lda L001D              ; A5 1D
            cmp #$0C               ; C9 0C
            bne L1DCD              ; D0 D4
            rts                    ; 60

;--------------------------------------------

L1DFA:      inc L0028              ; E6 28
            ldx #$10               ; A2 10
            ldy #$0B               ; A0 0B
            stx L002D              ; 86 2D
            sty L002E              ; 84 2E
L1E04:      ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            dey                    ; 88
            jsr L1EBA              ; 20 BA 1E
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L1EC9              ; 20 C9 1E
            dec L002D              ; C6 2D
            bpl L1E04              ; 10 ED
            lda #$10               ; A9 10
            sta L002D              ; 85 2D
            dec L002E              ; C6 2E
            bne L1E04              ; D0 E5
            lda #$0B               ; A9 0B
            sta L001D              ; 85 1D
            lda #$0F               ; A9 0F
            sta L001B              ; 85 1B
L1E27:      lda L0028              ; A5 28
            clc                    ; 18
            adc L001D              ; 65 1D
            tay                    ; A8
            ldx L001B              ; A6 1B
            jsr L1E70              ; 20 70 1E
            pha                    ; 48
            ldx L001B              ; A6 1B
            lda #$0B               ; A9 0B
            sec                    ; 38
            sbc L001D              ; E5 1D
            tay                    ; A8
            pla                    ; 68
            jsr L1EC9              ; 20 C9 1E
            dec L001B              ; C6 1B
            bpl L1E27              ; 10 E4
            rts                    ; 60

;--------------------------------------------

L1E44:      ldy #$C0               ; A0 C0
            inc L002B              ; E6 2B
            lda L002B              ; A5 2B
            and #$0F               ; 29 0F
            bne L1E6F              ; D0 21
L1E4E:      lda L5700,Y            ; B9 00 57
            cmp #$AE               ; C9 AE
            bne L1E57              ; D0 02
            lda #$B0               ; A9 B0
L1E57:      cmp #$C0               ; C9 C0
            bne L1E5D              ; D0 02
            lda #$C1               ; A9 C1
L1E5D:      cmp #$C4               ; C9 C4
            bne L1E63              ; D0 02
            lda #$C5               ; A9 C5
L1E63:      cmp #$C8               ; C9 C8
            bne L1E69              ; D0 02
            lda #$C9               ; A9 C9
L1E69:      sta L5700,Y            ; 99 00 57
            dey                    ; 88
            bne L1E4E              ; D0 DF
L1E6F:      rts                    ; 60

;--------------------------------------------

L1E70:      lda #<L4D00               ; A9 00
            sta L0022              ; 85 22
            txa                    ; 8A
            lsr A                  ; 4A
            ror L0022              ; 66 22
            adc #>L4D00               ; Leveldata at $4D00
            sta L0008              ; 85 08
            tya                    ; 98
            adc L0022              ; 65 22
            adc #$00               ; 69 00
            sta L0007              ; 85 07
            bcc L1E87              ; 90 02
            inc L0008              ; E6 08
L1E87:      ldy #$00               ; A0 00
            lda (L0007),Y          ; B1 07
            rts                    ; 60

;--------------------------------------------

L1E8C:      lda #$10               ; A9 10
            sta L001B              ; 85 1B
            lda #$0B               ; A9 0B
            sta L001D              ; 85 1D
L1E94:      ldx L001B              ; A6 1B
            ldy L001D              ; A4 1D
            jsr L1EBA              ; 20 BA 1E
            pha                    ; 48
            lda L001B              ; A5 1B
            asl A                  ; 0A
            tax                    ; AA
            lda L001D              ; A5 1D
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            adc L0027              ; 65 27
            tay                    ; A8
            pla                    ; 68
            jsr L2491              ; 20 91 24
            dec L001B              ; C6 1B
            bpl L1E94              ; 10 E3
            lda #$10               ; A9 10
            sta L001B              ; 85 1B
            dec L001D              ; C6 1D
            bpl L1E94              ; 10 DB
            rts                    ; 60

;--------------------------------------------

L1EBA:      tya                    ; 98
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            stx L0022              ; 86 22
            clc                    ; 18
            adc L0022              ; 65 22
            tay                    ; A8
            lda L5700,Y            ; B9 00 57
            rts                    ; 60

;--------------------------------------------

L1EC9:      pha                    ; 48
            tya                    ; 98
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            stx L0022              ; 86 22
            clc                    ; 18
            adc L0022              ; 65 22
            tay                    ; A8
            pla                    ; 68
            sta L5700,Y            ; 99 00 57
            rts                    ; 60

;--------------------------------------------

L1EDA:      .byte $00,$01,$02,$03,$04,$05,$06,$07
            .byte $08,$09,$0A,$0B,$0C,$0D,$0E,$0F
            .byte $10,$11,$12,$13,$14,$15,$16,$17
            .byte $18,$19,$1A,$1B,$1C,$1D,$1E,$1F
            .byte $20,$21,$22,$23,$24,$25,$26,$27
            .byte $28,$29,$2A,$2B,$2C,$2D,$2E,$2F
            .byte $30,$31,$32,$33,$34,$35,$36,$37
            .byte $38,$39,$3A,$3B,$3C,$3D,$3E,$3F
            .byte $40,$41,$EA,$F1,$44,$45,$46,$47
            .byte $48,$49,$4A,$4B,$4C,$4D,$4E,$4F
            .byte $50,$51,$52,$53,$54,$55,$56,$57
            .byte $58,$59,$5A,$5B,$5C,$5D,$5E,$5F
            .byte $60,$61,$62,$63,$64,$65,$66,$67
            .byte $68,$69,$6A,$6B,$6C,$6D,$6E,$6F
            .byte $70,$71,$72,$73,$74,$76,$77,$78
            .byte $75,$79,$7A,$7B,$7C,$7D,$7E,$7F
            .byte $80,$81,$82,$83,$84,$85,$86,$87
            .byte $88,$89,$8A,$8B,$8C,$8D,$8E,$8F
            .byte $90,$91,$92,$93,$94,$95,$96,$97
            .byte $98,$99,$9A,$9B,$9C,$9D,$9E,$9F
            .byte $A0,$A1,$A2,$A3,$A4,$A5,$A6,$A7
            .byte $A8,$A9,$AA,$AB,$AC,$AD,$AE,$BC
            .byte $B1,$B2,$B3,$B4,$B5,$B6,$B7,$B8
            .byte $B9,$BA,$BB,$BB,$BD,$BE,$BF,$AF
            .byte $C0,$C2,$C3,$C0,$C4,$C6,$C7,$C4
            .byte $C8,$CA,$CB,$C8,$CD,$CE,$CF,$D0
            .byte $D1,$D2,$D3,$CC,$D4,$D5,$D6,$D7
            .byte $D8,$D9,$DA,$DB,$DD,$DE,$DF,$E0
            .byte $E1,$E2,$E3,$E3,$E4,$E5,$E7,$E8
            .byte $E9,$E6,$EB,$EC,$ED,$EE,$EF,$F0
            .byte $42,$F2,$F3,$F4,$F5,$F6,$F7,$43
            .byte $F8,$F9,$FA,$FB,$FC,$FD,$FE,$FF

;--------------------------------------------

L1FDA:      ldx #$00               ; A2 00
            lda L002B              ; A5 2B
            and #$0F               ; 29 0F
            beq L1FE4              ; F0 02
            ldx #$FF               ; A2 FF

L1FE4:      stx L002C              ; 86 2C
            ldx L0004              ; X-pos plane
            ldy L0005              ; Y-pos plane
            jsr L196E              ; 20 6E 19

            txa                    ; 8A
            cmp L002D              ; C5 2D
            bcc L2032              ; 90 40
            beq L2014              ; F0 20
            bcs L1FF6              ; B0 00
L1FF6:      tya                    ; 98
            cmp L002E              ; C5 2E
            bcc L1FFF              ; 90 04
            beq L2006              ; F0 09
            bcs L200D              ; B0 0E
L1FFF:      pla                    ; 68
            lda #$D5               ; A9 D5
            pha                    ; 48
            jmp L2121              ; 4C 21 21

L2006:      pla                    ; 68
            lda #$D6               ; A9 D6
            pha                    ; 48
            jmp L20B7              ; 4C B7 20

L200D:      pla                    ; 68
            lda #$D7               ; A9 D7
            pha                    ; 48
            jmp L2147              ; 4C 47 21

L2014:      tya                    ; 98
            cmp L002E              ; C5 2E
            bcc L201D              ; 90 04
            beq L2024              ; F0 09
            bcs L202B              ; B0 0E
L201D:      pla                    ; 68
            lda #$D4               ; A9 D4
            pha                    ; 48
            jmp L20FB              ; 4C FB 20

L2024:      pla                    ; 68
            lda #$BB               ; A9 BB
            pha                    ; 48
            jmp L208F              ; 4C 8F 20

L202B:      pla                    ; 68
            lda #$D8               ; A9 D8
            pha                    ; 48
            jmp L20A4              ; 4C A4 20

L2032:      tya                    ; 98
            cmp L002E              ; C5 2E
            bcc L203B              ; 90 04
            beq L2042              ; F0 09
            bcs L2049              ; B0 0E
L203B:      pla                    ; 68
            lda #$DB               ; A9 DB
            pha                    ; 48
            jmp L210E              ; 4C 0E 21

L2042:      pla                    ; 68
            lda #$DA               ; A9 DA
            pha                    ; 48
            jmp L20CA              ; 4C CA 20

L2049:      pla                    ; 68
            lda #$D9               ; A9 D9
            pha                    ; 48
            jmp L2134              ; 4C 34 21

;--------------------------------------------

L2050:      lda L0027              ; A5 27
            ror A                  ; 6A
            bcs L20A3              ; B0 4E
            ldx #$0F               ; A2 0F
            ldy #$0B               ; A0 0B
            stx L002D              ; 86 2D
            sty L002E              ; 84 2E
L205D:      ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L1EBA              ; 20 BA 1E
            tay                    ; A8
            lda #$00               ; A9 00
            sta L002C              ; 85 2C
            lda L1EDA,Y            ; B9 DA 1E
            pha                    ; 48

            cmp #$BB               ; C9 BB
            bne L2074              ; D0 03
            jmp L1FDA              ; 4C DA 1F

L2074:      cmp #$BF               ; C9 BF
            beq L20DD              ; F0 65

            cmp #$C2               ; C9 C2
            beq L20A4              ; F0 28

            cmp #$C6               ; C9 C6
            beq L20B7              ; F0 37

            cmp #$CA               ; C9 CA
            beq L20CA              ; F0 46

            cmp #$D4               ; C9 D4
            bcc L208F              ; 90 07

            cmp #$DC               ; C9 DC
            bcs L208F              ; B0 03

            jmp L1FDA              ; 4C DA 1F

L208F:      pla                    ; 68
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L1EC9              ; 20 C9 1E
            dec L002D              ; C6 2D
            bpl L205D              ; 10 C2
            lda #$0F               ; A9 0F
            sta L002D              ; 85 2D
            dec L002E              ; C6 2E
            bpl L205D              ; 10 BA
L20A3:      rts                    ; 60

L20A4:      lda L002C              ; A5 2C
            bne L208F              ; D0 E7
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L2162              ; 20 62 21
            lda #$01               ; A9 01
            jsr L217D              ; 20 7D 21
            jmp L208F              ; 4C 8F 20

L20B7:      lda L002C              ; A5 2C
            bne L208F              ; D0 D4
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L2162              ; 20 62 21
            lda #$02               ; A9 02
            jsr L217D              ; 20 7D 21
            jmp L208F              ; 4C 8F 20

L20CA:      lda L002C              ; A5 2C
            bne L208F              ; D0 C1
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L2162              ; 20 62 21
            lda #$04               ; A9 04
            jsr L217D              ; 20 7D 21
            jmp L208F              ; 4C 8F 20

L20DD:      lda L002C              ; A5 2C
            bne L208F              ; D0 AE
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L2162              ; 20 62 21
            stx L0022              ; 86 22
            jsr L22E7              ; 20 E7 22
            and #$07               ; 29 07
            tax                    ; AA
            lda L215A,X            ; BD 5A 21
            ldx L0022              ; A6 22
            jsr L217D              ; 20 7D 21
            jmp L208F              ; 4C 8F 20

L20FB:      lda L002C              ; A5 2C
            bne L208F              ; D0 90
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L2162              ; 20 62 21
            lda #$08               ; A9 08
            jsr L217D              ; 20 7D 21
            jmp L208F              ; 4C 8F 20

L210E:      lda L002C              ; A5 2C
            bne L211E              ; D0 0C
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L2162              ; 20 62 21
            lda #$0C               ; A9 0C
            jsr L217D              ; 20 7D 21
L211E:      jmp L208F              ; 4C 8F 20

L2121:      lda L002C              ; A5 2C
            bne L211E              ; D0 F9
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L2162              ; 20 62 21
            lda #$0A               ; A9 0A
            jsr L217D              ; 20 7D 21
            jmp L208F              ; 4C 8F 20

L2134:      lda L002C              ; A5 2C
            bne L211E              ; D0 E6
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L2162              ; 20 62 21
            lda #$05               ; A9 05
            jsr L217D              ; 20 7D 21
            jmp L208F              ; 4C 8F 20

L2147:      lda L002C              ; A5 2C
            bne L211E              ; D0 D3
            ldx L002D              ; A6 2D
            ldy L002E              ; A4 2E
            jsr L2162              ; 20 62 21
            lda #$03               ; A9 03
            jsr L217D              ; 20 7D 21
            jmp L208F              ; 4C 8F 20

L215A:      .byte $01,$02,$04,$08,$03,$05,$0A,$0C

L2162:      txa                    ; 8A
            asl A                  ; 0A
            clc                    ; 18
            adc #$30               ; 69 30
            clc                    ; 18
            adc $00                ; 65 00
            tax                    ; AA
            clc                    ; 18
            iny                    ; C8
            iny                    ; C8
            tya                    ; 98
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            eor #$FF               ; 49 FF
            sec                    ; 38
            sbc L0027              ; E5 27
            clc                    ; 18
            adc #$06               ; 69 06
            tay                    ; A8
            rts                    ; 60
L217D:      pha                    ; 48
            tya                    ; 98
            pha                    ; 48
            txa                    ; 8A
            pha                    ; 48
            lda L0025              ; A5 25
            tay                    ; A8
            pla                    ; 68
            sta L5628,Y            ; 99 28 56
            pla                    ; 68
            sta L5658,Y            ; 99 58 56
            pla                    ; 68
            sta L5688,Y            ; 99 88 56
            inc L0025              ; E6 25
            rts                    ; 60

            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00

L21A5:      lda L002F              ; A5 2F
            bne L21B0              ; D0 07
            lda #$FF               ; A9 FF
            sta L002F              ; 85 2F
            jmp L223F              ; 4C 3F 22

;--------------------------------------------
; Scroll screen
;
;--------------------------------------------

L21B0:      ldx #$9F               ; A0 9F
L21B2:
	ldy buftab,x
            lda $1260,X            ; B9 60 12
            sta L78B8,Y            ; 99 B8 78
            lda $11C0,X            ; B9 C0 11
            sta L7778,Y            ; 99 78 77
            lda $1120,X            ; B9 20 11
            sta L7638,Y            ; 99 38 76
            lda $1080,X            ; B9 80 10
            sta L74F8,Y            ; 99 F8 74
            lda $0FE0,X            ; B9 E0 0F
            sta L73B8,Y            ; 99 B8 73
            lda $0F40,X            ; B9 40 0F
            sta L7278,Y            ; 99 78 72
            lda $0EA0,X            ; B9 A0 0E
            sta L7138,Y            ; 99 38 71
            lda $0E00,X            ; B9 00 0E
            sta L6FF8,Y            ; 99 F8 6F
            lda $0D60,X            ; B9 60 0D
            sta L6EB8,Y            ; 99 B8 6E
            lda $0CC0,X            ; B9 C0 0C
            sta L6D78,Y            ; 99 78 6D
            lda $0C20,X            ; B9 20 0C
            sta L6C38,Y            ; 99 38 6C
            lda $0B80,X            ; B9 80 0B
            sta L6AF8,Y            ; 99 F8 6A
            lda $0AE0,X            ; B9 E0 0A
            sta L69B8,Y            ; 99 B8 69
            lda $0A40,X            ; B9 40 0A
            sta L6878,Y            ; 99 78 68
            lda $09A0,X            ; B9 A0 09
            sta L6738,Y            ; 99 38 67
            lda $0900,X            ; B9 00 09
            sta L65F8,Y            ; 99 F8 65
            lda $0860,X            ; B9 60 08
            sta L64B8,Y            ; 99 B8 64
            lda $07C0,X            ; B9 C0 07
            sta L6378,Y            ; 99 78 63
            lda $0720,X            ; B9 20 07
            sta L6238,Y            ; 99 38 62
            lda $0680,X            ; B9 80 06
            sta L60F8,Y            ; 99 F8 60
            lda $05E0,X            ; B9 E0 05
            sta L5FB8,Y            ; 99 B8 5F
            lda $0540,X            ; B9 40 05
            sta L5E78,Y            ; 99 78 5E
            dex                    ; 88
            cpx #$FF               ; C0 FF
            beq L223E              ; F0 03
            jmp L21B2              ; 4C B2 21
L223E:      rts                    ; 60

;--------------------------------------------
; Copy screenbuffer to screen
;--------------------------------------------

L223F:      ldx #$9F               ; A0 9F
L2241:
	ldy buftab,x
            lda $1260,X            ; B9 60 12
            sta L78B8,Y            ; 99 B8 78
            lda $11C0,X            ; B9 C0 11
            sta L7778,Y            ; 99 78 77
            lda $1120,X            ; B9 20 11
            sta L7638,Y            ; 99 38 76
            lda $1080,X            ; B9 80 10
            sta L74F8,Y            ; 99 F8 74
            lda $0FE0,X            ; B9 E0 0F
            sta L73B8,Y            ; 99 B8 73
            lda $0F40,X            ; B9 40 0F
            sta L7278,Y            ; 99 78 72
            lda $0EA0,X            ; B9 A0 0E
            sta L7138,Y            ; 99 38 71
            lda $0E00,X            ; B9 00 0E
            sta L6FF8,Y            ; 99 F8 6F
            lda $0D60,X            ; B9 60 0D
            sta L6EB8,Y            ; 99 B8 6E
            lda $0CC0,X            ; B9 C0 0C
            sta L6D78,Y            ; 99 78 6D
            lda $0C20,X            ; B9 20 0C
            sta L6C38,Y            ; 99 38 6C
            lda $0B80,X            ; B9 80 0B
            sta L6AF8,Y            ; 99 F8 6A
            lda $0AE0,X            ; B9 E0 0A
            sta L69B8,Y            ; 99 B8 69
            lda $0A40,X            ; B9 40 0A
            sta L6878,Y            ; 99 78 68
            lda $09A0,X            ; B9 A0 09
            sta L6738,Y            ; 99 38 67
            lda $0900,X            ; B9 00 09
            sta L65F8,Y            ; 99 F8 65
            lda $0860,X            ; B9 60 08
            sta L64B8,Y            ; 99 B8 64
            lda $07C0,X            ; B9 C0 07
            sta L6378,Y            ; 99 78 63
            lda $0720,X            ; B9 20 07
            sta L6238,Y            ; 99 38 62
            lda $0680,X            ; B9 80 06
            sta L60F8,Y            ; 99 F8 60
            lda $05E0,X            ; B9 E0 05
            sta L5FB8,Y            ; 99 B8 5F
            lda $0540,X            ; B9 40 05
            sta L5E78,Y            ; 99 78 5E

; Wait loop
            ldy #$FF               ; A2 FF
L22C7:      nop                    ; EA
;            nop                    ; EA
            dey                    ; CA
            bne L22C7              ; D0 FB

            dex                    ; 88
            dex                    ; 88
            dex                    ; 88
            cpx #$FD               ; C0 FD
            beq L22DE              ; F0 0B

            cpx #$FF               ; C0 FF
            beq L22DE              ; F0 07

            cpx #$FE               ; C0 FE
            beq L22E6              ; F0 0B

            jmp L2241              ; 4C 41 22

L22DE:      clc                    ; 18
            txa                    ; 98
            adc #$9E               ; 69 9E
            tax                    ; A8
            jmp L2241              ; 4C 41 22

L22E6:      rts                    ; 60

;--------------------------------------------

L22E7:      txa                    ; 8A
            pha                    ; 48
            tya                    ; 98
            pha                    ; 48
            txa                    ; 8A
            eor L0033              ; 45 33
            sta L0033              ; 85 33
            tsx                    ; BA
            txa                    ; 8A
            eor L0033              ; 45 33
            asl A                  ; 0A
            rol L0033              ; 26 33
            tya                    ; 98
            eor L0033              ; 45 33
            sta L0033              ; 85 33
            pla                    ; 68
            tay                    ; A8
            pla                    ; 68
            tax                    ; AA
            lda L0033              ; A5 33
            rts                    ; 60

L2303:      .byte $00
L2304:      .byte $00,$00,$00,$00,$00,$00,$00

L230B:      .byte $00
L230C:      .byte $00
            .byte $A0,$00,$40,$01,$E0,$01,$80,$02
            .byte $20,$03,$C0,$03,$60,$04,$00,$05
            .byte $A0,$05,$40,$06,$E0,$06,$80,$07
            .byte $20,$08,$C0,$08,$60,$09,$00,$0A
            .byte $A0,$0A,$40,$0B,$E0,$0B,$80,$0C
            .byte $20,$0D,$C0,$0D,$60,$0E,$00,$0F
            .byte $A0,$0F

L233F:      pha                    ; 48
            tya                    ; 98
            clc                    ; 18
            adc L0010              ; 65 10
            clc                    ; 18
            cmp L0012              ; C5 12
            bcc L2361              ; 90 18
            beq L2361              ; F0 16
            cpy L0014              ; C4 14
            bcs L2361              ; B0 12
            cpx L0013              ; E4 13
            bcs L2361              ; B0 0E
            txa                    ; 8A
            clc                    ; 18
            adc L000F              ; 65 0F
            clc                    ; 18
            cmp L0011              ; C5 11
            bcc L2361              ; 90 05
            beq L2361              ; F0 03
            jmp L2363              ; 4C 63 23

L2361:      pla                    ; 68
            rts                    ; 60

L2363:      lda #$00               ; A9 00
            sta L0015              ; 85 15
            lda L000F              ; A5 0F
            sta L0017              ; 85 17
            txa                    ; 8A
            sec                    ; 38
            sbc L0011              ; E5 11
            sta L0019              ; 85 19
            cpx L0011              ; E4 11
            bcs L2389              ; B0 14
            txa                    ; 8A
            clc                    ; 18
            adc L000F              ; 65 0F
            sec                    ; 38
            sbc L0011              ; E5 11
            sta L0015              ; 85 15
            lda L000F              ; A5 0F
            sec                    ; 38
            sbc L0015              ; E5 15
            sta L0015              ; 85 15
            lda #$00               ; A9 00
            sta L0019              ; 85 19
L2389:      txa                    ; 8A
            clc                    ; 18
            adc L000F              ; 65 0F
            clc                    ; 18
            cmp L0013              ; C5 13
            beq L2394              ; F0 02
            bcc L239D              ; 90 09
L2394:      stx L0017              ; 86 17
            lda L0013              ; A5 13
            sec                    ; 38
            sbc L0017              ; E5 17
            sta L0017              ; 85 17
L239D:      lda #$00               ; A9 00
            sta L0016              ; 85 16
            lda L0010              ; A5 10
            sta L0018              ; 85 18
            sty L001A              ; 84 1A
            cpy L0012              ; C4 12
            bcs L23BF              ; B0 14

            tya                    ; 98
            clc                    ; 18
            adc L0010              ; 65 10
            sec                    ; 38
            sbc L0012              ; E5 12
            sta L0016              ; 85 16
            lda L0010              ; A5 10
            sec                    ; 38
            sbc L0016              ; E5 16
            sta L0016              ; 85 16
            lda L0012              ; A5 12
            sta L001A              ; 85 1A

L23BF:      tya                    ; 98
            adc L0010              ; 65 10
            clc                    ; 18
            cmp L0014              ; C5 14
            beq L23C9              ; F0 02
            bcc L23D2              ; 90 09
L23C9:      sty L0018              ; 84 18
            lda L0014              ; A5 14
            sec                    ; 38
            sbc L0018              ; E5 18
            sta L0018              ; 85 18

L23D2:      pla                    ; 68
            and #$0F               ; 29 0F
            asl A                  ; 0A
            tay                    ; A8
            lda L284C,Y            ; B9 4C 28
            sta L0007              ; 85 07
            lda L285E,Y            ; B9 5E 28
            sta L0009              ; 85 09
            lda L284D,Y            ; B9 4D 28
            sta L0008              ; 85 08
            lda L285F,Y            ; B9 5F 28
            sta L000A              ; 85 0A
            ldx L0019              ; A6 19
            ldy L001A              ; A4 1A
            tya                    ; 98
            eor #$FF               ; 49 FF
            pha                    ; 48
            lsr A                  ; 4A
            lsr A                  ; 4A
            lsr A                  ; 4A
            asl A                  ; 0A
            tay                    ; A8

            lda #<L0540               ; A9 40
            sta L000B              ; 85 0B
            lda #>L0540               ; A9 05
            sta L000C              ; 85 0C

            lda L2303,Y            ; B9 03 23
            clc                    ; 18
            adc L000B              ; 65 0B
            sta L000B              ; 85 0B
            lda L2304,Y            ; B9 04 23
            adc L000C              ; 65 0C
            sta L000C              ; 85 0C
            pla                    ; 68
            and #$07               ; 29 07
            adc L000B              ; 65 0B
            sta L000B              ; 85 0B
            txa                    ; 8A
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            adc L000B              ; 65 0B
            sta L000B              ; 85 0B
            bcc L2422              ; 90 02
            inc L000C              ; E6 0C
L2422:      lda L0015              ; A5 15
            sta L000D              ; 85 0D
            lda #$00               ; A9 00
            sta L0021              ; 85 21
L242A:      lda L0016              ; A5 16
            sta L000E              ; 85 0E
            ldx L000D              ; A6 0D
            ldy L0010              ; A4 10
            stx L0022              ; 86 22
            sty L0023              ; 84 23
            ldx #$08               ; A2 08
            lda #$00               ; A9 00
L243A:      asl A                  ; 0A
            asl L0022              ; 06 22
            bcc L2442              ; 90 03
            clc                    ; 18
            adc L0023              ; 65 23
L2442:      dex                    ; CA
            bne L243A              ; D0 F5
            clc                    ; 18
            adc L0016              ; 65 16
            tay                    ; A8
            lda L0021              ; A5 21
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            adc L000B              ; 65 0B
            sta L001B              ; 85 1B
            lda L000C              ; A5 0C
            adc #$00               ; 69 00
            sta L001C              ; 85 1C

; Plot plane
	lda L0008
	cmp #>L2B60
	bne no_explosion
	lda L0007
	cmp #<L2B60
	bne no_explosion

	lda #<K1D61		; Explosion sound
	sty save_y
	ldy #>K1D61
	jsr K1006
	ldy save_y
	
no_explosion:
            ldx #$00               ; A2 00
L245A:      lda (L001B,X)          ; A1 1B
            and (L0009),Y          ; 31 09
            adc (L0007),Y          ; 71 07
            sta (L001B,X)          ; 81 1B
            iny                    ; C8
            inc L000E              ; E6 0E
            lda L001B              ; A5 1B
            bne L246B              ; D0 02
            dec L001C              ; C6 1C
L246B:      dec L001B              ; C6 1B
            lda L001B              ; A5 1B
            and #$07               ; 29 07
            cmp #$07               ; C9 07
            bne L2480              ; D0 0B
            lda L001B              ; A5 1B
            sec                    ; 38
            sbc #$98               ; E9 98
            sta L001B              ; 85 1B
            bcs L2480              ; B0 02
            dec L001C              ; C6 1C
L2480:      lda L000E              ; A5 0E
            cmp L0018              ; C5 18
            bne L245A              ; D0 D4

            inc L000D              ; E6 0D
            inc L0021              ; E6 21
            lda L000D              ; A5 0D
            cmp L0017              ; C5 17
            bne L242A              ; D0 9A

            rts                    ; 60

save_y:	.byte 0
L2491:      stx L002D              ; 86 2D
            sty L002E              ; 84 2E
            jsr L2588              ; 20 88 25
            sec                    ; 38
            lda #$10               ; A9 10
            sbc $00                ; E5 00
            sta L002D              ; 85 2D
            sec                    ; 38
            txa                    ; 8A
            sbc L002D              ; E5 2D
            cmp #$FF               ; C9 FF
            beq L24CC              ; F0 25
            bmi L24BF              ; 30 16
            cmp #$13               ; C9 13
            beq L24C0              ; F0 13
            bcs L24BF              ; B0 10
            tax                    ; AA
            jsr L25B1              ; 20 B1 25
            jsr L24E5              ; 20 E5 24
            jsr L24FC              ; 20 FC 24
            jsr L251C              ; 20 1C 25
            jsr L253C              ; 20 3C 25
L24BF:      rts                    ; 60

;--------------------------------------------
; Scroll data in buffer
;--------------------------------------------

L24C0:      ldx #$13               ; A2 13
            jsr L25B1              ; 20 B1 25
            jsr L24E5              ; 20 E5 24
            jsr L251C              ; 20 1C 25
            rts                    ; 60

L24CC:      ldx #$00               ; A2 00
            jsr L25B1              ; 20 B1 25
            sec                    ; 38
            lda L000B              ; A5 0B
            sbc #$08               ; E9 08
            sta L000B              ; 85 0B
            lda L000C              ; A5 0C
            sbc #$00               ; E9 00
            sta L000C              ; 85 0C
            jsr L24FC              ; 20 FC 24
            jsr L253C              ; 20 3C 25
            rts                    ; 60

L24E5:      lda L000B              ; A5 0B
            sta L2563+1              ; 8D 64 25
            lda L000C              ; A5 0C
            sta L2563+2              ; 8D 65 25
            lda L0007              ; A5 07
            sta L2560+1              ; 8D 61 25
            lda L0008              ; A5 08
            sta L2560+2              ; 8D 62 25
            jmp L255C              ; 4C 5C 25

L24FC:      clc                    ; 18
            lda L000B              ; A5 0B
            adc #$08               ; 69 08
            sta L2563+1              ; 8D 64 25
            lda L000C              ; A5 0C
            adc #$00               ; 69 00
            sta L2563+2              ; 8D 65 25
            lda L0007              ; A5 07
            adc #$08               ; 69 08
            sta L2560+1              ; 8D 61 25
            lda L0008              ; A5 08
            adc #$00               ; 69 00
            sta L2560+2              ; 8D 62 25
            jmp L255C              ; 4C 5C 25

L251C:      clc                    ; 18
            lda L000B              ; A5 0B
            adc #$A0               ; 69 A0
            sta L2563+1              ; 8D 64 25
            lda L000C              ; A5 0C
            adc #$00               ; 69 00
            sta L2563+2              ; 8D 65 25
            lda L0007              ; A5 07
            adc #$10               ; 69 10
            sta L2560+1              ; 8D 61 25
            lda L0008              ; A5 08
            adc #$00               ; 69 00
            sta L2560+2              ; 8D 62 25
            jmp L255C              ; 4C 5C 25

L253C:      clc                    ; 18
            lda L000B              ; A5 0B
            adc #$A8               ; 69 A8
            sta L2563+1              ; 8D 64 25
            lda L000C              ; A5 0C
            adc #$00               ; 69 00
            sta L2563+2              ; 8D 65 25
            lda L0007              ; A5 07
            adc #$18               ; 69 18
            sta L2560+1              ; 8D 61 25
            lda L0008              ; A5 08
            adc #$00               ; 69 00
            sta L2560+2              ; 8D 62 25
            jmp L255C              ; 4C 5C 25
L255C:
            ldx #$08               ; A2 08
            ldy #$00               ; A0 00
L2560:
            lda LFFFF,Y            ; B9 FF FF

L2563:
            sta LFFFF              ; 8D FF FF
            iny                    ; C8
            inc L2563+1              ; EE 64 25
            lda L2563+1              ; AD 64 25
            bne L2572              ; D0 03
            inc L2563+2              ; EE 65 25
L2572:
            and #$07               ; 29 07
            bne L2584              ; D0 0E
            lda L2563+1              ; AD 64 25
            clc                    ; 18
            adc #$98               ; 69 98
            sta L2563+1              ; 8D 64 25
            bcc L2584              ; 90 03
            inc L2563+2              ; EE 65 25
L2584:
            dex                    ; CA
            bne L2560              ; D0 D9
            rts                    ; 60

L2588:
            pha                    ; 48

            lda #<L2DC0               ; A9 C0
            sta L0007              ; 85 07

            lda #$00               ; A9 00
            sta L0022              ; 85 22

            lda #>L2DC0               ; A9 2D
            sta L0008              ; 85 08

            pla                    ; 68
            asl A                  ; 0A
            rol L0022              ; 26 22
            asl A                  ; 0A
            rol L0022              ; 26 22
            asl A                  ; 0A
            rol L0022              ; 26 22
            asl A                  ; 0A
            rol L0022              ; 26 22
            asl A                  ; 0A
            rol L0022              ; 26 22
            clc                    ; 18
            adc L0007              ; 65 07
            sta L0007              ; 85 07
            lda L0022              ; A5 22
            adc L0008              ; 65 08
            sta L0008              ; 85 08
            rts                    ; 60

L25B1:      txa                    ; 8A
            asl A                  ; 0A
            asl A                  ; 0A
            asl A                  ; 0A
            tax                    ; AA
            sty L0022              ; 84 22
            clc                    ; 18
            tya                    ; 98
            and #$F8               ; 29 F8
            ror A                  ; 6A
            ror A                  ; 6A
            tay                    ; A8

            lda #<L0400               ; A9 00
            sta L000B              ; 85 0B
            lda #>L0400               ; A9 04
            sta L000C              ; 85 0C

            lda L230B,Y            ; B9 0B 23
            clc                    ; 18
            adc L000B              ; 65 0B
            sta L000B              ; 85 0B
	sta $9b00
            lda L230C,Y            ; B9 0C 23
            adc L000C              ; 65 0C
            sta L000C              ; 85 0C
	sta $9b01
            txa                    ; 8A
            clc                    ; 18
            adc L000B              ; 65 0B
            sta L000B              ; 85 0B
            lda L000C              ; A5 0C
            adc #$00               ; 69 00
            sta L000C              ; 85 0C
            lda L0022              ; A5 22
            and #$07               ; 29 07
            adc L000B              ; 65 0B
            sta L000B              ; 85 0B
            rts                    ; 60

            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00

L27F0:      .byte $00,$3C,$72,$72,$72,$72,$3C,$00
            .byte $00,$1C,$3C,$1C,$1C,$1C,$3E,$00
            .byte $00,$3C,$4E,$0E,$3C,$70,$7E,$00
            .byte $00,$3C,$4E,$1C,$0E,$4E,$3C,$00
            .byte $00,$0C,$1C,$3C,$5C,$7E,$1C,$00
            .byte $00,$7E,$70,$7C,$0E,$4E,$3C,$00
            .byte $00,$3C,$70,$7C,$72,$72,$3C,$00
            .byte $00,$7E,$0E,$1C,$38,$30,$30,$00
            .byte $00,$3C,$72,$3C,$72,$72,$3C,$00
            .byte $00,$3C,$4E,$4E,$3E,$0E,$3C,$00
L2840:
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$00,$00,$00

;------------------------------------------------
; Address tables sprites/mask
;------------------------------------------------

L284C:      .byte <L2870	; Plane
L284D:      .byte >L2870
            .byte <L29B0,>L29B0	; Enemy1
            .byte <L2A40,>L2A40	; Enemy2
            .byte <L2AD0,>L2AD0	; Enemy3
            .byte <L2B60,>L2B60	; Explosion1
            .byte <L2BF0,>L2BF0	; Explosion2
            .byte <L2C80,>L2C80	; Explosion3
            .byte <L2D10,>L2D10	; Explosion4
            .byte <L2DA0,>L2DA0	; Bullet

L285E:      .byte <L2910	; Plane mask
L285F:      .byte >L2910
            .byte <L29F8,>L29F8	; Enemy1 mask
            .byte <L2A88,>L2A88	; Enemy2 mask
            .byte <L2B18,>L2B18	; Enemy3 mask
            .byte <L2BA8,>L2BA8	; Explosion1 mask
            .byte <L2C38,>L2C38	; Explosion2 mask
            .byte <L2CC8,>L2CC8	; Explosion3 mask
            .byte <L2D58,>L2D58	; Explosion4 mask
            .byte <L2DB0,>L2DB0	; Bullet mask

;------------------------------------------------
; Sprites, vertical build upside down (X x Y bytes)
;
; xx
; ..
; 03 
; 02  ..
; 01 xx+2
; 00 xx+1 ..

L2870:
            .byte $00,$00,$00,$00,$00,$07,$07		; Plane (4x40 bytes)
            .byte $03,$01,$00,$00,$00,$70,$0F,$70
            .byte $33,$12,$0E,$06,$02,$01,$01,$01
            .byte $01,$01,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$80,$40
            .byte $6D,$60,$6D,$29,$2D,$29,$D0,$35
            .byte $75,$ED,$EA,$EA,$0A,$57,$FA,$FA
            .byte $70,$17,$75,$68,$68,$68,$0F,$08
            .byte $08,$08,$08,$05,$07,$07,$02,$02
            .byte $00,$00,$00,$00,$00,$00,$0F,$17
            .byte $B6,$34,$B0,$20,$A0,$20,$5F,$20
            .byte $56,$BA,$B3,$BB,$82,$54,$F8,$FC
            .byte $74,$44,$70,$B0,$A0,$B0,$80,$80
            .byte $80,$80,$80,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$70,$80,$70
            .byte $60,$40,$80,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00
L2910:
            .byte $FF,$FE,$FE,$FF,$F8,$F0,$F0		; Plane mask (4x40 bytes)
            .byte $F0,$E0,$E0,$E0,$80,$00,$00,$00
            .byte $00,$80,$C0,$E0,$F0,$F8,$FC,$FC
            .byte $FC,$FC,$FE,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$0F,$07,$02,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$80,$E0
            .byte $E0,$E0,$E0,$F0,$F0,$F0,$F0,$F8
            .byte $F8,$FE,$FC,$48,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$01,$07,$07,$07,$07,$0F
            .byte $0F,$0F,$1F,$1F,$3F,$7F,$7F,$FF
            .byte $FF,$1F,$0F,$0F,$1F,$3F,$7F,$7F
            .byte $F1,$00,$00,$00,$00,$01,$03,$07
            .byte $07,$0F,$1F,$3F,$3F,$3F,$7F,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF
L29B0:
            .byte $00,$00,$00,$00,$00,$10,$11		; Enemy1 (3x24 bytes)
            .byte $10,$37,$7B,$02,$76,$00,$7F,$7F
            .byte $00,$19,$02,$02,$02,$19,$65,$79
            .byte $00,$00,$00,$00,$00,$00,$60,$68
            .byte $60,$6E,$6D,$64,$66,$00,$6F,$6F
            .byte $00,$69,$64,$64,$64,$69,$6A,$69
            .byte $00,$00,$00,$00,$00,$00,$80,$80
            .byte $80,$C0,$E0,$00,$E0,$00,$E0,$E0
            .byte $00,$80,$00,$00,$00,$80,$60,$E0
            .byte $00
L29F8:
            .byte $FE,$FC,$FC,$FC,$E8,$C0,$C0		; Enemy1 mask (3x24 bytes)
            .byte $C0,$80,$00,$80,$00,$00,$00,$00
            .byte $80,$80,$C0,$E0,$E0,$00,$00,$00
            .byte $86,$F0,$60,$40,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $06,$F7,$63,$23,$03,$01,$00,$01
            .byte $00,$00,$00,$01,$01,$01,$03,$07
            .byte $07,$00,$00,$00,$61,$0F,$0F,$0F
            .byte $1F
L2A40:
            .byte $00,$00,$00,$00,$00,$10,$15		; Enemy2 (3x24 bytes)
            .byte $0C,$36,$7B,$03,$75,$69,$76,$06
            .byte $31,$17,$0E,$38,$12,$09,$65,$79
            .byte $00,$00,$00,$00,$00,$00,$F0,$08
            .byte $F0,$9E,$09,$67,$9E,$99,$66,$66
            .byte $98,$99,$6C,$8C,$96,$6B,$6A,$F9
            .byte $00,$00,$00,$00,$00,$00,$80,$80
            .byte $00,$80,$C0,$00,$E0,$60,$E0,$00
            .byte $C0,$80,$00,$00,$00,$00,$60,$E0
            .byte $00
L2A88:
            .byte $FE,$FC,$FC,$FC,$E8,$C0,$C0		; Enemy2 mask (3x24 bytes)
            .byte $C0,$80,$00,$80,$00,$00,$00,$80
            .byte $80,$80,$C0,$80,$C0,$00,$00,$00
            .byte $86,$F0,$60,$40,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $06,$F7,$63,$23,$03,$01,$00,$01
            .byte $00,$00,$00,$01,$01,$01,$03,$07
            .byte $07,$00,$00,$00,$61,$0F,$0F,$0F
            .byte $1F
L2AD0:
            .byte $00,$00,$00,$00,$00,$10,$11		; Enemy3 (3x24 bytes)
            .byte $00,$17,$39,$0E,$77,$69,$76,$06
            .byte $31,$19,$03,$03,$06,$0D,$65,$79
            .byte $00,$00,$00,$00,$00,$00,$F0,$0A
            .byte $F3,$96,$0D,$6C,$9A,$99,$66,$66
            .byte $98,$9E,$67,$11,$94,$69,$6A,$F9
            .byte $00,$00,$00,$00,$00,$00,$80,$80
            .byte $00,$C0,$E0,$00,$E0,$60,$E0,$00
            .byte $C0,$80,$00,$C0,$80,$00,$60,$E0
            .byte $00
L2B18:
            .byte $FE,$FC,$FC,$FC,$E8,$C0,$C0		; Enemy3 mask (3x24 bytes)
            .byte $C0,$80,$00,$80,$00,$00,$00,$80
            .byte $80,$80,$C0,$E0,$E0,$00,$00,$00
            .byte $86,$F0,$60,$40,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $06,$F7,$63,$23,$03,$01,$00,$01
            .byte $00,$00,$00,$01,$01,$01,$03,$07
            .byte $07,$00,$00,$00,$21,$0F,$0F,$0F
            .byte $1F
L2B60:
            .byte $00,$00,$00,$05,$09,$18,$34		; Explosion1 (3x24 bytes)
            .byte $35,$35,$09,$14,$1D,$1D,$04,$0A
            .byte $0C,$0A,$15,$19,$1D,$02,$03,$00
            .byte $00,$00,$00,$00,$49,$90,$55,$16
            .byte $47,$80,$D4,$01,$52,$0B,$A1,$CA
            .byte $ED,$0E,$40,$8C,$CE,$30,$BB,$00
            .byte $00,$00,$00,$50,$60,$70,$04,$9A
            .byte $2C,$92,$28,$30,$B8,$00,$A8,$00
            .byte $50,$60,$70,$88,$30,$60,$00,$00
            .byte $00
L2BA8:
            .byte $FF,$FF,$FA,$F0,$E0,$C0,$80		; Explosion1 mask (3x24 bytes)
            .byte $80,$80,$C0,$C0,$C0,$C0,$E0,$E0
            .byte $E0,$E0,$C0,$C0,$C0,$E0,$F8,$FC
            .byte $FF,$FF,$FF,$B6,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$44
            .byte $FF,$FF,$AF,$07,$0F,$03,$01,$00
            .byte $01,$00,$01,$07,$03,$07,$03,$07
            .byte $07,$0F,$07,$03,$07,$0F,$1F,$FF
            .byte $FF
L2BF0:
            .byte $00,$00,$00,$00,$01,$01,$00		; Explosion2 (3x24 bytes)
            .byte $05,$0A,$0F,$0E,$04,$0A,$14,$1D
            .byte $1D,$0C,$02,$07,$03,$00,$00,$00
            .byte $00,$00,$00,$50,$A8,$F4,$00,$A9
            .byte $32,$B8,$05,$A6,$C7,$E0,$1C,$4A
            .byte $9C,$C1,$06,$DF,$8E,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$A0,$D0
            .byte $E8,$70,$68,$30,$00,$A8,$B0,$38
            .byte $00,$D0,$E0,$00,$00,$00,$00,$00
            .byte $00
L2C38:
            .byte $FF,$FF,$FF,$FE,$FC,$FC,$FA		; Explosion2 mask (3x24 bytes)
            .byte $F0,$E0,$E0,$E0,$F0,$E0,$C0,$C0
            .byte $C0,$E0,$F0,$F0,$F8,$FC,$FF,$FF
            .byte $FF,$FF,$AF,$07,$03,$01,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$20,$71,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$5F,$0F,$07
            .byte $03,$07,$03,$07,$07,$03,$07,$03
            .byte $07,$07,$0F,$1F,$FF,$FF,$FF,$FF
            .byte $FF
L2C80:
            .byte $00,$00,$00,$00,$00,$00,$00		; Explosion3 (3x24 bytes)
            .byte $01,$01,$04,$05,$05,$01,$04,$0A
            .byte $0F,$0E,$06,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$0A,$05,$A6
            .byte $53,$E1,$94,$5A,$9D,$C3,$28,$B1
            .byte $19,$A5,$D8,$E5,$1B,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$80
            .byte $00,$C0,$20,$88,$20,$B8,$10,$40
            .byte $80,$C0,$00,$80,$00,$00,$00,$00
            .byte $00
L2CC8:
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FE		; Explosion3 mask (3x24 bytes)
            .byte $FC,$F8,$F0,$F0,$F0,$F8,$F0,$E0
            .byte $E0,$E0,$F0,$F8,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$F5,$E0,$40,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$E4,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$7F,$3F
            .byte $3F,$1F,$07,$03,$07,$03,$07,$0F
            .byte $3F,$1F,$3F,$3F,$7F,$FF,$FF,$FF
            .byte $FF
L2D10:
            .byte $00,$00,$00,$00,$00,$00,$00		; Explosion4 (3x24 bytes)
            .byte $00,$04,$0A,$0F,$0C,$02,$03,$03
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$50,$A9
            .byte $F2,$E5,$66,$17,$28,$BC,$39,$82
            .byte $2B,$B3,$38,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$20,$50,$60,$D0,$E0,$00,$80
            .byte $00,$80,$00,$00,$00,$00,$00,$00
            .byte $00
L2D58:
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FE		; Explosion4 mask (3x24 bytes)
            .byte $FA,$F0,$E0,$E0,$E0,$F0,$F8,$F8
            .byte $FC,$FE,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$8F,$06,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$C7,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$7F
            .byte $5F,$0F,$07,$07,$07,$0F,$1F,$3F
            .byte $3F,$3F,$7F,$FF,$FF,$FF,$FF,$FF
            .byte $FF
L2DA0:
            .byte $00,$00,$01,$03,$03,$01,$00		; Bullet (2x8 bytes)
            .byte $00,$00,$00,$80,$C0,$C0,$80,$00
            .byte $00
L2DB0:
            .byte $FF,$FE,$FC,$F8,$F8,$FC,$FE		; Bullet mask (2x8 bytes)
            .byte $FF,$FF,$7F,$3F,$1F,$1F,$3F,$7F
            .byte $FF

;------------------------------------------------

L2DC0:
            .byte $B0,$42,$95,$AB,$1F,$2F,$5E
            .byte $3D,$00,$AA,$4D,$EF,$EF,$4D,$AA
            .byte $4D,$5A,$3D,$5A,$00,$7F,$3D,$5A
            .byte $3D,$AA,$4D,$AA,$1D,$FA,$50,$AA
            .byte $51,$00,$AA,$4D,$EF,$EF,$4D,$AA
            .byte $4D,$00,$AA,$4D,$EF,$EF,$4D,$AA
            .byte $4D,$AA,$4D,$AA,$48,$AF,$15,$AA
            .byte $75,$AA,$4D,$AA,$1D,$FA,$50,$AA
            .byte $51,$5A,$BD,$7A,$FD,$FA,$5D,$AA
            .byte $4D,$80,$02,$85,$0F,$8F,$1D,$8A
            .byte $4D,$AA,$4D,$AA,$48,$AF,$15,$AA
            .byte $75,$AA,$4D,$AA,$1D,$FA,$50,$AA
            .byte $51,$00,$AA,$4D,$EF,$EF,$4D,$AA
            .byte $4D,$15,$A2,$51,$E8,$FD,$FE,$BE
            .byte $1C,$AA,$4D,$AA,$48,$AF,$15,$AA
            .byte $75,$80,$00,$80,$00,$D4,$00,$80
            .byte $00,$AA,$75,$EA,$50,$AF,$4D,$AA
            .byte $4D,$80,$02,$85,$0F,$AF,$15,$8A
            .byte $55,$AA,$4D,$AA,$48,$AF,$15,$AA
            .byte $75,$AA,$54,$A0,$00,$C0,$40,$80
            .byte $00,$AA,$75,$EA,$50,$AF,$4D,$AA
            .byte $4D,$AA,$51,$AB,$05,$FA,$4D,$AA
            .byte $4D,$AA,$01,$00,$05,$1A,$3D,$5A
            .byte $3D,$AA,$4D,$AA,$1D,$FA,$50,$AA
            .byte $51,$AA,$75,$EA,$50,$AF,$4D,$AA
            .byte $4D,$AA,$51,$AB,$05,$FA,$4D,$AA
            .byte $4D,$AA,$4D,$AA,$48,$AF,$15,$AA
            .byte $75,$AA,$40,$80,$00,$80,$00,$80
            .byte $00,$5A,$BD,$7A,$FC,$FF,$55,$AA
            .byte $55,$AA,$51,$AB,$05,$FA,$4D,$AA
            .byte $4D,$AA,$01,$00,$04,$1F,$3D,$5A
            .byte $3D,$AA,$4D,$AA,$1D,$FA,$50,$AA
            .byte $51,$5A,$3D,$5A,$00,$7F,$3D,$5A
            .byte $3D,$AA,$51,$AB,$05,$FA,$4D,$AA
            .byte $4D,$5A,$3D,$5A,$00,$7F,$3D,$5A
            .byte $3D,$AA,$4D,$AA,$1D,$FA,$50,$AA
            .byte $51,$AA,$75,$EA,$50,$AF,$4D,$AA
            .byte $4D,$80,$02,$85,$0F,$8F,$15,$8A
            .byte $55,$AA,$4D,$AA,$48,$AF,$15,$AA
            .byte $75,$AA,$4D,$AA,$1D,$FA,$50,$AA
            .byte $51,$5A,$BD,$7A,$FD,$FA,$55,$AA
            .byte $55,$AA,$51,$AB,$05,$FA,$4D,$AA
            .byte $4D,$AA,$4D,$AA,$48,$AF,$15,$AA
            .byte $75,$AA,$4D,$AA,$1D,$FA,$50,$AA
            .byte $51,$AA,$75,$EA,$50,$AF,$4D,$AA
            .byte $4D,$80,$00,$80,$00,$D4,$00,$80
            .byte $00,$AA,$4D,$AA,$48,$AF,$15,$AA
            .byte $75,$80,$00,$80,$00,$D4,$00,$80
            .byte $00,$AA,$75,$EA,$50,$AF,$4D,$AA
            .byte $4D,$80,$00,$80,$00,$D4,$00,$80
            .byte $00,$AA,$08,$00,$08,$00,$08,$00
            .byte $00,$00,$00,$00,$01,$01,$02,$05
            .byte $1A,$AA,$75,$EA,$50,$AF,$4D,$AA
            .byte $4D,$AA,$51,$AB,$05,$FA,$4D,$AA
            .byte $4D,$AA,$01,$08,$05,$1A,$3D,$5A
            .byte $3D,$AA,$4C,$A0,$40,$80,$40,$80
            .byte $00,$AA,$75,$EA,$50,$AF,$4D,$AA
            .byte $4D,$AA,$51,$AB,$05,$FA,$4D,$AA
            .byte $4D,$AA,$08,$00,$08,$00,$08,$00
            .byte $00,$AA,$08,$00,$08,$00,$08,$00
            .byte $00,$5A,$3D,$5A,$00,$7F,$3D,$5A
            .byte $3D,$AA,$51,$AB,$05,$FA,$4D,$AA
            .byte $4D,$5E,$3C,$5E,$AF,$1F,$8F,$46
            .byte $A8,$AA,$08,$00,$08,$00,$08,$00
            .byte $00,$5A,$3D,$5A,$00,$7F,$3D,$5A
            .byte $3D,$80,$00,$80,$00,$C0,$00,$80
            .byte $00,$5A,$3D,$5A,$00,$7F,$3D,$5A
            .byte $3D,$80,$00,$80,$00,$C0,$00,$80
            .byte $00,$5A,$BD,$7A,$FD,$FA,$55,$AA
            .byte $55,$80,$02,$85,$0F,$8F,$15,$8A
            .byte $55,$AA,$00,$00,$00,$00,$00,$00
            .byte $00,$AA,$00,$00,$00,$00,$00,$00
            .byte $00,$5A,$3D,$5A,$3D,$5A,$3D,$5A
            .byte $3D,$80,$02,$85,$0F,$8F,$15,$8A
            .byte $55,$5A,$3D,$5A,$3D,$5A,$3D,$5A
            .byte $3D,$AA,$54,$A0,$40,$80,$40,$80
            .byte $00,$5A,$BD,$7A,$FD,$FA,$55,$AA
            .byte $55,$80,$00,$80,$00,$80,$00,$80
            .byte $00,$AA,$01,$00,$05,$1A,$3D,$5A
            .byte $3D,$80,$00,$80,$00,$80,$00

L303E:
            .byte $80
            .byte $00,$B0,$42,$95,$AB,$1F,$2F,$5F
            .byte $3F,$00,$AA,$55,$FF,$FF,$F5,$AA
            .byte $55,$5E,$3D,$5E,$3D,$5A,$3D,$5A
            .byte $3D,$AA,$54,$A0,$40,$80,$40,$80
            .byte $00,$5A,$BD,$7A,$FD,$FA,$55,$AA
            .byte $55,$80,$00,$80,$00,$80,$00,$00
            .byte $00,$A8,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$01,$01,$02,$05
            .byte $1A,$00,$AA,$55,$FF,$FF,$55,$AA
            .byte $55,$1A,$A5,$52,$E9,$FD,$FE,$BE
            .byte $5E,$AA,$01,$00,$05,$1A,$3D,$5A
            .byte $3D,$AE,$56,$AC,$00,$80,$00,$80
            .byte $00,$00,$AA,$4D,$EF,$EF,$4D,$AA
            .byte $4D,$00,$AA,$4D,$EF,$EF,$4D,$AA
            .byte $4D,$AA,$08,$00,$00,$00,$00,$00
            .byte $00,$AA,$08,$00,$00,$00,$00,$00
            .byte $00,$5A,$BD,$7A,$FD,$FA,$55,$AA
            .byte $55,$80,$02,$85,$0F,$8F,$15,$8A
            .byte $55,$AA,$01,$00,$05,$1A,$3D,$5A
            .byte $3D,$AA,$54,$A0,$40,$80,$40,$80
            .byte $00,$5A,$3D,$5A,$3D,$5A,$3D,$5A
            .byte $3D,$80,$02,$85,$0F,$8F,$15,$8A
            .byte $55,$5E,$3D,$5E,$AF,$9F,$4F,$A7
            .byte $58,$AA,$40,$A0,$40,$A0,$E0,$C0
            .byte $00,$00,$AA,$55,$FF,$FF,$55,$AA
            .byte $55,$00,$AA,$55,$FF,$FF,$55,$AA
            .byte $55,$AA,$01,$00,$05,$1A,$3D,$5A
            .byte $3D,$AA,$54,$A0,$40,$80,$40,$80
            .byte $00,$AA,$75,$EA,$50,$AF,$4D,$AA
            .byte $4D,$AA,$51,$AB,$05,$FA,$4D,$AA
            .byte $4D,$AA,$4D,$AA,$48,$AF,$15,$AA
            .byte $75,$AA,$4D,$AA,$1D,$FA,$50,$AA
            .byte $51,$5A,$3D,$5A,$00,$7F,$00,$60
            .byte $7E,$AA,$51,$AB,$05,$FA,$0D,$0A
            .byte $0D,$6F,$4F,$7F,$00,$7F,$3D,$5A
            .byte $3D,$EA,$ED,$EA,$1D,$FA,$50,$AA
            .byte $51,$00,$AB,$4B,$E9,$E9,$49,$A9
            .byte $48,$00,$EA,$AD,$2F,$EF,$ED,$EA
            .byte $ED,$A8,$48,$A8,$48,$AF,$15,$AA
            .byte $75,$EA,$ED,$EA,$1D,$FA,$50,$AA
            .byte $51,$AA,$75,$EA,$50,$AF,$48,$A8
            .byte $48,$AA,$51,$AB,$05,$FA,$0D,$0A
            .byte $0D,$A8,$08,$00,$08,$01,$09,$03
            .byte $00,$EA,$E8,$E0,$E8,$A0,$28,$E0
            .byte $00,$AA,$75,$EA,$50,$AF,$48,$A8
            .byte $48,$80,$00,$80,$00,$D4,$00,$02
            .byte $0E,$A8,$48,$A8,$48,$AF,$15,$AA
            .byte $75,$FA,$F2,$FE,$00,$D4,$00,$80
            .byte $00,$04,$04,$04,$04,$F5,$04,$04
            .byte $F8,$30,$08,$08,$08,$87,$40,$40
            .byte $43,$00,$00,$03,$04,$04,$F5,$04
            .byte $04,$44,$44,$84,$04,$04,$83,$40
            .byte $40,$04,$04,$7B,$80,$03,$04,$04
            .byte $F5,$40,$47,$58,$40,$5F,$40,$40
            .byte $81,$04,$04,$35,$44,$44,$84,$04
            .byte $04,$02,$32,$CA,$0A,$08,$37,$40
            .byte $44,$04,$00,$35,$25,$87,$38,$08
            .byte $B8,$20,$00,$58,$48,$C3,$38,$A0
            .byte $BB,$0B,$38,$07,$25,$35,$80,$04
            .byte $04,$A0,$38,$C0,$48,$58,$03,$40
            .byte $40,$04,$00,$3F,$40,$50,$48,$00
            .byte $FE,$40,$00,$F8,$04,$03,$70,$80
            .byte $BF,$00,$48,$48,$47,$40,$84,$04
            .byte $04,$80,$88,$88,$08,$48,$47,$40
            .byte $40,$04,$00,$04,$00,$55,$00,$04
            .byte $A8,$10,$08,$00,$08,$05,$00,$40
            .byte $02,$00,$00,$01,$00,$04,$A0,$04
            .byte $00,$44,$00,$04,$00,$04,$82,$40
            .byte $00,$04,$00,$51,$80,$01,$00,$04
            .byte $A0,$40,$02,$50,$00,$55,$00,$40
            .byte $80,$04,$00,$15,$00,$44,$80,$04
            .byte $00,$00,$22,$40,$0A,$00,$22,$40
            .byte $00,$00,$7F,$40,$40,$40,$40,$40
            .byte $40,$00,$FE,$02,$02,$02,$02,$02
            .byte $02,$40,$40,$40,$40,$40,$40,$7F
            .byte $00,$02,$02,$02,$02,$02,$02,$FE
            .byte $00,$04,$04,$03,$00,$FF,$00,$01
            .byte $FA,$40,$00,$E0,$10,$57,$50,$90
            .byte $11,$42,$42,$4A,$48,$47,$80,$04
            .byte $04,$12,$E2,$02,$02,$82,$5F,$40
            .byte $40,$00,$3C,$01,$3C,$35,$00,$7F
            .byte $6D,$00,$3C,$00,$3C,$35,$00,$7F
            .byte $77,$49,$7F,$00,$04,$20,$38,$3C
            .byte $34,$66,$7F,$00,$28,$20,$20,$00
            .byte $00,$00,$3C,$01,$3C,$35,$00,$FE
            .byte $76,$00,$3C,$00,$3C,$35,$00,$00
            .byte $66,$66,$FE,$00,$04,$20,$38,$00
            .byte $00,$44,$00,$00,$28,$20,$A0,$04
            .byte $04,$24,$3C,$3D,$24,$24,$AD,$2C
            .byte $AD,$24,$3C,$3C,$24,$24,$AD,$2C
            .byte $AD,$AD,$AD,$2C,$2C,$2C,$3C,$3C
            .byte $34,$AD,$AD,$2C,$2C,$AC,$3C,$BC
            .byte $34,$24,$3C,$3D,$24,$A5,$00,$7F
            .byte $6D,$24,$3C,$3C,$24,$A5,$00,$7F
            .byte $77,$49,$7F,$00,$04,$20,$00,$00
            .byte $00,$66,$7F,$00,$20,$B0,$00,$00
            .byte $00,$24,$3C,$3D,$24,$A5,$00,$FE
            .byte $76,$24,$3C,$3C,$24,$A5,$00,$00
            .byte $66,$66,$FE,$00,$04,$20,$00,$00
            .byte $00,$44,$00,$00,$20,$B0,$00,$00
            .byte $00,$05,$00,$FF,$60,$67,$FF,$00
            .byte $45,$C0,$00,$FF,$07,$FE,$FF,$00
            .byte $C4,$15,$00,$FF,$60,$67,$FF,$00
            .byte $05,$C6,$00,$FF,$07,$FE,$FF,$00
            .byte $C8,$00,$03,$5B,$5A,$53,$5B,$02
            .byte $43,$00,$C0,$4F,$47,$C6,$53,$40
            .byte $C0,$18,$03,$5B,$5B,$52,$5B,$03
            .byte $03,$00,$C0,$DC,$40,$50,$C0,$C0
            .byte $40,$08,$03,$FB,$62,$63,$FB,$02
            .byte $4B,$00,$C0,$48,$40,$C0,$50,$40
            .byte $C0,$08,$03,$FB,$63,$62,$FB,$03
            .byte $0B,$00,$C0,$D8,$48,$40,$C0,$C0
            .byte $48,$0A,$03,$FB,$63,$62,$FB,$03
            .byte $48,$40,$C0,$C8,$40,$40,$D0,$C0
            .byte $00,$08,$01,$F9,$60,$60,$F9,$01
            .byte $08,$04,$80,$18,$08,$00,$80,$00
            .byte $08,$02,$03,$5B,$5B,$52,$5B,$03
            .byte $40,$40,$C0,$CC,$44,$44,$D0,$C0
            .byte $00,$18,$01,$59,$58,$50,$59,$01
            .byte $00,$04,$80,$1C,$00,$10,$83,$00
            .byte $00,$EB,$57,$AF,$7E,$FB,$F3,$3F
            .byte $7F,$DB,$B7,$6E,$DD,$EA,$C5,$EF
            .byte $7F,$3E,$3F,$1F,$93,$8F,$CF,$C7
            .byte $E7,$FF,$FD,$EA,$C5,$EF,$DF,$FE
            .byte $ED,$EB,$57,$AF,$7E,$FB,$F3,$BF
            .byte $7F,$DB,$B7,$6E,$DD,$EA,$C5,$EF
            .byte $7F,$BE,$7F,$FB,$F3
L3434:
            .byte $AF,$5F,$BF
            .byte $7F,$FF,$FC,$EA,$C5,$ED,$DB,$FB
            .byte $E7,$63,$73,$31,$39,$18,$1C,$0C
            .byte $0E,$DB,$B7,$6E,$DD,$AA,$D5,$7F
            .byte $7F,$06,$07,$23,$03,$01,$41,$00
            .byte $04,$3F,$3D,$0A,$85,$8F,$CF,$C7
            .byte $E5,$00,$10,$00,$20,$00,$20,$08
            .byte $00,$63,$73,$30,$39,$98,$1C,$0C
            .byte $8E,$00,$00,$21,$00,$00,$41,$00
            .byte $04,$06,$07,$0B,$03,$01,$11,$00
            .byte $04,$00,$3F,$7F,$60,$60,$63,$67
            .byte $67,$00,$FF,$FF,$00,$00,$C5,$EF
            .byte $7F,$66,$67,$63,$63,$67,$67,$67
            .byte $67,$FF,$FD,$EA,$C5,$EF,$DF,$FE
            .byte $ED,$63,$67,$67,$66,$63,$63,$67
            .byte $67,$DB,$B7,$6E,$DD,$EA,$C5,$EF
            .byte $7F,$66,$67,$63,$63,$67,$67,$67
            .byte $67,$FF,$FD,$EA,$C5,$EF,$DF,$FE
            .byte $ED,$63,$73,$31,$39,$18,$1C,$0C
            .byte $0E,$DB,$B7,$6E,$DD,$AA,$55,$3F
            .byte $3F,$06,$07,$23,$03,$01,$41,$00
            .byte $04,$1F,$0D,$0A,$85,$83,$C1,$C1
            .byte $E0,$00,$10,$00,$20,$00,$20,$08
            .byte $00,$60,$70,$30,$38,$98,$1C,$0C
            .byte $8E,$00,$00,$21,$00,$00,$41,$00
            .byte $04,$06,$07,$0B,$03,$01,$11,$00
            .byte $04,$11,$54,$92,$92,$82,$42,$7C
            .byte $01,$88,$2A,$49,$49,$41,$42,$3E
            .byte $01,$0E,$07,$03,$83,$80,$FF,$FF
            .byte $00,$FF,$FC,$EA,$C5,$01,$FF,$FF
            .byte $00,$E6,$4E,$2C,$5C,$58,$B8,$B0
            .byte $70,$00,$80,$04,$00,$90,$00,$02
            .byte $88,$60,$E0,$C1,$C0,$80,$81,$00
            .byte $04,$00,$00,$08,$00,$02,$10,$00
            .byte $04,$EB,$57,$AF,$7E,$FB,$F3,$BF
            .byte $7F,$D6,$AE,$6C,$DC,$D8,$B8,$B2
            .byte $70,$BE,$7E,$FA,$F1,$AD,$5B,$BB
            .byte $77,$60,$E0,$C8,$C0,$82,$90,$00
            .byte $04,$EB,$57,$AF,$7E,$FB,$F3,$BF
            .byte $7F,$D6,$B6,$66,$D6,$E6,$C6,$E6
            .byte $76,$BE,$7F,$FB,$F3,$AF,$5F,$BF
            .byte $7F,$F6,$F6,$E6,$C6,$E6,$D6,$F6
            .byte $E6,$00,$FF,$FF,$00,$00,$F3,$BF
            .byte $7F,$00,$FC,$FE,$06,$06,$C6,$E6
            .byte $76,$BE,$7F,$FB,$F3,$AF,$5F,$BF
            .byte $7F,$F6,$F6,$E6,$C6,$E6,$D6,$F6
            .byte $E6,$00,$FF,$FF,$00,$00,$F3,$BF
            .byte $7F,$00,$FF,$FF,$00,$00,$C5,$EF
            .byte $7F,$BE,$7F,$FB,$F3,$AF,$5F,$BF
            .byte $7F,$FF,$FD,$EA,$C5,$EF,$DF,$FE
            .byte $ED,$EB,$57,$AF,$7E,$FB,$F3,$BF
            .byte $7F,$DB,$B7,$6E,$DD,$EA,$C5,$EF
            .byte $7F,$BE,$7F,$FB,$F3,$AF,$5F,$BF
            .byte $7F,$FF,$FD,$EA,$C5,$EF,$DF,$FE
            .byte $ED,$83,$55,$52,$42,$42,$52,$54
            .byte $11,$C1,$AA,$4A,$42,$42,$4A,$2A
            .byte $89,$91,$54,$52,$42,$42,$52,$54
            .byte $11,$89,$2A,$4A,$42,$42,$4A,$2A
            .byte $89,$05,$00,$FF,$60,$67,$FF,$00
            .byte $05,$C0,$00,$FF,$07,$FE,$FF,$00
            .byte $C0,$05,$00,$FF,$60,$67,$FF,$00
            .byte $05,$C0,$00,$FF,$07,$FE,$FF,$00
            .byte $C0,$05,$00,$55,$20,$45,$AA,$00
            .byte $02,$40,$00,$55,$02,$54,$AA,$00
            .byte $80,$05,$00,$55,$20,$45,$AA,$00
            .byte $02,$40,$00,$55,$02,$54,$AA,$00
            .byte $80,$FB,$FF,$FB,$FF,$FF,$DF,$B8
            .byte $F0,$FF,$DF,$FF,$FF,$B7,$0F,$07
            .byte $02,$C0,$90,$00,$20,$00,$00,$08
            .byte $00,$01,$00,$05,$00,$90,$00,$02
            .byte $08,$FB,$FF,$FB,$FF,$FF,$DF,$BF
            .byte $FF,$FF,$DF,$FF,$FF,$B7,$BF,$FF
            .byte $FE,$FB,$F9,$BE,$0F,$07,$00,$08
            .byte $00,$FD,$E0,$C4,$C0,$90,$00,$02
            .byte $08,$FB,$FF,$FB,$FF,$FF,$DF,$BF
            .byte $FF,$C0,$80,$08,$00,$80,$80,$F0
            .byte $FC,$FB,$F9,$FE,$FF,$EF,$FF,$BF
            .byte $FF,$FC,$DE,$FA,$E0,$80,$00,$02
            .byte $C8,$03,$07,$21,$00,$00,$41,$00
            .byte $00,$FF,$DF,$FF,$FF,$37,$3F,$3F
            .byte $9E,$00,$10,$00,$22,$00,$00,$08
            .byte $03,$1D,$9E,$3B,$3F,$6F,$FF,$7F
            .byte $FE,$03,$0F,$23,$01,$01,$41,$01
            .byte $07,$FF,$DF,$FF,$FF,$B7,$BF,$FF
            .byte $FE,$0B,$01,$06,$23,$03,$03,$07
            .byte $07,$FD,$DE,$FB,$FF,$EF,$FF,$7F
            .byte $FE,$FB,$FF,$FB,$FF,$FF,$DF,$BF
            .byte $FF,$E0,$C0,$F0,$F0,$B0,$BC,$FE
            .byte $FE,$FB,$F9,$FE,$FF,$EF,$FF,$BF
            .byte $FF,$FC,$DE,$F8,$F8,$E8,$F0,$62
            .byte $E8,$00,$00,$21,$00,$00,$C1,$B0
            .byte $F8,$00,$00,$08,$00,$00,$11,$07
            .byte $06,$F8,$F8,$FE,$FF,$EF,$FF,$BF
            .byte $FF,$0D,$1E,$3B,$FF,$EF,$FF,$7F
            .byte $FE,$00,$00,$20,$03,$1F,$DF,$BF
            .byte $FF,$00,$00,$08,$00,$80,$81,$EF
            .byte $FE,$FB,$F9,$FE,$FF,$EF,$FF,$BF
            .byte $FF,$FD,$DE,$FB,$FF,$EF,$FF,$7F
            .byte $FE,$FB,$F9,$FE,$FF,$EF,$FF,$BF
            .byte $FF,$FD,$DE,$FB,$FF,$EF,$FF,$7F
            .byte $FE,$FB,$FF,$FB,$FF,$FF,$DF,$BF
            .byte $FF,$FF,$DF,$FF,$FC,$BA,$A4,$F0
            .byte $E8,$FB,$F9,$FE,$FF,$EF,$FF,$BF
            .byte $FF,$E4,$C8,$E4,$F9,$FA,$FF,$7F
            .byte $FE,$FB,$FF,$FB,$FF,$FF,$DF,$BF
            .byte $FF,$FF,$DF,$FF,$FF,$B7,$BF,$FF
            .byte $FE,$13,$09,$A6,$5F,$1F,$FF,$BF
            .byte $FF,$FD,$DE,$FB,$FF,$EF,$FF,$7F
            .byte $FE,$FB,$FF,$FB,$FF,$FF,$DF,$BF
            .byte $FF,$FF,$DF,$FF,$FF,$B7,$BF,$FF
            .byte $FE,$FB,$F9,$FE,$FF,$EF,$FF,$BF
            .byte $FF,$FD,$DE,$FB,$FF,$EF,$FF,$7F
            .byte $FE,$FB,$FF,$FB,$5F,$9F,$27,$17
            .byte $27,$FF,$DF,$FF,$FF,$B7,$BF,$FF
            .byte $FE,$00,$80,$04,$00,$90,$00,$02
            .byte $88,$00,$10,$00,$18,$2E,$3F,$3F
            .byte $3F,$00,$04,$1B,$1F,$17,$0F,$07
            .byte $06,$3B,$7F,$FB,$FF,$FF,$DF,$BF
            .byte $FF,$00,$80,$04,$00,$10,$80,$02
            .byte $C8,$00,$10,$00,$20,$00,$20,$08
            .byte $00,$E0,$D8,$F0,$E0,$A2,$B0,$FF
            .byte $FE,$00,$00,$21,$00,$00,$01,$80
            .byte $C4,$FD,$DE,$FB,$FF,$EF,$FF,$7F
            .byte $FE,$E0,$D0,$80,$A0,$80,$C0,$88
            .byte $E0,$FF,$C3,$88,$00,$02,$10,$00
            .byte $04,$C0,$00,$21,$00,$00,$41,$00
            .byte $04,$05,$82,$01,$00,$90,$00,$02
            .byte $80,$FB,$F9,$FE,$7F,$6F,$3F,$3F
            .byte $7F,$07,$0F,$1D,$1F,$07,$00,$00
            .byte $04,$FB,$C7,$83,$80,$80,$41,$00
            .byte $04,$D5,$88,$C5,$E2,$E1,$FA,$B1
            .byte $F2,$55,$A8,$15,$AA,$51,$2A,$45
            .byte $AA,$F5,$F2,$F8,$FF,$FF,$DF,$BF
            .byte $FF,$11,$AA,$55,$00,$B4,$BE,$FF
            .byte $FE,$55,$AA,$51,$AA,$55,$8A,$55
            .byte $AA,$55,$A2,$55,$AA,$45,$2A,$55
            .byte $AA,$15,$A8,$55,$AA,$41,$0A,$B8
            .byte $FF,$45,$AA,$55,$AA,$51,$84,$1F
            .byte $FE,$55,$AA,$44,$AA,$55,$AA,$45
            .byte $AA,$51,$AA,$53,$A9,$45,$A9,$03
            .byte $7E,$54,$8A,$51,$AA,$54,$28,$83
            .byte $FF,$FF,$5F,$7F,$7F,$37,$BF,$FF
            .byte $FE,$55,$AA,$55,$8A,$55,$A8,$55
            .byte $AA,$51,$A6,$4B,$8F,$4F,$8F,$47
            .byte $AA,$45,$AA,$55,$AA,$55,$A8,$55
            .byte $AA,$53,$2B,$57,$A7,$47,$AF,$47
            .byte $AA,$55,$8A,$54,$AA,$55,$A2,$55
            .byte $AA,$55,$AA,$45,$AA,$55,$AA,$45
            .byte $AA,$45,$AA,$51,$AA,$15,$AA,$45
            .byte $AA,$15,$AA,$51,$AA,$15,$AA,$55
            .byte $AA,$D5,$C8,$E5,$F2,$E9,$FA,$B5
            .byte $F2,$55,$AA,$51,$AA,$15,$AA,$55
            .byte $A2,$F5,$F2,$F9,$FA,$F9,$D2,$A5
            .byte $CA,$55,$2A,$55,$A2,$55,$2A,$51
            .byte $AA,$FB,$F9,$FE,$FF,$EF,$FF,$BF
            .byte $FF,$FD,$DE,$FA,$F8,$E1,$82,$15
            .byte $2A,$FA,$FC,$F8,$FC,$F9,$C2,$85
            .byte $CA,$51,$AA,$51,$AA,$55,$8A,$55
            .byte $AA,$FB,$E1,$06,$A0,$55,$AA,$55
            .byte $A2,$FD,$DE,$08,$28,$41,$AA,$15
            .byte $AA,$55,$AA,$55,$8A,$54,$AA,$55
            .byte $AA,$45,$AA,$55,$AA,$55,$A2,$55
            .byte $AA,$FB,$19,$40,$AA,$55,$AA,$51
            .byte $AA,$FD,$DE,$FB,$3F,$0F,$9F,$1F
            .byte $8E,$55,$8A,$55,$A2,$54,$AA,$15
            .byte $AA,$53,$A3,$57,$A7,$57,$A7,$47
            .byte $A2,$55,$AA,$51,$AA,$55,$AA,$55
            .byte $AA,$55,$A2,$55,$AA,$55,$2A,$54
            .byte $AA,$45,$AA,$54,$AA,$45,$AA,$55
            .byte $A2,$15,$AA,$55,$AA,$51,$A0,$53
            .byte $A6,$55,$AA,$55,$A8,$55,$AA,$55
            .byte $8A,$51,$A6,$50,$A0,$55,$AA,$15
            .byte $AA,$55,$A2,$55,$AA,$55,$AA,$55
            .byte $AA,$55,$AA,$51,$AA,$55,$8A,$55
            .byte $AA,$D5,$0A,$55,$AA,$55,$A8,$55
            .byte $AA,$55,$AA,$55,$2A,$45,$AA,$55
            .byte $AA,$55,$A2,$55,$AA,$55,$8A,$55
            .byte $AA,$55,$AA,$45,$AA,$55,$AA,$55
            .byte $AA,$55,$AA,$15,$AA,$55,$88,$55
            .byte $AA,$55,$8A,$55,$AA,$55,$AA,$45
            .byte $AA,$55,$2A,$54,$AA,$05,$0A,$85
            .byte $CA,$55,$AA,$55,$AA,$45,$AA,$55
            .byte $8A,$55,$AA,$51,$A8,$41,$A2,$54
            .byte $A8,$55,$AA,$55,$A2,$11,$C2,$85
            .byte $0A,$54,$A0,$51,$A2,$58,$90,$41
            .byte $A2,$11,$A4,$51,$2A,$95,$2A,$55
            .byte $AA,$55,$AA,$54,$A1,$59,$91,$40
            .byte $A0,$51,$AC,$08,$40,$81,$42,$85
            .byte $02,$40,$9C,$58,$94,$40,$A0,$55
            .byte $AA,$49,$00,$01,$12,$81,$02,$15
            .byte $AA,$55,$AA,$50,$8A,$5D,$BA,$15
            .byte $AA,$55,$AA,$05,$62,$45,$02,$19
            .byte $10,$14,$80,$41,$A1,$55,$A8,$54
            .byte $AA,$01,$02,$C5,$8A,$45,$0A,$15
            .byte $AA,$55,$AA,$44,$91,$41,$A0,$12
            .byte $47,$55,$AA,$15,$8A,$15,$0A,$01
            .byte $18,$02,$85,$50,$AC,$48,$A0,$51
            .byte $AA,$91,$00,$01,$0A,$11,$A4,$50
            .byte $A8,$55,$8A,$50,$A0,$46,$A1,$41
            .byte $8D,$55,$A2,$00,$4C,$1A,$DC,$5A
            .byte $ED,$10,$87,$4F,$8A,$05,$A0,$45
            .byte $89,$61,$0A,$D4,$CA,$14,$61,$ED
            .byte $5A,$55,$0A,$04,$82,$19,$E2,$A1
            .byte $EC,$55,$AA,$45,$AA,$55,$AA,$45
            .byte $AA,$82,$38,$FC,$D4,$29,$80,$E8
            .byte $A4,$15,$AA,$51,$AA,$15,$8A,$05
            .byte $0A,$E0,$10,$80,$00,$00,$00,$00
            .byte $00,$05,$02,$01,$02,$05,$02,$05
            .byte $0A,$00,$00,$00,$00,$00,$88,$45
            .byte $AA,$05,$02,$01,$0A,$15,$AA,$55
            .byte $AA,$41,$82,$50,$A8,$55,$A2,$55
            .byte $AA,$DC,$1A,$4C,$00,$00,$80,$40
            .byte $A0,$45,$AA,$51,$AA,$15,$AA,$45
            .byte $AA,$00,$80,$50,$A0,$14,$AA,$55
            .byte $AA,$00,$7F,$40,$58,$58,$4B,$47
            .byte $45,$00,$FE,$02,$1A,$1A,$D2,$E2
            .byte $A2,$47,$43,$4A,$59,$58,$40,$7F
            .byte $00,$E2,$C2,$52,$9A,$1A,$02,$FE
            .byte $00,$55,$8A,$54,$AA,$54,$A0,$54
            .byte $A8,$40,$8E,$16,$3D,$65,$D5,$CD
            .byte $E3,$40,$A2,$53,$A0,$10,$A2,$43
            .byte $A0,$1A,$7A,$5A,$56,$55,$55,$77
            .byte $38,$50,$00,$07,$EF,$2C,$6B,$2B
            .byte $6A,$15,$08,$E0,$D0,$21,$90,$01
            .byte $90,$A8,$EC,$AF,$E8,$AB,$64,$C3
            .byte $03,$00,$10,$E0,$10,$E0,$22,$00
            .byte $80,$01,$70,$68,$3C,$26,$2B,$33
            .byte $07,$55,$AA,$45,$2A,$15,$0A,$05
            .byte $02,$80,$00,$84,$04,$44,$84,$00
            .byte $00,$01,$42,$C1,$00,$00,$40,$C0
            .byte $00,$50,$82,$53,$A0,$50,$A2,$53
            .byte $A0,$00,$A0,$90,$20,$10,$A0,$90
            .byte $20,$40,$A2,$53,$A0,$10,$A2,$43
            .byte $A0,$10,$A0,$90,$20,$10,$A0,$90
            .byte $20,$00,$29,$65,$68,$64,$69,$65
            .byte $68,$00,$40,$C0,$00,$00,$40,$C0
            .byte $00,$64,$69,$65,$68,$64,$69,$65
            .byte $68,$00,$40,$C0,$00,$00,$40,$C0
            .byte $00,$50,$82,$53,$A8,$50,$A1,$51
            .byte $A9,$11,$A3,$91,$00,$F0,$01,$10
            .byte $01,$40,$A8,$50,$AA,$15,$AA,$45
            .byte $AA,$90,$40,$00,$00,$00,$A0,$50
            .byte $AA,$07,$F8,$FF,$0F,$A0,$17,$14
            .byte $10,$E0,$1F,$FF,$F0,$02,$E4,$00
            .byte $04,$E0,$02,$26,$60,$00,$00,$40
            .byte $A0,$03,$20,$62,$06,$00,$00,$00
            .byte $AA,$E4,$E9,$C5,$00,$87,$48,$48
            .byte $48,$00,$40,$C0,$00,$80,$00,$81
            .byte $00,$80,$05,$00,$00,$00,$02,$05
            .byte $0A,$81,$00,$01,$00,$01,$02,$05
            .byte $AA,$1D,$3A,$1D,$2A,$14,$48,$40
            .byte $41,$40,$80,$40,$80,$00,$02,$02
            .byte $82,$7B,$01,$7B,$71,$2B,$59,$3B
            .byte $01,$1E,$80,$00,$80,$00,$80,$00
            .byte $80,$00,$2D,$5D,$6D,$75,$00,$55
            .byte $FF,$00,$E0,$0A,$17,$2F,$1F,$2E
            .byte $95,$AA,$00,$00,$41,$01,$01,$01
            .byte $40,$AA,$15,$20,$00,$00,$00,$E0
            .byte $00,$00,$07,$A8,$F4,$F8,$54,$A8
            .byte $51,$00,$B4,$BA,$B6,$AE,$00,$AA
            .byte $FF,$A9,$40,$00,$00,$00,$00,$07
            .byte $00,$55,$00,$00,$80,$80,$80,$80
            .byte $00,$00,$10,$00,$00,$00,$20,$08
            .byte $00,$00,$80,$04,$00,$80,$00,$02
            .byte $88,$00,$00,$20,$00,$00,$41,$00
            .byte $04,$00,$00,$08,$00,$02,$00,$00
            .byte $04,$00,$10,$00,$20,$00,$00,$08
            .byte $00,$00,$00,$04,$00,$90,$00,$02
            .byte $08,$00,$00,$21,$00,$00,$41,$00
            .byte $00,$00,$00,$08,$00,$00,$10,$00
            .byte $04,$00,$00,$00,$20,$00,$20,$08
            .byte $00,$00,$80,$04,$00,$10,$00,$02
            .byte $80,$00,$00,$20,$00,$00,$41,$00
            .byte $00,$00,$00,$08,$00,$02,$10,$00
            .byte $04,$00,$10,$00,$00,$00,$20,$00
            .byte $00,$00,$80,$04,$00,$90,$00,$00
            .byte $88,$00,$00,$21,$00,$00,$40,$00
            .byte $04,$00,$00,$08,$00,$02,$10,$00
            .byte $04,$05,$8A,$00,$00,$8A,$4D,$8A
            .byte $45,$08,$10,$00,$00,$00,$00,$01
            .byte $10,$8A,$00,$20,$01,$00,$82,$44
            .byte $2A,$21,$50,$A0,$40,$00,$00,$01
            .byte $82,$41,$88,$66,$44,$10,$2C,$08
            .byte $61,$41,$A8,$70,$28,$50,$00,$00
            .byte $01,$4A,$00,$80,$05,$02,$08,$00
            .byte $2A,$12,$00,$80,$42,$80,$00,$03
            .byte $0A,$54,$8A,$40,$A0,$11,$B8,$7D
            .byte $38,$54,$28,$00,$82,$F3,$E2,$51
            .byte $A2,$54,$28,$00,$80,$00,$6B,$42
            .byte $88,$00,$0C,$4A,$14,$00,$20,$40
            .byte $A8,$01,$82,$41,$94,$3A,$7C,$3A
            .byte $54,$D4,$88,$40,$00,$01,$14,$3A
            .byte $7C,$28,$80,$00,$A0,$04,$AD,$5C
            .byte $89,$3A,$54,$28,$00,$81,$A8,$74
            .byte $38,$44,$B1,$23,$87,$13,$65,$42
            .byte $88,$15,$42,$A0,$C5,$AE,$47,$0A
            .byte $A5,$41,$94,$39,$7C,$3A,$54,$29
            .byte $83,$D0,$E0,$51,$A2,$00,$0A,$5D
            .byte $8A,$14,$BA,$7C,$3A,$90,$05,$8E
            .byte $05,$51,$26,$04,$10,$3A,$34,$8A
            .byte $10,$0A,$01,$28,$74,$28,$54,$28
            .byte $00,$8A,$10,$0A,$1D,$0E,$95,$4A
            .byte $A0,$41,$94,$3A,$74,$02,$28,$74
            .byte $38,$51,$2C,$49,$22,$15,$02,$11
            .byte $68,$54,$28,$02,$07,$13,$25,$02
            .byte $28,$50,$28,$90,$40,$81,$40,$80
            .byte $14,$54,$81,$29,$70,$28,$50,$00
            .byte $80,$08,$44,$A0,$40,$A0,$40,$01
            .byte $02,$42,$85,$52,$E1,$50,$A0,$01
            .byte $02,$41,$22,$01,$02,$05,$02,$08
            .byte $10,$55,$80,$14,$B8,$74,$29,$50
            .byte $01,$55,$8A,$65,$40,$89,$D0,$A8
            .byte $50,$00,$03,$A2,$D0,$A1,$50,$A1
            .byte $00,$A0,$00,$01,$80,$D0,$A8,$50
            .byte $A0,$55,$8A,$50,$AA,$5C,$8A,$54
            .byte $80,$44,$B3,$22,$04,$38,$14,$28
            .byte $00,$30,$A0,$41,$A8,$70,$28,$50
            .byte $01,$C0,$80,$11,$A2,$41,$02,$00
            .byte $00,$4E,$81,$14,$B8,$74,$28,$50
            .byte $00,$94,$08,$14,$08,$05,$36,$25
            .byte $02,$00,$80,$41,$B0,$25,$88,$44
            .byte $AA,$51,$E8,$F0,$E8,$50,$A0,$01
            .byte $00,$20,$0A,$1D,$BE,$1C,$AB,$12
            .byte $00,$28,$00,$28,$74,$F8,$74,$28
            .byte $50,$0A,$9D,$3E,$9D,$2A,$15,$0A
            .byte $00,$00,$06,$94,$10,$85,$1A,$11
            .byte $22,$00,$7D,$6A,$45,$20,$05,$B7
            .byte $B7,$00,$7E,$FA,$F2,$2E,$1E,$9E
            .byte $0E,$B5,$01,$03,$18,$6B,$43,$60
            .byte $05,$8A,$46,$8E,$0E,$9A,$92,$0E
            .byte $40,$00,$7D,$6A,$45,$6E,$58,$79
            .byte $73,$00,$7E,$FA,$F2,$0C,$A1,$EC
            .byte $4D,$76,$73,$66,$50,$6B,$43,$60
            .byte $05,$AC,$41,$80,$1C,$9A,$92,$0E
            .byte $40,$03,$70,$6B,$43,$60,$57,$73
            .byte $77,$80,$0E,$9A,$92,$0C,$81,$2C
            .byte $ED,$72,$70,$68,$5C,$6A,$45,$6F
            .byte $00,$AC,$41,$00,$0C,$FA,$F2,$FE
            .byte $00,$03,$78,$6B,$43,$28,$03,$B7
            .byte $B6,$80,$0E,$9A,$92,$0E,$8E,$4E
            .byte $8E,$B5,$02,$00,$10,$6A,$45,$6F
            .byte $00,$0A,$16,$0E,$7E,$FA,$F2,$FE
            .byte $00,$00,$7D,$6B,$42,$0B,$40,$9D
            .byte $DD,$00,$BE,$5A,$52,$C8,$01,$3A
            .byte $BB,$9C,$41,$00,$1C,$6B,$4A,$6D
            .byte $00,$BA,$01,$00,$38,$5A,$52,$BE
            .byte $00,$03,$75,$60,$43,$6B,$43,$38
            .byte $69,$80,$4E,$0A,$90,$9E,$92,$0C
            .byte $8A,$4B,$38,$43,$73,$6B,$40,$73
            .byte $05,$42,$0C,$92,$9E,$9A,$02,$8E
            .byte $40,$03,$75,$60,$4B,$10,$45,$93
            .byte $D7,$80,$4E,$0A,$92,$08,$81,$2A
            .byte $EB,$92,$45,$00,$00,$6E,$4B,$7D
            .byte $00,$AA,$41,$00,$08,$BA,$52,$FE
            .byte $00,$00,$7D,$6B,$4E,$00,$45,$97
            .byte $D5,$00,$FE,$5A,$B2,$08,$41,$EA
            .byte $AB,$93,$41,$03,$10,$6B,$40,$73
            .byte $05,$4A,$01,$80,$08,$9A,$02,$8E
            .byte $40,$03,$75,$60,$43,$70,$53,$36
            .byte $63,$80,$4E,$0A,$92,$08,$01,$AA
            .byte $EB,$57,$62,$57,$70,$6B,$40,$73
            .byte $05,$2A,$81,$00,$08,$9A,$02,$8E
            .byte $40,$03,$75,$60,$4B,$10,$45,$93
            .byte $D7,$80,$4E,$0A,$82,$0E,$8A,$4C
            .byte $86,$95,$41,$03,$10,$6B,$40,$73
            .byte $05,$4A,$86,$4A,$0E,$9A,$02,$8E
            .byte $40,$03,$75,$60,$4B,$10,$45,$93
            .byte $D7,$80,$4E,$0A,$92,$08,$81,$2A
            .byte $EB,$95,$41,$03,$10,$6B,$40,$73
            .byte $05,$AA,$01,$80,$08,$9A,$02,$8E
            .byte $40,$80,$1F,$3F,$75,$6A,$70,$6B
            .byte $73,$00,$F8,$FC,$54,$A8,$04,$F8
            .byte $54,$6A,$73,$6A,$73,$6A,$35,$00
            .byte $00,$A8,$40,$80,$01,$8F,$0F,$09
            .byte $1F,$00,$3F,$7F,$75,$6A,$70,$6F
            .byte $75,$03,$F9,$FC,$50,$A8,$10,$E8
            .byte $30,$6A,$21,$02,$87,$F6,$77,$D4
            .byte $F8,$A8,$30,$A8,$30,$A8,$50,$00
            .byte $00,$80,$3F,$7F,$75,$6A,$73,$6A
            .byte $73,$1D,$CE,$8B,$0F,$87,$03,$8F
            .byte $0E,$6A,$73,$6A,$73,$6A,$35,$00
            .byte $80,$8F,$0F,$8B,$0F,$87,$0F,$0F
            .byte $1E,$B8,$B3,$F7,$F7,$E6,$E7,$B6
            .byte $F7,$01,$F8,$FC,$54,$A8,$30,$A8
            .byte $30,$F6,$B7,$D6,$F7,$F6,$D3,$B0
            .byte $D8,$A8,$30,$A8,$30,$A8,$50,$00
            .byte $01,$00,$3F,$7F,$75,$6A,$73,$6A
            .byte $73,$1D,$8F,$CB,$0F,$81,$1E,$BC
            .byte $54,$6A,$72,$6B,$75,$6A,$20,$80
            .byte $C0,$A8,$04,$F8,$54,$A8,$00,$00
            .byte $00,$B8,$F3,$D7,$77,$86,$3F,$7E
            .byte $75,$00,$F8,$FC,$50,$A8,$30,$A8
            .byte $30,$6A,$70,$6F,$75,$6A,$20,$00
            .byte $00,$A8,$30,$E8,$50,$A0,$00,$01
            .byte $03,$80,$3F,$7F,$75,$6A,$70,$6F
            .byte $75,$01,$F8,$FC,$54,$A8,$04,$F8
            .byte $54,$6A,$30,$00,$80,$FF,$D9,$BF
            .byte $FE,$A8,$00,$00,$01,$B7,$BB,$F7
            .byte $FE,$FB,$F9,$FE,$FF,$80,$3F,$7F
            .byte $75,$FD,$DE,$FB,$FF,$01,$F8,$FC
            .byte $54,$6A,$70,$6F,$75,$6A,$30,$00
            .byte $80,$A8,$04,$F8,$54,$A8,$00,$00
            .byte $01,$DB,$F9,$7E,$FF,$E8,$F3,$B7
            .byte $F7,$FD,$DE,$FB,$FF,$01,$F8,$FC
            .byte $54,$F6,$B7,$F6,$F7,$F6,$D3,$90
            .byte $F8,$08,$5C,$28,$7C,$A8,$00,$00
            .byte $01,$FF,$FD,$FF,$FF,$80,$3F,$7F
            .byte $75,$DD,$EE,$BB,$FF,$1F,$8F,$CF
            .byte $4E,$60,$75,$62,$77,$6A,$30,$00
            .byte $80,$8B,$CF,$8F,$CF,$87,$0F,$0D
            .byte $1E,$80,$3F,$7F,$75,$60,$75,$62
            .byte $77,$1D,$8E,$CB,$4F,$8B,$CF,$8F
            .byte $CE,$6A,$30,$00,$80,$FF,$DF,$BE
            .byte $FF,$8F,$0D,$0E,$1F,$B7,$BF,$FD
            .byte $FE,$F8,$F3,$B7,$F7,$E6,$F7,$B6
            .byte $F7,$01,$F8,$FC,$54,$08,$5C,$28
            .byte $7C,$F6,$F3,$90,$F8,$FF,$DF,$BC
            .byte $FF,$A8,$00,$00,$01,$B7,$B7,$E7
            .byte $FE,$5A,$00,$7F,$3A,$5D,$3A,$55
            .byte $0A,$80,$00,$C0,$80,$00,$80,$00
            .byte $00,$40,$60,$68,$5D,$6A,$45,$6F
            .byte $00,$02,$06,$0E,$7E,$FA,$F2,$FE
            .byte $00,$00,$7F,$6A,$44,$6D,$5A,$29
            .byte $5A,$00,$2A,$53,$AB,$7B,$FB,$FA
            .byte $FB,$39,$72,$79,$74,$6C,$4E,$7F
            .byte $00,$FA,$EB,$50,$A0,$00,$00,$00
            .byte $00,$00,$7D,$6A,$45,$6F,$58,$65
            .byte $4A,$00,$7E,$FA,$F2,$AE,$1E,$46
            .byte $A2,$17,$2F,$5F,$2F,$5F,$00,$7F
            .byte $3D,$D0,$E0,$D0,$A0,$C0,$00,$C0
            .byte $40,$00,$AA,$34,$BE,$BD,$2A,$B5
            .byte $2A,$00,$FE,$6A,$10,$3A,$16,$0A
            .byte $16,$B4,$20,$00,$00,$00,$00,$00
            .byte $00,$0E,$1E,$1E,$1E,$3A,$72,$FE
            .byte $00,$AF,$7C,$F3,$6C,$D0,$D3,$A4
            .byte $A3,$EA,$3D,$CB,$31,$08,$C9,$04
            .byte $C4,$A0,$A4,$D3,$50,$EC,$43,$A0
            .byte $74,$24,$24,$C8,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D4,$A6
            .byte $A5,$EA,$3D,$CB,$31,$08,$29,$64
            .byte $A4,$A5,$A4,$D4,$50,$EC,$43,$A0
            .byte $74,$A4,$24,$28,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D7,$A4
            .byte $A7,$EA,$3D,$CB,$31,$08,$C9,$24
            .byte $C4,$A4,$A4,$D7,$50,$EC,$43,$A0
            .byte $74,$24,$24,$C8,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D3,$A0
            .byte $A0,$EA,$3D,$CB,$31,$08,$89,$44
            .byte $44,$A1,$A2,$D3,$50,$EC,$43,$A0
            .byte $74,$84,$04,$C8,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D3,$A0
            .byte $A0,$EA,$3D,$CB,$31,$08,$C9,$24
            .byte $24,$A3,$A0,$D0,$53,$EC,$43,$A0
            .byte $74,$C4,$24,$28,$C9,$30,$C0,$02
            .byte $11,$FB,$F9,$FE,$FF,$EF,$FF,$BF
            .byte $FF,$FD,$DE,$FB,$FF,$EF,$FF,$7F
            .byte $FE,$FB,$FF,$FB,$FF,$FF,$DF,$BF
            .byte $FF,$FF,$DF,$FF,$FF,$B7,$BF,$FF
            .byte $FE,$00,$7D,$6A,$45,$6F,$5F,$7E
            .byte $7D,$00,$7E,$F8,$F2,$A4,$4A,$94
            .byte $2A,$7B,$76,$6C,$58,$60,$40,$45
            .byte $00,$40,$02,$04,$2A,$50,$A2,$54
            .byte $00,$00,$28,$40,$00,$45,$0A,$54
            .byte $28,$00,$2A,$50,$A2,$04,$0A,$14
            .byte $2A,$51,$22,$44,$08,$40,$00,$45
            .byte $00,$40,$02,$04,$2A,$50,$A2,$54
            .byte $00,$00,$7D,$6A,$05,$4F,$0F,$56
            .byte $29,$00,$7E,$FA,$F2,$AE,$5E,$BE
            .byte $7E,$51,$22,$44,$08,$40,$00,$45
            .byte $00,$EA,$56,$2E,$3E,$5A,$A2,$56
            .byte $00,$00,$68,$60,$40,$6D,$5E,$7E
            .byte $7D,$00,$2A,$50,$A2,$04,$0A,$14
            .byte $2A,$7B,$77,$6E,$5D,$6A,$45,$6F
            .byte $00,$C0,$42,$A4,$7A,$F8,$F2,$FE
            .byte $00,$00,$7D,$6A,$45,$6F,$5F,$7E
            .byte $7D,$00,$7E,$FA,$F2,$AE,$5E,$BE
            .byte $7E,$7B,$77,$6E,$5D,$6A,$45,$6F
            .byte $00,$EA,$56,$AE,$7E,$FA,$F2,$FE
            .byte $00,$00,$28,$40,$00,$45,$0A,$54
            .byte $28,$00,$2A,$52,$A2,$0E,$1E,$3E
            .byte $7E,$51,$23,$46,$0D,$4A,$05,$6F
            .byte $00,$EA,$56,$AE,$7E,$FA,$F2,$FE
            .byte $00,$AF,$7C,$F3,$6C,$D1,$D1,$A1
            .byte $AE,$EA,$3D,$CB,$31,$88,$09,$84
            .byte $74,$AA,$A1,$D1,$51,$EC,$43,$A0
            .byte $74,$54,$84,$08,$89,$30,$C0,$02
            .byte $11,$00,$44,$00,$50,$60,$40,$02
            .byte $40,$00,$20,$50,$0E,$16,$10,$86
            .byte $0E,$00,$48,$70,$22,$0A,$17,$19
            .byte $00,$02,$04,$0A,$04,$46,$9E,$FE
            .byte $00,$00,$18,$07,$5F,$5F,$3F,$3F
            .byte $3F,$00,$1D,$44,$A3,$D2,$A1,$D0
            .byte $A1,$1F,$2A,$15,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$AF,$7C,$F3,$6C,$D2,$D7,$AF
            .byte $A7,$FA,$39,$CB,$31,$88,$A9,$C4
            .byte $84,$AF,$A2,$D4,$D0,$EC,$03,$A0
            .byte $74,$44,$84,$08,$09,$30,$C0,$02
            .byte $11,$00,$18,$00,$41,$47,$0F,$0F
            .byte $1F,$00,$1D,$04,$A3,$D2,$A1,$D0
            .byte $A1,$1F,$0A,$15,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$00,$40,$40,$03,$07
            .byte $07,$00,$1D,$04,$03,$52,$A1,$D0
            .byte $A1,$07,$0A,$05,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$00,$40,$40,$00,$01
            .byte $03,$00,$1D,$04,$03,$02,$21,$50
            .byte $A1,$01,$02,$05,$42,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$00,$40,$40,$00,$00
            .byte $00,$00,$1D,$04,$03,$02,$01,$00
            .byte $01,$00,$00,$00,$40,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$00,$40,$40,$00,$00
            .byte $00,$00,$1D,$04,$03,$02,$01,$00
            .byte $01,$00,$00,$00,$40,$40,$60,$18
            .byte $55,$00,$01,$00,$03,$02,$07,$1E
            .byte $55,$00,$18,$00,$40,$40,$00,$00
            .byte $00,$00,$1D,$04,$03,$02,$01,$00
            .byte $01,$00,$00,$00,$40,$40,$60,$18
            .byte $55,$00,$21,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$00,$40,$40,$00,$00
            .byte $00,$00,$1D,$04,$03,$02,$01,$00
            .byte $01,$00,$00,$00,$40,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$00,$40,$40,$00,$00
            .byte $00,$00,$1D,$04,$03,$02,$01,$10
            .byte $41,$00,$01,$00,$42,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$00,$40,$40,$00,$01
            .byte $02,$00,$1D,$04,$03,$02,$21,$90
            .byte $41,$02,$01,$04,$42,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$00,$40,$40,$02,$05
            .byte $02,$00,$1D,$04,$03,$D2,$21,$90
            .byte $41,$0A,$09,$04,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$00,$41,$47,$0E,$0D
            .byte $1A,$00,$1D,$04,$A3,$D2,$21,$90
            .byte $41,$1A,$09,$14,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$07,$5F,$5F,$3E,$3D
            .byte $3A,$00,$1D,$44,$A3,$D2,$21,$90
            .byte $41,$1A,$29,$14,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$AF,$7C,$F3,$6C,$D2,$D6,$AE
            .byte $A0,$FA,$39,$CB,$31,$88,$A9,$C4
            .byte $04,$AE,$A2,$D4,$D0,$EC,$03,$A0
            .byte $74,$44,$84,$08,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D2,$D6,$AE
            .byte $A0,$FA,$39,$CB,$31,$08,$29,$44
            .byte $04,$A0,$A2,$D4,$D0,$EC,$03,$A0
            .byte $74,$04,$04,$08,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D4,$A0
            .byte $A0,$FA,$39,$CB,$31,$08,$29,$04
            .byte $04,$A0,$A0,$D4,$D0,$EC,$03,$A0
            .byte $74,$04,$04,$08,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D0,$A0
            .byte $A0,$EA,$3D,$CB,$31,$08,$09,$04
            .byte $04,$A0,$A0,$D0,$50,$EC,$43,$A0
            .byte $74,$04,$04,$08,$09,$30,$C0,$02
            .byte $11,$03,$54,$2D,$7D,$79,$71,$3D
            .byte $7D,$00,$00,$80,$10,$A0,$00,$A0
            .byte $30,$3D,$7D,$79,$71,$2D,$5E,$3F
            .byte $00,$B0,$30,$A0,$00,$A0,$50,$F0
            .byte $00,$03,$54,$2D,$7D,$79,$72,$3F
            .byte $7F,$00,$00,$80,$10,$A0,$40,$E0
            .byte $70,$3E,$7F,$7B,$73,$2F,$5F,$3F
            .byte $00,$FE,$FC,$EA,$C4,$EE,$DE,$FE
            .byte $00,$03,$54,$2D,$7D,$79,$71,$3D
            .byte $7D,$00,$00,$80,$10,$A0,$00,$A0
            .byte $30,$3E,$7F,$7B,$73,$2F,$5F,$3F
            .byte $00,$70,$F0,$E0,$C4,$EE,$DE,$FE
            .byte $00,$03,$54,$2D,$7D,$79,$71,$3D
            .byte $7D,$00,$00,$80,$10,$A0,$00,$A0
            .byte $30,$3D,$7D,$7A,$73,$2F,$5F,$3F
            .byte $00,$B0,$30,$60,$C0,$E0,$DE,$FE
            .byte $00,$00,$5D,$3F,$5F,$3B,$73,$80
            .byte $BF,$00,$74,$FA,$F6,$EE,$CE,$06
            .byte $FA,$2A,$00,$0B,$11,$00,$00,$00
            .byte $00,$AA,$06,$EA,$C6,$00,$00,$00
            .byte $00,$00,$5D,$3F,$5F,$3B,$73,$87
            .byte $BB,$00,$74,$FA,$F6,$EE,$CE,$FE
            .byte $7E,$2A,$07,$0B,$11,$00,$00,$00
            .byte $00,$FE,$FE,$EA,$C6,$EE,$DE,$AE
            .byte $00,$00,$5D,$3F,$5F,$3B,$73,$80
            .byte $BF,$00,$74,$FA,$F6,$EE,$CE,$FE
            .byte $7E,$2A,$00,$0B,$11,$00,$00,$00
            .byte $00,$7E,$FE,$EA,$C6,$0E,$1E,$0E
            .byte $00,$00,$5D,$3F,$5F,$3B,$73,$80
            .byte $BF,$00,$74,$FA,$F6,$EE,$CE,$3E
            .byte $DE,$2A,$00,$0B,$11,$00,$00,$00
            .byte $00,$9E,$3E,$EA,$C6,$06,$06,$06
            .byte $00,$00,$2E,$5F,$6F,$77,$73,$20
            .byte $5F,$00,$BA,$FC,$FA,$DC,$CE,$01
            .byte $FD,$55,$60,$57,$63,$76,$78,$74
            .byte $00,$54,$00,$DE,$8E,$00,$00,$00
            .byte $00,$00,$2E,$5F,$6F,$77,$73,$3F
            .byte $5E,$00,$BA,$FC,$FA,$DC,$CE,$E1
            .byte $DD,$7F,$7F,$57,$63,$77,$7B,$75
            .byte $00,$54,$E0,$DC,$FE,$FE,$EC,$DE
            .byte $00,$00,$2E,$5F,$6F,$77,$73,$3F
            .byte $5E,$00,$BA,$FC,$FA,$DC,$CE,$01
            .byte $FD,$7E,$7F,$57,$63,$77,$7B,$75
            .byte $00,$54,$00,$BE,$DE,$F0,$E0,$F0
            .byte $00,$00,$2E,$5F,$6F,$77,$73,$3C
            .byte $5B,$00,$BA,$FC,$FA,$DC,$CE,$01
            .byte $FD,$79,$7C,$57,$62,$77,$7B,$75
            .byte $00,$54,$00,$DE,$CE,$C0,$80,$C0
            .byte $00,$AF,$7C,$F3,$6C,$D0,$D0,$A0
            .byte $A0,$EA,$3D,$CB,$31,$88,$89,$84
            .byte $84,$A0,$A0,$D0,$50,$EC,$43,$A0
            .byte $74,$04,$04,$08,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D0,$A0
            .byte $A0,$EA,$3D,$CB,$31,$08,$29,$44
            .byte $84,$A0,$A0,$D0,$50,$EC,$43,$A0
            .byte $74,$04,$04,$08,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D0,$A0
            .byte $A0,$EA,$3D,$CB,$31,$08,$09,$04
            .byte $04,$A0,$A0,$D0,$50,$EC,$43,$A0
            .byte $74,$F4,$04,$08,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D0,$A0
            .byte $A0,$EA,$3D,$CB,$31,$08,$09,$04
            .byte $04,$A0,$A0,$D0,$50,$EC,$43,$A0
            .byte $74,$84,$44,$28,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D0,$A0
            .byte $A0,$EA,$3D,$CB,$31,$08,$09,$04
            .byte $04,$A1,$A1,$D3,$51,$EC,$43,$A0
            .byte $74,$04,$04,$88,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D0,$A0
            .byte $A0,$EA,$3D,$CB,$31,$08,$09,$04
            .byte $04,$A1,$A2,$D5,$51,$EC,$43,$A0
            .byte $74,$04,$04,$88,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D0,$A0
            .byte $AF,$EA,$3D,$CB,$31,$08,$09,$04
            .byte $04,$A0,$A0,$D1,$51,$EC,$43,$A0
            .byte $74,$04,$04,$08,$09,$30,$C0,$02
            .byte $11,$AF,$7C,$F3,$6C,$D0,$D4,$A2
            .byte $A1,$EA,$3D,$CB,$31,$08,$09,$04
            .byte $04,$A0,$A0,$D1,$50,$EC,$43,$A0
            .byte $74,$04,$04,$08,$09,$30,$C0,$02
            .byte $11,$00,$18,$06,$5D,$5A,$3A,$3D
            .byte $3E,$00,$1D,$44,$A3,$52,$41,$90
            .byte $21,$1F,$2A,$15,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$07,$5F,$5F,$3E,$3E
            .byte $3F,$00,$1D,$44,$83,$62,$91,$90
            .byte $61,$1F,$2A,$15,$4A,$40,$60,$18
            .byte $55,$00,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$07,$5F,$5F,$3F,$3F
            .byte $3F,$00,$1D,$44,$A3,$D2,$81,$B0
            .byte $49,$1F,$2A,$15,$4A,$40,$60,$18
            .byte $55,$48,$B1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$07,$5F,$5F,$3F,$3F
            .byte $3F,$00,$1D,$44,$A3,$D2,$A1,$D0
            .byte $81,$1F,$2A,$14,$4A,$40,$60,$18
            .byte $55,$60,$91,$90,$63,$02,$07,$1E
            .byte $55,$00,$18,$07,$5F,$5F,$3F,$3F
            .byte $3F,$00,$1D,$44,$A3,$D2,$A1,$D0
            .byte $A1,$1E,$29,$12,$4A,$41,$60,$18
            .byte $55,$40,$A1,$40,$43,$82,$07,$1E
            .byte $55,$00,$18,$07,$5F,$5F,$3F,$3F
            .byte $39,$00,$1D,$44,$A3,$D2,$A1,$D0
            .byte $A1,$16,$29,$09,$46,$40,$60,$18
            .byte $55,$40,$21,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$07,$5F,$5F,$33,$2D
            .byte $12,$00,$1D,$44,$A3,$D2,$A1,$D0
            .byte $A1,$12,$2C,$11,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$07,$59,$56,$29,$29
            .byte $36,$00,$1D,$44,$A3,$D2,$21,$50
            .byte $A1,$19,$2A,$15,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$50,$27,$49,$18,$37,$26
            .byte $15,$00,$0A,$40,$02,$E0,$70,$60
            .byte $30,$18,$1A,$08,$40,$0E,$45,$28
            .byte $00,$28,$10,$E8,$C0,$A2,$00,$0A
            .byte $00,$00,$28,$61,$1D,$59,$15,$21
            .byte $2A,$00,$14,$C2,$E0,$82,$70,$64
            .byte $50,$07,$35,$38,$3E,$14,$40,$20
            .byte $00,$8C,$28,$00,$38,$70,$22,$02
            .byte $00,$00,$27,$47,$3B,$32,$29,$05
            .byte $69,$00,$02,$9C,$3E,$3C,$DE,$94
            .byte $40,$5C,$39,$15,$21,$3E,$54,$00
            .byte $00,$38,$DC,$98,$40,$30,$62,$54
            .byte $00,$00,$28,$60,$1D,$59,$13,$27
            .byte $1F,$00,$14,$C2,$60,$02,$B0,$C4
            .byte $F0,$07,$33,$39,$3D,$54,$00,$50
            .byte $00,$CC,$A8,$00,$38,$70,$22,$02
            .byte $00,$00,$27,$47,$3B,$32,$29,$05
            .byte $69,$00,$02,$9C,$3E,$3C,$DE,$94
            .byte $40,$5C,$39,$15,$21,$3E,$14,$40
            .byte $00,$38,$DC,$98,$40,$30,$62,$54
            .byte $00,$00,$28,$61,$1D,$59,$15,$21
            .byte $2A,$00,$14,$C2,$E0,$82,$70,$64
            .byte $50,$07,$35,$38,$3E,$14,$40,$20
            .byte $00,$8C,$28,$00,$38,$70,$22,$02
            .byte $00,$00,$28,$67,$09,$58,$37,$26
            .byte $15,$00,$14,$42,$00,$E2,$70,$60
            .byte $30,$18,$1A,$08,$40,$0E,$45,$28
            .byte $00,$28,$10,$E8,$C2,$A0,$02,$14
            .byte $00,$00,$28,$65,$08,$50,$10,$00
            .byte $00,$00,$14,$02,$20,$02,$00,$10
            .byte $00,$00,$20,$00,$48,$00,$40,$28
            .byte $00,$00,$00,$00,$02,$00,$02,$24
            .byte $00,$00,$18,$07,$5F,$5F,$3F,$3F
            .byte $3F,$00,$1D,$44,$A2,$CE,$A8,$CE
            .byte $A2,$1F,$2A,$15,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$18,$07,$4F,$77,$17,$77
            .byte $4F,$00,$1D,$44,$A3,$D2,$A1,$D0
            .byte $A1,$1F,$2A,$15,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55,$00,$3D,$7E,$C3,$AD,$9D,$7E
            .byte $7C,$00,$7E,$FA,$72,$2E,$81,$81
            .byte $C3,$7A,$76,$2C,$1C,$00,$00,$01
            .byte $00,$7E,$3C,$00,$7E,$7A,$F2,$FE
            .byte $00,$00,$DD,$EA,$35,$57,$58,$68
            .byte $6C,$00,$7B,$F7,$EC,$AA,$1A,$16
            .byte $36,$67,$63,$40,$45,$02,$05,$0F
            .byte $00,$E2,$C6,$02,$62,$E0,$F0,$F8
            .byte $00,$00,$7D,$6A,$44,$6E,$81,$81
            .byte $C3,$00,$3C,$7E,$C3,$AD,$9D,$3E
            .byte $7E,$7E,$3C,$00,$5C,$6A,$45,$6F
            .byte $00,$6A,$56,$2C,$3C,$00,$00,$80
            .byte $00,$00,$7B,$67,$4C,$6B,$1B,$16
            .byte $35,$00,$DE,$EA,$32,$96,$58,$A8
            .byte $6C,$E3,$C7,$02,$41,$60,$40,$68
            .byte $00,$E7,$43,$80,$46,$02,$02,$1E
            .byte $00,$00,$FF,$60,$67,$FF,$00,$05
            .byte $05,$00,$FF,$07,$FE,$FF,$00,$C0
            .byte $C0,$00,$FF,$60,$67,$FF,$00,$05
            .byte $05,$00,$FF,$07,$FE,$FF,$00,$C0
            .byte $C0,$FF,$60,$67,$FF,$00,$05,$05
            .byte $00,$FF,$07,$FE,$FF,$00,$C0,$C0
            .byte $00,$FF,$60,$67,$FF,$00,$05,$05
            .byte $00,$FF,$07,$FE,$FF,$00,$C0,$C0
            .byte $00,$60,$67,$FF,$00,$05,$05,$00
            .byte $FF,$07,$FE,$FF,$00,$C0,$C0,$00
            .byte $FF,$60,$67,$FF,$00,$05,$05,$00
            .byte $FF,$07,$FE,$FF,$00,$C0,$C0,$00
            .byte $FF,$67,$FF,$00,$05,$05,$00,$FF
            .byte $60,$FE,$FF,$00,$C0,$C0,$00,$FF
            .byte $07,$67,$FF,$00,$05,$05,$00,$FF
            .byte $60,$FE,$FF,$00,$C0,$C0,$00,$FF
            .byte $07,$FF,$00,$05,$05,$00,$FF,$60
            .byte $67,$FF,$00,$C0,$C0,$00,$FF,$07
            .byte $FE,$FF,$00,$05,$05,$00,$FF,$60
            .byte $67,$FF,$00,$C0,$C0,$00,$FF,$07
            .byte $FE,$00,$05,$05,$00,$FF,$60,$67
            .byte $FF,$00,$C0,$C0,$00,$FF,$07,$FE
            .byte $FF,$00,$05,$05,$00,$FF,$60,$67
            .byte $FF,$00,$C0,$C0,$00,$FF,$07,$FE
            .byte $FF,$05,$05,$00,$FF,$60,$67,$FF
            .byte $00,$C0,$C0,$00,$FF,$07,$FE,$FF
            .byte $00,$05,$05,$00,$FF,$60,$67,$FF
            .byte $00,$C0,$C0,$00,$FF,$07,$FE,$FF
            .byte $00,$00,$55,$20,$45,$AA,$00,$02
            .byte $05,$00,$55,$02,$54,$AA,$00,$80
            .byte $40,$00,$55,$20,$45,$AA,$00,$02
            .byte $05,$00,$55,$02,$54,$AA,$00,$80
            .byte $40,$55,$20,$45,$AA,$00,$02,$05
            .byte $00,$55,$02,$54,$AA,$00,$80,$40
            .byte $00,$55,$20,$45,$AA,$00,$02,$05
            .byte $00,$55,$02,$54,$AA,$00,$80,$40
            .byte $00,$20,$45,$AA,$00,$02,$05,$00
            .byte $55,$02,$54,$AA,$00,$80,$40,$00
            .byte $55,$20,$45,$AA,$00,$02,$05,$00
            .byte $55,$02,$54,$AA,$00,$80,$40,$00
            .byte $55,$45,$AA,$00,$02,$05,$00,$55
            .byte $20,$54,$AA,$00,$80,$40,$00,$55
            .byte $02,$45,$AA,$00,$02,$05,$00,$55
            .byte $20,$54,$AA,$00,$80,$40,$00,$55
            .byte $02,$AA,$00,$02,$05,$00,$55,$20
            .byte $45,$AA,$00,$80,$40,$00,$55,$02
            .byte $54,$AA,$00,$02,$05,$00,$55,$20
            .byte $45,$AA,$00,$80,$40,$00,$55,$02
            .byte $54,$00,$02,$05,$00,$55,$20,$45
            .byte $AA,$00,$80,$40,$00,$55,$02,$54
            .byte $AA,$00,$02,$05,$00,$55,$20,$45
            .byte $AA,$00,$80,$40,$00,$55,$02,$54
            .byte $AA,$02,$05,$00,$55,$20,$45,$AA
            .byte $00,$80,$40,$00,$55,$02,$54,$AA
            .byte $00,$02,$05,$00,$55,$20,$45,$AA
            .byte $00,$80,$40,$00,$55,$02,$54,$AA
            .byte $00,$5E,$BD,$7E,$FC,$F3,$F4,$A8
            .byte $49,$A0,$5A,$BD,$3F,$CF,$25,$12
            .byte $95,$A9,$48,$B4,$73,$78,$3D,$5A
            .byte $3D,$92,$14,$20,$C0,$00,$40,$80
            .byte $00,$00,$18,$07,$4F,$77,$17,$77
            .byte $4F,$00,$1D,$44,$A2,$CE,$A8,$CE
            .byte $A2,$1F,$2A,$15,$4A,$40,$60,$18
            .byte $55,$40,$A1,$40,$03,$02,$07,$1E
            .byte $55

	.repeat $5800-*
	  .byte 0
	.endrep

