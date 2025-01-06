/*
 * adc.inc
 *
 *  Created: 12/5/2024 9:11:09 AM
 *   Author: janos
 */ 
/*
adc_debug:
ldi temp,36
call lcd_gotoxy_80
lds fbin0,radcl
lds fbin1,radch
clr fbin2
call bin3bcd
call dig04_disp
ret
*/

adc_print_scr:
ldi temp,60
call lcd_gotoxy_80

lds temp,flag_reg0
sbrs temp,ch0_ef
jmp ch0_in_range
	ldi 	zh, high(table_chx_err*2)
	ldi 	zl, low(table_chx_err*2)
	call 	Lcd_msg_put
jmp ch1_dsp
ch0_in_range:
lds temp,ch0_sram	
call lcd_put
lds temp,ch0_sram+1
call lcd_put
ch1_dsp:
ldi temp,76
call lcd_gotoxy_80

lds temp,flag_reg0
sbrs temp,ch1_ef
jmp ch1_in_range
	ldi 	zh, high(table_chx_err*2)
	ldi 	zl, low(table_chx_err*2)
	call 	Lcd_msg_put
jmp dia_dsp
ch1_in_range:
lds temp,ch1_sram
call lcd_put
lds temp,ch1_sram+1
call lcd_put
dia_dsp:
ldi temp,44
call lcd_gotoxy_80

lds temp,flag_reg0
sbrs temp,dia_ef
jmp dia_in_range
	ldi 	zh, high(table_dia_err*2)
	ldi 	zl, low(table_dia_err*2)
	call 	Lcd_msg_put
ret
dia_in_range:
lds temp,dia_sram
call lcd_put
lds temp,dia_sram+1
call lcd_put
lds temp,dia_sram+2
call lcd_put
lds temp,dia_sram+3
call lcd_put
lds temp,dia_sram+4
call lcd_put
ret

adc_calc:
lds temp,flag_reg2
sbrs temp,adc_frf
ret
cbr temp,0x40
sts flag_reg2,temp
call dia_act
ch0_calc:				;adc1 = ch0
ldi yl,low(1023)
ldi yh,high(1023)
lds zl,ch0_adc
lds zh,ch0_adc+1
cp zl,yl
cpc zh,yh
brlo val_okch1
lds temp,flag_reg0
sbr temp,0x20
sts flag_reg0,temp
ret
val_okch1:
lds temp,flag_reg0
cbr temp,0x20
sts flag_reg0,temp
mov b0,zl
mov b1,zh
clr b2
clr b3
clr b4
clr b5
clr b6
clr b7
ldi a0,low(4882)
ldi a1,high(4882)
ldi a2,0x00
ldi a3,0x00
ldi a4,0x00
ldi a5,0x00
ldi a6,0x00
ldi a7,0x00
call __muldi3
ldi zl,low(4990)
ldi zh,high(4990)
mov b0,zl
mov b1,zh
clr b2
clr b3
clr b4
clr b5
clr b6
clr b7
call __udivdi3
mov fbin0,a0
mov fbin1,a1
mov fbin2,a2
call bin3bcd
mov temp,tbcd1
andi temp,0x0f
ori temp,0x30
sts ch0_sram,temp
mov temp,tbcd0
swap temp
andi temp,0x0f
ori temp,0x30
sts ch0_sram+1,temp
clr temp
sts ch0_sram+2,temp
ch1_calc:					;adc2 = ch1
ldi yl,low(1023)
ldi yh,high(1023)
lds zl,ch1_adc
lds zh,ch1_adc+1
cp zl,yl
cpc zh,yh
brlo val_okch2
lds temp,flag_reg0
sbr temp,0x10
sts flag_reg0,temp
ret
val_okch2:
lds temp,flag_reg0
cbr temp,0x10
sts flag_reg0,temp
mov b0,zl
mov b1,zh
clr b2
clr b3
clr b4
clr b5
clr b6
clr b7
ldi a0,low(4882)
ldi a1,high(4882)
ldi a2,0x00
ldi a3,0x00
ldi a4,0x00
ldi a5,0x00
ldi a6,0x00
ldi a7,0x00
call __muldi3
ldi zl,low(4990)
ldi zh,high(4990)
mov b0,zl
mov b1,zh
clr b2
clr b3
clr b4
clr b5
clr b6
clr b7
call __udivdi3
mov fbin0,a0
mov fbin1,a1
mov fbin2,a2
call bin3bcd
mov temp,tbcd1
andi temp,0x0f
ori temp,0x30
sts ch1_sram,temp
mov temp,tbcd0
swap temp
andi temp,0x0f
ori temp,0x30
sts ch1_sram+1,temp
clr temp
sts ch1_sram+2,temp
dia_calc:						;adc0 = dia
;call adc_debug
;ldi xl,low(174)
;ldi xh,high(174)
ldi yl,low(850)
ldi yh,high(850)
lds zl,dia_adc
lds zh,dia_adc+1
;cp zl,xl
;cpc zh,xh
;brsh val_ok
;clr fbin0
;clr fbin1
;clr fbin2
;jmp val_mi
;val_ok:
cp zl,yl
cpc zh,yh
brlo val_ok2
lds temp,flag_reg0
sbr temp,0x08
sts flag_reg0,temp
ret
val_ok2:
lds temp,flag_reg0
cbr temp,0x08
sts flag_reg0,temp
mov b0,zl
mov b1,zh
clr b2
clr b3
clr b4
clr b5
clr b6
clr b7
ldi a0,low(4882)
ldi a1,high(4882)
ldi a2,0x00
ldi a3,0x00
ldi a4,0x00
ldi a5,0x00
ldi a6,0x00
ldi a7,0x00
call __muldi3
;ldi xl,0x18 ;2v
;ldi xh,0xd0
;ldi zl,0x0c
;ldi xl,0xfa ;1v
;ldi xh,0x54
;ldi zl,0x06
;clc
;clt
;sub a0,xl
;sbc a1,xh
;sbc a2,zl
ldi zl,low(1597)
ldi zh,high(1597)
mov b0,zl
mov b1,zh
clr b2
clr b3
clr b4
clr b5
clr b6
clr b7
call __udivdi3
mov fbin0,a0
sts dia_val,a0
mov fbin1,a1
sts dia_val+1,a1
mov fbin2,a2
call bin3bcd
mov temp,tbcd1
swap temp
andi temp,0x0f
ori temp,0x30
sts dia_sram,temp
mov temp,tbcd1
andi temp,0x0f
ori temp,0x30
sts dia_sram+1,temp
ldi temp,'.'
sts dia_sram+2,temp
mov temp,tbcd0
swap temp
andi temp,0x0f
ori temp,0x30
sts dia_sram+3,temp
mov temp,tbcd0
andi temp,0x0f
ori temp,0x30
sts dia_sram+4,temp
clr temp
sts dia_sram+5,temp
ret
