#include <xc.h>
#include <pic16lf1459.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "spi.h"
#include "user.h"

void SPI_init()
{
    SSPCON1bits.SSPEN = 1;      //1 - omoguci serial port, zbog upisa
    //SSPCON1bits.SSPEN = 0;    //0 - onemoguci serial port 
    SSPBUF = 0; //clear SPI buffer    
    
  //  ANSELA = 0x00; // Turn off analog inputs port A
   // ANSELB = 0x00; // Turn off analog inputs port B
   // ANSELC = 0x00; // Turn off analog inputs port C
    
    TRISBbits.TRISB6 = 0;   // SCK ,clock, port RB6, izlazni
    TRISBbits.TRISB4 = 1;   // SDI , serial data input,  port RB4, ulazni
    TRISCbits.TRISC7 = 0;   // SDO, serial data output, port RC7, izlazni
    TRISCbits.TRISC6 = 0;   // CSN, chip select not, port RC6, izlazni
    TRISAbits.TRISA4 = 0;   // CE, chip enable, port RA4, izlazni
    
    CSN = 1;                    // pin CSN disable
    CE = 0;                     // pin CE disable
    
    //Postavke na?ina prijenosa podataka
    SSPSTATbits.CKE = 1;    // 0 - Transmit occurs on transition from Idle to active clock state, 
                            // 1 - Transmit occurs on transition from Active to Idle clock state,
    SSPCON1bits.CKP = 0;    // 0 - idle state for a clock is a low level
                            // 1- idle state for a clock is a high level
    SSPSTATbits.SMP = 0;    // 1 - podatak je uzorkovan na kraju izlaznog vremena podataka
                            // 0 - na sredini
    // SPI master mode
    // 0000 = SPI Master mode, clock = FOSC/4
    // 0001 = SPI Master mode, clock = FOSC/16 
    // 0010 = SPI Master mode, clock = FOSC/64 
    SSPCON1bits.SSPM = 0b0000;
}

//funkcija za prijenos podatka na sabirnicu
uint8_t SPI_transfer(uint8_t data)  // 8 - bitni SPI transfer
{
    SSPBUF = data;                  // Put command into SPI buffer
    while (!SSPSTATbits.BF);        // Wait for the transfer to finish - ako je BF (buffer full)
                                    // postavljena u 1, prekida se petlja i vra?a se vrijednost, podatak je procitan s buffera
    return SSPBUF;                  // Save the read value
}