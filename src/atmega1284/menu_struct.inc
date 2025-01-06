/*
 * menu_struct.inc
 *
 *  Created: 12/5/2024 9:12:11 AM
 *   Author: janos submenu functions for 4x20 lcd
 */ 
;********************************radio jdy settings 
sm_radio:
cbi portb,radio_set
delayms20 10
call jdy_tx_read					;AA E2 04 00 09 A0 66 77 88 55 00 05 0D 0A
lds temp,flag_reg0
cbr temp,0x04	;1000ms
sts flag_reg0,temp

wait_jdy_response:
call rx0_recv_raw

lds temp,flag_reg0
sbrc temp,time_scrf
jmp jdy_no_rx

lds temp,flag_reg1
sbrs temp,rx0_data
jmp wait_jdy_response
cbr temp,0x01
sts flag_reg1,temp

call lcd_clr
ldi temp,1
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_sradio*2)
	ldi 	zl, low(table_menu_sradio*2)
	call 	Lcd_msg_put
ldi temp,20
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_radio_channel*2)
	ldi 	zl, low(table_menu_radio_channel*2)
	call 	Lcd_msg_put
ldi temp,40
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_radio_txpower*2)
	ldi 	zl, low(table_menu_radio_txpower*2)
	call 	Lcd_msg_put
ldi temp,60
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_radio_range*2)
	ldi 	zl, low(table_menu_radio_range*2)
	call 	Lcd_msg_put
call jdy_channel_dsp
call jdy_txpower_dsp
ldi temp,38
call lcd_gotoxy_80
call key_clr
radio_channel_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp radio_txpower_set
sbrc temp,key_exitf
jmp jdy_no_rx
sbrc temp,key_upf
jmp jdy_up_channel
sbrc temp,key_downf
jmp jdy_down_channel
jmp radio_channel_loop

jdy_up_channel:
lds temp,jdy_channel
inc temp
cpi temp,128
brne ch_in_range_up
ldi temp,127
ch_in_range_up:
sts jdy_channel,temp
call jdy_channel_dsp
call key_clr
jmp radio_channel_loop

jdy_down_channel:
lds temp,jdy_channel
dec temp
tst temp
brpl ch_in_range_down
clr temp
ch_in_range_down:
sts jdy_channel,temp
call jdy_channel_dsp
call key_clr
jmp radio_channel_loop

radio_txpower_set:
ldi temp,58
call lcd_gotoxy_80
call key_clr
radio_txpower_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp radio_save
sbrc temp,key_exitf
jmp jdy_no_rx
sbrc temp,key_upf
jmp jdy_up_tx
sbrc temp,key_downf
jmp jdy_down_tx
jmp radio_txpower_loop

jdy_up_tx:
lds temp,jdy_txpower
inc temp
cpi temp,10
brne tx_in_range_up
ldi temp,9
tx_in_range_up:
sts jdy_txpower,temp
call jdy_txpower_dsp
call key_clr
jmp radio_txpower_loop

jdy_down_tx:
lds temp,jdy_txpower
dec temp
tst temp
brpl tx_in_range_down
clr temp
tx_in_range_down:
sts jdy_txpower,temp
call jdy_txpower_dsp
call key_clr
jmp radio_txpower_loop

radio_save:
call lcd_nbnk
ldi temp,69
call lcd_gotoxy_80
ldi zh,high(table_radio_cfg*2)
ldi zl,low(table_radio_cfg*2)
ldi yh,high(tx0_buffer)
ldi yl,low(tx0_buffer)
tx0_loop:
lpm temp,z+
st y+,temp
cpi temp,0xff
brne tx0_loop
ldi yh,high(tx0_buffer+3)
ldi yl,low(tx0_buffer+3)
lds temp,jdy_channel
st y,temp
ldi yh,high(tx0_buffer+4)
ldi yl,low(tx0_buffer+4)
lds temp,jdy_txpower
st y,temp
call tx0_send_buf
ldi temp,low(rx0_buffer)
sts rx0_ptr,temp
ldi temp,high(rx0_buffer)
sts rx0_ptr+1,temp

lds temp,flag_reg0
cbr temp,0x04	;1000ms
sts flag_reg0,temp

wait_jdy_response_ok:
lds temp,flag_reg0
sbrc temp,time_scrf
jmp jdy_no_rx

lds temp,flag_reg0
sbrs temp,rx0_int
jmp wait_jdy_response_ok
cbr temp,0x80
sts flag_reg0,temp

ldi zl,low(rx0_buffer)
ldi zh,high(rx0_buffer)
ld temp,z+
call lcd_put
ld temp,z+
call lcd_put
ld temp,z
call lcd_put
call key_clr
radio_ok_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp jdy_no_rx
sbrc temp,key_exitf
jmp jdy_no_rx
sbrc temp,key_upf
jmp jdy_no_rx
sbrc temp,key_downf
jmp jdy_no_rx
jmp radio_ok_loop

jdy_channel_dsp:
ldi temp,35
call lcd_gotoxy_80
lds fbin0,jdy_channel
clr fbin1
clr fbin2
call bin3bcd
call dig03_disp
ret
jdy_txpower_dsp:
ldi temp,57
call lcd_gotoxy_80
lds temp,jdy_txpower
ldi temp2,0x30
or temp,temp2
call lcd_put
ret
jdy_no_rx:
sbi portb,radio_set
jmp menu_exit
;********************************display submenu
sm_display:
call lcd_clr
ldi temp,4
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_sdisplay*2)
	ldi 	zl, low(table_menu_sdisplay*2)
	call 	Lcd_msg_put
ldi temp,20
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_sdisplaydate*2)
	ldi 	zl, low(table_menu_sdisplaydate*2)
	call 	Lcd_msg_put
ldi temp,40
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_sdisplaytemp*2)
	ldi 	zl, low(table_menu_sdisplaytemp*2)
	call 	Lcd_msg_put
ldi temp,60
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_sdisplayhum*2)
	ldi 	zl, low(table_menu_sdisplayhum*2)
	call 	Lcd_msg_put
lds temp,flag_reg3
sbrc temp,dsp_datef
ldi temp2,2
sbrc temp,dsp_tempf
ldi temp2,3
sbrc temp,dsp_humf
ldi temp2,4
sts menu_index,temp2
call lcd_bnk
call menu_lnbnk
call key_clr

sm_display_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp sm_display_save
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call display_up
sbrc temp,key_downf
call display_dn

jmp sm_display_loop

display_up:
lds temp,menu_index
inc temp
cpi temp,5
brne dsp_up_ok
ldi temp,4
dsp_up_ok:
sts menu_index,temp
;call index_dsp
call menu_lnbnk
call key_clr
ret

display_dn:
lds temp,menu_index
dec temp
cpi temp,1
brne dsp_dn_ok
ldi temp,2
dsp_dn_ok:
sts menu_index,temp
;call index_dsp
call menu_lnbnk
call key_clr
ret

index_dsp:
ldi temp,0
call lcd_gotoxy_80
ldi temp2,0x30
lds temp,menu_index
or temp,temp2
call lcd_put
ret

sm_display_save:
lds temp,menu_index
cpi temp,2
breq save_date
cpi temp,3
breq save_temp
cpi temp,4
breq save_hum

save_date:
lds temp,flag_reg3
sbr temp,0x10
cbr temp,0x0c
sts flag_reg3,temp
ldi xh,high(table_eeflag0)
ldi xl,low(table_eeflag0)
ldi temp,0x80
call eeprom_write
jmp menu_exit

save_temp:
lds temp,flag_reg3
sbr temp,0x08
cbr temp,0x14
sts flag_reg3,temp
ldi xh,high(table_eeflag0)
ldi xl,low(table_eeflag0)
ldi temp,0x40
call eeprom_write
jmp menu_exit

save_hum:
lds temp,flag_reg3
sbr temp,0x04
cbr temp,0x18
sts flag_reg3,temp
ldi xh,high(table_eeflag0)
ldi xl,low(table_eeflag0)
ldi temp,0x20
call eeprom_write
jmp menu_exit
;********************************pulse submenu
sm_pulse:
call lcd_clr
ldi temp,4
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_spulse*2)
	ldi 	zl, low(table_menu_spulse*2)
	call 	Lcd_msg_put

ldi temp,47 
call lcd_gotoxy_80
lds fbin0,ppm24
lds fbin1,ppm24+1
lds fbin2,ppm24+2
call bin3bcd
sts ppm24_bcd,tbcd0
sts ppm24_bcd+1,tbcd1
sts ppm24_bcd+2,tbcd2
call dig06_disp
ldi temp,47 
call lcd_gotoxy_80
call lcd_bnk
lds temp,ppm24_bcd+2			;fbcd2 high digit 6
swap temp
andi temp,0x0f
sts m_set6,temp
call key_clr

ppm24menuget1_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp ppm24menuget2
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_6
jmp ppm24menuget1_loop

ppm24menuget2:
lds temp2,ppm24_bcd+2			;save back fbcd2 high digit 6
andi temp2,0x0f
lds temp,m_set6
andi temp,0x0f
swap temp
or temp,temp2
sts ppm24_bcd+2,temp

ldi temp,48 
call lcd_gotoxy_80
lds temp,ppm24_bcd+2			;fbcd2 low digit 5
andi temp,0x0f
sts m_set6,temp
call key_clr

ppm24menuget2_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp ppm24menuget3
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_5
jmp ppm24menuget2_loop

ppm24menuget3:
lds temp2,ppm24_bcd+2			;save back fbcd2 low digit 5
andi temp2,0xf0
lds temp,m_set6
andi temp,0x0f
or temp,temp2
sts ppm24_bcd+2,temp

ldi temp,49 
call lcd_gotoxy_80
lds temp,ppm24_bcd+1			;fbcd1 high digit 4
swap temp
andi temp,0x0f
sts m_set6,temp
call key_clr

ppm24menuget3_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp ppm24menuget4
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_4
jmp ppm24menuget3_loop

ppm24menuget4:
lds temp2,ppm24_bcd+1				;save back tbcd1 high digit 4 
andi temp2,0x0f
lds temp,m_set6
andi temp,0x0f
swap temp
or temp,temp2
sts ppm24_bcd+1,temp

ldi temp,50 
call lcd_gotoxy_80
lds temp,ppm24_bcd+1				;tbcd1 low digit 3
andi temp,0x0f
sts m_set6,temp
call key_clr

ppm24menuget4_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp ppm24menuget5
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_3
jmp ppm24menuget4_loop

ppm24menuget5:
lds temp2,ppm24_bcd+1				;save back tbcd1 low digit 3
andi temp2,0xf0
lds temp,m_set6
andi temp,0x0f
or temp,temp2
sts ppm24_bcd+1,temp

ldi temp,51 
call lcd_gotoxy_80
lds temp,ppm24_bcd				;tbcd0 high digit 2
swap temp
andi temp,0x0f
sts m_set6,temp
call key_clr

ppm24menuget5_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp ppm24menuget6
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_2
jmp ppm24menuget5_loop

ppm24menuget6:
lds temp2,ppm24_bcd				;save back tbcd0 high digit 2
andi temp2,0x0f
lds temp,m_set6
andi temp,0x0f
swap temp
or temp,temp2
sts ppm24_bcd,temp

ldi temp,52 
call lcd_gotoxy_80
lds temp,ppm24_bcd				;tbcd0 low digit 1
andi temp,0x0f
sts m_set6,temp
call key_clr

ppm24menuget6_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp ppm24get6_exit
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_1
jmp ppm24menuget6_loop

ppm24get6_exit:
lds temp2,ppm24_bcd				;save back tbcd0 low digit 1
andi temp2,0xf0
lds temp,m_set6
andi temp,0x0f
or temp,temp2
sts ppm24_bcd,temp

lds fbcd0,ppm24_bcd
lds fbcd1,ppm24_bcd+1
lds fbcd2,ppm24_bcd+2
clr fbcd3
call bcd3bin
sts ppm24,tbin0
sts ppm24+1,tbin1
sts ppm24+2,tbin2
lds temp,ppm24
ldi xh,high(table_ppm24)
ldi xl,low(table_ppm24)
call eeprom_write
lds temp,ppm24+1
ldi xh,high(table_ppm24+1)
ldi xl,low(table_ppm24+1)
call eeprom_write
lds temp,ppm24+2
ldi xh,high(table_ppm24+2)
ldi xl,low(table_ppm24+2)
call eeprom_write
jmp menu_exit
;********************************dia limit submenu
sm_limit:
call lcd_clr
ldi temp,3
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_slimit*2)
	ldi 	zl, low(table_menu_slimit*2)
	call 	Lcd_msg_put
call lim_dsp
call lcd_bnk
call key_clr

dia100_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp dia100_set
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call dia100_up
sbrc temp,key_downf
call dia100_dn
jmp dia100_loop

dia100_up:
lds temp,dia_100
inc temp
cpi temp,101
brne dia_upok
ldi temp,100
dia_upok:
sts dia_100,temp
call lim_dsp
call key_clr
ret

dia100_dn:
lds temp,dia_100
dec temp
tst temp
brne dia_dnok
ldi temp,1
dia_dnok:
sts dia_100,temp
call lim_dsp
call key_clr
ret

dia100_set:
lds temp,dia_100
ldi xh,high(table_dia100)
ldi xl,low(table_dia100)
call eeprom_write
jmp menu_exit

lim_dsp:
ldi temp,48
call lcd_gotoxy_80
lds fbin0,dia_100
clr fbin1
clr fbin2
call bin3bcd
call dig03_disp
ldi temp,' '
call lcd_put
ldi temp,'%'
call lcd_put
ldi temp,51
call lcd_gotoxy_80
ret
;********************************output 1 submenu
sm_oc1:
call lcd_clr
ldi temp,4
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_soc1*2)
	ldi 	zl, low(table_menu_soc1*2)
	call 	Lcd_msg_put
ldi temp,47 
call lcd_gotoxy_80
lds fbin0,oc1_val
lds fbin1,oc1_val+1
lds fbin2,oc1_val+2
call bin3bcd
sts oc1_bcd_val,tbcd0
sts oc1_bcd_val+1,tbcd1
sts oc1_bcd_val+2,tbcd2
call dig06_disp
ldi temp,47 
call lcd_gotoxy_80
call lcd_bnk
lds temp,oc1_bcd_val+2			;fbcd2 high digit 6
swap temp
andi temp,0x0f
sts m_set6,temp
call key_clr

oc1menuget1_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc1menuget2
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_6
jmp oc1menuget1_loop

oc1menuget2:
lds temp2,oc1_bcd_val+2			;save back fbcd2 high digit 6
andi temp2,0x0f
lds temp,m_set6
andi temp,0x0f
swap temp
or temp,temp2
sts oc1_bcd_val+2,temp

ldi temp,48 
call lcd_gotoxy_80
lds temp,oc1_bcd_val+2			;fbcd2 low digit 5
andi temp,0x0f
sts m_set6,temp
call key_clr

oc1menuget2_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc1menuget3
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_5
jmp oc1menuget2_loop

oc1menuget3:
lds temp2,oc1_bcd_val+2			;save back fbcd2 low digit 5
andi temp2,0xf0
lds temp,m_set6
andi temp,0x0f
or temp,temp2
sts oc1_bcd_val+2,temp

ldi temp,49 
call lcd_gotoxy_80
lds temp,oc1_bcd_val+1			;fbcd1 high digit 4
swap temp
andi temp,0x0f
sts m_set6,temp
call key_clr

oc1menuget3_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc1menuget4
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_4
jmp oc1menuget3_loop

oc1menuget4:
lds temp2,oc1_bcd_val+1				;save back tbcd1 high digit 4 
andi temp2,0x0f
lds temp,m_set6
andi temp,0x0f
swap temp
or temp,temp2
sts oc1_bcd_val+1,temp

ldi temp,50 
call lcd_gotoxy_80
lds temp,oc1_bcd_val+1				;tbcd1 low digit 3
andi temp,0x0f
sts m_set6,temp
call key_clr

oc1menuget4_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc1menuget5
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_3
jmp oc1menuget4_loop

oc1menuget5:
lds temp2,oc1_bcd_val+1				;save back tbcd1 low digit 3
andi temp2,0xf0
lds temp,m_set6
andi temp,0x0f
or temp,temp2
sts oc1_bcd_val+1,temp

ldi temp,51 
call lcd_gotoxy_80
lds temp,oc1_bcd_val				;tbcd0 high digit 2
swap temp
andi temp,0x0f
sts m_set6,temp
call key_clr

oc1menuget5_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc1menuget6
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_2
jmp oc1menuget5_loop

oc1menuget6:
lds temp2,oc1_bcd_val				;save back tbcd0 high digit 2
andi temp2,0x0f
lds temp,m_set6
andi temp,0x0f
swap temp
or temp,temp2
sts oc1_bcd_val,temp

ldi temp,52 
call lcd_gotoxy_80
lds temp,oc1_bcd_val				;tbcd0 low digit 1
andi temp,0x0f
sts m_set6,temp
call key_clr

oc1menuget6_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc1get6_exit
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_1
jmp oc1menuget6_loop

oc1get6_exit:
lds temp2,oc1_bcd_val				;save back tbcd0 low digit 1
andi temp2,0xf0
lds temp,m_set6
andi temp,0x0f
or temp,temp2
sts oc1_bcd_val,temp

lds fbcd0,oc1_bcd_val
lds fbcd1,oc1_bcd_val+1
lds fbcd2,oc1_bcd_val+2
clr fbcd3
call bcd3bin
sts oc1_val,tbin0
sts oc1_val+1,tbin1
sts oc1_val+2,tbin2
lds temp,oc1_val
ldi xh,high(table_oc1_val)
ldi xl,low(table_oc1_val)
call eeprom_write
lds temp,oc1_val+1
ldi xh,high(table_oc1_val+1)
ldi xl,low(table_oc1_val+1)
call eeprom_write
lds temp,oc1_val+2
ldi xh,high(table_oc1_val+2)
ldi xl,low(table_oc1_val+2)
call eeprom_write
jmp menu_exit
;********************************output 2 submenu
sm_oc2:
call lcd_clr
ldi temp,4
call lcd_gotoxy_80
	ldi 	zh, high(table_menu_soc2*2)
	ldi 	zl, low(table_menu_soc2*2)
	call 	Lcd_msg_put
ldi temp,47 
call lcd_gotoxy_80
lds fbin0,oc2_val
lds fbin1,oc2_val+1
lds fbin2,oc2_val+2
call bin3bcd
sts oc2_bcd_val,tbcd0
sts oc2_bcd_val+1,tbcd1
sts oc2_bcd_val+2,tbcd2
call dig06_disp
ldi temp,47 
call lcd_gotoxy_80
call lcd_bnk
lds temp,oc2_bcd_val+2			;fbcd2 high digit 6
swap temp
andi temp,0x0f
sts m_set6,temp
call key_clr

oc2menuget1_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc2menuget2
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_6
jmp oc2menuget1_loop

oc2menuget2:
lds temp2,oc2_bcd_val+2			;save back fbcd2 high digit 6
andi temp2,0x0f
lds temp,m_set6
andi temp,0x0f
swap temp
or temp,temp2
sts oc2_bcd_val+2,temp

ldi temp,48 
call lcd_gotoxy_80
lds temp,oc2_bcd_val+2			;fbcd2 low digit 5
andi temp,0x0f
sts m_set6,temp
call key_clr

oc2menuget2_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc2menuget3
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_5
jmp oc2menuget2_loop

oc2menuget3:
lds temp2,oc2_bcd_val+2			;save back fbcd2 low digit 5
andi temp2,0xf0
lds temp,m_set6
andi temp,0x0f
or temp,temp2
sts oc2_bcd_val+2,temp

ldi temp,49 
call lcd_gotoxy_80
lds temp,oc2_bcd_val+1			;fbcd1 high digit 4
swap temp
andi temp,0x0f
sts m_set6,temp
call key_clr

oc2menuget3_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc2menuget4
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_4
jmp oc2menuget3_loop

oc2menuget4:
lds temp2,oc2_bcd_val+1				;save back tbcd1 high digit 4 
andi temp2,0x0f
lds temp,m_set6
andi temp,0x0f
swap temp
or temp,temp2
sts oc2_bcd_val+1,temp

ldi temp,50 
call lcd_gotoxy_80
lds temp,oc2_bcd_val+1				;tbcd1 low digit 3
andi temp,0x0f
sts m_set6,temp
call key_clr

oc2menuget4_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc2menuget5
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_3
jmp oc2menuget4_loop

oc2menuget5:
lds temp2,oc2_bcd_val+1				;save back tbcd1 low digit 3
andi temp2,0xf0
lds temp,m_set6
andi temp,0x0f
or temp,temp2
sts oc2_bcd_val+1,temp

ldi temp,51 
call lcd_gotoxy_80
lds temp,oc2_bcd_val				;tbcd0 high digit 2
swap temp
andi temp,0x0f
sts m_set6,temp
call key_clr

oc2menuget5_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc2menuget6
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_2
jmp oc2menuget5_loop

oc2menuget6:
lds temp2,oc2_bcd_val				;save back tbcd0 high digit 2
andi temp2,0x0f
lds temp,m_set6
andi temp,0x0f
swap temp
or temp,temp2
sts oc2_bcd_val,temp

ldi temp,52 
call lcd_gotoxy_80
lds temp,oc2_bcd_val				;tbcd0 low digit 1
andi temp,0x0f
sts m_set6,temp
call key_clr

oc2menuget6_loop:
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp oc2get6_exit
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
call oc_get6_up
sbrc temp,key_downf
call oc_get6_dn

call oc_lcd_1
jmp oc2menuget6_loop

oc2get6_exit:
lds temp2,oc2_bcd_val				;save back tbcd0 low digit 1
andi temp2,0xf0
lds temp,m_set6
andi temp,0x0f
or temp,temp2
sts oc2_bcd_val,temp

lds fbcd0,oc2_bcd_val
lds fbcd1,oc2_bcd_val+1
lds fbcd2,oc2_bcd_val+2
clr fbcd3
call bcd3bin
sts oc2_val,tbin0
sts oc2_val+1,tbin1
sts oc2_val+2,tbin2
lds temp,oc2_val
ldi xh,high(table_oc2_val)
ldi xl,low(table_oc2_val)
call eeprom_write
lds temp,oc2_val+1
ldi xh,high(table_oc2_val+1)
ldi xl,low(table_oc2_val+1)
call eeprom_write
lds temp,oc2_val+2
ldi xh,high(table_oc2_val+2)
ldi xl,low(table_oc2_val+2)
call eeprom_write
jmp menu_exit

oc_lcd_6:						;move to digit 6
lds temp,flag_reg3
sbrs temp,oc_blcd
ret
cbr temp,0x20
sts flag_reg3,temp
ldi temp,47
call lcd_gotoxy_80
ret
oc_lcd_5:						;move to digit 5
lds temp,flag_reg3
sbrs temp,oc_blcd
ret
cbr temp,0x20
sts flag_reg3,temp
ldi temp,48
call lcd_gotoxy_80
ret
oc_lcd_4:							;move to digit 4
lds temp,flag_reg3
sbrs temp,oc_blcd
ret
cbr temp,0x20
sts flag_reg3,temp
ldi temp,49
call lcd_gotoxy_80
ret
oc_lcd_3:							;move to digit 3
lds temp,flag_reg3
sbrs temp,oc_blcd
ret
cbr temp,0x20
sts flag_reg3,temp
ldi temp,50
call lcd_gotoxy_80
ret
oc_lcd_2:							;move to digit 2
lds temp,flag_reg3
sbrs temp,oc_blcd
ret
cbr temp,0x20
sts flag_reg3,temp
ldi temp,51
call lcd_gotoxy_80
ret
oc_lcd_1:							;move to digit 1
lds temp,flag_reg3
sbrs temp,oc_blcd
ret
cbr temp,0x20
sts flag_reg3,temp
ldi temp,52
call lcd_gotoxy_80
ret

oc_get6_up:
lds temp,m_set6
inc temp
jmp oc_arange_num6
oc_get6_dn:
lds temp,m_set6
dec temp
oc_arange_num6:
tst temp
brpl oc_in_range_plus
ldi temp,9
oc_in_range_plus:
cpi temp,10
brlo oc_in_range6
clr temp
oc_in_range6:
sts m_set6,temp
ori temp,0x30
call lcd_put
call key_clr
lds temp,flag_reg3
sbr temp,0x20
sts flag_reg3,temp
ret
;*************************************server info submenu
sm_srv_inf:
call lcd_nbnk
call lcd_clr
ldi temp,0
call lcd_gotoxy_80
	ldi 	zh, high(table_srv_ip*2)
	ldi 	zl, low(table_srv_ip*2)
	call 	Lcd_msg_put
	ldi yl,low(ip_sram)
	ldi yh,high(ip_sram)
	call Lcd_ram_msg_put
ldi temp,20
call lcd_gotoxy_80
	ldi 	zh, high(table_srv_hstnm*2)
	ldi 	zl, low(table_srv_hstnm*2)
	call 	Lcd_msg_put
ldi temp,40
call lcd_gotoxy_80
	ldi 	zh, high(table_srv_temp*2)
	ldi 	zl, low(table_srv_temp*2)
	call 	Lcd_msg_put
ldi temp,60
call lcd_gotoxy_80
	ldi 	zh, high(table_srv_hum*2)
	ldi 	zl, low(table_srv_hum*2)
	call 	Lcd_msg_put

call key_clr
srv_inf_loop:
call srv_data_dsp
call background_tasks
lds temp,flag_reg1
sbrc temp,key_menuf
jmp menu_exit
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
jmp menu_exit
sbrc temp,key_downf
jmp menu_exit
jmp srv_inf_loop
srv_data_dsp:
lds temp,flag_reg0
sbrs temp,time_scrf
ret
cbr temp,0x04
sts flag_reg0,temp
ldi temp,54
call lcd_gotoxy_80
	ldi yl,low(temp_sram)
	ldi yh,high(temp_sram)
	call Lcd_ram_msg_put
	ldi temp,0xb2
	call lcd_put
ldi temp,74
call lcd_gotoxy_80
	ldi yl,low(hum_sram)
	ldi yh,high(hum_sram)
	call Lcd_ram_msg_put
	ldi temp,'%'
	call lcd_put
ret
;***********************************device info submenu
sm_dev_inf:
call lcd_nbnk
call lcd_clr
	ldi 	zh, high(table_janos*2)
	ldi 	zl, low(table_janos*2)
	sts ptr_rot,zh
	sts ptr_rot+1,zl
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
ldi temp,60
call lcd_gotoxy_80
	ldi 	zh, high(table_lt_inf*2)
	ldi 	zl, low(table_lt_inf*2)
	call 	Lcd_msg_put
call key_clr
dev_inf_loop:
call background_tasks
call adv
lds temp,flag_reg1
sbrc temp,key_menuf
jmp menu_exit
sbrc temp,key_exitf
jmp menu_exit
sbrc temp,key_upf
jmp menu_exit
sbrc temp,key_downf
jmp menu_exit
jmp dev_inf_loop


;*****************************************
;jmp trap
;*****************************************
sub_menu_exit:
;***********
call key_clr
call lcd_nbnk
call lcd_clr
jmp pre_main
