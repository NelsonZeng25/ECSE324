	.text
	
	.equ VGA_pixelbuff, 0xC8000000
	.equ VGA_charbuff, 0xC9000000

	.global VGA_clear_charbuff_ASM
	.global VGA_clear_pixelbuff_ASM
	.global VGA_write_char_ASM
	.global VGA_write_byte_ASM
	.global VGA_draw_point_ASM

/*
 *	This method iterates through the character buffer column by column
 */
VGA_clear_charbuff_ASM:
	PUSH {R4-R12} 	
	MOV R2, #0				// 0 constant stored in R2
	LDR R3, =VGA_charbuff	// Address of character buffer
	MOV R0, #0				// x counter

X_CHAR_LOOP: 
	MOV R1, #0				// Reset y counter
	ADD R4, R3, R0			// Iterate to next column

Y_CHAR_LOOP: 
	ADD R5, R4, R1, LSL #7	// Iterate to next row
	
	STRB R2, [R5]			// Clear specific byte
	
	ADD R1, R1, #1			// increment y counter
	CMP R1, #60				// check if it's at the bottom of the screen
	BLT Y_CHAR_LOOP
	
	ADD R0, R0, #1			// increment x counter	
	CMP R0, #80				// check if it's at the right of the screen
	BLT X_CHAR_LOOP

	POP {R4-R5}
	BX LR


/*
 *	This method also iterates column by column but in the pixel buffer
 */
VGA_clear_pixelbuff_ASM:
	PUSH {R4-R5}	
	MOV R2, #0						// 0 constant stored in R2
	LDR R3, =VGA_pixelbuff			// Base address of pixel buffer
	MOV R0, #0						// x counter

X_PIXEL_LOOP:
	MOV R1, #0						// Reset y counter
	ADD R4, R3, R0, LSL #1			// Iterate to next column
Y_PIXEL_LOOP:
	ADD R5, R4, R1, LSL #10			// Iterate to next row
	
	STRH R2, [R5]					// Clear specific pixel
	
	ADD R1, R1, #1					// Increment y counter
	CMP R1, #240					// Check if we're at the bottom of the screen
	BLT Y_PIXEL_LOOP				// If not, continue incrementing y
	
	ADD R0, R0, #1					// If yes, increment x counter
	CMP R0, #320					// Check if we're done clearing
	BLT X_PIXEL_LOOP

	POP {R4-R5}
	BX LR

VGA_write_char_ASM:	
	PUSH {R3-R7}
	CMP R0, #0			// Check x and y for negative number inputs
	BXLT LR
	CMP R1, #0
	BXLT LR
	CMP R0, #79			// Check for out of bounds input
	BXGT LR
	CMP R1, #59
	BXGT LR

	LDR R3, =VGA_charbuff		// Set base address
	ADD R3, R3, R0				// Set x address
	ADD R3, R3, R1, LSL #7		// Set y address
	STRB R2, [R3]				// Store input to that location
	POP {R3-R7}
	BX LR

VGA_write_byte_ASM:
	PUSH {R3-R7}
	CMP R0, #0					// Check x and y for negative inputs
	BXLT LR
	CMP R1, #0
	BXLT LR
	CMP R0, #78					// Check for out of bound inputs
	BXGT LR						// 78 because there must be enough space to write 2 char
	CMP R1, #59
	BXGT LR
	
	LDR R3, =VGA_charbuff			// Set base address
	ADD R3, R3, R0					// Set X-address			
	ADD R3, R3, R1, LSL #7			// Add Y-address
									// R3 now holds the address of where we want to write
	LSR R4, R2, #4					// Get most significant hex					
	AND R5, R2, #0x0F				// Get least significant hex
	
	CMP R4, #9						// Check if input is A,B,C,D,E,F
	ADDGT R4, R4, #7				// Add 7 so that the ASCII value is right
	CMP R5, #9						
	ADDGT R5, R5, #7
	ADD R4, R4, #48					// Add base address of 0 (ASCII value 48)
	ADD R5, R5, #48
	
	STRB R4, [R3]					// Store most significant hex at input location
	STRB R5, [R3, #1]				// Store least significant hex at input location + 1
	POP {R3-R7}
	BX LR

VGA_draw_point_ASM:
	PUSH {R3}
	CMP R0, #0					// Check x and y for negative inputs
	BXLT LR
	CMP R1, #0
	BXLT LR
	LDR R3, =319				// Must use LDR since 319 is too high (overflow)
	CMP R0, R3					// Check for out of bounds input
	BXGT LR
	CMP R1, #239
	BXGT LR
	
	LDR R3, =VGA_pixelbuff		// Set base address
	ADD R3, R3, R0, LSL #1		// Set x address
	ADD R3, R3, R1, LSL #10		// Set y address
	STRH R2, [R3]				// Write to location
	POP {R3}
	BX LR

.end
