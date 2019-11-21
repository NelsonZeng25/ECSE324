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

int main() {
	// Audio Part 0
	int hi = 0x00FFFFFF; 	// High signal
	int lo = 0x00000000;	// Low signal
	int x = hi;
	int i;
	while(1){
		for(i = 0 ; i < 240;){ 			// To get a frequency of 100Hz, there must be 480 samples per full cycle
			if(audio_write_ASM(x))		// Therefore, there must be 240 samples per half cycle
				i++;
		}
		x =(x==hi)?lo:hi;				// Invert the x setting (high -> low, low -> high)
	}


	return 0;
}
