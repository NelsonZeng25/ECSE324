			.text
			.global _start

_start:		LDR R0, =NUMBERS
			LDR R1, =RESULT
			LDR R2, =N			// N variable
			LDR R8, [R2]		// Immediate N value
			LDR R7, =M			// M variable
			LDR R12, [R7]		// Immediate M value
			MOV R6, #0			// Sum variable
			MOV R3, #0			// Counter for NUMBERS
			MOV R4, #4			// Array size of NUMBERS
			MOV R9, #0			// variable to hold sums of N
			MOV R10, #0			// Counter for number of multiplications done
			CMP R8, R12			// Checks if m > n
			BGT SET_RESULT
			ADD R11, R8, #1		// Number of results (or multiplications)	if n=2,3

SET_RESULT:	ADD R11, R12, #1	// Number of results	if n=1 and m=3
			B SUM

SUM:		CMP R3, R4			// This adds all the numbers of the array together to get S
			BEQ RESET			// Stops when counter R3 is at array size R4
			ADD R3, R3, #1		// Increment counter
			LDR R5, [R0]		// R5 holds the number at index R0
			ADD R6, R6, R5		// Add number to R6
			ADD R0, #4			// Increment index
			B SUM

RESET:		LDR R0, =NUMBERS	// This resets the loop increments (R0 index and R3 counter)
			MOV R3, #1			// R3 counter starts at 1
			LDR R5, [R0]		// R5 holds the first numbers
			CMP R8, R12			// Checks if n = m (i.e. n=2, m=2)
			BEQ SUM_N			// For the case that n = 2, m = 2
			B SUM_N2			// For the case that n = 3, m = 1 or n = 1, m = 3

SUM_N:		CMP R3, R8			// Check if counter is done iterating
			BEQ MULTI
			ADD R3, R3, #1		// Increment counter R3
			LDR R7, [R0, #4]!	// Load R7 with the next number
			ADD R9, R7, R5		// Sum N is stored in R9
			B SUM_N
MULTI:		SUB R7, R6, R9		// R7 holds Sum M = S - Sum N
			MUL R7, R7, R9		// Multiply Sum M and Sum N and store it in R7
			STR R7, [R1]		// Store R7 in result
			ADD R1, R1, #4		// Increment result index
			ADD R10, R10, #1	// Increment result counter
			CMP R10, R11		// Checks if result counter R10 is done (R11 represents the number of results)
			BEQ SET_UP
			MOV R3, #1			// Reset counter for sums back to 1
			B SUM_N

SUM_N2:		CMP R10, R11				// Checks if result counter is done (R11 represents the number of results)
			BEQ SET_UP
			ADD R10, #1					// Increment result counter
			SUB R7, R6, R5				// R7 holds Sum M = S - Sum N
			MUL R7, R7, R5				// Multiply Sum M and Sum N and store it in R7
			STR R7, [R1], #4			// Store R7 to result and increment the index
			LDR R5, [R0, #4]!			// R5 (Sum N) now points to the next number
			B SUM_N2

SET_UP:		LDR R1, =RESULT				// R1 points to first number of results
			LDR R2, =MIN_NUM
			LDR R3, =MAX_NUM
			LDR R5, [R1]				// R5 will hold Max value
			LDR R6, [R1]				// R6 will hold Min value
			STR R5, [R3]				// initialize min result variable with 1st number
			STR R6, [R2]				// Do the same with the max result variable
			B FINAL						// Both initialized at first number

FINAL:		SUBS R11, R11, #1			// Decrement result's array size
			BEQ END						// End when done iterating
			ADD R1, R1, #4				// Increment R1 to next number
			LDR R7, [R1]				// R7 holds the next number in the list
			CMP R7, R5					// Compare R7 with R5
			BGE	MAX						// If R7 is bigger than R5, go to MAX
			CMP	R7,	R6					// Compare R7 with R6
			BLE MIN						// If R6 is smaller than R1, go to MIN
			B FINAL

MIN:		STR R7, [R2]				// Update R6 with new minimum
			B FINAL
MAX:		STR R7, [R3]					// Update R5 with new maximum
			B FINAL

END:    	B END               // infinite loop!

RESULT: 	.word 0,0,0,0		//memory assigned for result location
N:      	.word 2
M:			.word 2
NUMBERS:	.word 1, 2, 3, 4	//list
MIN_NUM:	.word 0
MAX_NUM:	.word 0
