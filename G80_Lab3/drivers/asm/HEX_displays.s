  	.text
	//HEX displays addresses
	.equ HEX_0_3, 0xFF200020
	.equ HEX_4_5, 0xFF200030
	//functions
	.global HEX_clear_ASM
	.global HEX_flood_ASM
	.global HEX_write_ASM

// turns off all the LED segments of the HEX displays
HEX_clear_ASM:
			LDR R2, =HEX_4_5
			MOV R3, #0x00000066
			LSL R3, #8
			ORR R3, #0x66
			STR R3, [R2]

			LDR R1, =HEX_0_3
			MOV R5, #0x0000005B		//give it an initial value
			LSL R5, #8
			ORR R5, #0X3F
			LSL R5, #8
			ORR R5, #0X7D
			LSL R5, #8
			ORR R5, #0X66
			STR R5, [R1]

			PUSH {R1-R8,LR}			
			MOV R3, #0			// Index of which hex we're at
			
Clear_Loop:	CMP R3, #6
			BEQ Clear_DONE
			AND R4, R0, #1
			CMP R4, #1
			BLEQ Clear

			LSR R0, R0, #1 		// Shift the input right by 1 bit since the input is hot encoded
								// So we move on to the next bit to check if that value (HEX) is 1
			ADD R3, R3, #1		// Increment counter
			B Clear_Loop

Clear:		CMP R3, #3			// Check if we're at the HEX 4 or 5
			SUBGT R3, R3, #4	// Sets the counter to 0 or 1 when it's > 3 (the counter refers to HEX 4-5 when it's 0-1 after this is called)
			LDRGT R1, =HEX_4_5	// Set it to the the other disp HEX
			LDR R2, [R1]		// Set R2 to address of HEX 4-5
			LDR R5, =CLEAR_N
			LSL R6, R3, #2
			LDR R5, [R5, R6]
			AND R2, R2, R5
			STR R2, [R1]
			BX LR

Clear_DONE:	POP {R1-R8, LR}
			BX LR

HEX_flood_ASM:
			LDR R2, =HEX_4_5
			MOV R3, #0x00000066
			LSL R3, #8
			ORR R3, #0x66
			STR R3, [R2]

			LDR R1, =HEX_0_3
			MOV R5, #0x0000005B		//give it an initial value
			LSL R5, #8
			ORR R5, #0X3F
			LSL R5, #8
			ORR R5, #0X7D
			LSL R5, #8
			ORR R5, #0X66
			STR R5, [R1]

			PUSH {R1-R8,LR}			
			MOV R3, #0			// Index of which hex we're at
			
Flood_Loop:	CMP R3, #6
			BEQ Flood_DONE
			AND R4, R0, #1
			CMP R4, #1
			BLEQ Flood

			LSR R0, R0, #1 		// Shift the input right by 1 bit since the input is hot encoded
								// So we move on to the next bit to check if that value (HEX) is 1
			ADD R3, R3, #1		// Increment counter
			B Flood_Loop

Flood:		CMP R3, #3			// Check if we're at the HEX 4 or 5
			SUBGT R3, R3, #4	// Sets the counter to 0 or 1 when it's > 3 (the counter refers to HEX 4-5 when it's 0-1 after this is called)
			LDRGT R1, =HEX_4_5	// Set it to the the other disp HEX
			LDR R2, [R1]		// Set R2 to address of HEX 4-5
			LDR R5, =FLOOD_N
			LSL R6, R3, #2
			LDR R5, [R5, R6]
			ORR R2, R2, R5
			STR R2, [R1]
			BX LR

Flood_DONE:	POP {R1-R8, LR}
			BX LR

CLEAR_N:	.word 0xFFFFFF00
			.word 0xFFFF00FF
			.word 0xFF00FFFF
			.word 0x00FFFFFF

FLOOD_N:	.word 0x000000FF
			.word 0x0000FF00
			.word 0x00FF0000
			.word 0xFF000000


ZEROS:		.word 0x00000000
ONES:		.word 0x000000FF
LIGHTS:		.word 0x0000003F //(00111111)b = 0 on the HEX LEDs
			.word 0x00000006 //(00000110)b = 1 on the HEX LEDs
			.word 0x0000005B //(01011011)b = 2 on the HEX LEDs
			.word 0x0000004F //(01001111)b = 3 on the HEX LEDs
			.word 0x00000066 //(01100110)b = 4 on the HEX LEDs
			.word 0x0000006D //(01101101)b = 5 on the HEX LEDs
			.word 0x0000007D //(01111101)b = 6 on the HEX LEDs
			.word 0x00000007 //(00000111)b = 7 on the HEX LEDs	
			.word 0x0000007F //(01111111)b = 8 on the HEX LEDs	
			.word 0x00000067 //(01100111)b = 9 on the HEX LEDs	

			.word 0x00000077 //(01110111)b = A on the HEX LEDs
			.word 0x0000007C //(01111100)b = b on the HEX LEDs
			.word 0x00000039 //(00111001)b = C on the HEX LEDs
			.word 0x0000005E //(01011110)b = d on the HEX LEDs
			.word 0x00000079 //(01111001)b = E on the HEX LEDs
			.word 0x00000071 //(01110001)b = F on the HEX LEDs

	.end
