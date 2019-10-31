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
	LDR R1, [R0]
	MOV R2, #0


CONFIG_LOOP:	CMP R2, #4
				B CONFIG_DONE
				ANDS R4, R1, #0x1
				BNE CONFIG
				LSL R1, R1, #1
				ADD R2, R2, #1
				B CONFIG_LOOP

CONFIG:
	PUSH {R1-R8}				// load word address of the chosen timer is at R3
	LDR R3, =HPS_TIM
	ADD R3, R2, LSL #2
	MOV R4, R3
	ADD R4, R4, #8 				// the control word address is 8 bytes after the load word
	
	// set the control word to all 0's
	MOV R5, #0
	STR R5, [R4]
	
	LDR R5, [R0, #4] 				// R5 is the timeout that we pass as argument
	CMP R2, #2
	MOVGE R6, #25 					// multiply the input period in usec according to which clock used
	MOVLT R6, #100 					// either x25 or x100
	MUL R5, R5, R6
	STR R5, [R3] 					// set corrected timeout value to Load register
	
	MOV R5, #0
	LDR R6, [R0, #8] // loads LD to correct place in control word
	LSL R6, R6, #1
	ADD R5, R5, R6

	LDR R6, [R0, #12] // loads INT to correct place in control word
	EOR R6, R6, #1
	LSL R6, R6, #2
	ADD R5, R5, R6 
	// I needs to be 0 to assert S, so flip the input

	LDR R6, [R0, #16] // load enable to control bit, (1=start 0=stop)
	ADD R5, R5, R6
	STR R5, [R4] // actually put the updated control word in place

	POP {R1-R8}
	BX LR

CONFIG_DONE:	POP {R1-R8,LR}
				BX LR

HPS_TIM_read_INT_ASM:	PUSH {R1-R8,LR}
						MOV R1, #0
						LDR R3, =HPS_TIM

READ_LOOP:	CMP R1, #4
			BEQ READ_DONE
			ANDS R2, R0, #0x1
			LDREQ R3, [R3, R1, LSL #2]
			LDREQ R0, [R3, #16]
			BEQ READ_DONE
			ADD R1, R1, #1
			B READ_LOOP

READ_DONE:	POP {R1-R8, LR}
			BX LR

HPS_TIM_clear_INT_ASM:	PUSH {R1-R8,LR}
						MOV R1, #0
						LDR R3, =HPS_TIM

CLEAR_LOOP:	CMP R1, #4
			BEQ CLEAR_DONE
			ANDS R2, R0, #0x1
			LDREQ R3, [R3, R1, LSL #2]
			LDREQ R4, [R3, #12]
			BEQ CLEAR_DONE
			ADD R1, R1, #1
			B CLEAR_LOOP

CLEAR_DONE:	POP {R1-R8, LR}
			BX LR

HPS_TIM:	.word 0xFFC08000, 0xFFC09000, 0xFFD00000, 0xFFD01000

.end
