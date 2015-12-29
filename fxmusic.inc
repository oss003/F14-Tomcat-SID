;
; SID Music
;
; Load: $1000
; Init: $1000
; Play: $1003
;
; FX1 : A=$3E, Y=$1D, JSR K1006
; FX2 : A=$61, Y=$1D, JSR K1006
; FX3 : A=$80, Y=$1D, JSR K1006
; FX4 : A=$A9, Y=$1D, JSR K1006

L00FC       = $00FC
L00FD       = $00FD

LBDC0       = $BDC0
LBDC1       = $BDC1
LBDC2       = $BDC2
LBDC3       = $BDC3
LBDC4       = $BDC4
LBDC5       = $BDC5
LBDC6       = $BDC6
LBDD5       = $BDD5
LBDD7       = $BDD7
LBDD8       = $BDD8

;
; Start of code
;
;            *= $1000
;

SIDinit:    jmp K10A4              ; 4C A4 10
SIDplay:    jmp K10D3              ; 4C D3 10
K1006:      jmp K10A8              ; 4C A8 10
K1009:      jmp K1013              ; 4C 13 10
K100C:
            tay                    ; A8
            lda #$00               ; A9 00
            sta K13C5,X            ; 9D C5 13
            tya                    ; 98
K1013:      sta K139C,X            ; 9D 9C 13
            lda K138B,X            ; BD 8B 13
            sta K139B,X            ; 9D 9B 13
            rts                    ; 60
K101D:
            sta K13DB,X            ; 9D DB 13
            rts                    ; 60
K1021:
            sta K13B2              ; 8D B2 13
            sta K13B9              ; 8D B9 13
            sta K13C0              ; 8D C0 13
            rts                    ; 60

K102B:      jmp K1208              ; 4C 08 12
K102E:
            lda #$00               ; A9 00
            sta L00FD              ; 85 FD
            lda K13C5,X            ; BD C5 13
            bmi K1040              ; 30 09
            cmp K15D5,Y            ; D9 D5 15
            bcc K1041              ; 90 05
            beq K1040              ; F0 02
            eor #$FF               ; 49 FF
K1040:      clc                    ; 18
K1041:      adc #$02               ; 69 02
            sta K13C5,X            ; 9D C5 13
            lsr A                  ; 4A
            bcc K1077              ; 90 2E
            bcs K108E              ; B0 43
K104B:
            tya                    ; 98
            beq K109E              ; F0 50
K104E:
            lda K15D5,Y            ; B9 D5 15
            sta L00FD              ; 85 FD
            lda K139B,X            ; BD 9B 13
            cmp #$02               ; C9 02
            bcc K1077              ; 90 1D
            beq K108E              ; F0 32
            ldy K13B4,X            ; BC B4 13
            lda K13C8,X            ; BD C8 13
            sbc K13EF,Y            ; F9 EF 13
            pha                    ; 48
            lda K13C9,X            ; BD C9 13
            sbc K144F,Y            ; F9 4F 14
            tay                    ; A8
            pla                    ; 68
            bcs K1087              ; B0 17
            adc L00FC              ; 65 FC
            tya                    ; 98
            adc L00FD              ; 65 FD
            bpl K109E              ; 10 27
K1077:      lda K13C8,X            ; BD C8 13
            adc L00FC              ; 65 FC
            sta K13C8,X            ; 9D C8 13
            lda K13C9,X            ; BD C9 13
            adc L00FD              ; 65 FD
            jmp K1205              ; 4C 05 12
K1087:      sbc L00FC              ; E5 FC
            tya                    ; 98
            sbc L00FD              ; E5 FD
            bmi K109E              ; 30 10
K108E:      lda K13C8,X            ; BD C8 13
            sbc L00FC              ; E5 FC
            sta K13C8,X            ; 9D C8 13
            lda K13C9,X            ; BD C9 13
            sbc L00FD              ; E5 FD
            jmp K1205              ; 4C 05 12
K109E:      ldy K13B4,X            ; BC B4 13
            jmp K11F7              ; 4C F7 11
K10A4:      sta K10D5+1              ; 8D D6 10
            rts                    ; 60
K10A8:      sta K10C8+1              ; 8D C9 10
            sty K10CD+1              ; 8C CE 10
            lda K13DC,X            ; BD DC 13
            beq K10C3              ; F0 10
            tya                    ; 98
            cmp K13DE,X            ; DD DE 13
            bcc K10D2              ; 90 19
            bne K10C3              ; D0 08
            lda K10C8+1              ; AD C9 10
            cmp K13DD,X            ; DD DD 13
            bcc K10D2              ; 90 0F
K10C3:      lda #$01               ; A9 01
            sta K13DC,X            ; 9D DC 13
K10C8:
            lda #$00               ; A9 00
            sta K13DD,X            ; 9D DD 13
K10CD:
            lda #$00               ; A9 00
            sta K13DE,X            ; 9D DE 13
K10D2:      rts                    ; 60
K10D3:      ldx #$00               ; A2 00
K10D5:
            ldy #$00               ; A0 00
            bmi K1106              ; 30 2D
            txa                    ; 8A
            ldx #$29               ; A2 29
K10DC:      sta K1386,X            ; 9D 86 13
            dex                    ; CA
            bpl K10DC              ; 10 FA
            sta LBDD5              ; 8D D5 BD
            sta K1106+1              ; 8D 07 11
            stx K10D5+1              ; 8E D6 10
            tax                    ; AA
            jsr K10F6              ; 20 F6 10
            ldx #$07               ; A2 07
            jsr K10F6              ; 20 F6 10
            ldx #$0E               ; A2 0E
K10F6:      lda #$05               ; A9 05
            sta K13B2,X            ; 9D B2 13
            lda #$01               ; A9 01
            sta K13B3,X            ; 9D B3 13
            sta K13B5,X            ; 9D B5 13
            jmp K12FE              ; 4C FE 12
K1106:      lda #$00               ; A9 00
            sta LBDD7              ; 8D D7 BD
            lda #$00               ; A9 00
            ora #$0F               ; 09 0F
            sta LBDD8              ; 8D D8 BD
            jsr K111C              ; 20 1C 11
            ldx #$07               ; A2 07
            jsr K111C              ; 20 1C 11
            ldx #$0E               ; A2 0E
K111C:      dec K13B3,X            ; DE B3 13
            beq K112C              ; F0 0B
            bpl K1129              ; 10 06
            lda K13B2,X            ; BD B2 13
            sta K13B3,X            ; 9D B3 13
K1129:      jmp K11B4              ; 4C B4 11
K112C:      ldy K138B,X            ; BC 8B 13
            lda K1371,Y            ; B9 71 13
            sta K11A8+1              ; 8D A9 11
            sta K11B1+1              ; 8D B2 11
            lda K1389,X            ; BD 89 13
            bne K1161              ; D0 24
            ldy K13B0,X            ; BC B0 13

            lda K14AF,Y            ; B9 AF 14
            sta L00FC              ; 85 FC
            lda K14B2,Y            ; B9 B2 14
            sta L00FD              ; 85 FD

            ldy K1386,X            ; BC 86 13
            lda (L00FC),Y          ; B1 FC
            cmp #$FF               ; C9 FF
            bcc K1159              ; 90 06
            iny                    ; C8
            lda (L00FC),Y          ; B1 FC
            tay                    ; A8
            lda (L00FC),Y          ; B1 FC
K1159:      sta K13B1,X            ; 9D B1 13
            iny                    ; C8
            tya                    ; 98
            sta K1386,X            ; 9D 86 13
K1161:      ldy K13B5,X            ; BC B5 13
            lda K139D,X            ; BD 9D 13
            beq K11AE              ; F0 45
            sec                    ; 38
            sbc #$60               ; E9 60
            sta K13B4,X            ; 9D B4 13
            lda #$00               ; A9 00
            sta K139B,X            ; 9D 9B 13
            sta K139D,X            ; 9D 9D 13
            lda K138B,X            ; BD 8B 13
            cmp #$03               ; C9 03
            beq K11AE              ; F0 30
            lda #$09               ; A9 09
            sta K139F,X            ; 9D 9F 13
            inc K13B6,X            ; FE B6 13
            lda K151F,Y            ; B9 1F 15
            beq K1193              ; F0 08
            sta K13A0,X            ; 9D A0 13
            lda #$00               ; A9 00
            sta K13A1,X            ; 9D A1 13
K1193:      lda K150E,Y            ; B9 0E 15
            sta K139E,X            ; 9D 9E 13
            lda K14FD,Y            ; B9 FD 14
            sta K13DB,X            ; 9D DB 13
            lda K14EC,Y            ; B9 EC 14
            sta K13DA,X            ; 9D DA 13
            lda K138C,X            ; BD 8C 13
K11A8:
            jsr K1009              ; 20 09 10
            jmp K12D5              ; 4C D5 12
K11AE:      lda K138C,X            ; BD 8C 13
K11B1:
            jsr K1009              ; 20 09 10
K11B4:      ldy K139E,X            ; BC 9E 13
            beq K11D6              ; F0 1D
            lda K1530,Y            ; B9 30 15
            beq K11C1              ; F0 03
            sta K139F,X            ; 9D 9F 13
K11C1:      lda K1531,Y            ; B9 31 15
            cmp #$FF               ; C9 FF
            iny                    ; C8
            tya                    ; 98
            bcc K11CE              ; 90 04
            clc                    ; 18
            lda K1566,Y            ; B9 66 15
K11CE:      sta K139E,X            ; 9D 9E 13
            lda K1565,Y            ; B9 65 15
            bne K11EF              ; D0 19
K11D6:      lda K13B3,X            ; BD B3 13
            beq K120B              ; F0 30
            ldy K139B,X            ; BC 9B 13
            lda K1381,Y            ; B9 81 13
            sta K11EC+1              ; 8D ED 11
            ldy K139C,X            ; BC 9C 13

            lda K15DF,Y            ; B9 DF 15
            sta L00FC              ; 85 FC
K11EC:
            jmp K102B              ; 4C 2B 10
K11EF:      bpl K11F6              ; 10 05
            adc K13B4,X            ; 7D B4 13
            and #$7F               ; 29 7F
K11F6:      tay                    ; A8
K11F7:      lda #$00               ; A9 00
            sta K13C5,X            ; 9D C5 13
            lda K13EF,Y            ; B9 EF 13
            sta K13C8,X            ; 9D C8 13
            lda K144F,Y            ; B9 4F 14
K1205:      sta K13C9,X            ; 9D C9 13
K1208:      lda K13B3,X            ; BD B3 13
K120B:      cmp #$02               ; C9 02
            beq K125D              ; F0 4E
            ldy K13A0,X            ; BC A0 13
            beq K125A              ; F0 46
            ora K1389,X            ; 1D 89 13
            beq K125A              ; F0 41
            lda K13A1,X            ; BD A1 13
            bne K1232              ; D0 14
            lda K159C,Y            ; B9 9C 15
            bpl K122F              ; 10 0C
            sta K13CB,X            ; 9D CB 13
            lda K15B8,Y            ; B9 B8 15
            sta K13CA,X            ; 9D CA 13
            jmp K124B              ; 4C 4B 12
K122F:      sta K13A1,X            ; 9D A1 13
K1232:      lda K15B8,Y            ; B9 B8 15
            clc                    ; 18
            bpl K123B              ; 10 03
            dec K13CB,X            ; DE CB 13
K123B:      adc K13CA,X            ; 7D CA 13
            sta K13CA,X            ; 9D CA 13
            bcc K1246              ; 90 03
            inc K13CB,X            ; FE CB 13
K1246:      dec K13A1,X            ; DE A1 13
            bne K125A              ; D0 0F
K124B:      lda K159D,Y            ; B9 9D 15
            cmp #$FF               ; C9 FF
            iny                    ; C8
            tya                    ; 98
            bcc K1257              ; 90 03
            lda K15B8,Y            ; B9 B8 15
K1257:      sta K13A0,X            ; 9D A0 13
K125A:      jmp K12D5              ; 4C D5 12
K125D:      ldy K13B1,X            ; BC B1 13
            lda K14B5,Y            ; B9 B5 14
            sta L00FC              ; 85 FC
            lda K14D1,Y            ; B9 D1 14
            sta L00FD              ; 85 FD
            ldy K1389,X            ; BC 89 13
            lda (L00FC),Y          ; B1 FC
            cmp #$40               ; C9 40
            bcc K128B              ; 90 18
            cmp #$60               ; C9 60
            bcc K1295              ; 90 1E
            cmp #$C0               ; C9 C0
            bcc K12A9              ; 90 2E
            lda K138A,X            ; BD 8A 13
            bne K1282              ; D0 02
            lda (L00FC),Y          ; B1 FC
K1282:      adc #$00               ; 69 00
            sta K138A,X            ; 9D 8A 13
            beq K12CC              ; F0 43
            bne K12D5              ; D0 4A
K128B:      sta K13B5,X            ; 9D B5 13
            iny                    ; C8
            lda (L00FC),Y          ; B1 FC
            cmp #$60               ; C9 60
            bcs K12A9              ; B0 14
K1295:      cmp #$50               ; C9 50
            and #$0F               ; 29 0F
            sta K138B,X            ; 9D 8B 13
            beq K12A4              ; F0 06
            iny                    ; C8
            lda (L00FC),Y          ; B1 FC
            sta K138C,X            ; 9D 8C 13
K12A4:      bcs K12CC              ; B0 26
            iny                    ; C8
            lda (L00FC),Y          ; B1 FC
K12A9:      cmp #$BD               ; C9 BD
            bcc K12B3              ; 90 06
            beq K12CC              ; F0 1D
            ora #$F0               ; 09 F0
            bne K12C9              ; D0 16
K12B3:      sta K139D,X            ; 9D 9D 13
            lda K138B,X            ; BD 8B 13
            cmp #$03               ; C9 03
            beq K12CC              ; F0 0F
            lda #$00               ; A9 00
            sta K13DB,X            ; 9D DB 13
            lda #$0F               ; A9 0F
            sta K13DA,X            ; 9D DA 13
            lda #$FE               ; A9 FE
K12C9:      sta K13B6,X            ; 9D B6 13
K12CC:      iny                    ; C8
            lda (L00FC),Y          ; B1 FC
            beq K12D2              ; F0 01
            tya                    ; 98
K12D2:      sta K1389,X            ; 9D 89 13
K12D5:      ldy K13DC,X            ; BC DC 13
            bne K1308              ; D0 2E
            lda K13CA,X            ; BD CA 13
            sta LBDC2,X            ; 9D C2 BD
            lda K13CB,X            ; BD CB 13
            sta LBDC3,X            ; 9D C3 BD
            lda K13DB,X            ; BD DB 13
            sta LBDC6,X            ; 9D C6 BD
            lda K13DA,X            ; BD DA 13
            sta LBDC5,X            ; 9D C5 BD
K12F2:      lda K13C8,X            ; BD C8 13
            sta LBDC0,X            ; 9D C0 BD
            lda K13C9,X            ; BD C9 13
            sta LBDC1,X            ; 9D C1 BD
K12FE:      lda K139F,X            ; BD 9F 13
            and K13B6,X            ; 3D B6 13
            sta LBDC4,X            ; 9D C4 BD
            rts                    ; 60
K1308:      lda K13DD,X            ; BD DD 13
            sta L00FC              ; 85 FC
            lda K13DE,X            ; BD DE 13
            sta L00FD              ; 85 FD
            lda #$FE               ; A9 FE
            sta K13B6,X            ; 9D B6 13
            lda #$00               ; A9 00
            sta K139E,X            ; 9D 9E 13
            inc K13DC,X            ; FE DC 13
            cpy #$02               ; C0 02
            beq K132D              ; F0 0A
            bcs K134B              ; B0 26
            sta LBDC6,X            ; 9D C6 BD
            sta LBDC5,X            ; 9D C5 BD
            bcc K12F2              ; 90 C5
K132D:      tay                    ; A8
            lda (L00FC),Y          ; B1 FC
            sta LBDC5,X            ; 9D C5 BD
            iny                    ; C8
            lda (L00FC),Y          ; B1 FC
            sta LBDC6,X            ; 9D C6 BD
            iny                    ; C8
            lda (L00FC),Y          ; B1 FC
            sta LBDC2,X            ; 9D C2 BD
            sta LBDC3,X            ; 9D C3 BD
            lda #$09               ; A9 09
K1344:      sta K139F,X            ; 9D 9F 13
            sta LBDC4,X            ; 9D C4 BD
K134A:
            rts                    ; 60

K134B:      lda (L00FC),y
            bne K1354
            sta K13DC,x
            beq K1344
K1354:      tay
            lda K136F,y
            sta LBDC0,x
            lda K13CF,y
            sta LBDC1,x
            ldy K13DC,x
            lda (L00FC),y
            beq K134A
            cmp #$82
            bcs K134A
            inc K13DC,x
K136F:      bcc K1344

;            .byte $B1,$FC,$D0,$05,$9D,$DC,$13,$F0
;            .byte $F0,$A8,$B9,$6F,$13,$9D,$C0,$BD
;            .byte $B9,$CF,$13,$9D,$C1,$BD,$BC,$DC
;            .byte $13,$B1,$FC,$F0,$E2,$C9,$82,$B0
;            .byte $DE,$FE,$DC,$13,$90,$D3

K1371:
	.byte <K1009,<K100C,<K100C,<K1013,<K1013,<K101D,<K101D,<K1021
	.byte <K1021,<K1021,<K1021,<K1021,<K1021,<K1021,<K1021,<K1021

;            .byte $09,$0C,$0C,$13,$13,$1D,$1D,$21
;            .byte $21,$21,$21,$21,$21,$21,$21,$21

K1381:
	.byte <K102B,<K104E,<K104E,<K104B,<K102E

;            .byte $2B,$4E,$4E,$4B,$2E

K1386:      .byte $00,$00,$00
K1389:      .byte $00
K138A:      .byte $00
K138B:      .byte $00
K138C:      .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00
K139B:      .byte $00
K139C:      .byte $00
K139D:      .byte $00
K139E:      .byte $00
K139F:      .byte $00
K13A0:      .byte $00
K13A1:      .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00
K13B0:      .byte $00
K13B1:      .byte $00
K13B2:      .byte $00
K13B3:      .byte $00
K13B4:      .byte $00
K13B5:      .byte $01
K13B6:      .byte $FE,$01,$00
K13B9:      .byte $00,$00,$00,$01,$FE,$02,$00
K13C0:      .byte $00,$00,$00,$01,$FE
K13C5:      .byte $00,$00,$00
K13C8:      .byte $00
K13C9:      .byte $00
K13CA:      .byte $00
K13CB:      .byte $00,$00,$00,$00
K13CF:      .byte $00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00
K13DA:      .byte $00
K13DB:      .byte $00
K13DC:      .byte $00
K13DD:      .byte $00
K13DE:      .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00

K13EF:      .byte $17,$27,$39,$4B,$5F,$74,$8A,$A1
            .byte $BA,$D4,$F0,$0E,$2D,$4E,$71,$96
            .byte $BE,$E8,$14,$43,$74,$A9,$E1,$1C
            .byte $5A,$9C,$E2,$2D,$7C,$CF,$28,$85
            .byte $E8,$52,$C1,$37,$B4,$39,$C5,$5A
            .byte $F7,$9E,$4F,$0A,$D1,$A3,$82,$6E
            .byte $68,$71,$8A,$B3,$EE,$3C,$9E,$15
            .byte $A2,$46,$04,$DC,$D0,$E2,$14,$67
            .byte $DD,$79,$3C,$29,$44,$8D,$08,$B8
            .byte $A1,$C5,$28,$CD,$BA,$F1,$78,$53
            .byte $87,$1A,$10,$71,$42,$89,$4F,$9B
            .byte $74,$E2,$F0,$A6,$0E,$33,$20,$FF
K144F:      .byte $01,$01,$01,$01,$01,$01,$01,$01
            .byte $01,$01,$01,$02,$02,$02,$02,$02
            .byte $02,$02,$03,$03,$03,$03,$03,$04
            .byte $04,$04,$04,$05,$05,$05,$06,$06
            .byte $06,$07,$07,$08,$08,$09,$09,$0A
            .byte $0A,$0B,$0C,$0D,$0D,$0E,$0F,$10
            .byte $11,$12,$13,$14,$15,$17,$18,$1A
            .byte $1B,$1D,$1F,$20,$22,$24,$27,$29
            .byte $2B,$2E,$31,$34,$37,$3A,$3E,$41
            .byte $45,$49,$4E,$52,$57,$5C,$62,$68
            .byte $6E,$75,$7C,$83,$8B,$93,$9C,$A5
            .byte $AF,$B9,$C4,$D0,$DD,$EA,$F8,$FF

K14AF:      .byte <K15E9,<K15F8,<K1607	;$E9,$F8,$07

K14B2:      .byte >K15E9,>K15F8,>K1607	;$15,$15,$16

K14B5:      .byte <K1616,<K1639,<K167C,<K168B,<K1709,<K174F,<K1753,<K17D0
            .byte <K1813,<K184B,<K1879,<K18D7,<K18EE,<K192E,<K199A,<K19D5
            .byte <K1A0B,<K1A78,<K1AAB,<K1ACE,<K1B2C,<K1B4E,<K1B91,<K1C0F
            .byte <K1C52,<K1C95,<K1CEF,<K1D35

K14D1:      .byte >K1616,>K1639,>K167C,>K168B,>K1709,>K174F,>K1753,>K17D0
            .byte >K1813,>K184B,>K1879,>K18D7,>K18EE,>K192E,>K199A,>K19D5
            .byte >K1A0B,>K1A78,>K1AAB,>K1ACE,>K1B2C,>K1B4E,>K1B91,>K1C0F
            .byte >K1C52,>K1C95,>K1CEF
K14EC:      .byte >K1D35

K14ED:      .byte $07,$09,$04,$03,$06,$08,$FC
            .byte $04,$04,$03,$00,$00,$02,$04,$04
            .byte $00
K14FD:      .byte $06,$99,$99,$7A,$99,$98,$99,$8B
            .byte $69,$69,$69,$8C,$3C,$05,$99,$99
            .byte $5C
K150E:      .byte $29,$01,$05,$0B,$0F,$13,$1A,$21
            .byte $21,$21,$21,$23,$26,$28,$2B,$2F
            .byte $21
K151F:      .byte $33,$01,$01,$06,$01,$01,$01,$0B
            .byte $10,$00,$15,$0B,$00,$00,$01
K152E:      .byte $01
            .byte $0B
K1530:      .byte $00
K1531:      .byte $81,$41,$40,$FF,$81,$41,$81,$81
            .byte $40,$FF,$13,$41,$40,$FF,$81,$41
            .byte $40,$FF,$81,$41,$40,$00,$00,$00
            .byte $FF,$81
K154B:      .byte $41,$81,$80
K154E:      .byte $80,$00,$FF
            .byte $41,$FF,$43,$41,$FF,$15,$FF,$21
            .byte $20,$FF,$21,$41,$40,$FF,$21,$41
            .byte $40,$FF,$21,$21
K1565:      .byte $20
K1566:      .byte $FF,$48,$28,$80,$00,$4C,$2A,$4C
            .byte $48,$80,$00,$80,$80,$80,$00,$48
            .byte $80,$80,$00,$48,$28,$24,$20,$1C
            .byte $18,$00,$4C,$2A,$4C,$46,$44,$48
            .byte $1D,$80,$00,$80,$80,$00,$80,$00
            .byte $80,$80,$00,$58,$80,$80,$00,$53
            .byte $80,$80,$00,$80,$80,$80
K159C:      .byte $00
K159D:      .byte $88,$25,$15,$15,$FF,$81,$1C,$18
            .byte $18,$FF,$82,$20,$25,$05,$FF,$81
            .byte $40,$40,$40,$FF,$80,$7F,$69,$7F
            .byte $61,$7F,$61
K15B8:      .byte $FF,$00,$30,$D0,$30,$03,$00,$40
            .byte $C0,$40,$08,$00,$30,$D0,$30,$0C
            .byte $00,$38,$C8,$38,$12,$C0,$08,$08
            .byte $F8,$F8,$08,$08,$18
K15D5:      .byte $00,$02,$02,$00,$00,$00,$00,$02
            .byte $00,$00

K15DF:      .byte $00,$40,$00,$40,$C0,$E0,$80,$50
            .byte $20,$60
K15E9:      .byte $00,$03,$06,$03,$06,$0A
            .byte $0D,$10,$13,$16,$19,$0D,$10,$FF
            .byte $00
K15F8:      .byte $01,$04,$04,$08,$09,$0B,$0E
            .byte $11,$15,$17,$1A,$0E,$11,$FF,$00
            
K1607:      .byte $02,$05,$05,$07,$07,$0C,$0F,$12
            .byte $14,$18,$1B,$0F,$12,$FF,$00
K1616:      .byte $01
            .byte $4F,$06,$75,$40,$75,$BE,$F4,$05
            .byte $75,$01,$75,$75,$BE,$F3,$4F,$06
            .byte $75,$40,$75,$BE,$F4,$05,$75,$01
            .byte $75,$75,$BE,$F6,$05,$75,$06,$75
            .byte $75,$00
K1639:      .byte $03,$40,$81,$81,$8D,$81
            .byte $8F,$88,$81,$90,$81,$90,$8F,$81
            .byte $8F,$88,$8B,$8D,$81,$81,$8D,$81
            .byte $8F,$88,$81,$90,$81,$90,$8F,$81
            .byte $8F,$88,$8B,$8D,$81,$81,$8D,$81
            .byte $8F,$88,$81,$90,$81,$90,$8F,$81
            .byte $8F,$88,$8B,$8D,$81,$81,$8D,$81
            .byte $8F,$88,$81,$90,$81,$90,$8F,$81
            .byte $8F,$88,$8B,$8D,$00
K167C:      .byte $56,$00,$50
            .byte $D2,$07,$A5,$FD,$54,$01,$F9,$BE
            .byte $52,$02,$FE,$00
K168B:      .byte $01,$40,$81,$04
            .byte $81,$02,$81,$04,$81,$81,$01,$81
            .byte $02,$81,$04,$81,$01,$81,$04,$81
            .byte $02,$81,$01,$81,$04,$81,$01,$81
            .byte $02,$81,$04,$81,$01,$81,$04,$81
            .byte $02,$81,$04,$81,$81,$01,$81,$02
            .byte $81,$04,$81,$01,$81,$04,$81,$02
            .byte $81,$01,$81,$04,$81,$01,$81,$02
            .byte $7F,$04,$7F,$01
            .byte $7D,$04,$7D,$02
            .byte $7D,$04,$7D,$7D,$01,$7D,$02,$7D
            .byte $04,$7D,$01,$7D,$04,$7D,$02,$7D
            .byte $01,$7D,$04,$7D,$01,$7D,$02,$7D
            .byte $04,$7D,$01,$7D,$04,$7D,$02,$7D
            .byte $04,$7D,$7D,$01,$7D,$02,$7D,$04
            .byte $7D,$01,$7D,$04,$7D,$02,$7D,$01
            .byte $7D,$04,$7D,$01,$7D,$02,$7C,$04
            .byte $7C,$00
K1709:      .byte $08,$40,$8D,$09,$94,$99
            .byte $8D,$9B
            .byte $94,$8D,$9C,$8D
            .byte $9C,$9B
            .byte $8D,$9B,$94,$97,$99,$8D,$94,$99
            .byte $8D,$9B,$94,$8D,$9E,$8D,$9B,$9C
            .byte $8D,$9B,$94,$97,$99,$08,$8D,$09
            .byte $94,$99,$8D,$9B,$94,$8D,$9C,$8D
            .byte $9C,$9B,$8D,$9B,$94,$97,$99,$8D
            .byte $94,$99,$8D,$9B,$94,$8D,$9E,$8D
            .byte $9B,$9C,$8D,$9B,$94,$97,$99,$00
K174F:
            .byte $40,$BE,$C1,$00
K1753:      .byte $01,$40,$7A,$04
            .byte $7A,$02,$7A,$04,$7A,$7A,$01,$7A
            .byte $02,$7A,$04,$7A,$01,$7A,$04,$7A
            .byte $02,$7A,$01,$7A,$04,$7A,$01,$7A
            .byte $02,$7A,$04,$7A,$01,$7A,$04,$7A
            .byte $02,$7A,$04,$7A,$7A,$01,$7A,$02
            .byte $7A,$04,$7A,$01,$7F,$04,$7F,$02
            .byte $7F,$01,$7F,$04,$7F,$01,$7F,$02
            .byte $7C,$04,$7C,$01,$81,$04,$81,$02
            .byte $81,$04,$81,$81,$01,$81,$02,$81
            .byte $04,$81,$01,$81,$04,$81,$02,$81
            .byte $01,$81,$04,$81,$01,$81,$02,$81
            .byte $04,$81,$01,$81,$04,$81,$02,$81
            .byte $04,$81,$81,$01,$81,$02,$81,$04
            .byte $81,$01,$81,$04,$81,$02,$81,$01
            .byte $81,$04,$81,$01,$81,$02,$7F,$7F
            .byte $00
K17D0:      .byte $0A,$40,$8D,$94,$99,$8D,$9B
            .byte $94,$8D,$9C,$8D,$9C,$9B,$8D,$9B
            .byte $94,$97,$99,$8D,$94,$99,$8D,$9B
            .byte $94,$8D,$9E,$8D,$9B,$9C,$8D,$9B
            .byte $94,$97,$99,$8D,$94,$99,$8D,$9B
            .byte $94,$8D,$9C,$8D,$9C,$9B,$8D,$9B
            .byte $94,$97,$99,$8D,$94,$99,$8D,$9B
            .byte $94,$8D,$9E,$8D,$9B,$9C,$8D,$9B
            .byte $94,$97,$99,$00
K1813:
            .byte $0B,$40,$8D,$BD
            .byte $51,$03,$BD,$51,$04,$51,$05,$43
            .byte $00

            .byte $99,$50,$54,$01,$F7,$BE,$FB
            .byte $41,$04,$9E,$43,$04,$A0,$BD,$43
            .byte $00

            .byte $9E,$50,$BD,$9B,$BD,$99,$FE
            .byte $54,$01,$F2,$BE,$FB,$41,$04,$97
            .byte $43,$04,$99,$BD,$43,$00

            .byte $9B,$50
            .byte $BD,$9C,$BD,$00
K184B:
            .byte $0B,$40,$99,$FE
            .byte $54,$01,$FE,$43,$00

            .byte $92,$40,$92
            .byte $FE,$54,$01,$F9,$BE,$FB,$41,$03
            .byte $90,$43,$06,$92,$BD,$43,$00

            .byte $90
            .byte $50,$BD,$8F,$BD,$8F,$43,$00

            .byte $90
            .byte $8F,$40,$8D,$FE,$54,$01,$F5,$BE
            .byte $F3,$00
K1879:
            .byte $01,$40,$77,$04,$77,$83
            .byte $77,$02,$85,$04,$7E,$77,$86,$77
            .byte $86,$01,$85,$77,$02,$85,$04,$7E
            .byte $81,$83,$01,$77,$04,$77,$83,$77
            .byte $02,$85,$04,$7E,$77,$86,$77,$86
            .byte $01,$85,$77,$02,$85,$04,$7E,$81
            .byte $83,$01,$77,$04,$77,$83,$77,$02
            .byte $85,$04,$7E,$77,$86,$77,$86,$01
            .byte $85,$77,$02,$85,$04,$7E,$81,$83
            .byte $01,$77,$04,$77,$83,$77,$02,$85
            .byte $04,$7E,$77,$86,$77,$86,$01,$85
            .byte $77,$02,$85,$04,$7E,$81,$83,$00
K18D7:
            .byte $0C,$40,$A2,$F9,$BE,$FD,$9D,$FD
            .byte $96,$F9,$BE,$FD,$94,$FD,$96,$F9
            .byte $BE,$F9,$8F,$F9,$BE,$F9,$00
K18EE:
            .byte $40
            .byte $BE,$BD,$0D,$8F,$8F,$BD,$83,$BD
            .byte $AE,$BD,$A7,$8F,$8F,$BD,$83,$8F
            .byte $8F,$FE,$8F,$8F,$BD,$83,$BD,$AE
            .byte $BD,$A7,$8F,$8F,$BD,$83,$8F,$8F
            .byte $FE,$8F,$8F,$BD,$83,$BD,$AE,$BD
            .byte $A7,$8F,$8F,$BD,$83,$8F,$8F,$FE
            .byte $8F,$8F,$BD,$83,$BD,$AE,$BD,$A7
            .byte $8F,$8F,$BD,$83,$8F,$8F,$00
K192E:
            .byte $01
            .byte $40,$78,$04,$78,$88,$78,$02,$8A
            .byte $04,$83,$78,$0E,$8B,$04,$78,$0F
            .byte $8B,$01,$8A,$78,$02,$8A,$01,$83
            .byte $04,$86,$88,$01,$7A,$04,$7A,$88
            .byte $7A,$02,$8A,$04,$83,$7A,$0E,$8D
            .byte $04,$7A,$0F,$8A,$01,$8B,$7A,$02
            .byte $8A,$01,$83,$04,$86,$88,$01,$7C
            .byte $04,$7C,$88,$7C,$02,$8A,$04,$83
            .byte $7C,$0E,$8B,$04,$7C,$0F,$8B,$01
            .byte $8A,$7C,$02,$8A,$04,$83,$86,$88
            .byte $01,$7C,$04,$7C,$88,$7C,$02,$8A
            .byte $04,$83,$7C,$0E,$8B,$04,$7C,$0F
            .byte $8B,$01,$8A,$7C,$02,$8A,$04,$83
            .byte $86,$88,$00
K199A:
            .byte $10,$41,$03,$A5,$43
            .byte $04,$A7,$FC,$A5,$FE,$54,$07,$FC
            .byte $BE,$BD,$41,$03,$A3,$43,$04,$A5
            .byte $FC,$A3,$FE,$50,$A2,$BD,$54,$07
            .byte $FD,$41,$08,$A0,$43,$04,$A2,$BD
            .byte $54,$07,$43,$00

            .byte $A3,$50,$43,$00
            .byte $A0,$50,$BD,$54,$07,$F8,$BE,$F7
            .byte $40,$A0,$A2,$A3,$A5,$00

K19D5:      .byte $10,$41
            .byte $03,$A2,$43,$04,$A3,$FC,$A2,$FE
            .byte $54,$07,$FC,$BE,$BD,$41,$03,$A0
            .byte $43,$04,$A2,$FC,$A0,$FE,$50,$9E
            .byte $BD,$54,$07,$FD,$41,$09,$9B,$43
            .byte $04,$9E,$BD,$54,$07,$43,$00

            .byte $A0
            .byte $50,$43,$00

            .byte $9B,$50,$BD,$54,$07
            .byte $F8,$BE,$F3,$00
K1A0B:
            .byte $01,$40,$78,$04
            .byte $78,$88,$78,$02,$8A,$04,$83,$78
            .byte $0E,$8B,$04,$78,$0F,$8B,$01,$8A
            .byte $78,$02,$8A,$01,$83,$04,$86,$88
            .byte $01,$7A,$04,$7A,$88,$7A,$02,$8A
            .byte $04,$83,$7A,$0E,$8D,$04,$7A,$0F
            .byte $8A,$01,$8B,$7A,$02,$8A,$01,$83
            .byte $04,$86,$88,$01,$7C,$04,$7C,$02
            .byte $88,$04,$7C,$8A,$01,$83,$02,$7C
            .byte $04,$8B,$01,$7C,$04,$8B,$02,$8A
            .byte $01,$7C,$04,$8A,$01,$83,$02,$86
            .byte $88,$05,$90,$BD,$06,$90,$FE,$05
            .byte $90,$06,$90,$BD,$05,$90,$BD,$06
            .byte $90,$05,$90,$BD,$06,$90,$90,$90
            .byte $00
K1A78:
            .byte $10,$41,$03,$A5,$43,$04,$A7
            .byte $FC,$A5,$FE,$54,$07,$FC,$BE,$BD
            .byte $41,$03,$A3,$43,$04,$A5,$FC,$A3
            .byte $FE,$50,$A2,$BD,$54,$07,$FD,$0D
            .byte $40,$88,$88,$94,$88,$96,$8F,$88
            .byte $97,$88,$97,$96,$88,$96,$8F,$92
            .byte $94,$88,$F1,$00
K1AAB:
            .byte $10,$41,$03,$A2
            .byte $43,$04,$A3,$FC,$A2,$FE,$54,$07
            .byte $FC,$BE,$BD,$41,$03,$A0,$43,$04
            .byte $A2,$FC,$A0,$FE,$50,$9E,$BD,$54
            .byte $07,$FD,$0D,$40,$83,$E1,$00
K1ACE:
            .byte $05
            .byte $40,$90,$BD,$06,$90,$FE,$05,$90
            .byte $06,$90,$BD,$05,$90,$BD,$06,$90
            .byte $05,$90,$BD,$90,$06,$90,$05,$90
            .byte $90,$BD,$06,$90,$FE,$05,$90,$06
            .byte $90,$BD,$05,$90,$BD,$06,$90,$05
            .byte $90,$BD,$90,$06,$90,$90,$05,$90
            .byte $BD,$06,$90,$FE,$05,$90,$06,$90
            .byte $BD,$05,$90,$BD,$06,$90,$05,$90
            .byte $BD,$90,$06,$90,$BD,$05,$90,$BD
            .byte $06,$90,$FE,$05,$90,$06,$90,$BD
            .byte $05,$90,$BD,$06,$90,$05,$90,$BD
            .byte $06,$90,$90,$90,$00
K1B2C:
            .byte $10,$40,$97
            .byte $FE,$54,$01,$F8,$40,$9B,$FE,$54
            .byte $01,$40,$96,$FE,$54,$01,$F8,$40
            .byte $92,$FE,$54,$01,$40,$94,$BD,$92
            .byte $94,$BE,$BD,$54,$01,$E7,$00
K1B4E:
            .byte $11
            .byte $40,$94,$96,$97,$8F,$94,$96,$8F
            .byte $9B,$8F,$96,$97,$8F,$97,$99,$8F
            .byte $92,$94,$96,$97,$8F,$94,$96,$8F
            .byte $9B,$8F,$96,$97,$8F,$97,$99,$8F
            .byte $92,$94,$96,$97,$8F,$94,$96,$8F
            .byte $9B,$8F,$96,$97,$8F,$97,$99,$8F
            .byte $92,$94,$96,$97,$8F,$94,$96,$8F
            .byte $9B,$8F,$96,$97,$8F,$97,$99,$8F
            .byte $92,$00
K1B91:
            .byte $01,$40,$78,$04,$78,$02
            .byte $84,$04,$78,$84,$01,$84,$02,$78
            .byte $04,$84,$01,$78,$04,$78,$02,$84
            .byte $01,$78,$04,$84,$01,$84,$02,$78
            .byte $04,$84,$01,$7A,$04,$7A,$02,$86
            .byte $04,$7A,$86,$01,$86,$02,$7A,$04
            .byte $86,$01,$7A,$04,$7A,$02,$86,$01
            .byte $7A,$04,$86,$01,$86,$02,$7A,$04
            .byte $86,$01,$78,$04,$78,$02,$84,$04
            .byte $78,$84,$01,$84,$02,$78,$04,$84
            .byte $01,$78,$04,$78,$02,$84,$01,$78
            .byte $04,$84,$01,$84,$02,$78,$04,$84
            .byte $01,$7A,$04,$7A,$02,$86,$04,$7A
            .byte $86,$01,$86,$02,$7A,$04,$86,$01
            .byte $7A,$04,$7A,$02,$86,$01,$7A,$04
            .byte $86,$01,$86,$02,$7A,$04,$86,$00
K1C0F:
            .byte $11,$40,$94,$96,$97,$90,$94,$96
            .byte $90,$9C,$90,$96,$97,$90,$97,$99
            .byte $90,$92,$94,$96,$97,$92,$94,$96
            .byte $92,$9E,$92,$96,$97,$92,$97,$99
            .byte $92,$94,$94,$96,$97,$90,$94,$96
            .byte $90,$9C,$90,$96,$97,$90,$97,$99
            .byte $90,$92,$94,$96,$97,$92,$94,$96
            .byte $92,$9E,$92,$96,$97,$92,$97,$99
            .byte $92,$94,$00
K1C52:
            .byte $0A,$40,$90,$92,$94
            .byte $8B,$90,$92,$8B,$97,$8B,$92,$94
            .byte $8B,$94,$96,$8B,$8D,$8F,$92,$94
            .byte $8D,$8F,$92,$8D,$99,$8D,$92,$94
            .byte $8D,$94,$96,$8D,$8F,$90,$92,$94
            .byte $8B,$90,$92,$8B,$97,$8B,$92,$94
            .byte $8B,$94,$96,$8B,$8D,$8F,$92,$94
            .byte $8D,$8F,$92,$8D,$99,$8D,$92,$94
            .byte $8D,$94,$96,$8D,$8F,$00
K1C95:
            .byte $01,$40
            .byte $7C,$BD,$04,$7C,$7C,$02,$7C,$BD
            .byte $04,$7C,$7C,$7C,$BD,$01,$7A,$7C
            .byte $02,$7F,$01,$7E,$04,$7C,$7A,$01
            .byte $7C,$BD,$04,$7C,$7C,$06,$7C,$04
            .byte $7C,$7C,$FD,$05,$7C,$7C,$06,$7C
            .byte $05,$7C,$FE,$01,$7C,$BD,$04,$7C
            .byte $7C,$02,$7C,$BD,$04,$7C,$7C,$7C
            .byte $BD,$01,$7A,$7C,$02,$7F,$01,$7E
            .byte $04,$7C,$7A,$01,$7C,$BD,$04,$7C
            .byte $7C,$06,$7C,$04,$7C,$7C,$FD,$05
            .byte $7C,$7C,$06,$7C,$05,$7C,$FE,$00
K1CEF:
            .byte $40,$BE,$BD,$0D,$88,$88,$BD,$7C
            .byte $BD,$B3,$BD,$AC,$88,$88,$BD,$7C
            .byte $88,$88,$FE,$88,$88,$BD,$7C,$BD
            .byte $B1,$BD,$B3,$88,$AC,$BD,$7C,$88
            .byte $88,$FE,$07,$97,$43,$00

            .byte $88,$96
            .byte $88,$92,$88,$8D,$8F,$97,$88,$96
            .byte $88,$92,$88,$8D,$8F,$97,$88,$96
            .byte $88,$92,$88,$8D,$8F,$97,$88,$10
            .byte $40,$A0,$A2,$A3,$A5,$00
K1D35:
            .byte $40,$BE
            .byte $C5,$0A,$8B,$8D,$8F,$90,$00

; FX sounds

K1D3E:      .byte $00
            .byte $F9,$08,$C4,$81,$A8,$41,$C0,$81
            .byte $BE,$BC,$80,$BA,$B8,$B6,$B4,$B2
            .byte $B0,$AE,$AC,$AA,$A8,$A6,$A4,$A2
            .byte $A0,$9E,$9C,$9A,$98,$96,$94,$92
            .byte $90,$00

K1D61:      .byte $00,$FA,$08,$B8,$81,$A4
            .byte $41,$A0,$B4,$81,$98,$92,$9C,$90
            .byte $95,$9E,$92,$80,$94,$8F,$8E,$8D
            .byte $8C,$8B,$8A,$89,$88,$87,$86,$84
            .byte $00

K1D80:      .byte $0A,$00,$02,$A0,$41,$A0,$A0
            .byte $A4,$A4,$A4,$A7,$A7,$A7,$A0,$A0
            .byte $A0,$A4,$A4,$A4,$A7,$A7,$A7,$A0
            .byte $A0,$A0,$A4,$A4,$A4,$A7,$A7,$A7
            .byte $A0,$A0,$A0,$A4,$A4,$A4,$A7,$A7
            .byte $A7,$00

K1DA9:      .byte $00,$89,$04,$A2,$41,$A2
            .byte $A2,$A6,$A6,$A6,$40,$A9,$A9,$A9
            .byte $A2,$A2,$A2,$A6,$A6,$A6,$A9,$A9
            .byte $A9,$A2,$A2,$A2,$A6,$A6,$A6,$A9
            .byte $A9,$A9,$A2,$A2,$A2,$A6,$A6,$A6
            .byte $A9,$A9,$A9,$00




