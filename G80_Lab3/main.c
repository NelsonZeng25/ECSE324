#include <stdio.h>

#include "./drivers/inc/LEDs.h"
#include "./drivers/inc/slider_switches.h"
#include "./drivers/inc/pushbuttons.h"
#include "./drivers/inc/HEX_displays.h"
#include "./drivers/inc/HPS_TIM.h"
#include "./drivers/inc/ISRs.h"
#include "./drivers/inc/address_map_arm.h"
#include "./drivers/inc/int_setup.h"
int main() {
	
	

	//HEX_clear_ASM(HEX1 | HEX3 | HEX4 | HEX5);
	//HEX_flood_ASM(HEX0 | HEX2 | HEX3 | HEX4 | HEX5);
	//HEX_write_ASM(HEX1 | HEX4, 15);


	/*
	Part 2, HEX Display and Pushbuttons
	Infinite loop with while loop with writeLEDs constantly running
	Check if the last switch is turned on (0x200 == 1000 0000 0000) and clear all HEX if it's on
	Else, always flood HEX4-5 and read the slider value of the first 4 sliders (0xF == 0000 0000 1111)
	then read the pushbutton value to check which HEX we want to write on.
	*/
	while(1){
		write_LEDs_ASM(read_slider_switches_ASM());
		if (read_slider_switches_ASM() & 0x200) {
			HEX_clear_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5); 
		} else {
			HEX_flood_ASM(HEX4 | HEX5);
			char hex_val = (0xF & read_slider_switches_ASM());
			int pushbutton = (0xF & read_PB_data_ASM());
			HEX_write_ASM(pushbutton, hex_val);
		}
	}


	/*
	Part 3, Timer
	Provided code
	*/
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



	/*
	Part 3, Stopwatch
	First, config both timers (TIM0 for the timer, TIM1 for pushbutton polling)
	Then enter infinite loop where we constantly check if the timer was started which increments in 10 milliseconds and
	increments seconds and minutes accordingly
	It also write the values of the time units to the corresponding HEX
	It also checks if the pushbuttons are pressed and either Starts, Stops or Resets the timer
	*/
	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0;
	hps_tim.timeout = 10000;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;
	HPS_TIM_config_ASM(&hps_tim); // Config timer 0
	
	// This timer is for the pushbutton polling
	HPS_TIM_config_t hps_tim_pb;
	hps_tim_pb.tim = TIM1;
	hps_tim_pb.timeout = 5000;
	hps_tim_pb.LD_en = 1;
	hps_tim_pb.INT_en = 1;
	hps_tim_pb.enable = 1;
	HPS_TIM_config_ASM(&hps_tim_pb); // Config timer 1

	// Declare time unit variables
	int milli = 0, second = 0, minute = 0, timerstart = 0;
	while (1) {
		// Checks if timer was started
		if (HPS_TIM_read_INT_ASM(TIM0) && timerstart) {
			HPS_TIM_clear_INT_ASM(TIM0);
			milli += 10; 					// Timer increments every 10 milliseconds
			// When milliseconds reach 1000 (1s), increment seconds and reset milliseconds
			if (milli >= 1000) {
				milli = 0;
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

			HEX_write_ASM(HEX0, (milli % 100) / 10);	// Milli increments in 10 microseconds so we must modulo and divide by 10 to get the digit at 0.01 s
			HEX_write_ASM(HEX1, milli / 100);			// Divide Milli by 100 to get 0.1s digit
			HEX_write_ASM(HEX2, second % 10);			// Modulo seconds to get 1s digit
			HEX_write_ASM(HEX3, second / 10);			// Divide by 10 to get 10s digit
			HEX_write_ASM(HEX4, minute % 10);			// Same for minutes
			HEX_write_ASM(HEX5, minute / 10);
		}
		//Pushbuttons polling
		if (HPS_TIM_read_INT_ASM(TIM1)) {
			int pushbutton = 0xF & read_PB_data_ASM();
			
			//Start (Check if button at 1000 = 8 is pressed)
			if ((pushbutton & 8) && (!timerstart)) {
				timerstart = 1;
			}
			//Stop	(Button at 0100 = 4)
			else if ((pushbutton & 4) && (timerstart)) {
				timerstart = 0;
			}
			//Reset (Button at 0010 = 2)
			else if (pushbutton & 2) {
				milli = 0;
				second = 0;
				minute = 0;
				timerstart = 0;

				HEX_write_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5, 0);
			}
		}
	}


	/*
	Part 4, Interrupt based Stopwatch
	Very similar to the Stopwatch from Part 3
	First, call init_setup for the TIM0 timer (199) and the Pushbuttons (73)
	Then enable interrupts for the Pushbuttons you want to use
	Config the TIM0 timer
	Same infinite loop with the time units as Part 3 but with interrupt flags
	Check if the PB flag was changed and depending on the flag, do the corresponding acion
	*/
	int_setup(2, (int[]) {73, 199});
	enable_PB_INT_ASM(PB3 | PB2 | PB1);

	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0;
	hps_tim.timeout = 10000;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;
	HPS_TIM_config_ASM(&hps_tim);

	int micro = 0, second = 0, minute = 0, timerstart = 0;

	while (1) {
		if (hps_tim0_int_flag && timerstart) {		// Check if the TIM0 flag is on to run timer
			hps_tim0_int_flag = 0;					// Reset flag of TIM0
			micro += 10; 

			if (micro >= 1000) {
				micro = 0;
				second++;
				if (second >= 60) {
					second = 0;
					minute++;
					if (minute >= 60) {
						minute = 0;
					}
				}
			}

			HEX_write_ASM(HEX0, (micro % 100) / 10);
			HEX_write_ASM(HEX1, micro / 100);
			HEX_write_ASM(HEX2, second % 10);
			HEX_write_ASM(HEX3, second / 10);
			HEX_write_ASM(HEX4, minute % 10);
			HEX_write_ASM(HEX5, minute / 10); 
		}
		
		// The pushbutton flag corresponds to which button is pressed (e.g if flag = 0100, PB2 is pressed)
		if (pb_int_flag != 0){
			// Start
			if (pb_int_flag == 8 && !timerstart) timerstart = 1;
			// Stop
			else if (pb_int_flag == 4 && timerstart) timerstart = 0;
			// Reset
			else if (pb_int_flag == 2) {
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
			pb_int_flag = 0;		// Reset pushbutton flag
		}
	}

	return 0;
}
