	.text
	
	.equ VGA_pixelbuff, 0xC8000000
	.equ VGA_charbuff, 0xC9000000

	.equ X_offset, 0x00000001
	.equ Y_offset, 0x00000080
	.equ Y_offset_pixel, 0x00000400
	.equ pixel_end_Y, 0xC803BE7E
	.equ pixel_end_X, 0x0000013F	
	.equ char_end_Y, 0x00001DFF		// 59
	.equ char_end_X, 0x0000004F		// 79

	.global VGA_clear_charbuff_ASM
	.global VGA_clear_pixelbuff_ASM
	.global VGA_write_char_ASM
	.global VGA_write_byte_ASM
	.global VGA_draw_point_ASM

VGA_clear_charbuff_ASM:
	PUSH {R4-R12} 				//save the state of the system
	MOV R2, #0
	LDR R3, =VGA_charbuff
	MOV R0, #0

X_CHAR_LOOP: 
	MOV R1, #0
	ADD R4, R3, R0			// Iterate x-axis

Y_CHAR_LOOP: 
	ADD R5, R4, R1, LSL #8			// Iterate y-axis
	
	STRB R2, [R5]
	
	ADD R1, R1, #1
	CMP R1, #59
	BLT Y_CHAR_LOOP
	
	ADD R0, R0, #1
	CMP R0, #79
	BLT X_CHAR_LOOP

	POP {R4-R5}
	BX LR


VGA_clear_pixelbuff_ASM:
	PUSH {R4-R5}	
	MOV R2, #0
	LDR R3, =VGA_pixelbuff
	MOV R0, #0

X_PIXEL_LOOP:
	MOV R1, #0
	ADD R4, R3, R0, LSL #1			// Iterate x-axis
Y_PIXEL_LOOP:
	ADD R5, R4, R1, LSL #10			// Iterate y-axis
	
	STRH R2, [R5]
	
	ADD R1, R1, #1
	CMP R1, #240
	BLT Y_PIXEL_LOOP
	
	ADD R0, R0, #1
	CMP R0, #320
	BLT X_PIXEL_LOOP

	POP {R4-R5}
	BX LR
.end
