			.text
			.global _start

_start:
			LDR R1, =NUMBER			// R1 holds the address to number
			LDR R0, [R1]			// R0 holds N
			BL FACTORIAL			// Calls factorial()
			LDR R2, =RESULT
			STR R0, [R2]			// Store R0 into results
			B END

FACTORIAL:	PUSH {R1, LR}			// Remembers the state of R1 and LR
			MOV R1, R0				// Store intial R0 value into R1
			CMP R0, #0				// Checks when R0 is 0
			BNE NOT_ZERO			// Branch out to compute (n-1)
			MOV R0, #1
			B FINAL

NOT_ZERO:	SUB R0, R0, #1			// R0 = n-1
			BL FACTORIAL			// Recursively call Factorial()
									// R0 now contains factorial(n-1)
			MUL R0, R0, R1			// R0 stores factorial(n-1) * n

FINAL:		POP {R1, LR}			// Restore previous version of R1 and LR
			BX LR					// Exit factorial call

END:		B END

NUMBER:		.word 3
RESULT:		.word 0
