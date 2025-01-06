/*
 * uart.inc
 *
 *  Created: 12/5/2024 9:09:28 AM
 *   Author: janos
 */ 
tx0_send_buf:
ldi yh,high(tx0_buffer)
ldi yl,low(tx0_buffer)
ld temp,y+
sts tx0_ptr,yl
sts tx0_ptr+1,yh
sts udr0,temp
ldi temp,0b11111000
sts ucsr0b,temp
ret

/*
txc1_busy:
lds temp,flag_reg2
;sbrs temp,txc1_bufc
jmp txc1_busy
txc1_busyc:
lds temp,flag_reg2
;sbrs temp,txc1_done
jmp txc1_busyc
ret

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
*/