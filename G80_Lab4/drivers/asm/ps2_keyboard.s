	.text
	.equ PS2_Data, 0xFF200100
	.global read_PS2_data_ASM

read_PS2_data_ASM:
	PUSH {R1-R4} 
	LDR R1, =PS2_Data				// Load R1 with PS2 data address
	LDR R3, [R1]					// R3 holds value of PS2_Data
	ANDS R2, R3, #0x00008000		// Get the RVALID bit (bit 15)
	MOVEQ R0, #0					// If it's 0, return 0
	POPEQ {R1-R4}
	BXEQ LR
									// If it's 1,
	AND R2, R3, #0x000000FF			// Get data from PS2 data address (first 8 bits)
	STR R2, [R0]					// Store data to input pointer value
	MOV R0, #1						// return 1
	POP {R1-R4}
	BX LR  

.end
