#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include <stdio.h>

void blink(int, int); // blink the LEDs function

int main(void) {
    char m_in[100];
    char message[100];
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    init_mpu6050();
	
	// char array for the raw data
    uint8_t raw[IMU_ARRAY_LEN];
	// floats to store the data
	float store[IMU_ARRAY_LEN];
    
	// read whoami
    uint8_t whomst = whoami();
	// print whoami
    //sprintf(message, "%d", whomst);
    //NU32DIP_WriteUART1(message);
	// if whoami is not 0x68, stuck in loop with LEDs on
    if (whomst != 0x68) {
        while(1) {
            blink(1, 1000);
        }
    }
	// wait to print until you get a newline
    // NU32DIP_ReadUART1(m_in,100);

    while (1) {
		// use core timer for exactly 100Hz loop
        _CP0_SET_COUNT(0);
        blink(1, 5);

        // read IMU
        burst_read_mpu6050(raw);
        
		// convert data
       
        for (int i=0; i<IMU_ARRAY_LEN; i++) {
           
            
            float x = conv_xXL(raw);
            float y = conv_yXL(raw);
            float z = conv_zXL(raw);
            float xg = conv_xG(raw);
            float yg = conv_yG(raw);
            float zg = conv_zG(raw);
            float t = conv_temp(raw);
            
            //sprintf(message, "%f\r\n", z);
            sprintf(message, "x: %f  y: %f  z: %f xg: %f  yg:  %f  zg: %f  t: %f\r\n", x, y, z, xg, yg ,zg, t); 
            NU32DIP_WriteUART1(message);
        }
  
        // print out the data
        

        while (_CP0_GET_COUNT() < 48000000 / 2 / 100) {
        }
    }
}

// blink the LEDs
void blink(int iterations, int time_ms) {
    int i;
    unsigned int t;
    for (i = 0; i < iterations; i++) {
        NU32DIP_GREEN = 0; // on
        NU32DIP_YELLOW = 1; // off
        t = _CP0_GET_COUNT(); // should really check for overflow here
        // the core timer ticks at half the SYSCLK, so 24000000 times per second
        // so each millisecond is 24000 ticks
        // wait half in each delay
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }

        NU32DIP_GREEN = 1; // off
        NU32DIP_YELLOW = 0; // on
        t = _CP0_GET_COUNT(); // should really check for overflow here
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }
    }
}

