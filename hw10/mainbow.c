#include "nu32dip.h" // constants, functions for startup and UART
#include "ws2812b.h"

void blink(int, int); // blink the LEDs function

int main(void) {
  char message[100];
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  ws2812b_setup();
  int hues[8] = {0, 45, 90, 135, 180, 225, 270, 315};
  wsColor rainbow[8];
  /*
  wsColor r;
  r.r = 254;
  r.g = 0;
  r.b = 0;
  */
  int k;
  while (1) {
    blink(5, 500); // 5 times, 500ms each time
    for (int j = 0; j < 8; j++){
        for (int i = 0; i < 8; i++) {
            k = i + j;
            if ((i+j) > 8) {
                k = i+j - 7;
            }
            rainbow[i] = HSBtoRGB(hues[k], 1, 0.01);
            ws2812b_setColor(rainbow, 8);
            int t = 0;
            while (t < 2000000) {
                t++;
                }
            }
        }
  }
}

// blink the LEDs
void blink(int iterations, int time_ms){
	int i;
	unsigned int t;
	for (i=0; i< iterations; i++){
		NU32DIP_GREEN = 0; // on
		NU32DIP_YELLOW = 1; // off
		t = _CP0_GET_COUNT(); // should really check for overflow here
		// the core timer ticks at half the SYSCLK, so 24000000 times per second
		// so each millisecond is 24000 ticks
		// wait half in each delay
		while(_CP0_GET_COUNT() < t + 12000*time_ms){}
		
		NU32DIP_GREEN = 1; // off
		NU32DIP_YELLOW = 0; // on
		t = _CP0_GET_COUNT(); // should really check for overflow here
		while(_CP0_GET_COUNT() < t + 12000*time_ms){}
	}
}
		
