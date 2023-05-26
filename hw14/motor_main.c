#include "nu32dip.h" // constants, functions for startup and UART

void blink(int, int); // blink the LEDs function
void init(void);
void delay(void);

int main(void) {
  char message[100];
  init();
  while(1) {
      OC1RS = 3000;
      delay();
      OC1RS = 6000;
      delay();
  }
}

void delay(void) {
    int t = 0;
    while (t < (24000000)) {
        t++;
    }
}

void init(void) {
    RPA0Rbits.RPA0R = 0b0101; // set b6 to OC1 
    
    T2CONbits.TCKPS = 4;     // Timer2 prescaler N=4 (1:4)
    PR2 = 60000 - 1;              // period = (PR2+1) * N * (1/48000000) = 6 kHz
    TMR2 = 0;                // initial TMR2 count is 0
    OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC1CONbits.OCTSEL = 0;   // Use timer2
    OC1RS = 6000; // 4500;             // duty cycle = OC1RS/(PR2+1) = 25%
    OC1R = 6000;              // initialize before turning OC1 on; afterward it is read-only
    T2CONbits.ON = 1;        // turn on Timer2
    OC1CONbits.ON = 1;       // turn on OC1
    
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
