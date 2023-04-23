#include "nu32dip.h" // constants, functions for startup and UART
#include "spi.h"

void blink(int, int); // blink the LEDs function
void delay(void);

int main(void) {
  char message[100];
  
  int m = 0;
  int n = 0;
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  initSPI();
  while(1) {
      unsigned short send = 1000; //  
      unsigned char first = (0b0111 << 12) | (send >> 8);
      unsigned char second = (send & 0xFF) << 2;
      CS = 0;
      spi_io(first);
      spi_io(second);
      CS = 1;
      delay();
  }
  
}

void delay(void) {
    int t = 0;
    while(t < 48000000) {
    ;
    }
}