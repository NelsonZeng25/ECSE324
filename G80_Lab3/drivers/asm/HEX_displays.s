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
			STR R3, [R2]

			LDR R1, =HEX_0_3
			MOV R5, #0x0000005B		//give it an initial value
			LSL R5, #8
			ORR R5, #0X3F
			LSL R5, #8
			ORR R5, #0X7D
			LSL R5, #8
			ORR R5, #0X67
			STR R5, [R1]
			BX LR

CLEARING:	.word 0xFFFFFF00
			.word 0xFFFF00FF
			.word 0xFF00FFFF
			.word 0x00FFFFFF

ZEROS:		.word 0x00000000
ONES:		.word 0x000000FF
LIGHTS:		.word 0x0000003F //(00111111)b = 0 on the HEX LEDs
			.word 0x00000006 //(00000110)b = 1 on the HEX LEDs
			.word 0x0000005B //(01011011)b = 2 on the HEX LEDs
			.word 0x0000004F //(01001111)b = 3 on the HEX LEDs
			.word 0x00000066 //(01100110)b = 4 on the HEX LEDs
			.word 0x0000006D //(01101101)b = 5 on the HEX LEDs
			.word 0x0000007D //(01111101)b = 6 on the HEX LEDs
			.word 0x000000007 //(00000111)b = 7 on the HEX LEDs	
			.word 0x0000007F //(01111111)b = 8 on the HEX LEDs	
			.word 0x00000067 //(01100111)b = 9 on the HEX LEDs	

			.word 0x00000077 //(01110111)b = A on the HEX LEDs
			.word 0x0000007C //(01111100)b = b on the HEX LEDs
			.word 0x00000039 //(00111001)b = C on the HEX LEDs
			.word 0x0000005E //(01011110)b = d on the HEX LEDs
			.word 0x00000079 //(01111001)b = E on the HEX LEDs
			.word 0x00000071 //(01110001)b = F on the HEX LEDs

	.end
