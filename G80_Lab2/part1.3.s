			.text
			.global _start

_start:
			LDR R1, =NUMBER
			LDR R0, [R1]
			BL FACTORIAL
			LDR R2, =RESULT
			STR R0, [R2]
			B END

FACTORIAL:	PUSH {R1, LR}
			MOV R1, R0
			CMP R0, #0
			BNE NOT_ZERO
			MOV R0, #1
			B FINAL

NOT_ZERO:	SUB R0, R0, #1
			BL FACTORIAL
			MOV R2, R1
			MUL R0, R0, R2

FINAL:		POP {R1, LR}
			BX LR

END:		B END

NUMBER:		.word 3
RESULT:		.word 0