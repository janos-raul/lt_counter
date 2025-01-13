/*
 * interupts.inc
 *
 *  Created: 12/5/2024 11:08:00 AM
 *   Author: janos
 */ 
;***************************** key int
int1_isr:
push temp
push temp2
in temp,sreg
push temp
ldi temp,0b00000001 ; disable key_int
out eimsk,temp
lds temp,flag_reg1
in temp2,pinc
sbrs temp2,key_menu
sbr temp,0x80 ; menu key pressed
sbrs temp2,key_down
sbr temp,0x40 ; down key pressed
sbrs temp2,key_up
sbr temp,0x20 ; up key pressed
sbrs temp2,key_exit
sbr temp,0x10 ; exit key pressed
sbrs temp2,key_start
sbr temp,0x08 ; start key pressed
sbrs temp2,key_dia
sbr temp,0x04 ; dia key pressed
sbr temp,0x02 ; set key debounce delay
sts flag_reg1,temp
clr temp
sts cnt4,temp ; clear debounce timer 
sts cnt1,temp ; reset buz timer
sbi portd,led_red
sbi portd,buz_
lds temp,flag_reg2
sbr temp,0x04	;buz_key
sts flag_reg2,temp
pop temp2
pop temp
out sreg,temp
pop temp
reti

;****************************** cnt_int
int0_isr:
push temp
in temp,sreg
push temp
push temp2
push temp3
push xl
push xh
push yl
push yh

clr temp2            ; Clear temp2 register (set to 0)
lds xl, enc_cnt      ; Load LSB from enc_cnt into xl
lds xh, enc_cnt+1    ; Load middle byte from enc_cnt+1 into xh
lds temp3, enc_cnt+2 ; Load MSB from enc_cnt+2 into temp3
adiw x, 1            ; Add 1 to the 16-bit word (xh:xl)
adc temp3, temp2     ; Add carry to the MSB in temp3

sts enc_cnt,xl
sts enc_cnt+1,xh
sts enc_cnt+2,temp3

lds yl,ppm24
lds yh,ppm24+1
lds temp2,ppm24+2

cp yl,xl
brne no_met
cpc yh,xh
brne no_met
cpc temp2,temp3
brne no_met
clr temp
sts enc_cnt,temp
sts enc_cnt+1,temp
sts enc_cnt+2,temp

clr temp2            ; Clear temp2 register (set to 0)
lds xl, met_cnt      ; Load LSB from enc_cnt into xl
lds xh, met_cnt+1    ; Load middle byte from enc_cnt+1 into xh
lds temp3, met_cnt+2 ; Load MSB from enc_cnt+2 into temp3
adiw x, 1            ; Add 1 to the 16-bit word (xh:xl)
adc temp3, temp2     ; Add carry to the MSB in temp3

sts met_cnt,xl
sts met_cnt+1,xh
sts met_cnt+2,temp3

lds xl, speed_cnt      ; Load LSB from enc_cnt into xl
lds xh, speed_cnt+1    ; Load middle byte from enc_cnt+1 into xh
adiw x, 1              ; Add 1 to the 16-bit word (xh:xl)
sts speed_cnt,xl
sts speed_cnt+1,xh

lds temp,flag_reg0
sbr temp,0x40
sts flag_reg0,temp
sbi portd,led_green
no_met:

pop yh
pop yl
pop xh
pop xl
pop temp3
pop temp2
pop temp
out sreg,temp
pop temp
reti

tim2_ovf_isr:
push temp
in temp,sreg
push temp
;
	ldi temp,tim2_val
	sts tcnt2,temp

lds temp,flag_reg2
sbrs temp,buz_key
jmp ovf2_exit0
lds temp,cnt1
inc temp
sts cnt1,temp
cpi temp,5
brne ovf2_exit0
lds temp,flag_reg2
cbr temp,0x04	;buz_key
sts flag_reg2,temp
	cbi portd,buz_
ovf2_exit0:
lds temp,cnt2
inc temp
sts cnt2,temp
cpi temp,50
brne ovf2_exit1
lds temp,flag_reg2
sbr temp,0x42	;refresh adc_calc 400ms & refresh scr_rotate
sts flag_reg2,temp
clr temp
sts cnt2,temp
cbi portd,led_green
ovf2_exit1:
lds temp,flag_reg1
sbrs temp,key_debounce
jmp ovf2_exit3
lds temp,cnt4
inc temp
sts cnt4,temp
cpi temp,30
brne ovf2_exit3
lds temp,flag_reg1
cbr temp,0x02	;key debounce clr
sts flag_reg1,temp
ldi temp,0b00000011
out eimsk,temp
cbi portd,led_red
ovf2_exit3:
lds temp,flag_reg2
sbrs temp,cnt_out
jmp ovf2_exit4
lds temp,cnt5
inc temp
sts cnt5,temp
cpi temp,125
brne ovf2_exit4
clr temp
sts cnt5,temp
cbi portc,out0
cbi portc,out1
lds temp,flag_reg2
cbr temp,0x01	;rel_ out0 & out1 delay(1000)
sts flag_reg2,temp
ovf2_exit4:
lds temp,cnt6
inc temp
sts cnt6,temp
cpi temp,60
brne ovf2_exit5
clr temp
sts cnt6,temp
lds temp,flag_reg0
sbr temp,0x02	;adc screen refresh 480ms
sts flag_reg0,temp
ovf2_exit5:
lds temp,flag_reg3
sbrs temp,buz_ms
jmp ovf2_exit6
lds temp,cnt3
inc temp
sts cnt3,temp
cpi temp,35			;buzzer on
brne ovf2_exit6
clr temp
sts cnt3,temp
sbis portd,buz_
jmp buz_on
cbi portd,buz_
jmp ovf2_exit6
buz_on:
sbi portd,buz_
ovf2_exit6:
lds temp,cnt0
inc temp
sts cnt0,temp
cpi temp,125
brne ovf2_exit
clr temp
sts cnt0,temp
lds temp,flag_reg0
sbr temp,0x04	;time refresh scr 1000ms
sts flag_reg0,temp
lds temp,sec_
cpi temp,30
brne svr_chk
lds temp,flag_reg2
cbr temp,0x08	;clear server alive flag
sts flag_reg2,temp
svr_chk:
lds temp,sec_
inc temp
sts sec_,temp
cpi temp,60
brne ovf2_exit
lds temp,flag_reg2
sbr temp,0x80	;speed refresh/calc 1min m/M 
sts flag_reg2,temp
clr temp
sts sec_,temp
lds temp,min_
inc temp
sts min_,temp
cpi temp,60
brlo ovf2_exit
lds temp,flag_reg2
sbr temp,0x10	;time synch 1/h
sts flag_reg2,temp
clr temp
sts min_,temp
lds temp,hour
inc temp
sts hour,temp
cpi temp,24
brlo ovf2_exit
clr temp
sts hour,temp
ovf2_exit:
;
pop temp
out sreg,temp
pop temp
reti

adc_isr:
push temp
in temp,sreg
push temp

lds temp,admux
cpi temp,0x40
brne not_adc0
lds temp,adcl
sts ch1_adc,temp
lds temp,adch
sts ch1_adc+1,temp
not_adc0:
lds temp,admux
cpi temp,0x41
brne not_adc1
lds temp,adcl
sts dia_adc,temp
lds temp,adch
sts dia_adc+1,temp
not_adc1:
lds temp,admux
cpi temp,0x42
brne not_adc2
lds temp,adcl
sts ch0_adc,temp
lds temp,adch
sts ch0_adc+1,temp
not_adc2:

lds temp,admux
inc temp
cpi temp,0x43
brne adc_in_range
ldi temp,0x40
adc_in_range:
sts admux,temp

pop temp
out sreg,temp
pop temp
reti

/*
adc_isr:
push temp
in temp,sreg
push temp

lds temp,admux
cpi temp,0x40
breq adc1_sel
cpi temp,0x41
breq adc2_sel
lds temp,adcl
sts ch1_adc,temp
lds temp,adch
sts ch1_adc+1,temp
ldi temp,0b01000000 ;adc vref AVCC , adc0 free run selected !!!
jmp adc_isr_e
adc1_sel:
lds temp,adcl
sts dia_adc,temp
lds temp,adch
sts dia_adc+1,temp
ldi temp,0b01000001 ;adc vref AVCC , adc1 free run selected !!!
jmp adc_isr_e
adc2_sel:
lds temp,adcl
sts ch0_adc,temp
lds temp,adch
sts ch0_adc+1,temp
ldi temp,0b01000010 ;adc vref AVCC , adc2 free run selected !!!
adc_isr_e:
sts admux,temp

pop temp
out sreg,temp
pop temp
reti
*/

USART0_RXC:
push temp
in temp,sreg
push temp
push temp2
push zl
push zh
;-------------------------------
	;sbi portb,led_
;-------------------------------

lds temp,rx0_err_cnt
inc temp
sts rx0_err_cnt,temp
cpi temp,64
brne rx0_no_err
clr temp
sts rx0_err_cnt,temp
ldi temp,low(rx0_buffer)
sts rx0_ptr,temp
ldi temp,high(rx0_buffer)
sts rx0_ptr+1,temp
jmp rx0_exit
rx0_no_err:

lds temp2,udr0

cpi temp2,0x0a
brne not_0a
ldi temp,low(rx0_buffer)
sts rx0_ptr,temp
ldi temp,high(rx0_buffer)
sts rx0_ptr+1,temp
jmp rx0_exit
not_0a:

lds zl,rx0_ptr
lds zh,rx0_ptr+1
st z+,temp2
sts rx0_ptr,zl
sts rx0_ptr+1,zh

cpi temp2,0x0d
brne rx0_exit
clr temp
sbiw z,1
st z,temp
sts rx0_err_cnt,temp
ldi temp,low(rx0_buffer)
sts rx0_ptr,temp
ldi temp,high(rx0_buffer)
sts rx0_ptr+1,temp
lds temp,flag_reg0
sbr temp,0x80
sts flag_reg0,temp
rx0_exit:
;-------------------------------
pop zh
pop zl
pop temp2
pop temp
out sreg,temp
pop temp
reti

USART0_UDRE:
push temp
in temp,sreg
push temp
push xl
push xh
;-----------------------------
	;sbi portb,led_
;-----------------------------
lds xl,tx0_ptr
lds xh,tx0_ptr+1
ld temp,x+
cpi temp,0xff
breq buf_eof
sts UDR0,temp
sts tx0_ptr,xl
sts tx0_ptr+1,xh

jmp udre_exit
buf_eof:
ldi temp,0b11011000
sts ucsr0b,temp
;lds temp,flag_reg2
;sbr temp,0x02			;txc1_bufcomplete set
;sts flag_reg2,temp
udre_exit:

pop xh
pop xl
pop temp
out sreg,temp
pop temp
reti
