/*
 * strings.inc
 *
 *  Created: 12/5/2024 9:09:46 AM
 *   Author: janos
 */ 

 table_chx_err:
.db "XX",0x00
 table_dia_err:
.db "XX.XX",0x00
 table_format_scr:
.db "00:00 -000000- 00/00",0x00
 table_format_line:
.db "M/min",0x00
table_janos:
.db "                    Electric-Group ",0xf8,"2025 *WWW.ELECTRICGROUP.RO *info@electricgroup.ro ***janos.szabo@gmail.com                    ",0x00
table_menu_radio:
.db "1.Set - Radio module",0x00
table_menu_sradio:
.db "Set - Radio module",0x00
table_menu_radio_channel:
.db "Radio Channel:",0x00
table_menu_radio_txpower:
.db "Radio TxPower:",0x00
table_menu_radio_range:
.db "Ch:0-127     TxP:1-9",0x00
table_menu_display:
.db "2.Set - Display",0x00
table_menu_sdisplay:
.db "Set - Display",0x00
table_menu_sdisplaydate:
.db "1.Display date",0x00
table_menu_sdisplaytemp:
.db "2.Display temp",0x00
table_menu_sdisplayhum:
.db "3.Display hum",0x00
table_menu_pulse:
.db "3.Set - Pulse/M",0x00
table_menu_spulse:
.db "Set - Pulse/M",0x00
table_menu_limit:
.db "4.Set - DIA limit",0x00
table_menu_slimit:
.db "Set -DIA limit",0x00
table_menu_oc1:
.db "5.Set - Output 1",0x00
table_menu_soc1:
.db "Set - Output 1",0x00
table_menu_oc2:
.db "6.Set - Output 2",0x00
table_menu_soc2:
.db "Set - Output 2",0x00
table_menu_lot:
.db "6.Set - LOT Number",0x00
table_menu_slot:
.db "Set - LOT Number",0x00
table_menu_sinfo:
.db "7.Server info",0x00
table_menu_info:
.db "8.Device info",0x00
table_l3live:
.db "L3LIVE",0x00
table_l3live_start:
.db "L3LIVESTART",0x00
table_l3live_stop:
.db "L3LIVESTOP",0x00
table_temp:
.db "00.0",0x00
table_hum:
.db "00.0",0x00
table_ip:
.db "*.*.*.*",0x00

table_fy05:						;8
.db "FY-0.5      ",0x00 ;0
table_fy1:
.db "FY-1        ",0x00 ;1
table_fy15:
.db "FY-1.5      ",0x00 ;2
table_fy25:
.db "FY-2.5      ",0x00 ;3
table_fy4:
.db "FY-4        ",0x00 ;4
table_fy6:
.db "FY-6        ",0x00 ;5
table_fy10:
.db "FY-10       ",0x00 ;6
table_fy16:
.db "FY-16       ",0x00 ;7

table_myf05:						;9
.db "MYF-0.5     ",0x00 ;8
table_myf75:
.db "MYF-0.75    ",0x00 ;9
table_myf1:
.db "MYF-1       ",0x00 ;10
table_myf15:
.db "MYF-1.5     ",0x00 ;11
table_myf25:
.db "MYF-2.5     ",0x00 ;12
table_myf4:
.db "MYF-4       ",0x00 ;13
table_myf6:
.db "MYF-6       ",0x00 ;14
table_myf10:
.db "MYF-10      ",0x00 ;15
table_myf16:
.db "MYF-16      ",0x00 ;16

table_cyf2x15:						;8
.db "CYYF-2x1.5  ",0x00 ;17
table_cyf3x15:
.db "CYYF-3x1.5  ",0x00 ;18
table_cyf4x15:
.db "CYYF-4x1.5  ",0x00 ;19
table_cyf5x15:
.db "CYYF-5x1.5  ",0x00 ;20
table_cyf2x25:
.db "CYYF-2x2.5  ",0x00 ;21
table_cyf3x25:
.db "CYYF-3x2.5  ",0x00 ;22
table_cyf4x25:
.db "CYYF-4x2.5  ",0x00 ;23
table_cyf5x25:
.db "CYYF-5x2.5  ",0x00 ;24

table_mym2x05:						;4
.db "MYYM-2x0.5  ",0x00 ;25
table_mym3x05:
.db "MYYM-3x0.5  ",0x00 ;26
table_mym4x05:
.db "MYYM-4x0.5  ",0x00 ;27
table_mym5x05:
.db "MYYM-5x0.5  ",0x00 ;28
table_mym2x075:						;8
.db "MYYM-2x0.75 ",0x00 ;29
table_mym3x075:
.db "MYYM-3x0.75 ",0x00 ;30
table_mym4x075:
.db "MYYM-4x0.75 ",0x00 ;31
table_mym5x075:
.db "MYYM-5x0.75 ",0x00 ;32
table_mym2x1:
.db "MYYM-2x1    ",0x00 ;33
table_mym3x1:
.db "MYYM-3x1    ",0x00 ;34
table_mym4x1:
.db "MYYM-4x1    ",0x00 ;35
table_mym5x1:
.db "MYYM-5x1    ",0x00 ;36

table_mym2x15:						;8
.db "MYYM-2x1.5  ",0x00 ;37
table_mym3x15:
.db "MYYM-3x1.5  ",0x00 ;38
table_mym4x15:
.db "MYYM-4x1.5  ",0x00 ;39
table_mym5x15:
.db "MYYM-5x1.5  ",0x00 ;40
table_mym2x25:
.db "MYYM-2x2.5  ",0x00 ;41
table_mym3x25:
.db "MYYM-3x2.5  ",0x00 ;42
table_mym4x25:
.db "MYYM-4x2.5  ",0x00 ;43
table_mym5x25:
.db "MYYM-5x2.5  ",0x00 ;44

table_h072x15:						;8
.db "H07RNF-2x1.5",0x00 ;45
table_h073x15:
.db "H07RNF-3x1.5",0x00 ;46
table_h074x15:
.db "H07RNF-4x1.5",0x00 ;47
table_h075x15:
.db "H07RNF-5x1.5",0x00 ;48
table_h072x25:
.db "H07RNF-2x2.5",0x00 ;49
table_h073x25:
.db "H07RNF-3x2.5",0x00 ;50
table_h074x25:
.db "H07RNF-4x2.5",0x00 ;51
table_h075x25:
.db "H07RNF-5x2.5",0x00 ;52

table_rvk2x15:						;8
.db "RVK-2x1.5   ",0x00 ;53
table_rvk3x15:
.db "RVK-3x1.5   ",0x00 ;54
table_rvk4x15:
.db "RVK-4x1.5   ",0x00 ;55
table_rvk5x15:
.db "RVK-5x1.5   ",0x00 ;56
table_rvk2x25:
.db "RVK-2x2.5   ",0x00 ;57
table_rvk3x25:
.db "RVK-3x2.5   ",0x00 ;58
table_rvk4x25:
.db "RVK-4x2.5   ",0x00 ;59
table_rvk5x25:
.db "RVK-5x2.5   ",0x00 ;60

table_nym2x15:						;8
.db "NYMJ-2x1.5  ",0x00 ;61
table_nym3x15:
.db "NYMJ-3x1.5  ",0x00 ;62
table_nym4x15:
.db "NYMJ-4x1.5  ",0x00 ;63
table_nym5x15:
.db "NYMJ-5x1.5  ",0x00 ;64
table_nym2x25:
.db "NYMJ-2x2.5  ",0x00 ;65
table_nym3x25:
.db "NYMJ-3x2.5  ",0x00 ;66
table_nym4x25:
.db "NYMJ-4x2.5  ",0x00 ;67
table_nym5x25:
.db "NYMJ-5x2.5  ",0x00 ;68

table_myup2x05:						;5
.db "MYUP-2x0.5  ",0x00 ;69
table_myup2x075:
.db "MYUP-2x0.75 ",0x00 ;70
table_myup2x1:
.db "MYUP-2x1    ",0x00 ;71
table_myup2x15:
.db "MYUP-2x1.5  ",0x00 ;72
table_myup2x25:
.db "MYUP-2x2.5  ",0x00 ;73

table_cu15:							;2
.db "CU-1.5      ",0x00 ;74
table_cu25:
.db "CU-2.5      ",0x00 ;75

table_ce15:							;2
.db "CE-1.5      ",0x00 ;76
table_ce25:
.db "CE-2.5      ",0x00 ;77

table_red:
.db "RED    ",0x00
table_blue:
.db "BLUE   ",0x00
table_brown:
.db "BROWN  ",0x00
table_black:
.db "BLACK  ",0x00
table_grey:
.db "GREY   ",0x00
table_yg:
.db "Y/GREEN",0x00
table_white:
.db "WHITE  ",0x00
table_orange:
.db "ORANGE ",0x00
table_none:
.db "-none- ",0x00

table_lt_cnt:
.db "  -==LT COUNTER==-",0x00
table_lt_cnt2:
.db "ver: 4.0  L80  ",0xD0,"2025",0x00
table_lt_inf:
.db "Atmega-1284P   20Mhz",0x00
table_lt_egr:
.db " Electric-Group.ro",0x00
table_srv_ip:
.db "Ip: ",0x00
table_srv_hstnm:
.db "Host: esp32-server",0x00
table_srv_temp:
.db "Ambient temp:",0x00
table_srv_hum:
.db "Ambient hum:",0x00
table_radio_cfg:
.db 0xA9,0xE1,0x06,0x64,0x03,0xA0,0x66,0x77,0x88,0x99,0x00,0x05,0x0D,0x0A,0xff
table_radio_read_cfg:
.db 0xAA,0xE2,0x0D,0x0A,0xff
table_rx_err:
.db "crc-error!",0x0D,0x0A,0xff