/*
 * cable_type_color.inc
 *
 *  Created: 12/23/2024 8:05:23 PM
 *   Author: janos
 */

color_dsp:
ldi temp,33
call lcd_gotoxy_80
call 	Lcd_ram_msg_put
ret
type_dsp:
ldi temp,20
call lcd_gotoxy_80
call 	Lcd_ram_msg_put
ret  
;************************************** type scan
type_scan:
lds temp,flag_reg1
sbrs temp,key_downf
ret
cbr temp,0x40
sts flag_reg1,temp
ldi xh,high(type_sram)
ldi xl,low(type_sram)
movw y,x
lds temp,type_index
inc temp
cpi temp,58
brne type_index_ok
clr temp
type_index_ok:
sts type_index,temp
tst temp
brne type_index0
jmp type_index0handler
type_index0:
cpi temp,1
brne type_index1
jmp type_index1handler
type_index1:
cpi temp,2
brne type_index2
jmp type_index2handler
type_index2:
cpi temp,3
brne type_index3
jmp type_index3handler
type_index3:
cpi temp,4
brne type_index4
jmp type_index4handler
type_index4:
cpi temp,5
brne type_index5
jmp type_index5handler
type_index5:
cpi temp,6
brne type_index6
jmp type_index6handler
type_index6:
cpi temp,7
brne type_index7
jmp type_index7handler
type_index7:
cpi temp,8
brne type_index8
jmp type_index8handler
type_index8:
cpi temp,9
brne type_index9
jmp type_index9handler
type_index9:
cpi temp,10
brne type_index10
jmp type_index10handler
type_index10:
cpi temp,11
brne type_index11
jmp type_index11handler
type_index11:
cpi temp,12
brne type_index12
jmp type_index12handler
type_index12:
cpi temp,13
brne type_index13
jmp type_index13handler
type_index13:
cpi temp,14
brne type_index14
jmp type_index14handler
type_index14:
cpi temp,15
brne type_index15
jmp type_index15handler
type_index15:
cpi temp,16
brne type_index16
jmp type_index16handler
type_index16:
cpi temp,17
brne type_index17
jmp type_index17handler
type_index17:
cpi temp,18
brne type_index18
jmp type_index18handler
type_index18:
cpi temp,19
brne type_index19
jmp type_index19handler
type_index19:
cpi temp,20
brne type_index20
jmp type_index20handler
type_index20:
cpi temp,21
brne type_index21
jmp type_index21handler
type_index21:
cpi temp,22
brne type_index22
jmp type_index22handler
type_index22:
cpi temp,23
brne type_index23
jmp type_index23handler
type_index23:
cpi temp,24
brne type_index24
jmp type_index24handler
type_index24:
cpi temp,25
brne type_index25
jmp type_index25handler
type_index25:
cpi temp,26
brne type_index26
jmp type_index26handler
type_index26:
cpi temp,27
brne type_index27
jmp type_index27handler
type_index27:
cpi temp,28
brne type_index28
jmp type_index28handler
type_index28:
cpi temp,29
brne type_index29
jmp type_index29handler
type_index29:
cpi temp,30
brne type_index30
jmp type_index30handler
type_index30:
cpi temp,31
brne type_index31
jmp type_index31handler
type_index31:
cpi temp,32
brne type_index32
jmp type_index32handler
type_index32:
cpi temp,33
brne type_index33
jmp type_index33handler
type_index33:
cpi temp,34
brne type_index34
jmp type_index34handler
type_index34:
cpi temp,35
brne type_index35
jmp type_index35handler
type_index35:
cpi temp,36
brne type_index36
jmp type_index36handler
type_index36:
cpi temp,37
brne type_index37
jmp type_index37handler
type_index37:
cpi temp,38
brne type_index38
jmp type_index38handler
type_index38:
cpi temp,39
brne type_index39
jmp type_index39handler
type_index39:
cpi temp,40
brne type_index40
jmp type_index40handler
type_index40:
cpi temp,41
brne type_index41
jmp type_index41handler
type_index41:
cpi temp,42
brne type_index42
jmp type_index42handler
type_index42:
cpi temp,43
brne type_index43
jmp type_index43handler
type_index43:
cpi temp,44
brne type_index44
jmp type_index44handler
type_index44:
cpi temp,45
brne type_index45
jmp type_index45handler
type_index45:
cpi temp,46
brne type_index46
jmp type_index46handler
type_index46:
cpi temp,47
brne type_index47
jmp type_index47handler
type_index47:
cpi temp,48
brne type_index48
jmp type_index48handler
type_index48:
cpi temp,49
brne type_index49
jmp type_index49handler
type_index49:
cpi temp,50
brne type_index50
jmp type_index50handler
type_index50:
cpi temp,51
brne type_index51
jmp type_index51handler
type_index51:
cpi temp,52
brne type_index52
jmp type_index52handler
type_index52:
cpi temp,53
brne type_index53
jmp type_index53handler
type_index53:
cpi temp,54
brne type_index54
jmp type_index54handler
type_index54:
cpi temp,55
brne type_index55
jmp type_index55handler
type_index55:
cpi temp,56
brne type_index56
jmp type_index56handler
type_index56:
cpi temp,57
brne type_index57
jmp type_index57handler
type_index57:
ret

type_index0handler:
ldi zh,high(table_fy05*2)
ldi zl,low(table_fy05*2)
call ld_sram
call type_dsp
ret
type_index1handler:
ldi zh,high(table_fy1*2)
ldi zl,low(table_fy1*2)
call ld_sram
call type_dsp
ret
type_index2handler:
ldi zh,high(table_fy15*2)
ldi zl,low(table_fy15*2)
call ld_sram
call type_dsp
ret
type_index3handler:
ldi zh,high(table_fy25*2)
ldi zl,low(table_fy25*2)
call ld_sram
call type_dsp
ret
type_index4handler:
ldi zh,high(table_fy4*2)
ldi zl,low(table_fy4*2)
call ld_sram
call type_dsp
ret
type_index5handler:
ldi zh,high(table_fy6*2)
ldi zl,low(table_fy6*2)
call ld_sram
call type_dsp
ret
type_index6handler:
ldi zh,high(table_fy10*2)
ldi zl,low(table_fy10*2)
call ld_sram
call type_dsp
ret
type_index7handler:
ldi zh,high(table_fy16*2)
ldi zl,low(table_fy16*2)
call ld_sram
call type_dsp
ret
type_index8handler:
ldi zh,high(table_myf05*2)
ldi zl,low(table_myf05*2)
call ld_sram
call type_dsp
ret
type_index9handler:
ldi zh,high(table_myf75*2)
ldi zl,low(table_myf75*2)
call ld_sram
call type_dsp
ret
type_index10handler:
ldi zh,high(table_myf1*2)
ldi zl,low(table_myf1*2)
call ld_sram
call type_dsp
ret
type_index11handler:
ldi zh,high(table_myf15*2)
ldi zl,low(table_myf15*2)
call ld_sram
call type_dsp
ret
type_index12handler:
ldi zh,high(table_myf25*2)
ldi zl,low(table_myf25*2)
call ld_sram
call type_dsp
ret
type_index13handler:
ldi zh,high(table_myf4*2)
ldi zl,low(table_myf4*2)
call ld_sram
call type_dsp
ret
type_index14handler:
ldi zh,high(table_myf6*2)
ldi zl,low(table_myf6*2)
call ld_sram
call type_dsp
ret
type_index15handler:
ldi zh,high(table_myf10*2)
ldi zl,low(table_myf10*2)
call ld_sram
call type_dsp
ret
type_index16handler:
ldi zh,high(table_myf16*2)
ldi zl,low(table_myf16*2)
call ld_sram
call type_dsp
ret
type_index17handler:
ldi zh,high(table_cyf2x15*2)
ldi zl,low(table_cyf2x15*2)
call ld_sram
call type_dsp
ret
type_index18handler:
ldi zh,high(table_cyf3x15*2)
ldi zl,low(table_cyf3x15*2)
call ld_sram
call type_dsp
ret
type_index19handler:
ldi zh,high(table_cyf4x15*2)
ldi zl,low(table_cyf4x15*2)
call ld_sram
call type_dsp
ret
type_index20handler:
ldi zh,high(table_cyf5x15*2)
ldi zl,low(table_cyf5x15*2)
call ld_sram
call type_dsp
ret
type_index21handler:
ldi zh,high(table_cyf2x25*2)
ldi zl,low(table_cyf2x25*2)
call ld_sram
call type_dsp
ret
type_index22handler:
ldi zh,high(table_cyf3x25*2)
ldi zl,low(table_cyf3x25*2)
call ld_sram
call type_dsp
ret
type_index23handler:
ldi zh,high(table_cyf4x25*2)
ldi zl,low(table_cyf4x25*2)
call ld_sram
call type_dsp
ret
type_index24handler:
ldi zh,high(table_cyf5x25*2)
ldi zl,low(table_cyf5x25*2)
call ld_sram
call type_dsp
ret
type_index25handler:
ldi zh,high(table_mym2x15*2)
ldi zl,low(table_mym2x15*2)
call ld_sram
call type_dsp
ret
type_index26handler:
ldi zh,high(table_mym3x15*2)
ldi zl,low(table_mym3x15*2)
call ld_sram
call type_dsp
ret
type_index27handler:
ldi zh,high(table_mym4x15*2)
ldi zl,low(table_mym4x15*2)
call ld_sram
call type_dsp
ret
type_index28handler:
ldi zh,high(table_mym5x15*2)
ldi zl,low(table_mym5x15*2)
call ld_sram
call type_dsp
ret
type_index29handler:
ldi zh,high(table_mym2x25*2)
ldi zl,low(table_mym2x25*2)
call ld_sram
call type_dsp
ret
type_index30handler:
ldi zh,high(table_mym3x25*2)
ldi zl,low(table_mym3x25*2)
call ld_sram
call type_dsp
ret
type_index31handler:
ldi zh,high(table_mym4x25*2)
ldi zl,low(table_mym4x25*2)
call ld_sram
call type_dsp
ret
type_index32handler:
ldi zh,high(table_mym5x25*2)
ldi zl,low(table_mym5x25*2)
call ld_sram
call type_dsp
ret
type_index33handler:
ldi zh,high(table_rvk2x15*2)
ldi zl,low(table_rvk2x15*2)
call ld_sram
call type_dsp
ret
type_index34handler:
ldi zh,high(table_rvk3x15*2)
ldi zl,low(table_rvk3x15*2)
call ld_sram
call type_dsp
ret
type_index35handler:
ldi zh,high(table_rvk4x15*2)
ldi zl,low(table_rvk4x15*2)
call ld_sram
call type_dsp
ret
type_index36handler:
ldi zh,high(table_rvk5x15*2)
ldi zl,low(table_rvk5x15*2)
call ld_sram
call type_dsp
ret
type_index37handler:
ldi zh,high(table_rvk2x25*2)
ldi zl,low(table_rvk2x25*2)
call ld_sram
call type_dsp
ret
type_index38handler:
ldi zh,high(table_rvk3x25*2)
ldi zl,low(table_rvk3x25*2)
call ld_sram
call type_dsp
ret
type_index39handler:
ldi zh,high(table_rvk4x25*2)
ldi zl,low(table_rvk4x25*2)
call ld_sram
call type_dsp
ret
type_index40handler:
ldi zh,high(table_rvk5x25*2)
ldi zl,low(table_rvk5x25*2)
call ld_sram
call type_dsp
ret
type_index41handler:
ldi zh,high(table_nym2x15*2)
ldi zl,low(table_nym2x15*2)
call ld_sram
call type_dsp
ret
type_index42handler:
ldi zh,high(table_nym3x15*2)
ldi zl,low(table_nym3x15*2)
call ld_sram
call type_dsp
ret
type_index43handler:
ldi zh,high(table_nym4x15*2)
ldi zl,low(table_nym4x15*2)
call ld_sram
call type_dsp
ret
type_index44handler:
ldi zh,high(table_nym5x15*2)
ldi zl,low(table_nym5x15*2)
call ld_sram
call type_dsp
ret
type_index45handler:
ldi zh,high(table_nym2x25*2)
ldi zl,low(table_nym2x25*2)
call ld_sram
call type_dsp
ret
type_index46handler:
ldi zh,high(table_nym3x25*2)
ldi zl,low(table_nym3x25*2)
call ld_sram
call type_dsp
ret
type_index47handler:
ldi zh,high(table_nym4x25*2)
ldi zl,low(table_nym4x25*2)
call ld_sram
call type_dsp
ret
type_index48handler:
ldi zh,high(table_nym5x25*2)
ldi zl,low(table_nym5x25*2)
call ld_sram
call type_dsp
ret
type_index49handler:
ldi zh,high(table_myup2x05*2)
ldi zl,low(table_myup2x05*2)
call ld_sram
call type_dsp
ret
type_index50handler:
ldi zh,high(table_myup2x075*2)
ldi zl,low(table_myup2x075*2)
call ld_sram
call type_dsp
ret
type_index51handler:
ldi zh,high(table_myup2x1*2)
ldi zl,low(table_myup2x1*2)
call ld_sram
call type_dsp
ret
type_index52handler:
ldi zh,high(table_myup2x15*2)
ldi zl,low(table_myup2x15*2)
call ld_sram
call type_dsp
ret
type_index53handler:
ldi zh,high(table_myup2x25*2)
ldi zl,low(table_myup2x25*2)
call ld_sram
call type_dsp
ret
type_index54handler:
ldi zh,high(table_cu15*2)
ldi zl,low(table_cu15*2)
call ld_sram
call type_dsp
ret
type_index55handler:
ldi zh,high(table_cu25*2)
ldi zl,low(table_cu25*2)
call ld_sram
call type_dsp
ret
type_index56handler:
ldi zh,high(table_ce15*2)
ldi zl,low(table_ce15*2)
call ld_sram
call type_dsp
ret
type_index57handler:
ldi zh,high(table_ce25*2)
ldi zl,low(table_ce25*2)
call ld_sram
call type_dsp
ret
;************************************ color scan
color_scan:
lds temp,flag_reg1
sbrs temp,key_upf
ret
cbr temp,0x20
sts flag_reg1,temp
ldi xh,high(color_sram)
ldi xl,low(color_sram)
movw y,x
lds temp,color_index
inc temp
cpi temp,8
brne color_index_ok
clr temp
color_index_ok:
sts color_index,temp
tst temp
breq color_index0
cpi temp,1
breq color_index1
cpi temp,2
breq color_index2
cpi temp,3
breq color_index3
cpi temp,4
breq color_index4
cpi temp,5
breq color_index5
cpi temp,6
breq color_index6
cpi temp,7
breq color_index7
ret
color_index0:
jmp color_index0handler
color_index1:
jmp color_index1handler
color_index2:
jmp color_index2handler
color_index3:
jmp color_index3handler
color_index4:
jmp color_index4handler
color_index5:
jmp color_index5handler
color_index6:
jmp color_index6handler
color_index7:
jmp color_index7handler

color_index0handler:
ldi zh,high(table_red*2)
ldi zl,low(table_red*2)
call ld_sram
call color_dsp
ret
color_index1handler:
ldi zh,high(table_blue*2)
ldi zl,low(table_blue*2)
call ld_sram
call color_dsp
ret
color_index2handler:
ldi zh,high(table_brown*2)
ldi zl,low(table_brown*2)
call ld_sram
call color_dsp
ret
color_index3handler:
ldi zh,high(table_black*2)
ldi zl,low(table_black*2)
call ld_sram
call color_dsp
ret
color_index4handler:
ldi zh,high(table_grey*2)
ldi zl,low(table_grey*2)
call ld_sram
call color_dsp
ret
color_index5handler:
ldi zh,high(table_yg*2)
ldi zl,low(table_yg*2)
call ld_sram
call color_dsp
ret
color_index6handler:
ldi zh,high(table_white*2)
ldi zl,low(table_white*2)
call ld_sram
call color_dsp
ret
color_index7handler:
ldi zh,high(table_orange*2)
ldi zl,low(table_orange*2)
call ld_sram
call color_dsp
ret