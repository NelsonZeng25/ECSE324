#include <stdio.h>

#include "./drivers/inc/LEDs.h"
#include "./drivers/inc/slider_switches.h"
#include "./drivers/inc/pushbuttons.h"
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
	//HEX_write_ASM(HEX1 | HEX4, 15);
	//HEX_flood_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5);
	HEX_write_ASM(HEX1, 15);
	while(1){
		write_LEDs_ASM(read_slider_switches_ASM());
		if(read_slider_switches_ASM() & 0x200){
			HEX_clear_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5); //should clear all
		}else{
			HEX_flood_ASM(HEX4 | HEX5);
			char hex_val = (0xF & read_slider_switches_ASM());
			int pushbutton = (0xF & read_PB_data_ASM());
			HEX_write_ASM(pushbutton, hex_val);
		}
	}
	return 0;
}
