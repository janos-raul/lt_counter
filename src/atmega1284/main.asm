;
; lt_counter_v4.asm
;
; Created: 12/5/2024 8:40:39 AM
; Author : janos
;
;.include "m1284def.inc" ; 20 Mhz crystal osc. 
.include "reg_def.inc"

.dseg
.org sram_start
flag_reg0:	.byte 1
flag_reg1:	.byte 1
flag_reg2:	.byte 1
flag_reg3:	.byte 1
ptr_rot:	.byte 2
cnt0:		.byte 1 ;internal clock 1000ms
cnt1:		.byte 1 ;key buz interval
cnt2:		.byte 1 ;adc calc refresh 400ms
cnt3:		.byte 1 ;buzzer 
cnt4:		.byte 1 ;key debounce 240ms
cnt5:		.byte 1 ;rel_ delay(1000ms)
cnt6:		.byte 1 ;adc screen refresh 480ms
sec_:		.byte 1
min_:		.byte 1
hour:		.byte 1
dia_adc:	.byte 2
ch0_adc:	.byte 2
ch1_adc:	.byte 2

oc1_val:	.byte 3
oc1_bcd_val:	.byte 3
oc2_val:	.byte 3
oc2_bcd_val:	.byte 3

m_set6:		.byte 1
menu_index: 	.byte 1

ppm24:		.byte 3
ppm24_bcd:	.byte 3
dia_100:	.byte 1
dia_llm:	.byte 2
dia_hlm:	.byte 2
dia_cmp:	.byte 2
dia_cmp_bcd: 	.byte 6
dia_val:	.byte 2

enc_cnt:	.byte 3
met_cnt:	.byte 3
speed_cnt:	.byte 2
scr_index:	.byte 1
rx0_buffer: 	.byte 65
rx0_ptr:	.byte 2
rx0_err_cnt:	.byte 1
rx0_len:	.byte 1
rx0_crc8:	.byte 1
tx0_buffer: 	.byte 65
tx0_ptr:	.byte 2
ch0_sram:	.byte 3
ch1_sram:	.byte 3
dia_sram:	.byte 6
met_sram:	.byte 7
time_sram:	.byte 9
date_sram:	.byte 6
speed_sram: 	.byte 4
type_sram:  	.byte 12 ;"MYUP-2x0.75",0x00
type_index: 	.byte 1
color_sram: 	.byte 8  ;"Y/GREEN",0x00
color_index:	.byte 1
l3live_sram:	.byte 7
l3live_start_sram:	.byte 12
l3live_stop_sram:	.byte 11
ip_sram:	.byte 16
temp_sram:	.byte 5
hum_sram:	.byte 5
jdy_channel: 	.byte 1
jdy_txpower: 	.byte 1

.MACRO delayms20 
	ldi temp,@0
delay_loopms:
	call wait_20ms
	dec temp
	tst temp
brne delay_loopms
.ENDMACRO

.MACRO delay 
	ldi temp,@0
delay_loops:
	call wait_1000ms
	dec temp
	tst temp
brne delay_loops
.ENDMACRO

.cseg
.org 0x0000 					
jmp RESET 
.org INT0addr
jmp int0_isr ; IRQ0  
.org INT1addr
jmp int1_isr ; IRQ1  
;jmp INT2 ; IRQ2 
;jmp PCINT0 ; PCINT0 
;jmp PCINT1 ; PCINT1 
;jmp PCINT2 ; PCINT2  
;jmp PCINT3 ; PCINT3 
;jmp WDT ; Watchdog Timeout 
;jmp TIM2_COMPA ; Timer2 CompareA 
;jmp TIM2_COMPB ; Timer2 CompareB  
.org OVF2addr
jmp tim2_ovf_isr ; Timer2 Overflow  
;jmp TIM1_CAPT ; Timer1 Capture 
;jmp TIM1_COMPA ; Timer1 CompareA 
;jmp TIM1_COMPB ; Timer1 CompareB 
;jmp TIM1_OVF ; Timer1 Overflow 
;.org 0x0020
;jmp TIM0_COMPA ; Timer0 CompareA 
;.org 0x0022
;jmp TIM0_COMPB ; Timer0 CompareB  
;.org 0x0024
;jmp TIM0_OVF ; Timer0 Overflow  
;jmp SPI_STC ; SPI Transfer Complete 
.org URXC0addr
jmp USART0_RXC ; USART0 RX Complete 
.org UDRE0addr
jmp USART0_UDRE ; USART0,UDR Empty 
;.org 0x002A 
;jmp USART0_TXC ; USART0 TX Complete 
;jmp ANA_COMP ; Analog Comparator  
.org ADCCaddr
jmp adc_isr ; ADC Conversion Complete  
;jmp EE_RDY ; EEPROM Ready 
;jmp TWI ; two-wire Serial 
;jmp SPM_RDY ; SPM Ready  
;.org URXC1addr
;jmp USART1_RXC ; USART1 RX Complete  
;.org UDRE1addr
;jmp USART1_UDRE ; USART1,UDR Empty 
;.org UTXC1addr
;jmp USART1_TXC ; USART1 TX Complete  
;jmp TIM3_CAPT ; Timer3 Capture(1)  
;jmp TIM3_COMPA ; Timer3 Compare(1) 
;jmp TIM3_COMPB ; Timer3 CompareB(1)
;jmp TIM3_OVF ; Timer3 Overflow(1) 
;********************* mcu stack init
reset:
	ldi 	temp,low(ramend)
	out 	spl,temp
	ldi 	temp,high(ramend)
	out 	sph,temp
;*********************
; Turn off global interrupt
cli
; Reset Watchdog Timer
wdr
; Clear WDRF in MCUSR
in r16, MCUSR
andi r16, ~(1<<WDRF)
out MCUSR, r16
; Write logical one to WDCE and WDE
; Keep old prescaler setting to prevent unintentional time-out
lds r16, WDTCSR
ori r16, (1<<WDCE) | (1<<WDE)
sts WDTCSR, r16
; Turn off WDT
ldi r16, (0<<WDE)
sts WDTCSR, r16
;*********************
clr temp
ldi yl,low(sram_start)
ldi yh,high(sram_start)
ldi xl,low(ramend)
ldi xh,high(ramend)
wipe_sram0:
st y+,temp
cp yl,xl
cpc yh,xh
brne wipe_sram0
;********************* mcu port init
ldi temp,0b00000000
out ddra,temp
ldi temp,0b11111000
out porta,temp

ldi temp,0b11111111
out ddrb,temp
ldi temp,0b00000001
out portb,temp

ldi temp,0b11110001
out ddrd,temp
ldi temp,0b00001110
out portd,temp

ldi temp,0b11000000
out ddrc,temp
ldi temp,0b00111111
out portc,temp 

;********************* mcu init
;adc_init:
ldi temp,0b01000000 ;adc vref AVCC , adc0 selected !!!
sts admux,temp
ldi temp,0b00000000
sts adcsrb,temp
ldi temp,0b11111111
sts didr0,temp
ldi temp,0b11101111
sts adcsra,temp
;ext int 0,1 init:
ldi temp,0b00000011
out eimsk,temp
ldi temp,0b00001010
sts eicra,temp
;uart0 init:
ldi temp,0			;set uart 0 radio rx
sts ubrr0h,temp
ldi temp,0x20		;set baud rate to 38,400 bps
sts ubrr0l,temp
ldi temp,0b00000110
sts ucsr0c,temp
ldi temp,0b11011000
sts ucsr0b,temp
;timer 0 init:
ldi temp,0b00000000
sts assr,temp
ldi temp,0b00000000
sts tccr2a,temp
ldi temp,0b00000111 ;clk/1024 presc
sts tccr2b,temp
ldi temp,0b00000001
sts timsk2,temp

;********************global var init
pre_main:
call lcd_init
call intro_scr

ldi temp,low(rx0_buffer)
sts rx0_ptr,temp
ldi temp,high(rx0_buffer)
sts rx0_ptr+1,temp

ldi xh,high(table_eeflag0)
ldi xl,low(table_eeflag0)
call eeprom_read
lds temp2,flag_reg3
sbrc temp,7
sbr temp2,0x10
sbrc temp,6
sbr temp2,0x08
sbrc temp,5
sbr temp2,0x04
sts flag_reg3,temp2

ldi xh,high(table_oc1_val)
ldi xl,low(table_oc1_val)
call eeprom_read
sts oc1_val,temp
ldi xh,high(table_oc1_val+1)
ldi xl,low(table_oc1_val+1)
call eeprom_read
sts oc1_val+1,temp
ldi xh,high(table_oc1_val+2)
ldi xl,low(table_oc1_val+2)
call eeprom_read
sts oc1_val+2,temp

ldi xh,high(table_oc2_val)
ldi xl,low(table_oc2_val)
call eeprom_read
sts oc2_val,temp
ldi xh,high(table_oc2_val+1)
ldi xl,low(table_oc2_val+1)
call eeprom_read
sts oc2_val+1,temp
ldi xh,high(table_oc2_val+2)
ldi xl,low(table_oc2_val+2)
call eeprom_read
sts oc2_val+2,temp

ldi xh,high(table_ppm24)
ldi xl,low(table_ppm24)
call eeprom_read
sts ppm24,temp
ldi xh,high(table_ppm24+1)
ldi xl,low(table_ppm24+1)
call eeprom_read
sts ppm24+1,temp
ldi xh,high(table_ppm24+2)
ldi xl,low(table_ppm24+2)
call eeprom_read
sts ppm24+2,temp

ldi xh,high(table_dia100)
ldi xl,low(table_dia100)
call eeprom_read
sts dia_100,temp

ldi temp,'0'
sts date_sram,temp
ldi temp,'1'
sts date_sram+1,temp
ldi temp,'/'
sts date_sram+2,temp
ldi temp,'0'
sts date_sram+3,temp
ldi temp,'1'
sts date_sram+4,temp
ldi temp,'0'
sts speed_sram,temp
sts speed_sram+1,temp
sts speed_sram+2,temp

ldi xh,high(type_sram)
ldi xl,low(type_sram)
ldi zh,high(table_fy15*2)
ldi zl,low(table_fy15*2)
call ld_sram
ldi temp,2
sts type_index,temp

ldi xh,high(color_sram)
ldi xl,low(color_sram)
ldi zh,high(table_red*2)
ldi zl,low(table_red*2)
call ld_sram

ldi xh,high(l3live_sram)
ldi xl,low(l3live_sram)
ldi zh,high(table_l3live*2)
ldi zl,low(table_l3live*2)
call ld_sram

ldi xh,high(l3live_start_sram)
ldi xl,low(l3live_start_sram)
ldi zh,high(table_l3live_start*2)
ldi zl,low(table_l3live_start*2)
call ld_sram

ldi xh,high(l3live_stop_sram)
ldi xl,low(l3live_stop_sram)
ldi zh,high(table_l3live_stop*2)
ldi zl,low(table_l3live_stop*2)
call ld_sram

ldi xh,high(temp_sram)
ldi xl,low(temp_sram)
ldi zh,high(table_temp*2)
ldi zl,low(table_temp*2)
call ld_sram

ldi xh,high(hum_sram)
ldi xl,low(hum_sram)
ldi zh,high(table_hum*2)
ldi zl,low(table_hum*2)
call ld_sram

ldi xh,high(ip_sram)
ldi xl,low(ip_sram)
ldi zh,high(table_ip*2)
ldi zl,low(table_ip*2)
call ld_sram

call met_st
call speed_st

sei							;enable global interupts
	ldi temp,tim2_val
	sts tcnt2,temp

pre_main_loop:

call rx0_recv
call tx0_send

lds temp,flag_reg0
sbrs temp,time_scrf
jmp pre_main_loop
cbr temp,0x04
sts flag_reg0,temp
lds temp,cnt3
inc temp
sts cnt3,temp
cpi temp,4
brne pre_main_loop

lds temp,flag_reg2
sbr temp,0x10			;time synch flag
sts flag_reg2,temp
call lcd_clr
call format_scr


main_loop:

call adc_calc
call adc_refresh_scr
call met_refresh_scr
call time_refresh_scr
call speed_refresh_scr
call reset_scan
call type_scan
call color_scan
call rx0_recv
call tx0_send

call log_scan
call time_synch
call log_scr
call dia_scan

jmp menu_scan

dia_scan:
lds temp,flag_reg1
sbrs temp,key_diaf
ret
cbr temp,0x04
sts flag_reg1,temp

lds temp,flag_reg3
sbrs temp,dia_actf
jmp toggle_dia
cbr temp,0x03		;dia off buz_ms off
sts flag_reg3,temp
ret
toggle_dia:
sbr temp,0x02		;dia on
sts flag_reg3,temp

lds temp,dia_val
sts dia_cmp,temp
lds temp,dia_val+1
sts dia_cmp+1,temp

lds a0,dia_cmp
lds a1,dia_cmp+1
ldi a2,0x00
ldi a3,0x00
ldi a4,0x00
ldi a5,0x00
ldi a6,0x00
ldi a7,0x00
lds temp,dia_100
mov b0,temp
clr b1
clr b2
clr b3
clr b4
clr b5
clr b6
clr b7
call __muldi3
ldi temp,100
mov b0,temp
clr b1
clr b2
clr b3
clr b4
clr b5
clr b6
clr b7
call __udivdi3
lds b0,dia_cmp
lds b1,dia_cmp+1
add b0,a0
adc b1,a1
sts dia_hlm,b0
sts dia_hlm+1,b1
lds b0,dia_cmp
lds b1,dia_cmp+1
sub b0,a0
sbc b1,a1
sts dia_llm,b0
sts dia_llm+1,b1
lds fbin0,dia_cmp
lds fbin1,dia_cmp+1
clr fbin2
call bin3bcd
mov temp,tbcd1
swap temp
andi temp,0x0f
ori temp,0x30
sts dia_cmp_bcd,temp
mov temp,tbcd1
andi temp,0x0f
ori temp,0x30
sts dia_cmp_bcd+1,temp
ldi temp,'.'
sts dia_cmp_bcd+2,temp
mov temp,tbcd0
swap temp
andi temp,0x0f
ori temp,0x30
sts dia_cmp_bcd+3,temp
mov temp,tbcd0
andi temp,0x0f
ori temp,0x30
sts dia_cmp_bcd+4,temp
clr temp
sts dia_cmp_bcd+5,temp
ret

dia_act:
lds temp,flag_reg3
sbrs temp,dia_actf
ret
lds b0,dia_val
lds b1,dia_val+1
lds a0,dia_hlm
lds a1,dia_hlm+1
cp b0,a0
cpc b1,a1
brlo dia_okh
;**************************
sbi portd,led_red
lds temp,flag_reg3
sbr temp,0x01
sts flag_reg3,temp
ret
dia_okh:
lds b0,dia_val
lds b1,dia_val+1
lds a0,dia_llm
lds a1,dia_llm+1
cp b0,a0
cpc b1,a1
brge dia_okl
sbi portd,led_red
lds temp,flag_reg3
sbr temp,0x01
sts flag_reg3,temp
ret
dia_okl:
cbi portd,led_red
lds temp,flag_reg3
cbr temp,0x01
sts flag_reg3,temp
cbi portd,buz_
ret

log_scr:
lds temp,flag_reg3
sbrs temp,log_state
ret

lds temp,flag_reg2
sbrs temp,scr_rot
ret
cbr temp,0x02
sts flag_reg2,temp

lds temp,scr_index
inc temp
cpi temp,5
brne scr_indexok
ldi temp,1
scr_indexok:
sts scr_index,temp
cpi temp,1
breq chr1
cpi temp,2
breq chr2
cpi temp,3
breq chr3
cpi temp,4
breq chr4
ret

chr1:
jmp chr1_hndlr
chr2:
jmp chr2_hndlr
chr3:
jmp chr3_hndlr
chr4:
jmp chr4_hndlr

chr1_hndlr:
ldi temp,6
call lcd_gotoxy_80
ldi temp,'-'
call lcd_put
ldi temp,13
call lcd_gotoxy_80
ldi temp,'-'
call lcd_put
ret
chr2_hndlr:
ldi temp,6
call lcd_gotoxy_80
ldi temp,'/'
call lcd_put
ldi temp,13
call lcd_gotoxy_80
ldi temp,'/'
call lcd_put
ret
chr3_hndlr:
ldi temp,6
call lcd_gotoxy_80
ldi temp,'|'
call lcd_put
ldi temp,13
call lcd_gotoxy_80
ldi temp,'|'
call lcd_put
ret
chr4_hndlr:
ldi temp,6
call lcd_gotoxy_80
ldi temp,0x5c
call lcd_put
ldi temp,13
call lcd_gotoxy_80
ldi temp,0x5c
call lcd_put
ret

menu_scan:
lds temp,flag_reg1
sbrs temp,key_menuf
jmp main_loop
cbr temp,0x80
sts flag_reg1,temp

menu_1:
call lcd_clr
	ldi 	zh, high(table_menu_radio*2)
	ldi 	zl, low(table_menu_radio*2)
	call 	Lcd_msg_put
ldi temp,20
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_display*2)
	ldi 	zl, low(table_menu_display*2)
	call 	Lcd_msg_put
ldi temp,40
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_pulse*2)
	ldi 	zl, low(table_menu_pulse*2)
	call 	Lcd_msg_put
ldi temp,60
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_limit*2)
	ldi 	zl, low(table_menu_limit*2)
	call 	Lcd_msg_put
ldi temp,0
call lcd_gotoxy_80
call lcd_bnk
ldi temp,1
sts menu_index,temp
call key_clr
menu1_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp get_menu1
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
jmp set_m1up
sbrc temp,key_downf
jmp set_m1dn
jmp menu1_loop

set_m1up:
call key_clr
lds temp,menu_index
inc temp
sts menu_index,temp

cpi temp,5
breq menu_2
call menu_lnbnk

jmp menu1_loop

set_m1dn:
call key_clr
lds temp,menu_index
dec temp
sts menu_index,temp

tst temp
breq menu_1reset
call menu_lnbnk

jmp menu1_loop
menu_1reset:
ldi temp,1
sts menu_index,temp
jmp menu1_loop
menu_2:
call lcd_clr
	ldi 	zh, high(table_menu_oc1*2)
	ldi 	zl, low(table_menu_oc1*2)
	call 	Lcd_msg_put
ldi temp,20
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_oc2*2)
	ldi 	zl, low(table_menu_oc2*2)
	call 	Lcd_msg_put
ldi temp,40
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_sinfo*2)
	ldi 	zl, low(table_menu_sinfo*2)
	call 	Lcd_msg_put
ldi temp,60
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_info*2)
	ldi 	zl, low(table_menu_info*2)
	call 	Lcd_msg_put
ldi temp,0
call lcd_gotoxy_80
call lcd_bnk
ldi temp,1
sts menu_index,temp
call key_clr
menu2_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp get_menu2
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
jmp set_m2up
sbrc temp,key_downf
jmp set_m2dn
jmp menu2_loop

set_m2up:
call key_clr
lds temp,menu_index
inc temp
sts menu_index,temp
cpi temp,5
breq menu_2reset

call menu_lnbnk
jmp menu2_loop

set_m2dn:
call key_clr
lds temp,menu_index
dec temp
sts menu_index,temp
tst temp
breq menu_1jmp

call menu_lnbnk
jmp menu2_loop
menu_1jmp:
jmp menu_1
menu_2reset:
ldi temp,4
sts menu_index,temp
jmp menu2_loop
;*****************************************
get_menu1:
lds temp,menu_index
cpi temp,1
breq subm_1
cpi temp,2
breq subm_2
cpi temp,3
breq subm_3
cpi temp,4
breq subm_4
jmp menu_exit
subm_1:
jmp sm_radio
subm_2:
jmp sm_display
subm_3:
jmp sm_pulse
subm_4:
jmp sm_limit
;*****************************************
jmp menu_exit
;*****************************************
get_menu2:
lds temp,menu_index
cpi temp,1
breq subm_21
cpi temp,2
breq subm_22
cpi temp,3
breq subm_23
cpi temp,4
breq subm_24
jmp menu_exit
subm_21:
jmp sm_oc1
subm_22:
jmp sm_oc2
subm_23:
jmp sm_srv_inf
subm_24:
jmp sm_dev_inf
;*****************************************
;jmp menu_exit
;*****************************************

menu_exit:
call key_clr
call lcd_nbnk
call lcd_clr
call format_scr
call met_scr
ldi yh,high(type_sram)
ldi yl,low(type_sram)
call type_dsp
ldi yh,high(color_sram)
ldi yl,low(color_sram)
call color_dsp
jmp main_loop

background_tasks:
call adc_calc
call met_calc
call speed_calc
call rx0_recv
call tx0_send
ret


menu_lnbnk:
lds temp,menu_index
cpi temp,1
breq set_ln1
cpi temp,2
breq set_ln2
cpi temp,3
breq set_ln3
cpi temp,4
breq set_ln4
set_ln1:
ldi temp,0
call lcd_gotoxy_80
ret
set_ln2:
ldi temp,20
call lcd_gotoxy_80
ret
set_ln3:
ldi temp,40
call lcd_gotoxy_80
ret
set_ln4:
ldi temp,60
call lcd_gotoxy_80
ret

trap:
nop
jmp trap
ret

key_clr:
lds temp,flag_reg1
cbr temp,0xfc
sts flag_reg1,temp
ret

log_scan:
lds temp,flag_reg1
sbrs temp,key_startf
ret
cbr temp,0x08
sts flag_reg1,temp

ldi temp,6
call lcd_gotoxy_80
ldi temp,'-'
call lcd_put
ldi temp,13
call lcd_gotoxy_80
ldi temp,'-'
call lcd_put

lds temp,flag_reg3
sbrs temp,log_state
jmp toggle_log
cbr temp,0x40		;send log finished
jmp log_toggled
toggle_log:
sbr temp,0x40		;send log started
log_toggled:
sbr temp,0x80
sts flag_reg3,temp
ret


time_synch:
lds temp,flag_reg2
sbrs temp,tsynch_f
ret
cbr temp,0x10
sts flag_reg2,temp

lds temp,flag_reg2
sbrs temp,server_alivef		;if server alive then time synch
ret

ldi xl,low(time_sram)
ldi xh,high(time_sram)
ld temp,x+
andi temp,0x0f
swap temp
ld temp2,x+
andi temp2,0x0f
or temp,temp2
mov fbcd0,temp
clr fbcd1
clr fbcd2
clr fbcd3
call bcd3bin
sts hour,tbin0
adiw x,1
ld temp,x+
andi temp,0x0f
swap temp
ld temp2,x+
andi temp2,0x0f
or temp,temp2
mov fbcd0,temp
clr fbcd1
clr fbcd2
clr fbcd3
call bcd3bin
sts min_,tbin0
adiw x,1
ld temp,x+
andi temp,0x0f
swap temp
ld temp2,x
andi temp2,0x0f
or temp,temp2
mov fbcd0,temp
clr fbcd1
clr fbcd2
clr fbcd3
call bcd3bin
sts sec_,tbin0
ret

/*
jdy_set:
cbi portb,radio_set
delayms20 20
ldi zl,byte3(table_radio_cfg*2)
out RAMPZ,ZL
ldi zh,high(table_radio_cfg*2)
ldi zl,low(table_radio_cfg*2)
ldi yh,high(tx0_buffer)
ldi yl,low(tx0_buffer)
tx0_loop:
elpm temp,z+
st y+,temp
cpi temp,0xff
brne tx0_loop
call tx0_send_buf
delayms20 20
sbi portb,radio_set
ret
*/

jdy_tx_read:
ldi zh,high(table_radio_read_cfg*2)
ldi zl,low(table_radio_read_cfg*2)
ldi yh,high(tx0_buffer)
ldi yl,low(tx0_buffer)
tx_read_jdy_loop:
elpm temp,z+
st y+,temp
cpi temp,0xff
brne tx_read_jdy_loop
call tx0_send_buf
ldi temp,low(rx0_buffer)
sts rx0_ptr,temp
ldi temp,high(rx0_buffer)
sts rx0_ptr+1,temp
ret

tx0_send:
lds temp,flag_reg1
sbrs temp,rx0_data
ret
cbr temp,0x01
sts flag_reg1,temp

cbi portd,led_red

ldi zl,low(tx0_buffer)
ldi zh,high(tx0_buffer)

lds temp,flag_reg3
sbrs temp,log_send
jmp send_ord
cbr temp,0x80
sts flag_reg3,temp
sbrs temp,log_state
jmp send_stop_log
ldi xh,high(l3live_start_sram)
ldi xl,low(l3live_start_sram)
jmp send_log
send_stop_log:
ldi xh,high(l3live_stop_sram)
ldi xl,low(l3live_stop_sram)
jmp send_log
send_ord:
ldi xh,high(l3live_sram)
ldi xl,low(l3live_sram)
send_log:
call copy_sram
sbiw z,1
ldi temp,','
st z+,temp
ldi xh,high(type_sram)
ldi xl,low(type_sram)
call copy_sram
sbiw z,1
ldi temp,','
st z+,temp
ldi xh,high(color_sram)
ldi xl,low(color_sram)
call copy_sram
sbiw z,1
ldi temp,','
st z+,temp
ldi xh,high(met_sram)
ldi xl,low(met_sram)
call copy_sram
sbiw z,1
ldi temp,','
st z+,temp
ldi xh,high(speed_sram)
ldi xl,low(speed_sram)
call copy_sram
sbiw z,1
ldi temp,','
st z+,temp
ldi xh,high(dia_sram)
ldi xl,low(dia_sram)
call copy_sram
sbiw z,1
ldi temp,','
st z+,temp
ldi xh,high(ch1_sram)
ldi xl,low(ch1_sram)
call copy_sram
sbiw z,1
ldi temp,','
st z+,temp
ldi xh,high(ch0_sram)
ldi xl,low(ch0_sram)
call copy_sram
sbiw z,1
ldi temp,','
st z+,temp
clr temp
st z,temp
clr crc8
ldi poly,0x07
ldi zl,low(tx0_buffer)
ldi zh,high(tx0_buffer)
tx0_crc_loop:
ld data,z+
tst data
breq tx0_crc_loop_exit
call crc8_calc
jmp tx0_crc_loop
tx0_crc_loop_exit:
mov hex_byte,temp
call hex_to_ascii
sbiw z,1
st z+,high_nibble
st z+,low_nibble
ldi temp,0x0d
st z+,temp
ldi temp,0x0a
st z+,temp
ldi temp,0xff
st z,temp
call tx0_send_buf
ret

rx0_recv_raw:
lds temp,flag_reg0
sbrs temp,rx0_int
ret
cbr temp,0x80
sts flag_reg0,temp

ldi zl,low(rx0_buffer)
ldi zh,high(rx0_buffer)
ld temp,z+
cpi temp,0xAA
breq jdy_ok
ret
jdy_ok:
ld temp,z+
cpi temp,0xE2
breq jdy_okk
ret
jdy_okk:
adiw z,1
ld temp,z+
sts jdy_channel,temp
ld temp,z
sts jdy_txpower,temp

lds temp,flag_reg1
sbr temp,0x01			;data valid  send response
sts flag_reg1,temp
ret

rx0_recv:
lds temp,flag_reg0
sbrs temp,rx0_int
ret
cbr temp,0x80
sts flag_reg0,temp

ldi zl,low(rx0_buffer)
ldi zh,high(rx0_buffer)
rx0_loop:
ld temp,z+
tst temp
brne rx0_loop
sbiw z,3
ld temp,z+
ld temp2,z
call ascii_to_hex_byte
sts rx0_crc8,temp
ldi zl,low(rx0_buffer)
ldi zh,high(rx0_buffer)
rx0_loop1:
ld temp,z+
tst temp
brne rx0_loop1
sbiw z,3
clr temp
st z,temp
clr crc8
ldi poly,0x07
ldi zl,low(rx0_buffer)
ldi zh,high(rx0_buffer)
rx0_loop2:
ld data,z+
tst data
breq rx0_loop2_exit
call crc8_calc
jmp rx0_loop2
rx0_loop2_exit:
lds temp2,rx0_crc8
cp crc8,temp2
breq data_crc8
sbi portd,led_red
ret
data_crc8:
ldi zl,low(rx0_buffer+4)
ldi zh,high(rx0_buffer+4)
ld temp,z
cpi temp,'3'
breq l3live
ret
l3live:
ldi zl,low(rx0_buffer+6)
ldi zh,high(rx0_buffer+6)
ldi xl,low(date_sram)
ldi xh,high(date_sram)
ldi temp2,5
date_sloop:
ld temp,z+
st x+,temp
dec temp2
tst temp2
brne date_sloop
st x,temp2
adiw z,1
ldi zl,low(rx0_buffer+12)
ldi zh,high(rx0_buffer+12)
ldi xl,low(time_sram)
ldi xh,high(time_sram)
ldi temp2,8
time_sloop:
ld temp,z+
st x+,temp
dec temp2
tst temp2
brne time_sloop
st x,temp2
adiw z,1
ldi zl,low(rx0_buffer+21)
ldi zh,high(rx0_buffer+21)
ldi xl,low(ip_sram)
ldi xh,high(ip_sram)
ldi temp2,','
ip_sloop:
ld temp,z+
st x+,temp
cp temp,temp2
brne ip_sloop
clr temp
st -x,temp
ldi xl,low(temp_sram)
ldi xh,high(temp_sram)
ldi temp2,4
temp_sloop:
ld temp,z+
st x+,temp
dec temp2
tst temp2
brne temp_sloop
st x,temp2
adiw z,1
ldi xl,low(hum_sram)
ldi xh,high(hum_sram)
ldi temp2,4
hum_sloop:
ld temp,z+
st x+,temp
dec temp2
tst temp2
brne hum_sloop
st x,temp2

lds temp,flag_reg1
sbr temp,0x01			;data valid  send response
sts flag_reg1,temp

lds temp,flag_reg2
sbr temp,0x08			;server alive flag
sts flag_reg2,temp
ret

ld_sram:
lpm temp,z+
st x+,temp
tst temp
brne ld_sram
ret

copy_sram:
ld temp,x+
st z+,temp
tst temp
brne copy_sram
ret

reset_scan:
lds temp,flag_reg1
sbrs temp,key_exitf
ret
cbr temp,0x10
sts flag_reg1,temp
clr temp
sts enc_cnt,temp
sts enc_cnt+1,temp
sts enc_cnt+2,temp
sts met_cnt,temp
sts met_cnt+1,temp
sts met_cnt+2,temp
call met_scr
ret

speed_refresh_scr:
lds temp,flag_reg2
sbrs temp,speed_scrf
ret
cbr temp,0x80
sts flag_reg2,temp
ldi temp,66
call lcd_gotoxy_80
lds fbin0,speed_cnt
lds fbin1,speed_cnt+1
clr fbin2
call bin3bcd
call dig03_disp
jmp speed_st

speed_calc:
lds temp,flag_reg2
sbrs temp,speed_scrf
ret
cbr temp,0x80
sts flag_reg2,temp
speed_st:
lds fbin0,speed_cnt
lds fbin1,speed_cnt+1
clr fbin2
call bin3bcd
mov temp,tbcd1
andi temp,0x0f
ori temp,0x30
sts speed_sram,temp
mov temp,tbcd0
swap temp
andi temp,0x0f
ori temp,0x30
sts speed_sram+1,temp
mov temp,tbcd0
andi temp,0x0f
ori temp,0x30
sts speed_sram+2,temp
clr temp
sts speed_sram+3,temp
sts speed_cnt,temp
sts speed_cnt+1,temp
ret


time_refresh_scr:
lds temp,flag_reg0
sbrs temp,time_scrf
ret
cbr temp,0x04
sts flag_reg0,temp
call time_dsp0

lds temp,flag_reg3
sbrc temp,dsp_datef
call date_dsp0
sbrc temp,dsp_tempf
call temp_dsp0
sbrc temp,dsp_humf
call hum_dsp0
ret

met_refresh_scr:
lds temp,flag_reg0
sbrs temp,cnt_int
ret
cbr temp,0x40
sts flag_reg0,temp
met_scr:
ldi temp,7
call lcd_gotoxy_80
lds fbin0,met_cnt
lds fbin1,met_cnt+1
lds fbin2,met_cnt+2
call bin3bcd
call dig06_disp
jmp met_st

met_calc:
lds temp,flag_reg0
sbrs temp,cnt_int
ret
cbr temp,0x40
sts flag_reg0,temp
met_st:
lds fbin0,met_cnt
lds fbin1,met_cnt+1
lds fbin2,met_cnt+2
call bin3bcd
mov temp,tbcd2
swap temp
andi temp,0x0f
ori temp,0x30
sts met_sram,temp
mov temp,tbcd2
andi temp,0x0f
ori temp,0x30
sts met_sram+1,temp
mov temp,tbcd1
swap temp
andi temp,0x0f
ori temp,0x30
sts met_sram+2,temp
mov temp,tbcd1
andi temp,0x0f
ori temp,0x30
sts met_sram+3,temp
mov temp,tbcd0
swap temp
andi temp,0x0f
ori temp,0x30
sts met_sram+4,temp
mov temp,tbcd0
andi temp,0x0f
ori temp,0x30
sts met_sram+5,temp
clr temp
sts met_sram+6,temp

lds temp,met_cnt
lds temp2,met_cnt+1
lds temp3,met_cnt+2
lds temp4,oc1_val
lds temp5,oc1_val+1
lds temp6,oc1_val+2
cp temp3,temp6
brne no_out0
cp temp5,temp2
brne no_out0
cp temp4,temp
brne no_out0
clr temp
sts met_cnt,temp
sts met_cnt+1,temp
sts met_cnt+2,temp
lds temp,flag_reg2
sbr temp,0x01
sts flag_reg2,temp
sbi portc,out0
no_out0:

lds temp,met_cnt
lds temp2,met_cnt+1
lds temp3,met_cnt+2
lds temp4,oc2_val
lds temp5,oc2_val+1
lds temp6,oc2_val+2
cp temp3,temp6
brne no_out1
cp temp5,temp2
brne no_out1
cp temp4,temp
brne no_out1
clr temp
sts met_cnt,temp
sts met_cnt+1,temp
sts met_cnt+2,temp
lds temp,flag_reg2
sbr temp,0x01
sts flag_reg2,temp
sbi portc,out1
no_out1:
ret

adc_refresh_scr:
lds temp,flag_reg0
sbrs temp,adc_scrf
ret
cbr temp,0x02
sts flag_reg0,temp
call adc_print_scr

lds temp,flag_reg3
sbrs temp,dia_actf
jmp dia_off
ldi temp,53
call lcd_gotoxy_80
ldi temp,0x1d
call lcd_put
ldi yl,low(dia_cmp_bcd)
ldi yh,high(dia_cmp_bcd)
call lcd_ram_msg_put
ldi temp,0x1d
call lcd_put
ret
dia_off:
ldi temp,53
call lcd_gotoxy_80
	ldi 	zh, high(table_dia_off*2)
	ldi 	zl, low(table_dia_off*2)
	call 	Lcd_msg_put
ret

intro_scr:
ldi temp,0
call lcd_gotoxy_80
	ldi 	zh, high(table_lt_cnt*2)
	ldi 	zl, low(table_lt_cnt*2)
	call 	Lcd_msg_put
ldi temp,20
call lcd_gotoxy_80
	ldi 	zh, high(table_lt_cnt2*2)
	ldi 	zl, low(table_lt_cnt2*2)
	call 	Lcd_msg_put
ldi temp,40
call lcd_gotoxy_80
	ldi 	zh, high(table_lt_inf*2)
	ldi 	zl, low(table_lt_inf*2)
	call 	Lcd_msg_put
ldi temp,60
call lcd_gotoxy_80
	ldi 	zh, high(table_lt_egr*2)
	ldi 	zl, low(table_lt_egr*2)
	call 	Lcd_msg_put
	ret

format_scr:
ldi temp,0
call lcd_gotoxy_80
	ldi 	zh, high(table_format_scr*2)
	ldi 	zl, low(table_format_scr*2)
	call 	Lcd_msg_put
ldi temp,20
call lcd_gotoxy_80
	ldi 	zh, high(table_fy15*2)
	ldi 	zl, low(table_fy15*2)
	call 	Lcd_msg_put
ldi temp,33
call lcd_gotoxy_80
	ldi 	zh, high(table_red*2)
	ldi 	zl, low(table_red*2)
	call 	Lcd_msg_put
ldi temp,69
call lcd_gotoxy_80
	ldi 	zh, high(table_format_line*2)
	ldi 	zl, low(table_format_line*2)
	call 	Lcd_msg_put
adc_format_scr:
ldi temp,62
call lcd_gotoxy_80
ldi temp,'%'
call lcd_put
ldi temp,'A'
call lcd_put
ldi temp,78
call lcd_gotoxy_80
ldi temp,'%'
call lcd_put
ldi temp,'A'
call lcd_put
ldi temp,40
call lcd_gotoxy_80
ldi temp,'D'
call lcd_put
ldi temp,'i'
call lcd_put
ldi temp,'a'
call lcd_put
ldi temp,49
call lcd_gotoxy_80
ldi temp,'m'
call lcd_put
ldi temp,'m'
call lcd_put
ret

sbi portd,buz_
delayms20 16
cbi portd,buz_
delayms20 28




.include "interupts.inc"
.include "adc.inc"
.include "math_routines.inc"
.include "delay_routines.inc"
.include "strings.inc"
.include "lcd_func.inc"
.include "menu_struct.inc"
.include "uart.inc"
.include "cable_type_color.inc"
.include "eeprom.inc"

