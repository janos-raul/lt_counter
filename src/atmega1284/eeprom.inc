/*
 * eeprom.inc
 *
 *  Created: 12/5/2024 9:13:29 AM
 *   Author: janos
 */ 

 EEPROM_read:
; Wait for completion of previous write
sbic EECR,EEPE
jmp EEPROM_read
; Set up address (r18:r17) in address register
out EEARH, xh
out EEARL, xl
; Start eeprom read by writing EERE
sbi EECR,EERE
; Read data from data register
in temp,EEDR
ret 
EEPROM_write:
; Wait for completion of previous write
sbic EECR,EEPE
jmp EEPROM_write
; Set up address (r18:r17) in address register
out EEARH, xh
out EEARL, xl
; Write data (r16) to data register
out EEDR,temp
; Write logical one to EEMWE
sbi EECR,EEMPE
; Start eeprom write by setting EEWE
sbi EECR,EEPE
ret

.eseg
table_ppm24:
.db 0x02,0x00,0x00
table_oc1_val:
.db 0x20,0xa1,0x07
table_oc2_val:
.db 0xc0,0x27,0x09
table_eeflag0:
.db 0x80
table_dia100:
.db 7