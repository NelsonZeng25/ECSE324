		.text
		.equ PB_DATA, 0xFF200050
		.equ PB_MASK, 0xFF200058
		.equ PB_EDGE, 0xFF20005C
		.global read_PB_data_ASM
		.global PB_data_is_pressed_ASM
		.global read_PB_edgecap_ASM
		.global PB_edgecap_is_pressed_ASM
		.global PB_clear_edgecap_ASM
		.global enable_PB_INT_ASM
		.global disable_PB_INT_ASM

read_PB_data_ASM:					// Access the pushbutton data register
		LDR R1, =PB_DATA			// R1 holds the contents of the pushbutton register
		LDR R0, [R1]				// Use R0 to pass arguments back
		BX LR		

PB_data_is_pressed_ASM:				// R0 contains which button to check, hot-one encoding
		LDR R1, =PB_DATA	
		LDR R2, [R1]				// R2 holds the contents of the pushbutton register
		AND R2, R2, R0				// AND R2 and R0 to check if pushbutton register content corresponds to the button we want to check
		CMP R2, R0
		MOVEQ R0, #1				// True, if pressed
		MOVNE R0, #0				// False, if not pressed
		BX LR

read_PB_edgecap_ASM:				// Access edgecapture register
		LDR R1, =PB_EDGE	
		LDR R0, [R1]				// Load pushbutton edgecap content to R0
		BX LR						// Use R0 to pass arguments back		
		
PB_edgecap_is_pressed_ASM:			//R0 contains which button to check, hot-one encoding
		LDR R1, =PB_EDGE
		LDR R2, [R1]	
		CMP R0, R2
		MOVEQ R0, #1
		MOVNE R0, #0
		BX LR

PB_clear_edgecap_ASM:				// R0 contains which pushbutton we want to clear
		LDR R1, =PB_EDGE
		STR R0, [R1] 				// Simply writing to it clears it
		BX LR

enable_PB_INT_ASM:					// R0 contains which button to enable, hot-one encoding
		LDR R1, =PB_MASK
		STR R0, [R1]				// Storing button (one hot encoded) to Mask register enables that button
		BX LR

disable_PB_INT_ASM:					// R0 contains which button to disable, hot-one encoding
		LDR R1, =PB_MASK			
		LDR R2, [R1]				// R2 holds mask register content
		BIC R2, R2, R0				// AND on the complement of R0
									// This will effectively turn off the corresponding bits (aka the buttons we want off)
		STR R2, [R1]				// Store result into the mask
		BX LR
		
		.end
