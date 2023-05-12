#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include <stdio.h>
#include "ssd1306.h"
#include "font.h"

void blink(int, int); // blink the LEDs function
void other_delay(void);
void letter(unsigned char x, unsigned char y, char letter);
void phrase(unsigned char x, unsigned char y, char message[100]);

int main(void) {
    char message[100];
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    i2c_master_setup();
    ssd1306_setup();
    init_mpu6050();
    
    uint8_t raw[IMU_ARRAY_LEN];
    // sprintf(message, "Oluwaseyi Balogun\0");
    // phrase(120, 10, message);
    float fps = 0;
    int dt = 0;
    
    while(1) {
        blink(1, 500);
        /*
        ssd1306_drawPixel(64, 16, 1); // blinking
        ssd1306_update();
        other_delay();
        ssd1306_clear();
        ssd1306_update();
        other_delay();
        */ 
        
        _CP0_SET_COUNT(0); // checking fps
        burst_read_mpu6050(raw);
        float z = conv_zXL(raw);
        sprintf(message, "zXL:%f fps:%f\0", z, fps);
        phrase(127, 30, message);
        float dt = _CP0_GET_COUNT();
        fps = 48000000.0/2.0/dt;
    }
}

// write letter at position
void letter(unsigned char x, unsigned char y, char letter) {
    int ascii = letter;
    // get row, starts at 32 for space
    for (int i = 0; i<5; i++) { // loop through 5 columns
        for (int j = 0; j<8; j++) { // loop through 8 bits
            char val = ASCII[(ascii - 32)][i];
            int on = (val >> j) & 1;
            ssd1306_drawPixel((x-i), (y-j), on);
        }
    }
    ssd1306_update();
    
}

void phrase(unsigned char x, unsigned char y, char message[100]) {
    int i = 0;
    while (message[i] != '\0') {
        letter(x, y, message[i]);
        x = x - 5;
        i++;
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

void other_delay(void) {
    int t = 0;
    while (t < 48000000/2/20){
        t++;
    }
}

