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


	int count0 = 0, count1 = 0, count2 = 0, count3 = 0;
	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0|TIM1|TIM2|TIM3;
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
		HEX_write_ASM(HEX0, (count0));
		}
		if (HPS_TIM_read_INT_ASM(TIM1)) {
			HPS_TIM_clear_INT_ASM(TIM1);
			if (++count1 == 16)
				count1 = 0;
			HEX_write_ASM(HEX1, (count1));
		}
		if (HPS_TIM_read_INT_ASM(TIM2)) {
			HPS_TIM_clear_INT_ASM(TIM2);
			if (++count2 == 16)
				count2 = 0;
		HEX_write_ASM(HEX2, (count2));
		}
		if (HPS_TIM_read_INT_ASM(TIM3)) {
			HPS_TIM_clear_INT_ASM(TIM3);
			if (++count3 == 16)
				count3 = 0;
			HEX_write_ASM(HEX3, (count3));
		}
	}

*/

	
	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0;
	hps_tim.timeout = 10000;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;
	HPS_TIM_config_ASM(&hps_tim); //Config timer 0
	
	//Pushbutton polling
	HPS_TIM_config_t hps_tim_pb;
	hps_tim_pb.tim = TIM1;
	hps_tim_pb.timeout = 5000;
	hps_tim_pb.LD_en = 1;
	hps_tim_pb.INT_en = 1;
	hps_tim_pb.enable = 1;
	HPS_TIM_config_ASM(&hps_tim_pb); //config timer 1
	
	//Declare time unit variables
	int micro = 0, second = 0, minute = 0, timerstart = 0;
	while (1) {
		// Checks if timer was started
		if (HPS_TIM_read_INT_ASM(TIM0) && timerstart) {
			HPS_TIM_clear_INT_ASM(TIM0);
			micro += 10; 				//Timer is for 10 milliseconds
			//When microseconds reach 1000 (1s), increment seconds and reset mircoseconds
			if (micro >= 1000) {
				micro = 0;
				second++;
				// Reset seconds when > 60 then increment minutes
				if (second >= 60) {
					second = 0;
					minute++;
					// Reset after 60 min
					if (minute >= 60) {
						minute = 0;
					}
				}
			}
			//Display every value and convert the count to ascii values
			HEX_write_ASM(HEX0, (micro % 100) / 10);	// Micro increments in 10 microseconds so we must modulo and divide by 10 to get the digit at 0.01 s
			HEX_write_ASM(HEX1, micro / 100);			// Divide Micro by 100 to get 0.1s digit
			HEX_write_ASM(HEX2, second % 10);			// Modulo seconds to get 1s digit
			HEX_write_ASM(HEX3, second / 10);			// Divide by 10 to get 10s digit
			HEX_write_ASM(HEX4, minute % 10);			// Same for minutes
			HEX_write_ASM(HEX5, minute / 10);
		}
		//Pushbuttons polling
		if (HPS_TIM_read_INT_ASM(TIM1)) {
			HPS_TIM_clear_INT_ASM(TIM1);
			int pushbutton = 0xF & read_PB_data_ASM();
			
			//Start
			if ((pushbutton & 8) && (!timerstart)) {
				timerstart = 1;
			}
			//Stop
			else if ((pushbutton & 4) && (timerstart)) {
				timerstart = 0;
			}
			//Reset
			else if (pushbutton & 2) {
				micro = 0;
				second = 0;
				minute = 0;
				timerstart = 0;

				HEX_write_ASM(HEX0, 0);
				HEX_write_ASM(HEX1, 0);
				HEX_write_ASM(HEX2, 0);
				HEX_write_ASM(HEX3, 0);
				HEX_write_ASM(HEX4, 0);
				HEX_write_ASM(HEX5, 0);
			}
		}
	}
	return 0;
}
