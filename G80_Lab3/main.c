#include <stdio.h>

#include "./drivers/inc/LEDs.h"
#include "./drivers/inc/slider_switches.h"
#include "./drivers/inc/pushbuttons.h"
#include "./drivers/inc/HEX_displays.h"
#include "./drivers/inc/HPS_TIM.h"

int main() {
	
	/*
	while(1) {
		write_LEDs_ASM(read_slider_switches_ASM());
HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5
	}
	

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
*/

	int count0 = 0, count1 = 0, count2 = 0, count3 = 0;
	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0;
	hps_tim.timeout = 1000000;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;
	HPS_TIM_config_ASM(&hps_tim);
	while (1) {
		write_LEDs_ASM(read_slider_switches_ASM());
		if (HPS_TIM_read_INT_ASM(TIM0)) {
			HPS_TIM_clear_INT_ASM(TIM0);
			if (++count0 == 16)
				count0 = 0;
		HEX_write_ASM(HEX0, (count0+48));
		}
		if (HPS_TIM_read_INT_ASM(TIM1)) {
			HPS_TIM_clear_INT_ASM(TIM1);
			if (++count1 == 16)
				count1 = 0;
			HEX_write_ASM(HEX1, (count1+48));
		}
		if (HPS_TIM_read_INT_ASM(TIM2)) {
			HPS_TIM_clear_INT_ASM(TIM2);
			if (++count2 == 16)
				count2 = 0;
		HEX_write_ASM(HEX2, (count2+48));
		}
		if (HPS_TIM_read_INT_ASM(TIM3)) {
			HPS_TIM_clear_INT_ASM(TIM3);
			if (++count3 == 16)
				count3 = 0;
			HEX_write_ASM(HEX3, (count3+48));
		}
	}
	return 0;
}
