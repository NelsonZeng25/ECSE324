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
	float index = (f*t) - 48000 * (((int) ((f * t) / 48000)));				// Gets the remainder (i.e (f*t) % 48000)
	int x = (index == (int) index)? (int) index : ((int) (index)) + 1; 		// x holds upper int of index
	int y = (int) index;													// y holds lower int of index
	float signal;
	if (x!=y) {							// Check if index isn't a whole number
		float remainder = index - y;	// Get remainder
		signal = (1-remainder)*sine[y] + remainder * sine[x];	// Linear interpolation of wavetable
	} else {
		signal = sine[y];			// If index is a whole number, simply get the index of the wavetable
	}

	return signal;
}


int main() {
	// Audio Part 0
	
	int hi = 0x00FFFFFF; 	// High signal
	int lo = 0x00000000;	// Low signal
	int x = hi;
	int i;
	while(1){
		for(i = 0 ; i < 240;){ 			// To get a frequency of 100Hz, there must be 480 samples per full cycle (48000 Samples/s / 100 Hz)
			if (audio_write_ASM(x)) {
				i++;	
			}
		}
		x =(x==hi)?lo:hi;				// Invert the x setting (high -> low, low -> high)
	}
	

	
	// Control Waves Part 2
	int time = 0;				// Initialize time variable
	float amplitude = 1.5;		// Initialize volume amplitude to 1.5

	HPS_TIM_config_t hps_tim;	// Configure timer 0
	hps_tim.tim = TIM0;
	hps_tim.timeout = 20;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;
	HPS_TIM_config_ASM(&hps_tim);
	int_setup(1, (int []){199});	// Set up interrupts

	float signal = 0;				// Initialize total signal variable
	int i = 0;
	int n[8] = {0,0,0,0,0,0,0,0};	// Initialize key press checks
	char *data = 0;					// Initialize data variable
	int isPressed = 1;				// 1 = pressed, 0 = released
	float sounds[8][48000];			// 2 dimensional arrays to contain all the waves of different frequencies at all time (i.e [0, 48000])

	// Calculate all waves for every frequency at every possible time
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
	time = 0;	// Reset time to 0

	while(1) {
		signal = 0;
		if(hps_tim0_int_flag && read_ps2_data_ASM(data)) { // At every tim0 interrupt, check if key is pressed and assign scan code to data
			switch (*data) {
				case 0x1C:		// A
					n[0] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x1B:		// S
					n[1] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x23:		// D
					n[2] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x2B:		// F
					n[3] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x3B:		// J
					n[4] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x42:		// K
					n[5] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x4B:		// L
					n[6] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0x4C:		// ;
					n[7] = (isPressed) ? 1 : 0;
					isPressed = 1;
					break;
				case 0xF0:		// key press release scan code
					isPressed = 0;
					break;
				case 0x4E:		// '-' key for volume down
					amplitude = (amplitude <= 0) ? 0 : amplitude - 0.5;
					break;
				case 0x55:		// '=' key for volume up
					amplitude = (amplitude >= 5) ? 5 : amplitude + 0.5;
					break;
				default:		// If none of those keys were pressed, assume you didn't press anything
					isPressed = 0;
					break;
			}
		}

		// Check which keys were pressed and add the waves together at this specific time
		for(i = 0; i < 8; i++) {
			if(n[i] != 0) signal += sounds[i][time];
		}
		
		// Scale signal with amplitude (i.e volume)
		signal *= amplitude;

		// At tim0 interrupt and when audio is properly written, reset flag and increment time
		if (hps_tim0_int_flag && audio_write_data_ASM(signal, signal)){
			hps_tim0_int_flag = 0;
			time++;
		}
		// If time is over 48000, reset it back to 0
		if(time >= 48000) time = 0;
	}
	
	return 0;
}
