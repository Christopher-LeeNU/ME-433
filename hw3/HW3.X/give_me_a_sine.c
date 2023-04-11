#include "nu32dip.h" // constants, functions for startup and UART
#include "math.h"

int main(void) {
  char message[100];
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  NU32DIP_WriteUART1("Hello\r\n");
  int i = 0;
  while(1) {
    if (NU32DIP_USER == 0) {
      for (i = 0; i < 100; i++) {

        float x = 1.65*sin(2*M_PI*((float) i)/100) + 1.5;

        sprintf(message, "%f\r\n", x);
        NU32DIP_WriteUART1(message); // send message back
        int t = 0;
        while (t < 240000) { // waiting 0.01 s
            t++;
        }

      }
    }
  }
}

		
