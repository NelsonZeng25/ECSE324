         .text
		 .global _start
_start:
 			LDR R4, =RESULT			//points to result
			LDR R2, [R4, #4]   		// holds the length of list
			ADD R3, R4, #8			// points to the first number
			       					// R0 holds the first number

LOOP:  		SUBS R2, R2, #1     	// counter
			BEQ DONE				// branch to done when loop ends
			LDR R0, [R3]			// R0 holds the first number
			ADD R3, R3, #4			// points to second number 			
			LDR R1, [R3]			// R1 holds the second number
			CMP R0, R1			
			BGT SWAP           		// If not, branch to min
			B LOOP				

SWAP:		ADD R7, R0, #0			// R7 holds smaller number
			ADD R8, R1, #0			// R8 holds the bigger number
			STR R7, [R3]			// Put the bigger number in R7
			SUB R3, R3, #4			// P3 points to the previous number
			STR R8, [R3]			// R8 now holds the smaller number
			ADD R3, R3, #4			// R3 points to current number
			LDR R2, [R4, #4]		//reset R2 to hold the number of numbers in the list
			ADD R3, R4, #8 			//reset R3 so it holds the first number in the list
			B LOOP				

DONE:		LDR R5, [R3]
			LDR R6, [R4, #8]
			SUB R4, R5, R6

END: 		B END					//infinite loop

RESULT:		.word	0
N:			.word	7
NUMBERS:	.word	4, 5, 3, 6
			.word	1, 8, 2
