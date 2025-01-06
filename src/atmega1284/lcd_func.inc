/*
 * lcd_func.inc
 *
 *  Created: 12/5/2024 9:10:51 AM
 *   Author: janos
 */ 

time_dsp0:
ldi temp,0
call lcd_gotoxy_80
clr fbin2
clr fbin1
lds fbin0,hour
call bin3bcd
call dig02_disp
lds temp,flag_reg0
sbrs temp,time_05f
jmp dd_dsp
ldi temp,' '
call lcd_put
lds temp,flag_reg0
cbr temp,0x01
sts flag_reg0,temp
jmp s_dsp
dd_dsp:
ldi temp,':'
call lcd_put
lds temp,flag_reg0
sbr temp,0x01
sts flag_reg0,temp
s_dsp:
clr fbin2
clr fbin1
lds fbin0,min_
call bin3bcd
call dig02_disp
ret

date_dsp0:
ldi temp,15
call lcd_gotoxy_80
lds temp,date_sram
call lcd_put
lds temp,date_sram+1
call lcd_put
lds temp,date_sram+2
call lcd_put
lds temp,date_sram+3
call lcd_put
lds temp,date_sram+4
call lcd_put
ret

temp_dsp0:
ldi temp,15
call lcd_gotoxy_80
lds temp,temp_sram
call lcd_put
lds temp,temp_sram+1
call lcd_put
lds temp,temp_sram+2
call lcd_put
lds temp,temp_sram+3
call lcd_put
ldi temp,0xb2
call lcd_put
ret

hum_dsp0:
ldi temp,15
call lcd_gotoxy_80
lds temp,hum_sram
call lcd_put
lds temp,hum_sram+1
call lcd_put
lds temp,hum_sram+2
call lcd_put
lds temp,hum_sram+3
call lcd_put
ldi temp,'%'
call lcd_put
ret

dig08_disp:
mov temp,tbcd3
swap temp
andi temp,0x0f
ori temp,0x30
call lcd_put
mov temp,tbcd3
andi temp,0x0f
ori temp,0x30
call lcd_put
dig06_disp:
mov temp,tbcd2
swap temp
andi temp,0x0f
ori temp,0x30
call lcd_put
mov temp,tbcd2
andi temp,0x0f
ori temp,0x30
call lcd_put
dig04_disp:
mov temp,tbcd1
swap temp
andi temp,0x0f
ori temp,0x30
call lcd_put
dig03_disp:
mov temp,tbcd1
andi temp,0x0f
ori temp,0x30
call lcd_put
dig02_disp:
mov temp,tbcd0
swap temp
andi temp,0x0f
ori temp,0x30
call lcd_put
mov temp,tbcd0
andi temp,0x0f
ori temp,0x30
call lcd_put
ret

dig04_dispc:
mov temp,tbcd1
swap temp
andi temp,0x0f
ori temp,0x30
call lcd_put
mov temp,tbcd1
andi temp,0x0f
ori temp,0x30
call lcd_put
ldi temp,'.'
call lcd_put
mov temp,tbcd0
swap temp
andi temp,0x0f
ori temp,0x30
call lcd_put
mov temp,tbcd0
andi temp,0x0f
ori temp,0x30
call lcd_put
ret

dig03_dispvo:
mov temp,tbcd1
swap temp
andi temp,0x0f
ori temp,0x30
call lcd_put
mov temp,tbcd1
andi temp,0x0f
ori temp,0x30
call lcd_put
mov temp,tbcd0
swap temp
andi temp,0x0f
ori temp,0x30
call lcd_put
ret

adv:
lds temp,flag_reg2
sbrs temp,scr_rot
ret
cbr temp,0x02
sts flag_reg2,temp
ldi temp,40
call lcd_gotoxy_80
ldi temp4,20
lds zh,ptr_rot
lds zl,ptr_rot+1
rot:
lpm temp,z+
tst temp
brne rot_ok
	ldi 	zh, high(table_janos*2)
	ldi 	zl, low(table_janos*2)
	sts ptr_rot,zh
	sts ptr_rot+1,zl
lpm temp,z
rot_ok:
call lcd_put
dec temp4
tst temp4
brne rot
lds zh,ptr_rot
lds zl,ptr_rot+1
adiw z,1
sts ptr_rot,zh
sts ptr_rot+1,zl
ret

Lcd_gotoxy_80:
cpi temp,20
brsh writeln_2
ori temp,0x80
call lcd_cmd
ret
writeln_2:
cpi temp,40
brsh writeln_3
subi temp,20
ori temp,0xc0 
call lcd_cmd
ret
writeln_3:
cpi temp,60
brsh writeln_4
subi temp,40
ldi temp2,148
add temp,temp2
call lcd_cmd
ret
writeln_4:
subi temp,60
ldi temp2,212
add temp,temp2
call lcd_cmd
ret

Lcd_ram_msg_put:				;rutina de incarcare sir de caractere din memorie
ldi temp2,20
Lcd_ram_next_char:				;adresare indirecta prin reg. pointer Z(ZL si ZH)
	ld		temp,y+				;incarc in reg R0 caracterul
	tst 	temp				;verific pt caracterul de sfarsit 0h
	breq	lcd_ram_out			;daca caract este  0h at iesire din rutina	
	call	Lcd_put			;salt la rutina de afisare pe Lcd 
	dec temp2
	tst temp2
	breq lcd_ram_out
	jmp	Lcd_ram_next_char	;salt pentru urmatorul caracter
lcd_ram_out:
ret

Lcd_msg_put:				;rutina de incarcare sir de caractere din memorie
Lcd_next_char:				;adresare indirecta prin reg. pointer Z(ZL si ZH)
	lpm		temp,z+				;incarc in reg R0 caracterul
	tst 	temp				;verific pt caracterul de sfarsit 0h
	breq	lcd_out			;daca caract este  0h at iesire din rutina	
	call	Lcd_put			;salt la rutina de afisare pe Lcd 
	jmp	Lcd_next_char	;salt pentru urmatorul caracter
lcd_out:
ret
Lcd_put:					;rutina de afisare pe Lcd  
	push    temp            ;salvez octetul pe stiva
call lcd_wrdb
    pop     temp            ;citesc caracterul de pe stiva
	swap	temp
call lcd_wrdb
    call   Lcd_wait  
ret
lcd_wrdb:
andi    temp,0xf0       ;sterg  [HIGH NIBBLE]
in		temp3,portb
andi	temp3,0x0f
or		temp3,temp
out     portb,temp3      	;pun pe port octetul
;*************************** lcd overide !!!!
    sbi     portb,lcdrs     ;activez receptia in DR [Data Register] Lcd
nop
nop
togle_lcde:
    sbi    	portb,lcde      ;activez transmisia
	nop
	nop
	nop
	nop
    cbi     portb,lcde
ret 
lcd_init: 
call wait_lcd       
	ldi 	temp,0x30			;incarc in reg. temp 3 
call lcd_wrcmd_1
call lcd_wait 
    ldi     temp,0b00101100 ;setare parametrii:  16 caractere, 2 linii, 5x7 font 
    call   Lcd_cmd			;salt la rutina trimitere comanda spre Lcd
    ldi     temp,0b00101100 ;setare parametrii:  16 caractere, 2 linii, 5x7 font 
    call   Lcd_cmd			;salt la rutina trimitere comanda spre Lcd
    ldi     temp,0b00000110 ;setare parametrii:incrementare adresa DD RAM, scroll inactiv 
    call   Lcd_cmd
	ldi     temp,0b00001100 ;Display activ, cursor inactiv, blink inactiv
    call   Lcd_cmd
	call	lcd_clr			;Display clear
ret
lcd_wrcmd:
andi    temp,0xf0  
lcd_wrcmd_1:
in		temp3,portb
andi	temp3,0x0f
or		temp3,temp
out     portb,temp3 
cbi     portb,lcdrs
call togle_lcde
ret
Lcd_cmd:
	push    temp      
call lcd_wrcmd
	pop     temp
	swap	temp            
call lcd_wrcmd	
call lcd_wait						
ret
Lcd_clr: 
	ldi 	temp,1
	call 	Lcd_cmd
call wait_1_52ms
ret
Lcd_bnk: 
	ldi 	temp,0b00001101
	call 	Lcd_cmd
ret
Lcd_nbnk: 
	ldi 	temp,0b00001100
	call 	Lcd_cmd
ret
wait_1_52ms:
; Assembly code auto-generated
; by utility from Bret Mulvey
; Delay 59 996 cycles
; 2ms 999us 800 ns at 20 MHz
push r18
push r19
    ldi  r18, 78
    ldi  r19, 234
L3ms: dec  r19
    brne L3ms
    dec  r18
    brne L3ms
    nop
pop r19
pop r18
ret
Lcd_wait:
; 40us at 20 MHz
; by utility from Bret Mulvey
; Delay 796 cycles
; 39us 800 ns at 20 MHz
push r18
push r19
    ldi  r18, 2
    ldi  r19, 7
L40: dec  r19
    brne L40
    dec  r18
    brne L40
pop r19
pop r18
ret
wait_lcd:
; 65ms at 20 MHz
; by utility from Bret Mulvey
; Delay 1 299 996 cycles
; 64ms 999us 800 ns at 20 MHz
push r18
push r19
push r20
    ldi  r18, 7
    ldi  r19, 153
    ldi  r20, 72
L65: dec  r20
    brne L65
    dec  r19
    brne L65
    dec  r18
    brne L65
pop r20
pop r19
pop r18
ret