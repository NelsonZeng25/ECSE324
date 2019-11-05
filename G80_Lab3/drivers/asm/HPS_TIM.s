   		.text
		.equ TIM_0, 0xFFC08000
    	.equ TIM_1, 0xFFC09000
		.equ TIM_2, 0xFFD00000
		.equ TIM_3, 0xFFD01000
    	.global HPS_TIM_config_ASM
		.global HPS_TIM_read_INT_ASM
		.global HPS_TIM_clear_INT_ASM

HPS_TIM_config_ASM:
			PUSH {R1-R8,LR}
			LDR R1, [R0]		// R1 holds the TIM parameter
			MOV R2, #0			// Counter to keep track of which Timer we're on


CONFIG_LOOP:CMP R2, #4			// Checks when we're done going through all the timers
			BEQ CONFIG_DONE
			ANDS R4, R1, #1		// AND R1 with 1 since the Timers are one hot encoded
			BLNE CONFIG
			LSR R1, R1, #1		// Shift Right by 1 bit to check the next timer
			ADD R2, R2, #1		// Increment counter
			B CONFIG_LOOP

CONFIG:
			LDR R3, =HPS_TIM			// R3 = address of first TIMER
			LDR R3, [R3, R2, LSL #2]	// R3 = value of the chosen TIMER using the index R2 * 4
			MOV R4, R3					// Copy R3 into R4
			ADD R4, R4, #8 				// Make R4 point to the Control address which is 8 bytes after the first address 
			
			MOV R5, #0					// Set the control word to 0's
			STR R5, [R4]	
			
			LDR R5, [R0, #4] 			// R5 is the timeout that we passed as argument
			CMP R2, #2					// If the index is past or equal to 2, the timer has 25MHz. Else, it has 100MHz
			MOVGE R6, #25 				// Set R6 to either 25 or 100 (depending on timer)
			MOVLT R6, #100 				
			MUL R5, R5, R6				// Multiply timeout with the R6 (25 | 100) to get a consistent time between timers
			STR R5, [R3] 				// Set corrected timeout value to Load register
			
			MOV R5, #0					// R5 will hold the 3 bits used to change the Control word (i.e IME). Initialized to 0 at the beginning
			LDR R6, [R0, #8] 			// R6 holds the LD parameter
			LSL R6, R6, #1				// Shift R6 by 1 bit so LD is now on the 2nd bit 
			ADD R5, R5, R6				// Add that bit to R5 so it's now on the "M" of the "IME" control word

			LDR R6, [R0, #12] 			// R6 holds the INT parameter
			EOR R6, R6, #1				// Exclusive OR since I bit must be a 0 for the S-bit in the interrupt status register to be asserted
										// INT must be a 1 for the "I" to be 0
			LSL R6, R6, #2				// Shift R6 by 2 bits so the result is now on the "I" of the "IME" control word
			ADD R5, R5, R6 

			LDR R6, [R0, #16] 			// R6 holds the En paramater ( 1=start, 0=stop )
			ADD R5, R5, R6				// Add directly to R5 since "E" is the first bit of "IME"
			STR R5, [R4] 				// Store R5 into R4 to change the settings of the control word
			BX LR

CONFIG_DONE:	POP {R1-R8,LR}
				BX LR

HPS_TIM_read_INT_ASM:	PUSH {R1-R8,LR}
						MOV R1, #0			// Index of which Timer we're on
						LDR R3, =HPS_TIM	// R3 holds all the timers (with pointer initially on the first timer)

READ_LOOP:	CMP R1, #4						// Ends loop if no Timers were detected in the input
			BEQ READ_DONE
			ANDS R2, R0, #1					// AND 1 with the input	to check if that timer is inputted
			LDRNE R3, [R3, R1, LSL #2]		// Make R3 point to the corresponding Timer using the index 
			LDRNE R0, [R3, #16]				// R0 holds the S-bit of the Timer
			BNE READ_DONE
			LSR R0, R0, #1					// Right shift input to check next Timer input
			ADD R1, R1, #1					// Increment index
			B READ_LOOP

READ_DONE:	POP {R1-R8, LR}
			BX LR


/*
Very similar to the Read method above
 */
HPS_TIM_clear_INT_ASM:	PUSH {R1-R8,LR}
						MOV R1, #0
						LDR R3, =HPS_TIM

CLEAR_LOOP:	CMP R1, #4
			BEQ CLEAR_DONE
			ANDS R2, R0, #1
			LDRNE R3, [R3, R1, LSL #2]
			LDRNE R4, [R3, #12]				// Simply loading the F and S bits to a register clears it
			LDRNE R4, [R3, #16]
			LSR R0, R0, #1
			ADD R1, R1, #1
			B CLEAR_LOOP

CLEAR_DONE:	POP {R1-R8, LR}
			BX LR

HPS_TIM:	.word 0xFFC08000, 0xFFC09000, 0xFFD00000, 0xFFD01000

.end
