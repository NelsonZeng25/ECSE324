#include "./drivers/inc/vga.h"
#include "./drivers/inc/ISRs.h"
#include "./drivers/inc/LEDs.h"
#include "./drivers/inc/audio.h"
#include "./drivers/inc/HPS_TIM.h"
#include "./drivers/inc/int_setup.h"
#include "./drivers/inc/wavetable.h"
#include "./drivers/inc/pushbuttons.h"
#include "./drivers/inc/ps2_keyboard.h"
#include "./drivers/inc/HEX_displays.h"
#include "./drivers/inc/slider_switches.h"

// Make Waves Part 1
float make_wave(double f, int t) {
	double index = (f*t) - 48000 * (((int) ((f * t) / 48000)));
	int x = (index == (int) index)? index : (int) (index) + 1; 		// x holds upper int of index
	int y = (int) index;											// y holds lower int of index
	float signal;
	if (x!=y) {						// Check if index isn't a whole number
		int remainder = index - y;	// Get remainder
		signal = (1-remainder)*sine[y] + remainder * sine[x];	// Linear interpolation of wavetable
	} else {
		signal = sine[x];			// If index is a whole number, simply get the index of the wavetable
	}

	return signal;
}


int main() {
	// Audio Part 0
	/*
	int hi = 0x00FFFFFF; 	// High signal
	int lo = 0x00000000;	// Low signal
	int x = hi;
	int i;
	while(1){
		for(i = 0 ; i < 240;){ 			// To get a frequency of 100Hz, there must be 480 samples per full cycle (48000 Samples/s / 100 Hz)
			if(audio_write_ASM(x))		// Therefore, there must be 240 samples per half cycle (480 samples/cycle / 2)
				i++;
		}
		x =(x==hi)?lo:hi;				// Invert the x setting (high -> low, low -> high)
	}
	*/

	// Control Waves Part 2
	int time = 0, amplitude = 1;
	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0;
	hps_tim.timeout = 20;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;

	HPS_TIM_config_ASM(&hps_tim);
	int_setup(1, (int []){199});

	float signal =0;
	int i = 0;
	int keys = 0;

	float frequencyTable [] = {130.813, 146.832, 164.814, 174.615, 195.998, 220.000, 246.942, 261.626};
	float f [8] = {0,0,0,0,0,0,0,0};

	char *data;
	int status = 1;	// 1 = pressed, 0 = released
	while(1) {
		i = 0;
		if(read_ps2_data_ASM(data)) { //reads one key pressed
			if( *data == 0x1C) {
				if(status == 1){
					f [0] = frequencyTable[0];
				}
				else{
					//Key is released
					f [0] = 0;
					status = 1;
				}
			}
			if( *data == 0x1B) {
				if(status == 1){
					f [1] = frequencyTable[1];
				}
				else{
					//Key is released
					f [1] = 0;
					status = 1;
				}
			}
			if( *data == 0x23) {
				if(status == 1){
					f [2] = frequencyTable[2];
				}
				else{
					//Key is released
					f [2] = 0;
					status = 1;
				}
			}
			if( *data == 0x2B) {
				if(status == 1){
					f [3] = frequencyTable[3];
				}
				else{
					//Key is released
					f [3] = 0;
					status = 1;
				}
			}
			
			if( *data == 0x3B) {
				if(status == 1){
					f [4] = frequencyTable[4];
				}
				else{
					//Key is released
					f [4] = 0;
					status = 1;
				}
			}
			if( *data == 0x42) {
				if(status == 1){
					f [5] = frequencyTable[5];
				}
				else{
					//Key is released
					f [5] = 0;
					status = 1;
				}
			}
			if( *data == 0x4B) {
				if(status == 1){
					f [6] = frequencyTable[6];
				}
				else{
					//Key is released
					f [6] = 0;
					status = 1;
				}
			}
			if( *data == 0x4C) {
				if(status == 1){
					f [7] = frequencyTable[7];
				}
				else{
					//Key is released
					f [7] = 0;
					status = 1;
				}
			}
			if( *data == 0xf0 ){
				status = 0; //key is released
			}
			//VOLUME DOWN
			if( *data == 0x54) {
				if (amplitude == 0) amplitude = 0;
				else amplitude -= 10;
			}	
			if( *data == 0x5B) {
				if (amplitude == 100) amplitude = 100;
				else amplitude += 10; 
			}	
		}
		signal = 0;
		for(i = 0; i < 8; i++) {
			if(f [i] != 0) signal += make_wave(f [i], time);
		}
		signal = amplitude * signal;

		if (hps_tim0_int_flag){
			hps_tim0_int_flag = 0;
			audio_write_data_ASM(signal, signal);
			time += 1;
			time = time % 48000;
		}
	}
	
	return 0;
}
