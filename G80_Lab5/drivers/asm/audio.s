	.text
	.equ left_fifo, 0xFF203048
	.equ right_fifo, 0xFF20304C
	.equ fifospace, 0xFF203044
	.global audio_write_ASM

audio_write_ASM:
			PUSH {R1-R5}
			LDR R1, =left_fifo 	// Loads the address of the data register for the left fifo
			LDR R2, =right_fifo // Loads the address of the data register for the right fifo
			LDR R3, =fifospace 	// Loads the address of the data register for the fifospace

			LDR R4, [R3, #16] 	// Loads the value of WSRC onto R4
			CMP R4, #0
			MOVEQ R0, #0 		// If it's 0, the subroutine returns 0 because the FIFO is full
			POPEQ {R1-R5}
			BXEQ LR
			LDR R5, [R3, #24] 	// Loads the value of WSLC onto R5
			CMP R5, #0
			MOVEQ R0, #0		// If it's 0, the subroutine returns 0 because the FIFO is full
			POPEQ {R1-R5}
			BXEQ LR

			STR R0, [R1] 		// If the FIFOs are not full, the data is stored in them
			STR R0, [R2]
			MOV R0, #1 			// and subroutine returns 1
			POP {R1-R5}
			BX LR

	.end
