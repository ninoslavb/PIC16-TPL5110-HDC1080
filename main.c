/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

//#if defined(__XC)
//    #include <xc.h>         /* XC8 General Include File */
//#elif defined(HI_TECH_C)
//    #include <htc.h>        /* HiTech General Include File */
//#endif
#include <xc.h> 
//#include <htc.h> 
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <pic16lf1459.h>
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "i2c.h"          /* User funct/params, such as InitApp */
#include "nRF24L01P.h"
#include "spi.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (ECH, External Clock, High Power Mode (4-20 MHz): device clock supplied to CLKIN pins)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF        // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config CPUDIV = NOCLKDIV// CPU System Clock Selection Bit (CPU system clock divided by 6)
#pragma config USBLSCLK = 48MHz // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
#pragma config PLLMULT = 3x     // PLL Multipler Selection Bit (3x Output Frequency Selected)
#pragma config PLLEN = DISABLED  // PLL Enable Bit (3x or 4x PLL Enabled)
#pragma config STVREN = OFF      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)


//#define SLEEP()     asm("sleep")

void main(void)
{
    
    /* Configure the oscillator for the device */
    ConfigureOscillator();                                  // oscillator configuration
    __delay_ms(10);
    InitApp();
    /* ANSELA = 0;                                             // turn off analog functions
    ANSELB = 0;
    ANSELC = 0;
    
    TRISBbits.TRISB5 = 0;                                   // DONE OUTPUT
    TRISCbits.TRISC4 = 0;                                   // indication LED output
    LATCbits.LATC4 = 0;
    TRISAbits.TRISA5 = 0; //IRQ output
  
    */
    TRISBbits.TRISB5 = 0; //DONE output
     //InitApp();
    SPI_init();         //nRF24L01+ setup
    
    uint8_t data[4];    // data array
    data[2]=3; //ID
    data[3]= 13; //terminator
    __delay_ms(5);
        
        //ugasi nRF dok ne izmjeris temperaturu
       
        //WriteRegister(NRF_CONFIG, 0x00);
        
                // set delay for SDA and SCL
       
        
        // read senor
    uint8_t wdata[2] = {0x06, 0x00 }; //temp. measurement, heated disabled, 11 bit resoluton
    //uint8_t wdata[2] = {0x02, 0x00 }; //temp. measurement, heater disabled, 14 bit resolution
    //uint8_t wdata[2] = {0x22, 0x00 }; //temp. measurement, heater enabled, 14 bit resolution
    uint8_t rdata[2] = { 0x00, 0x00};
    
    //while(1) {
    //***********SENSOR***********
        // to prevent current flow to SDA and SCL pins after I2C comm.
        
        TRISBbits.TRISB4 = 1;
        TRISBbits.TRISB6 = 1;      
        //LATBbits.LATB4 = 0; //SDA u 1
        //LATBbits.LATB6 = 0; //SCL u 1  
        __delay_ms(1);     
        IRQ = 0 ;               // daj napajanje senzoru        
        __delay_ms(15);         // device start-up time 
        
        //SSPCON1bits.SSPEN = 0; //disable I2C module  
        SSPCON1bits.SSPEN = 1;
        i2cReset();
        // enable I2C module
        SSPCON1bits.SSPEN = 1;
        i2cSetup();
       
        // u result stavi izlaz iz A/D pretvornika, vrijednost temperature je 16 bitna
        int result = i2cXfer( wdata,2,rdata,2);
      
        data[0] = result & 0x00FF;
        data[1] = (result & 0xFF00)>>8 ;
        
        SSPCON1bits.SSPEN = 0;//disable i2c 
       
        IRQ = 1 ;       // gasi senzor
        
        
        __delay_ms(5);
  
       //********RF MODULE*********
        
        CE = 0;
        SPI_init();
        nRF_Setup(); 
        FlushTXRX();
        WriteRegister(NRF_STATUS,0x70);         // Reset status register
        __delay_ms(2);
        
        CE = 1; //bez ovog
        //__delay_us(150); // bez
       WritePayload(4, data); 
       LATCbits.LATC5 = 1;
       __delay_ms(100);
       LATCbits.LATC5 = 0;
       __delay_ms(50);
       __delay_us(202);  // 130 us + Ttransmit???
       //__delay_ms(5); 
       FlushTXRX();
       
      WriteRegister(NRF_CONFIG, 0x00);        // turn off module
      // __delay_ms(5);                
       

       
        
       LATBbits.LATB5 = 1;         // DONE=1
      
       LATBbits.LATB5 = 0;   // DONE = 0 
   
  
     //  }

  }