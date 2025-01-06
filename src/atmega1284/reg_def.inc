/*
 * def.inc
 *
 *  Created: 12/5/2024 9:10:13 AM
 *   Author: janos
 */ 
.def	fbin0	=r16	; binary value byte 0 (LSB)
.def	fbin1	=r17	; binary value byte 1
.def	fbin2	=r18	; binary value byte 2 (MSB)
.def	tBCD0	=r20	; BCD value digits 0 and 1
.def	tBCD1	=r21	; BCD value digits 2 and 3
.def	tBCD2	=r22	; BCD value digits 4 and 5
.def	tBCD3	=r23	; BCD value digits 6 and 7 (MSD)
.def	fBCD0	=r16	; BCD value digits 0 and 1
.def	fBCD1	=r17	; BCD value digits 2 and 3
.def	fBCD2	=r18	; BCD value digits 4 and 5
.def	fBCD3	=r19	; BCD value digits 6 and 7 (MSD is 0|1)
.def	tbin0	=r20	; binary value byte 0 (LSB)
.def	tbin1	=r21	; binary value byte 1
.def	tbin2	=r22	; binary value byte 2 (MSB)

.equ PAGESIZEB = PAGESIZE*2			;*LINE3,29/12,19:23:46,192.168.188.58,22.2,57.7,63
									;L3LIVE,CYYF 3X1.5,Grey,224,7970,1247,3461,8677,14
.def crc8   =r16 
.def data	=r17
.def poly	=r19

.def hex_byte = r17
.def high_nibble = r18
.def low_nibble = r19
.def ascii_offset = r20 ; Register to hold ASCII offset

.def temp	=r16
.def temp2	=r17
.def temp3	=r18
.def temp4	=r19
.def temp5	=r20
.def temp6	=r21
.def templ	=r22
.def temph	=r23
.def regl	=r24
.def regh	=r25
.def _xl = r26
.def _xh = r27
.def _yl = r28
.def _yh = r29
.def _zl = r30
.def _zh = r31

.equ radio_set 			= pb0
.equ radio_css			= pb1
.equ lcde 				= pb2
.equ lcdrs				= pb3

.equ key_menu			= pinc0 ;menu
.equ key_down			= pinc1 ;down
.equ key_up				= pinc2 ;up
.equ key_exit			= pinc3 ;exit
.equ key_start			= pinc4
.equ key_dia			= pinc5

.equ out0				= pc6
.equ out1				= pc7

.equ led_green			= pd4
.equ led_red			= pd5
.equ buz_				= pd7

.equ tim0_val = 0xbb	;6.2 ms
.equ tim2_val = 0x64	;8ms
.equ tim1_val = 0xc2f7	;

;****flags reg0
.equ rx0_int		= 7 ;0x80
.equ cnt_int		= 6 ;0x40
.equ ch0_ef			= 5 ;0x20
.equ ch1_ef			= 4 ;0x10
.equ dia_ef		 	= 3 ;0x08
.equ time_scrf		= 2 ;0x04
.equ adc_scrf		= 1 ;0x02
.equ time_05f		= 0 ;0x01

;****flags reg1
.equ key_menuf		= 7 ;0x80
.equ key_downf		= 6 ;0x40
.equ key_upf		= 5 ;0x20
.equ key_exitf		= 4 ;0x10
.equ key_startf	 	= 3 ;0x08
.equ key_diaf	 	= 2 ;0x04
.equ key_debounce	= 1 ;0x02
.equ rx0_data		= 0 ;0x01

;****flags reg2
.equ speed_scrf			= 7 ;0x80
.equ adc_frf			= 6 ;0x40
.equ speed_frf			= 5 ;0x20
.equ tsynch_f			= 4 ;0x10
.equ server_alivef 		= 3 ;0x08
.equ buz_key			= 2 ;0x04
.equ scr_rot			= 1 ;0x02
.equ cnt_out			= 0 ;0x01

;****flags reg3
.equ log_send				= 7 ;0x80
.equ log_state				= 6 ;0x40
.equ oc_blcd				= 5 ;0x20
.equ dsp_datef				= 4 ;0x10	eeflag0 = 0x80
.equ dsp_tempf		 		= 3 ;0x08	eeflag0 = 0x40
.equ dsp_humf				= 2 ;0x04	eeflag0 = 0x20
.equ dia_actf				= 1 ;0x02
.equ buz_ms					= 0 ;0x01

.def	t0=	r4
.def	t1=	r5
.def	t2=	r6
.def	t3=	r7
.def	t4=	r8
.def	t5=	r9
.def	t6=	r26
.def	t7=	r27
.def   cnt= r31

.def	a0=	r18 ;a  axb=a(rez)
.def	a1=	r19 ;a  a/b=a(rez)
.def	a2=	r20
.def	a3=	r21
.def	a4=	r22
.def	a5=	r23
.def	a6=	r24
.def	a7=	r25

.def	b0=	r10 ;b
.def	b1=	r11
.def	b2=	r12
.def	b3=	r13
.def	b4=	r14
.def	b5=	r15
.def	b6=	r16
.def	b7=	r17