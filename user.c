/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

void InitApp(void)
{
      //disable analog functions
   ANSELA = 0x00;       
   ANSELB = 0x00;  
   ANSELC = 0x00;
   
   TRISCbits.TRISC0 = 0; //ICSPDAT output +
   TRISCbits.TRISC1 = 0; //ICSPCLK input   + 
   TRISCbits.TRISC2 = 0; //AN0 output   +
   TRISCbits.TRISC3 = 0; //AN1 output    +
   TRISCbits.TRISC4 = 0; //Green led output +
   TRISCbits.TRISC5 = 0; //Red led output +
   TRISCbits.TRISC6 = 0; //CSN output +
   TRISCbits.TRISC7 = 0; //SDO output  +
   
   TRISAbits.TRISA4 = 0; //CE output +
   TRISAbits.TRISA5 = 0; //IRQ output  +
   
   TRISBbits.TRISB4 = 1; //SDI input
   
   TRISBbits.TRISB6 = 0; //clock output
   TRISBbits.TRISB7 = 1; //IOC input
  
   LATCbits.LATC0 = 0; 
   LATCbits.LATC1 = 0;
   LATCbits.LATC2 = 0;
   LATCbits.LATC3 = 0;
   LATCbits.LATC4 = 0;
   LATCbits.LATC5 = 0;
   LATCbits.LATC6 = 0;
   LATCbits.LATC7 = 0;
   
   LATAbits.LATA4 = 0;
   LATAbits.LATA5 = 0;
   
   //LATBbits.LATB4 =0;
  //LATBbits.LATB5 = 1;
   LATBbits.LATB6 = 0;
  // LATBbits.LATB7 = 0;
    
   
}
/*
void timer_setup_and_start_IE(void)
{
    // Timer1 set-up
    T1CONbits.TMR1CS = 00;                    // 00 - FOSC/4, 01 - FOSC
    T1CONbits.T1CKPS = 11;                    // 11 - 8, 00 - 1         
    TMR1H = 0x00;           
    TMR1L = 0x00; 
    TMR_CNT = 0;
    PIR1bits.TMR1IF = 0;                      // Clear flag            
    T1CONbits.TMR1ON = 1;                     // Timer1 ON
    PIE1bits.TMR1IE = 1;                      // Enable interrupt
    
    // Timer0 set-up
    //TMR0 = 0;                                 // 
    //OPTION_REGbits.TMR0CS = 0;                // 0 - Internal instruction cycle clock (FOSC/4)
    //OPTION_REGbits.PSA = 0;                   // 0 - Prescaler is assigned to the Timer0 module
    //OPTION_REGbits.PS = 0b111;                // 111 - 256 prescaler rate    
    //INTCONbits.T0IF = 0;                      // clear flag   
    //INTCONbits.T0IE = 1;                      // Enable the interrupt           

    // Timer2 set-up
    //PIR1bits.TMR2IF = 0;                      // clear flag   
    //T2CONbits.T2CKPS = 11;                    // 11 = Prescaler is 64, 00 = Prescaler is 1
    //T2CONbits.TMR2ON = 1;                     // Timer2 ON       
    //PIE1bits.TMR2IE = 1;                      // Enable the interrupt         
}
*/