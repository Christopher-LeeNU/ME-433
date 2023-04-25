#include "nu32dip.h" // constants, functions for startup and UART
#include "spi.h"
#include <stdio.h>
#include <math.h>

int sine_vals[100];
int dorito_vals[100];

void blink(int, int); // blink the LEDs function
void delay(void);
void make_sine(void);
void make_dorito(void);

int main(void) {
  char message[200];
  
  int m = 0;
  int n = 0;
  
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  initSPI();
  make_sine();
  make_dorito();
  for (int i = 0; i < 100; i++) {
    sprintf(message, "%d\r\n", dorito_vals[i]);
    NU32DIP_WriteUART1(message);
  }
  while(1) {
    for (int i = 0; i < 100; i++) { 
        unsigned int val1 = sine_vals[i];
        unsigned short send = (0b111 << 12) | (val1 << 2);
        unsigned int val2 = dorito_vals[i];
        unsigned short send2 = (0b1111 << 12) | (val2 << 2);
        CS = 0;
        spi_io(send >> 8);
        spi_io(send);
        CS = 1;
        CS = 0;
        spi_io(send2 >> 8);
        spi_io(send2);
        CS = 1;
        delay();
    }
  }
}

void delay(void) {
    int t = 0;
    while(t < (48000000/1000)) {
    t++;
    }
}

void make_sine(void) {
    for (int i = 0; i < 100; i++) {
        sine_vals[i] = 1023/2 * sin(2* M_PI /50.0 * i) + (1023/2); 
    }
}

void make_dorito(void) {
    for (int i = 0; i < 100; i++) {
        if (i < 50) {
            dorito_vals[i] = 1023/50.0 * i;
        }
        else {
            dorito_vals[i] = 1023 - (1023/50.0 * (i - 50));
        }
    }
}