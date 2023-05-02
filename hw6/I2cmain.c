#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"

void blink(int, int); // blink the LEDs function
void IOset(void);
void OLATlight(void);
void OLATdark(void);
unsigned char readGP0(void);
void delay(void);

int main(void) {
  char message[100];
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  IOset();
  OLATdark();
  while(1) {
      OLATdark();
      blink(1, 1000);      
      unsigned char recv = readGP0();
      int i = !(recv && 0b1);
      // sprintf(message, "%d\r\n", i);
      // NU32DIP_WriteUART1(message);
      if (i) {
          OLATlight();
          delay();
      }
  }
}


void IOset(void) {
  i2c_master_setup();
  // start
  i2c_master_start(); 
  // address
  unsigned char address = 0b0100000 << 1;
  i2c_master_send(address);
  // data frame 1
  unsigned char iodir= 0x00;
  i2c_master_send(iodir);
  // data frame 2
  unsigned char iobits = 0b01111111;
  i2c_master_send(iobits);
  // stop
  i2c_master_stop();
}


void OLATlight(void) {
  i2c_master_setup();
  // start
  i2c_master_start(); 
  // address
  unsigned char address = 0b0100000 << 1;
  i2c_master_send(address);
  // data frame 1
  unsigned char olat= 0x0A;
  i2c_master_send(olat);
  // data frame 2
  unsigned char olatbits = 0b10000000;
  i2c_master_send(olatbits);
  // stop
  i2c_master_stop();
}

void OLATdark(void) {
  i2c_master_setup();
  // start
  i2c_master_start(); 
  // address
  unsigned char address = 0b0100000 << 1;
  i2c_master_send(address);
  // data frame 1
  unsigned char olat= 0x0A;
  i2c_master_send(olat);
  // data frame 2
  unsigned char olatbits = 0b00000000;
  i2c_master_send(olatbits);
  // stop
  i2c_master_stop();
}

unsigned char readGP0(void) {
  // start
  i2c_master_start();
  // send addy with write bit
  unsigned char address = 0b0100000 << 1;
  i2c_master_send(address);
  // send register
  unsigned char gpio = 0x09;
  i2c_master_send(gpio);
  // restart
  i2c_master_restart(); 
  // send addy with read bit 
  unsigned char addy2 = 0b01000001 ;
  i2c_master_send(addy2);
  // recv
  unsigned char recv = i2c_master_recv();
  // ack w/ 1
  i2c_master_ack(1);
  // stop
  i2c_master_stop();
  return recv;
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

void delay(void) {
    int t = 0;
    while(t < 24000000)
        t++;
}
		
