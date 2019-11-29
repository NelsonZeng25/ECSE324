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
float make_wave(float f, int t) {
	float index = (f*t) - 48000 * (((int) ((f * t) / 48000)));
	int x = (index == (int) index)? (int) index : ((int) (index)) + 1; 		// x holds upper int of index
	int y = (int) index;											// y holds lower int of index
	float signal;
	if (x!=y) {						// Check if index isn't a whole number
		float remainder = index - y;	// Get remainder
		signal = (1-remainder)*sine[y] + remainder * sine[x];	// Linear interpolation of wavetable
	} else {
		signal = sine[y];			// If index is a whole number, simply get the index of the wavetable
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
	int time = 0;
	float amplitude = 1.5;

	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0;
	hps_tim.timeout = 20;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;

	HPS_TIM_config_ASM(&hps_tim);
	int_setup(1, (int []){199});

	float signal = 0;
	int i = 0;
	int n [8] = {0,0,0,0,0,0,0,0};
	char *data = 0;
	int isPressed = 1;	// 1 = pressed, 0 = released
	float sounds[8][48000];

	for(time=0; time < 48000; time++){
		sounds[0][time] = make_wave(130.813, time); //generates low C
		sounds[1][time] = make_wave(146.832, time); //generates D
		sounds[2][time] = make_wave(164.814, time); //generates E
		sounds[3][time] = make_wave(174.614, time); //generates F
		sounds[4][time] = make_wave(195.998, time); //generates G
		sounds[5][time] = make_wave(220.000, time); //generates A
		sounds[6][time] = make_wave(246.942, time); //generates B
		sounds[7][time] = make_wave(261.626, time); //generates high C
	}
	time = 0;

	while(1) {
		signal = 0;
		if(hps_tim0_int_flag && read_ps2_data_ASM(data)) { //reads one key pressed
			switch (*data) {
				case 0x1C:
					n[0] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x1B:
					n[1] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x23:
					n[2] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x2B:
					n[3] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x3B:
					n[4] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x42:
					n[5] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x4B:
					n[6] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x4C:
					n[7] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0xF0:
					isPressed = 0;
					break;
				case 0x4E:
					amplitude = (amplitude <= 0) ? 0 : amplitude - 0.5;
					break;
				case 0x55:
					amplitude = (amplitude >= 5) ? 5 : amplitude + 0.5;
					break;
			}
			/*
			if( *data == 0x1C) {
				n[0] = (isPressed) ? 1 : 0;
				isPressed = 1;
			}
			else if( *data == 0x1B) {
				n[1] = (isPressed) ? 1 : 0;
				isPressed = 1;
			}
			else if( *data == 0x23) {
				n[2] = (isPressed) ? 1 : 0;
				isPressed = 1;
			}
			else if( *data == 0x2B) {
				n[3] = (isPressed) ? 1 : 0;
				isPressed = 1;
			}	
			else if( *data == 0x3B) {
				n[4] = (isPressed) ? 1 : 0;
				isPressed = 1;
			}
			else if( *data == 0x42) {
				n[5] = (isPressed) ? 1 : 0;
				isPressed = 1;
			}
			else if( *data == 0x4B) {
				n[6] = (isPressed) ? 1 : 0;
				isPressed = 1;
			}
			else if( *data == 0x4C) {
				n[7] = (isPressed) ? 1 : 0;
				isPressed = 1;
			}
			else if( *data == 0xF0 ){
			 isPressed = 0; 		//key is released
			}
			//VOLUME DOWN
			else if( *data == 0x4E) {
				if (amplitude == 0) amplitude = 0;
				else amplitude -= 0.5;
			}	
			else if( *data == 0x55) {
				if (amplitude == 100) amplitude = 100;
				else amplitude += 0.5; 
			}
			*/	
		}

		for(i = 0; i < 8; i++) {
			if(n[i] != 0) signal += sounds[i][time];
		}
		
		signal *= amplitude;

		if (hps_tim0_int_flag && audio_write_data_ASM(signal, signal)){
			hps_tim0_int_flag = 0;
			time++;
		}
		if(time >= 48000) time = 0;
	}
	
	return 0;
}
