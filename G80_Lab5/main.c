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

#include <math.h>

// Make Waves Part 1
float make_wave(float f, int t) {
	float index = (f * t) % 48000;
	int x = ceil(index);			// x holds upper int of index
	int y = floor(index);			// y holds lower int of index
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


	// Control Waves Part 2
	
	return 0;
}
