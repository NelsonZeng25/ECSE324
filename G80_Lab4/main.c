#include <stdio.h>

#include "./drivers/inc/VGA.h"
#include "./drivers/inc/ps2_keyboard.h"

void test_char() {
	int x,y;
	char c = 0;
	
	for (y=0; y <= 59; y++) {
		for (x=0; x<= 79; x++) {
			VGA_write_char_ASM(x, y, c++);
		}
	}
}

void test_byte() {
	int x,y;
	char c = 0;
	
	for (y=0; y<= 59; y++) {
		for (x=0; x<= 79; x+=3) {
			VGA_write_byte_ASM(x, y, c++);
		}
	}
}

void test_pixel() {
	int x,y;
	unsigned short colour = 0;
	
	for (y=0; y<=239; y++) {
		for (x=0; x<=319; x++) {
			VGA_draw_point_ASM(x,y,colour++);
		}
	}
}

int main() {
	
	// PART 1 PUSHBUTTONS
	while(1) {
		int pushbutton = 0xF & read_PB_data_ASM();		// Get pushbutton value
		
		switch (pushbutton) {
			case 1:										// Check if PB0 was pressed
				if (read_slider_switches_ASM()) {		// Check if a slider is on
					test_byte();
				} else { 
					test_char(); 
				}
				break;
			case 2:										// Check if PB1 was pressed
				test_pixel();
				break;
			case 4:										// Check if PB2 was pressed
				VGA_clear_charbuff_ASM();
				break;
			case 8:										// Check if PB2 was pressed
				VGA_clear_pixelbuff_ASM();
				break;
		}
	}
	

	// PART 2 KEYBOARD
	int x = 0;
	int y = 0;
	int ps2 = 0;
	char *data = NULL;
	
	VGA_clear_charbuff_ASM();						// Initially clear screen
	VGA_clear_pixelbuff_ASM();
	while(1) {
		ps2 = read_PS2_data_ASM(data);				// Get RVALID bit and if it's valid, map PS2 data to data var
		if (ps2) {
			VGA_write_byte_ASM(x, y, *data);		// Write to screen
			x += 3;									// increment by 3
		}
		if (x > 79) {								// Check for x bounds
			x = 0;
			y++;
		}
		if (y > 59) {								// Check for y bounds
			VGA_clear_charbuff_ASM();				// Clear screen when it's filled out
			y = 0;									// reset y
		}
	}
	return 0;
}
