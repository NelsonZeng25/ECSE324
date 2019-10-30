#include <stdio.h>

#include "./drivers/inc/LEDs.h"
#include "./drivers/inc/slider_switches.h"

#include "./drivers/inc/HEX_displays.h"

int main() {
	
	/*
	while(1) {
		write_LEDs_ASM(read_slider_switches_ASM());
HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5
	}
	*/

	//HEX_clear_ASM(HEX1 | HEX3 | HEX4 | HEX5);
	//HEX_flood_ASM(HEX0 | HEX2 | HEX3 | HEX4 | HEX5);
	HEX_write_ASM(HEX1 | HEX4, 15);
	return 0;
}
