#include <xc.h>

// initialize SPI1
void initSPI() {
    // Pin B14 has to be SCK1
    // Turn of analog pins
    
    
    //...
    // Make an output pin for CS
    
    TRISBbits.TRISB13 = 0;
    
    
    //...
    //...
    // Set SDO1
    
    RPB11Rbits.RPB11R = 0b0011; 
    
    //...
    // Set SDI1
    
    
    
    //...

    // setup SPI1
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 1000; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 
    
    // Class notes
    /*
     CS to IO pin
     * #define CS LATbits.LATB3
     * TRISbits.TRISB3 = 0; // make IO pin
     * LATbits.LATB3 = 1; // set output high
     How to turn IO pin to SDO
     * Table 11.2 will show bit to SFR for
     * have to do spio() twice because it turns unsigned short into 2 unsigned chars
     * RPB11Rbits
     * 
     * for b12 turn analog off first or any analog pin with ANSELBbits.ANSELB12 = 0; or ANSELA = 0;
     */
}


// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}